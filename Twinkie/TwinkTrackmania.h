#pragma once

#include "TrackMania.h"

#include <d3d9.h>
#include <string>
#include <iomanip>
#include <sstream>
#include <Windows.h>

struct SSimulationWheel
{
    char _[0x2fc];
};

struct ChallengeInfo
{
    int AuthorScore;
    int AuthorTime;
    int GoldTime;
    int SilverTime;
    int BronzeTime;
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

struct CameraInfo
{
    TM::GmIso4 Position;
	float Fov;
    float NearZ;
	float FarZ;
    float AspectRatio;
	uintptr_t HmsPocCamera;
};

typedef int Integer;
typedef unsigned int Bool;
typedef unsigned char Nat8;
typedef unsigned short Nat16;
typedef unsigned int Natural, Nat32;
typedef unsigned __int64 Nat64;
typedef float Real, Real32;

typedef void(*pFun)(void*);

struct CMwParam {
    void* ptr;
};

struct CMwMemberInfo
{
    enum eType
    {
        ACTION = 0,
        BOOL = 1,
        BOOLARRAY = 2,
        BOOLBUFFER = 3,
        BOOLBUFFERCAT = 4,
        CLASS = 5,
        CLASSARRAY = 6,
        CLASSBUFFER = 7,
        CLASSBUFFERCAT = 8,
        COLOR = 9,
        COLORARRAY = 10,
        COLORBUFFER = 11,
        COLORBUFFERCAT = 12,
        ENUM = 13,

        INT = 14,
        INTARRAY = 15,
        INTBUFFER = 16,
        INTBUFFERCAT = 17,
        INTRANGE = 18,
        ISO4 = 19,
        ISO4ARRAY = 20,
        ISO4BUFFER = 21,
        ISO4BUFFERCAT = 22,
        ISO3 = 23,
        ISO3ARRAY = 24,
        ISO3BUFFER = 25,
        ISO3BUFFERCAT = 26,
        ID = 27,
        IDARRAY = 28,
        IDBUFFER = 29,
        IDBUFFERCAT = 30,
        NATURAL = 31,
        NATURALARRAY = 32,
        NATURALBUFFER = 33,
        NATURALBUFFERCAT = 34,
        NATURALRANGE = 35,
        REAL = 36,
        REALARRAY = 37,
        REALBUFFER = 38,
        REALBUFFERCAT = 39,
        REALRANGE = 40,
        STRING = 41,
        STRINGARRAY = 42,
        STRINGBUFFER = 43,
        STRINGBUFFERCAT = 44,
        STRINGINT = 45,
        STRINGINTARRAY = 46,
        STRINGINTBUFFER = 47,
        STRINGINTBUFFERCAT = 48,
        VEC2 = 49,
        VEC2ARRAY = 50,
        VEC2BUFFER = 51,
        VEC2BUFFERCAT = 52,
        VEC3 = 53,
        VEC3ARRAY = 54,
        VEC3BUFFER = 55,
        VEC3BUFFERCAT = 56,
        VEC4 = 57,
        VEC4ARRAY = 58,
        VEC4BUFFER = 59,
        VEC4BUFFERCAT = 60,
        INT3 = 61,
        INT3ARRAY = 62,
        INT3BUFFER = 63,
        INT3BUFFERCAT = 64,
        PROC = 65
    };

    enum eFlags {
        READ = 0b00000001,
        WRITE = 0b00000010,
        U1 = 0b00000100,
        U2 = 0b00001000,
        VIRTUAL_GET = 0b00010000,
        VIRTUAL_SET = 0b00100000,
        VIRTUAL_ADD = 0b01000000,
        VIRTUAL_SUB = 0b10000000,
    };

    eType type;
    int memberID;
    CMwParam* pParam;     
    int fieldOffset;
    const char* pszName;
    int flags;
    int flags2;
};

class CMwStack {
public:
    pFun* vftable;
    int u2;
    int u3;
    int m_Size;
    CMwMemberInfo** ppMemberInfos;
    int* ppTypes;
    int iCurrentPos;
};

class CMwValueStd {
public:
    void* pValue = nullptr;
    void* pValue2 = nullptr;
};

class TwinkTrackmania
{
public:
    PlayerInfo CurPlayerInfo = {};

    const float MINRPM = 200.f;
    const float MAXRPM = 11000.f;

