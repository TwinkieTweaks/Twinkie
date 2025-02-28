#pragma once

#include "TrackMania.h"

#include <string>
#include <iomanip>
#include <sstream>
#include <Windows.h>

struct ChallengeInfo
{
    unsigned int AuthorScore;
    unsigned int AuthorTime;
    unsigned int GoldTime;
    unsigned int SilverTime;
    unsigned int BronzeTime;
    int ChallengeType;
    unsigned int CheckpointCount;
    uintptr_t Challenge;
    uintptr_t ChallengeParams;
};

struct PlayerInfo
{
    uintptr_t Player;
    uintptr_t Mobil;
    uintptr_t Vehicle;
    uintptr_t PlayerInfo;
    uintptr_t TrackmaniaRace;
    bool IsNetworked;
};

struct VehicleInputs
{
    float fGas;
    float fBrake;
    float Steer;

    bool get_Gas() const
    {
        return fGas > 0.5f;
    }

    bool get_Brake() const
    {
        return fBrake > 0.5f;
    }

    bool IsSteering() const
    {
        return Steer != 0.f;
    }
};

class TwinkTrackmania
{
public:
    PlayerInfo CurPlayerInfo = {};

    const float MINRPM = 200.f;
    const float MAXRPM = 11000.f;

#ifdef BUILD_UNITED
    uintptr_t O_CTRACKMANIA = 0x96A2A4;
    TM::GameType TMType = TM::GameType::United;
#endif
#ifdef BUILD_NATIONS
    uintptr_t O_CTRACKMANIA = 0x968C44;
    TM::GameType TMType = TM::GameType::Nations;
#endif

    TwinkTrackmania(){}

    template <typename T>
    T Read(uintptr_t Addr)
    {
        return *reinterpret_cast<T*>(Addr);
    }

    template <typename T>
    void Write(T Value, uintptr_t Addr)
    {
        *reinterpret_cast<T*>(Addr) = Value;
    }

    template <int Idx>
    uintptr_t Virtual(uintptr_t This)
    {
        return Read<uintptr_t>(Read<uintptr_t>(This) + Idx * 4);
    }

    uintptr_t GetExeBaseAddr()
    {
        return (unsigned long)GetModuleHandle(NULL);
    }

    uintptr_t GetTrackmania()
    {
        return Read<uintptr_t>(GetExeBaseAddr() + O_CTRACKMANIA);
    }

    uintptr_t GetMenuManager()
    {
        return Read<uintptr_t>(GetTrackmania() + 0x194);
    }

    void CallMenuGhostEditor()
    {
        using MenuGhostEditorFn = int(__thiscall*)(uintptr_t);

        uintptr_t GhostEditorPtr = TMType == TM::GameType::United ? 0x4D93D0 : 0x4D9780;
        if (GetMenuManager())
        {
            reinterpret_cast<MenuGhostEditorFn>(GhostEditorPtr)(GetMenuManager());
        }
    }

    ChallengeInfo GetChallengeInfo()
    {
        ChallengeInfo InfoStruct{ 0,0,0,0,0,0 };

        auto GameApp = GetTrackmania();
        auto Challenge = Read<uintptr_t>(GameApp + 0x198);

        if (Challenge)
        {
            auto ChallengeParameters = Read<uintptr_t>(Challenge + 0xb4);
            if (ChallengeParameters)
            {
                InfoStruct.AuthorScore = Read<unsigned int>(ChallengeParameters + 0x28);
                InfoStruct.AuthorTime = Read<unsigned int>(ChallengeParameters + 0x20);
                InfoStruct.GoldTime = Read<unsigned int>(ChallengeParameters + 0x1c);
                InfoStruct.SilverTime = Read<unsigned int>(ChallengeParameters + 0x18);
                InfoStruct.BronzeTime = Read<unsigned int>(ChallengeParameters + 0x14);

                InfoStruct.ChallengeType = Read<int>(Challenge + 0xf8);

                InfoStruct.CheckpointCount = Read<TM::CFastBuffer<TM::GmNat3>>(Challenge + 0x60).Size;

                InfoStruct.Challenge = Challenge;
                InfoStruct.ChallengeParams = ChallengeParameters;
            }
        }

        return InfoStruct;
    }

    bool ChallengeUsesScore()
    {
        return GetChallengeInfo().ChallengeType == 1 or GetChallengeInfo().ChallengeType == 5;
    }

