#pragma once

#include "../IModule.h"

class DashboardTachometerModule : public IModule
{
public:
	bool HasSettings = true;
	bool IsDebug = false;
	std::string Name = "Dashboard";
	std::string FancyName = "Tachometer";

	ImVec4 ColorUpshift = ImVec4(1.f, 0.f, 0.f, 1.f);
	ImVec4 ColorDownshift = ImVec4(1.f, 1.f, 0.f, 1.f);
	ImVec4 ColorMiddle = ImVec4(0.f, 1.f, 0.f, 1.f);
	ImVec4 ColorDefault = ImVec4(0.f, 0.f, 0.f, 0.25f);
	ImVec4 ColorBackground = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 0.8f);
	float DownshiftRpm = 6500;
	float UpshiftRpm = 10000;

	virtual void Render(TwinkTrackmania& Twinkie) {}
	virtual void RenderAnyways(TwinkTrackmania& Twinkie) 
	{
		using namespace ImGui;
			
		if (!Twinkie.GetPlayerInfo().Vehicle) return;
			
		int DashboardWindowFlags = ImGuiWindowFlags_NoTitleBar;
			
		PushStyleColor(ImGuiCol_WindowBg, ColorBackground);
			
		Begin("Dashboard##Rpm", nullptr, DashboardWindowFlags);
			
		PopStyleColor();
			
		auto UIDrawList = GetWindowDrawList();
			
		auto CursorPos = GetCursorScreenPos();
			
		float WindowWidth = GetWindowWidth();
		float WindowHeight = GetWindowHeight();
			
		int BarsToDraw = (int)WindowWidth / 10; // cast so we don't get the "possible loss of data" bullshit
		BarsToDraw--; // last bar almost always gets cutoff, i don't like that
			
		for (int Idx = 0; Idx < BarsToDraw; Idx++)
		{
			float RepresentedRpm = (((float)Idx) / ((float)BarsToDraw)) * Twinkie.MAXRPM;
			float Rpm = Twinkie.GetRpm();
			ImVec4 Color = ColorDefault;
			if (Rpm > Twinkie.MINRPM and Rpm >= RepresentedRpm) {
			    if (RepresentedRpm <= DownshiftRpm) {
			        Color = ColorDownshift;
			    }
			    else if (RepresentedRpm <= UpshiftRpm) {
			        Color = ColorMiddle;
			    }
			    else {
			        Color = ColorUpshift;
			    }
			}
			else
			{
			    Color = ColorDefault;
			}
			UIDrawList->AddRectFilled(CursorPos, ImVec2(CursorPos.x + 5.f, CursorPos.y + GetWindowHeight() - (2.f * GetStyle().WindowPadding.y)), ColorConvertFloat4ToU32(Color));
			CursorPos.x += 10.f; // 5.f from width of each bar and 5.f for padding between each bar
		}
			
		End();
	}
	virtual void RenderSettings() 
	{
		using namespace ImGui;
		if (BeginTabItem(FancyName.c_str()))
		{
			ColorEdit4("Upshift", &ColorUpshift.x, ImGuiColorEditFlags_NoInputs);
			ColorEdit4("Downshift", &ColorDownshift.x, ImGuiColorEditFlags_NoInputs);
			ColorEdit4("Middle", &ColorMiddle.x, ImGuiColorEditFlags_NoInputs);
			ColorEdit4("Default", &ColorDefault.x, ImGuiColorEditFlags_NoInputs);

			Separator();
			ColorEdit4("Background color", &ColorBackground.x, ImGuiColorEditFlags_NoInputs);

			Separator();

			InputFloat("Upshift RPM", &UpshiftRpm);
			InputFloat("Downshift RPM", &DownshiftRpm);

			EndTabItem();
		}
	}
	virtual void RenderMenuItem(TwinkTrackmania& Twinkie)
	{
		using namespace ImGui;
		if (MenuItem("Tachometer", "Dashboard", Enabled))
		{
			Enabled = !Enabled;
		}
		Separator();
	}

	virtual void SettingsInit(SettingMgr& Settings) 
	{
		Settings["Dashboard"]["Enable tachometer"].GetAsBool(&Enabled);

		Settings["Dashboard"]["Upshift"].GetAsVec4(&ColorUpshift);
		Settings["Dashboard"]["Downshift"].GetAsVec4(&ColorDownshift);
		Settings["Dashboard"]["Middle"].GetAsVec4(&ColorMiddle);
		Settings["Dashboard"]["Default"].GetAsVec4(&ColorDefault);

		Settings["Dashboard"]["Tachometer background color"].GetAsVec4(&ColorBackground);

		Settings["Dashboard"]["Upshift RPM"].GetAsFloat(&UpshiftRpm);
		Settings["Dashboard"]["Downshift RPM"].GetAsFloat(&DownshiftRpm);
	}
	virtual void SettingsSave(SettingMgr& Settings) 
	{
		Settings["Dashboard"]["Enable tachometer"].Set(Enabled);

		Settings["Dashboard"]["Upshift"].Set(ColorUpshift);
		Settings["Dashboard"]["Downshift"].Set(ColorDownshift);
		Settings["Dashboard"]["Middle"].Set(ColorMiddle);
		Settings["Dashboard"]["Default"].Set(ColorDefault);

		Settings["Dashboard"]["Tachometer background color"].Set(ColorBackground);

		Settings["Dashboard"]["Upshift RPM"].Set(UpshiftRpm);
		Settings["Dashboard"]["Downshift RPM"].Set(DownshiftRpm);
	}
};