#include "TwinkTrackmania.h"

bool VehicleInputs::Gas() const
{
    return fGas > 0.5f;
}

bool VehicleInputs::Brake() const
{
    return fBrake > 0.5f;
}

bool VehicleInputs::IsSteering() const
{
    return Steer != 0.f;
}

// CTOR
TwinkTrackmania::TwinkTrackmania()
{
    auto TMInterface = LoadLibraryA("TMInterface.dll");
    if (TMInterface)
    {
        TMInterfaceLoaded = true;
    }
}

// DMA
template <typename T>
T TwinkTrackmania::Read(uintptr_t Addr)
{
    return *reinterpret_cast<T*>(Addr);
}

template <typename T>
void TwinkTrackmania::Write(T Value, uintptr_t Addr)
{
    *reinterpret_cast<T*>(Addr) = Value;
}

template <int Idx>
uintptr_t TwinkTrackmania::Virtual(uintptr_t This)
{
    return Read<uintptr_t>(Read<uintptr_t>(This) + Idx * 4);
}

// GETTERS
uintptr_t TwinkTrackmania::GetExeBaseAddr()
{
    return (uintptr_t)GetModuleHandle(NULL);
}

uintptr_t TwinkTrackmania::GetTrackmania()
{
    return Read<uintptr_t>(GetExeBaseAddr() + 0x972EB8);
}

uintptr_t TwinkTrackmania::GetGbxApp()
{
    return Read<uintptr_t>(GetExeBaseAddr() + 0x966ff8);
}

TM::CFastStringInt TwinkTrackmania::GetChatText()
{
    return Read<TM::CFastStringInt>(GetExeBaseAddr() + 0x96C558);
}

int TwinkTrackmania::GetMwClassId(uintptr_t This)
{
    using GetClassId = int(__thiscall*)(uintptr_t);
    return reinterpret_cast<GetClassId>(Virtual<3>(This))(This);
}

std::string TwinkTrackmania::GetNameOfNod(uintptr_t Nod)
{
    auto StringPtr = VirtualParamGet<TM::CFastString>(Nod, CMwMemberInfo::STRING, 0x01001000);
    if (!StringPtr) return "";
    return StringPtr->Cstr;
}

int TwinkTrackmania::GetSignedRaceTime()
{
    if (!IsPlaying()) return -1;
    uintptr_t ActualPlayerInfo = Read<uintptr_t>(CurPlayerInfo.Player + 0x1C);

    return Read<int>(ActualPlayerInfo + 0x2B0);
}

float TwinkTrackmania::GetHmsCameraFov()
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

float TwinkTrackmania::GetHmsCameraAspectRatio()
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

bool TwinkTrackmania::IsInterfaceHidden()
{
    if (!IsPlaying()) return false;

    uintptr_t Nod = CurPlayerInfo.TrackmaniaRace;

    uintptr_t* InterfacePtr = VirtualParamGet<uintptr_t>(Nod, CMwMemberInfo::CLASS, 0x0300d000);
    if (!InterfacePtr) return false;

    return Read<int>((*InterfacePtr) + 32); // +16 also works
}

int TwinkTrackmania::GetCurCheckpointTime()
{
    if (!IsPlaying()) return -1;
    uintptr_t ActualPlayerInfo = Read<uintptr_t>(CurPlayerInfo.Player + 0x1C);

    auto NatPtr = VirtualParamGet<int>(ActualPlayerInfo, CMwMemberInfo::NATURAL, 0x24036017);
    if (!NatPtr) return -1;
    return *NatPtr;
}

int TwinkTrackmania::GetStuntsScore()
{
    if (!IsPlaying()) return -1;
    uintptr_t ActualPlayerInfo = Read<uintptr_t>(CurPlayerInfo.Player + 0x1C);

    return Read<int>(ActualPlayerInfo + 0x2D4);
}

uintptr_t TwinkTrackmania::GetMenuManager()
{
    return Read<uintptr_t>(GetTrackmania() + 0x194);
}

uintptr_t TwinkTrackmania::GetInputPort()
{
    return Read<uintptr_t>(GetTrackmania() + 0x6c);
}

uintptr_t TwinkTrackmania::GetVisionViewport()
{
    return Read<uintptr_t>(GetTrackmania() + 0x64);
}

int TwinkTrackmania::ViewportOverlayDisabled()
{
    return Read<int>(GetVisionViewport() + 0x7c);
}

IDirect3DDevice9* TwinkTrackmania::GetD3DDevice()
{
    return Read<IDirect3DDevice9*>(GetVisionViewport() + 0x9F8);
}

