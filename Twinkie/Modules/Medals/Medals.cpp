#include "Medals.h"

ImVec4 g_MedalModuleMedalIconColors[4] = {
	{ 0.f, 0.875f, 0.15f, 1.f },
	{ 0.4643f, 0.3929f, 0.1429f, 1.f },
	{ 0.3077f, 0.3462f, 0.3462f, 1.f },
	{ 0.4737f, 0.3158f, 0.2105f, 1.f }
};

void MedalsModule::RenderAnyways()
{
	using namespace ImGui;
	if (Twinkie->IsPlaying())
	{
		ChallengeInfo InfoStruct = Twinkie->GetChallengeInfo();

		if (InfoStruct.AuthorTime == (MAXDWORD)) return;

		ChallengeInfo InfoStructDiff = {};

		InfoStructDiff.AuthorTime = (Twinkie->ChallengeUsesScore() ? InfoStruct.AuthorScore : InfoStruct.AuthorTime) - Twinkie->GetBestTime();
		InfoStructDiff.GoldTime = InfoStruct.GoldTime - Twinkie->GetBestTime();
		InfoStructDiff.SilverTime = InfoStruct.SilverTime - Twinkie->GetBestTime();
		InfoStructDiff.BronzeTime = InfoStruct.BronzeTime - Twinkie->GetBestTime();

		PushStyleColor(ImGuiCol_WindowBg, ColorBackground);
		auto WindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize;
		if (!*UiRenderEnabled) WindowFlags |= ImGuiWindowFlags_NoInputs;
		Begin("##Medals", nullptr, WindowFlags);
		PopStyleColor();

		TextColored(ColorTextPersonalBest, (PersonalBestName + ": " + (Twinkie->ChallengeUsesScore() ? std::to_string(Twinkie->GetBestTime()) : Twinkie->FormatTmDuration(Twinkie->GetBestTime()))).c_str());

		Separator();

		if (!Twinkie->ChallengeUsesScore())
		{
			TextColored(g_MedalModuleMedalIconColors[0], ICON_FK_CIRCLE);
			SameLine();
			TextColored(ColorTextMedals, (AuthorName + (AuthorName != "" ? ": " : "") + Twinkie->FormatTmDuration(InfoStruct.AuthorTime)).c_str());
			if (Twinkie->GetBestTime() != MAXDWORD)
			{
				SameLine();
				TextColored(InfoStructDiff.AuthorTime < 0 ? ColorTextBadDelta : ColorTextGoodDelta, ((InfoStructDiff.AuthorTime < 0 ? "+" : "-") + Twinkie->FormatTmDuration(abs(InfoStructDiff.AuthorTime))).c_str());
			}

			TextColored(g_MedalModuleMedalIconColors[1], ICON_FK_CIRCLE);
			SameLine();
			TextColored(ColorTextMedals, (GoldName + (GoldName != "" ? ": " : "") + Twinkie->FormatTmDuration(InfoStruct.GoldTime)).c_str());
			if (Twinkie->GetBestTime() != MAXDWORD)
			{
				SameLine();
				TextColored(InfoStructDiff.GoldTime < 0 ? ColorTextBadDelta : ColorTextGoodDelta, ((InfoStructDiff.GoldTime < 0 ? "+" : "-") + Twinkie->FormatTmDuration(abs(InfoStructDiff.GoldTime))).c_str());
			}

			TextColored(g_MedalModuleMedalIconColors[2], ICON_FK_CIRCLE);
			SameLine();
			TextColored(ColorTextMedals, (SilverName + (SilverName != "" ? ": " : "") + Twinkie->FormatTmDuration(InfoStruct.SilverTime)).c_str());
			if (Twinkie->GetBestTime() != MAXDWORD)
			{
				SameLine();
				TextColored(InfoStructDiff.SilverTime < 0 ? ColorTextBadDelta : ColorTextGoodDelta, ((InfoStructDiff.SilverTime < 0 ? "+" : "-") + Twinkie->FormatTmDuration(abs(InfoStructDiff.SilverTime))).c_str());
			}

			TextColored(g_MedalModuleMedalIconColors[3], ICON_FK_CIRCLE);
			SameLine();
			TextColored(ColorTextMedals, (BronzeName + (BronzeName != "" ? ": " : "") + Twinkie->FormatTmDuration(InfoStruct.BronzeTime)).c_str());
			if (Twinkie->GetBestTime() != MAXDWORD)
			{
				SameLine();
				TextColored(InfoStructDiff.BronzeTime < 0 ? ColorTextBadDelta : ColorTextGoodDelta, ((InfoStructDiff.BronzeTime < 0 ? "+" : "-") + Twinkie->FormatTmDuration(abs(InfoStructDiff.BronzeTime))).c_str());
			}
		}
		else
		{
			TextColored(g_MedalModuleMedalIconColors[0], ICON_FK_CIRCLE);
			SameLine();
			TextColored(ColorTextMedals, (AuthorName + (AuthorName != "" ? ": %lu" : "%lu")).c_str(), InfoStruct.AuthorScore);
			if (Twinkie->GetBestTime() != MAXDWORD)
			{
				SameLine();
				TextColored((InfoStruct.ChallengeType == 5 ? InfoStructDiff.AuthorTime > 0 : InfoStructDiff.AuthorTime < 0) ? ColorTextBadDelta : ColorTextGoodDelta, (((InfoStruct.ChallengeType == 5 ? InfoStructDiff.AuthorTime > 0 : InfoStructDiff.AuthorTime < 0) ? "+" : "-") + std::to_string(abs(InfoStructDiff.AuthorTime))).c_str());
			}

			TextColored(g_MedalModuleMedalIconColors[1], ICON_FK_CIRCLE);
			SameLine();
			TextColored(ColorTextMedals, (GoldName + (GoldName != "" ? ": %lu" : "%lu")).c_str(), InfoStruct.GoldTime);
			if (Twinkie->GetBestTime() != MAXDWORD)
			{
				SameLine();
				TextColored((InfoStruct.ChallengeType == 5 ? InfoStructDiff.GoldTime > 0 : InfoStructDiff.GoldTime < 0) ? ColorTextBadDelta : ColorTextGoodDelta, (((InfoStruct.ChallengeType == 5 ? InfoStructDiff.GoldTime > 0 : InfoStructDiff.GoldTime < 0) ? "+" : "-") + std::to_string(abs(InfoStructDiff.GoldTime))).c_str());
			}

			TextColored(g_MedalModuleMedalIconColors[2], ICON_FK_CIRCLE);
			SameLine();
			TextColored(ColorTextMedals, (SilverName + (SilverName != "" ? ": %lu" : "%lu")).c_str(), InfoStruct.SilverTime);
			if (Twinkie->GetBestTime() != MAXDWORD)
			{
				SameLine();
				TextColored((InfoStruct.ChallengeType == 5 ? InfoStructDiff.SilverTime > 0 : InfoStructDiff.SilverTime < 0) ? ColorTextBadDelta : ColorTextGoodDelta, (((InfoStruct.ChallengeType == 5 ? InfoStructDiff.SilverTime > 0 : InfoStructDiff.SilverTime < 0) ? "+" : "-") + std::to_string(abs(InfoStructDiff.SilverTime))).c_str());
			}

			TextColored(g_MedalModuleMedalIconColors[3], ICON_FK_CIRCLE);
			SameLine();
			TextColored(ColorTextMedals, (BronzeName + (BronzeName != "" ? ": %lu" : "%lu")).c_str(), InfoStruct.BronzeTime);
			if (Twinkie->GetBestTime() != MAXDWORD)
			{
				SameLine();
				TextColored((InfoStruct.ChallengeType == 5 ? InfoStructDiff.BronzeTime > 0 : InfoStructDiff.BronzeTime < 0) ? ColorTextBadDelta : ColorTextGoodDelta, (((InfoStruct.ChallengeType == 5 ? InfoStructDiff.BronzeTime > 0 : InfoStructDiff.BronzeTime < 0) ? "+" : "-") + std::to_string(abs(InfoStructDiff.BronzeTime))).c_str());
			}
		}

		End();
	}
}

