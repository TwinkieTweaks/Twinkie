#pragma once

#include "../../IModule.h"
#include "../../LuaEngine/LuaEngine2.h"

#define StringBufferMaxSize 512

extern "C" int LuaConsolePrintOverride(lua_State* L);
extern std::string g_LuaConsoleModuleOutputStr;
extern std::vector<std::string> g_LuaConsoleModulePreviousStatements;

class LuaConsoleModule : public IModule
{
	char* LuaStringBuffer = nullptr;
	char* ErrorBuffer = nullptr;
	bool AreBuffersZeroed = false;

	bool PreviousFrameWantsTextInputFocus = false;
	int PreviouslyWrittenStatementIndex = -1;
	bool WasPreviouslyWrittenStatementCopied = true;

	unsigned int InputTextID = 0;
public:
	LuaConsoleModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;

		this->LuaStringBuffer = new char[StringBufferMaxSize];
		this->ErrorBuffer = new char[StringBufferMaxSize];
	}

	~LuaConsoleModule()
	{
		delete[] this->LuaStringBuffer;
		delete[] this->ErrorBuffer;
	}

	LuaConsoleModule() = default;

	virtual void Render();
	virtual void RenderAnyways() {}
	virtual void RenderInactive() {}
	virtual void RenderSettings() {}
	virtual void RenderMenuItem() 
	{
		using namespace ImGui;
		if (MenuItem(ICON_FK_CODE " Lua Console", "", Enabled))
		{
			Enabled = !Enabled;
		}
	}

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}

	virtual bool IsDebug() { return false; }
	virtual bool HasSettings() { return false; }
};