#pragma once

#include "../../IModule.h"
#include "../../Version.h"

class AboutModule : public IModule
{
public:
	bool HasSettings = false;

	Versioning Versions;

	AboutModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "About";
		this->FancyName = "About";
	}

	virtual void Render();
	virtual void RenderAnyways() {}
	virtual void RenderSettings() {}
	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}
};