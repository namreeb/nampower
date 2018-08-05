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

namespace
{
static DWORD gCooldown;

#ifdef _DEBUG
static DWORD gLastCast;
#endif

static bool gCancelling;
static bool gCancelFromClient;

using CastSpellT = bool(__fastcall *)(void *, int, void *, std::uint64_t);
using CancelSpellT = int(__fastcall *)(bool, bool, int);
using SignalEventT = void(__fastcall *)(game::Events);
using PacketHandlerT = int(__stdcall *)(int, game::CDataStore *);

std::unique_ptr<hadesmem::PatchDetour<CastSpellT>> gCastDetour;
std::unique_ptr<hadesmem::PatchDetour<CancelSpellT>> gCancelSpellDetour;
std::unique_ptr<hadesmem::PatchDetour<SignalEventT>> gSignalEventDetour;
std::unique_ptr<hadesmem::PatchDetour<PacketHandlerT>> gSpellDelayedDetour;
std::unique_ptr<hadesmem::PatchRaw> gCastbarPatch;

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

    auto const spell = game::GetSpellInfo(spellId);

    auto const castSpell = detour->GetTrampolineT<CastSpellT>();
    auto ret = castSpell(unit, spellId, item, guid);

    // if this is a trade skill, do nothing further
    if (spell->Effect[0] == game::SpellEffects::SPELL_EFFECT_TRADE_SKILL)
        return ret;

    // haven't gotten spell result yet, probably due to latency.  simulate a cancel to clear the cast bar
    if (!ret)
    {
        auto const cancelSpell = reinterpret_cast<CancelSpellT>(Offsets::CancelSpell);

        cancelSpell(false, false, 28);

        // try again...
        ret = castSpell(unit, spellId, item, guid);
    }

    if (ret)
    {
        auto const castTime = game::GetCastTime(unit, spellId);

        if (!!spell && castTime > 0 && !(spell->Attributes & game::SPELL_ATTR_RANGED))
        {
            gCooldown = currentTime + castTime;

#ifdef _DEBUG
            // don't bother building the string if nobody will see it
            if (::IsDebuggerPresent())
            {
                std::stringstream str;
                str << "Casting " << game::GetSpellName(spellId) << " with cast time " << castTime << " at time " << currentTime;

                if (gLastCast)
                    str << " elapsed: " << (currentTime - gLastCast);

                str << std::endl;

                ::OutputDebugStringA(str.str().c_str());
            }

            gLastCast = currentTime;
#endif
            
            void(*signalEvent)(std::uint32_t, const char *, ...) = reinterpret_cast<decltype(signalEvent)>(Offsets::SignalEventParam);
            signalEvent(game::Events::SPELLCAST_START, "%s%d", game::GetSpellName(spellId), castTime);
        }
    }

    return ret;
}

int CancelSpellHook(hadesmem::PatchDetourBase *detour, bool failed, bool notifyServer, int reason)
{
    gCancelling = true;
    gCancelFromClient = notifyServer;

    auto const cancelSpell = detour->GetTrampolineT<CancelSpellT>();
    auto const ret = cancelSpell(failed, notifyServer, reason);

    gCancelling = false;

    return ret;
}

void SignalEventHook(hadesmem::PatchDetourBase *detour, game::Events eventId)
{
    auto const currentTime = ::GetTickCount();

    // if we are not in the process of cancelling a spell at all then no intervention is necessary on our part
    if (gCancelling)
    {
#ifdef _DEBUG
        if (::IsDebuggerPresent())
        {
            if (eventId == game::Events::SPELLCAST_STOP ||
                eventId == game::Events::SPELLCAST_FAILED)
            {
                std::stringstream str;

                str << "Event " << (eventId == game::Events::SPELLCAST_STOP ? "SPELLCAST_STOP" : "SPELLCAST_FAILED")
                    << " at time " << currentTime << " gLastCast = " << gLastCast << " gCooldown = " << gCooldown << std::endl;

                ::OutputDebugStringA(str.str().c_str());
            }
        }
#endif

        // prevent the result of a previous cast from stopping the current castbar
        if (!gCancelFromClient && (eventId == game::Events::SPELLCAST_STOP || eventId == game::Events::SPELLCAST_FAILED) && currentTime < gCooldown)
            return;
    }

    auto const signalEvent = detour->GetTrampolineT<SignalEventT>();
    signalEvent(eventId);
}

int SpellDelayedHook(hadesmem::PatchDetourBase *detour, int opCode, game::CDataStore *packet)
{
    auto const spellDelayed = detour->GetTrampolineT<PacketHandlerT>();

    auto const rpos = packet->m_read;

    auto const guid = packet->Get<std::uint64_t>();
    auto const delay = packet->Get<std::uint32_t>();

    packet->m_read = rpos;

    auto const activePlayer = game::ClntObjMgrGetActivePlayer();

    if (guid == activePlayer)
    {
        auto const currentTime = ::GetTickCount();

        // if we are casting a spell and it was delayed, update our own state so we do not allow a cast too soon
        if (currentTime < gCooldown)
        {
            gCooldown += delay;
            gLastCast += delay;
        }
    }

    return spellDelayed(opCode, packet);
}
}

extern "C" __declspec(dllexport) DWORD Load()
{
    gCooldown = 0;

#ifdef _DEBUG
    gLastCast = 0;
#endif

    gCancelling = false;
    gCancelFromClient = false;

    const hadesmem::Process process(::GetCurrentProcessId());

    // activate spellbar and our own internal cooldown on a successful cast attempt (result from server not available yet)
    auto const castSpellOrig = hadesmem::detail::AliasCast<CastSpellT>(Offsets::CastSpell);
    gCastDetour = std::make_unique<hadesmem::PatchDetour<CastSpellT>>(process, castSpellOrig, &CastSpellHook);
    gCastDetour->Apply();

    // monitor for client-based spell interruptions to stop the castbar
    auto const cancelSpellOrig = hadesmem::detail::AliasCast<CancelSpellT>(Offsets::CancelSpell);
    gCancelSpellDetour = std::make_unique<hadesmem::PatchDetour<CancelSpellT>>(process, cancelSpellOrig, &CancelSpellHook);
    gCancelSpellDetour->Apply();

    // this hook will alter cast bar behavior based on events from the game
    auto const signalEventOrig = hadesmem::detail::AliasCast<SignalEventT>(Offsets::SignalEvent);
    gSignalEventDetour = std::make_unique<hadesmem::PatchDetour<SignalEventT>>(process, signalEventOrig, &SignalEventHook);
    gSignalEventDetour->Apply();

    // watch for pushback notifications from the server
    auto const spellDelayedOrig = hadesmem::detail::AliasCast<PacketHandlerT>(Offsets::SpellDelayed);
    gSpellDelayedDetour = std::make_unique<hadesmem::PatchDetour<PacketHandlerT>>(process, spellDelayedOrig, &SpellDelayedHook);
    gSpellDelayedDetour->Apply();

    // prevent spellbar re-activation upon successful cast notification from server
    const std::vector<std::uint8_t> patch(5, 0x90);
    gCastbarPatch = std::make_unique<hadesmem::PatchRaw>(process, reinterpret_cast<void *>(Offsets::CreateCastbar), patch);
    gCastbarPatch->Apply();

    return EXIT_SUCCESS;
}
