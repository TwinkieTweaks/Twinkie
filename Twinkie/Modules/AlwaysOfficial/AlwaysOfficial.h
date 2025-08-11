#pragma once

#include "../../IModule.h"

class AlwaysOfficialModule : public IModule
{
public:
	int CurrentRacetime = 0;
	int PreviousRacetime = 0;

	AlwaysOfficialModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;

		this->Name = "AlwaysOfficial";
		this->FancyName = "Always Official";
	}

	AlwaysOfficialModule() = default;

	virtual void Render() {};
	virtual void RenderAnyways();
	virtual void RenderInactive() {};
	virtual void RenderSettings() {};
	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings) {};
	virtual void SettingsSave(SettingMgr& Settings) {};

	virtual bool IsDebug() { return false; };
	virtual bool HasSettings() { return false; };
};