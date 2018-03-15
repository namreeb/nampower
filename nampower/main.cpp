/*
    Copyright (c) 2017-2018, namreeb (legal@namreeb.org)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    The views and conclusions contained in the software and documentation are those
    of the authors and should not be interpreted as representing official policies,
    either expressed or implied, of the FreeBSD Project.
*/

#include "offsets.hpp"
#include "game.hpp"

#include <hadesmem/process.hpp>
#include <hadesmem/patcher.hpp>

#include <Windows.h>

#include <cstdint>
#include <memory>

#ifdef _DEBUG
#include <sstream>
#endif

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID);

static DWORD gCooldown;

#ifdef _DEBUG
static DWORD gLastCast;
#endif

namespace
{
using CastSpellT = bool(__fastcall *)(void *, int, void *, std::uint64_t);

bool CastSpellHook(hadesmem::PatchDetourBase *detour, void *unit, int spellId, void *item, std::uint64_t guid)
{
    auto const currentTime = ::GetTickCount();

    // is there a cooldown?
    if (gCooldown)
    {
        // is it still active?
        if (gCooldown > currentTime)
            return false;

        gCooldown = 0;
    }

    auto const castSpell = detour->GetTrampolineT<CastSpellT>();
    auto ret = castSpell(unit, spellId, item, guid);

    // haven't gotten spell result yet, but we should have?  local cancel...
    if (!ret)
    {
        int(__fastcall *cancelCast)(bool, bool, int) = reinterpret_cast<decltype(cancelCast)>(Offsets::CancelCast);

        cancelCast(false, false, 28);

        // try again...
        ret = castSpell(unit, spellId, item, guid);
    }

    if (ret)
    {
        constexpr std::uint32_t SPELLCAST_START = 337;
        void(*signalEvent)(std::uint32_t, const char *, ...) = reinterpret_cast<decltype(signalEvent)>(Offsets::SignalEvent);

        auto const castTime = game::GetCastTime(unit, spellId);
        auto const spell = game::GetSpellInfo(spellId);

        if (!!spell && castTime > 0 && !(spell->Attributes & game::SPELL_ATTR_RANGED))
        {
            gCooldown = currentTime + castTime;

#ifdef _DEBUG
            std::stringstream str;
            str << "Casting " << game::GetSpellName(spellId) << " with cast time " << castTime << " at time " << currentTime;

            if (gLastCast)
                str << " elapsed: " << (currentTime - gLastCast);

            str << std::endl;

            ::OutputDebugStringA(str.str().c_str());

            gLastCast = currentTime;
#endif

            signalEvent(SPELLCAST_START, "%s%d", game::GetSpellName(spellId), castTime);
        }
    }

    return ret;
}
}

std::unique_ptr<hadesmem::PatchDetour<CastSpellT>> gCastDetour;
std::unique_ptr<hadesmem::PatchRaw> gCastbarPatch;
std::unique_ptr<hadesmem::PatchRaw> gCastbarCancelPatch;

extern "C" __declspec(dllexport) DWORD Load()
{
    gCooldown = 0;

#ifdef _DEBUG
    gLastCast = 0;
#endif

    const hadesmem::Process process(::GetCurrentProcessId());

    // activate spellbar and our own internal cooldown on a successful cast attempt (result from server not available yet)
    auto const castSpellOrig = hadesmem::detail::AliasCast<CastSpellT>(Offsets::CastSpell);
    gCastDetour = std::make_unique<hadesmem::PatchDetour<CastSpellT>>(process, castSpellOrig, &CastSpellHook);
    gCastDetour->Apply();

    // prevent spellbar re-activation upon successful cast notification from server
    const std::vector<std::uint8_t> patch(5, 0x90);
    gCastbarPatch = std::make_unique<hadesmem::PatchRaw>(process, reinterpret_cast<void *>(Offsets::CreateCastbar), patch);
    gCastbarPatch->Apply();

    // prevent spell result from the last spell cancelling the cast bar for the next one
    gCastbarCancelPatch = std::make_unique<hadesmem::PatchRaw>(process, reinterpret_cast<void *>(Offsets::CancelCastbar), patch);
    gCastbarCancelPatch->Apply();

    return EXIT_SUCCESS;
}
