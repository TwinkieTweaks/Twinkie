#include "AlwaysOffical.h"

void AlwaysOfficalModule::RenderAnyways()
{
	if (Twinkie->GetPlayerInfo().TrackmaniaRace)
	{
		CurrentState = Twinkie->GetState();

		if (CurrentState == TM::RaceState::BeforeStart and CurrentState != PreviousState)
		{
			Twinkie->CallSetOfficalRace();
		}

		PreviousState = Twinkie->GetState();
	}
}

void AlwaysOfficalModule::SettingsInit(SettingMgr& Settings)
{
	Settings["Always Offical"]["Enabled"].GetAsBool(&Enabled);
}

void AlwaysOfficalModule::SettingsSave(SettingMgr& Settings)
{
	Settings["Always Offical"]["Enabled"].Set(Enabled);
}