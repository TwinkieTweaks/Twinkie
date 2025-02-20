#pragma once

#include <string>
#include <format>
#include <iomanip>
#include <sstream>
#include <vector>

#include "TrackMania.h"
#include "Version.h"
#include "SettingMgr.h"

#include "imgui-dx9/imgui.h"

#include "kiero/kiero.h"
#include "kiero/minhook/include/MinHook.h"

// #define BUILD_DEBUG
#define BUILD_UNITED
// #define BUILD_NATIONS
// #define BUILD_TMMC

struct ChallengeInfo
{
    unsigned int AuthorTime;
    unsigned int GoldTime;
    unsigned int SilverTime;
    unsigned int BronzeTime;
};

struct PlayerInfo
{
    uintptr_t Player;
    uintptr_t Mobil;
    uintptr_t Vehicle;
    uintptr_t PlayerInfo;
    uintptr_t TrackmaniaRace;
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

class Twink
{
public:
	// uintptr_t O_CTRACKMANIA_UNITED = 0xD6A2A4;
    // uintptr_t O_CTRACKMANIA_NATIONS = 0xD68C44;

#ifdef BUILD_UNITED
    uintptr_t O_CTRACKMANIA = 0x96A2A4;
    TM::GameType TMType = TM::GameType::United;
#endif
#ifdef BUILD_NATIONS
    uintptr_t O_CTRACKMANIA = 0x968C44;
    TM::GameType TMType = TM::GameType::Nations;
#endif

    Versioning Versions;

    bool DoRender = true;
#ifdef BUILD_DEBUG
    bool EnablePlayerInfo = false;
#endif
    PlayerInfo CurPlayerInfo = {};

    bool EnableLog = false;
    std::string LogStr = "";

    bool EnableDashboard = false;
    ImVec4 ColorSteer = ImVec4(0.976f, 0.737f, 0.008f, 1.f);
    ImVec4 ColorSteerI = ImVec4(1.f, 1.f, 1.f, 0.5f);
    ImVec4 ColorAccel = ImVec4(0.f, 0.94f, 0.f, 1.f);
    ImVec4 ColorAccelI = ImVec4(1.f, 1.f, 1.f, 0.5f);
    ImVec4 ColorBrake = ImVec4(0.94f, 0.f, 0.f, 1.f);
    ImVec4 ColorBrakeI = ImVec4(1.f, 1.f, 1.f, 0.5f);

    bool EnableAboutWindow = false;

    bool EnableSettings = false;

    bool EnableSplitSpeeds = false;

    bool EnableMedals = false;

    ImFont* KanitFont16 = nullptr;
    ImFont* KanitFont48 = nullptr;

    kiero::Status::Enum DX9HookStatus = kiero::Status::UnknownError;

    SettingMgr Settings;

    void SaveSettings()
    {
        Settings["Dashboard"]["Steer color"].Set(ColorSteer);
        Settings["Dashboard"]["Brake color"].Set(ColorBrake);
        Settings["Dashboard"]["Acceleration color"].Set(ColorAccel);

        Settings["Dashboard"]["Steer color (inactive)"].Set(ColorSteerI);
        Settings["Dashboard"]["Brake color (inactive)"].Set(ColorBrakeI);
        Settings["Dashboard"]["Acceleration color (inactive)"].Set(ColorAccelI);

        Settings["Dashboard"]["Enable"].Set(EnableDashboard);
        
        Settings["Medals"]["Enable"].Set(EnableMedals);

        Settings.Save();
    }

	Twink()
    {
        if (Settings.Status == 0)
        {
            ColorSteer = Settings["Dashboard"]["Steer color"].GetAsVec4();
            ColorBrake = Settings["Dashboard"]["Brake color"].GetAsVec4();
            ColorAccel = Settings["Dashboard"]["Acceleration color"].GetAsVec4();

            ColorSteerI = Settings["Dashboard"]["Steer color (inactive)"].GetAsVec4();
            ColorBrakeI = Settings["Dashboard"]["Brake color (inactive)"].GetAsVec4();
            ColorAccelI = Settings["Dashboard"]["Acceleration color (inactive)"].GetAsVec4();

            EnableDashboard = Settings["Dashboard"]["Enable"].GetAsBool();

            EnableMedals = Settings["Medals"]["Enable"].GetAsBool();
        }
    }

