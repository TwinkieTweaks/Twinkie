#include "AlwaysOfficial.h"

void AlwaysOfficialModule::RenderAnyways()
{
	if (Twinkie->GetPlayerInfo().TrackmaniaRace)
	{
		CurrentState = Twinkie->GetState();

		if (CurrentState == TM::RaceState::BeforeStart and CurrentState != PreviousState)
		{
			Twinkie->CallSetOfficialRace();
		}

		PreviousState = Twinkie->GetState();
	}
}

void AlwaysOfficialModule::SettingsInit(SettingMgr& Settings)
{
	Settings["Always Offical"]["Enabled"].GetAsBool(&Enabled);
}

void AlwaysOfficialModule::SettingsSave(SettingMgr& Settings)
{
	Settings["Always Offical"]["Enabled"].Set(Enabled);
}