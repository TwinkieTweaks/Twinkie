#pragma once

#include "../../Telemetry.h"
#include "../../IModule.h"

#define NoGripBitFlag 1 << 4
#define NoSteerBitFlag 1 << 3
#define NoBrakesBitFlag 1 << 2
#define ForcedAccelBitFlag 1 << 1
#define NoEngineBitFlag 1 << 0

long long Now();
void RenderFromState(NManiaPlanet::STelemetry& TelemetryStruct);

class TelemetryModule : public IModule
{
public:
	HANDLE TelemetryFileMapping = NULL;
	void* FileView = nullptr;
	NManiaPlanet::STelemetry Telemetry = {};

	bool Inited = false;

	TelemetryModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled);

	inline ~TelemetryModule()
	{
		UnmapViewOfFile(FileView);
		CloseHandle(TelemetryFileMapping);
	}

	TelemetryModule() = default;

	void UpdateTelemetry();

	virtual void Render();
	virtual void RenderAnyways() {};
	virtual void RenderInactive() { UpdateTelemetry(); };
	virtual void RenderSettings() {};
	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings) {};
	virtual void SettingsSave(SettingMgr& Settings) {};

	virtual bool IsDebug() { return false; };
	virtual bool HasSettings() { return false; };
};