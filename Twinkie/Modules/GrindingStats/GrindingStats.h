#pragma once

#include <map>
#include "../../IModule.h"

struct Stat
{
	unsigned long long Playtime;
	unsigned int Attempts;
	unsigned int Finishes;
	unsigned int Respawns;

	void FromString(std::string String)
	{
		std::stringstream Stream(String);
		std::string Token = "";

		try
		{
			std::getline(Stream, Token, ',');
			Playtime = std::stoull(Token);
			std::getline(Stream, Token, ',');
			Attempts = std::stoi(Token);
			std::getline(Stream, Token, ',');
			Finishes = std::stoi(Token);
			std::getline(Stream, Token, ',');
			Respawns = std::stoi(Token);
		}
		catch (...)
		{
			return;
		}
	}

	void ToString(std::string& String)
	{
		String = std::to_string(Playtime) + "," + std::to_string(Attempts) + "," + std::to_string(Finishes) + "," + std::to_string(Respawns);
	}

	operator std::string()
	{
		return std::to_string(Playtime) + "," + std::to_string(Attempts) + "," + std::to_string(Finishes) + "," + std::to_string(Respawns);
	}
};

class GrindingStatsModule : public IModule
{
	std::map<std::string, Stat> AllStats;
	Stat CurrentStat = {};
	Stat* CurrentTotalStat = nullptr;

	unsigned long long CurrentRaceTime = 0;
	unsigned long long PreviousRaceTime = 0;

	int CurrentSignedRaceTime = 0;
	int PreviousSignedRaceTime = 0;

	uintptr_t CurrentChallenge = 0;
	uintptr_t PreviousChallenge = 0;

	unsigned int CurrentRespawnCount = 0;
	unsigned int PreviousRespawnCount = 0;

	TM::RaceState CurrentState = TM::Finished;
	TM::RaceState PreviousState = TM::Finished;

	std::string LastLoadedChallengeUID = "";
	unsigned long long LastNonZeroUnsignedRaceTime = 0;

	ImVec4 ColorText = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImVec4 ColorBackground = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 0.8f);

public:
	GrindingStatsModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "GrindingStats";
		this->FancyName = "Grinding stats";
	}

	GrindingStatsModule() = default;

	void OnReset();
	void OnRespawn();
	void OnMapLoad();
	void OnMapUnload();
	void OnFinish();

	Stat& GetCurrentTotalStat();

	virtual void Render() {}
	virtual void RenderAnyways();
	virtual void RenderInactive();
	virtual void RenderSettings();
	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings);
	virtual void SettingsSave(SettingMgr& Settings);

	virtual bool IsDebug() { return false; }
	virtual bool HasSettings() { return true; }
};