	uintptr_t O_CTRACKMANIA = 0x972EB8;

    TwinkTrackmania() {}

    std::string WStringToUTF8(const std::wstring& wstr) {
        if (wstr.empty()) return {};

        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
        std::string strTo(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &strTo[0], size_needed, nullptr, nullptr);
        return strTo;
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

    // Credit to greffmaster for helping me implement this function
    bool VirtualParamGet(uintptr_t Nod, CMwStack* MwStack, void** Value)
    {
        CMwValueStd ValueStd;
        ValueStd.pValue = 0;
        ValueStd.pValue2 = 0;

        using VirtualParamGetFn = int(__thiscall*)(uintptr_t, CMwStack*, CMwValueStd*);
        bool ReturnVal = reinterpret_cast<VirtualParamGetFn>(Virtual<9>(Nod))(Nod, MwStack, &ValueStd) == 0;

        *Value = ValueStd.pValue;

        return ReturnVal;
    }

    // The MemberId is the most important thing to pass to this function
    // Credit to greffmaster for helping me implement this function
    template<typename T>
    T* VirtualParamGet(uintptr_t Nod, CMwMemberInfo::eType Type, unsigned int MemberId)
    {
        CMwMemberInfo* MemberInfo = new CMwMemberInfo();
        MemberInfo->type = Type;
        MemberInfo->fieldOffset = -1;
        MemberInfo->pszName = nullptr;
        MemberInfo->memberID = MemberId;
        MemberInfo->pParam = nullptr;
        MemberInfo->flags = -1;
        MemberInfo->flags2 = -1;

        CMwStack* MwStack = new CMwStack();
        MwStack->m_Size = 1;
        MwStack->ppMemberInfos = new CMwMemberInfo * [MwStack->m_Size] {MemberInfo};
        MwStack->iCurrentPos = 0;

        T* ReturnVal = nullptr;
        if (!VirtualParamGet(Nod, MwStack, (void**)&ReturnVal))
        {
            delete[] MwStack->ppMemberInfos;
            delete MwStack;
            delete MemberInfo;
            return ReturnVal;
        }
        delete[] MwStack->ppMemberInfos;
        delete MwStack;
        delete MemberInfo;
        return ReturnVal;
    }

    std::string GetNameOfNod(uintptr_t Nod)
    {
        auto StringPtr = VirtualParamGet<TM::CFastString>(Nod, CMwMemberInfo::STRING, 0x01001000);
        if (!StringPtr) return "";
        return StringPtr->Cstr;
    }

    int GetSignedRaceTime()
    {
        if (!IsPlaying()) return 0;
        uintptr_t Nod = CurPlayerInfo.TrackmaniaRace;

        auto NatPtr = VirtualParamGet<int>(Nod, CMwMemberInfo::NATURAL, 0x2401400d);
    }

    float GetHmsCameraFov()
    {
        if (!IsPlaying()) return 0;
		if (!GetGameCamera()) return 0;
        if (!GetHmsPocCamera()) return 0;
		if (!IsHmsPocHmsCamera(GetHmsPocCamera())) return 0;

        uintptr_t Nod = GetHmsPocCamera();

        auto FloatPtr = VirtualParamGet<float>(Nod, CMwMemberInfo::REAL, 0x6001014);
        if (!FloatPtr) return 0;
        return *FloatPtr;
    }

    float GetHmsCameraAspectRatio()
    {
        if (!IsPlaying()) return 0;
        if (!GetGameCamera()) return 0;
        if (!GetHmsPocCamera()) return 0;
        if (!IsHmsPocHmsCamera(GetHmsPocCamera())) return 0;

        uintptr_t Nod = GetHmsPocCamera();

        auto FloatPtr = VirtualParamGet<float>(Nod, CMwMemberInfo::REAL, 0x6001015);
        if (!FloatPtr) return 0;
        return *FloatPtr;
    }

    int GetCurCheckpointTime()
    {
        if (!IsPlaying()) return -1;
        uintptr_t ActualPlayerInfo = Read<uintptr_t>(CurPlayerInfo.Player + 0x1C);

		auto NatPtr = VirtualParamGet<int>(ActualPlayerInfo, CMwMemberInfo::NATURAL, 0x24036017);
		if (!NatPtr) return -1;
		return *NatPtr;
    }

    int GetStuntsScore()
    {
        if (!IsPlaying()) return -1;
        uintptr_t ActualPlayerInfo = Read<uintptr_t>(CurPlayerInfo.Player + 0x1C);

        return Read<int>(ActualPlayerInfo + 0x2D4);
    }

    uintptr_t GetMenuManager()
    {
        return Read<uintptr_t>(GetTrackmania() + 0x194);
    }

    uintptr_t GetInputPort()
    {
        return Read<uintptr_t>(GetTrackmania() + 0x6c);
    }

    uintptr_t GetVisionViewport()
    {
        return Read<uintptr_t>(GetTrackmania() + 0x64);
    }

	IDirect3DDevice9* GetD3DDevice()
	{
        return Read<IDirect3DDevice9*>(GetVisionViewport() + 0x9F8);
	}

    uintptr_t GetProfileScores()
    {
        return Read<uintptr_t>(GetTrackmania() + 0x16c);
    }
    
    TM::CFastArray<uintptr_t> GetDevices()
    {
        return Read<TM::CFastArray<uintptr_t>>(GetInputPort() + 0x2c);
    }

    uintptr_t GetProfile()
    {
        return Read<uintptr_t>(GetTrackmania() + 0x168);
    }

    bool IsOfficial()
    {
        return Read<unsigned int>(GetTrackmania() + 0x268) != 0;
    }

    std::string GetChallengeDecorationName()
    {
        uintptr_t Decoration = Read<uintptr_t>(GetChallenge() + 0x114);
        return GetNameOfNod(Decoration);
    }

    uintptr_t GetNetwork()
    {
        return Read<uintptr_t>(GetTrackmania() + 0x12c);
    }

    void CallSaveChallengeFromMemory()
    {
        using SaveChallengeFromMemoryFn = int(__thiscall*)(uintptr_t);
        reinterpret_cast<SaveChallengeFromMemoryFn>(GetExeBaseAddr() + 0x278300)(GetNetwork());
    }

    void CallGameAppExit()
    {
        using ExitFn = void(__thiscall*)(uintptr_t);
        reinterpret_cast<ExitFn>(GetExeBaseAddr() + 0x19b060)(GetTrackmania());
    }

    void PatchNicknameEntry()
    {
        std::vector<int> IndiciesToNicknameEntry = { 0, 0, 1, 1, 1 };

        auto MenuManagerMenus = Read<uintptr_t>(GetMenuManager() + 0x788);

        TM::CFastArray<uintptr_t>* Frames = (TM::CFastArray<uintptr_t>*)(MenuManagerMenus + 104);

        auto FrameProfile2 = Frames->Ptr[4];

        uintptr_t CurrentFrame = FrameProfile2;
        for (int Idx : IndiciesToNicknameEntry)
        {
            CurrentFrame = ((TM::CFastArray<uintptr_t>*)(CurrentFrame + 324))->Ptr[Idx];
        }
        uintptr_t NicknameEntry = CurrentFrame;

        *(int*)(NicknameEntry + 0x150) = 75;
    }

    bool IsProfileUnited()
    {
        return GetPayingAccountType() == TM::United;
    }

    TM::AccountType GetPayingAccountType()
    {
        using GetPayingAccountTypeFn = TM::AccountType(__thiscall*)(uintptr_t);
        uintptr_t FnPtr = 0x19b350 + GetExeBaseAddr();

        return reinterpret_cast<GetPayingAccountTypeFn>(FnPtr)(GetTrackmania());
    }

    bool IsGameInstallUnited()
    {
        using IsPayingInstallFn = int(__thiscall*)(uintptr_t);
        uintptr_t FnPtr = 0x19b340 + GetExeBaseAddr();

        return reinterpret_cast<IsPayingInstallFn>(FnPtr)(GetTrackmania()) != 0;
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

    void CallSetOfficialRace()
    {
		using SetOfficialRaceFn = void(__thiscall*)(uintptr_t);
		uintptr_t SetOfficialRacePtr = GetExeBaseAddr() + 0x7B5F0;
        if (GetPlayerInfo().TrackmaniaRace)
        {
			reinterpret_cast<SetOfficialRaceFn>(SetOfficialRacePtr)(GetPlayerInfo().TrackmaniaRace);
        }
    }

    bool IsOnline()
    {
        // GetTrackmania() + 0x418 is ChallengeType
		return (Read<unsigned int>(GetTrackmania() + 0x418) & 16) == 16;
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

	CameraInfo GetCamInfo()
	{
		CameraInfo CamInfo{};

		if (!GetGameCamera() or !IsPlaying()) return CamInfo;

		uintptr_t Camera = GetHmsPocCamera();
		CamInfo.HmsPocCamera = Camera;
		if (IsHmsPocHmsCamera(Camera))
		{
			CamInfo.Position = Read<TM::GmIso4>(Camera + 0x18); // CHmsZoneElem.Location
			CamInfo.Fov = GetHmsCameraFov(); // CHmsCamera.Fov (virtual)
			CamInfo.NearZ = Read<float>(Camera + 292); // CHmsPoc.NearZ (virtual but i found the offset)
			CamInfo.FarZ = Read<float>(Camera + 304); // CHmsPoc.FarZ (virtual but i found the offset)
			CamInfo.AspectRatio = GetHmsCameraAspectRatio(); // CHmsCamera.RatioXY (virtual)
		}

		return CamInfo;
	}

    void CallMenuGhostEditor()
    {
        using MenuGhostEditorFn = int(__thiscall*)(uintptr_t);

        uintptr_t GhostEditorPtr = GetExeBaseAddr() + 0xD9780;
        if (GetMenuManager())
        {
            reinterpret_cast<MenuGhostEditorFn>(GhostEditorPtr)(GetMenuManager());
        }
    }

    void SetAnalogInfo(float Deadzone = -1.f, float Sensitivity = -1.f)
    {
        using DialogInputOnQuitFn = int(__thiscall*)(uintptr_t);

        uintptr_t OnQuitPtr = GetExeBaseAddr() + 0x22A550;
        if (GetMenuManager())
        {
            if (Deadzone >= 0) Write(Sensitivity, GetMenuManager() + 0x114);
            if (Sensitivity >= 0) Write(Deadzone, GetMenuManager() + 0x118);
            reinterpret_cast<DialogInputOnQuitFn>(OnQuitPtr)(GetMenuManager());
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
        // 5 -> Stunt
		// 1 -> Platform
        return GetChallengeInfo().ChallengeType == 1 or GetChallengeInfo().ChallengeType == 5;
    }

    bool IsChallengePlatform()
    {
        return GetChallengeInfo().ChallengeType == 1;
    }

    PlayerInfo GetPlayerInfo()
    {
        // code graciously yoinked from brokenphilip
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
        CurPlayerInfo = InfoStruct;
        return InfoStruct;
    }

    TM::CFastBuffer<SSimulationWheel> GetVehicleWheels()
    {
        return Read<TM::CFastBuffer<SSimulationWheel>>(CurPlayerInfo.Vehicle + 744);
    }

    bool GetVehicleWheelIsContactingGround(SSimulationWheel* Wheel)
    {
        return Read<unsigned int>((uintptr_t)(Wheel) + 292) == 1;
    }

    unsigned int GetVehicleWheelMatId(SSimulationWheel* Wheel)
    {
        return Read<unsigned int>((uintptr_t)(Wheel) + 576);
    }

    bool GetVehicleWheelIsSlipping(SSimulationWheel* Wheel)
    {
        return Read<unsigned int>((uintptr_t)(Wheel) + 300) == 1;
    }

    VehicleInputs GetInputInfo()
    {
        CurPlayerInfo = GetPlayerInfo();
        return Read<VehicleInputs>(CurPlayerInfo.Vehicle + 80);
    }

    void GetIdName(unsigned int Ident, TM::CFastString* String)
    {
        using GetIdNameFn = void* (__thiscall*)(unsigned int* Ident, TM::CFastString* String);
		reinterpret_cast<GetIdNameFn>(GetExeBaseAddr() + 0x3357D0 + 0x200000)(&Ident, String); // the +0x200000 is on purpose, might be modloader related
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

    std::string GetChallengeName()
    {
        if (GetChallenge())
        {
            TM::CFastStringInt ChallengeName = Read<TM::CFastStringInt>(GetChallenge() + 0x108);
            return WStringToUTF8(ChallengeName.Cstr);
        }
        return "";
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
        return 0;
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

    bool IsPaused()
    {
        return Read<bool>(CurPlayerInfo.TrackmaniaRace + 0x48);
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
        if (Duration == MAXDWORD) return "--:--:--";
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