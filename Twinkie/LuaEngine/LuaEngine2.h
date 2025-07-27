#pragma once
#ifdef BUILD_DEBUG
#include "include/lua.hpp"

extern lua_CFunction PrintFunc;

extern "C" 
{
	__declspec(dllimport) lua_State* GetLuaState(const char* StateName);
	__declspec(dllimport) bool LuaInited(const char* StateName);
	__declspec(dllimport) void SetLuaPrintFunction(lua_CFunction PrintFunction);
	__declspec(dllimport) void InitLua(const char* StateName);
	__declspec(dllimport) void RunRender(const char* StateName, char* ErrorStrBuffer, size_t ErrorStrSize);
	__declspec(dllimport) void RunLua(const char* StateName, const char* LuaScript, char* ErrorStrBuffer, size_t ErrorStrSize);
	__declspec(dllimport) void RunLuaFile(const char* StateName, const char* FileName, char* ErrorStrBuffer, size_t ErrorStrSize);
	__declspec(dllimport) void CloseLua(const char* StateName);
	__declspec(dllimport) void CloseLuaAll();
}
#endif