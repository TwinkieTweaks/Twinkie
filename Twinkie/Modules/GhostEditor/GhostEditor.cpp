#include "GhostEditor.h"

void GhostEditorModule::RenderMenuItem()
{
	using namespace ImGui;
	if (MenuItem(FancyName.c_str()) and !Twinkie->GetChallenge() and Twinkie->GetProfileScores())
	{
		Twinkie->CallMenuGhostEditor();
	}
}