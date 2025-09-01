#pragma once

#include <iostream>
#include "../TrackMania.h"
#include "../TmForeverLib/TmForever.h"

#include <d3d9.h>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
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

    bool Gas() const;
	bool Brake() const;
	bool IsSteering() const;
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

struct CMwParam 
{
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

    enum eFlags 
    {
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

class CMwStack 
{
public:
    pFun* vftable;
    int u2;
    int u3;
    int m_Size;
    CMwMemberInfo** ppMemberInfos;
    int* ppTypes;
    int iCurrentPos;
};

class CMwValueStd 
{
public:
    void* pValue = nullptr;
    void* pValue2 = nullptr;
};

class __declspec(dllexport) TwinkTrackmania
{
public:
    PlayerInfo CurPlayerInfo = {};

    bool TMInterfaceLoaded = false;

    const float MINRPM = 200.f;
    const float MAXRPM = 11000.f;

    // CTOR
    TwinkTrackmania();

    // DMA
    template <typename T>
    T Read(uintptr_t Addr);

    template <typename T>
    void Write(T Value, uintptr_t Addr);

    template <int Idx>
    uintptr_t Virtual(uintptr_t This);

    uintptr_t VirtualWrite(unsigned int Idx, uintptr_t This, uintptr_t ToWrite);

    // GETTERS
    uintptr_t GetExeBaseAddr();
    uintptr_t GetTrackmania();
    uintptr_t GetGbxApp();
    uintptr_t GetMenuManager();
    uintptr_t GetInputPort();
    uintptr_t GetVisionViewport();
    uintptr_t GetProfileScores();
    uintptr_t GetProfile();
    uintptr_t GetNetwork();
    uintptr_t GetCoreCmdBuffer();
    uintptr_t GetMainEngine();
    uintptr_t GetGameCamera();
    uintptr_t GetSceneCamera();
    uintptr_t GetHmsPocCamera();
    uintptr_t GetSceneVehicleStruct();
    uintptr_t GetChallenge();
    uintptr_t GetDriveByIdx(size_t Idx);
    uintptr_t GetEngineByIdx(size_t Idx);
    uintptr_t GetSystemEngine();
    uintptr_t GetSceneEngine();
    uintptr_t GetDataDrive();
    uintptr_t GetEditor();
    IDirect3DDevice9* GetD3DDevice();
    uintptr_t LoadNodFromFilename(wchar_t* Filename, uintptr_t Drive);
    uintptr_t SceneEngineCreateInstance(unsigned int ClassId);

    TM::CFastStringInt GetChatText();
    TM::CFastStringInt GetDocumentsFolderTM();
    std::string GetNameOfNod(uintptr_t Nod);
    std::string GetChallengeDecorationName();
    std::string GetChallengeUID();
    std::string GetChallengeName();
    std::string FormatTmDuration(unsigned int Duration);
    std::string WStringToUTF8(const std::wstring& wstr);
    std::wstring UTF8ToWString(const std::string& str);
    
    void GetIdName(unsigned int Ident, TM::CFastString* String);
    char* GetDriveName(uintptr_t Drive);
    unsigned int GetVehicleWheelMatId(SSimulationWheel* Wheel);
    int GetMwClassId(uintptr_t This);
    int GetSignedRaceTime();
    int GetCurCheckpointTime();
    int GetStuntsScore();
    int GetBestTime();
    int GetResets();
    int GetRespawns();
    int GetGear();
    int GetCheckpointCount();
    long GetRaceTime();
    float GetDisplaySpeed();
    float GetRpm();
    float GetHmsCameraFov();
    float GetHmsCameraAspectRatio();
    bool GetVehicleWheelIsSlipping(SSimulationWheel* Wheel);
    bool GetVehicleWheelIsContactingGround(SSimulationWheel* Wheel);
    bool GetWaterPhysicsApplied();
    bool IsInterfaceHidden();
    bool IsOfficial();
    bool IsProfileUnited();
    bool IsGameInstallUnited();
    bool IsDeviceKeyboard(unsigned int ClassId);
    bool IsDeviceMouse(unsigned int ClassId);
    bool IsOnline();
    bool IsHmsPocHmsCamera(uintptr_t HmsPoc);
    bool IsPersonalBest();
    bool IsPaused();
    bool IsMwNodKindOf(uintptr_t Nod, unsigned int Id);
    bool IsChallengePlatform();
    bool IsPlaying();
	bool IsInEditor();
	bool IsInMediaTracker();
    bool ChallengeUsesScore();
    int ViewportOverlayDisabled();

    TM::CFastArray<uintptr_t> GetDevices();
    TM::AccountType GetPayingAccountType();
    TM::CFastBuffer<SSimulationWheel> GetVehicleWheels();
    TM::RaceState GetState();

    CameraInfo GetCamInfo();
    ChallengeInfo GetChallengeInfo();
    PlayerInfo GetPlayerInfo();
    VehicleInputs GetInputInfo();

    // SETTERS
    uintptr_t SetNodRef(uintptr_t NodRef, uintptr_t Nod);

    void ForceDevicePoll(uintptr_t Device, int MustNotPoll);
    void InitEmitter(uintptr_t Emitter, int ThatOneInt);
    void SetString(TM::CFastStringInt* String, wchar_t** CString);
    void SetString(TM::CFastString* String, char** CString);
    void SetStringLength(TM::CFastStringInt* String, size_t Length);
    void SetStringLength(TM::CFastString* String, size_t Length);
    void SetIdName(uintptr_t Nod, char* CString);
    void SetCameraZClip(bool Enable = false, float Distance = 50000.f, bool Void = false);
    void SetAnalogInfo(float Deadzone = -1.f, float Sensitivity = -1.f);
    void SetEmitterNodRefs(uintptr_t Emitter, uintptr_t Nod);
    void SetFullscreenWindowedResolution(bool Enable, unsigned int Width, unsigned int Height);

    // FUNCTIONS AND PATCHES
    void CallMenuGhostEditor();
    void CallSetOfficialRace();
    void CallSaveChallengeFromMemory();
    void CallGbxAppExit();
    void UseEmittersInVehicleStruct(uintptr_t FirstEmitter, uintptr_t SecondEmitter, uintptr_t VehicleStruct);

    bool VirtualParamGet(uintptr_t Nod, CMwStack* MwStack, void** Value);
    template<typename T>
    T* VirtualParamGet(uintptr_t Nod, CMwMemberInfo::eType Type, unsigned int MemberId);
};