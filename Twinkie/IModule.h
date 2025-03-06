#pragma once

#include "SettingMgr.h"
#include "TwinkTrackmania.h"
#include "TwinkLogs.h"
#include "Utils.h"
#include "imgui-dx9/imgui.h"
#include <vector>

// this isn't *technically* an interface but it's the class all modules inherit from
class IModule
{
public:
	bool Enabled = false;
	std::string Name = "";
	std::string FancyName = "";
	TwinkTrackmania* Twinkie = nullptr;
	TwinkLogs* Logger = nullptr;

	IModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger)
	{
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
	}

	IModule(){}

	virtual void Render() {}
	virtual void RenderAnyways() {}
	virtual void RenderInactive() {}
	virtual void RenderSettings() {}
	virtual void RenderMenuItem() {}

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}

	virtual bool IsDebug() { return false; }
	virtual bool HasSettings() { return true; }
};