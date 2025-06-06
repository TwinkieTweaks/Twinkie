#include "SplitSpeeds.h"

void SplitSpeedsModule::DrawSpeedAndSplitText(ImDrawList* DrawList, std::string ValueText, std::string DiffText, ImVec4 Color)
{
	using namespace ImGui;
	auto ScreenSize = ImGui::GetMainViewport()->Size;

	ImVec2 SizeVal = CalcTextSize(ValueText.c_str());
	ImVec2 SizeDiff = CalcTextSize(DiffText.c_str());

	DrawList->AddText(ImVec2((ScreenSize.x / 2.f) - (SizeVal.x / 2.f) + 3, ScreenSize.y / 10.f + 3), ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), ValueText.c_str());
	DrawList->AddText(ImVec2((ScreenSize.x / 2.f) - (SizeVal.x / 2.f), ScreenSize.y / 10.f), ColorConvertFloat4ToU32(Color), ValueText.c_str());
	DrawList->AddText(ImVec2((ScreenSize.x / 2.f) - (SizeDiff.x / 2.f) + 3, ScreenSize.y / 10.f + 3 + SizeVal.y), ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), DiffText.c_str());
	DrawList->AddText(ImVec2((ScreenSize.x / 2.f) - (SizeDiff.x / 2.f), ScreenSize.y / 10.f + SizeVal.y), ColorConvertFloat4ToU32(Color), DiffText.c_str());
}

void SplitSpeedsModule::RenderAnyways()
{
	static long LastCheckpoint = 0;
	static long CurrentCheckpoint = 0;

	static TM::RaceState CurrentState = TM::RaceState::Finished;
	static TM::RaceState LastState = TM::RaceState::Finished;

	static long LastRaceTime = 0;
	static long CurrentRaceTime = 0;

	static unsigned long CurrentCheckpointIdx = -1;

	static std::vector<long> Splits = {};
	static std::vector<long> BestSplits = { 434, 444, 355 };

	static bool DrawlistTesting = true;

	using namespace ImGui;


	if (Twinkie->IsPlaying())
	{
		if (Begin("SplitSpeeds", &Enabled))
		{
			Text("Last checkpoint: %lu", Twinkie->GetCurCheckpointTime());
			Checkbox("Test drawlist rendering", &DrawlistTesting);

			CurrentState = Twinkie->GetState();
			Text(std::format("BestTime: {}", Twinkie->GetBestTime()).c_str());

			CurrentRaceTime = Twinkie->GetRaceTime();
			if (CurrentRaceTime < LastRaceTime)
			{
				CurrentCheckpoint = 0;
				LastCheckpoint = 0;

				CurrentCheckpointIdx = -1;

				CurrentRaceTime = 0;
				LastRaceTime = 0;

				LastState = CurrentState;

				Splits = {};

				End();
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
		}

		End();

		if (DrawlistTesting and CurrentCheckpointIdx != -1)
		{
			auto FGDrawList = GetForegroundDrawList();
			auto ScreenSize = GetWindowSize();

			long CurrentSplit = Splits[CurrentCheckpointIdx];

			long BestSplit = -1;
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
			long SplitDiff = CurrentSplit - BestSplit;
			std::string IFuckingHateC20 = !IsNew ? (SplitDiff < 0 ? "{}" : "+{}") : "";
			std::string DiffText = std::vformat(IFuckingHateC20, std::make_format_args(SplitDiff));

			DrawSpeedAndSplitText(FGDrawList, ValueText, DiffText, SplitTextCol);
		}
	}
}