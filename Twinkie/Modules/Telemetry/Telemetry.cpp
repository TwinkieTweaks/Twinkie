#include "Telemetry.h"

NManiaPlanet::STelemetry::EGameState TelemetryModule::GetGameState()
{
	// NManiaPlanet::STelemetry::EGameState::EState_Starting could mean "the game is Starting", in that case it is never reachable because Twinkie never starts before the game does.
	// It could also mean "the race is Starting", which makes no sense because ERaceState already covers this.
	// But because Nadeo, no documentation on this exists, so you'll have to look for it yourself via Maniaplanet.
	// TODO: Do the above

	if (!Twinkie->IsPlaying()) return NManiaPlanet::STelemetry::EGameState::EState_Menus;
	if (Twinkie->IsPaused()) return NManiaPlanet::STelemetry::EGameState::EState_Paused;
	return NManiaPlanet::STelemetry::EGameState::EState_Running;
}

bool IsHandicapFlagSet(int HandicapFlag, int HandicapBitField)
{
	return (HandicapBitField & HandicapFlag) == HandicapFlag;
}

// ONLY USE THIS IF YOU'RE DEBUGGING THIS MODULE
//void RenderFromState(NManiaPlanet::STelemetry& TelemetryStruct)
//{
//	using namespace ImGui;
//
//	SeparatorText("Header");
//
//	Text("Magic: %s", TelemetryStruct.Header.Magic);
//	Text("Version: %d", TelemetryStruct.Header.Version);
//	Text("Size: %d", TelemetryStruct.Header.Size);
//	Text("UpdateNumber: %d", TelemetryStruct.UpdateNumber);
//
//	SeparatorText("Game");
//
//	Text("State: %d", TelemetryStruct.Game.State);
//	Text("GameplayVariant: %s", TelemetryStruct.Game.GameplayVariant);
//	Text("MapId: %s", TelemetryStruct.Game.MapId);
//	Text("MapName: %s", TelemetryStruct.Game.MapName);
//
//	SeparatorText("Race");
//
//	Text("State: %d", TelemetryStruct.Race.State);
//	Text("Time: %d", TelemetryStruct.Race.Time);
//	Text("NbRespawns: %d", TelemetryStruct.Race.NbRespawns);
//	Text("NbCheckpoints: %d", TelemetryStruct.Race.NbCheckpoints);
//	Text("NbCheckpointsPerLap: %d", TelemetryStruct.Race.NbCheckpointsPerLap);
//	Text("NbLapsPerRace: %d", TelemetryStruct.Race.NbLapsPerRace);
//	Text("Timestamp: %d", TelemetryStruct.Race.Timestamp);
//	Text("StartTimestamp: %d", TelemetryStruct.Race.StartTimestamp);
//
//	SeparatorText("Object");
//
//	Text("Timestamp: %d", TelemetryStruct.Object.Timestamp);
//	Text("DiscontinuityCount: %d", TelemetryStruct.Object.DiscontinuityCount);
//	Text("LatestStableGroundContactTime: %d", TelemetryStruct.Object.LatestStableGroundContactTime);
//
//	SeparatorText("Vehicle");
//
//	Text("Timestamp: %d", TelemetryStruct.Vehicle.Timestamp);
//	Text("InputSteer: %f", TelemetryStruct.Vehicle.InputSteer);
//	Text("InputGasPedal: %f", TelemetryStruct.Vehicle.InputGasPedal);
//	Text("InputIsBraking: %d", TelemetryStruct.Vehicle.InputIsBraking);
//	Text("InputIsHorn: %d", TelemetryStruct.Vehicle.InputIsHorn);
//	Text("EngineRpm: %f", TelemetryStruct.Vehicle.EngineRpm);
//	Text("EngineCurGear: %d", TelemetryStruct.Vehicle.EngineCurGear);
//	Text("EngineTurboRatio: %f", TelemetryStruct.Vehicle.EngineTurboRatio);
//	Text("EngineFreeWheeling: %d", TelemetryStruct.Vehicle.EngineFreeWheeling);
//	Text("RumbleIntensity: %f", TelemetryStruct.Vehicle.RumbleIntensity);
//	Text("SpeedMeter: %d", TelemetryStruct.Vehicle.SpeedMeter);
//	Text("IsInWater: %d", TelemetryStruct.Vehicle.IsInWater);
//	Text("IsSparkling: %d", TelemetryStruct.Vehicle.IsSparkling);
//	Text("IsLightTrails: %d", TelemetryStruct.Vehicle.IsLightTrails);
//	Text("IsLightsOn: %d", TelemetryStruct.Vehicle.IsLightsOn);
//	Text("IsFlying: %d", TelemetryStruct.Vehicle.IsFlying);
//	Text("IsOnIce: %d", TelemetryStruct.Vehicle.IsOnIce);
//	Text("HandicapNoEngine: %d", IsHandicapFlagSet(NoEngineBitFlag, TelemetryStruct.Vehicle.Handicap));
//
//	SeparatorText("Device");
//
//	Text("0");
//
//	SeparatorText("Player");
//
//	Text("Hue: %f", TelemetryStruct.Player.Hue);
//	Text("Username: %s", TelemetryStruct.Player.UserName);
//}