uintptr_t TwinkTrackmania::GetProfileScores()
{
    return Read<uintptr_t>(GetTrackmania() + 0x16c);
}

TM::CFastArray<uintptr_t> TwinkTrackmania::GetDevices()
{
    return Read<TM::CFastArray<uintptr_t>>(GetInputPort() + 0x2c);
}

uintptr_t TwinkTrackmania::GetProfile()
{
    return Read<uintptr_t>(GetTrackmania() + 0x168);
}

std::string TwinkTrackmania::GetChallengeDecorationName()
{
    uintptr_t Decoration = Read<uintptr_t>(GetChallenge() + 0x114);
    return GetNameOfNod(Decoration);
}

uintptr_t TwinkTrackmania::GetNetwork()
{
    return Read<uintptr_t>(GetTrackmania() + 0x12c);
}

TM::AccountType TwinkTrackmania::GetPayingAccountType()
{
    using GetPayingAccountTypeFn = TM::AccountType(__thiscall*)(uintptr_t);
    uintptr_t FnPtr = 0x19b350 + GetExeBaseAddr();

    return reinterpret_cast<GetPayingAccountTypeFn>(FnPtr)(GetTrackmania());
}

uintptr_t TwinkTrackmania::GetCoreCmdBuffer()
{
    return Read<uintptr_t>(GetExeBaseAddr() + 0x9731e0);
}

uintptr_t TwinkTrackmania::GetMainEngine()
{
    return Read<uintptr_t>(GetExeBaseAddr() + 0x973300);
}

uintptr_t TwinkTrackmania::GetGameCamera()
{
    return Read<uintptr_t>(GetTrackmania() + 0x174);
}

uintptr_t TwinkTrackmania::GetSceneCamera()
{
    return Read<uintptr_t>(GetGameCamera() + 0x74);
}

uintptr_t TwinkTrackmania::GetHmsPocCamera()
{
    return Read<uintptr_t>(GetSceneCamera() + 0x30);
}

CameraInfo TwinkTrackmania::GetCamInfo()
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

ChallengeInfo TwinkTrackmania::GetChallengeInfo()
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

PlayerInfo TwinkTrackmania::GetPlayerInfo()
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
    return InfoStruct;
}

uintptr_t TwinkTrackmania::GetSceneVehicleStruct()
{
    if (!IsPlaying()) return 0;
    return Read<uintptr_t>(CurPlayerInfo.Vehicle + 0x60);
}

TM::CFastBuffer<SSimulationWheel> TwinkTrackmania::GetVehicleWheels()
{
    return Read<TM::CFastBuffer<SSimulationWheel>>(CurPlayerInfo.Vehicle + 744);
}

bool TwinkTrackmania::GetVehicleWheelIsContactingGround(SSimulationWheel* Wheel)
{
    return Read<unsigned int>((uintptr_t)(Wheel)+292) == 1;
}

unsigned int TwinkTrackmania::GetVehicleWheelMatId(SSimulationWheel* Wheel)
{
    return Read<unsigned int>((uintptr_t)(Wheel)+576);
}

bool TwinkTrackmania::GetVehicleWheelIsSlipping(SSimulationWheel* Wheel)
{
    return Read<unsigned int>((uintptr_t)(Wheel)+300) == 1;
}

VehicleInputs TwinkTrackmania::GetInputInfo()
{
    return Read<VehicleInputs>(CurPlayerInfo.Vehicle + 80);
}

void TwinkTrackmania::GetIdName(unsigned int Ident, TM::CFastString* String)
{
    using GetIdNameFn = void* (__thiscall*)(unsigned int* Ident, TM::CFastString* String);
    reinterpret_cast<GetIdNameFn>(GetExeBaseAddr() + 0x3357D0 + 0x200000)(&Ident, String); // the +0x200000 is on purpose, might be modloader related
}

std::string TwinkTrackmania::GetChallengeUID()
{
    if (GetChallenge())
    {
        TM::CFastString ChallengeUID;
        GetIdName(Read<unsigned int>(GetChallenge() + 220), &ChallengeUID);
        return ChallengeUID.Cstr;
    }
    return "Unassigned";
}

uintptr_t TwinkTrackmania::GetChallenge()
{
    return Read<uintptr_t>(GetTrackmania() + 0x198);
}

std::string TwinkTrackmania::GetChallengeName()
{
    if (GetChallenge())
    {
        TM::CFastStringInt ChallengeName = Read<TM::CFastStringInt>(GetChallenge() + 0x108);
        return WStringToUTF8(ChallengeName.Cstr);
    }
    return "";
}

