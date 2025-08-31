#pragma once

#include "../imgui-dx9/imgui.h"

#define MINI_CASE_SENSITIVE
#include "../mINI/ini.h"

#include <filesystem>
#include <shlobj.h>
#include <shlwapi.h>

inline std::string WStringToUTF8(const std::wstring& wstr)
{
	if (wstr.empty()) return {};

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size() + 1, &strTo[0], size_needed, nullptr, nullptr);
	return strTo;
}

namespace Filesystem = std::filesystem;
inline std::string GetDocumentsFolder()
{
	CoInitialize(NULL);
	PWSTR path = nullptr;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);
	std::string result;

	if (SUCCEEDED(hr) && path != nullptr)
	{
		std::wstring widePath(path);
		result = WStringToUTF8(widePath);
		CoTaskMemFree(path);
	}
	else
	{
		return "";
	}

	CoUninitialize();

	return result;
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