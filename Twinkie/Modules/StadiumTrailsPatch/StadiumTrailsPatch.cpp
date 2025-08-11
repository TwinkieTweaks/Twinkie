#include "StadiumTrailsPatch.h"

void StadiumTrailsPatchModule::RenderInactive()
{
	using namespace ImGui;
	static uintptr_t PrevLightTrailsFilePtr = 0;

	Begin("StadiumTrailsPatch");

	SeparatorText("Steps");

	if (Button("Create emitters"))
	{
		Emitters[0] = Twinkie->SceneEngineCreateInstance(0x0A010000);
		Emitters[1] = Twinkie->SceneEngineCreateInstance(0x0A010000);
		Logger->PrintInternalArgs("Emitters: {:x} {:x}", Emitters[0], Emitters[1]);

		HasCreatedEmitters = true;
	}

	LightTrailsFilePtr = Twinkie->LoadNodFromFilename((wchar_t*)L"Vehicles/MotionParticleEmitterModel/LightTrail.ParticleModel.Gbx", Twinkie->GetDataDrive());

	if (Button("Load light trails") or PrevLightTrailsFilePtr != LightTrailsFilePtr)
	{
		Logger->PrintInternalArgs("LightTrails: {:x}", LightTrailsFilePtr);
		((CMwNod*)LightTrailsFilePtr)->m_Dependants++;
		((CMwNod*)(((CMwNod*)LightTrailsFilePtr)->m_SystemFid))->m_Dependants++;
	}

	if (Button("Load VehicleStruct from file"))
	{
		VehicleStructFilePtr = Twinkie->LoadNodFromFilename((wchar_t*)L"Vehicles/VehicleStructs/StadiumCar.VehicleStruct.Gbx", Twinkie->GetDataDrive());
		Logger->PrintInternalArgs("VehicleStruct: {:x}", VehicleStructFilePtr);
		TM::CFastBuffer<uintptr_t>* EmittersBuffer = (TM::CFastBuffer<uintptr_t>*)(VehicleStructFilePtr + 0x38);
		Logger->PrintInternalArgs("Emitters: {:x} {}", (unsigned int)EmittersBuffer->Ptr, EmittersBuffer->Size);
	}

	if (Button("Load VehicleStruct from file (GreffMASTER)"))
	{
		VehicleStructFilePtr = Twinkie->LoadNodFromFilename((wchar_t*)L"Vehicles/VehicleStructs/_StadiumCar.VehicleStruct.Gbx", Twinkie->GetDataDrive());
		Logger->PrintInternalArgs("VehicleStruct: {:x}", VehicleStructFilePtr);
		TM::CFastBuffer<uintptr_t>* EmittersBuffer = (TM::CFastBuffer<uintptr_t>*)(VehicleStructFilePtr + 0x38);
		Logger->PrintInternalArgs("Emitters: {:x} {}", (unsigned int)EmittersBuffer->Ptr, EmittersBuffer->Size);
	}

	if (Button("Load VehicleStruct from CSceneVehicleCar"))
	{
		VehicleStructFilePtr = Twinkie->Read<uintptr_t>(Twinkie->CurPlayerInfo.Vehicle + 0x60);
		Logger->PrintInternalArgs("VehicleStruct: {:x}", VehicleStructFilePtr);
		TM::CFastBuffer<uintptr_t>* EmittersBuffer = (TM::CFastBuffer<uintptr_t>*)(VehicleStructFilePtr + 0x38);
		Logger->PrintInternalArgs("Emitters: {:x} {}", (unsigned int)EmittersBuffer->Ptr, EmittersBuffer->Size);
	}

	if (Button("Force current VehicleStruct into CSceneVehicleCar"))
	{
		Twinkie->Write<uintptr_t>(VehicleStructFilePtr, Twinkie->CurPlayerInfo.Vehicle + 0x60);
	}

	if (Emitters[0] != 0 and LightTrailsFilePtr != 0)
	{
		Twinkie->SetEmitterNodRefs(Emitters[0], LightTrailsFilePtr);
		Twinkie->SetEmitterNodRefs(Emitters[1], LightTrailsFilePtr);
	}

	if (Button("Initialize emitters"))
	{
		Twinkie->InitEmitter(Emitters[0], 1);
		Twinkie->InitEmitter(Emitters[1], 0);
		HasSetEmitterValues = true;
	}

	if (Button("Patch VehicleStructs"))
	{
		Twinkie->UseEmittersInVehicleStruct(Emitters[0], Emitters[1], VehicleStructFilePtr);
		HasPatchedVehicleStruct = true;
	}

	SeparatorText("Engines");

	TM::CFastBuffer<uintptr_t> Engines = Twinkie->Read<TM::CFastBuffer<uintptr_t>>(Twinkie->GetMainEngine() + 0x28);
	for (uintptr_t Engine : Engines)
	{
		Text("%x %x", Engine, Twinkie->GetMwClassId(Engine));
	}

	SeparatorText("Drives");

	for (size_t Idx = 0; Idx < 4; Idx++)
	{
		uintptr_t Drive = Twinkie->GetDriveByIdx(Idx);
		std::string DriveName = Twinkie->GetDriveName(Drive);
		Text("%s: %x", DriveName.c_str(), Drive);
	}
	End();

	PrevLightTrailsFilePtr = LightTrailsFilePtr;
}

void StadiumTrailsPatchModule::RenderSettings()
{

}