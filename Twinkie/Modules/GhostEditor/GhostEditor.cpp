#include "GhostEditor.h"

void GhostEditorModule::RenderMenuItem()
{
	using namespace ImGui;
	if (MenuItem(ICON_FK_CAR " Ghost editor") and !Twinkie->GetChallenge() and Twinkie->GetProfileScores())
	{
		Twinkie->CallMenuGhostEditor();
	}
}