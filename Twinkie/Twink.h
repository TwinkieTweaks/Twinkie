#pragma once

#include <string>
#include <format>

#include "TrackMania.h"
#include "ProcHandler.h"

#include "imgui-dx9/imgui.h"
#include "imgui-dx9/imgui_impl_win32.h"
#include "imgui-dx9/imgui_impl_dx9.h"

struct PlayerInfo
{
    uintptr_t Player;
    uintptr_t Mobil;
    uintptr_t Vehicle;
};

class Twink
{
public:
	uintptr_t O_TRACKMANIA = 0xD6A2A4;
	uintptr_t O_DX9DEVICE = 0xD70C00;

	ProcHandler Handler;

    bool DoRender = true;

    bool EnableTrails = false;
    TM::GmVec3 TrailsColor = { 1,1,0 };
    float TrailsSpeed = 0.01f;

    bool EnablePlayerInfo = true;
    PlayerInfo CurPlayerInfo = {};

    bool EnableLog = true;
    std::string LogStr = "";

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

    void Print(const char* Str)
    {
        LogStr = LogStr + "[LOG] " + Str + "\n";
    }

    void RenderLog()
    {
        ImGui::TextWrapped(LogStr.c_str());
    }

    PlayerInfo GetPlayerInfo()
    {
        PlayerInfo ReturnVal{ 0,0,0 };

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
                        ReturnVal = {player, game_mobil, scene_vehicle_car};
                    }
                }
            }
        }
        return ReturnVal;
    }

    unsigned long GetRaceTime(PlayerInfo CurPlayerInfo)
    {
        return Read<unsigned long>(CurPlayerInfo.Player + 0x44) - 2600;
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
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 1.0f);
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

        static bool ShowThing = false;

        static int OffsetPlayer = 0x44;
        static int OffsetMobil = 0x44;
        static int OffsetVehicle = 0x44;

        Begin("TwinkieTweaks");

        Checkbox("Trails", &EnableTrails);
        Checkbox("Player Information", &EnablePlayerInfo);
        Checkbox("Logs", &EnableLog);

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

        if (EnablePlayerInfo)
        {
            Begin("Player Information");

            CurPlayerInfo = GetPlayerInfo();

            if (CurPlayerInfo.Player)
            {
                SeparatorText("Player Information");

                if (Button("Test logs"))
                {
                    Print("guh");
                }

                Text("Address of Player: %x", CurPlayerInfo.Player);
                SameLine();
                std::string PlayerAddrStr = std::format("{}\n", CurPlayerInfo.Player);
                if (Button("Copy##Player"))
                {
                    SetClipboardText(PlayerAddrStr.c_str());
                    Print("Copied to clipboard:");
                    Print(PlayerAddrStr.c_str());
                }

                Text("Address of Mobil: %x", CurPlayerInfo.Mobil);
                SameLine();
                std::string MobilAddrStr = std::format("{}\n", CurPlayerInfo.Mobil);
                if (Button("Copy##Mobil"))
                {
                    SetClipboardText(MobilAddrStr.c_str());
                    Print("Copied to clipboard:");
                    Print(MobilAddrStr.c_str());
                }

                Text("Address of Vehicle: %x", CurPlayerInfo.Vehicle);
                SameLine();
                std::string VehicleAddrStr = std::format("{}\n", CurPlayerInfo.Vehicle);
                if (Button("Copy##Vehicle"))
                {
                    SetClipboardText(VehicleAddrStr.c_str());
                    Print("Copied to clipboard:");
                    Print(VehicleAddrStr.c_str());
                }

                SeparatorText("Race data");

                if (CurPlayerInfo.Vehicle) Text("Time: %lu", GetRaceTime(CurPlayerInfo));

                SeparatorText("Offset Testing");

                Checkbox("Show", &ShowThing);

                if (ShowThing)
                {
                    if (CurPlayerInfo.Player)
                    {
                        InputInt("PlayerOffset", &OffsetPlayer, 2);
                        Text("Value: %x", Read<unsigned long>(CurPlayerInfo.Player + OffsetPlayer));
                    }

                    if (CurPlayerInfo.Mobil)
                    {
                        InputInt("MobilOffset", &OffsetMobil, 2);
                        Text("Value: %x", Read<unsigned long>(CurPlayerInfo.Mobil + OffsetMobil));
                    }

                    if (CurPlayerInfo.Vehicle)
                    {
                        InputInt("VehicleOffset", &OffsetVehicle, 2);
                        Text("Value: %x", Read<unsigned long>(CurPlayerInfo.Vehicle + OffsetVehicle));
                    }
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
};