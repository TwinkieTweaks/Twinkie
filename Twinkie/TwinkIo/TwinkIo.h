#pragma once

#include "../TwinkTrackmania/TwinkTrackmania.h"
#include "../imgui-dx9/imgui.h"

class TwinkIo
{
public:
	TwinkTrackmania* Twinkie;

	TwinkIo(TwinkTrackmania& Twinkie)
	{
		this->Twinkie = &Twinkie;
	}

	void Update();
};