void MedalsModule::RenderSettings()
{
	using namespace ImGui;
	ColorEdit4("Background color", &ColorBackground.x, ImGuiColorEditFlags_NoInputs);
	ColorEdit4("Medals text color", &ColorTextMedals.x, ImGuiColorEditFlags_NoInputs);
	ColorEdit4("Bad delta color", &ColorTextBadDelta.x, ImGuiColorEditFlags_NoInputs);
	ColorEdit4("Good delta color", &ColorTextGoodDelta.x, ImGuiColorEditFlags_NoInputs);
	ColorEdit4("Personal best text color", &ColorTextPersonalBest.x, ImGuiColorEditFlags_NoInputs);

	InputText("Personal best name", BufferPersonalBest, 255);
	PersonalBestName = BufferPersonalBest;

	InputText("Author name", BufferAuthor, 255);
	AuthorName = BufferAuthor;

	InputText("Gold name", BufferGold, 255);
	GoldName = BufferGold;

	InputText("Silver name", BufferSilver, 255);
	SilverName = BufferSilver;

	InputText("Bronze name", BufferBronze, 255);
	BronzeName = BufferBronze;
}

void MedalsModule::RenderMenuItem()
{
	using namespace ImGui;
	if (MenuItem(ICON_FK_ADJUST " Medals", "", Enabled))
	{
		Enabled = !Enabled;
	}
	Separator();
}

