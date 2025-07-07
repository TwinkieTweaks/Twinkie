#include "About.h"

void AboutModule::Render()
{
	using namespace ImGui;
	if (Begin(ICON_FK_INFO_CIRCLE " About Twinkie", &Enabled, ImGuiWindowFlags_AlwaysAutoResize))
	{
		Text(std::format("Twinkie for TrackMania Forever. Version {}", Versions.TwinkieVer).c_str());
		Text(std::format("Account type: {}", Twinkie->GetPayingAccountType() == TM::AccountType::Disconnected ? "Disconnected" : (Twinkie->GetPayingAccountType() == TM::AccountType::Nations ? "Nations" : "United")).c_str());
		Text(std::format("Game type: {}", Twinkie->IsGameInstallUnited() ? "United" : "Nations").c_str());
		Text("Made with love by jailman. <3");
#ifdef BUILD_DEBUG
		Separator();
		Text("This is a debug copy, please report any bugs to the author.");
#endif
		Separator();

		Text(ICON_FK_GITHUB); 
		SameLine();
		TextLinkOpenURL("https://github.com/TwinkieTweaks/Twinkie");

		Text(ICON_FK_DISCORD_ALT); 
		SameLine();
		TextLinkOpenURL("https://discord.gg/kRZ4MdCkVf");
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