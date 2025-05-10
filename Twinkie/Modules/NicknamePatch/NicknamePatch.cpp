#include "NicknamePatch.h"

void NicknamePatchModule::RenderMenuItem()
{
	if (ImGui::MenuItem("Nickname Patch"))
	{
		std::vector<int> IndiciesToNicknameEntry = { 0, 0, 1, 1, 1 };
		
		auto MenuManagerMenus = Twinkie->Read<uintptr_t>(Twinkie->GetMenuManager() + 0x788);
		
		TM::CFastArray<uintptr_t> Frames = Twinkie->Read<TM::CFastArray<uintptr_t>>(MenuManagerMenus + 104);
		auto FrameProfile2 = Frames.Ptr[4];
		
		uintptr_t CurrentFrame = FrameProfile2;
		for (int Idx : IndiciesToNicknameEntry)
		{
			CurrentFrame = (Twinkie->Read<TM::CFastArray<uintptr_t>>(CurrentFrame + 324)).Ptr[Idx];
		}
		uintptr_t NicknameEntry = CurrentFrame;

		Twinkie->Write<int>(NicknameEntry + 0x150, 70);

		Logger->PrintInternal("NicknamePatch: patched.");
	}
}