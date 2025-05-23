#pragma once

#include "../IModule.h"

class CheckpointCounterModule : public IModule
{
public:

	ImVec4 ColorText = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImVec4 ColorBackground = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 0.8f);
	TwinkTrackmania* Twinkie = nullptr;
	TwinkLogs* Logger = nullptr;

	CheckpointCounterModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "CheckpointCounter";
		this->FancyName = "Checkpoint counter";
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

	virtual void RenderAnyways()
	{
		using namespace ImGui;
		if (Twinkie->IsPlaying())
		{
			if (Twinkie->GetChallengeInfo().CheckpointCount == 0) return;
			PushStyleColor(ImGuiCol_WindowBg, ColorBackground);
			auto WindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize;
			if (!*UiRenderEnabled) WindowFlags |= ImGuiWindowFlags_NoInputs;
			Begin("##CheckpointCounter", nullptr, WindowFlags);
			PopStyleColor();

			TextColored(ColorText, "%d / %d", Twinkie->GetCheckpointCount(), Twinkie->GetChallengeInfo().CheckpointCount);

			End();
		}
	}

	virtual void Render() {}

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
		Settings["CheckpointCounter"]["Enable"].GetAsBool(&Enabled);

		Settings["CheckpointCounter"]["Background color"].GetAsVec4(&ColorBackground);
		Settings["CheckpointCounter"]["Text color"].GetAsVec4(&ColorText);
	}

	virtual void SettingsSave(SettingMgr& Settings)
	{
		Settings["CheckpointCounter"]["Enable"].Set(Enabled);

		Settings["CheckpointCounter"]["Background color"].Set(ColorBackground);
		Settings["CheckpointCounter"]["Text color"].Set(ColorText);
	}
};