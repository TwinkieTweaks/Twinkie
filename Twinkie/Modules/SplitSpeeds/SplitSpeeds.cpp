#include "SplitSpeeds.h"

void SplitSpeedsModule::DrawSpeedAndSplitText(ImDrawList* DrawList, std::string ValueText, std::string DiffText, ImVec4 TextColor, ImVec4 BgColor)
{
	using namespace ImGui;
	auto ScreenSize = GetMainViewport()->Size;

	ImVec2 SizeVal = CalcTextSize(ValueText.c_str());
	ImVec2 SizeDiff = DiffText == "" ? SizeVal : CalcTextSize(DiffText.c_str());

	ImVec2 ValCoord = { (ScreenSize.x / 2.f) - (SizeVal.x / 2.f), ScreenSize.y / 10.f };
	ImVec2 DiffCoord = DiffText != "" ? ImVec2((ScreenSize.x / 2.f) - (SizeDiff.x / 2.f), ScreenSize.y / 10.f + SizeVal.y) : ValCoord;

	ImVec2 RectMin;
	ImVec2 RectMax;

	RectMin.x = min(ValCoord.x, DiffCoord.x) - 5.f;
	RectMin.y = min(ValCoord.y, DiffCoord.y) - 5.f;

	RectMax.x = max(ValCoord.x + (SizeVal.x), DiffCoord.x + (SizeDiff.x)) + 5.f;
	RectMax.y = max(ValCoord.y + (SizeVal.y), DiffCoord.y + (SizeDiff.y)) + 5.f;

	DrawList->AddRectFilled(RectMin, RectMax, ColorConvertFloat4ToU32(BgColor));
	DrawList->AddText(ImVec2((ScreenSize.x / 2.f) - (SizeVal.x / 2.f), ScreenSize.y / 10.f), ColorConvertFloat4ToU32(TextColor), ValueText.c_str());
	DrawList->AddText(ImVec2((ScreenSize.x / 2.f) - (SizeDiff.x / 2.f), ScreenSize.y / 10.f + SizeVal.y), ColorConvertFloat4ToU32(TextColor), DiffText.c_str());
}

bool SplitSpeedsModule::IsPersonalBest()
{
	int PersonalBest = Twinkie->GetBestTime();

	if (PersonalBest == -1) return true;
	if (Twinkie->GetRaceTime() == PersonalBest and !Twinkie->ChallengeUsesScore())
	{
		return true;
	}
	else if (Twinkie->GetStuntsScore() == PersonalBest and !Twinkie->IsChallengePlatform())
	{
		return true;
	}
	return false;
}

void SplitSpeedsModule::RenderInactive()
{
	using namespace ImGui;

	if (Twinkie->TMInterfaceLoaded) return;

	CurrentChallenge = Twinkie->GetChallenge();

	if (CurrentChallenge != PrevChallenge and CurrentChallenge)
	{
		BestSplits = LoadedSplits[Twinkie->GetChallengeUID()];
	}

	if (Twinkie->IsPlaying())
	{
		if (Twinkie->IsChallengePlatform() or Twinkie->IsOnline()) return;
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
			Splits.push_back(UnsignedSpeed ? fabs(Twinkie->GetDisplaySpeed()) : Twinkie->GetDisplaySpeed());
			CurrentCheckpointIdx++;
		}

		LastCheckpoint = CurrentCheckpoint;

		if (CurrentCheckpointIdx != -1 and CurrentState != TM::RaceState::BeforeStart)
		{
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

			if (Enabled)
			{
				auto FGDrawList = GetForegroundDrawList();
				auto ScreenSize = GetWindowSize();

				ImVec4 SplitTextCol = IsNew ? ColorTextEqual : (IsFaster ? ColorTextAhead : ColorTextBehind);
				std::string ValueTextFormatText = std::vformat("{{:.{}f}}", std::make_format_args(DigitsToShow));
				std::string ValueText = std::vformat(ValueTextFormatText, std::make_format_args(CurrentSplit));
				float SplitDiff = CurrentSplit - BestSplit;
				std::string SignText = !IsNew ? (SplitDiff < 0 ? ValueTextFormatText : "+" + ValueTextFormatText) : "";
				std::string DiffText = std::vformat(SignText, std::make_format_args(SplitDiff));

				DrawSpeedAndSplitText(FGDrawList, ValueText, DiffText, SplitTextCol, ColorBg);
			}
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

	if (MenuItem(ICON_FK_CLOCK_O " Split Speeds", "", Enabled))
	{
		Enabled = !Enabled;
	}
}

void SplitSpeedsModule::RenderSettings()
{
	using namespace ImGui;

	ColorEdit4("Ahead color", (float*)&ColorTextAhead, ImGuiColorEditFlags_NoInputs);
	ColorEdit4("Behind color", (float*)&ColorTextBehind, ImGuiColorEditFlags_NoInputs);
	ColorEdit4("Equal/New color", (float*)&ColorTextEqual, ImGuiColorEditFlags_NoInputs);
	ColorEdit4("Background color", (float*)&ColorBg, ImGuiColorEditFlags_NoInputs);

	Separator();

	SliderInt("Digits to show", &DigitsToShow, 0, 10);
}

void SplitSpeedsModule::SettingsInit(SettingMgr& Settings)
{
	auto& SplitSpeedsSection = Settings["Split speeds"];
	for (auto& Value : SplitSpeedsSection.Settings)
	{
		if (!Value.Name.starts_with("U_")) continue;
		std::string KeyToAccess = Value.Name.substr(2);
		LoadedSplits[KeyToAccess] = Value.GetAsFloats();
	}
	SplitSpeedsSection["Enabled"].GetAsBool(&Enabled);

	SplitSpeedsSection["Ahead color"].GetAsVec4(&ColorTextAhead);
	SplitSpeedsSection["Behind color"].GetAsVec4(&ColorTextBehind);
	SplitSpeedsSection["Equal color"].GetAsVec4(&ColorTextEqual);
	SplitSpeedsSection["Background color"].GetAsVec4(&ColorBg);

	SplitSpeedsSection["Digits to show"].GetAsInt(&DigitsToShow);

	SplitSpeedsSection["Show absolute speed"].GetAsBool(&UnsignedSpeed);
}

void SplitSpeedsModule::SettingsSave(SettingMgr& Settings)
{
	auto& SplitSpeedsSection = Settings["Split speeds"];
	for (auto& Value : LoadedSplits)
	{
		if (Value.second.size() == 0) continue;
		std::string KeyToAccess = "U_" + Value.first;
		SplitSpeedsSection[KeyToAccess].Set(Value.second);
	}
	SplitSpeedsSection["Enabled"].Set(Enabled);

	SplitSpeedsSection["Ahead color"].Set(ColorTextAhead);
	SplitSpeedsSection["Behind color"].Set(ColorTextBehind);
	SplitSpeedsSection["Equal color"].Set(ColorTextEqual);
	SplitSpeedsSection["Background color"].Set(ColorBg);

	SplitSpeedsSection["Digits to show"].Set(DigitsToShow);

	SplitSpeedsSection["Show absolute speed"].Set(UnsignedSpeed);
}