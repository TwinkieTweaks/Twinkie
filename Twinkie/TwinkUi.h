#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <format>
#include <iomanip>
#include <sstream>
#include <vector>

#include "TwinkTrackmania.h"
#include "Version.h"
#include "SettingMgr.h"
#include "TwinkLogs.h"

#include "imgui-dx9/imgui.h"
#include "GlyphTable/IconsForkAwesome.h"
#include "GlyphTable/IconsKenney.h"
#include "TwinkIo.h"

#include "Modules/CheckpointCounter/CheckpointCounter.h"
#include "Modules/About/About.h"
#include "Modules/Dashboard/DashboardInputs.h"
#include "Modules/Dashboard/DashboardTacho.h"
#include "Modules/Medals/Medals.h"
#include "Modules/GhostEditor/GhostEditor.h"
#include "Modules/Dashboard/DashboardGears.h"
#include "Modules/Tweaker/Tweaker.h"
#include "Modules/AlwaysOfficial/AlwaysOfficial.h"
#include "Modules/NicknamePatch/NicknamePatch.h"
#include "Modules/Telemetry/Telemetry.h"
#include "Modules/DownloadServerMaps/DownloadServerMaps.h"
#include "Modules/SplitSpeeds/SplitSpeeds.h"
#include "Modules/GrindingStats/GrindingStats.h"
#ifdef BUILD_DEBUG
#include "Modules/PlayerInfo/PlayerInfo.h"
#endif
#ifdef BUILD_PREMIUM
#include "Modules/AppExplorer.h"
#endif

#include <d3d9.h>

#include "kiero/kiero.h"
#include "kiero/minhook/include/MinHook.h"

using WNDPROC = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);
using ResetFn = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
using PresentFn = long(__stdcall*)(LPDIRECT3DDEVICE9 pDevice, LPVOID, LPVOID, HWND, LPVOID);

class TwinkUi
{
public:
    Versioning Versions;
    TwinkTrackmania TrackmaniaMgr;
    TwinkLogs Logger;
    TwinkIo* IoMgr;
	ImFont* FontMain = nullptr;

    float UiScale = 1.f;

    bool DoRender = true;
    bool Initialized = false;
    bool ForceModulesNoRender = false;

    ResetFn oReset = NULL;
    PresentFn oPresent = NULL;
    WNDPROC oWndProc = NULL;
    HWND Window = NULL;

    bool EnableSettings = false;
    bool EnableImGuiDemo = false;

    std::vector<IModule*> Modules = {};

    kiero::Status::Enum DX9HookStatus = kiero::Status::UnknownError;

    SettingMgr Settings;

    size_t ActiveModuleIdx = 0;
    bool IsTwinkieSettingsOpen = true;

    void SettingsInit()
    {
        for (IModule* Module : Modules)
        {
            Module->SettingsInit(Settings);
        }

        Settings["Twinkie"]["UI Scale"].GetAsFloat(&UiScale);
    }

    void SettingsSave()
    {
        for (IModule* Module : Modules)
        {
            Module->SettingsSave(Settings);
        }

		Settings["Twinkie"]["UI Scale"].Set(UiScale);
    }

