#pragma once

#include "SettingMgr.h"
#include "TwinkTrackmania.h"
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

	virtual void Render(TwinkTrackmania& Twinkie) {}
	virtual void RenderAnyways(TwinkTrackmania& Twinkie) {}
	virtual void RenderSettings() {}
	virtual void RenderMenuItem(TwinkTrackmania& Twinkie) {}

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}

	virtual bool IsDebug() { return false; }
	virtual bool HasSettings() { return true; }
};