#pragma once

#include "../IModule.h"

class MedalsModule : public IModule
{
public:
	ImVec4 ColorText = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImVec4 ColorBackground = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 0.8f);

	MedalsModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger)
	{
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "Medals";
		this->FancyName = "Medals";
	}

	virtual void Render() {}

	virtual void RenderAnyways() 
	{
		using namespace ImGui;
		if (Twinkie->GetChallenge())
		{
			ChallengeInfo InfoStruct = Twinkie->GetChallengeInfo();
			PushStyleColor(ImGuiCol_WindowBg, ColorBackground);
			Begin("##Medals", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
			PopStyleColor();

			if (!Twinkie->ChallengeUsesScore())
			{
				TextColored(ColorText, ("Author: " + Twinkie->FormatTmDuration(InfoStruct.AuthorTime)).c_str());
				TextColored(ColorText, ("Gold: " + Twinkie->FormatTmDuration(InfoStruct.GoldTime)).c_str());
				TextColored(ColorText, ("Silver: " + Twinkie->FormatTmDuration(InfoStruct.SilverTime)).c_str());
				TextColored(ColorText, ("Bronze: " + Twinkie->FormatTmDuration(InfoStruct.BronzeTime)).c_str());
			}
			else
			{
				TextColored(ColorText, "Author: %lu", InfoStruct.AuthorScore);
				TextColored(ColorText, "Gold: %lu", InfoStruct.GoldTime);
				TextColored(ColorText, "Silver: %lu", InfoStruct.SilverTime);
				TextColored(ColorText, "Bronze: %lu", InfoStruct.BronzeTime);
			}

			End();
		}
	}

	virtual void RenderSettings()
	{
		using namespace ImGui;
		if (BeginTabItem(FancyName.c_str()))
		{
			ColorEdit4("Background color", &ColorBackground.x, ImGuiColorEditFlags_NoInputs);
			ColorEdit4("Text color", &ColorText.x, ImGuiColorEditFlags_NoInputs);

			EndTabItem();
		}
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
		Settings["Medals"]["Enable"].GetAsBool(&Enabled);

		Settings["Medals"]["Background color"].GetAsVec4(&ColorBackground);
		Settings["Medals"]["Text color"].GetAsVec4(&ColorText);
	}
	virtual void SettingsSave(SettingMgr& Settings)
	{
		Settings["Medals"]["Enable"].Set(Enabled);

		Settings["Medals"]["Background color"].Set(ColorBackground);
		Settings["Medals"]["Text color"].Set(ColorText);
	}
};