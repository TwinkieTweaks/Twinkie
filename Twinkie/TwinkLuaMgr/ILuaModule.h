#pragma once
#include "../LuaEngine/LuaEngine2.h"
#include "../IModule.h"

class ILuaModule : public IModule
{
public:
	std::string Filename = "";
	ModuleInfo LuaModuleInfo;

	bool HasErrored = false;

	bool HasSettingsFn = true;
	bool HasMainMenuItemFn = true;
	bool HasMenuItemFn = true;

	bool ChecksumValid = false;

	ILuaModule(TwinkTrackmania& Twinkie, TwinkLogs& Logger, const bool* UiRenderEnabled, const char* Filename, ModuleInfo* LuaModuleInfo)
	{
		this->UiRenderEnabled = UiRenderEnabled;
		this->Twinkie = &Twinkie;
		this->Logger = &Logger;
		this->Filename = Filename;
		this->LuaModuleInfo = *LuaModuleInfo;
	}

	ILuaModule(const char* Filename, ModuleInfo* LuaModuleInfo)
	{
		this->Filename = Filename;
		this->LuaModuleInfo = *LuaModuleInfo;
	}

	void Reset()
	{
		HasErrored = false;

		HasSettingsFn = true;
		HasMainMenuItemFn = true;
		HasMenuItemFn = true;
	}

	ILuaModule() = default;

	virtual ~ILuaModule() = default;
};