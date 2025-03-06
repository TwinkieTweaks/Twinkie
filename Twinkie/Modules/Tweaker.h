#pragma once

#include "../IModule.h"

class TweakerModule : public IModule
{
public:
	float RenderDistance = 5000.f;
	bool UseVoid = false;

	TweakerModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger)
	{
		this->Name = "Tweaker";
		this->FancyName = "Tweaker";
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
	}

	virtual void Render() {}
	virtual void RenderInactive() 
	{
		if (Twinkie->IsPlaying()) Twinkie->SetCameraZClip(Enabled, RenderDistance, UseVoid);
	}
	virtual void RenderSettings() 
	{
		using namespace ImGui;
		if (BeginTabItem(FancyName.c_str()))
		{
			Checkbox("Enable render distance", &Enabled);
			Checkbox("Replace unrendered things with void", &UseVoid);
			SliderFloat("Render distance", &RenderDistance, 50.f, 5000.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
			EndTabItem();
		}

	}
	virtual void RenderMenuItem() {}

	virtual void SettingsInit(SettingMgr& Settings) 
	{
		Settings["Tweaker"]["Enable"].GetAsBool(&Enabled);
		Settings["Tweaker"]["Use void"].GetAsBool(&UseVoid);
		Settings["Tweaker"]["Render distance"].GetAsFloat(&RenderDistance);
	}
	virtual void SettingsSave(SettingMgr& Settings) 
	{
		Settings["Tweaker"]["Enable"].Set(Enabled);
		Settings["Tweaker"]["Use void"].Set(UseVoid);
		Settings["Tweaker"]["Render distance"].Set(RenderDistance);
	}
};