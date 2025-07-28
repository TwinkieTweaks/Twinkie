#pragma once

#ifdef BUILD_DEBUG
#include <filesystem>
#include "../../IModule.h"
#include "../../LuaEngine/LuaEngine2.h"

namespace Filesystem = std::filesystem;

extern "C" int LuaConsolePrintOverride(lua_State* L);
extern std::string g_LuaConsoleModuleOutputStr;

class LuaConsoleModule : public IModule
{
public:
	LuaConsoleModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
	}

	~LuaConsoleModule()
	{
		CloseLua("TwinkieLuaConsole");
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
#endif