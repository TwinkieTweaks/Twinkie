#pragma once

#include "../../IModule.h"

class MedalsModule : public IModule
{
public:
	ImVec4 ColorTextMedals = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImVec4 ColorTextPersonalBest = ImVec4(0.443f, 0.573f, 0.988f, 1.f);
	ImVec4 ColorTextBadDelta = ImVec4(0.98f, 0.353f, 0.467f, 1.f);
	ImVec4 ColorTextGoodDelta = ImVec4(0.443f, 0.573f, 0.988f, 1.f);

	ImVec4 ColorBackground = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 0.8f);
	std::string PersonalBestName = "Personal best";
	char Buffer[255] = "Personal best";

	MedalsModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "Medals";
		this->FancyName = "Medals";
	}

	virtual void Render() {}
	virtual void RenderAnyways();
	virtual void RenderSettings();
	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings);
	virtual void SettingsSave(SettingMgr& Settings);
};