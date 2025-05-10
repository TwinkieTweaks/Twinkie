#include "NicknamePatch.h"

void NicknamePatchModule::RenderMenuItem()
{
	if (ImGui::MenuItem("Nickname Patch"))
	{
		Twinkie->PatchNicknameEntry();

		Logger->PrintInternal("NicknamePatch: patched.");
	}
}