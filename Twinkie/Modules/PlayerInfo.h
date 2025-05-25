#pragma once

#include "../IModule.h"

class PlayerInfoModule : public IModule
{
public:
	PlayerInfoModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Name = "PlayerInfo";
		this->FancyName = "PlayerInformation";
	}

	virtual void Render() 
	{
		using namespace ImGui;
		static bool ShowOffsetTesting = false;

		static uintptr_t OffsetAddr = 0;
		static uintptr_t AddrOffset = 0;

		Begin("Player Information", &Enabled);

		if (Twinkie->CurPlayerInfo.Vehicle) 
		{
			if (!Twinkie->CurPlayerInfo.Player) return;
			SeparatorText("Addresses");

			Text("Address of GameApp: %x", Twinkie->GetTrackmania());
			SameLine();
			std::string GameAppAddrStr = ToHex(Twinkie->GetTrackmania());
			if (Button("Copy##GameApp"))
			{
				SetClipboardText(GameAppAddrStr.c_str());
				OffsetAddr = Twinkie->GetTrackmania();
			}

			Text("Address of Player: %x", Twinkie->CurPlayerInfo.Player);
			SameLine();
			std::string PlayerAddrStr = ToHex(Twinkie->CurPlayerInfo.Player);
			if (Button("Copy##Player"))
			{
				SetClipboardText(PlayerAddrStr.c_str());
				OffsetAddr = Twinkie->CurPlayerInfo.Player;
			}

			Text("Address of Mobil: %x", Twinkie->CurPlayerInfo.Mobil);
			SameLine();
			std::string MobilAddrStr = ToHex(Twinkie->CurPlayerInfo.Mobil);
			if (Button("Copy##Mobil"))
			{
				SetClipboardText(MobilAddrStr.c_str());
				OffsetAddr = Twinkie->CurPlayerInfo.Mobil;
			}

			Text("Address of Vehicle: %x", Twinkie->CurPlayerInfo.Vehicle);
			SameLine();
			std::string VehicleAddrStr = ToHex(Twinkie->CurPlayerInfo.Vehicle);
			if (Button("Copy##Vehicle"))
			{
				SetClipboardText(VehicleAddrStr.c_str());
				OffsetAddr = Twinkie->CurPlayerInfo.Vehicle;
			}

			Text("Address of PlayerInfo: %x", Twinkie->CurPlayerInfo.PlayerInfo);
			SameLine();
			std::string PlayerInfoAddrStr = ToHex(Twinkie->CurPlayerInfo.PlayerInfo);
			if (Button("Copy##PlayerInfo"))
			{
				SetClipboardText(PlayerInfoAddrStr.c_str());
				OffsetAddr = Twinkie->CurPlayerInfo.PlayerInfo;
			}

			Text("Address of TrackmaniaRace: %x", Twinkie->CurPlayerInfo.TrackmaniaRace);
			SameLine();
			std::string TrackmaniaRaceAddrStr = ToHex(Twinkie->CurPlayerInfo.TrackmaniaRace);
			if (Button("Copy##TrackmaniaRace"))
			{
				SetClipboardText(TrackmaniaRaceAddrStr.c_str());
				OffsetAddr = Twinkie->CurPlayerInfo.TrackmaniaRace;
			}

			SeparatorText("Race data");

			Text("Time: %lu", Twinkie->GetRaceTime());
			Text("Speed: %f", Twinkie->GetDisplaySpeed());
			Text("RPM: %f", Twinkie->GetRpm());
			Text("Gear: %lu", Twinkie->GetGear());
			Text("IsWet: %lu", Twinkie->GetWaterPhysicsApplied());
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
			Text("Turbo factor: %f", *((float*)Twinkie->CurPlayerInfo.Vehicle + 0x182) + 1.0f);

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
				if (Button("Copy"))
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

	virtual void RenderAnyways() {}
	virtual void RenderSettings() {}
	virtual void RenderMenuItem()
	{
		using namespace ImGui;
		if (MenuItem(FancyName.c_str(), "", Enabled))
		{
			Enabled = !Enabled;
		}
	}

	virtual void SettingsInit(SettingMgr& Settings) {}
	virtual void SettingsSave(SettingMgr& Settings) {}

	virtual bool IsDebug() { return true; }
	virtual bool HasSettings() { return false; }
};