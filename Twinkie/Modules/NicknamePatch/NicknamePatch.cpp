#include "NicknamePatch.h"

void NicknamePatchModule::RenderMenuItem()
{
	if (ImGui::MenuItem(ICON_FK_TEXT_WIDTH " Nickname Patch"))
	{
		Twinkie->PatchNicknameEntry();
	}
}