#include "GrindingStats.h"

void GrindingStatsModule::RenderMenuItem()
{
	using namespace ImGui;

	if (MenuItem(ICON_FK_CALENDAR " Grinding stats", "", &Enabled))
	{
		Enabled = !Enabled;
	}
}