#pragma once

#include "imgui-dx9/imgui.h"

#define MINI_CASE_SENSITIVE
#include "mINI/ini.h"

#include <filesystem>

namespace Filesystem = std::filesystem;

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

	std::vector<int> GetAsIntArray()
	{
		// FORMAT: "i,i,i..."
		std::stringstream Stream(Value);
		std::vector<int> Values = {};
		std::string Token = "";

		while (std::getline(Stream, Token, ','))
		{
			Values.push_back(std::stoi(Token));
		}

		return Values;
	}

	bool GetAsBool()
	{
		// FORMAT: "true|false"
		return Value == "true" ? true : false;
	}

	float GetAsFloat()
	{
		// FORMAT: "f"
		return std::stof(Value);
	}

	void Set(ImVec4 Value)
	{
		this->Value = std::to_string(Value.x) + "," + std::to_string(Value.y) + "," + std::to_string(Value.z) + "," + std::to_string(Value.w);
	}

	void Set(std::vector<int> Value)
	{
		this->Value = "";
		for (auto& Int : Value)
		{
			this->Value = this->Value + std::to_string(Int) + ",";
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
	mINI::INIFile IniFile = mINI::INIFile("Twinkie/Twinkie.ini");
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
				Filesystem::create_directories("Twinkie");
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