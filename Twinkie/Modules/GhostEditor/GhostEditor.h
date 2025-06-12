#pragma once

#include "../../IModule.h"

class GhostEditorModule : public IModule
{
public:
	GhostEditorModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled);

	void PatchChallengeMainLoop();

	virtual void Render() {}
	virtual void RenderAnyways() {}
	virtual void RenderSettings() {}

	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}

	virtual bool HasSettings() { return false; }
	virtual bool IsDebug() { return false; }
};