#pragma once

#include "../IModule.h"
#include <map>

std::string FormatTmDuration(unsigned long long Duration) {
	unsigned long long TotalSeconds = Duration / 1000ull;
	unsigned long long Millis = (Duration % 1000ull) / 10ull;
	unsigned long long Seconds = TotalSeconds % 60ull;
	unsigned long long Minutes = (TotalSeconds / 60ull) % 60ull;
	unsigned long long Hours = TotalSeconds / 3600ull;

	std::ostringstream StringStream;
	StringStream << std::setfill('0');

	if (Hours > 0ull) {
		StringStream << std::setw(2) << Hours << ":";
	}

	StringStream << std::setw(2) << Minutes << ":"
		<< std::setw(2) << Seconds << "."
		<< std::setw(2) << Millis;

	return StringStream.str();
}

struct PerMapStats
{
	int TotalFinishes = 0;
	int CurrentFinishes = 0;

	int TotalAttempts = 0;
	int CurrentAttempts = 0;

	int TotalRespawns = 0;
	int CurrentRespawns = 0;

	unsigned long long TotalTime = 0ull;
	unsigned long long CurrentTime = 0ull;

	std::string ToString()
	{
		return std::format("{},{},{},{}", TotalFinishes, TotalAttempts, TotalRespawns, TotalTime);
	}

	void MergeCurrentWithTotal()
	{
		TotalFinishes += CurrentFinishes;
		CurrentFinishes = 0;

		TotalAttempts += CurrentAttempts;
		CurrentAttempts = 0;

		TotalRespawns += CurrentRespawns;
		CurrentRespawns = 0;

		TotalTime += CurrentTime;
		CurrentTime = 0;
	}

	void LoadFromString(std::string String)
	{
		std::stringstream Stream(String);
		std::string Token;
		std::vector<std::string> Tokens;

		while (std::getline(Stream, Token, ','))
		{
			Tokens.push_back(Token);
		}

		if (Tokens.size() != 4) return;

		try
		{
			TotalFinishes = std::stoi(Tokens[0]);
			TotalAttempts = std::stoi(Tokens[1]);
			TotalRespawns = std::stoi(Tokens[2]);
			TotalTime = std::stoull(Tokens[3]);
		}
		catch (...)
		{
			return;
		}
	}

	void RenderStatistic(unsigned long long OffsetTime)
	{
		using namespace ImGui;
		Text("Finishes: %d / %d", CurrentFinishes, TotalFinishes);
		Text("Attempts: %d / %d", CurrentAttempts, TotalAttempts);
		Text("Respawns: %d / %d", CurrentRespawns, TotalRespawns);
		Text(std::format("Total time: {}", FormatTmDuration(TotalTime + OffsetTime)).c_str());
		Text(std::format("Session time: {}", FormatTmDuration(CurrentTime + OffsetTime)).c_str());
	}
};

class HuntingStatsModule : public IModule
{
public:
	int LastRespawns = 0;
	int CurrentRespawns = 0;

	TM::RaceState LastState = TM::RaceState::BeforeStart;
	TM::RaceState CurrentState = TM::RaceState::BeforeStart;

	long long CurrentRaceTime = 0;

	PerMapStats* CurrentStatistics = nullptr;

	std::map<std::string, PerMapStats> Stats;

	HuntingStatsModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "HuntingStats";
		this->FancyName = "Hunting stats";
	}

	virtual void RenderAnyways()
	{
		using namespace ImGui;

		if (!Twinkie->GetChallenge()) return;

		Begin("Hunting Stats", &Enabled, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);

		if (CurrentState == TM::RaceState::BeforeStart)
		{
			CurrentRaceTime = 0;
		}

		if (Stats.contains(Twinkie->GetChallengeUID()))
		{
			CurrentStatistics = &Stats[Twinkie->GetChallengeUID()];
			CurrentStatistics->RenderStatistic(CurrentRaceTime);
		}
		else
		{
			Stats[Twinkie->GetChallengeUID()] = {};
		}

		End();
	}

	virtual void RenderInactive()
	{
		if (!Twinkie->GetChallenge() or !Twinkie->GetPlayerInfo().Player) return;
		if (Twinkie->GetChallengeUID() == "Unassigned") return;

		CurrentRespawns = Twinkie->GetRespawns();
		CurrentState = Twinkie->GetState();
		CurrentRaceTime = Twinkie->GetRaceTime();

		if (Stats.contains(Twinkie->GetChallengeUID()))
		{
			CurrentStatistics = &Stats[Twinkie->GetChallengeUID()];

			if (LastState != CurrentState and CurrentState == TM::RaceState::Finished and CurrentRaceTime != 0 and CurrentRaceTime != -1) // ULL_MAX
			{
				CurrentStatistics->CurrentFinishes++;

				CurrentStatistics->CurrentTime += CurrentRaceTime;
				CurrentStatistics->TotalTime += CurrentRaceTime;
			}

			if (LastState != CurrentState and CurrentState == TM::RaceState::BeforeStart and CurrentRaceTime != 0 and CurrentRaceTime != -1) // ULL_MAX
			{
				// the Twinkie->GetRaceTime() != 0 might seem nonsensical, since when you begin the race then your racetime should reset
				// however it is *only* reset *after* you begin the race
				// so e.g. let's say you reset at 10100
				// then: 
				// Running <-> 10000
				// Running <-> 10100       tick
				// BeforeStart <-> 10100   reset
				// ...
				// Running <-> 0           race begun
				// the reason we do not want the racetime to be 0 is that it is 0 when we first load the map

				CurrentStatistics->CurrentAttempts++;

				CurrentStatistics->CurrentTime += CurrentRaceTime;
				CurrentStatistics->TotalTime += CurrentRaceTime;
			}

			if (LastRespawns != CurrentRespawns and CurrentRespawns > LastRespawns)
			{
				CurrentStatistics->CurrentRespawns++;
			}
		}
		else
		{
			Stats[Twinkie->GetChallengeUID()] = {};
			CurrentStatistics = &Stats[Twinkie->GetChallengeUID()];
		}

		Stats[Twinkie->GetChallengeUID()] = *CurrentStatistics;

		LastState = CurrentState;
		LastRespawns = CurrentRespawns;
	}

	virtual void RenderMenuItem()
	{
		using namespace ImGui;
		if (MenuItem(FancyName.c_str(), "", Enabled))
		{
			Enabled = !Enabled;
		}
		Separator();
	}

	virtual void SettingsInit(SettingMgr& Settings)
	{
		auto& Section = Settings["HuntingStats"];

		for (auto& Key : Section.Settings)
		{
			Stats.insert(std::pair<std::string, PerMapStats>(Key.Name, {}));

			Stats[Key.Name].LoadFromString(Key.Value);
		}

		Section["Enable"].GetAsBool(&Enabled);
	}

	virtual void SettingsSave(SettingMgr& Settings)
	{
		auto& Section = Settings["HuntingStats"];

		for (auto& Stat : Stats)
		{
			Stat.second.MergeCurrentWithTotal();
			Section[Stat.first].Value = Stat.second.ToString();
		}

		Section["Enable"].Set(Enabled);
	}

	virtual bool HasSettings() { return false; }
};