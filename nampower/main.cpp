#include <vector>
#include <Windows.h>

#include <boost/filesystem.hpp>

#include <hadesmem/process.hpp>
#include <hadesmem/patcher.hpp>

#include "misc.hpp"

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID);

hadesmem::PatchDetour *gRegisterHook;

// here we hook the wow script load function so we know when to register
// our function and initialize the lua ToNumber pointer for later.
// we also check to see if we are already in-game (in the case of injection
// into a running process).  if we are, register the lua function immediately.
extern "C" HADESMEM_DETAIL_DLLEXPORT DWORD Load()
{
    const hadesmem::Process process(::GetCurrentProcessId());
    
    gRegisterHook = new hadesmem::PatchDetour(process, (FARPROC)0x490250, &LuaLoadScripts);
    gRegisterHook->Apply();

    union
    {
        DWORD func;
        double(__fastcall *ToNumber)(PVOID, unsigned int);
    } f;

    f.func = 0x6F3620;
    LuaToNumber = f.ToNumber;

    // are we in a game?

    unsigned __int64(__stdcall *GetPlayerGuid)() = (decltype(GetPlayerGuid))(0x468550);

    if ((*GetPlayerGuid)())
        RegisterLuaFunction();

    return EXIT_SUCCESS;
}
