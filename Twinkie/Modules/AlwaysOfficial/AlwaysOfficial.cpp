#include "AlwaysOfficial.h"

void AlwaysOfficialModule::RenderAnyways()
{
	if (!Twinkie->IsProfileUnited()) return;
	if (!Twinkie->IsPlaying()) return;
	if (!Twinkie->CurPlayerInfo.TrackmaniaRace) return;

	if (!Twinkie->IsOnline())
	{
		CurrentState = Twinkie->GetState();

		if (CurrentState == TM::RaceState::BeforeStart and CurrentState != PreviousState and (!Twinkie->IsOfficial() or PreviousState == TM::RaceState::Running))
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

	if (MenuItem(ICON_FK_TROPHY " Always Official", "", Enabled))
	{
		Enabled = !Enabled;
	}

	if (IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) and !Twinkie->IsProfileUnited())
	{
		SetTooltip("This module is deactivated because you are not logged in/have a Nations account.");
	}

	EndDisabled();
}