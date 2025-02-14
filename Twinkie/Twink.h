#pragma once

#include <string>
#include <format>
#include <iomanip>
#include <sstream>

#include "TrackMania.h"
#include "Version.h"

#include "imgui-dx9/imgui.h"

#include "kiero/kiero.h"
#include "kiero/minhook/include/MinHook.h"

// #define TT_EXTERNAL_CONSOLE
#define BUILD_UNITED
// #define BUILD_NATIONS

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
    HMODULE LibraryHandle = NULL;

	// uintptr_t O_CTRACKMANIA_UNITED = 0xD6A2A4;
    // uintptr_t O_CTRACKMANIA_NATIONS = 0xD68C44;

#ifdef BUILD_UNITED
    uintptr_t O_CTRACKMANIA = 0xD6A2A4;
    TM::GameType TMType = TM::GameType::United;
#endif
#ifdef BUILD_NATIONS
    uintptr_t O_CTRACKMANIA = 0xD68C44;
    TM::GameType TMType = TM::GameType::Nations;
#endif

    Versioning Versions;

    bool DoRender = true;
    bool IsDivorced = false;
    bool WantDivorce = false;

    bool EnablePlayerInfo = true;
    PlayerInfo CurPlayerInfo = {};

    bool EnableLog = true;
    std::string LogStr = "";

    bool EnableDashboard = false;
    ImVec4 ColorSteer = ImVec4(0.976f, 0.737f, 0.008f, 1.f);
    ImVec4 ColorSteerI = ImVec4(1.f, 1.f, 1.f, 0.5f);
    ImVec4 ColorAccel = ImVec4(0.f, 0.94f, 0.f, 1.f);
    ImVec4 ColorAccelI = ImVec4(1.f, 1.f, 1.f, 0.5f);
    ImVec4 ColorBrake = ImVec4(0.94f, 0.f, 0.f, 1.f);
    ImVec4 ColorBrakeI = ImVec4(1.f, 1.f, 1.f, 0.5f);

    kiero::Status::Enum DX9HookStatus = kiero::Status::UnknownError;

	Twink(){}

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
            PrintErrorArgs("Tried to poke: {}", O_CTRACKMANIA);
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

	uintptr_t GetTrackmania()
	{
        return Read <uintptr_t>(O_CTRACKMANIA);
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

    void RenderLog() const
    {
        ImGui::TextWrapped(LogStr.c_str());
    }

    PlayerInfo GetPlayerInfo()
    {
        PlayerInfo ReturnVal{ 0,0,0,0,0 };

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
                        local_player_info = (unsigned long)local_player_infos.Ptr;
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
                        ReturnVal = {player, game_mobil, scene_vehicle_car, local_player_info, race};
                    }
                }
            }
        }
        return ReturnVal;
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

    long GetRaceTime(PlayerInfo CurPlayerInfo)
    {
        auto RaceTime = Read<unsigned long>(CurPlayerInfo.Player + 0x44);
        auto Offset = Read<unsigned long>(CurPlayerInfo.Vehicle + 0x5D0);
        if (RaceTime != 0 and Offset <= RaceTime)
        {
            return RaceTime - Offset; // 0x5D0 contains the racetime offset
        }
        return -1;
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
        style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1490196138620377f, 0.1568627506494522f, 0.1882352977991104f, 1.0f);
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
        static_assert(std::is_integral<T>::value, "T must be an integral type");

        std::stringstream oss;
        oss << std::hex << std::uppercase << Value; // Apply hex formatting first
        return "0x" + oss.str(); // Prepend "0x" separately
    }

    TM::GmVec3 HsvToRgb(TM::GmVec3 input)
    {
        float hh, p, q, t, ff;
        long i;
        TM::GmVec3 output{0.f, 0.f, 0.f};

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

    bool IsPlaying()
    {
        return GetPlayerInfo().Vehicle;
    }

    // Render() only renders when the GUI is active (Twink.DoRender)
    void Render()
    {
        using namespace ImGui;
        CurPlayerInfo = GetPlayerInfo();

        Begin("Twinkie");

        SeparatorText("Modules");

        Checkbox("Player Information", &EnablePlayerInfo);
        Checkbox("Logs", &EnableLog);
        Checkbox("Dashboard", &EnableDashboard);

        SeparatorText("Dashboard");

        ColorEdit4("Steer", &ColorSteer.x);
        ColorEdit4("Steer inactive", &ColorSteerI.x);
        ColorEdit4("Accel", &ColorAccel.x);
        ColorEdit4("Accel inactive", &ColorAccelI.x);
        ColorEdit4("Brake", &ColorBrake.x);
        ColorEdit4("Brake inactive", &ColorBrakeI.x);

        End();

        if (EnablePlayerInfo)
        {
            static bool ShowOffsetTesting = false;

            static int OffsetPlayer = 0;
            static int OffsetMobil = 0;
            static int OffsetVehicle = 0;
            static int OffsetPlayerInfo = 0;
            static int OffsetTrackmaniaRace = 0;

            static int WritePlayer = 0;
            static int WriteMobil = 0;
            static int WriteVehicle = 0;
            static int WritePlayerInfo = 0;
            static int WriteTrackmaniaRace = 0;

            Begin("Player Information");

            if (CurPlayerInfo.Player)
            {
                SeparatorText("Player Information");

                if (Button("Test logs"))
                {
                    Print("testing testing blabla");
                }

                Text("Address of Player: %x", CurPlayerInfo.Player);
                SameLine();
                std::string PlayerAddrStr = ToHex(CurPlayerInfo.Player);
                if (Button("Copy##Player"))
                {
                    SetClipboardText(PlayerAddrStr.c_str());
                    PrintArgs("Copied to clipboard: {}", PlayerAddrStr.c_str());
                }

                Text("Address of Mobil: %x", CurPlayerInfo.Mobil);
                SameLine();
                std::string MobilAddrStr = ToHex(CurPlayerInfo.Mobil);
                if (Button("Copy##Mobil"))
                {
                    SetClipboardText(MobilAddrStr.c_str());
                    PrintArgs("Copied to clipboard: {}", MobilAddrStr.c_str());
                }

                Text("Address of Vehicle: %x", CurPlayerInfo.Vehicle);
                SameLine();
                std::string VehicleAddrStr = ToHex(CurPlayerInfo.Vehicle);
                if (Button("Copy##Vehicle"))
                {
                    SetClipboardText(VehicleAddrStr.c_str());
                    PrintArgs("Copied to clipboard: {}", VehicleAddrStr.c_str());
                }

                Text("Address of PlayerInfo: %x", CurPlayerInfo.PlayerInfo);
                SameLine();
                std::string PlayerInfoAddrStr = ToHex(CurPlayerInfo.PlayerInfo);
                if (Button("Copy##PlayerInfo"))
                {
                    SetClipboardText(PlayerInfoAddrStr.c_str());
                    PrintArgs("Copied to clipboard: {}", PlayerInfoAddrStr.c_str());
                }

                SeparatorText("Race data");

                Text("Time: %lu", GetRaceTime(CurPlayerInfo));

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
                    BeginChild("Offsets");

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

                    EndChild();
                }
            }
            else
            {
                Text("Not playing.");
            }

            End();
        }

        if (EnableLog)
        {
            Begin("Logs");
            RenderLog();
            End();
        }
    }

    // RenderAnyways() always gets called regardless of the current GUI state

    void RenderAnyways()
    {
        using namespace ImGui;
        CurPlayerInfo = GetPlayerInfo();

        if (EnableDashboard)
        {
            if (IsPlaying() and GetInputInfoWrite())
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
                {
                    UIDrawList->AddTriangleFilled(TipSteer, UpperL, LowerL, ColorConvertFloat4ToU32(ColorSteer));
                    Print("render 1");
                }
                else if (InputInfo.Steer > 0)
                {
                    UIDrawList->AddTriangleFilled(TipSteer, UpperR, LowerR, ColorConvertFloat4ToU32(ColorSteer));
                    Print("render 2");
                }

                UIDrawList->AddRectFilled(ImVec2(UpperL.x + 6.f, UpperL.y), ImVec2(BottomCornerGas.x - 6.f, BottomCornerGas.y - 3.f), InputInfo.get_Gas() ? ColorConvertFloat4ToU32(ColorAccel) : ColorConvertFloat4ToU32(ColorAccelI));
                UIDrawList->AddRectFilled(ImVec2(TopCornerBrake.x + 6.f, TopCornerBrake.y + 3.f), ImVec2(LowerR.x - 6.f, LowerR.y), InputInfo.get_Brake() ? ColorConvertFloat4ToU32(ColorBrake) : ColorConvertFloat4ToU32(ColorBrakeI));

                End();
            }
        }
    }
};