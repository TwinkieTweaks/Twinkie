#pragma once

#include "TwinkTrackmania.h"
#include "imgui-dx9/imgui.h"

class TwinkIo
{
public:
	TwinkTrackmania* Twinkie;

	TwinkIo(TwinkTrackmania& Twinkie)
	{
		this->Twinkie = &Twinkie;
	}

	void Update()
	{
		ImGuiIO& ImIo = ImGui::GetIO();

		TM::CFastArray<uintptr_t> Devices = Twinkie->GetDevices();

		for (size_t Idx = 0; Idx < Devices.Size; Idx++)
		{
			uintptr_t Device = Devices[Idx];

			int ClassType = Twinkie->GetMwClassId(Device);

			// Force devices to not be polled when ImGui wants them
			if (Twinkie->IsDeviceKeyboard(ClassType))
			{
				if (ImIo.WantCaptureKeyboard)
				{
					Twinkie->ForceDevicePoll(Device, 1);
				}
				else
				{
					Twinkie->ForceDevicePoll(Device, 0);
				}
			}
			else if (Twinkie->IsDeviceMouse(ClassType))
			{
				if (ImIo.WantCaptureMouse)
				{
					Twinkie->ForceDevicePoll(Device, 1);
				}
				else
				{
					Twinkie->ForceDevicePoll(Device, 0);
				}
			}
		}

		// Force mouse to appear when ImGui wants it
		if (ImIo.WantCaptureMouse)
		{
			uintptr_t InputPort = Twinkie->GetInputPort();
			if (InputPort) Twinkie->Write<int>(0, InputPort + 0xF4);
		}
	}
};