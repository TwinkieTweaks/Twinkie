#pragma once

#include "../IModule.h"

class GhostEditorModule : public IModule
{
public:
	bool Enabled = false;
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
	}

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}
};