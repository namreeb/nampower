#pragma once

#include <memory>

#include <hadesmem/patcher.hpp>

extern hadesmem::PatchDetour *gRegisterHook;

extern double(__fastcall *LuaToNumber)(PVOID, unsigned int);
extern void RegisterLuaFunction();
extern void __cdecl LuaLoadScripts();

extern int CastSpellAtTarget(void *luaState);
