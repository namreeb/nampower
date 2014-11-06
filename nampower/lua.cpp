#include <memory>

#include <hadesmem/patcher.hpp>

#include "misc.hpp"

double(__fastcall *LuaToNumber)(PVOID, unsigned int);

void RegisterLuaFunction()
{
    const DWORD trampolineAddress = 0x40100A;
    const hadesmem::Process process(::GetCurrentProcessId());

    std::vector<BYTE> patch(5);

    patch[0] = 0xE9;    // JMP

    union
    {
        DWORD castSpellLocation;
        int(*Func)(void *);
    } f;

    f.Func = &CastSpellAtTarget;

    const DWORD relativeJumpValue = f.castSpellLocation - trampolineAddress - 5;

    memcpy(&patch[1], &relativeJumpValue, sizeof(DWORD));

    // write JMP to wow's .text section so it can be registered with lua
    auto trampoline = new hadesmem::PatchRaw(process, (PVOID)trampolineAddress, patch);
    trampoline->Apply();

    // register with lua
    union
    {
        DWORD frameScriptRegisterLocation;
        void(__fastcall *FrameScriptRegister)(const char *, DWORD);
    } g;

    g.frameScriptRegisterLocation = 0x704120;
    (*g.FrameScriptRegister)("CastSpellAtTarget", trampolineAddress);
}

void __cdecl LuaLoadScripts()
{
    void(__cdecl *OriginalRegister)();

    OriginalRegister = gRegisterHook->GetTrampoline<decltype(OriginalRegister)>();

    (*OriginalRegister)();

    RegisterLuaFunction();
}