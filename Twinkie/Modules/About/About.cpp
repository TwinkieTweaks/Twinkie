#include "About.h"

#define LinkOSS(name, url) Text(name); SameLine(); TextLinkOpenURL(url)

void AboutModule::Render()
{
	using namespace ImGui;
	if (Begin(ICON_FK_INFO_CIRCLE " About Twinkie", &Enabled, ImGuiWindowFlags_AlwaysAutoResize))
	{
		SeparatorText("About");
		Text(std::format("Twinkie for TrackMania Forever. Version {}", Versions.TwinkieVer).c_str());
		Text(std::format("Lua Engine Version {}", Versions.LuaVer).c_str());
		Text(std::format("Account type: {}", Twinkie->GetPayingAccountType() == TM::AccountType::Disconnected ? "Disconnected" : (Twinkie->GetPayingAccountType() == TM::AccountType::Nations ? "Nations" : "United")).c_str());
		Text(std::format("Game type: {}", Twinkie->IsGameInstallUnited() ? "United" : "Nations").c_str());
		Text("Made with love by jailman. <3");

		Text(ICON_FK_GITHUB); 
		SameLine();
		TextLinkOpenURL("https://github.com/TwinkieTweaks/Twinkie");

		Text(ICON_FK_DISCORD_ALT); 
		SameLine();
		TextLinkOpenURL("https://discord.gg/kRZ4MdCkVf");

#ifdef BUILD_DEBUG
		SeparatorText("Debug");
		Text("This is a debug copy, please report any bugs to the author, and do not share this with anyone.");
#endif

		SeparatorText("OSS");
		Text("Twinkie uses several open-source libraries. All of which are listed below.");
		LinkOSS("dear imgui: ", "https://github.com/ocornut/imgui");
		LinkOSS("mINI: ", "https://github.com/metayeti/mINI");
		LinkOSS("IconFontCppHeaders: ", "https://github.com/juliettef/IconFontCppHeaders");
	}
	End();
}

void AboutModule::RenderMenuItem()
{
	using namespace ImGui;
	if (MenuItem(ICON_FK_INFO_CIRCLE " About", "", Enabled))
	{
		Enabled = !Enabled;
	}
	Separator();
}