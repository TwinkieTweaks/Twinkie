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

	uintptr_t O_CTRACKMANIA = 0x972EB8;
    TM::GameType TMType = TM::GameType::Modded;

    TwinkTrackmania()
    {
		// 0x700b15 + ExeBaseAddr (0x400000) = 0xb00b15 (boobies)
		if (Read<unsigned char>(GetExeBaseAddr() + 0x700b15) == 0xA3u)
		{
			TMType = TM::GameType::Nations;
		}
		else if (Read<unsigned char>(GetExeBaseAddr() + 0x700b15) == 0xF7u)
		{
			TMType = TM::GameType::United;
		}
    }

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
        // Gets the base address of the .exe
        return (unsigned long)GetModuleHandle(NULL); // could be better
    }

    uintptr_t GetTrackmania()
    {
        return Read<uintptr_t>(GetExeBaseAddr() + O_CTRACKMANIA);
    }

    int GetMwClassId(uintptr_t This)
    {
        using GetClassId = int(__thiscall*)(uintptr_t);
        return reinterpret_cast<GetClassId>(Virtual<3>(This))(This);
    }

    uintptr_t GetMenuManager()
    {
        return Read<uintptr_t>(GetTrackmania() + 0x194);
    }

    uintptr_t GetInputPort()
    {
        return Read<uintptr_t>(GetTrackmania() + 0x6c);
    }

    uintptr_t GetProfileScores()
    {
        return Read<uintptr_t>(GetTrackmania() + 0x16c);
    }
    
    TM::CFastArray<uintptr_t> GetDevices()
    {
        return Read<TM::CFastArray<uintptr_t>>(GetInputPort() + 0x2c);
    }

    void ForceDevicePoll(uintptr_t Device, int MustNotPoll)
    {
        Write<int>(MustNotPoll, Device + 0x1c);
    }

    bool IsDeviceKeyboard(unsigned int ClassId)
    {
        return ClassId == 0x1300b000u;
    }

    bool IsDeviceMouse(unsigned int ClassId)
    {
        return ClassId == 0x1300a000u;
    }

    uintptr_t GetGameCamera()
    {
        return Read<uintptr_t>(GetTrackmania() + 0x174);
    }

    uintptr_t GetSceneCamera()
    {
        return Read<uintptr_t>(GetGameCamera() + 0x74);
    }

    uintptr_t GetHmsPocCamera()
    {
        return Read<uintptr_t>(GetSceneCamera() + 0x30);
    }

    bool IsHmsPocHmsCamera(uintptr_t HmsPoc)
    {
        return GetMwClassId(HmsPoc) == 0x6001000u;
    }

    void SetCameraZClip(bool Enable = false, float Distance = 50000.f, bool Void = false)
    {
        if (!GetGameCamera() or !IsPlaying()) return;
        uintptr_t Camera = GetHmsPocCamera();

        if (IsHmsPocHmsCamera(Camera))
        {
            Write<int>(Enable ? 1 : 0, Camera + 0x174); // ZClipEnable
            Write<float>(Distance, Camera + 0x178); // ZClipValue
            Write<float>(Void ? Distance * -10000.f : 200.f, Camera + 0x17c); // ZClipMargin
        }
    }

    void CallMenuGhostEditor()
    {
        using MenuGhostEditorFn = int(__thiscall*)(uintptr_t);

        uintptr_t GhostEditorPtr = TMType == TM::GameType::United ? GetExeBaseAddr() + 0xD93D0 : GetExeBaseAddr() + 0xD9780;
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

    void GetIdName(unsigned int Ident, TM::CFastString* String)
    {
        using GetIdNameFn = void* (__thiscall*)(unsigned int* Ident, TM::CFastString* String);
		reinterpret_cast<GetIdNameFn>(TMType == TM::GameType::United ? GetExeBaseAddr() + 0x335660 + 0x200000 : GetExeBaseAddr() + 0x3357D0 + 0x200000)(&Ident, String);
    }

    std::string GetChallengeUID()
    {
        if (GetChallenge())
        {
            TM::CFastString ChallengeUID;
            GetIdName(Read<unsigned int>(GetChallenge() + 220), &ChallengeUID);
            return ChallengeUID.Cstr;
        }
        return "Unassigned";
    }

    uintptr_t GetChallenge()
    {
        return Read<uintptr_t>(GetTrackmania() + 0x198);
    }

    int GetBestTime()
    {
        uintptr_t StructPtr = Read<uintptr_t>(CurPlayerInfo.PlayerInfo);
        if (StructPtr and IsPlaying())
        {
            if (ChallengeUsesScore())
                return Read<int>(StructPtr + 0x2E0);
            return Read<int>(StructPtr + 0x2B4); // i have no idea
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

    int GetResets()
    {
        return Read<int>(CurPlayerInfo.TrackmaniaRace + 0x340);
    }

    int GetRespawns()
    {
        uintptr_t Struct = Read<uintptr_t>(CurPlayerInfo.Player + 0x1C);
        if (Struct)
            return Read<int>(Struct + 0x2C4);
        return 0;
    }

    int GetGear()
    {
        return Read<int>(CurPlayerInfo.Vehicle + 1480);
    }

    bool GetWaterPhysicsApplied()
    {
        return Read<bool>(CurPlayerInfo.Vehicle + 1508);
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

    TM::RaceState GetState()
    {
        uintptr_t MoreInfoStruct = Read<uintptr_t>(CurPlayerInfo.Player + 0x1C);
        if (MoreInfoStruct)
        {
            return Read<TM::RaceState>(MoreInfoStruct + 0x314);
        }
        return TM::RaceState::BeforeStart;
    }

    bool IsPlaying()
    {
        return GetPlayerInfo().Vehicle and 
               GetPlayerInfo().TrackmaniaRace and 
               GetPlayerInfo().Mobil and 
               GetPlayerInfo().Player and 
               GetPlayerInfo().PlayerInfo;
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