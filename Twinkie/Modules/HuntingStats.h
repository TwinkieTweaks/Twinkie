#pragma once

#include "../IModule.h"

class HuntingStatsModule : public IModule
{
public:
	int Offset = 0;

	std::pair<std::string, int> Attempts;

	HuntingStatsModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger)
	{
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "HuntingStats";
		this->FancyName = "Hunting stats";
	}

	virtual void Render()
	{
		using namespace ImGui;

		Begin("HuntingStatsDbg", &Enabled);

		if (Twinkie->GetChallenge())
		{
			Text(Twinkie->GetChallengeUID().c_str());
		}

		End();
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
};