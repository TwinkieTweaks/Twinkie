#include "CheckpointCounter.h"

void CheckpointCounterModule::RenderSettings()
{
	using namespace ImGui;
	ColorEdit4("Background color", &ColorBackground.x, ImGuiColorEditFlags_NoInputs);
	ColorEdit4("Text color", &ColorText.x, ImGuiColorEditFlags_NoInputs);
}

void CheckpointCounterModule::RenderAnyways()
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

void CheckpointCounterModule::RenderMenuItem()
{
	using namespace ImGui;
	if (MenuItem(ICON_FK_SORT_NUMERIC_ASC " Checkpoint counter", "", Enabled))
	{
		Enabled = !Enabled;
	}
	Separator();
}

void CheckpointCounterModule::SettingsInit(SettingMgr& Settings)
{
	Settings["CheckpointCounter"]["Enable"].GetAsBool(&Enabled);

	Settings["CheckpointCounter"]["Background color"].GetAsVec4(&ColorBackground);
	Settings["CheckpointCounter"]["Text color"].GetAsVec4(&ColorText);
}

void CheckpointCounterModule::SettingsSave(SettingMgr& Settings)
{
	Settings["CheckpointCounter"]["Enable"].Set(Enabled);

	Settings["CheckpointCounter"]["Background color"].Set(ColorBackground);
	Settings["CheckpointCounter"]["Text color"].Set(ColorText);
}
