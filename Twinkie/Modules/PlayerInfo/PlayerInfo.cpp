#include "PlayerInfo.h"

void PlayerInfoModule::Render()
{
	using namespace ImGui;
	static bool ShowOffsetTesting = false;

	static uintptr_t OffsetAddr = 0;
	static uintptr_t AddrOffset = 0;

	Begin(ICON_FK_CODE " Player Information", &Enabled);

	auto DrawList = GetForegroundDrawList();

	if (Twinkie->CurPlayerInfo.Vehicle)
	{
		if (!Twinkie->CurPlayerInfo.Player) return;
		SeparatorText("Addresses");

		Text("Address of GameApp: %x", Twinkie->GetTrackmania());
		SameLine();
		std::string GameAppAddrStr = ToHex(Twinkie->GetTrackmania());
		if (SmallButton("Copy##GameApp"))
		{
			SetClipboardText(GameAppAddrStr.c_str());
			OffsetAddr = Twinkie->GetTrackmania();
		}

		Text("Address of Player: %x", Twinkie->CurPlayerInfo.Player);
		SameLine();
		std::string PlayerAddrStr = ToHex(Twinkie->CurPlayerInfo.Player);
		if (SmallButton("Copy##Player"))
		{
			SetClipboardText(PlayerAddrStr.c_str());
			OffsetAddr = Twinkie->CurPlayerInfo.Player;
		}

		Text("Address of Mobil: %x", Twinkie->CurPlayerInfo.Mobil);
		SameLine();
		std::string MobilAddrStr = ToHex(Twinkie->CurPlayerInfo.Mobil);
		if (SmallButton("Copy##Mobil"))
		{
			SetClipboardText(MobilAddrStr.c_str());
			OffsetAddr = Twinkie->CurPlayerInfo.Mobil;
		}

		Text("Address of Vehicle: %x", Twinkie->CurPlayerInfo.Vehicle);
		SameLine();
		std::string VehicleAddrStr = ToHex(Twinkie->CurPlayerInfo.Vehicle);
		if (SmallButton("Copy##Vehicle"))
		{
			SetClipboardText(VehicleAddrStr.c_str());
			OffsetAddr = Twinkie->CurPlayerInfo.Vehicle;
		}

		Text("Address of PlayerInfo: %x", Twinkie->CurPlayerInfo.PlayerInfo);
		SameLine();
		std::string PlayerInfoAddrStr = ToHex(Twinkie->CurPlayerInfo.PlayerInfo);
		if (SmallButton("Copy##PlayerInfo"))
		{
			SetClipboardText(PlayerInfoAddrStr.c_str());
			OffsetAddr = Twinkie->CurPlayerInfo.PlayerInfo;
		}

		Text("Address of TrackmaniaRace: %x", Twinkie->CurPlayerInfo.TrackmaniaRace);
		SameLine();
		std::string TrackmaniaRaceAddrStr = ToHex(Twinkie->CurPlayerInfo.TrackmaniaRace);
		if (SmallButton("Copy##TrackmaniaRace"))
		{
			SetClipboardText(TrackmaniaRaceAddrStr.c_str());
			OffsetAddr = Twinkie->CurPlayerInfo.TrackmaniaRace;
		}

		SeparatorText("Race data");

		Text("Time: %lu", Twinkie->GetRaceTime());
		Text("Speed: %f", Twinkie->GetDisplaySpeed());
		Text("RPM: %f", Twinkie->GetRpm());
		Text("Gear: %lu", Twinkie->GetGear());
		Checkbox("IsWet", (bool*)(Twinkie->CurPlayerInfo.Vehicle + 1508));
		Text("Personal best: %lu", Twinkie->GetBestTime());

		VehicleInputs InputInfo = Twinkie->GetInputInfo();

		// Text("Steer: %f", InputInfo.Steer);
		SliderFloat("Steer", (float*)(Twinkie->CurPlayerInfo.Vehicle + 88), -1.f, 1.f);
		Text("Gas: %f", InputInfo.fGas);
		Text("Brake: %f", InputInfo.fBrake);

		Checkbox("Mediatracker enabled", (bool*)Twinkie->CurPlayerInfo.Player + 56); // no Read here, ImGui reads the value internally
		auto MTClipIndex = Twinkie->Read<unsigned long>(Twinkie->CurPlayerInfo.Player + 72);
		if (*((bool*)Twinkie->CurPlayerInfo.Player + 56))
		{
			Text("Index of active mediatracker clip: %lu", Twinkie->Read<unsigned long>(Twinkie->CurPlayerInfo.Player + 72));
		}

		Checkbox("Free wheeling", (bool*)Twinkie->CurPlayerInfo.Vehicle + 1548);
		Checkbox("Turbo", (bool*)Twinkie->CurPlayerInfo.Vehicle + 948);
		Checkbox("Light trails", (bool*)Twinkie->CurPlayerInfo.Vehicle + 0x74);
		Text("Turbo factor: %f", *((float*)Twinkie->CurPlayerInfo.Vehicle + 0x182) + 1.0f);
		ColorEdit3("Trail color", (float*)(Twinkie->CurPlayerInfo.Vehicle + 0xC8));

		SeparatorText("Wheels");

		Indent();

		auto WheelBuf = Twinkie->GetVehicleWheels();

		Text("BufferPtr: %x", WheelBuf.Ptr);

		for (int Idx = 0; Idx < WheelBuf.Size; Idx++)
		{
			auto Wheel = WheelBuf[Idx];

			PushID(Idx);

			Text("Wheel #%d", Idx);
			Text("0x%x", Wheel);
			SameLine();
			if (SmallButton("Copy"))
			{
				SetClipboardText(ToHex((unsigned int)(uintptr_t)Wheel).c_str());
				OffsetAddr = (uintptr_t)Wheel;
			}

			Text("Material: %d", Twinkie->GetVehicleWheelMatId(Wheel));
			Text("Slipping: %d", Twinkie->GetVehicleWheelIsSlipping(Wheel) ? 1 : 0);
			Text("GroundContact: %d", Twinkie->GetVehicleWheelIsContactingGround(Wheel) ? 1 : 0);

			PopID();
		}

		Unindent();

		SeparatorText("Camera");

		CameraInfo CamInfo = Twinkie->GetCamInfo();

		TM::GmVec3 ExamplePoint = {};

		Text("Camera: 0x%x", CamInfo.HmsPocCamera);
		SameLine();
		if (SmallButton("Copy"))
		{
			SetClipboardText(ToHex((unsigned int)CamInfo.HmsPocCamera).c_str());
			OffsetAddr = CamInfo.HmsPocCamera;
		}

		Text("Fov: %f", CamInfo.Fov);
		Text("FarZ: %f", CamInfo.FarZ);
		Text("NearZ: %f", CamInfo.NearZ);
		Text("RatioXY: %f", CamInfo.AspectRatio);

		Text("Position: <%f, %f, %f>", CamInfo.Position.t.x, CamInfo.Position.t.y, CamInfo.Position.t.z);

		InputFloat3("Example point", (float*)&ExamplePoint);

		SeparatorText("Offset Testing");

		Checkbox("Show", &ShowOffsetTesting);

		if (ShowOffsetTesting)
		{
			InputInt("Address", reinterpret_cast<int*>(&OffsetAddr), 2);
			InputInt("Offset", reinterpret_cast<int*>(&AddrOffset), 2);

			if (OffsetAddr)
			{
				Text("Value: 0x%x, %lu, %f", Twinkie->Read<unsigned long>(OffsetAddr + AddrOffset), Twinkie->Read<unsigned long>(OffsetAddr + AddrOffset), Twinkie->Read<float>(OffsetAddr + AddrOffset));
			}
		}
	}
	else
	{
		Text("Not playing.");
	}

	End();
}

void PlayerInfoModule::RenderMenuItem()
{
	using namespace ImGui;
	if (MenuItem(ICON_FK_EXCLAMATION_TRIANGLE " PlayerInformation", "", Enabled))
	{
		Enabled = !Enabled;
	}
}
