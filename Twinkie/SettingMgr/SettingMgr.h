#pragma once

#include "../imgui-dx9/imgui.h"

#define MINI_CASE_SENSITIVE
#include "../mINI/ini.h"

#include <filesystem>
#include <shlobj.h>
#include <shlwapi.h>

namespace Filesystem = std::filesystem;
inline std::string GetDocumentsFolder()
{
	std::string path;

	char szPath[MAX_PATH + 1] = {};
	if (SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szPath) == S_OK)
		path = szPath;

	return path;
}

class __declspec(dllexport) Setting
{
public:
	std::string Name = "";
	std::string Value = "";

	Setting() {}

	Setting(std::string Name, std::string Value);

	void GetAsVec4(ImVec4* DefaultValue);

	std::vector<float> GetAsFloats();
	void GetAsBool(bool* DefaultValue);
	void GetAsFloat(float* DefaultValue);
	void GetAsString(std::string* DefaultValue);
	void GetAsInt(int* DefaultValue);

	void Set(ImVec4 Value);
	void Set(bool Value);
	void Set(float Value);
	void Set(int Value);
	void Set(std::string Value);
	void Set(std::vector<float>& Value);
};

class __declspec(dllexport) Tab
{
public:
	std::string Name = "";
	std::vector<Setting> Settings = {};

	Tab() {}
	Tab(std::string Name)
	{
		this->Name = Name;
	}

	void AddSetting(std::string Name, std::string Value);
	Setting& operator[](std::string Name);
};

class __declspec(dllexport) SettingMgr
{
public:
	std::vector<Tab> Tabs;
	mINI::INIFile IniFile = mINI::INIFile(GetDocumentsFolder() + "\\Twinkie\\Twinkie.ini");
	mINI::INIStructure IniStruct;
	std::string Error = "";

	int Status = 0;

	SettingMgr();

	Tab& operator[](std::string Name);
	Setting& operator()(std::string TabName, std::string SettingName);

	void LoadIni();
	void GenerateIni();
	void Save();
};