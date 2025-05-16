#include "ViewValidation.h"

void ViewValidationModule::Render()
{
	if (!Twinkie->GetGameCamera()) return;
	auto Camera = Twinkie->GetHmsPocCamera();
	if (!Twinkie->IsHmsPocHmsCamera(Camera)) return;
	Twinkie->Write<int>(1, Camera + 84);
}

void ViewValidationModule::RenderMenuItem()
{
	using namespace ImGui;
	if (MenuItem("View validation", "", Enabled))
	{
		Enabled = !Enabled;
	}
}