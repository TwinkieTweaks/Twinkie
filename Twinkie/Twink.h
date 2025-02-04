#pragma once

#include "TrackMania.h"
#include "ProcHandler.h"

#include "imgui-dx9/imgui.h"
#include "imgui-dx9/imgui_impl_win32.h"
#include "imgui-dx9/imgui_impl_dx9.h"

class Twink
{
public:
	uintptr_t O_TRACKMANIA = 0xD6A2A4;
	uintptr_t O_DX9DEVICE = 0xD70C00;

	ProcHandler Handler;

    bool DoRender = true;

    bool EnableTrails = true;
    TM::GmVec3 TrailsColor = { 1,1,0 };
    float TrailsSpeed = 0.01f;

	Twink(){}

	template <typename T>
	T Read(uintptr_t Addr)
	{
		return Handler.Read<T>(Addr);
	}

    template <int index>
    inline uintptr_t PtrVirtual(uintptr_t this_)
    {
        return Read<uintptr_t>(this_) + index * 4;
    }

	template <typename T>
	void Write(T Value, uintptr_t Addr)
	{
		Handler.Write<T>(Value, Addr);
	}

    template <int index>
    inline uintptr_t Virtual(uintptr_t this_)
    {
        return Read<uintptr_t>(Read<uintptr_t>(this_) + index * 4);
    }

	uintptr_t GetTrackmania()
	{
		return Handler.Read<uintptr_t>(O_TRACKMANIA);
	}
	uintptr_t GetDX9Device()
	{
		return Handler.Read<uintptr_t>(O_DX9DEVICE);
	}

	void SetTrailColorRace(TM::GmVec3 TrailColor)
	{
        auto trackmania = GetTrackmania();

        auto race = Read<uintptr_t>(trackmania + 0x454);
        if (race)
        {
            uintptr_t player = 0;

            auto local_player_info = Read<uintptr_t>(race + 0x330);
            if (local_player_info)
            {
                player = Read<uintptr_t>(local_player_info + 0x238);
            }
            else
            {
                auto network = Read<uintptr_t>(trackmania + 0x12C);
                if (network)
                {
                    auto local_player_infos = Read<TM::CFastBuffer<uintptr_t>>(network + 0x2FC);
                    if (local_player_infos.Size >= 0)
                    {
                        player = Read<uintptr_t>(Read<uintptr_t>((unsigned long)local_player_infos.Ptr) + 0x238);
                    }
                }
            }

            if (player)
            {
                auto game_mobil = Read<uintptr_t>(player + 0x28);
                if (game_mobil)
                {
                    auto scene_vehicle_car = Read<uintptr_t>(game_mobil + 0x14);
                    if (scene_vehicle_car)
                    {
                        Write<TM::GmVec3>(TrailColor, scene_vehicle_car + 0xA8 + 0x20);
                    }
                }
            }
        }
	}

    TM::GmVec3 HsvToRgb(TM::GmVec3 input)
    {
        float hh, p, q, t, ff;
        long i;
        TM::GmVec3 output;

        if (input.y <= 0.0f) {
            output.x = input.z;
            output.y = input.z;
            output.z = input.z;
            return output;
        }

        hh = input.x;
        if (hh >= 360.0) hh = 0.0f;
        hh /= 60.0f;
        i = (long)hh;
        ff = hh - i;
        p = input.z * (1.0f - input.y);
        q = input.z * (1.0f - (input.y * ff));
        t = input.z * (1.0f - (input.y * (1.0f - ff)));

        switch (i) {
        case 0:
            output.x = input.z;
            output.y = t;
            output.z = p;
            break;
        case 1:
            output.x = q;
            output.y = input.z;
            output.z = p;
            break;
        case 2:
            output.x = p;
            output.y = input.z;
            output.z = t;
            break;
        case 3:
            output.x = p;
            output.y = q;
            output.z = input.z;
            break;
        case 4:
            output.x = t;
            output.y = p;
            output.z = input.z;
            break;
        case 5:
        default:
            output.x = input.z;
            output.y = p;
            output.z = q;
            break;
        }
        return output;
    }

    void Render()
    {
        using namespace ImGui;

        Begin("TwinkieTweaks");

        Checkbox("Trails", &EnableTrails);

        End();

        if (EnableTrails)
        {
            Begin("Trails Testing");
            
            static float Hue = 0;
            Hue += TrailsSpeed;
            if (Hue > 360)
            {
                Hue = 0;
            }
            TrailsColor = HsvToRgb(TM::GmVec3{ Hue, 1, 1 });
            SetTrailColorRace(TrailsColor);

            SliderFloat("Speed", &TrailsSpeed, 0.f, 1.f);
            
            End();
        }
    }
};