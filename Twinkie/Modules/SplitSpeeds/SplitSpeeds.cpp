#include "SplitSpeeds.h"

void SplitSpeedsModule::DrawSpeedAndSplitText(ImDrawList* DrawList, std::string ValueText, std::string DiffText, ImVec4 Color)
{
	using namespace ImGui;
	auto ScreenSize = GetMainViewport()->Size;

	ImVec2 SizeVal = CalcTextSize(ValueText.c_str());
	ImVec2 SizeDiff = CalcTextSize(DiffText.c_str());

	DrawList->AddText(ImVec2((ScreenSize.x / 2.f) - (SizeVal.x / 2.f) + 3, ScreenSize.y / 10.f + 3), ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), ValueText.c_str());
	DrawList->AddText(ImVec2((ScreenSize.x / 2.f) - (SizeVal.x / 2.f), ScreenSize.y / 10.f), ColorConvertFloat4ToU32(Color), ValueText.c_str());
	DrawList->AddText(ImVec2((ScreenSize.x / 2.f) - (SizeDiff.x / 2.f) + 3, ScreenSize.y / 10.f + 3 + SizeVal.y), ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), DiffText.c_str());
	DrawList->AddText(ImVec2((ScreenSize.x / 2.f) - (SizeDiff.x / 2.f), ScreenSize.y / 10.f + SizeVal.y), ColorConvertFloat4ToU32(Color), DiffText.c_str());
}

bool SplitSpeedsModule::IsPersonalBest()
{
	int PersonalBest = Twinkie->GetBestTime();

	if (PersonalBest == -1) return true;
	if (Twinkie->GetRaceTime() < PersonalBest and !Twinkie->ChallengeUsesScore())
	{
		return true;
	}
	else if (Twinkie->GetStuntsScore() > PersonalBest and !Twinkie->IsChallengePlatform())
	{
		return true;
	}
	return false;
}

void SplitSpeedsModule::RenderAnyways()
{
	using namespace ImGui;

	CurrentChallenge = Twinkie->GetChallenge();

	if (CurrentChallenge != PrevChallenge and CurrentChallenge)
	{
		BestSplits = LoadedSplits[Twinkie->GetChallengeUID()];
	}

	if (Twinkie->IsPlaying())
	{
		if (Twinkie->IsChallengePlatform()) return;
		CurrentState = Twinkie->GetState();

		CurrentRaceTime = Twinkie->GetRaceTime();
		if (CurrentRaceTime < LastRaceTime)
		{
			CurrentCheckpoint = -1;
			LastCheckpoint = -1;

			CurrentCheckpointIdx = -1;

			CurrentRaceTime = 0;
			LastRaceTime = 0;

			LastState = CurrentState;

			Splits = {};
			return;
		}

		LastRaceTime = CurrentRaceTime;
		
		CurrentCheckpoint = Twinkie->GetCurCheckpointTime();
		
		if (CurrentCheckpoint != LastCheckpoint)
		{
			Splits.push_back(Twinkie->GetDisplaySpeed());
			CurrentCheckpointIdx++;
		}

		LastCheckpoint = CurrentCheckpoint;

		if (CurrentCheckpointIdx != -1)
		{
			auto FGDrawList = GetForegroundDrawList();
			auto ScreenSize = GetWindowSize();

			float CurrentSplit = Splits[CurrentCheckpointIdx];

			float BestSplit = -1;
			bool IsFaster = false;
			bool IsNew = false;

			if (CurrentCheckpointIdx < BestSplits.size())
			{
				BestSplit = BestSplits[CurrentCheckpointIdx];
				IsFaster = CurrentSplit >= BestSplit;
				IsNew = false;
			}
			else
			{
				IsNew = true;
			}

			ImVec4 SplitTextCol = IsNew ? ImVec4(0, 0, 1, 1) : (IsFaster ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1));
			std::string ValueText = std::format("{}", CurrentSplit);
			float SplitDiff = CurrentSplit - BestSplit;
			std::string SignText = !IsNew ? (SplitDiff < 0 ? "{}" : "+{}") : "";
			std::string DiffText = std::vformat(SignText, std::make_format_args(SplitDiff));

			DrawSpeedAndSplitText(FGDrawList, ValueText, DiffText, SplitTextCol);
		}

		if (CurrentState == TM::RaceState::Finished)
		{
			if (CurrentState != ActualLastState)
			{
				auto IsPb = IsPersonalBest();
				if (IsPb)
				{
					BestSplits = Splits;
					LoadedSplits[Twinkie->GetChallengeUID()] = Splits;
					Logger->PrintArgs("Len: {}", Splits.size());
				}
			}
		}

		ActualLastState = CurrentState;
	}

	PrevChallenge = CurrentChallenge;
}

void SplitSpeedsModule::RenderMenuItem()
{
	using namespace ImGui;

	if (MenuItem(ICON_FK_EXCLAMATION_TRIANGLE " Split Speeds", "", Enabled))
	{
		Enabled = !Enabled;
	}
}

void SplitSpeedsModule::SettingsInit(SettingMgr& Settings)
{
	auto& SplitSpeedsSection = Settings["Split speeds"];
	for (auto& Value : SplitSpeedsSection.Settings)
	{
		LoadedSplits[Value.Name] = Value.GetAsFloats();
	}
}

void SplitSpeedsModule::SettingsSave(SettingMgr& Settings)
{
	auto& SplitSpeedsSection = Settings["Split speeds"];
	for (auto& Value : LoadedSplits)
	{
		SplitSpeedsSection[Value.first].Set(Value.second);
	}
}