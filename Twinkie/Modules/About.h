#pragma once

#include "../IModule.h"
#include "../Version.h"

class AboutModule : public IModule
{
public:
	bool HasSettings = false;

	Versioning Versions;

	AboutModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "About";
		this->FancyName = "About";
	}

	virtual void Render() 
	{
		using namespace ImGui;
		if (Begin("About Twinkie", &Enabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			Text(std::format("Twinkie for TrackMania Forever. Version {}", Versions.TwinkieVer).c_str());
			Text("Made with love by jailman. <3");
#ifdef BUILD_DEBUG
			Separator();
			Text("This is a debug copy, please report any bugs to the author.");
#endif
			Separator();
			TextLinkOpenURL("https://github.com/TwinkieTweaks/Twinkie");
			TextLinkOpenURL("https://discord.gg/kRZ4MdCkVf");
		}
		End();
	}
	virtual void RenderAnyways() {}
	virtual void RenderSettings() {}
	virtual void RenderMenuItem()
	{
		using namespace ImGui;
		if (MenuItem("About", "", Enabled))
		{
			Enabled = !Enabled;
		}
		Separator();
	}

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}
};