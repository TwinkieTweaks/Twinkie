#include "AlwaysOfficial.h"

void AlwaysOfficialModule::RenderAnyways()
{
	if (!Twinkie->IsProfileUnited()) return;
	if (!Twinkie->IsPlaying()) return;
	if (!Twinkie->GetPlayerInfo().TrackmaniaRace) return;

	if (!Twinkie->IsOnline())
	{
		CurrentState = Twinkie->GetState();

		if (CurrentState == TM::RaceState::BeforeStart and CurrentState != PreviousState)
		{
			Twinkie->CallSetOfficialRace();
		}

		PreviousState = Twinkie->GetState();
	}
}

void AlwaysOfficialModule::RenderMenuItem()
{
	using namespace ImGui;

	BeginDisabled(!Twinkie->IsProfileUnited());

	if (MenuItem("Always Official", "", Enabled))
	{
		Enabled = !Enabled;
	}

	if (IsItemHovered() and !Twinkie->IsProfileUnited())
	{
		SetTooltip("This module is deactivated because you are not logged in/have a Nations account.");
	}

	EndDisabled();
}

void AlwaysOfficialModule::SettingsInit(SettingMgr& Settings)
{
	Settings["Always Official"]["Enabled"].GetAsBool(&Enabled);
}

void AlwaysOfficialModule::SettingsSave(SettingMgr& Settings)
{
	Settings["Always Official"]["Enabled"].Set(Enabled);
}