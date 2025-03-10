#pragma once

#include "../IModule.h"

class AppExplorerModule : public IModule
{
public:
	AppExplorerModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "AppExplorer";
		this->FancyName = "AppExplorer";
	}

	virtual void Render()
	{
		using namespace ImGui;
		Begin("AppExplorer", &Enabled);

		Text("ExeBaseAddr: %x", Twinkie->GetExeBaseAddr());
		Text("CTrackMania: %x", Twinkie->GetTrackmania());

		End();
	}

	virtual void RenderMenuItem()
	{
		using namespace ImGui;
		if (MenuItem(FancyName.c_str(), "", Enabled))
		{
			Enabled = !Enabled;
		}
	}

	virtual bool IsDebug() { return true; }
	virtual bool HasSettings() { return false; }
};