    ~Twink()
    {
        SaveSettings();
    }

    void InitFonts(ImGuiIO& ImIo)
    {
        // TODO
        /*KanitFont16 = ImIo.Fonts->AddFontFromFileTTF("Twinkie\\Fonts\\Kanit.ttf", 16.f);
        if (KanitFont16)
        {
            PrintInternal("Font \"Kanit16\" initialized.");
        }
        else
        {
            PrintError("Font \"Kanit16\" not initialized.");
        }

        KanitFont48 = ImIo.Fonts->AddFontFromFileTTF("Twinkie\\Fonts\\Kanit.ttf", 48.f);
        if (KanitFont48)
        {
            PrintInternal("Font \"Kanit48\" initialized.");
        }
        else
        {
            PrintError("Font \"Kanit48\" not initialized. SplitSpeeds extension is not available.");
        }*/
        return;
    }

    void Init()
    {
        using namespace TM;

        PrintInternal(":3c");
        PrintInternalArgs("Twinkie for TrackMania{} Forever. Version {}", TMType == TM::GameType::Nations ? " Nations" : (TMType == TM::GameType::United ? " United" : ""), Versions.TwinkieVer);
    
        if (GetTrackmania())
        {
            PrintInternal("Non-null CTrackMania.");
        }
        else
        {
            PrintError("Null CTrackMania.");
            PrintErrorArgs("Tried to poke: {}", GetExeBaseAddr() + O_CTRACKMANIA);
        }

        PrintInternalArgs("SettingMgr status: {}", Settings.Status);
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

    uintptr_t GetExeBaseAddr()
    {
        return (unsigned long)GetModuleHandle(NULL);
    }

	uintptr_t GetTrackmania()
	{
        return Read<uintptr_t>(GetExeBaseAddr() + O_CTRACKMANIA);
	}

    void PrintInternal(const char* Str)
    {
        LogStr = LogStr + "[TWINK] " + Str + "\n";
#ifdef TT_EXTERNAL_CONSOLE
        std::cout << "[TWINK] " << Str << "\n";
#endif
    }

    void PrintError(const char* Str)
    {
        LogStr = LogStr + "[ERR] " + Str + "\n";
#ifdef TT_EXTERNAL_CONSOLE
        std::cout << "[ERR] " << Str << "\n";
#endif
    }

    void Print(const char* Str)
    {
        LogStr = LogStr + "[LOG] " + Str + "\n";
#ifdef TT_EXTERNAL_CONSOLE
        std::cout << "[LOG] " << Str << "\n";
#endif
    }

    template<typename... Args>
    void PrintArgs(const char* Str, Args&&... args)
    {
        LogStr = LogStr + "[LOG] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef TT_EXTERNAL_CONSOLE
        std::cout << "[LOG] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#endif
    }

    template<typename... Args>
    void PrintErrorArgs(const char* Str, Args&&... args)
    {
        LogStr = LogStr + "[ERR] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef TT_EXTERNAL_CONSOLE
        std::cout << "[ERR] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#endif
    }

    template<typename... Args>
    void PrintInternalArgs(const char* Str, Args&&... args)
    {
        LogStr = LogStr + "[TWINK] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef TT_EXTERNAL_CONSOLE
        std::cout << "[TWINK] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#endif
    }

    void RenderLog()
    {
        using namespace ImGui;
        if (Begin("Logs", &EnableLog))
        {
            TextWrapped(LogStr.c_str());
        }
        End();
    }

    ChallengeInfo GetChallengeInfo()
    {
        ChallengeInfo InfoStruct{0,0,0,0};

        auto GameApp = GetTrackmania();
        auto Challenge = Read<uintptr_t>(GameApp + 0x198);

        if (Challenge)
        {
            auto ChallengeParameters = Read<uintptr_t>(Challenge + 0xb4);
            if (ChallengeParameters)
            {
                InfoStruct.AuthorTime = Read<unsigned int>(ChallengeParameters + 0x20);
                InfoStruct.GoldTime = Read<unsigned int>(ChallengeParameters + 0x1c);
                InfoStruct.SilverTime = Read<unsigned int>(ChallengeParameters + 0x18);
                InfoStruct.BronzeTime = Read<unsigned int>(ChallengeParameters + 0x14);
            }
        }

        return InfoStruct;
    }

    PlayerInfo GetPlayerInfo()
    {
        PlayerInfo InfoStruct{ 0,0,0,0,0 };

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
                        InfoStruct = {Player, Mobil, Vehicle, PlayerInfoNod, Race};
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

    VehicleInputs* GetInputInfoWrite()
    {
        return (VehicleInputs*)(CurPlayerInfo.Vehicle + 80);
    }

    uintptr_t GetChallenge()
    {
        return Read<uintptr_t>(GetTrackmania() + 0x198);
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

    int GetDisplaySpeed()
    {
        uintptr_t Struct = Read<uintptr_t>(CurPlayerInfo.PlayerInfo);
        return Read<int>(Struct + 0x340);
    }

    void SetupImGuiStyle()
    {
        // Soft Cherry style by Patitotective from ImThemes
        ImGuiStyle& style = ImGui::GetStyle();

        style.Alpha = 1.0f;
        style.DisabledAlpha = 0.4000000059604645f;
        style.WindowPadding = ImVec2(10.0f, 10.0f);
        style.WindowRounding = 4.0f;
        style.WindowBorderSize = 0.0f;
        style.WindowMinSize = ImVec2(50.0f, 50.0f);
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.WindowMenuButtonPosition = ImGuiDir_Left;
        style.ChildRounding = 0.0f;
        style.ChildBorderSize = 1.0f;
        style.PopupRounding = 1.0f;
        style.PopupBorderSize = 1.0f;
        style.FramePadding = ImVec2(5.0f, 3.0f);
        style.FrameRounding = 3.0f;
        style.FrameBorderSize = 0.0f;
        style.ItemSpacing = ImVec2(6.0f, 6.0f);
        style.ItemInnerSpacing = ImVec2(3.0f, 2.0f);
        style.CellPadding = ImVec2(3.0f, 3.0f);
        style.IndentSpacing = 6.0f;
        style.ColumnsMinSpacing = 6.0f;
        style.ScrollbarSize = 13.0f;
        style.ScrollbarRounding = 16.0f;
        style.GrabMinSize = 20.0f;
        style.GrabRounding = 4.0f;
        style.TabRounding = 4.0f;
        style.TabBorderSize = 1.0f;
        style.TabMinWidthForCloseButton = 0.0f;
        style.ColorButtonPosition = ImGuiDir_Right;
        style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
        style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

        style.Colors[ImGuiCol_Text] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 1.0f);
        style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5215686559677124f, 0.5490196347236633f, 0.5333333611488342f, 1.0f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 0.8f);
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1490196138620377f, 0.1568627506494522f, 0.1882352977991104f, 0.8f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
        style.Colors[ImGuiCol_Border] = ImVec4(0.1372549086809158f, 0.1137254908680916f, 0.1333333402872086f, 1.0f);
        style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.168627455830574f, 0.1843137294054031f, 0.2313725501298904f, 1.0f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
        style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2313725501298904f, 0.2000000029802322f, 0.2705882489681244f, 1.0f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.501960813999176f, 0.07450980693101883f, 0.2549019753932953f, 1.0f);
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
        style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.239215686917305f, 0.239215686917305f, 0.2196078449487686f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3725490272045135f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.6941176652908325f, 0.6941176652908325f, 0.686274528503418f, 1.0f);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.6941176652908325f, 0.6941176652908325f, 0.686274528503418f, 1.0f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.658823549747467f, 0.1372549086809158f, 0.1764705926179886f, 1.0f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.6509804129600525f, 0.1490196138620377f, 0.3450980484485626f, 1.0f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.7098039388656616f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
        style.Colors[ImGuiCol_Button] = ImVec4(0.6509804129600525f, 0.1490196138620377f, 0.3450980484485626f, 1.0f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
        style.Colors[ImGuiCol_Header] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.6509804129600525f, 0.1490196138620377f, 0.3450980484485626f, 1.0f);
        style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.501960813999176f, 0.07450980693101883f, 0.2549019753932953f, 1.0f);
        style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 1.0f);
        style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
        style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
        style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.6509804129600525f, 0.1490196138620377f, 0.3450980484485626f, 1.0f);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
        style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
        style.Colors[ImGuiCol_Tab] = ImVec4(0.1764705926179886f, 0.3490196168422699f, 0.5764706134796143f, 1.0f);
        style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.196078434586525f, 0.407843142747879f, 0.6784313917160034f, 1.0f);
        style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.1019607856869698f, 0.1450980454683304f, 1.0f);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 1.0f);
        style.Colors[ImGuiCol_PlotLines] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 1.0f);
        style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
        style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.3098039329051971f, 0.7764706015586853f, 0.196078434586525f, 1.0f);
        style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
        style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
        style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
        style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
        style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.3843137323856354f, 0.6274510025978088f, 0.9176470637321472f, 1.0f);
        style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
        style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 1.0f);
        style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.300000011920929f);
    }

    template <typename T>
    std::string ToHex(T Value) {
        // imma be honest ChatGPT wrote this shit
        // idk why but std::hex just doesn't like working with me
        // someone must've           HEXED ME LMAOAMOAMAOMOA,AK;AA;AA;;SM,A;OSMD;ASJKFOIUPOIERY9WRQOJGKSFJIKILLED17PEOPLEAIJDFNAOSGHAI
        static_assert(std::is_integral<T>::value, "T must be an integral type");

        std::stringstream oss;
        oss << std::hex << std::uppercase << Value;
        return "0x" + oss.str();
    }

    long GetCurrentCheckpoint()
    {
        // TODO: Fix last offset to support TMMC
        return Read<long>(GetExeBaseAddr() + (TMType == TM::GameType::United ? 0x957BFC : (TMType == TM::GameType::Nations ? 0x95659C : 0x957BFC)));
    }

    void SetCurrentCheckpoint(long Time)
    {
        // TODO: Fix last offset to support TMMC
        return Write<long>(Time, GetExeBaseAddr() + (TMType == TM::GameType::United ? 0x957BFC : (TMType == TM::GameType::Nations ? 0x95659C : 0x957BFC)));
    }

    //long GetCurrentLaps()
    //{
    //    return Read<long>(GetExeBaseAddr() + (TMType == TM::GameType::United ? 0x957BFC : (TMType == TM::GameType::Nations ? 0x9560EC : 0x957BFC)));
    //}

    bool IsPlaying()
    {
        return GetPlayerInfo().Vehicle and GetInputInfoWrite();
    }

    void RenderAboutWindow()
    {
        using namespace ImGui;

        if (Begin("About Twinkie", &EnableAboutWindow))
        {
            Text(std::format("Twinkie for TrackMania{} Forever. Version {}", TMType == TM::GameType::Nations ? " Nations" : (TMType == TM::GameType::United ? " United" : ""), Versions.TwinkieVer).c_str());
            Text("Made with love by jailman. <3");
        }
        End();
    }

    void DrawSpeedAndSplitText(ImDrawList* DrawList, std::string ValueText, std::string DiffText, ImVec4 Color)
    {
        using namespace ImGui;
        auto ScreenSize = ImGui::GetMainViewport()->Size;

        ImVec2 SizeVal = CalcTextSize(ValueText.c_str());
        ImVec2 SizeDiff = CalcTextSize(DiffText.c_str());

        DrawList->AddText(ImVec2((ScreenSize.x / 2.f) - (SizeVal.x / 2.f) + 3, ScreenSize.y / 10.f + 3), ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), ValueText.c_str());
        DrawList->AddText(ImVec2((ScreenSize.x / 2.f) - (SizeVal.x / 2.f), ScreenSize.y / 10.f), ColorConvertFloat4ToU32(Color), ValueText.c_str());

        DrawList->AddText(ImVec2((ScreenSize.x / 2.f) - (SizeDiff.x / 2.f) + 3, ScreenSize.y / 10.f + 3 + SizeVal.y), ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), DiffText.c_str());
        DrawList->AddText(ImVec2((ScreenSize.x / 2.f) - (SizeDiff.x / 2.f), ScreenSize.y / 10.f + SizeVal.y), ColorConvertFloat4ToU32(Color), DiffText.c_str());
    }

    void RenderSplitSpeeds()
    {
        static long LastCheckpoint = 0;
        static long CurrentCheckpoint = 0;
        static TM::RaceState CurrentState = TM::RaceState::Finished;
        static TM::RaceState LastState = TM::RaceState::Finished;
        static long LastRaceTime = 0;
        static long CurrentRaceTime = 0;

        static unsigned long CurrentCheckpointIdx = -1;

        static std::vector<long> Splits = {};
        static std::vector<long> BestSplits = {434, 444, 355};

        static bool DrawlistTesting = true;

        using namespace ImGui;

        if (IsPlaying())
        {
            if (Begin("SplitSpeeds", &EnableSplitSpeeds))
            {
                Text("Last checkpoint: %lu", GetCurrentCheckpoint());
                Checkbox("Test drawlist rendering", &DrawlistTesting);

                CurrentState = Read<TM::RaceState>(CurPlayerInfo.TrackmaniaRace + 0x50);
                std::string RaceStateText = CurrentState == TM::RaceState::Finished ? "Finished" : (CurrentState == TM::RaceState::Playing ? "Playing" : "Paused");

                Text(std::format("RaceState: {}", RaceStateText).c_str());

                CurrentRaceTime = GetRaceTime();
                if (CurrentRaceTime < LastRaceTime)
                {
                    CurrentCheckpoint = 0;
                    LastCheckpoint = 0;

                    CurrentCheckpointIdx = -1;

                    CurrentRaceTime = 0;
                    LastRaceTime = 0;

                    SetCurrentCheckpoint(0);
                    LastState = CurrentState;

                    Splits = {};

                    End();
                    return;
                }
                LastRaceTime = CurrentRaceTime;

                CurrentCheckpoint = GetCurrentCheckpoint();
                if (CurrentCheckpoint != LastCheckpoint)
                {
                    Splits.push_back(GetDisplaySpeed());
                    CurrentCheckpointIdx++;
                }
                LastCheckpoint = CurrentCheckpoint;
            }
            End();

            if (DrawlistTesting and CurrentCheckpointIdx != -1)
            {
                auto FGDrawList = ImGui::GetForegroundDrawList();
                auto ScreenSize = ImGui::GetWindowSize();

                long CurrentSplit = Splits[CurrentCheckpointIdx];

                long BestSplit = -1;
                bool IsFaster = false;
                bool IsNew = false;

                if (CurrentCheckpointIdx < BestSplits.size())
                {
                    BestSplit = BestSplits[CurrentCheckpointIdx];
                    IsFaster = CurrentSplit >= BestSplit;
                    IsNew = false;
                }
                else
                {
                    IsNew = true;
                }

                ImVec4 SplitTextCol = IsNew ? ImVec4(0, 0, 1, 1) : (IsFaster ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1));
                std::string ValueText = std::format("{}", CurrentSplit);
                long SplitDiff = CurrentSplit - BestSplit;
                std::string IFuckingHateC20 = !IsNew ? (SplitDiff < 0 ? "{}" : "+{}") : "";
                std::string DiffText = std::vformat(IFuckingHateC20, std::make_format_args(SplitDiff));

                if (KanitFont48) PushFont(KanitFont48);
                DrawSpeedAndSplitText(FGDrawList, ValueText, DiffText, SplitTextCol);
                if (KanitFont48) PopFont();
            }
        }
    }


