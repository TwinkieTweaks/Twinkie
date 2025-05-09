#pragma once

#include "../../IModule.h"

class AlwaysOfficalModule : public IModule
{
public:
	bool Enabled = false;
	const bool* UiRenderEnabled = nullptr;
	std::string Name = "AlwaysOffical";
	std::string FancyName = "Always Offical";
	TwinkTrackmania* Twinkie = nullptr;
	TwinkLogs* Logger = nullptr;

	TM::RaceState CurrentState = TM::RaceState::BeforeStart;
	TM::RaceState PreviousState = TM::RaceState::BeforeStart;

	AlwaysOfficalModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
	}

	AlwaysOfficalModule() = default;

	virtual void Render() {};
	virtual void RenderAnyways();
	virtual void RenderInactive() {};
	virtual void RenderSettings() {};
	virtual void RenderMenuItem() {};

	virtual void SettingsInit(SettingMgr& Settings);
	virtual void SettingsSave(SettingMgr& Settings);

	virtual bool IsDebug() { return false; };
	virtual bool HasSettings() { return false; };
};