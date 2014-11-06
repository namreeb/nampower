#include <Windows.h>
#include <sstream>
#include <vector>

#include "misc.hpp"
#include "CDataStore.hpp"

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
    const unsigned __int64 *targetGuid = (const unsigned __int64 *)0xB4E2D8;

    if (*targetGuid)
    {

        auto param = (DWORD)LuaToNumber(luaState, 1);

        std::vector<BYTE> packedGuid;
        BuildPackedGuid(*targetGuid, packedGuid);

        CDataStore packet(10 + packedGuid.size());

        packet.Write((DWORD)0x12E);                         // CMSG_CAST_SPELL
        packet.Write((DWORD)param);                         // spell id
        packet.Write((WORD)0x02);                           // TARGET_FLAG_UNIT
        packet.Write(&packedGuid[0], packedGuid.size());    // packed target guid

        packet.Send();
    }

    return 1;
}