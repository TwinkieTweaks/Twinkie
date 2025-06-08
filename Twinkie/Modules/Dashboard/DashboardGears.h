#pragma once

#include "../../IModule.h"

class DashboardGearsModule : public IModule
{
public:
	ImVec4 ColorText = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImVec4 ColorBackground = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 0.8f);

	std::vector<std::string> TMOCarNames = { "American", "Rally", "SnowCar" };
	bool DisableTMO = false;

	DashboardGearsModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Name = "Gears";
		this->FancyName = "Gear display";
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
	}

	virtual void Render() {}
	virtual void RenderAnyways();
	virtual void RenderSettings();
	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings);
	virtual void SettingsSave(SettingMgr& Settings);
};