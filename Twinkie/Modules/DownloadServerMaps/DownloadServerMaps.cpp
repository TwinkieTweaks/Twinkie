#include "DownloadServerMaps.h"

void DownloadServerMapsModule::RenderInactive()
{
	using namespace ImGui;

	Begin("DownloadServerMapsModule");

	if (Button("SaveMapToDisk"))
	{
		Twinkie->CallSaveChallengeFromMemory();
	}

	End();
}