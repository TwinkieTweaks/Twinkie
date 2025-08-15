#include "MapValidator.h"

void MapValidatorModule::RenderMenuItem()
{
	using namespace ImGui;
	if (MenuItem(ICON_FK_CHECK " Map Validator", "", Enabled))
	{
		Enabled = !Enabled;
	}
}

void MapValidatorModule::Render()
{
	static const char* MedalNames[] = {
		"Bronze",
		"Silver",
		"Gold",
		"Author"
	};

	if (!Twinkie->GetChallenge() or Twinkie->IsInMediaTracker())
	{
		LockValidation = false;
		memset(LockedTimes, 0, sizeof(LockedTimes));
		return;
	}

	if (!Twinkie->IsInEditor()) return;

	using namespace ImGui;
	if (Begin(ICON_FK_CHECK " Map Validator", &Enabled))
	{
		int* MapTimes = (int*)(Twinkie->Read<uintptr_t>(Twinkie->GetChallenge() + 0xB4) + 0x14);
		
		for (size_t Offset = 0; Offset < 4; Offset++)
		{
			int* MedalTime = MapTimes + Offset;
			InputInt(MedalNames[Offset], MedalTime);
			if (!LockValidation) LockedTimes[Offset] = *MedalTime;
		}

		Checkbox(ICON_FK_LOCK " Lock validation", &LockValidation);
	}
	End();

	if (LockValidation)
	{
		int* MapTimes = (int*)(Twinkie->Read<uintptr_t>(Twinkie->GetChallenge() + 0xB4) + 0x14);

		for (size_t Offset = 0; Offset < 4; Offset++)
		{
			int* MedalTime = MapTimes + Offset;
			*MedalTime = LockedTimes[Offset];
		}
	}
}