#include "Tweaker.h"

void TweakerModule::RenderInactive()
{
	if (Twinkie->IsPlaying()) Twinkie->SetCameraZClip(Enabled, RenderDistance, UseVoid);
}

void TweakerModule::RenderSettings()
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

void TweakerModule::SettingsInit(SettingMgr& Settings)
{
	Settings["Tweaker"]["Enable"].GetAsBool(&Enabled);
	Settings["Tweaker"]["Use void"].GetAsBool(&UseVoid);
	Settings["Tweaker"]["Render distance"].GetAsFloat(&RenderDistance);
}

void TweakerModule::SettingsSave(SettingMgr& Settings)
{
	Settings["Tweaker"]["Enable"].Set(Enabled);
	Settings["Tweaker"]["Use void"].Set(UseVoid);
	Settings["Tweaker"]["Render distance"].Set(RenderDistance);
}