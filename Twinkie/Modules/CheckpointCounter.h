#pragma once

#include "../IModule.h"

class CheckpointCounterModule : public IModule
{
public:
	std::string Name = "CheckpointCounter";
	std::string FancyName = "Checkpoint counter";

	ImVec4 ColorText = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImVec4 ColorBackground = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 0.8f);

	CheckpointCounterModule() {}

	virtual void RenderSettings()
	{
		using namespace ImGui;
		ColorEdit4("Background color", &ColorBackground.x, ImGuiColorEditFlags_NoInputs);
		ColorEdit4("Text color", &ColorText.x, ImGuiColorEditFlags_NoInputs);
	}

	virtual void RenderAnyways(TwinkTrackmania& Twinkie)
	{
		using namespace ImGui;
		if (Twinkie.GetPlayerInfo().Vehicle)
		{
			if (Twinkie.GetChallengeInfo().CheckpointCount == 0) return;
			PushStyleColor(ImGuiCol_WindowBg, ColorBackground);
			Begin("##CheckpointCounter", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
			PopStyleColor();

			TextColored(ColorText, "%d / %d", Twinkie.GetCheckpointCount(), Twinkie.GetChallengeInfo().CheckpointCount);

			End();
		}
	}

	virtual void Render(TwinkTrackmania& Twinkie) {}

	virtual void RenderMenuItem(TwinkTrackmania& Twinkie)
	{
		using namespace ImGui;
		if (MenuItem(FancyName.c_str(), "", Enabled))
		{
			Enabled = !Enabled;
		}
	}

	virtual void SettingsInit(SettingMgr& Settings)
	{
		Settings[Name]["Enable"].GetAsBool(&Enabled);

		Settings[Name]["Background color"].GetAsVec4(&ColorBackground);
		Settings[Name]["Text color"].GetAsVec4(&ColorText);
	}

	virtual void SettingsSet(SettingMgr& Settings)
	{
		Settings[Name]["Enable"].Set(Enabled);

		Settings[Name]["Background color"].Set(ColorBackground);
		Settings[Name]["Text color"].Set(ColorText);
	}
};