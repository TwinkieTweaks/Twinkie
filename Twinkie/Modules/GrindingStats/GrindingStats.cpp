#include "GrindingStats.h"

void GrindingStatsModule::RenderMenuItem()
{
	using namespace ImGui;

	if (MenuItem(ICON_FK_CALENDAR " Grinding Stats", "", Enabled))
	{
		Enabled = !Enabled;
	}
}

Stat& GrindingStatsModule::GetCurrentTotalStat()
{
	return AllStats[LastLoadedChallengeUID];
}

void GrindingStatsModule::SettingsInit(SettingMgr& Settings)
{
	auto& GrindingStatsSection = Settings["Grinding Stats"];

	for (Setting& SettingValue : GrindingStatsSection.Settings)
	{
		if (!SettingValue.Name.starts_with("U_")) continue;
		std::string KeyToAccess = SettingValue.Name.substr(2);
		Stat NewStat = {};
		NewStat.FromString(SettingValue.Value);
		AllStats[KeyToAccess] = NewStat;
	}

	GrindingStatsSection["Enable UI"].GetAsBool(&Enabled);
}

void GrindingStatsModule::SettingsSave(SettingMgr& Settings)
{
	auto& GrindingStatsSection = Settings["Grinding Stats"];

	for (auto& Pair : AllStats)
	{
		GrindingStatsSection["U_" + Pair.first].Value = Pair.second;
	}

	GrindingStatsSection["Enable UI"].Set(Enabled);
}

void GrindingStatsModule::RenderInactive()
{
	CurrentChallenge = Twinkie->GetChallenge();

	if (!Twinkie->IsPlaying())
	{
		if (PreviousChallenge != CurrentChallenge and CurrentChallenge == 0)
		{
			OnMapUnload();
		}
		if (PreviousChallenge != CurrentChallenge and CurrentChallenge != 0)
		{
			OnMapLoad();
		}
		PreviousChallenge = CurrentChallenge;
		return;
	}

	LastLoadedChallengeUID = Twinkie->GetChallengeUID();
	CurrentRaceTime = Twinkie->GetRaceTime();
	CurrentSignedRaceTime = Twinkie->GetSignedRaceTime();
	CurrentRespawnCount = Twinkie->GetRespawns();
	CurrentState = Twinkie->GetState();

	if (CurrentRaceTime != 0)
	{
		LastNonZeroUnsignedRaceTime = CurrentRaceTime;
	}

	if (PreviousChallenge != CurrentChallenge)
	{
		OnMapLoad();
	}

	// if (PreviousSignedRaceTime != CurrentSignedRaceTime and CurrentSignedRaceTime < 0 and PreviousSignedRaceTime > CurrentSignedRaceTime and CurrentSignedRaceTime != -1)
	if (PreviousState != CurrentState and CurrentState == TM::BeforeStart)
	{
		OnReset();
	}

	if (PreviousRespawnCount != CurrentRespawnCount and CurrentRespawnCount > PreviousRespawnCount)
	{
		OnRespawn();
	}

	if (PreviousState != CurrentState and CurrentState == TM::Finished)
	{
		OnFinish();
	}

	PreviousRaceTime = CurrentRaceTime;
	PreviousSignedRaceTime = CurrentSignedRaceTime;
	PreviousChallenge = CurrentChallenge;
	PreviousRespawnCount = CurrentRespawnCount;
	PreviousState = CurrentState;
}

void GrindingStatsModule::RenderAnyways()
{
	if (!Twinkie->IsPlaying()) return;

	using namespace ImGui;

	unsigned long long TotalPlaytime = GetCurrentTotalStat().Playtime;
	unsigned long long CurrentPlaytime = CurrentStat.Playtime;

	if (!CurrentState == TM::BeforeStart)
	{
		TotalPlaytime += CurrentRaceTime;
		CurrentPlaytime += CurrentRaceTime;
	}

	auto WindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize;
	if (!*UiRenderEnabled) WindowFlags |= ImGuiWindowFlags_NoInputs;
	Begin("##GrindingStats", nullptr, WindowFlags);

	Text(ICON_FK_CLOCK_O " Total time: %s", Twinkie->FormatTmDuration((unsigned int)TotalPlaytime).c_str());
	Text(ICON_FK_PLAY_CIRCLE_O " Session time: %s", Twinkie->FormatTmDuration((unsigned int)CurrentPlaytime).c_str());
	Text(ICON_FK_FLAG_CHECKERED " Finishes: %d / %d", CurrentStat.Finishes, GetCurrentTotalStat().Finishes);
	Text(ICON_FK_REPEAT " Attempts: %d / %d", CurrentStat.Attempts, GetCurrentTotalStat().Attempts);
	Text(ICON_FK_REFRESH " Respawns: %d / %d / %d", Twinkie->GetRespawns(), CurrentStat.Respawns, GetCurrentTotalStat().Respawns);

	End();
}

void GrindingStatsModule::OnReset()
{
	GetCurrentTotalStat().Attempts++;
	CurrentStat.Attempts++;
	GetCurrentTotalStat().Playtime += LastNonZeroUnsignedRaceTime;
	CurrentStat.Playtime += LastNonZeroUnsignedRaceTime;
}

void GrindingStatsModule::OnRespawn()
{
	GetCurrentTotalStat().Respawns++;
	CurrentStat.Respawns++;
}

void GrindingStatsModule::OnMapLoad()
{
	LastNonZeroUnsignedRaceTime = 0;
	CurrentStat = { 0, 0, 0, 0 };
}

void GrindingStatsModule::OnMapUnload()
{
	CurrentStat = { 0, 0, 0, 0 };
	GetCurrentTotalStat().Playtime += CurrentRaceTime;
}

void GrindingStatsModule::OnFinish()
{
	GetCurrentTotalStat().Finishes++;
	CurrentStat.Finishes++;
}