int TwinkTrackmania::GetBestTime()
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

long TwinkTrackmania::GetRaceTime()
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

float TwinkTrackmania::GetDisplaySpeed()
{
    return Read<float>(CurPlayerInfo.Vehicle + 760) * 3.6f;
}

float TwinkTrackmania::GetRpm()
{
    return Read<float>(CurPlayerInfo.Vehicle + 1464);
}

int TwinkTrackmania::GetResets()
{
    return Read<int>(CurPlayerInfo.TrackmaniaRace + 0x340);
}

int TwinkTrackmania::GetRespawns()
{
    uintptr_t Struct = Read<uintptr_t>(CurPlayerInfo.Player + 0x1C);
    if (Struct)
        return Read<int>(Struct + 0x2C4);
    return 0;
}

int TwinkTrackmania::GetGear()
{
    return Read<int>(CurPlayerInfo.Vehicle + 1480);
}

bool TwinkTrackmania::GetWaterPhysicsApplied()
{
    return Read<bool>(CurPlayerInfo.Vehicle + 1508);
}

int TwinkTrackmania::GetCheckpointCount()
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

TM::RaceState TwinkTrackmania::GetState()
{
    uintptr_t MoreInfoStruct = Read<uintptr_t>(CurPlayerInfo.Player + 0x1C);
    if (MoreInfoStruct)
    {
        return Read<TM::RaceState>(MoreInfoStruct + 0x314);
    }
    return TM::RaceState::BeforeStart;
}

bool TwinkTrackmania::IsOfficial()
{
    return Read<unsigned int>(GetTrackmania() + 0x268) != 0;
}

bool TwinkTrackmania::IsProfileUnited()
{
    return GetPayingAccountType() == TM::United;
}

bool TwinkTrackmania::IsGameInstallUnited()
{
    using IsPayingInstallFn = int(__thiscall*)(uintptr_t);
    uintptr_t FnPtr = 0x19b340 + GetExeBaseAddr();

    return reinterpret_cast<IsPayingInstallFn>(FnPtr)(GetTrackmania()) != 0;
}

bool TwinkTrackmania::IsDeviceKeyboard(unsigned int ClassId)
{
    return ClassId == 0x1300b000u;
}

bool TwinkTrackmania::IsDeviceMouse(unsigned int ClassId)
{
    return ClassId == 0x1300a000u;
}

bool TwinkTrackmania::IsOnline()
{
    // GetTrackmania() + 0x418 is ChallengeType
    return (Read<unsigned int>(GetTrackmania() + 0x418) & 16) == 16;
}

bool TwinkTrackmania::IsHmsPocHmsCamera(uintptr_t HmsPoc)
{
    return GetMwClassId(HmsPoc) == 0x6001000u;
}

bool TwinkTrackmania::IsPersonalBest()
{
    if (CurPlayerInfo.TrackmaniaRace)
    {
        // TODO: support stunts and platform mode here                 vvvvvvvvvvvvvvvvvvvvvvvvvvvvv
        return !ChallengeUsesScore() ? GetRaceTime() < GetBestTime() : GetRaceTime() > GetBestTime();
    }
    return false;
}

bool TwinkTrackmania::IsPaused()
{
    return Read<bool>(CurPlayerInfo.TrackmaniaRace + 0x48);
}

bool TwinkTrackmania::IsMwNodKindOf(uintptr_t Nod, unsigned int Id)
{
    using MwKindOfFn = int(__thiscall*)(uintptr_t, unsigned int);
    return reinterpret_cast<MwKindOfFn>(Virtual<4>(Nod))(Nod, Id) == 1;
}

bool TwinkTrackmania::IsChallengePlatform()
{
    return GetChallengeInfo().ChallengeType == 1;
}

bool TwinkTrackmania::IsPlaying()
{
    return CurPlayerInfo.Vehicle and
        CurPlayerInfo.TrackmaniaRace and
        CurPlayerInfo.Mobil and
        CurPlayerInfo.Player and
        CurPlayerInfo.PlayerInfo;
}

bool TwinkTrackmania::ChallengeUsesScore()
{
    // 5 -> Stunt
    // 1 -> Platform
    return GetChallengeInfo().ChallengeType == 1 or GetChallengeInfo().ChallengeType == 5;
}

uintptr_t TwinkTrackmania::LoadNodFromFilename(wchar_t* Filename, uintptr_t Drive)
{
    CFastStringInt String(Filename);
    CMwNod* ResultNod = nullptr;
    CSystemArchiveNod::LoadFileFrom(String, ResultNod, reinterpret_cast<CSystemFids*>(Drive), CSystemArchiveNod::SEVEN);
    return (uintptr_t)ResultNod;
}