    TwinkUi()
    {
        IoMgr = new TwinkIo(TrackmaniaMgr);

        Logger.PrintInternal(":3c");
        Logger.PrintInternalArgs("Twinkie for TrackMania Forever. Version {}", Versions.TwinkieVer);

#ifdef BUILD_DEBUG
        Modules.push_back(new PlayerInfoModule(TrackmaniaMgr, Logger, &DoRender));
#endif
#ifdef BUILD_PREMIUM
        Modules.push_back(new AppExplorerModule(TrackmaniaMgr, Logger, &DoRender));
#endif
        Modules.push_back(new AboutModule(TrackmaniaMgr, Logger, &DoRender));
        //
        Modules.push_back(new DashboardInputsModule(TrackmaniaMgr, Logger, &DoRender));
        Modules.push_back(new DashboardTachometerModule(TrackmaniaMgr, Logger, &DoRender));
        Modules.push_back(new DashboardGearsModule(TrackmaniaMgr, Logger, &DoRender));
        //
        Modules.push_back(new CheckpointCounterModule(TrackmaniaMgr, Logger, &DoRender));
        Modules.push_back(new SplitSpeedsModule(TrackmaniaMgr, Logger, &DoRender));
        Modules.push_back(new GrindingStatsModule(TrackmaniaMgr, Logger, &DoRender));
        //
        Modules.push_back(new MedalsModule(TrackmaniaMgr, Logger, &DoRender));
        //
        Modules.push_back(new GhostEditorModule(TrackmaniaMgr, Logger, &DoRender));
        Modules.push_back(new TweakerModule(TrackmaniaMgr, Logger, &DoRender));
        Modules.push_back(new AlwaysOfficialModule(TrackmaniaMgr, Logger, &DoRender));
        Modules.push_back(new NicknamePatchModule(TrackmaniaMgr, Logger, &DoRender));
        Modules.push_back(new TelemetryModule(TrackmaniaMgr, Logger, &DoRender));
        Modules.push_back(new DownloadServerMapsModule(TrackmaniaMgr, Logger, &DoRender));

        Logger.PrintInternalArgs("{} module{} initialized.", Modules.size(), Modules.size() == 1 ? "" : "s");

        SettingsInit();
    }

    ~TwinkUi()
    {
        SettingsSave();

        Settings.Save();

        for (IModule* Module : Modules)
        {
            delete Module;
            Module = nullptr;
        }

        delete IoMgr;
        IoMgr = nullptr;
    }

    void InitFonts(ImGuiIO& ImIo)
    {
        FontMain = ImIo.Fonts->AddFontFromFileTTF((GetDocumentsFolder() + "\\Twinkie\\Fonts\\Twinkie.ttf").c_str(), 14.f * UiScale);
       
        if (FontMain)
        {
            Logger.PrintInternal("Font \"Main\" initialized.");
        }
        else
        {
            Logger.PrintError("Font \"Main\" not initialized.");
            return;
        }

        // Taken example from https://github.com/juliettef/IconFontCppHeaders?tab=readme-ov-file#example-code
        float IconFontSize = (14.f * UiScale);

        static const ImWchar IconRange[] = { ICON_MIN_KI, ICON_MAX_FK, 0 };
        ImFontConfig IconCfg;
        IconCfg.MergeMode = true;
        IconCfg.PixelSnapH = true;
        IconCfg.GlyphMinAdvanceX = IconFontSize;

        auto FontManiaIcons = ImIo.Fonts->AddFontFromFileTTF((GetDocumentsFolder() + "\\Twinkie\\Fonts\\ManiaIcons.ttf").c_str(), IconFontSize, &IconCfg, IconRange);

        if (FontManiaIcons)
        {
            Logger.PrintInternal("Font \"ManiaIcons\" initialized.");
        }
        else
        {
            Logger.PrintError("Font \"ManiaIcons\" not initialized.");
        }
    }

