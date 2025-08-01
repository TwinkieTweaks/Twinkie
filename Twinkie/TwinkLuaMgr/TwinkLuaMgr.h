#pragma once
#include <vector>
#include "ILuaModule.h"

class TwinkLuaMgr
{
public:
	std::vector<ILuaModule*> LuaModules = {};
	TwinkLogs* Logger = nullptr;

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

	void GetModulesFromDocuments();
	void RunModulesRender();
	void RunModulesRenderMenuItem();
	void RunModulesRenderMainMenuItem();
	void RunModulesRenderSettings();
	void ReloadModule(const char* Filename);
};