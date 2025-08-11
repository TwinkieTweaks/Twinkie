#pragma once

#include "../../IModule.h"

class StadiumTrailsPatchModule : public IModule
{
	uintptr_t Emitters[2] = { 0, 0 };
	bool HasCreatedEmitters = false;
	bool HasSetEmitterNodRefs = false;
	bool HasSetEmitterValues = false;
	bool HasPatchedVehicleStruct = false;
	uintptr_t LightTrailsFilePtr = 0;
	uintptr_t VehicleStructFilePtr = 0;

public:
	StadiumTrailsPatchModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "StadiumTrailsPatch";
		this->FancyName = "Patch stadium trails";
	}

	virtual void Render() {}

	virtual void RenderInactive();
	virtual void RenderAnyways() {}
	virtual void RenderSettings();
	virtual void RenderMenuItem() {}

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}

	virtual bool IsDebug() { return true; }
	virtual bool HasSettings() { return true; }
};