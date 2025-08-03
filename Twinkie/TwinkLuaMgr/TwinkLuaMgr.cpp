#include "TwinkLuaMgr.h"
#include <filesystem>

#define ErrorStrBufferSize 4096

#define BASE_MODULE_CONTENTS "-- Module template\n\nfunction Render()\n	-- This function is called every frame to render the module's content.\nend\n\nfunction RenderMenuItem()\n	-- This function is called to render the module's menu item.\nend\n\nfunction RenderMainMenuItem()\n	-- This function is called to render the module's menu item, in the top bar alongside others.\nend\n\nfunction RenderSettings()\n	-- This function is called to render the module's settings.\nend\n\nfunction ModuleInfo()\n    -- This function is called to get the module's info.\n	return {\n		Name = \"Module Name\",\n		Description = \"Module Description\",\n		Version = \"1.0.0\",\n		Author = \"Author Name\"\n    }\nend\n"

namespace Filesystem = std::filesystem;

bool SetWorkingDirectory(const std::string& Directory) 
{
	return SetCurrentDirectoryA(Directory.c_str()) != 0;
}

void TwinkLuaMgr::GetModulesFromDocuments(bool EnableByDefault)
{
	SetWorkingDirectory(GetDocumentsFolder() + "\\Twinkie\\LuaScripts");
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
			char ErrorStrBuffer[ErrorStrBufferSize] = { 0 };

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

			LuaModule->FancyName = LuaModuleInfo.Name;
			LuaModule->Name = Filename;
			if (EnableByDefault)
			{
				LuaModule->Reset();
				LuaModule->Enabled = true;
			}

			LuaModules.push_back(LuaModule);
		}
	}
}

void TwinkLuaMgr::CreateModule(const char* Filename)
{
	unsigned int Offset = 0;

	std::string DocumentsFolder = GetDocumentsFolder();
	Filesystem::path LuaModulePath = DocumentsFolder + "\\Twinkie\\LuaScripts\\";

	LuaModulePath /= (Filename + (Offset == 0 ? "" : std::to_string(Offset)) + ".lua");

	if (!Filesystem::exists(LuaModulePath.string()))
	{
		Filesystem::create_directories(LuaModulePath.parent_path());
	}

	std::ofstream LuaFile(LuaModulePath);
	if (!LuaFile.is_open())
	{
		Logger->PrintErrorArgs("Could not create Lua module file at {}", LuaModulePath.string());
		return;
	}

	LuaFile << BASE_MODULE_CONTENTS;
	LuaFile.close();

	ReloadModule(LuaModulePath.string().c_str());
}

