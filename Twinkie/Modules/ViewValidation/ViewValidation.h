#pragma once

#include "../../IModule.h"

class ViewValidationModule : public IModule
{
public:
	ViewValidationModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;

		Name = "ViewValidation";
		FancyName = "View validation";
	}

	ViewValidationModule() = default;

	virtual void Render();
	virtual void RenderAnyways() {};
	virtual void RenderInactive() {};
	virtual void RenderSettings() {};
	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings) {};
	virtual void SettingsSave(SettingMgr& Settings) {};

	virtual bool IsDebug() { return false; };
	virtual bool HasSettings() { return false; };
};