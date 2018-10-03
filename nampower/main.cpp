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
static game::SpellFailedReason gCancelReason;

using CastSpellT = bool(__fastcall *)(void *, int, void *, std::uint64_t);
using SendCastT = void(__fastcall *)(game::SpellCast *);
using CancelSpellT = int(__fastcall *)(bool, bool, game::SpellFailedReason);
using SignalEventT = void(__fastcall *)(game::Events);
using PacketHandlerT = int(__stdcall *)(int, game::CDataStore *);

std::unique_ptr<hadesmem::PatchDetour<CastSpellT>> gCastDetour;
std::unique_ptr<hadesmem::PatchDetour<SendCastT>> gSendCastDetour;
std::unique_ptr<hadesmem::PatchDetour<CancelSpellT>> gCancelSpellDetour;
std::unique_ptr<hadesmem::PatchDetour<SignalEventT>> gSignalEventDetour;
std::unique_ptr<hadesmem::PatchDetour<PacketHandlerT>> gSpellDelayedDetour;
std::unique_ptr<hadesmem::PatchRaw> gCastbarPatch;

void BeginCast(DWORD currentTime, std::uint32_t castTime, int spellId)
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

    // haven't gotten spell result yet, probably due to latency.  simulate a cancel to clear the cast bar
    if (!ret)
    {
        auto const cancelSpell = reinterpret_cast<CancelSpellT>(Offsets::CancelSpell);

        cancelSpell(false, false, game::SpellFailedReason::SPELL_FAILED_ERROR);

        // try again...
        ret = castSpell(unit, spellId, item, guid);
    }

    auto const cursorMode = *reinterpret_cast<int *>(Offsets::CursorMode);

    if (ret)
    {
        auto const castTime = game::GetCastTime(unit, spellId);

        if (!!spell && castTime > 0 && !(spell->Attributes & game::SPELL_ATTR_RANGED) && cursorMode != 2)
            BeginCast(currentTime, castTime, spellId);
    }

    return ret;
}

int CancelSpellHook(hadesmem::PatchDetourBase *detour, bool failed, bool notifyServer, game::SpellFailedReason reason)
{
    gCancelling = true;
    gCancelFromClient = notifyServer;
    gCancelReason = reason;

    auto const cancelSpell = detour->GetTrampolineT<CancelSpellT>();
    auto const ret = cancelSpell(failed, notifyServer, reason);

    gCancelling = false;

    return ret;
}

void SendCastHook(hadesmem::PatchDetourBase *detour, game::SpellCast *cast)
{
    auto const cursorMode = *reinterpret_cast<int *>(Offsets::CursorMode);

    // if we were waiting for a target, it means there is no cast bar yet.  make one \o/
    if (cursorMode == 2)
    {
        auto const unit = game::GetObjectPtr(cast->caster);
        auto const castTime = game::GetCastTime(unit, cast->spellId);

        BeginCast(::GetTickCount(), castTime, cast->spellId);
    }

    auto const sendCast = detour->GetTrampolineT<SendCastT>();
    sendCast(cast);
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

        if (eventId == game::Events::SPELLCAST_STOP || eventId == game::Events::SPELLCAST_FAILED)
        {
            // if the current cast is cancelled (from the client for any reason or immediately by the server), reset our own
            // cooldown to allow another one.  this can come from the server for an instant cast (i.e. Presence of Mind)
            if (gCancelFromClient || gCancelReason == game::SpellFailedReason::SPELL_FAILED_ERROR)
                gCooldown = 0;
            // prevent the result of a previous cast from stopping the current castbar
            else if (currentTime < gCooldown)
                return;
        }
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
#ifdef _DEBUG
            gLastCast += delay;
#endif
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

    // monitor for spell cast triggered after target (terrain, item, etc.) is selected
    auto const sendCastOrig = hadesmem::detail::AliasCast<SendCastT>(Offsets::SendCast);
    gSendCastDetour = std::make_unique<hadesmem::PatchDetour<SendCastT>>(process, sendCastOrig, &SendCastHook);
    gSendCastDetour->Apply();

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
