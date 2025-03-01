#pragma once

#include "../IModule.h"

class DashboardGearsModule : public IModule
{
public:
	ImVec4 ColorText = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImVec4 ColorBackground = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 0.8f);

	DashboardGearsModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger)
	{
		this->Name = "Gears";
		this->FancyName = "Gear display";
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
	}

	virtual void Render() {}
	virtual void RenderAnyways() 
	{
		using namespace ImGui;
		if (Twinkie->IsPlaying())
		{
			PushStyleColor(ImGuiCol_WindowBg, ColorBackground);
			Begin("##Gears", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
			PopStyleColor();

			TextColored(ColorText, std::to_string(Twinkie->GetGear()).c_str());

			End();
		}
	}
	virtual void RenderSettings() 
	{
		using namespace ImGui;
		if (BeginTabItem(FancyName.c_str()))
		{
			ColorEdit4("Background color", &ColorBackground.x, ImGuiColorEditFlags_NoInputs);
			ColorEdit4("Text color", &ColorText.x, ImGuiColorEditFlags_NoInputs);

			EndTabItem();
		}
	}
	virtual void RenderMenuItem() 
	{
		using namespace ImGui;
		if (MenuItem(FancyName.c_str(), "Dashboard", Enabled))
		{
			Enabled = !Enabled;
		}
		Separator();
	}

	virtual void SettingsInit(SettingMgr& Settings) 
	{
		Settings["Gears"]["Enable"].GetAsBool(&Enabled);

		Settings["Gears"]["Background color"].GetAsVec4(&ColorBackground);
		Settings["Gears"]["Text color"].GetAsVec4(&ColorText);
	}

	virtual void SettingsSave(SettingMgr& Settings) 
	{
		Settings["Gears"]["Enable"].Set(Enabled);

		Settings["Gears"]["Background color"].Set(ColorBackground);
		Settings["Gears"]["Text color"].Set(ColorText);
	}
};