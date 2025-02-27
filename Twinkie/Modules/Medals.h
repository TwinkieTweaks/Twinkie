#pragma once

#include "../IModule.h"

class MedalsModule : public IModule
{
public:
	bool Enabled = false;
	std::string Name = "Medals";
	std::string FancyName = "Medals";

	ImVec4 ColorText = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImVec4 ColorBackground = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 0.8f);

	virtual void Render(TwinkTrackmania& Twinkie) {}

	virtual void RenderAnyways(TwinkTrackmania& Twinkie) 
	{
		using namespace ImGui;
		if (Twinkie.GetChallenge())
		{
			ChallengeInfo InfoStruct = Twinkie.GetChallengeInfo();
			PushStyleColor(ImGuiCol_WindowBg, ColorBackground);
			Begin("##Medals", &Enabled, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
			PopStyleColor();

			if (!Twinkie.ChallengeUsesScore())
			{
				TextColored(ColorText, ("Author: " + Twinkie.FormatTmDuration(InfoStruct.AuthorTime)).c_str());
				TextColored(ColorText, ("Gold: " + Twinkie.FormatTmDuration(InfoStruct.GoldTime)).c_str());
				TextColored(ColorText, ("Silver: " + Twinkie.FormatTmDuration(InfoStruct.SilverTime)).c_str());
				TextColored(ColorText, ("Bronze: " + Twinkie.FormatTmDuration(InfoStruct.BronzeTime)).c_str());
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

	virtual void RenderMenuItem(TwinkTrackmania& Twinkie)
	{
		using namespace ImGui;
		if (MenuItem(FancyName.c_str(), "", &Enabled))
		{
			Enabled = !Enabled;
		}
		Separator();
	}

	virtual void SettingsInit(SettingMgr& Settings) 
	{
		Settings[Name]["Enable"].Set(Enabled);

		Settings[Name]["Background color"].Set(ColorBackground);
		Settings[Name]["Text color"].Set(ColorText);
	}
	virtual void SettingsSave(SettingMgr& Settings) 
	{
		Settings[Name]["Enable"].GetAsBool(&Enabled);

		Settings[Name]["Background color"].GetAsVec4(&ColorBackground);
		Settings[Name]["Text color"].GetAsVec4(&ColorText);
	}
};