    void SetupImGuiStyle()
    {
        // Soft Cherry style by Patitotective from ImThemes, twinkified
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
        style.GrabMinSize = 8.0f;
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
        if (FontMain) PushFont(FontMain);

        for (IModule* Module : Modules)
        {
            if (ForceModulesNoRender) break;
            if (Module->Enabled) Module->Render();
        }

        if (BeginMainMenuBar()) {
            PushStyleColor(ImGuiCol_Text, ColorConvertFloat4ToU32({1.f, 0.f, 1.f, 1.f}));
            if (BeginMenu(ICON_FK_COGS " Twinkie")) {
                PopStyleColor();
                if (MenuItem(ICON_FK_WRENCH " Settings", "", EnableSettings))
                {
                    EnableSettings = !EnableSettings;
                }
                Separator();
                if (MenuItem(ICON_FK_POWER_OFF " Shutdown", ""))
                {
                    TrackmaniaMgr.CallGbxAppExit();
                }
                if (IsItemHovered())
                {
                    SetTooltip("Softly closes the game.");
                }
                if (MenuItem(ICON_FK_EXCLAMATION_TRIANGLE " Terminate", ""))
                {
                    Logger.Print("Terminating, as user commanded");
                    free((void*)TrackmaniaMgr.GetTrackmania());
                }
                if (IsItemHovered())
                {
                    SetTooltip(ICON_FK_EXCLAMATION_TRIANGLE " WARNING! This crashes the game.");
                }
                ImGui::EndMenu();
            }
            else PopStyleColor();
            if (BeginMenu(ICON_FK_SLIDERS " Modules"))
            {
                for (IModule* Module : Modules)
                {
                    if (!Module->IsDebug()) Module->RenderMenuItem();
                }
                ImGui::EndMenu();
            }
            if (BeginMenu(ICON_FK_CODE " Debug"))
            {
                if (MenuItem(ICON_FK_HDD_O " Log", "", Logger.EnableLog))
                {
                    Logger.EnableLog = !Logger.EnableLog;
                }
                if (MenuItem(ICON_FK_BAN " Disable all modules", "", ForceModulesNoRender))
                {
                    ForceModulesNoRender = !ForceModulesNoRender;
                }
                for (IModule* Module : Modules)
                {
                    if (Module->IsDebug()) Module->RenderMenuItem();
                }
#ifdef BUILD_DEBUG
                if (MenuItem("ImGui Demo", "", EnableImGuiDemo))
                {
                    EnableImGuiDemo = !EnableImGuiDemo;
                }
#endif
                ImGui::EndMenu();
            }
            EndMainMenuBar();
        }

        if (EnableSettings)
            RenderSettings();

        if (Logger.EnableLog)
            Logger.RenderLog();

#ifdef BUILD_DEBUG
        if (EnableImGuiDemo)
        {
            ShowDemoWindow(&EnableImGuiDemo);
        }
#endif

        if (FontMain) PopFont();
    }

    void RenderSettings()
    {
        using namespace ImGui;

        if (Begin(ICON_FK_WRENCH " Settings", &EnableSettings))
        {
            BeginChild("##TwinkieSettingsModulesList", {150.f, 0.f}, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);

            size_t CurModuleIdx = 0;

            PushStyleColor(ImGuiCol_Text, ColorConvertFloat4ToU32({ 1.f, 0.f, 1.f, 1.f }));
            if (Selectable(ICON_FK_COGS " Twinkie", IsTwinkieSettingsOpen))
            {  
                IsTwinkieSettingsOpen = !IsTwinkieSettingsOpen;
            }
            PopStyleColor();
            
            Separator();

            for (IModule* Module : Modules)
            {
                if (!Module->HasSettings())
                {
                    CurModuleIdx++;
                    continue;
                }

                if (Selectable(((Module->Enabled ? ICON_FK_CHECK " " : ICON_FK_TIMES " ") + Module->FancyName).c_str(), ActiveModuleIdx == CurModuleIdx))
                {
                    ActiveModuleIdx = CurModuleIdx;
                    IsTwinkieSettingsOpen = false;
                }

                CurModuleIdx++;
            }

            EndChild();
            
            SameLine();

            BeginGroup();

            PushStyleColor(ImGuiCol_ChildBg, ColorConvertFloat4ToU32({ 0.f, 0.f, 0.f, 0.f }));
            BeginChild("##TwinkieSettingsRender");
            PopStyleColor();

            if (!IsTwinkieSettingsOpen)
            {
                IModule* ActiveModule = Modules[ActiveModuleIdx];

                ActiveModule->RenderSettings();
            }
            else
            {
                SliderFloat("UI Scale", &UiScale, 0.25f, 5.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
                UiScale = roundf(UiScale / 0.25f) * 0.25f;

                SameLine();

                BeginDisabled();

                Text("(?)");
                if (IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
                {
                    SetTooltip("Applies only on restart.");
                }

                EndDisabled();
            }

            EndChild();

            EndGroup();
        }
        End();
    }

    void RenderAnyways()
    {
		using namespace ImGui;

		if (FontMain) PushFont(FontMain);

        IoMgr->Update();

        for (IModule* Module : Modules)
        {
            if (ForceModulesNoRender) break;
            if (Module->Enabled) Module->RenderAnyways();
            Module->RenderInactive();
        }

		if (FontMain) PopFont();
    }
};