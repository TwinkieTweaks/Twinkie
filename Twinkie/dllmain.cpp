#define WIN32_LEAN_AND_MEAN
// #define BUILD_DEBUG
// #define BUILD_PREMIUM

#include <iostream>
#include "TwinkUi.h"
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

static void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	Twinkie.SetupImGuiStyle();
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
	if (!Twinkie.Initialized)
	{
		InitImGui(pDevice);
		Twinkie.Initialized = true;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Twinkie.RenderAnyways(); // aintnoway
	if (Twinkie.DoRender) Twinkie.Render();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return Twinkie.oPresent(pDevice, A, B, C, D);
}

static LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	if (!Twinkie.Initialized) return CallWindowProcA(Twinkie.oWndProc, hWnd, uMsg, wParam, lParam);

	if (GetAsyncKeyState(VK_F3) & 1) Twinkie.DoRender = !Twinkie.DoRender;

	auto& ImIo = ImGui::GetIO();

	auto ImWndProcResult = ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	if (ImWndProcResult)
	{
		return ImWndProcResult;
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

	while (!Twinkie.TrackmaniaMgr.GetTrackmania())
	{
		Sleep(1);
	}
	while (!Twinkie.TrackmaniaMgr.GetVisionViewport())
	{
		Sleep(1);
	}
	while (!Twinkie.TrackmaniaMgr.GetD3DDevice())
	{
		Sleep(1);
	}
	auto D3DDevice = Twinkie.TrackmaniaMgr.GetD3DDevice();
	D3DDEVICE_CREATION_PARAMETERS D3DCreationParams = {};
	while ((D3DDevice->GetCreationParameters(&D3DCreationParams) != D3D_OK) or !D3DCreationParams.hFocusWindow)
	{
		Sleep(1);
	}

	bool HookedAndAttached = false;
	while (!HookedAndAttached)
	{
		Twinkie.Logger.PrintInternal("Hooking DX9...");
		if ((Twinkie.DX9HookStatus = kiero::init(kiero::RenderType::D3D9)) == kiero::Status::Success)
		{
			Twinkie.Logger.PrintInternal("kiero initialized");
			kiero::bind(16, (void**)&Twinkie.oReset, hkReset);
			Twinkie.DX9HookStatus = kiero::bind(17, (void**)&Twinkie.oPresent, hkPresent);
			Twinkie.Logger.PrintInternalArgs("kiero status: {}", (int)Twinkie.DX9HookStatus);
			
			if (D3DDevice->GetCreationParameters(&D3DCreationParams) == D3D_OK)
			{
				Twinkie.Window = D3DCreationParams.hFocusWindow;

				Twinkie.oWndProc = (WNDPROC)SetWindowLongPtr(Twinkie.Window, GWLP_WNDPROC, (LONG_PTR)WndProc);
				if (!Twinkie.oWndProc)
					Twinkie.Logger.PrintErrorArgs("Failed to hook WndProc");
				HookedAndAttached = true;
			}
			else
			{
				Twinkie.Logger.PrintError("Could not get D3D9CreationParameters");
			}
		}
	}

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
		kiero::shutdown();
		break;
	}
	return TRUE;
}