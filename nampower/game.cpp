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

#include "game.hpp"
#include "offsets.hpp"

#include <hadesmem/detail/alias_cast.hpp>

#include <cstdint>

namespace game
{
void *GetObjectPtr(std::uint64_t guid)
{
    void *(__stdcall *getObjectPtr)(std::uint64_t) = hadesmem::detail::AliasCast<decltype(getObjectPtr)>(Offsets::GetObjectPtr);

    return getObjectPtr(guid);
}

std::uint32_t GetCastTime(void *unit, int spellId)
{
    auto const vmt = *reinterpret_cast<std::uint8_t **>(unit);
    int(__thiscall *getSpellCastingTime)(void *, int) = *reinterpret_cast<decltype(&getSpellCastingTime)>(vmt + 4 * static_cast<std::uint32_t>(Offsets::GetCastingTimeIndex));

    return getSpellCastingTime(unit, spellId);
}

const SpellRec *GetSpellInfo(int spellId)
{
    auto const spellDb = reinterpret_cast<WowClientDB<SpellRec> *>(Offsets::SpellDb);

    if (spellId < 0 || spellId > spellDb->m_maxID)
        return nullptr;

    return spellDb->m_recordsById[spellId];
}

const char *GetSpellName(int spellId)
{
    auto const spell = GetSpellInfo(spellId);

    if (!spell || spell->AttributesEx3 & SPELL_ATTR_EX3_HIDE_NAME)
        return "";

    auto const language = *reinterpret_cast<std::uint32_t *>(Offsets::Language);

    return spell->SpellName[language];
}

std::uint64_t ClntObjMgrGetActivePlayer()
{
    auto const getActivePlayer = hadesmem::detail::AliasCast<decltype(&ClntObjMgrGetActivePlayer)>(Offsets::GetActivePlayer);

    return getActivePlayer();
}
}