void TwinkLuaMgr::RunModulesRender()
{
	for (ILuaModule* LuaModule : LuaModules)
	{
		if (LuaModule->Enabled and !LuaModule->HasErrored)
		{
			char ErrorStrBuffer[ErrorStrBufferSize] = { 0 };
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
		if (LuaModule->HasMenuItemFn and LuaModule->Enabled and !LuaModule->HasErrored)
		{
			char ErrorStrBuffer[ErrorStrBufferSize] = { 0 };
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
		if (LuaModule->HasMainMenuItemFn and LuaModule->Enabled and !LuaModule->HasErrored)
		{
			char ErrorStrBuffer[ErrorStrBufferSize] = { 0 };
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
		if (LuaModule->HasSettingsFn and LuaModule->Enabled and !LuaModule->HasErrored)
		{
			char ErrorStrBuffer[ErrorStrBufferSize] = { 0 };
			RunRenderSettings(LuaModule->Filename.c_str(), ErrorStrBuffer, sizeof(ErrorStrBuffer));
			if (strcmp(ErrorStrBuffer, "OK") != 0)
			{
				Logger->PrintWarnArgs("While running 'RenderSettings' in {}: {}", LuaModule->Filename, ErrorStrBuffer);
				LuaModule->HasSettingsFn = false;
			}
		}
	}
}

bool TwinkLuaMgr::ReloadModule(const char* Filename)
{
	std::string DocumentsFolder = GetDocumentsFolder();
	Filesystem::path LuaModulePath = DocumentsFolder + "\\Twinkie\\LuaScripts";

	if (!Filesystem::exists(LuaModulePath))
	{
		Filesystem::create_directories(LuaModulePath);
	}

	LuaModulePath /= Filename;

	if (!Filesystem::exists(LuaModulePath))
	{
		return false;
	}

	for (ILuaModule* LuaModule : LuaModules)
	{
		if (LuaModule->Filename == Filename)
		{
			char ErrorStrBuffer[ErrorStrBufferSize] = { 0 };

			CloseLua(LuaModule->Filename.c_str());
			LuaModule->HasErrored = false;

			InitLua(Filename);
			ExportImGuiForState(Filename);
			SetImGuiContext(ImGui::GetCurrentContext());

			RunLuaFile(Filename, LuaModulePath.string().c_str(), ErrorStrBuffer, sizeof(ErrorStrBuffer));
			if (strcmp(ErrorStrBuffer, "OK") != 0)
			{
				Logger->PrintErrorArgs("Error while reloading lua module {}: {}", Filename, ErrorStrBuffer);
				LuaModule->HasErrored = true;
				return true;
			}

			ModuleInfo* pLuaModuleInfo;
			ModuleInfo LuaModuleInfo;
			pLuaModuleInfo = RunModuleInfo(Filename, ErrorStrBuffer, sizeof(ErrorStrBuffer));
			if (!pLuaModuleInfo)
			{
				Logger->PrintErrorArgs("Error while getting module info for {}: {}", Filename, ErrorStrBuffer);
				delete pLuaModuleInfo;
				LuaModule->HasErrored = true;
				return true;
			}
			LuaModuleInfo = *pLuaModuleInfo;

			LuaModule->LuaModuleInfo = LuaModuleInfo;
			LuaModule->Name = Filename;
			LuaModule->FancyName = LuaModuleInfo.Name;
			LuaModule->Reset();

			delete pLuaModuleInfo;
			return true;
		}
	}
	return true;
}

void TwinkLuaMgr::RenderModuleManager()
{
	using namespace ImGui;
	
	if (Begin("Lua Module Manager", &ModuleManagerOpen))
	{
		if (Button("Reload all"))
		{
			for (ILuaModule* LuaModule : LuaModules)
			{
				delete LuaModule;
			}
			LuaModules.clear();
			GetModulesFromDocuments();
		}
		SameLine();
		if (Button("Disable all"))
		{
			for (ILuaModule* LuaModule : LuaModules)
			{
				LuaModule->Enabled = false;
			}
		}
		SameLine();
		if (Button("Enable all"))
		{
			for (ILuaModule* LuaModule : LuaModules)
			{
				LuaModule->Enabled = true;
				LuaModule->Reset();
			}
		}
		SameLine();
		if (Button("Create module"))
		{
			CreateModule("NewModule");
			for (ILuaModule* LuaModule : LuaModules)
			{
				delete LuaModule;
			}
			LuaModules.clear();
#define EnableReloadedModuleByDefault true
			GetModulesFromDocuments(EnableReloadedModuleByDefault);
		}
		Separator();
		for (size_t Idx = 0; Idx < LuaModules.size(); Idx++)
		{
			ILuaModule* LuaModule = LuaModules[Idx];
			BeginChild(LuaModule->Filename.c_str(), {}, ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AutoResizeX);
			
			if (LuaModule->Filename == "NewModule.lua")
			{
				TextColored({ 1.f, 0.5f, 0.f, 1.f }, ICON_FK_EXCLAMATION_TRIANGLE " %s (%s)", LuaModule->FancyName.c_str(), LuaModule->Filename.c_str());
				if (IsItemHovered())
				{
					BeginTooltip();
					TextColored({ 1.f, 0.5f, 0.f, 1.f }, ICON_FK_EXCLAMATION_TRIANGLE " This is a new module.\nPlease edit the file and its filename\nand set its name and description.\nIf not renamed, it will be overwritten\nwhen a new module is created.");
					EndTooltip();
				}
			}
			else
			{
				Text(ICON_FK_FILE " %s (%s)", LuaModule->FancyName.c_str(), LuaModule->Filename.c_str());
			}

			Text(ICON_FK_USER " by %s", LuaModule->LuaModuleInfo.Author.c_str());
			Text(ICON_FK_BUG " Version: %s", LuaModule->LuaModuleInfo.Version.c_str());
				
			BeginDisabled(!LuaModule->Enabled);

			Text(LuaModule->Enabled ? "Enabled" : "Disabled");

			EndDisabled();

			TextWrapped(LuaModule->LuaModuleInfo.Description.c_str());

			if (LuaModule->HasErrored)
			{
				TextColored({ 1.f, 0.f, 0.f, 1.f }, "Module has caused an exception! Check the logs.");
			}
			if (LuaModule->Enabled)
			{
				if (Button("Reload"))
				{
					if (!ReloadModule(LuaModule->Filename.c_str()))
					{
						for (ILuaModule* LuaModule : LuaModules)
						{
							delete LuaModule;
						}
						LuaModules.clear();
						GetModulesFromDocuments();
						break;
					}
				}
				SameLine();
				if (Button("Disable"))
				{
					LuaModule->Enabled = false;
				}
			}
			else
			{
				if (Button("Enable"))
				{
					LuaModule->Enabled = true;
					LuaModule->Reset();
				}
			}

			EndChild();

			if (Idx % 2 == 0)
			{
				SameLine();
			}
		}
	}
	End();
}

void TwinkLuaMgr::RenderModuleManagerMenuItem()
{
	using namespace ImGui;
	if (MenuItem(ICON_FK_CODE " Lua Modules", "", ModuleManagerOpen))
	{
		ModuleManagerOpen = !ModuleManagerOpen;
	}
}