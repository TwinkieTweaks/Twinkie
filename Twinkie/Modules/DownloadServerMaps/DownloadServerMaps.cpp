#include "DownloadServerMaps.h"

void DownloadServerMapsModule::RenderInactive()
{
	using namespace ImGui;

	if (!Enabled) return;

	CurrentChallenge = Twinkie->GetChallenge();

	if (CurrentChallenge != PreviousChallenge and Twinkie->IsOnline() and CurrentChallenge)
	{
		Twinkie->CallSaveChallengeFromMemory();
	}

	PreviousChallenge = Twinkie->GetChallenge();
}

void DownloadServerMapsModule::RenderSettings()
{
	using namespace ImGui;

	if (BeginTabItem("Map Downloader"))
	{
		Checkbox("Enable map downloads on servers", &Enabled);

		EndTabItem();
	}
}