void MedalsModule::SettingsInit(SettingMgr& Settings)
{
	Settings["Medals"]["Enable"].GetAsBool(&Enabled);

	Settings["Medals"]["Background color"].GetAsVec4(&ColorBackground);

	Settings["Medals"]["Text color"].GetAsVec4(&ColorTextMedals);

	Settings["Medals"]["Bad delta text color"].GetAsVec4(&ColorTextBadDelta);
	Settings["Medals"]["Good delta text color"].GetAsVec4(&ColorTextGoodDelta);

	Settings["Medals"]["Personal best text color"].GetAsVec4(&ColorTextPersonalBest);

	Settings["Medals"]["Personal best name"].GetAsString(&PersonalBestName);
	if (strlen(PersonalBestName.c_str()) < 254) strcpy_s(BufferPersonalBest, PersonalBestName.c_str());
	else Logger->PrintErrorArgs("Could not import setting \"Personal best name\": too large ({} >= 254)", strlen(PersonalBestName.c_str()));

	AuthorName = Settings["Medals"]["Author name"].Value;
	if (strlen(AuthorName.c_str()) < 254) strcpy_s(BufferAuthor, AuthorName.c_str());
	else Logger->PrintErrorArgs("Could not import setting \"Author name\": too large ({} >= 254)", strlen(AuthorName.c_str()));

	GoldName = Settings["Medals"]["Gold name"].Value;
	if (strlen(GoldName.c_str()) < 254) strcpy_s(BufferGold, GoldName.c_str());
	else Logger->PrintErrorArgs("Could not import setting \"Gold name\": too large ({} >= 254)", strlen(GoldName.c_str()));

	SilverName = Settings["Medals"]["Silver name"].Value;
	if (strlen(SilverName.c_str()) < 254) strcpy_s(BufferSilver, SilverName.c_str());
	else Logger->PrintErrorArgs("Could not import setting \"Silver name\": too large ({} >= 254)", strlen(SilverName.c_str()));

	BronzeName = Settings["Medals"]["Bronze name"].Value;
	if (strlen(BronzeName.c_str()) < 254) strcpy_s(BufferBronze, BronzeName.c_str());
	else Logger->PrintErrorArgs("Could not import setting \"Bronze name\": too large ({} >= 254)", strlen(BronzeName.c_str()));
}

void MedalsModule::SettingsSave(SettingMgr& Settings)
{
	Settings["Medals"]["Enable"].Set(Enabled);

	Settings["Medals"]["Background color"].Set(ColorBackground);

	Settings["Medals"]["Text color"].Set(ColorTextMedals);

	Settings["Medals"]["Bad delta text color"].Set(ColorTextBadDelta);
	Settings["Medals"]["Good delta text color"].Set(ColorTextGoodDelta);

	Settings["Medals"]["Personal best text color"].Set(ColorTextPersonalBest);

	Settings["Medals"]["Personal best name"].Set(PersonalBestName);
	Settings["Medals"]["Author name"].Set(AuthorName);
	Settings["Medals"]["Gold name"].Set(GoldName);
	Settings["Medals"]["Silver name"].Set(SilverName);
	Settings["Medals"]["Bronze name"].Set(BronzeName);
}