void TelemetryModule::UpdateTelemetry()
{
	if (Inited)
	{
		// Header
		Telemetry.UpdateNumber++;

		// Game
		Telemetry.Game.State = GetGameState();
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
			// Telemetry.Vehicle.Timestamp = (int)Now();
			Telemetry.Vehicle.InputSteer = Twinkie->GetInputInfo().Steer;
			Telemetry.Vehicle.InputGasPedal = Twinkie->GetInputInfo().fGas;
			Telemetry.Vehicle.InputIsBraking = Twinkie->GetInputInfo().get_Brake();
			Telemetry.Vehicle.EngineRpm = Twinkie->GetRpm();
			Telemetry.Vehicle.EngineCurGear = Twinkie->GetGear();
			Telemetry.Vehicle.EngineTurboRatio = *((bool*)Twinkie->CurPlayerInfo.Vehicle + 948) ? 1.f : 0.f;
			Telemetry.Vehicle.EngineFreeWheeling = *((bool*)Twinkie->CurPlayerInfo.Vehicle + 1548) ? 1 : 0;
			Telemetry.Vehicle.IsInWater = Twinkie->GetWaterPhysicsApplied();
			Telemetry.Vehicle.IsLightsOn = Twinkie->GetChallengeDecorationName().find("Night") != std::string::npos or Twinkie->GetChallengeDecorationName().find("Sunset") != std::string::npos;
		
			for (int Idx = 0; Idx < 4; Idx++)
			{
				Telemetry.Vehicle.WheelsIsGroundContact[Idx] = Twinkie->GetVehicleWheelIsContactingGround(Twinkie->GetVehicleWheels()[Idx]) ? 1 : 0;
				Telemetry.Vehicle.WheelsIsSliping[Idx] = Twinkie->GetVehicleWheelIsSlipping(Twinkie->GetVehicleWheels()[Idx]) ? 1 : 0;
			}
		}
	}

	CopyMemory(FileView, &Telemetry, sizeof(Telemetry));
}

unsigned long long Now() {
	using namespace std::chrono;
	auto now = system_clock::now();
	auto duration = now.time_since_epoch();
	return duration_cast<milliseconds>(duration).count();
}

void TelemetryModule::RenderMenuItem() {}

void TelemetryModule::RenderInactive()
{
	CurrentChallenge = Twinkie->GetChallenge();
	if (Enabled) UpdateTelemetry();
	PreviousChallenge = Twinkie->GetChallenge();
}

TelemetryModule::TelemetryModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
{
	this->UiRenderEnabled = UiRenderEnabled;
	this->Twinkie = &Twinkie;
	this->Logger = &Logger;
	this->Name = "Telemetry";
	this->FancyName = "Telemetry";

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

void TelemetryModule::RenderSettings()
{
	using namespace ImGui;

	if (BeginTabItem(FancyName.c_str()))
	{
		Checkbox("Opt in", &Enabled);
		 
		EndTabItem();
	}
}

void TelemetryModule::SettingsInit(SettingMgr& Settings)
{
	Settings["Telemetry"]["Opt in"].GetAsBool(&Enabled);
}

void TelemetryModule::SettingsSave(SettingMgr& Settings)
{
	Settings["Telemetry"]["Opt in"].Set(Enabled);
}