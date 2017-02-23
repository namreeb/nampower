/*
    Copyright (c) 2017, namreeb (legal@namreeb.org)
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
    Write(&val, sizeof(T));
}

struct ClientConnection
{
    int SendPacket(const CDataStore *);
};