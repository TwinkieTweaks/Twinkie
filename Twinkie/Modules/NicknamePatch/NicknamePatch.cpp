#include "NicknamePatch.h"

void NicknamePatchModule::RenderMenuItem()
{
	if (ImGui::MenuItem(ICON_FK_LIGHTBULB_O " Nickname Patch"))
	{
		Twinkie->PatchNicknameEntry();
	}
}