#pragma once
#include <string>
#include "include/lua.hpp"
#include "../imgui-dx9/imgui_internal.h"

struct ModuleInfo
{
	std::string Name = "";
	std::string Version = "";
	std::string Author = "";
	std::string Description = "";
};

extern lua_CFunction PrintFunc;

extern "C" 
{
	__declspec(dllimport) lua_State* GetLuaState(const char* StateName);
	__declspec(dllimport) bool LuaInited(const char* StateName);
	__declspec(dllimport) void SetLuaPrintFunction(lua_CFunction PrintFunction);
	__declspec(dllimport) void SetLuaPrintFunctionForState(const char* StateName, lua_CFunction PrintFunction);
	__declspec(dllexport) void ExportImGuiForState(const char* StateName);
	__declspec(dllimport) void InitLua(const char* StateName);
	__declspec(dllimport) void RunMain(const char* StateName, char* ErrorStrBuffer, size_t ErrorStrSize);
	__declspec(dllimport) void RunRender(const char* StateName, char* ErrorStrBuffer, size_t ErrorStrSize);
	__declspec(dllimport) void RunRenderInterface(const char* StateName, char* ErrorStrBuffer, size_t ErrorStrSize);
	__declspec(dllimport) void RunRenderMenuItem(const char* StateName, char* ErrorStrBuffer, size_t ErrorStrSize);
	__declspec(dllimport) void RunRenderMainMenuItem(const char* StateName, char* ErrorStrBuffer, size_t ErrorStrSize);
	__declspec(dllimport) void RunRenderSettings(const char* StateName, char* ErrorStrBuffer, size_t ErrorStrSize);
	__declspec(dllimport) void RunLua(const char* StateName, const char* LuaScript, char* ErrorStrBuffer, size_t ErrorStrSize);
	__declspec(dllimport) void RunLuaFile(const char* StateName, const char* FileName, char* ErrorStrBuffer, size_t ErrorStrSize);
	__declspec(dllimport) void UpdatePrintFunction(const char* StateName);
	__declspec(dllimport) void CloseLua(const char* StateName);
	__declspec(dllimport) void CloseLuaAll();
}

__declspec(dllimport) ModuleInfo* RunModuleInfo(const char* StateName, char* ErrorStrBuffer, size_t ErrorStrSize);
__declspec(dllimport) void SetImGuiContext(ImGuiContext* Ctx);