#pragma once

#include <Windows.h>
#include <cassert>

class CDataStore
{
    private:
        const PVOID _vmt;                       // 0x00-0x04

    public:
        const PVOID m_data;                     // 0x04-0x08
        const unsigned int m_base;              // 0x08-0x0C
        const unsigned int m_capacity;          // 0x0C-0x10
        unsigned int m_bytesWritten;            // 0x10-0x14
        unsigned int m_bytesRead;               // 0x14-0x18

        CDataStore(unsigned int size) : _vmt(nullptr), m_data(malloc(size)), m_base(0), m_capacity(size), m_bytesWritten(0), m_bytesRead(0) {}

        ~CDataStore()
        {
            free(m_data);
        }

        template <typename T> void Write(T);
        void Write(const PVOID, unsigned int);

        void Send() const;
};

template <typename T>
void CDataStore::Write(T val)
{
    assert((m_bytesWritten + sizeof(val)) <= m_capacity);

    memcpy((char *)m_data + m_bytesWritten, &val, sizeof(val));
    m_bytesWritten += sizeof(val);
}