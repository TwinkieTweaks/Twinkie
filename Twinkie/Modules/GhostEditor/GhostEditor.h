#pragma once

#include "../../IModule.h"

class GhostEditorModule : public IModule
{
public:
	GhostEditorModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "GhostEditor";
		this->FancyName = "Ghost editor";
	}

	virtual void Render() {}
	virtual void RenderAnyways() {}
	virtual void RenderSettings() {}

	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}

	virtual bool HasSettings() { return false; }
	virtual bool IsDebug() { return false; }
};