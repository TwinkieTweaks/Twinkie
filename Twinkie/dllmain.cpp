#ifndef LUAJIT
#include "LuaEngine/include/lua.hpp"
#pragma comment(lib, "LuaEngine/lua54.lib")
#else
#include "LuaEngine/include/lua.hpp"
#pragma comment(lib, "LuaEngine/lua51.lib")
#endif
#pragma comment(lib, "LuaEngine/LuaEngine2.lib")
#include "LuaEngine/LuaEngine2.h"
#define WIN32_LEAN_AND_MEAN

const char* g_NamesOfMwTypes[] =
{
	"ACTION",
	"BOOL",
	"BOOLARRAY",
	"BOOLBUFFER",
	"BOOLBUFFERCAT",
	"CLASS",
	"CLASSARRAY",
	"CLASSBUFFER",
	"CLASSBUFFERCAT",
	"COLOR",
	"COLORARRAY",
	"COLORBUFFER",
	"COLORBUFFERCAT",
	"ENUM",
	"INT",
	"INTARRAY",
	"INTBUFFER",
	"INTBUFFERCAT",
	"INTRANGE",
	"ISO4",
	"ISO4ARRAY",
	"ISO4BUFFER",
	"ISO4BUFFERCAT",
	"ISO3",
	"ISO3ARRAY",
	"ISO3BUFFER",
	"ISO3BUFFERCAT",
	"ID",
	"IDARRAY",
	"IDBUFFER",
	"IDBUFFERCAT",
	"NATURAL",
	"NATURALARRAY",
	"NATURALBUFFER",
	"NATURALBUFFERCAT",
	"NATURALRANGE",
	"REAL",
	"REALARRAY",
	"REALBUFFER",
	"REALBUFFERCAT",
	"REALRANGE",
	"STRING",
	"STRINGARRAY",
	"STRINGBUFFER",
	"STRINGBUFFERCAT",
	"STRINGINT",
	"STRINGINTARRAY",
	"STRINGINTBUFFER",
	"STRINGINTBUFFERCAT",
	"VEC2",
	"VEC2ARRAY",
	"VEC2BUFFER",
	"VEC2BUFFERCAT",
	"VEC3",
	"VEC3ARRAY",
	"VEC3BUFFER",
	"VEC3BUFFERCAT",
	"VEC4",
	"VEC4ARRAY",
	"VEC4BUFFER",
	"VEC4BUFFERCAT",
	"INT3",
	"INT3ARRAY",
	"INT3BUFFER",
	"INT3BUFFERCAT",
	"PROC"
};

const char* g_FontNames[3] =
{
	"CascadiaMono",
	"BricolageGrotesque",
	"DroidSans"
};

const char* g_ThemeNames[2] =
{
	"Twinkie",
	"Openplanet"
};

#include <iostream>
#include "TwinkUi/TwinkUi.h"
#include "imgui-dx9/imgui_impl_dx9.h"
#include "imgui-dx9/imgui_impl_win32.h"

#include <Windows.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#ifdef BUILD_TMMC
extern "C" __declspec(dllexport) int Bla() // TODO: Remove when modloader updates
{
	return 1;
}
#endif

TwinkUi Twinkie;
std::string g_LuaConsoleModuleOutputStr = "";
std::vector<std::string> g_LuaConsoleModulePreviousStatements = {};

__declspec(dllexport) void AddModule(IModule* Module)
{
	Twinkie.Modules.push_back(Module);
	Twinkie.Logger.PrintInternalArgs("New C++ module added: {}", Module->Name);
	Twinkie.Logger.PrintInternalArgs("{} C++ module{} initialized.", Twinkie.Modules.size(), Twinkie.Modules.size() == 1 ? "" : "s");
}

__declspec(dllexport) TwinkTrackmania* const GetTrackmaniaMgr()
{
	return &Twinkie.TrackmaniaMgr;
}

__declspec(dllexport) TwinkLogs* const GetLogger()
{
	return &Twinkie.Logger;
}

__declspec(dllexport) bool* const GetUiRenderEnabled()
{
	return &Twinkie.DoRender;
}

int LuaPrintFunction(lua_State* L)
{
	int NumberOfElems = lua_gettop(L);
	for (int Idx = 1; Idx <= NumberOfElems; Idx++) 
	{
		const char* LuaStr = lua_tostring(L, Idx);
		if (LuaStr) 
		{
			Twinkie.Logger.PrintCustom(LuaStr);
		}
		else 
		{
			if (lua_isnil(L, Idx))
			{
				Twinkie.Logger.PrintCustom("nil");
			}
			else if (lua_isboolean(L, Idx))
			{
				Twinkie.Logger.PrintCustom(lua_toboolean(L, Idx) ? "true" : "false");
			}
		}
		if (Idx < NumberOfElems) Twinkie.Logger.PrintCustom("\t");
	}
	Twinkie.Logger.PrintCustom("\n");
	return 0;
}