#ifdef BUILD_DEBUG
    void RenderPlayerInfo()
    {
        using namespace ImGui;
        static bool ShowOffsetTesting = false;

        static uintptr_t OffsetAddr = 0;

        Begin("Player Information", &EnablePlayerInfo);

        if (CurPlayerInfo.Player)
        {
            SeparatorText("Addresses");

            Text("Address of GameApp: %x", GetTrackmania());
            SameLine();
            std::string GameAppAddrStr = ToHex(GetTrackmania());
            if (Button("Copy##GameApp"))
            {
                SetClipboardText(GameAppAddrStr.c_str());
                PrintArgs("Copied to clipboard: {}", GameAppAddrStr);
            }

            Text("Address of Player: %x", CurPlayerInfo.Player);
            SameLine();
            std::string PlayerAddrStr = ToHex(CurPlayerInfo.Player);
            if (Button("Copy##Player"))
            {
                SetClipboardText(PlayerAddrStr.c_str());
                PrintArgs("Copied to clipboard: {}", PlayerAddrStr);
            }

            Text("Address of Mobil: %x", CurPlayerInfo.Mobil);
            SameLine();
            std::string MobilAddrStr = ToHex(CurPlayerInfo.Mobil);
            if (Button("Copy##Mobil"))
            {
                SetClipboardText(MobilAddrStr.c_str());
                PrintArgs("Copied to clipboard: {}", MobilAddrStr);
            }

            Text("Address of Vehicle: %x", CurPlayerInfo.Vehicle);
            SameLine();
            std::string VehicleAddrStr = ToHex(CurPlayerInfo.Vehicle);
            if (Button("Copy##Vehicle"))
            {
                SetClipboardText(VehicleAddrStr.c_str());
                PrintArgs("Copied to clipboard: {}", VehicleAddrStr);
            }

            Text("Address of PlayerInfo: %x", CurPlayerInfo.PlayerInfo);
            SameLine();
            std::string PlayerInfoAddrStr = ToHex(CurPlayerInfo.PlayerInfo);
            if (Button("Copy##PlayerInfo"))
            {
                SetClipboardText(PlayerInfoAddrStr.c_str());
                PrintArgs("Copied to clipboard: {}", PlayerInfoAddrStr);
            }

            Text("Address of TrackmaniaRace: %x", CurPlayerInfo.TrackmaniaRace);
            SameLine();
            std::string TrackmaniaRaceAddrStr = ToHex(CurPlayerInfo.TrackmaniaRace);
            if (Button("Copy##TrackmaniaRace"))
            {
                SetClipboardText(TrackmaniaRaceAddrStr.c_str());
                PrintArgs("Copied to clipboard: {}", TrackmaniaRaceAddrStr);
            }

            SeparatorText("Race data");

            Text("Time: %lu", GetRaceTime());
            Text("Speed: %d", GetDisplaySpeed());

            VehicleInputs InputInfo = Read<VehicleInputs>(CurPlayerInfo.Vehicle + 80);

            Text("Steer: %f", InputInfo.Steer);
            Text("Gas: %f", InputInfo.fGas);
            Text("Brake: %f", InputInfo.fBrake);

            BeginDisabled();
            Checkbox("Mediatracker enabled", (bool*)CurPlayerInfo.Player + 56); // no Read here, ImGui reads the value internally
            auto MTClipIndex = Read<unsigned long>(CurPlayerInfo.Player + 72);
            if (*((bool*)CurPlayerInfo.Player + 56))
            {
                Text("Index of active mediatracker clip: %lu", Read<unsigned long>(CurPlayerInfo.Player + 72));
            }

            Checkbox("Free wheeling", (bool*)CurPlayerInfo.Vehicle + 1548);
            Checkbox("Turbo", (bool*)CurPlayerInfo.Vehicle + 948);
            EndDisabled();

            SeparatorText("Offset Testing");

            Checkbox("Show", &ShowOffsetTesting);

            if (ShowOffsetTesting)
            {
                /*BeginChild("Offsets");

                if (CurPlayerInfo.Player)
                {
                    InputInt("PlayerOffset", &OffsetPlayer, 2);
                    Text("Value: %x", Read<unsigned long>(CurPlayerInfo.Player + OffsetPlayer));
                    InputInt("PlayerWrite", &WritePlayer);
                    SameLine();
                    if (Button("Write##Player"))
                    {
                        Write<int>(WritePlayer, CurPlayerInfo.Player + OffsetPlayer);
                    }
                }

                Separator();

                if (CurPlayerInfo.Mobil)
                {
                    InputInt("MobilOffset", &OffsetMobil, 2);
                    Text("Value: %x", Read<unsigned long>(CurPlayerInfo.Mobil + OffsetMobil));
                    InputInt("MobilWrite", &WriteMobil);
                    SameLine();
                    if (Button("Write##Mobil"))
                    {
                        Write<int>(WriteMobil, CurPlayerInfo.Mobil + OffsetMobil);
                    }
                }

                Separator();

                if (CurPlayerInfo.Vehicle)
                {
                    InputInt("VehicleOffset", &OffsetVehicle, 2);
                    Text("Value: %x", Read<unsigned long>(CurPlayerInfo.Vehicle + OffsetVehicle));
                    InputInt("VehicleWrite", &WriteVehicle);
                    SameLine();
                    if (Button("Write##Vehicle"))
                    {
                        Write<int>(WriteVehicle, CurPlayerInfo.Vehicle + OffsetVehicle);
                    }
                }

                Separator();

                if (CurPlayerInfo.PlayerInfo)
                {
                    InputInt("PlayerInfoOffset", &OffsetPlayerInfo, 2);
                    Text("Value: %x", Read<unsigned long>(CurPlayerInfo.PlayerInfo + OffsetPlayerInfo));
                    InputInt("PlayerInfoWrite", &WritePlayerInfo);
                    SameLine();
                    if (Button("Write##PlayerInfo"))
                    {
                        Write<int>(WritePlayerInfo, CurPlayerInfo.PlayerInfo + OffsetPlayerInfo);
                    }
                }

                Separator();

                if (CurPlayerInfo.TrackmaniaRace)
                {
                    InputInt("TrackmaniaRaceOffset", &OffsetTrackmaniaRace, 2);
                    Text("Value: %x", Read<unsigned long>(CurPlayerInfo.TrackmaniaRace + OffsetTrackmaniaRace));
                    InputInt("TrackmaniaRaceWrite", &WriteTrackmaniaRace);
                    SameLine();
                    if (Button("Write##TrackmaniaRace"))
                    {
                        Write<int>(WriteTrackmaniaRace, CurPlayerInfo.TrackmaniaRace + OffsetTrackmaniaRace);
                    }
                }

                EndChild();*/

                InputInt("Address", reinterpret_cast<int*>(&OffsetAddr), 2);

                if (OffsetAddr)
                {
                    Text("Value: 0x%x, %lu", Read<unsigned long>(OffsetAddr), Read<unsigned long>(OffsetAddr));
                }
            }
        }
        else
        {
            Text("Not playing.");
        }

        End();
    }
