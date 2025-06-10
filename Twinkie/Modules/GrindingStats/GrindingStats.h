#pragma once

#include "../../IModule.h"

class GrindingStatsModule : public IModule
{
public:
	GrindingStatsModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "GrindingStats";
		this->FancyName = "Grinding stats";
	}

	GrindingStatsModule() = default;

	virtual void Render() {}
	virtual void RenderAnyways() {}
	virtual void RenderInactive() {}
	virtual void RenderSettings() {}
	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}

	virtual bool IsDebug() { return false; }
	virtual bool HasSettings() { return true; }
};