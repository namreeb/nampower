/*
    Copyright (c) 2015, namreeb (legal@namreeb.org)
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

#include <Windows.h>
#include <sstream>
#include <vector>

#include "misc.hpp"
#include "CDataStore.hpp"
#include "offsets.hpp"

void BuildPackedGuid(unsigned __int64 guid, std::vector<BYTE> &result)
{
    result.clear();
    result.reserve(sizeof(unsigned __int64));

    if (!guid)
        return;

    result.push_back(0);

    for (int i = 0; guid != 0; ++i)
    {
        if ((guid & 0xFF) != 0)
        {
            result[0] |= (BYTE)(1 << i);
            result.push_back((BYTE)(guid & 0xFF));
        }

        guid >>= 8;
    }
}

int CastSpellAtTarget(void *luaState)
{
    if (const unsigned __int64 targetGuid = *(const unsigned __int64 *)Offsets::gTargetGuid)
    {
        auto param = (DWORD)LuaToNumber(luaState, 1);

        std::vector<BYTE> packedGuid;
        BuildPackedGuid(targetGuid, packedGuid);

        CDataStore packet(10 + packedGuid.size());

        packet.Write((DWORD)0x12E);                         // CMSG_CAST_SPELL
        packet.Write((DWORD)param);                         // spell id
        packet.Write((WORD)0x02);                           // TARGET_FLAG_UNIT
        packet.Write(&packedGuid[0], packedGuid.size());    // packed target guid

        packet.Send();
    }

    return 1;
}

int CastSpellAtMouseover(void *luaState)
{
    if (const unsigned __int64 mouseoverGuid = *(const unsigned __int64 *)Offsets::gMouseoverGuid)
    {
        auto param = (DWORD)LuaToNumber(luaState, 1);

        std::vector<BYTE> packedGuid;
        BuildPackedGuid(mouseoverGuid, packedGuid);

        CDataStore packet(10 + packedGuid.size());

        packet.Write((DWORD)0x12E);                         // CMSG_CAST_SPELL
        packet.Write((DWORD)param);                         // spell id
        packet.Write((WORD)0x02);                           // TARGET_FLAG_UNIT
        packet.Write(&packedGuid[0], packedGuid.size());    // packed target guid

        packet.Send();
    }
    //if mouseover guid is empty, use target instead
    else
        CastSpellAtTarget(luaState);

    return 1;
}