#pragma once

#include "../../IModule.h"

class SplitSpeedsModule : public IModule
{
public:
	SplitSpeedsModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
	}

	SplitSpeedsModule() = default;

	void DrawSpeedAndSplitText(ImDrawList* DrawList, std::string ValueText, std::string DiffText, ImVec4 Color);

	virtual void Render() {}
	virtual void RenderAnyways();
	virtual void RenderInactive() {}
	virtual void RenderSettings() {}
	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings) {};
	virtual void SettingsSave(SettingMgr& Settings) {};

	virtual bool IsDebug() { return false; }
	virtual bool HasSettings() { return false; }
};