std::string TwinkTrackmania::FormatTmDuration(unsigned int Duration)
{
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

std::string TwinkTrackmania::WStringToUTF8(const std::wstring& wstr)
{
    if (wstr.empty()) return {};

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &strTo[0], size_needed, nullptr, nullptr);
    return strTo;
}

std::wstring TwinkTrackmania::UTF8ToWString(const std::string& str)
{
    if (str.empty()) return {};

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

char* TwinkTrackmania::GetDriveName(uintptr_t Drive)
{
    using GetDriveNameFn = char* (__thiscall*)(uintptr_t SystemEngine, uintptr_t Drive);
    return reinterpret_cast<GetDriveNameFn>(0x1be70 + GetExeBaseAddr())(GetSystemEngine(), Drive);
}

uintptr_t TwinkTrackmania::GetDriveByIdx(size_t Idx)
{
    return Read<uintptr_t>(GetSystemEngine() + 0x24 + Idx * 4);
}

uintptr_t TwinkTrackmania::GetEngineByIdx(size_t Idx)
{
    TM::CFastBuffer<uintptr_t> Engines = Read<TM::CFastBuffer<uintptr_t>>(GetMainEngine() + 0x28);
    return Engines.Ptr[Idx];
}

uintptr_t TwinkTrackmania::GetSystemEngine()
{
    return GetEngineByIdx(2);
}

uintptr_t TwinkTrackmania::GetSceneEngine()
{
    return GetEngineByIdx(8);
}

uintptr_t TwinkTrackmania::GetDataDrive()
{
    CSystemEngine* SystemEngine = reinterpret_cast<CSystemEngine*>(GetSystemEngine());
    return (uintptr_t)SystemEngine->GetLocationData();
}

// SETTERS
void TwinkTrackmania::ForceDevicePoll(uintptr_t Device, int MustNotPoll)
{
    Write<int>(MustNotPoll, Device + 0x1c);
}

void TwinkTrackmania::SetCameraZClip(bool Enable, float Distance, bool Void)
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

void TwinkTrackmania::SetAnalogInfo(float Deadzone, float Sensitivity)
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

void TwinkTrackmania::SetEmitterNodRefs(uintptr_t Emitter, uintptr_t Nod)
{
    Write<uintptr_t>(Nod, Emitter + 0x18);
    Write<uintptr_t>(Nod, Emitter + 0x1c);
    Write<uintptr_t>(Nod, Emitter + 0x20);
}

void TwinkTrackmania::InitEmitter(uintptr_t Emitter, int ThatOneInt)
{
    Write<int>(4, Emitter + 20);
    Write<int>(ThatOneInt, Emitter + 52);
}

uintptr_t TwinkTrackmania::SetNodRef(uintptr_t NodRef, uintptr_t Nod)
{
    using SetNodRefFn = void(__thiscall*)(uintptr_t, uintptr_t);
    reinterpret_cast<SetNodRefFn>(0x7b520 + GetExeBaseAddr())(NodRef, Nod);
    return NodRef;
}

void TwinkTrackmania::SetString(TM::CFastStringInt* String, wchar_t** CString)
{
    using SetStringFn = void(__thiscall*)(TM::CFastStringInt*, wchar_t**);
    reinterpret_cast<SetStringFn>(0x503280 + GetExeBaseAddr())(String, CString);
}

void TwinkTrackmania::SetString(TM::CFastString* String, char** CString)
{
    using SetStringFn = void(__thiscall*)(TM::CFastString*, char**);
    reinterpret_cast<SetStringFn>(0x4fee60 + GetExeBaseAddr())(String, CString);
}

void TwinkTrackmania::SetStringLength(TM::CFastStringInt* String, size_t Length)
{
    using SetStringLengthFn = void(__thiscall*)(TM::CFastStringInt*, size_t);
    reinterpret_cast<SetStringLengthFn>(0x2e030 + GetExeBaseAddr())(String, Length);
}

void TwinkTrackmania::SetStringLength(TM::CFastString* String, size_t Length)
{
    using SetStringLengthFn = void(__thiscall*)(TM::CFastString*, size_t);
    reinterpret_cast<SetStringLengthFn>(0x1bff0 + GetExeBaseAddr())(String, Length);
}

void TwinkTrackmania::SetIdName(uintptr_t Nod, char* CString)
{
    using SetIdNameFn = void(__thiscall*)(uintptr_t, char*);
    reinterpret_cast<SetIdNameFn>(0x523f40 + GetExeBaseAddr())(Nod, CString);
}

// FUNCTIONS AND PATCHES
void TwinkTrackmania::CallMenuGhostEditor()
{
    using MenuGhostEditorFn = int(__thiscall*)(uintptr_t);

    uintptr_t GhostEditorPtr = GetExeBaseAddr() + 0xD9780;
    if (GetMenuManager())
    {
        reinterpret_cast<MenuGhostEditorFn>(GhostEditorPtr)(GetMenuManager());
    }
}

void TwinkTrackmania::CallSetOfficialRace()
{
    using SetOfficialRaceFn = void(__thiscall*)(uintptr_t);
    uintptr_t SetOfficialRacePtr = GetExeBaseAddr() + 0x7B5F0;
    if (CurPlayerInfo.TrackmaniaRace)
    {
        reinterpret_cast<SetOfficialRaceFn>(SetOfficialRacePtr)(CurPlayerInfo.TrackmaniaRace);
    }
}

void TwinkTrackmania::CallSaveChallengeFromMemory()
{
    using SaveChallengeFromMemoryFn = int(__thiscall*)(uintptr_t);
    reinterpret_cast<SaveChallengeFromMemoryFn>(GetExeBaseAddr() + 0x278300)(GetNetwork());
}

void TwinkTrackmania::CallGbxAppExit()
{
    using ExitFn = void(__thiscall*)(uintptr_t);
    reinterpret_cast<ExitFn>(Virtual<33>(GetGbxApp()))(GetGbxApp());
}

// Credit to greffmaster for helping me implement this function
bool TwinkTrackmania::VirtualParamGet(uintptr_t Nod, CMwStack* MwStack, void** Value)
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
T* TwinkTrackmania::VirtualParamGet(uintptr_t Nod, CMwMemberInfo::eType Type, unsigned int MemberId)
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
    VirtualParamGet(Nod, MwStack, (void**)&ReturnVal);
    delete[] MwStack->ppMemberInfos;
    delete MwStack;
    delete MemberInfo;
    return ReturnVal;
}

