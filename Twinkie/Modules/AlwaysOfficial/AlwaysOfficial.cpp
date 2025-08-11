#include "AlwaysOfficial.h"

void AlwaysOfficialModule::RenderAnyways()
{
	if (!Twinkie->IsProfileUnited()) return;
	if (!Twinkie->IsPlaying()) return;

	if (!Twinkie->IsOnline())
	{
		CurrentRacetime = Twinkie->GetSignedRaceTime();

		if (CurrentRacetime < PreviousRacetime and CurrentRacetime < 0 and !Twinkie->IsOfficial())
		{
			Twinkie->CallSetOfficialRace();
		}

		PreviousRacetime = CurrentRacetime;
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