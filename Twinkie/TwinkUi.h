#pragma once

#include <string>
#include <format>
#include <iomanip>
#include <sstream>
#include <vector>

#include "TwinkTrackmania.h"
#include "Version.h"
#include "SettingMgr.h"

#include "imgui-dx9/imgui.h"

#include "Modules/CheckpointCounter.h"
#include "Modules/About.h"
#include "Modules/DashboardInputs.h"
#include "Modules/DashboardTacho.h"
#include "Modules/Medals.h"
#include "Modules/PlayerInfo.h"
#include "Modules/GhostEditor.h"

#include <d3d9.h>

#include "kiero/kiero.h"
#include "kiero/minhook/include/MinHook.h"

typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
using ResetFn = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
using PresentFn = long(__stdcall*)(LPDIRECT3DDEVICE9 pDevice, LPVOID, LPVOID, HWND, LPVOID);

class TwinkUi
{
public:
    Versioning Versions;
    TwinkTrackmania TrackmaniaMgr;

    bool DoRender = true;

    ResetFn oReset = NULL;
    PresentFn oPresent = NULL;
    WNDPROC oWndProc = NULL;
    HWND Window = NULL;

    bool EnableSettings = false;

    std::vector<IModule*> Modules = {};

    bool EnableLog = false;
    std::string LogStr = "";

    kiero::Status::Enum DX9HookStatus = kiero::Status::UnknownError;

    SettingMgr Settings;

    void SettingsInit()
    {
        for (IModule* Module : Modules)
        {
            Module->SettingsInit(Settings);
        }
    }

    void SettingsSave()
    {
        for (IModule* Module : Modules)
        {
            Module->SettingsSave(Settings);
        }
    }

    TwinkUi()
    {
        SettingsInit();

        Modules.push_back(new AboutModule());
        //
        Modules.push_back(new DashboardInputsModule());
        Modules.push_back(new DashboardTachometerModule());
        //
        Modules.push_back(new CheckpointCounterModule());
        //
        Modules.push_back(new MedalsModule());
        //
        Modules.push_back(new GhostEditorModule());
        //
#ifdef BUILD_DEBUG
        Modules.push_back(new PlayerInfoModule());
#endif
    }

    ~TwinkUi()
    {
        for (IModule* Module : Modules)
        {
            delete Module;
            Module = nullptr;
        }
    }

    void InitFonts(ImGuiIO& ImIo)
    {
        /*
        KanitFont16 = ImIo.Fonts->AddFontFromFileTTF((GetDocumentsFolder() + "\\Twinkie\\Fonts\\Kanit.ttf").c_str(), 16.f);
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
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[ERR] " << Str << "\n";
#endif
    }

    void Print(const char* Str)
    {
        LogStr = LogStr + "[LOG] " + Str + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[LOG] " << Str << "\n";
#endif
    }

    template<typename... Args>
    void PrintArgs(const char* Str, Args&&... args)
    {
        LogStr = LogStr + "[LOG] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[LOG] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#endif
    }

    template<typename... Args>
    void PrintErrorArgs(const char* Str, Args&&... args)
    {
        LogStr = LogStr + "[ERR] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[ERR] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#endif
    }

    template<typename... Args>
    void PrintInternalArgs(const char* Str, Args&&... args)
    {
        LogStr = LogStr + "[TWINK] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
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

    void Render()
    {
        using namespace ImGui;
        for (IModule* Module : Modules)
        {
            Module->Render(TrackmaniaMgr);
        }

        if (BeginMainMenuBar()) {
            if (BeginMenu("Twinkie")) {
                if (MenuItem("Settings", "", EnableSettings))
                {
                    EnableSettings = !EnableSettings;
                }
                for (IModule* Module : Modules)
                {
                    Module->RenderMenuItem(TrackmaniaMgr);
                }
                ImGui::EndMenu();
            }
            EndMainMenuBar();
        }

        if (EnableSettings)
        {
            RenderSettings();
        }
    }

    void RenderSettings()
    {
        using namespace ImGui;

        if (Begin("Settings", &EnableSettings))
        {
            BeginTabBar("##TwinkieSettings");

            for (IModule* Module : Modules)
            {
                if (!Module->HasSettings) continue;

                Module->RenderSettings();
            }

            EndTabBar();
        }
        End();
    }

    void RenderAnyways()
    {
        TrackmaniaMgr.CurPlayerInfo = TrackmaniaMgr.GetPlayerInfo();

        for (IModule* Module : Modules)
        {
            Module->RenderAnyways(TrackmaniaMgr);
        }
    }
};