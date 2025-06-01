#pragma once

#include "../../IModule.h"

class CheckpointCounterModule : public IModule
{
public:

	ImVec4 ColorText = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImVec4 ColorBackground = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 0.8f);
	TwinkTrackmania* Twinkie = nullptr;
	TwinkLogs* Logger = nullptr;

	CheckpointCounterModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "CheckpointCounter";
		this->FancyName = "Checkpoint counter";
	}

	virtual void RenderSettings();

	virtual void RenderAnyways();

	virtual void Render() {}

	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings);

	virtual void SettingsSave(SettingMgr& Settings);
};