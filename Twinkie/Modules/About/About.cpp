#include "About.h"

void AboutModule::Render()
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

void AboutModule::RenderMenuItem()
{
	using namespace ImGui;
	if (MenuItem("About", "", Enabled))
	{
		Enabled = !Enabled;
	}
	Separator();
}