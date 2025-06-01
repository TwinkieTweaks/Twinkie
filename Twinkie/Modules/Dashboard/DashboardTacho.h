#pragma once

#include "../../IModule.h"

class DashboardTachometerModule : public IModule
{
public:
	bool HasSettings = true;
	bool IsDebug = false;

	ImVec4 ColorUpshift = ImVec4(1.f, 0.f, 0.f, 1.f);
	ImVec4 ColorDownshift = ImVec4(1.f, 1.f, 0.f, 1.f);
	ImVec4 ColorMiddle = ImVec4(0.f, 1.f, 0.f, 1.f);
	ImVec4 ColorDefault = ImVec4(0.f, 0.f, 0.f, 0.25f);
	ImVec4 ColorBackground = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 0.8f);
	float DownshiftRpm = 6500;
	float UpshiftRpm = 10000;

	DashboardTachometerModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "Dashboard";
		this->FancyName = "Tachometer";
	}

	virtual void Render() {}
	virtual void RenderAnyways();
	virtual void RenderSettings();
	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings);
	virtual void SettingsSave(SettingMgr& Settings);
};