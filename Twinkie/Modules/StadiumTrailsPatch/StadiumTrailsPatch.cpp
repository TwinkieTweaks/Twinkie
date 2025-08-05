#include "StadiumTrailsPatch.h"

void StadiumTrailsPatchModule::RenderInactive()
{
	using namespace ImGui;

	Begin("StadiumTrailsPatch");
	if (!HasCreatedEmitters)
	{
		if (Button("Create emitters"))
		{
			Emitters[0] = Twinkie->SceneEngineCreateInstance(0x0A010000);
			Emitters[1] = Twinkie->SceneEngineCreateInstance(0x0A010000);

			HasCreatedEmitters = true;
		}
	}
	else
	{
		Text("Emitter #0: %x", Emitters[0]);
		Text("Emitter #1: %x", Emitters[1]);
	}
	End();
}

void StadiumTrailsPatchModule::RenderSettings()
{

}