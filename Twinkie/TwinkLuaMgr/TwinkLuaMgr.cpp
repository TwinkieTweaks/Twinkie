#include "TwinkLuaMgr.h"
#include <filesystem>

namespace Filesystem = std::filesystem;
void TwinkLuaMgr::GetModulesFromDocuments()
{
	std::string DocumentsFolder = GetDocumentsFolder();
	Filesystem::path LuaModulesPath = DocumentsFolder + "\\Twinkie\\LuaScripts";

	if (!Filesystem::exists(LuaModulesPath))
	{
		Filesystem::create_directories(LuaModulesPath);
	}

	for (const auto& Entry : Filesystem::directory_iterator(LuaModulesPath))
	{
		if (Entry.is_regular_file() && Entry.path().extension() == ".lua")
		{
			std::string FilePath = Entry.path().string();
			std::string Filename = Entry.path().filename().string();
			char ErrorStrBuffer[512] = { 0 };

			InitLua(Filename.c_str());
			ExportImGuiForState(Filename.c_str());
			SetImGuiContext(ImGui::GetCurrentContext());
			RunLuaFile(Filename.c_str(), FilePath.c_str(), ErrorStrBuffer, sizeof(ErrorStrBuffer));
			if (strcmp(ErrorStrBuffer, "OK") != 0)
			{
				Logger->PrintErrorArgs("Error while initializing lua module {}: {}", Filename, ErrorStrBuffer);
				continue;
			}

			ModuleInfo* pLuaModuleInfo;
			ModuleInfo LuaModuleInfo;
			pLuaModuleInfo = RunModuleInfo(Filename.c_str(), ErrorStrBuffer, sizeof(ErrorStrBuffer));
			if (!pLuaModuleInfo)
			{
				Logger->PrintErrorArgs("Error while getting module info for {}: {}", Filename, ErrorStrBuffer);
				delete pLuaModuleInfo;
				continue;
			}
			LuaModuleInfo = *pLuaModuleInfo;

			ILuaModule* LuaModule = new ILuaModule(Filename.c_str(), &LuaModuleInfo);
			delete pLuaModuleInfo;

			LuaModules.push_back(LuaModule);
		}
	}
}

void TwinkLuaMgr::RunModulesRender()
{
	for (ILuaModule* LuaModule : LuaModules)
	{
		if (LuaModule->Enabled and !LuaModule->HasErrored)
		{
			char ErrorStrBuffer[512] = { 0 };
			RunRender(LuaModule->Filename.c_str(), ErrorStrBuffer, sizeof(ErrorStrBuffer));
			if (strcmp(ErrorStrBuffer, "OK") != 0)
			{
				Logger->PrintErrorArgs("Error while running lua module {}: {}", LuaModule->Filename, ErrorStrBuffer);
				CloseLua(LuaModule->Filename.c_str());
				LuaModule->HasErrored = true;
			}
		}
	}
}

void TwinkLuaMgr::RunModulesRenderMenuItem()
{
	for (ILuaModule* LuaModule : LuaModules)
	{
		if (LuaModule->HasMenuItemFn)
		{
			char ErrorStrBuffer[512] = { 0 };
			RunRenderMenuItem(LuaModule->Filename.c_str(), ErrorStrBuffer, sizeof(ErrorStrBuffer));
			if (strcmp(ErrorStrBuffer, "OK") != 0)
			{
				Logger->PrintWarnArgs("While running 'RenderMenuItem' in {}: {}", LuaModule->Filename, ErrorStrBuffer);
				LuaModule->HasMenuItemFn = false;
			}
		}
	}
}

void TwinkLuaMgr::RunModulesRenderMainMenuItem()
{
	for (ILuaModule* LuaModule : LuaModules)
	{
		if (LuaModule->HasMainMenuItemFn)
		{
			char ErrorStrBuffer[512] = { 0 };
			RunRenderMainMenuItem(LuaModule->Filename.c_str(), ErrorStrBuffer, sizeof(ErrorStrBuffer));
			if (strcmp(ErrorStrBuffer, "OK") != 0)
			{
				Logger->PrintWarnArgs("While running 'RenderMainMenuItem' in {}: {}", LuaModule->Filename, ErrorStrBuffer);
				LuaModule->HasMainMenuItemFn = false;
			}
		}
	}
}

void TwinkLuaMgr::RunModulesRenderSettings()
{
	for (ILuaModule* LuaModule : LuaModules)
	{
		if (LuaModule->HasSettingsFn)
		{
			char ErrorStrBuffer[512] = { 0 };
			RunRenderSettings(LuaModule->Filename.c_str(), ErrorStrBuffer, sizeof(ErrorStrBuffer));
			if (strcmp(ErrorStrBuffer, "OK") != 0)
			{
				Logger->PrintWarnArgs("While running 'RenderSettings' in {}: {}", LuaModule->Filename, ErrorStrBuffer);
				LuaModule->HasSettingsFn = false;
			}
		}
	}
}

void TwinkLuaMgr::ReloadModule(const char* Filename)
{
	std::string DocumentsFolder = GetDocumentsFolder();
	Filesystem::path LuaModulePath = DocumentsFolder + "\\Twinkie\\LuaScripts";

	if (!Filesystem::exists(LuaModulePath))
	{
		Filesystem::create_directories(LuaModulePath);
	}

	LuaModulePath /= Filename;

	for (ILuaModule* LuaModule : LuaModules)
	{
		if (LuaModule->Filename == Filename)
		{
			char ErrorStrBuffer[512] = { 0 };

			CloseLua(LuaModule->Filename.c_str());
			LuaModule->HasErrored = false;
			InitLua(Filename);
			SetImGuiContext(ImGui::GetCurrentContext());
			RunLuaFile(Filename, LuaModulePath.string().c_str(), ErrorStrBuffer, sizeof(ErrorStrBuffer));
			if (strcmp(ErrorStrBuffer, "OK") != 0)
			{
				Logger->PrintErrorArgs("Error while reloading lua module {}: {}", Filename, ErrorStrBuffer);
				LuaModule->HasErrored = true;
				return;
			}

			ModuleInfo* pLuaModuleInfo;
			ModuleInfo LuaModuleInfo;
			pLuaModuleInfo = RunModuleInfo(Filename, ErrorStrBuffer, sizeof(ErrorStrBuffer));
			if (!pLuaModuleInfo)
			{
				Logger->PrintErrorArgs("Error while getting module info for {}: {}", Filename, ErrorStrBuffer);
				delete pLuaModuleInfo;
				LuaModule->HasErrored = true;
				return;
			}
			LuaModuleInfo = *pLuaModuleInfo;

			LuaModule->LuaModuleInfo = LuaModuleInfo;
			LuaModule->HasErrored = false;
			LuaModule->HasMenuItemFn = true;
			LuaModule->HasMainMenuItemFn = true;
			LuaModule->HasSettingsFn = true;

			delete pLuaModuleInfo;
			return;
		}
	}
}