#pragma once

#include "imgui-dx9/imgui.h"

#define MINI_CASE_SENSITIVE
#include "mINI/ini.h"

#include <filesystem>
#include <shlobj.h>
#include <shlwapi.h>

namespace Filesystem = std::filesystem;

std::string GetDocumentsFolder()
{
	std::string path;

	char szPath[MAX_PATH + 1] = {};
	if (SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szPath) == S_OK)
		path = szPath;

	return path;
}

class Setting
{
public:
	std::string Name = "";
	std::string Value = "";

	Setting() {}

	Setting(std::string Name, std::string Value)
	{
		this->Name = Name;
		this->Value = Value;
	}

	ImVec4 GetAsVec4()
	{
		// FORMAT: "f,f,f,f"
		std::stringstream Stream(Value);
		std::vector<float> Values = {};
		std::string Token = "";

		while (std::getline(Stream, Token, ','))
		{
			Values.push_back(std::stof(Token));
		}

		if (Values.size() < 4)
		{
			return ImVec4{ 0, 0, 0, -1 };
		}

		return ImVec4{ Values[0], Values[1], Values[2], Values[3] };
	}

	ImVec4 GetAsVec4(ImVec4 DefaultValue)
	{
		// FORMAT: "f,f,f,f"
		std::stringstream Stream(Value);
		std::vector<float> Values = {};
		std::string Token = "";

		while (std::getline(Stream, Token, ','))
		{
			Values.push_back(std::stof(Token));
		}

		if (Values.size() < 4)
		{
			return DefaultValue;
		}

		return ImVec4{ Values[0], Values[1], Values[2], Values[3] };
	}

	std::vector<float> GetAsFloatArray()
	{
		// FORMAT: "i,i,i..."
		std::stringstream Stream(Value);
		std::vector<float> Values = {};
		std::string Token = "";

		while (std::getline(Stream, Token, ','))
		{
			Values.push_back(std::stof(Token));
		}

		return Values;
	}

	bool GetAsBoolFuzzy()
	{
		// FORMAT: "true|false"
		return Value == "true" ? true : false;
	}

	bool GetAsBool(bool DefaultValue)
	{
		// FORMAT: "true|false"
		return Value == "true" ? true : (Value == "false" ? false : DefaultValue);
	}

	float GetAsFloat()
	{
		// FORMAT: "f"
		return std::stof(Value);
	}

	float GetAsFloat(float Default)
	{
		// FORMAT: "f"
		return Value == "" ? Default : std::stof(Value);
	}

	void Set(ImVec4 Value)
	{
		this->Value = std::to_string(Value.x) + "," + std::to_string(Value.y) + "," + std::to_string(Value.z) + "," + std::to_string(Value.w);
	}

	void Set(std::vector<float> Value)
	{
		this->Value = "";
		unsigned int Idx = 0;
		for (auto& Float : Value)
		{
			//                                    more than or equal vv due to possible overflow from subtraction
			this->Value = this->Value + std::to_string(Float) + (Idx >= (Value.size() - 1) ? "" : ",");
			Idx++;
		}
	}

	void Set(bool Value)
	{
		this->Value = Value ? "true" : "false";
	}

	void Set(float Value)
	{
		this->Value = std::to_string(Value);
	}
};

class Tab
{
public:
	std::string Name = "";
	std::vector<Setting> Settings = {};

	Tab() {}

	Tab(std::string Name)
	{
		this->Name = Name;
	}

	void AddSetting(std::string Name, std::string Value)
	{
		Settings.push_back(Setting(Name, Value));
	}

	Setting& operator[](std::string Name)
	{
		for (auto& Setting : Settings)
		{
			if (Setting.Name == Name) return Setting;
		}
		Settings.push_back(Setting(Name, ""));
		return Settings.back();
	}
};

class SettingMgr
{
public:
	std::vector<Tab> Tabs;
	mINI::INIFile IniFile = mINI::INIFile(GetDocumentsFolder() + "\\Twinkie\\Twinkie.ini");
	mINI::INIStructure IniStruct;

	int Status = 0;

	SettingMgr()
	{
		if (!IniFile.read(IniStruct))
		{
			Status = -1;
			GenerateIni();
			if (!IniFile.generate(IniStruct))
			{
				Filesystem::create_directories(GetDocumentsFolder() + "\\Twinkie\\");
				IniFile.generate(IniStruct);
			}
		}
		else
		{
			LoadIni();
		}
	}

	Tab& operator[](std::string Name)
	{
		for (auto& Tab : Tabs)
		{
			if (Tab.Name == Name) return Tab;
		}
		Tabs.push_back(Tab(Name));
		return Tabs.back();
	}

	void LoadIni()
	{
		for (auto& Section : IniStruct)
		{
			std::string SecName = Section.first;
			Tabs.push_back(Tab(SecName));
			for (auto& Value : Section.second)
			{
				Tabs.back().AddSetting(Value.first, Value.second);
			}
		}
	}

	void GenerateIni()
	{
		for (auto& Tab : Tabs)
		{
			auto& Section = IniStruct[Tab.Name];
			for (auto& Setting : Tab.Settings)
			{
				Section[Setting.Name] = Setting.Value;
			}
		}
	}

	void Save()
	{
		GenerateIni();
		IniFile.write(IniStruct);
	}
};