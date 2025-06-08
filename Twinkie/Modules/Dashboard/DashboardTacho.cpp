#include "DashboardTacho.h"

void DashboardTachometerModule::RenderAnyways()
{
	using namespace ImGui;

	if (!Twinkie->GetPlayerInfo().Vehicle) return;
	if (Twinkie->GetNameOfNod(Twinkie->GetPlayerInfo().Vehicle))

	int DashboardWindowFlags = ImGuiWindowFlags_NoTitleBar;
	if (!*UiRenderEnabled) DashboardWindowFlags |= ImGuiWindowFlags_NoInputs;

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
		float RepresentedRpm = (((float)Idx) / ((float)BarsToDraw)) * Twinkie->MAXRPM;
		float Rpm = Twinkie->GetRpm();
		ImVec4 Color = ColorDefault;
		if (Rpm > Twinkie->MINRPM and Rpm >= RepresentedRpm) {
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

void DashboardTachometerModule::RenderSettings()
{
	using namespace ImGui;
	ColorEdit4("Upshift", &ColorUpshift.x, ImGuiColorEditFlags_NoInputs);
	ColorEdit4("Middle", &ColorMiddle.x, ImGuiColorEditFlags_NoInputs);
	ColorEdit4("Downshift", &ColorDownshift.x, ImGuiColorEditFlags_NoInputs);
	ColorEdit4("Default", &ColorDefault.x, ImGuiColorEditFlags_NoInputs);

	Separator();

	ColorEdit4("Background color", &ColorBackground.x, ImGuiColorEditFlags_NoInputs);

	Separator();

	InputFloat("Upshift RPM", &UpshiftRpm);
	InputFloat("Downshift RPM", &DownshiftRpm);
}

void DashboardTachometerModule::RenderMenuItem()
{
	using namespace ImGui;
	if (MenuItem(ICON_FK_TACHOMETER " Tachometer", "Dashboard", Enabled))
	{
		Enabled = !Enabled;
	}
}


void DashboardTachometerModule::SettingsInit(SettingMgr& Settings)
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

void DashboardTachometerModule::SettingsSave(SettingMgr& Settings)
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