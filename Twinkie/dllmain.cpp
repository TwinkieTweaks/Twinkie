#include <iostream>
#include "Twink.h"
#include "imgui-dx9/imgui_impl_dx9.h"
#include "imgui-dx9/imgui_impl_win32.h"

#include <Windows.h>
#include <d3d9.h>
#include <intrin.h>

// typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
using ResetFn = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
using PresentFn = long(__stdcall*)(LPDIRECT3DDEVICE9 pDevice, LPVOID, LPVOID, HWND, LPVOID);

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#ifdef BUILD_TMMC
extern "C" __declspec(dllexport) int Bla() // TODO: Remove when modloader updates
{
	return 1;
}
#endif

ResetFn oReset = NULL;
// EndScene oEndScene = NULL;
PresentFn oPresent = NULL;
WNDPROC oWndProc;
static HWND window = NULL;

Twink Twinkie;

static void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	Twinkie.SetupImGuiStyle();
	ImGuiIO& ImIo = ImGui::GetIO();
	ImIo.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);

	Twinkie.InitFonts(ImIo);
}

bool init = false;
static long __stdcall hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pParams)
{
	if (!init) return oReset(pDevice, pParams);
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const HRESULT result = oReset(pDevice, pParams);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result; 
}

static long __stdcall hkPresent(LPDIRECT3DDEVICE9 pDevice, LPVOID A, LPVOID B, HWND C, LPVOID D)
{
	if (GetAsyncKeyState(VK_F3) & 1) Twinkie.DoRender = !Twinkie.DoRender;

	if (!init)
	{
		InitImGui(pDevice);
		init = true;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (Twinkie.DoRender) Twinkie.Render();
	Twinkie.RenderAnyways();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oPresent(pDevice, A, B, C, D);
}

static LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if ((bool)ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

static BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE; // skip to next window

	window = handle;
	return FALSE; // window found abort search
}

static HWND GetProcessWindow()
{
	window = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return window;
}

static DWORD WINAPI MainThread(LPVOID lpReserved)
{
#ifdef BUILD_EXTERNAL_CONSOLE
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif

	while (!Twinkie.GetTrackmania())
	{
		Sleep(1);
	}

	Twinkie.Init();

	bool attached = false;
	do
	{
		Twinkie.PrintInternal("Hooking DX9...");
		if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
		{
			Twinkie.PrintInternal("kiero initialized");
			kiero::bind(16, (void**)&oReset, hkReset);
			Twinkie.DX9HookStatus = kiero::bind(17, (void**)&oPresent, hkPresent);
			Twinkie.PrintInternalArgs("kiero status: {}", (int)Twinkie.DX9HookStatus);
			do
				window = GetProcessWindow();
			while (window == NULL);
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
			attached = true;
		}
		else
		{
			Twinkie.PrintError("Error when hooking DX9.");
			Twinkie.PrintErrorArgs("kiero status: {}", (int)Twinkie.DX9HookStatus);
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