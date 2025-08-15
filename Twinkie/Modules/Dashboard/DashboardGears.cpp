#include "DashboardGears.h"

void DashboardGearsModule::SettingsSave(SettingMgr& Settings)
{
	Settings["Gears"]["Enable"].Set(Enabled);

	Settings["Gears"]["Background color"].Set(ColorBackground);
	Settings["Gears"]["Text color"].Set(ColorText);
}

void DashboardGearsModule::SettingsInit(SettingMgr& Settings)
{
	Settings["Gears"]["Enable"].GetAsBool(&Enabled);

	Settings["Gears"]["Background color"].GetAsVec4(&ColorBackground);
	Settings["Gears"]["Text color"].GetAsVec4(&ColorText);
}

void DashboardGearsModule::RenderMenuItem()
{
	using namespace ImGui;
	if (MenuItem(ICON_KI_COG " Gears", "Dashboard", Enabled))
	{
		Enabled = !Enabled;
	}
	Separator();
}

void DashboardGearsModule::RenderSettings()
{
	using namespace ImGui;
	ColorEdit4("Background color", &ColorBackground.x, ImGuiColorEditFlags_NoInputs);
	ColorEdit4("Text color", &ColorText.x, ImGuiColorEditFlags_NoInputs);
}

void DashboardGearsModule::RenderAnyways()
{
	using namespace ImGui;
	if (Twinkie->IsPlaying())
	{
		if (Twinkie->IsInEditor()) return;
		if (DisableTMO and std::find(TMOCarNames.begin(), TMOCarNames.end(), Twinkie->GetNameOfNod(Twinkie->CurPlayerInfo.Vehicle)) != TMOCarNames.end()) return;

		PushStyleColor(ImGuiCol_WindowBg, ColorBackground);

		auto WindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize;
		if (!*UiRenderEnabled) WindowFlags |= ImGuiWindowFlags_NoInputs;

		Begin("##Gears", nullptr, WindowFlags);

		PopStyleColor();

		TextColored(ColorText, "%d", Twinkie->GetGear());

		End();
	}
}