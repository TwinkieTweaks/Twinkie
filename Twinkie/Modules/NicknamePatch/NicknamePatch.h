#pragma once

#include "../../IModule.h"

class NicknamePatchModule : public IModule
{
public:
	NicknamePatchModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;

		Name = "NicknamePatch";
		FancyName = "Nickname Patch";
	}

	NicknamePatchModule() = default;

	virtual void Render() {}
	virtual void RenderAnyways() {}
	virtual void RenderInactive() {}
	virtual void RenderSettings() {}
	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}

	virtual bool IsDebug() { return false; }
	virtual bool HasSettings() { return false; }
};