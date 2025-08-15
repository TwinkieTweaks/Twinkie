#pragma once
#include <vector>
#include "ILuaModule.h"

extern bool SetWorkingDirectory(const std::string& Directory);

class TwinkLuaMgr
{
public:
	std::vector<ILuaModule*> LuaModules = {};
	TwinkLogs* Logger = nullptr;
	bool ModuleManagerOpen = false;

	TwinkLuaMgr(TwinkLogs* Logger)
	{
		this->Logger = Logger;
	}

	~TwinkLuaMgr()
	{
		for (auto Module : LuaModules)
		{
			CloseLua(Module->Filename.c_str());
			delete Module;
			Module = nullptr;
		}
	}

	void CreateModule(const char* Filename);

	void GetModulesFromDocuments(bool EnableByDefault = false);
	void RunModulesRender();
	void RunModulesRenderInterface();
	void RunModulesRenderMenuItem();
	void RunModulesRenderMainMenuItem();
	void RunModulesRenderSettings();
	bool ReloadModule(const char* Filename);
	void RenderModuleManager();
	void RenderModuleManagerMenuItem();
};