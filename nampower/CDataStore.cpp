#include <Windows.h>

#include "CDataStore.hpp"

void CDataStore::Write(const PVOID data, unsigned int length)
{
    assert(m_bytesWritten + length <= m_capacity);

    memcpy((char *)m_data + m_bytesWritten, data, length);
    m_bytesWritten += length;
}

void CDataStore::Send() const
{
    union
    {
        DWORD func;
        int(__thiscall CDataStore::*SendPacket)(const CDataStore *);
    } f;

    f.func = 0x5379A0;
    // not actually a CDataStore instance, but who cares? YOLO!
    CDataStore *clientConnection = *(CDataStore **)(0xC28128);

    (clientConnection->*f.SendPacket)(this);
}