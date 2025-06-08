#pragma once

#include <map>
#include "../../IModule.h"

class SplitSpeedsModule : public IModule
{
	long LastCheckpoint = -1;
	long CurrentCheckpoint = -1;

	TM::RaceState CurrentState = TM::RaceState::Finished;
	TM::RaceState LastState = TM::RaceState::Finished;
	TM::RaceState ActualLastState = TM::RaceState::Finished;

	long LastRaceTime = 0;
	long CurrentRaceTime = 0;

	unsigned int CurrentCheckpointIdx = -1;
	
	uintptr_t PrevChallenge = 0;
	uintptr_t CurrentChallenge = 0;

	std::vector<float> Splits = {};
	std::vector<float> BestSplits = {};
	
	std::map<std::string, std::vector<float>> LoadedSplits = {};

public:
	SplitSpeedsModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
	}

	SplitSpeedsModule() = default;

	void DrawSpeedAndSplitText(ImDrawList* DrawList, std::string ValueText, std::string DiffText, ImVec4 TextColor, ImVec4 BgColor);
	bool IsPersonalBest();

	virtual void Render() {}
	virtual void RenderAnyways();
	virtual void RenderInactive() {}
	virtual void RenderSettings() {}
	virtual void RenderMenuItem();

	virtual void SettingsInit(SettingMgr& Settings);
	virtual void SettingsSave(SettingMgr& Settings);

	virtual bool IsDebug() { return false; }
	virtual bool HasSettings() { return false; }
};