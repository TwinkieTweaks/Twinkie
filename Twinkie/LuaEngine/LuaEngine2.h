#pragma once
#ifdef BUILD_DEBUG
#include "include/lua.hpp"

extern lua_CFunction PrintFunc;

extern "C" {
    __declspec(dllimport) void RunLua(const char* LuaScript, char* ErrorStrBuffer, size_t ErrorStrSize);
    __declspec(dllimport) void RunLuaFile(const char* FileName, char* ErrorStrBuffer, size_t ErrorStrSize);
    __declspec(dllimport) void SetLuaPrintFunction(lua_CFunction PrintFunction);
    __declspec(dllimport) bool LuaInited();
    __declspec(dllimport) void RunRender(char* ErrorStrBuffer, size_t ErrorStrSize);
    __declspec(dllimport) void CloseLua();
    __declspec(dllimport) void InitLua();
    __declspec(dllimport) lua_State* GetLuaState();
}
#endif