#include "CheckpointCounter.h"

void CheckpointCounterModule::RenderSettings()
{
	using namespace ImGui;
	ColorEdit4("Background color", &ColorBackground.x, ImGuiColorEditFlags_NoInputs);
	ColorEdit4("Text color", &ColorText.x, ImGuiColorEditFlags_NoInputs);
	Checkbox("Enable when online", &EnableWhenOnline);
}

void CheckpointCounterModule::RenderAnyways()
{
	using namespace ImGui;
	if (Twinkie->IsPlaying())
	{
		if (Twinkie->GetChallengeInfo().CheckpointCount == 0) return;
		if (Twinkie->IsOnline() and !EnableWhenOnline) return;
		if (Twinkie->IsInEditor()) return;
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
	Settings["CheckpointCounter"]["Enable when online"].GetAsBool(&EnableWhenOnline);
}

void CheckpointCounterModule::SettingsSave(SettingMgr& Settings)
{
	Settings["CheckpointCounter"]["Enable"].Set(Enabled);

	Settings["CheckpointCounter"]["Background color"].Set(ColorBackground);
	Settings["CheckpointCounter"]["Text color"].Set(ColorText);
	Settings["CheckpointCounter"]["Enable when online"].Set(EnableWhenOnline);
}
