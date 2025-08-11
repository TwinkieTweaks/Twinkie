#include "TwinkIo.h"

void TwinkIo::Update()
{
	ImGuiIO& ImIo = ImGui::GetIO();

	TM::CFastArray<uintptr_t> Devices = Twinkie->GetDevices();

	for (uintptr_t Device : Devices)
	{
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