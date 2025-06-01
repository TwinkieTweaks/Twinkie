#pragma once

#include "../../IModule.h"

class PlayerInfoModule : public IModule
{
public:
	PlayerInfoModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "PlayerInfo";
		this->FancyName = "PlayerInformation";
	}

	virtual void Render();

	virtual void RenderAnyways() {}
	virtual void RenderSettings() {}
	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}

	virtual bool IsDebug() { return true; }
	virtual bool HasSettings() { return false; }
};