#pragma once

#include "../../IModule.h"

class DownloadServerMapsModule : public IModule
{
public:
	uintptr_t PreviousChallenge = 0;
	uintptr_t CurrentChallenge = 0;

	DownloadServerMapsModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;

		Name = "DownloadServerMaps";
		FancyName = "Map Downloader";
	}

	DownloadServerMapsModule() = default;

	virtual void Render() {};
	virtual void RenderAnyways() {};
	virtual void RenderInactive();
	virtual void RenderSettings();
	virtual void RenderMenuItem() {};

	virtual void SettingsInit(SettingMgr& Settings);
	virtual void SettingsSave(SettingMgr& Settings);

	virtual bool IsDebug() { return false; };
	virtual bool HasSettings() { return true; };
};