static void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	if (Twinkie.ThemeIdx == 0)
	{
		Twinkie.SetupTwinkieImGuiStyle();
	}
	else
	{
		Twinkie.SetupOpenplanetImGuiStyle();
	}
	ImGuiIO& ImIo = ImGui::GetIO();
	ImGui_ImplWin32_Init(Twinkie.Window);
	ImGui_ImplDX9_Init(pDevice);

	Twinkie.InitFonts(ImIo);
}

static long __stdcall hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pParams)
{
	if (!Twinkie.Initialized) return Twinkie.oReset(pDevice, pParams);
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const HRESULT result = Twinkie.oReset(pDevice, pParams);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result; 
}

static long __stdcall hkPresent(LPDIRECT3DDEVICE9 pDevice, LPVOID A, LPVOID B, HWND C, LPVOID D)
{
	Twinkie.TrackmaniaMgr.CurPlayerInfo = Twinkie.TrackmaniaMgr.GetPlayerInfo();

	if (!Twinkie.Initialized)
	{
		InitImGui(pDevice);
		Twinkie.Initialized = true;
		SetImGuiContext(ImGui::GetCurrentContext());
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Twinkie.RenderAnyways();
	if (Twinkie.DoRender) Twinkie.Render();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return Twinkie.oPresent(pDevice, A, B, C, D);
}

static LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	SetLuaPrintFunction(LuaPrintFunction);
	if (!Twinkie.Initialized) return CallWindowProcA(Twinkie.oWndProc, hWnd, uMsg, wParam, lParam);
	
	Twinkie.PatchFullscreenWindowed(hWnd);

	auto& ImIo = ImGui::GetIO();

	auto ImWndProcResult = ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	if (ImWndProcResult)
	{
		return ImWndProcResult;
	}

	if (uMsg == WM_KEYDOWN)
	{
		if (wParam == VK_F3 and !(lParam & 0xFF000000))
			Twinkie.DoRender = !Twinkie.DoRender;
	}

	if ((uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST && ImIo.WantCaptureKeyboard) || (uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST && ImIo.WantCaptureMouse))
	{
		return 1;
	}

	return CallWindowProcA(Twinkie.oWndProc, hWnd, uMsg, wParam, lParam);
}

static DWORD WINAPI MainThread(LPVOID lpReserved)
{
#ifdef BUILD_EXTERNAL_CONSOLE
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif

	// From: https://github.com/CodyNinja1/TMFHookDx9

	while (!Twinkie.TrackmaniaMgr.GetTrackmania()) Sleep(1);
	while (!Twinkie.TrackmaniaMgr.GetVisionViewport()) Sleep(1);
	while (!Twinkie.TrackmaniaMgr.GetD3DDevice()) Sleep(1);

	Twinkie.D3DDevice = Twinkie.TrackmaniaMgr.GetD3DDevice();
	D3DDEVICE_CREATION_PARAMETERS D3DCreationParams = {};
	while ((Twinkie.D3DDevice->GetCreationParameters(&D3DCreationParams) != D3D_OK) or !D3DCreationParams.hFocusWindow) Sleep(1);

	Twinkie.Window = D3DCreationParams.hFocusWindow;
	Twinkie.oWndProc = (WNDPROC)SetWindowLongPtr(D3DCreationParams.hFocusWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);
	Twinkie.oPresent = reinterpret_cast<PresentFn>(Twinkie.TrackmaniaMgr.VirtualWrite(17, (uintptr_t)Twinkie.D3DDevice, (uintptr_t)hkPresent));
	Twinkie.oReset = reinterpret_cast<ResetFn>(Twinkie.TrackmaniaMgr.VirtualWrite(16, (uintptr_t)Twinkie.D3DDevice, (uintptr_t)hkReset));

	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		Twinkie.TrackmaniaMgr.VirtualWrite(17, (uintptr_t)Twinkie.D3DDevice, (uintptr_t)Twinkie.oPresent);
		Twinkie.TrackmaniaMgr.VirtualWrite(16, (uintptr_t)Twinkie.D3DDevice, (uintptr_t)Twinkie.oReset);
		break;
	}
	return TRUE;
}