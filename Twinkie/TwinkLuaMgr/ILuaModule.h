#pragma once
#include "../LuaEngine/LuaEngine2.h"
#include "../IModule.h"

class ILuaModule : public IModule
{
public:
	std::string Filename = "";
	ModuleInfo LuaModuleInfo;

	bool HasErrored = false;

	bool HasRenderFn = true;
	bool HasRenderSettingsFn = true;
	bool HasRenderMainMenuItemFn = true;
	bool HasRenderMenuItemFn = true;

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

		HasRenderFn = true;
		HasRenderSettingsFn = true;
		HasRenderMainMenuItemFn = true;
		HasRenderMenuItemFn = true;
	}

	ILuaModule() = default;

	virtual ~ILuaModule() = default;
};