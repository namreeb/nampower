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

#include <vector>
#include <Windows.h>

#include <boost/filesystem.hpp>

#include <hadesmem/process.hpp>
#include <hadesmem/patcher.hpp>

#include "misc.hpp"
#include "offsets.hpp"

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID);

// here we hook the wow script load function so we know when to register
// our function and initialize the lua ToNumber pointer for later.
// we also check to see if we are already in-game (in the case of injection
// into a running process).  if we are, register the lua function immediately.
extern "C" __declspec(dllexport) DWORD Load()
{
    const hadesmem::Process process(::GetCurrentProcessId());
    
    auto const luaLoadScriptsOrig = hadesmem::detail::AliasCast<LuaLoadScriptsT>(Offsets::FrameScript__LoadWorldScripts);

    auto registerHook = new hadesmem::PatchDetour<LuaLoadScriptsT>(process, luaLoadScriptsOrig, &LuaLoadScripts);
    registerHook->Apply();

    LuaToNumber = hadesmem::detail::AliasCast<decltype(LuaToNumber)>(Offsets::Lua__ToNumber);

    // are we in a game?
    unsigned __int64(__stdcall *getPlayerGuid)() = (decltype(getPlayerGuid))(Offsets::GetPlayerGuid);

    if ((*getPlayerGuid)())
        RegisterLuaFunctions();

    return EXIT_SUCCESS;
}

// note: unloading of the dll is not currently supported