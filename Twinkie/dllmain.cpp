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
	ImIo.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
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
	if (GetAsyncKeyState(VK_F3) & 1) Twinkie.DoRender = !Twinkie.DoRender;

	if (!Twinkie.Initialized)
	{
		InitImGui(pDevice);
		Twinkie.Initialized = true;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (Twinkie.DoRender) Twinkie.Render();
	Twinkie.RenderAnyways();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return Twinkie.oPresent(pDevice, A, B, C, D);
}

static LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if ((bool)ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(Twinkie.oWndProc, hWnd, uMsg, wParam, lParam);
}

static BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE; // skip to next window

	Twinkie.Window = handle;
	return FALSE; // window found abort search
}

static HWND GetProcessWindow()
{
	Twinkie.Window = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return Twinkie.Window;
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

	bool attached = false;
	do
	{
		Twinkie.Logger.PrintInternal("Hooking DX9...");
		if ((Twinkie.DX9HookStatus = kiero::init(kiero::RenderType::D3D9)) == kiero::Status::Success)
		{
			Twinkie.Logger.PrintInternal("kiero initialized");
			kiero::bind(16, (void**)&Twinkie.oReset, hkReset);
			Twinkie.DX9HookStatus = kiero::bind(17, (void**)&Twinkie.oPresent, hkPresent);
			Twinkie.Logger.PrintInternalArgs("kiero status: {}", (int)Twinkie.DX9HookStatus);
			do
				Twinkie.Window = GetProcessWindow();
			while (Twinkie.Window == NULL);
			Twinkie.oWndProc = (WNDPROC)SetWindowLongPtr(Twinkie.Window, GWL_WNDPROC, (LONG_PTR)WndProc);
			attached = true;
		}
		else
		{
			Twinkie.Logger.PrintError("Error when hooking DX9.");
		}
	} while (!attached);

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