void TwinkTrackmania::SetFullscreenWindowedResolution(bool Enable, unsigned int Width, unsigned int Height)
{
    bool* ScreenShotForceRes = (bool*)(GetVisionViewport() + 0x13c);
    int* ScreenShotWidth = (int*)(GetVisionViewport() + 0x140);
    int* ScreenShotHeight = (int*)(GetVisionViewport() + 0x144);

    *ScreenShotForceRes = Enable;
    *ScreenShotWidth = Width;
    *ScreenShotHeight = Height;
}

uintptr_t TwinkTrackmania::SceneEngineCreateInstance(unsigned int ClassId)
{
    using CreateInstanceFn = void(__thiscall*)(uintptr_t SceneEngine, unsigned int ClassId, uintptr_t* ppNewNod, int UnusedParam);
    uintptr_t SceneEngine = GetSceneEngine();
    uintptr_t NewNod = 0;
    uintptr_t CreateInstancePtr = Virtual<33>(SceneEngine);
    CreateInstanceFn CreateInstance = reinterpret_cast<CreateInstanceFn>(CreateInstancePtr);
    CreateInstance(SceneEngine, ClassId, &NewNod, 0);
    return NewNod;
}

void TwinkTrackmania::UseEmittersInVehicleStruct(uintptr_t FirstEmitter, uintptr_t SecondEmitter, uintptr_t VehicleStruct)
{
    using BufferAddNewElemFn = uintptr_t(__thiscall*)(TM::CFastBuffer<uintptr_t>*);
    TM::CFastBuffer<uintptr_t>* Emitters = (TM::CFastBuffer<uintptr_t>*)(VehicleStruct + 0x38);
    BufferAddNewElemFn BufferAddNewElem = reinterpret_cast<BufferAddNewElemFn>(0x3f8470 + GetExeBaseAddr());
    SetNodRef(BufferAddNewElem(Emitters), FirstEmitter);
    SetNodRef(BufferAddNewElem(Emitters), SecondEmitter);
}

uintptr_t TwinkTrackmania::GetEditor()
{
    return Read<uintptr_t>(GetTrackmania() + 0x414);
}

bool TwinkTrackmania::IsInEditor()
{
    if (!GetChallenge()) return false;
    return Read<bool>(GetChallenge() + 0xD4);
}
bool TwinkTrackmania::IsInMediaTracker()
{
    if (!GetEditor()) return false;
    return IsMwNodKindOf(GetEditor(), 0x030b1000);
}