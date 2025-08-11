#pragma once
#pragma execution_character_set("utf-8")

#include <string>
#include <format>
#include <iomanip>
#include <sstream>
#include <vector>
#include <chrono>

#include "../Version.h"
#include "../SettingMgr/SettingMgr.h"
#include "../TwinkTrackmania/TwinkTrackmania.h"
#include "../TwinkLogs.h"
#include "../TwinkLuaMgr/TwinkLuaMgr.h"
#include "../TwinkIo/TwinkIo.h"

#include "../imgui-dx9/imgui.h"
#include "../GlyphTable/IconsForkAwesome.h"
#include "../GlyphTable/IconsKenney.h"

#include "../Modules/About/About.h"
#include "../Modules/AlwaysOfficial/AlwaysOfficial.h"
#include "../Modules/CheckpointCounter/CheckpointCounter.h"
#include "../Modules/Dashboard/DashboardInputs.h"
#include "../Modules/Dashboard/DashboardTacho.h"
#include "../Modules/Dashboard/DashboardGears.h"
#include "../Modules/DownloadServerMaps/DownloadServerMaps.h"
#include "../Modules/GhostEditor/GhostEditor.h"
#include "../Modules/GrindingStats/GrindingStats.h"
#include "../Modules/Medals/Medals.h"
#include "../Modules/SplitSpeeds/SplitSpeeds.h"
#include "../Modules/Tweaker/Tweaker.h"
#include "../Modules/Telemetry/Telemetry.h"
#ifdef BUILD_DEBUG
#include "../Modules/PlayerInfo/PlayerInfo.h"
#endif
#include "../Modules/LuaConsole/LuaConsole.h"
#ifdef BUILD_PREMIUM
#include "../Modules/AppExplorer.h"
#endif

#include <d3d9.h>

#include "../kiero/kiero.h"
#include "../kiero/minhook/include/MinHook.h"

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
    TwinkLuaMgr* LuaMgr;
    ImFont* FontMain = nullptr;

    float UiScale = 1.f;

    bool DoRender = true;
    bool Initialized = false;
    bool ForceModulesNoRender = false;

    bool WantFullscreenWindowed = false;
    bool FullscreenWindowed = false;
    RECT WindowedRect = { 0, 0, 0, 0 };

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

    bool LuaModulesLoaded = false;

    std::chrono::system_clock::time_point LastAutosaveTime = std::chrono::system_clock::now();
    float AutosaveIntervalMinutes = 2.f;

    void SettingsInit();
    void SettingsSave();

    TwinkUi();
    ~TwinkUi();
    void PatchFullscreenWindowed(HWND WindowHandle);
    void InitFonts(ImGuiIO& ImIo);
    void SetupImGuiStyle();

    void Render();
    void RenderSettings();
    void RenderAnyways();
};