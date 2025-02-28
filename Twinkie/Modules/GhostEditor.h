#pragma once

#include "../IModule.h"

class GhostEditorModule : public IModule
{
public:
	bool HasSettings = false;
	bool IsDebug = false;
	std::string Name = "GhostEditor";
	std::string FancyName = "Ghost editor";

	virtual void Render(TwinkTrackmania& Twinkie) {}
	virtual void RenderAnyways(TwinkTrackmania& Twinkie) {}
	virtual void RenderSettings() {}

	virtual void RenderMenuItem(TwinkTrackmania& Twinkie)
	{
		using namespace ImGui;
		if (MenuItem(FancyName.c_str()))
		{
			Twinkie.CallMenuGhostEditor();
		}
#ifdef BUILD_DEBUG
		Separator(); // because of the fact that PlayerInfo follows this item
#endif
	}

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}
};