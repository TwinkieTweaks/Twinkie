#include "Telemetry.h"

bool IsHandicapFlagSet(int HandicapFlag, int HandicapBitField)
{
	return (HandicapBitField & HandicapFlag) == HandicapFlag;
}

void RenderFromState(NManiaPlanet::STelemetry& TelemetryStruct)
{
	using namespace ImGui;

	SeparatorText("Header");

	Text("Magic: %s", TelemetryStruct.Header.Magic);
	Text("Version: %d", TelemetryStruct.Header.Version);
	Text("Size: %d", TelemetryStruct.Header.Size);
	Text("UpdateNumber: %d", TelemetryStruct.UpdateNumber);

	SeparatorText("Game");

	Text("State: %d", TelemetryStruct.Game.State);
	Text("GameplayVariant: %s", TelemetryStruct.Game.GameplayVariant);
	Text("MapId: %s", TelemetryStruct.Game.MapId);
	Text("MapName: %s", TelemetryStruct.Game.MapName);

	SeparatorText("Race");

	Text("State: %d", TelemetryStruct.Race.State);
	Text("Time: %d", TelemetryStruct.Race.Time);
	Text("NbRespawns: %d", TelemetryStruct.Race.NbRespawns);
	Text("NbCheckpoints: %d", TelemetryStruct.Race.NbCheckpoints);
	Text("NbCheckpointsPerLap: %d", TelemetryStruct.Race.NbCheckpointsPerLap);
	Text("NbLapsPerRace: %d", TelemetryStruct.Race.NbLapsPerRace);
	Text("Timestamp: %d", TelemetryStruct.Race.Timestamp);
	Text("StartTimestamp: %d", TelemetryStruct.Race.StartTimestamp);

	SeparatorText("Object");

	Text("Timestamp: %d", TelemetryStruct.Object.Timestamp);
	Text("DiscontinuityCount: %d", TelemetryStruct.Object.DiscontinuityCount);
	Text("LatestStableGroundContactTime: %d", TelemetryStruct.Object.LatestStableGroundContactTime);

	SeparatorText("Vehicle");

	Text("Timestamp: %d", TelemetryStruct.Vehicle.Timestamp);
	Text("InputSteer: %f", TelemetryStruct.Vehicle.InputSteer);
	Text("InputGasPedal: %f", TelemetryStruct.Vehicle.InputGasPedal);
	Text("InputIsBraking: %d", TelemetryStruct.Vehicle.InputIsBraking);
	Text("InputIsHorn: %d", TelemetryStruct.Vehicle.InputIsHorn);
	Text("EngineRpm: %f", TelemetryStruct.Vehicle.EngineRpm);
	Text("EngineCurGear: %d", TelemetryStruct.Vehicle.EngineCurGear);
	Text("EngineTurboRatio: %f", TelemetryStruct.Vehicle.EngineTurboRatio);
	Text("EngineFreeWheeling: %d", TelemetryStruct.Vehicle.EngineFreeWheeling);
	Text("RumbleIntensity: %f", TelemetryStruct.Vehicle.RumbleIntensity);
	Text("SpeedMeter: %d", TelemetryStruct.Vehicle.SpeedMeter);
	Text("IsInWater: %d", TelemetryStruct.Vehicle.IsInWater);
	Text("IsSparkling: %d", TelemetryStruct.Vehicle.IsSparkling);
	Text("IsLightTrails: %d", TelemetryStruct.Vehicle.IsLightTrails);
	Text("IsLightsOn: %d", TelemetryStruct.Vehicle.IsLightsOn);
	Text("IsFlying: %d", TelemetryStruct.Vehicle.IsFlying);
	Text("IsOnIce: %d", TelemetryStruct.Vehicle.IsOnIce);
	Text("HandicapNoEngine: %d", IsHandicapFlagSet(NoEngineBitFlag, TelemetryStruct.Vehicle.Handicap));

	SeparatorText("Device");

	Text("0");

	SeparatorText("Player");

	Text("Hue: %f", TelemetryStruct.Player.Hue);
	Text("Username: %s", TelemetryStruct.Player.UserName);
}

void TelemetryModule::UpdateTelemetry()
{
	if (Inited)
	{
		// Header
		Telemetry.UpdateNumber++;

		// Game
		if (Twinkie->IsPlaying())
		{
			strcpy_s(Telemetry.Game.GameplayVariant, Twinkie->GetNameOfNod(Twinkie->GetPlayerInfo().Vehicle).c_str());
			strcpy_s(Telemetry.Game.MapName, Twinkie->GetChallengeName().c_str());
			strcpy_s(Telemetry.Game.MapId, Twinkie->GetChallengeUID().c_str());
		}

		// Race
		if (Twinkie->IsPlaying())
		{
			Telemetry.Race.State = (NManiaPlanet::STelemetry::ERaceState)(int)Twinkie->GetState();
		}

		// Vehicle
		if (Twinkie->IsPlaying())
		{
			Telemetry.Vehicle.Timestamp = (int)Now();
			Telemetry.Vehicle.InputSteer = Twinkie->GetInputInfo().Steer;
			Telemetry.Vehicle.InputGasPedal = Twinkie->GetInputInfo().fGas;
			Telemetry.Vehicle.InputIsBraking = Twinkie->GetInputInfo().get_Brake();
			Telemetry.Vehicle.EngineRpm = Twinkie->GetRpm();
			Telemetry.Vehicle.EngineCurGear = Twinkie->GetGear();
			Telemetry.Vehicle.EngineTurboRatio = *((bool*)Twinkie->CurPlayerInfo.Vehicle + 948) ? 1.0 : 0.0;
			Telemetry.Vehicle.EngineFreeWheeling = *((bool*)Twinkie->CurPlayerInfo.Vehicle + 1548) ? 1 : 0;
			Telemetry.Vehicle.IsInWater = Twinkie->GetWaterPhysicsApplied();
		}
	}

	CopyMemory(FileView, &Telemetry, sizeof(Telemetry));
}

long long Now() {
	using namespace std::chrono;
	auto now = system_clock::now();
	auto duration = now.time_since_epoch();
	return duration_cast<seconds>(duration).count();
}

void TelemetryModule::RenderMenuItem()
{
	using namespace ImGui;

	if (MenuItem("TelemetryDbg", "", Enabled))
	{
		Enabled = !Enabled;
	}
}

void TelemetryModule::Render()
{
	using namespace ImGui;

	Begin("TelemetryDbg", &Enabled);

	RenderFromState(Telemetry);

	End();
}

TelemetryModule::TelemetryModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
{
	this->UiRenderEnabled = UiRenderEnabled;
	this->Twinkie = &Twinkie;
	this->Logger = &Logger;

	TelemetryFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4096, L"ManiaPlanet_Telemetry");

	if (TelemetryFileMapping == NULL)
	{
		this->Logger->PrintError("Could not initialize ManiaPlanet_Telemetry!");
		return;
	}

	FileView = (void*)MapViewOfFile(TelemetryFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 4096);
	if (FileView == nullptr)
	{
		this->Logger->PrintError("Could not view FileMapping for ManiaPlanet_Telemetry!");
		CloseHandle(TelemetryFileMapping);
		return;
	}

	if (TelemetryFileMapping != NULL and FileView != nullptr) UpdateTelemetry();
	Inited = true;
}