#pragma once

#include "TrackMania.h"
#include "ProcHandler.h"

class Twink
{
public:
	uintptr_t O_TRACKMANIA = 0xD6A2A4;
	uintptr_t O_DX9DEVICE = 0xD70C00;

	ProcHandler Handler;

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
};