#include <iostream>
#include "Twink.h"

#include <Windows.h>
#include <d3d9.h>
//#include <d3dx9.h>
#include "kiero/kiero.h"
#include "kiero/minhook/include/MinHook.h"
//#include "imgui-dx9/imgui.h"
//#include "imgui-dx9/imgui_impl_win32.h"
//#include "imgui-dx9/imgui_impl_dx9.h"
#define WINDOW_NAME "Dear ImGui DirectX9 Example"
typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

using ResetFn = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

#ifdef _WIN64
#define GWL_WNDPROC GWLP_WNDPROC
#endif

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern "C" __declspec(dllexport) int Bla() // TODO: Remove when modloader updates
{
	return 1;
}

ResetFn oReset = NULL;
EndScene oEndScene = NULL;
WNDPROC oWndProc;
static HWND window = NULL;

Twink Twinkie;

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	Twinkie.SetupImGuiStyle();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);
	ImGui_ImplDX9_CreateDeviceObjects();
}

bool init = false;
long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
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

	return oEndScene(pDevice);
}

long __stdcall hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pParams)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const HRESULT result = oReset(pDevice, pParams);
	ImGui_ImplDX9_CreateDeviceObjects();

	return result;
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE; // skip to next window

	window = handle;
	return FALSE; // window found abort search
}

HWND GetProcessWindow()
{
	window = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return window;
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	// BEGIN: Modloader
	while (!Twinkie.GetTrackmania())
	{
		Sleep(1);
	}

	Twinkie.ReInit();
	// END: Modloader

	bool attached = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
		{
			kiero::bind(42, (void**)&oEndScene, hkEndScene);
			// kiero::bind(16, (void**)&oReset, hkReset);
			do
				window = GetProcessWindow();
			while (window == NULL);
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
			attached = true;
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