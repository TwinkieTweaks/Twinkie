#pragma once

#include "../IModule.h"

class GhostEditorModule : public IModule
{
public:
	bool IsDebug = false;

	GhostEditorModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "GhostEditor";
		this->FancyName = "Ghost editor";
	}

	virtual void Render() {}
	virtual void RenderAnyways() {}
	virtual void RenderSettings() {}

	virtual void RenderMenuItem()
	{
		using namespace ImGui;
		if (MenuItem(FancyName.c_str()) and !Twinkie->GetChallenge() and Twinkie->GetProfileScores())
		{
			Twinkie->CallMenuGhostEditor();
		}
	}

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}

	virtual bool HasSettings() { return false; }
};