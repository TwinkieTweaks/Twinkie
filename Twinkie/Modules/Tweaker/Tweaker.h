#pragma once

#include "../../IModule.h"

class TweakerModule : public IModule
{
public:
	float RenderDistance = 5000.f;
	bool UseVoid = false;

	TweakerModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Name = "Tweaker";
		this->FancyName = "Tweaker";
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
	}

	virtual void Render() {}
	virtual void RenderInactive();
	virtual void RenderSettings();
	virtual void RenderMenuItem() {}

	virtual void SettingsInit(SettingMgr& Settings);
	virtual void SettingsSave(SettingMgr& Settings);
};