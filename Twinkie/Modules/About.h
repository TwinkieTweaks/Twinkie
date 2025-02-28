#pragma once

#include "../IModule.h"
#include "../Version.h"

class AboutModule : public IModule
{
public:
	bool HasSettings = false;
	std::string Name = "About";
	std::string FancyName = "About";

	Versioning Versions;

	virtual void Render(TwinkTrackmania& Twinkie) 
	{
		using namespace ImGui;
		if (Begin("About Twinkie", &Enabled))
		{
			Text(std::format("Twinkie for TrackMania{} Forever. Version {}", Twinkie.TMType == TM::GameType::Nations ? " Nations" : (Twinkie.TMType == TM::GameType::United ? " United" : ""), Versions.TwinkieVer).c_str());
			Text("Made with love by jailman. <3");
#ifdef BUILD_DEBUG
			Text("This is a debug copy, please report any bugs to the author.");
#endif
		}
		End();
	}
	virtual void RenderAnyways(TwinkTrackmania& Twinkie) {}
	virtual void RenderSettings() {}
	virtual void RenderMenuItem(TwinkTrackmania& Twinkie)
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