    PlayerInfo GetPlayerInfo()
    {
        // code graciously provided by brokenphilip
        PlayerInfo InfoStruct{ 0,0,0,0,0,false };

        auto GameApp = GetTrackmania();

        auto Race = Read<uintptr_t>(GameApp + 0x454);
        if (Race)
        {
            uintptr_t Player = 0;

            auto PlayerInfoNod = Read<uintptr_t>(Race + 0x330);
            if (PlayerInfoNod)
            {
                Player = Read<uintptr_t>(PlayerInfoNod + 0x238);
            }
            else
            {
                InfoStruct.IsNetworked = true;
                auto Network = Read<uintptr_t>(GameApp + 0x12C);
                if (Network)
                {
                    auto PlayerInfoBuffer = Read<TM::CFastBuffer<uintptr_t>>(Network + 0x2FC);
                    if (PlayerInfoBuffer.Size >= 0)
                    {
                        PlayerInfoNod = (unsigned long)PlayerInfoBuffer.Ptr;
                        Player = Read<uintptr_t>(Read<uintptr_t>((unsigned long)PlayerInfoBuffer.Ptr) + 0x238);
                    }
                }
            }

            if (Player)
            {
                auto Mobil = Read<uintptr_t>(Player + 0x28);
                if (Mobil)
                {
                    auto Vehicle = Read<uintptr_t>(Mobil + 0x14);
                    if (Vehicle)
                    {
                        InfoStruct = { Player, Mobil, Vehicle, PlayerInfoNod, Race };
                    }
                }
            }
        }
        return InfoStruct;
    }

    VehicleInputs GetInputInfo()
    {
        CurPlayerInfo = GetPlayerInfo();
        return Read<VehicleInputs>(CurPlayerInfo.Vehicle + 80);
    }

    uintptr_t GetChallenge()
    {
        return Read<uintptr_t>(GetTrackmania() + 0x198);
    }

    int GetBestTime()
    {
        using GetBestTimeOrScoreFn = int(__thiscall*)(uintptr_t);
        if (CurPlayerInfo.TrackmaniaRace)
        {
            return reinterpret_cast<GetBestTimeOrScoreFn>(Virtual<22>(CurPlayerInfo.TrackmaniaRace))(CurPlayerInfo.TrackmaniaRace);
        }
        return -1;
    }

    bool IsPersonalBest()
    {
        if (CurPlayerInfo.TrackmaniaRace)
        {
            // TODO: support stunts and platform mode here                 vvvvvvvvvvvvvvvvvvvvvvvvvvvvv
            return !ChallengeUsesScore() ? GetRaceTime() < GetBestTime() : GetRaceTime() > GetBestTime();
        }
        return false;
    }

    long GetRaceTime()
    {
        auto RaceTime = Read<unsigned long>(CurPlayerInfo.Player + 0x44);
        auto Offset = Read<unsigned long>(CurPlayerInfo.Vehicle + 0x5D0);
        if (RaceTime != 0 and Offset <= RaceTime)
        {
            return RaceTime - Offset;
        }

        // technically, the racetime is unsigned
        // but unless someone takes ~24 days to finish a run
        // this shouldn't be an issue
        return -1;
    }

    float GetDisplaySpeed()
    {
        return Read<float>(CurPlayerInfo.Vehicle + 760) * 3.6f;
    }

    float GetRpm()
    {
        return Read<float>(CurPlayerInfo.Vehicle + 1464);
    }

    int GetGear()
    {
        return Read<int>(CurPlayerInfo.Vehicle + 1480);
    }

    bool GetWaterPhysicsApplied()
    {
        return Read<bool>(CurPlayerInfo.Vehicle + 1508);
    }

    long GetCurrentCheckpoint()
    {
        return Read<long>(GetExeBaseAddr() + (TMType == TM::GameType::United ? 0x957BFC : (TMType == TM::GameType::Nations ? 0x95659C : 0x957BFC)));
    }

    void SetCurrentCheckpoint(long Time)
    {
        return Write<long>(Time, GetExeBaseAddr() + (TMType == TM::GameType::United ? 0x957BFC : (TMType == TM::GameType::Nations ? 0x95659C : 0x957BFC)));
    }

    int GetCheckpointCount()
    {
        if (CurPlayerInfo.Player)
        {
            // i have no idea what type is this struct
            uintptr_t MoreInfoStruct = Read<uintptr_t>(CurPlayerInfo.Player + 0x1C);
            if (MoreInfoStruct)
            {
                return Read<int>(MoreInfoStruct + 0x330);
            }
        }
        return 0;
    }

    bool IsPlaying()
    {
        return GetPlayerInfo().Vehicle and GetPlayerInfo().TrackmaniaRace;
    }

    std::string FormatTmDuration(unsigned int Duration) {
        unsigned int TotalSeconds = Duration / 1000;
        unsigned int Millis = (Duration % 1000) / 10;
        unsigned int Seconds = TotalSeconds % 60;
        unsigned int Minutes = (TotalSeconds / 60) % 60;
        unsigned int Hours = TotalSeconds / 3600;

        std::ostringstream StringStream;
        StringStream << std::setfill('0');

        if (Hours > 0) {
            StringStream << std::setw(2) << Hours << ":";
        }

        StringStream << std::setw(2) << Minutes << ":"
            << std::setw(2) << Seconds << "."
            << std::setw(2) << Millis;

        return StringStream.str();
    }
};