#pragma once

#include "../../IModule.h"

class MapValidatorModule : public IModule
{
	bool LockValidation = false;

	int LockedTimes[4] = { 0, 0, 0, 0 };

public:
	MapValidatorModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
	}

	MapValidatorModule() = default;

	virtual ~MapValidatorModule() = default;

	virtual void Render();
	virtual void RenderAnyways() {}
	virtual void RenderInactive() {}
	virtual void RenderSettings() {}
	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}

	virtual bool IsDebug() { return false; }
	virtual bool HasSettings() { return false; }
};