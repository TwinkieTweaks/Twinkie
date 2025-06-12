#include "GhostEditor.h"

void GhostEditorModule::RenderMenuItem()
{
	using namespace ImGui;
	if (MenuItem(ICON_FK_CAR " Ghost editor") and !Twinkie->GetChallenge() and Twinkie->GetProfileScores())
	{
		Twinkie->CallMenuGhostEditor();
		PatchChallengeMainLoop();
	}
}

GhostEditorModule::GhostEditorModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled)
{
	this->UiRenderEnabled = UiRenderEnabled;
	this->Twinkie = &Twinkie;
	this->Logger = &Logger;
	this->Name = "GhostEditor";
	this->FancyName = "Ghost editor";
}

void GhostEditorModule::PatchChallengeMainLoop()
{
#define CHALLENGEMAINLOOPPATCH_ADDR_START 0x958FC + (size_t)this->Twinkie->GetExeBaseAddr()
#define CHALLENGEMAINLOOPPATCH_ADDR_END 0x95912 + (size_t)this->Twinkie->GetExeBaseAddr()

	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	DWORD PageSize = SystemInfo.dwPageSize;

	size_t PatchSize = CHALLENGEMAINLOOPPATCH_ADDR_END - CHALLENGEMAINLOOPPATCH_ADDR_START;
	uintptr_t AlignedStart = CHALLENGEMAINLOOPPATCH_ADDR_START & ~(PageSize - 1);
	size_t TotalSize = CHALLENGEMAINLOOPPATCH_ADDR_END - AlignedStart;

	DWORD OldProtection;
	if (!VirtualProtect((LPVOID)AlignedStart, TotalSize, PAGE_EXECUTE_READWRITE, &OldProtection))
	{
		this->Logger->PrintErrorArgs("VirtualProtect failed: 0x{:x}", GetLastError());
		this->Logger->PrintErrorArgs("Attempted patch from 0x{:x} to 0x{:x}", CHALLENGEMAINLOOPPATCH_ADDR_START, CHALLENGEMAINLOOPPATCH_ADDR_END);
		return;
	}

	char Nop = (char)0x90;
	for (uintptr_t Addr = CHALLENGEMAINLOOPPATCH_ADDR_START; Addr < CHALLENGEMAINLOOPPATCH_ADDR_END; ++Addr)
	{
		memcpy((void*)Addr, &Nop, 1);
	}

	DWORD Dummy;
	VirtualProtect((LPVOID)AlignedStart, TotalSize, OldProtection, &Dummy);

	this->Logger->Print("Patched ChallengeMainLoop!");
}