#endif

    void RenderDashboard()
    {
        using namespace ImGui;
        if (IsPlaying())
        {
            static bool IsPrevHovered = false;
            VehicleInputs InputInfo = GetInputInfo();

            PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

            int DashboardWindowFlags = ImGuiWindowFlags_NoTitleBar;

            Begin("Dashboard", nullptr, DashboardWindowFlags);

            IsPrevHovered = IsWindowHovered();

            PopStyleColor();
            PopStyleVar();

            auto UIDrawList = GetWindowDrawList();
            auto CursorPos = GetCursorScreenPos();

            float WindowWidth = GetWindowWidth() / 3.f;
            float WindowHeight = GetWindowHeight();

            float Width = WindowWidth * -InputInfo.Steer;

            float OffsettedWidth = abs(WindowWidth - Width);

            auto TipSteer = ImVec2();

            if (InputInfo.Steer < 0)
                TipSteer = ImVec2(CursorPos.x + OffsettedWidth, CursorPos.y + WindowHeight / 2.f);
            else if (InputInfo.Steer > 0)
                TipSteer = ImVec2(CursorPos.x + WindowWidth * 2 - Width, CursorPos.y + WindowHeight / 2.f);
            auto TipBgL = ImVec2(CursorPos.x, CursorPos.y + WindowHeight / 2.f);
            auto TipBgR = ImVec2(CursorPos.x + GetWindowWidth(), CursorPos.y + WindowHeight / 2.f);

            auto UpperL = ImVec2(CursorPos.x + WindowWidth, CursorPos.y);
            auto LowerL = ImVec2(CursorPos.x + WindowWidth, CursorPos.y + WindowHeight);

            auto UpperR = ImVec2(CursorPos.x + WindowWidth * 2, CursorPos.y);
            auto LowerR = ImVec2(CursorPos.x + WindowWidth * 2, CursorPos.y + WindowHeight);

            auto BottomCornerGas = ImVec2(CursorPos.x + WindowWidth * 2, CursorPos.y + WindowHeight / 2.f);
            auto TopCornerBrake = ImVec2(CursorPos.x + WindowWidth, CursorPos.y + WindowHeight / 2.f);

            UIDrawList->AddTriangleFilled(UpperR, LowerR, TipBgR, ColorConvertFloat4ToU32(ColorSteerI)); // right one (no AA)
            UIDrawList->AddTriangleFilled(TipBgL, UpperL, LowerL, ColorConvertFloat4ToU32(ColorSteerI)); // left one (AA)
            if (InputInfo.Steer < 0)
                UIDrawList->AddTriangleFilled(TipSteer, UpperL, LowerL, ColorConvertFloat4ToU32(ColorSteer));
            else if (InputInfo.Steer > 0)
                UIDrawList->AddTriangleFilled(TipSteer, UpperR, LowerR, ColorConvertFloat4ToU32(ColorSteer));

            UIDrawList->AddRectFilled(ImVec2(UpperL.x + 6.f, UpperL.y), ImVec2(BottomCornerGas.x - 6.f, BottomCornerGas.y - 3.f), InputInfo.get_Gas() ? ColorConvertFloat4ToU32(ColorAccel) : ColorConvertFloat4ToU32(ColorAccelI));
            UIDrawList->AddRectFilled(ImVec2(TopCornerBrake.x + 6.f, TopCornerBrake.y + 3.f), ImVec2(LowerR.x - 6.f, LowerR.y), InputInfo.get_Brake() ? ColorConvertFloat4ToU32(ColorBrake) : ColorConvertFloat4ToU32(ColorBrakeI));

            End();
        }
    }

    void RenderSettings()
    {
        using namespace ImGui;
        if (Begin("Settings", &EnableSettings))
        {
            BeginTabBar("##SettingsTabBar");
            if (BeginTabItem("Dashboard"))
            {
                ColorEdit4("Steering", &ColorSteer.x);
                ColorEdit4("Acceleration", &ColorAccel.x);
                ColorEdit4("Brake", &ColorBrake.x);
                ColorEdit4("Steering (inactive)", &ColorSteerI.x);
                ColorEdit4("Acceleration (inactive)", &ColorAccelI.x);
                ColorEdit4("Brake (inactive)", &ColorBrakeI.x);

                EndTabItem();
            }
            EndTabBar();
        }
        End();
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

    void RenderMedals()
    {
        using namespace ImGui;
        if (GetChallenge())
        {
            ChallengeInfo InfoStruct = GetChallengeInfo();
            Begin("##Medals", &EnableMedals, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);

            Text(("Author: " + FormatTmDuration(InfoStruct.AuthorTime)).c_str());
            Text(("Gold: " + FormatTmDuration(InfoStruct.GoldTime)).c_str());
            Text(("Silver: " + FormatTmDuration(InfoStruct.SilverTime)).c_str());
            Text(("Bronze: " + FormatTmDuration(InfoStruct.BronzeTime)).c_str());

            End();
        }
    }

    // Render() only renders when the GUI is active (Twink.DoRender)
    void Render()
    {
        using namespace ImGui;
        CurPlayerInfo = GetPlayerInfo();

        if (KanitFont16) PushFont(KanitFont16);

        if (BeginMainMenuBar()) {
            if (BeginMenu("Twinkie")) {
                if (MenuItem("About", "", EnableAboutWindow))
                {
                    EnableAboutWindow = !EnableAboutWindow;
                }
                if (MenuItem("Settings", "", EnableSettings))
                {
                    EnableSettings = !EnableSettings;
                }
                Separator();
                if (MenuItem("Dashboard", "", EnableDashboard))
                {
                    EnableDashboard = !EnableDashboard;
                }
                Separator();
#ifdef BUILD_DEBUG
                if (MenuItem("SplitSpeeds", "", EnableSplitSpeeds))
                {
                    EnableSplitSpeeds = !EnableSplitSpeeds;
                }
#endif
                if (MenuItem("Medals", "", EnableMedals))
                {
                    EnableMedals = !EnableMedals;
                }
                ImGui::EndMenu();
            }
            if (BeginMenu("Debug"))
            {
                if (MenuItem("Logs", "", EnableLog))
                {
                    EnableLog = !EnableLog;
                }
#ifdef BUILD_DEBUG
                if (MenuItem("PlayerInformation", "", EnablePlayerInfo))
                {
                    EnablePlayerInfo = !EnablePlayerInfo;
                }
#endif
                ImGui::EndMenu();
            }
            EndMainMenuBar();
        }

#ifdef BUILD_DEBUG
        if (EnablePlayerInfo)
        {
            RenderPlayerInfo();
        }
#endif
        if (EnableLog)
        {
            RenderLog();
        }

        if (EnableAboutWindow)
        {
            RenderAboutWindow();
        }

        if (EnableSettings)
        {
            RenderSettings();
        }

#ifdef BUILD_DEBUG
        if (EnableSplitSpeeds)
        {
            RenderSplitSpeeds();
        }
#endif

        if (KanitFont16) PopFont();
    }

    // RenderAnyways() always gets called regardless of the current GUI state
    void RenderAnyways()
    {
        using namespace ImGui;
        CurPlayerInfo = GetPlayerInfo();

        if (EnableDashboard)
        {
            RenderDashboard();
        }

        if (EnableMedals)
        {
            RenderMedals();
        }
    }
};