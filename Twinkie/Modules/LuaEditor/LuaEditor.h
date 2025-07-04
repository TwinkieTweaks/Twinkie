#pragma once
#include <filesystem>
#include "../../IModule.h"
#include "../../LuaEngine/LuaEngine2.h"

namespace Filesystem = std::filesystem;

extern inline std::string AlsoGetDocumentsFolder();

class LuaEditorModule : public IModule
{
	bool LuaFileInitialized = false;
	bool LuaFileReload = false;
	bool LuaErrorOccured = false;
public:
	LuaEditorModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		InitLua();
		Filesystem::create_directories(Filesystem::path(AlsoGetDocumentsFolder() + "\\Twinkie\\LuaScripts"));
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
	}

	~LuaEditorModule()
	{
		CloseLua();
	}

	LuaEditorModule() = default;

	virtual void Render();
	virtual void RenderAnyways() {}
	virtual void RenderInactive() {}
	virtual void RenderSettings() {}
	virtual void RenderMenuItem() 
	{
		using namespace ImGui;
		if (MenuItem(ICON_FK_CODE " Lua Editor", "", Enabled))
		{
			Enabled = !Enabled;
		}
	}

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}

	virtual bool IsDebug() { return false; }
	virtual bool HasSettings() { return false; }
};