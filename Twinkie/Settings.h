#pragma once

#include <typeinfo>
#include <map>
#include <string>
#include <vector>
#include "imgui-dx9/imgui.h"
#include "mINI/ini.h"

class Setting
{
public:
	std::string Name = "";
	std::string Type = "";
	bool bValue = 0;
	ImVec4 v4Value = {};
	bool IsValid = false;

	Setting(std::string pName)
	{
		Name = pName;
		IsValid = true;
	}

	Setting(){}

	operator bool& ()
	{
		return IsValid;
	}
};

class Tab
{
public:
	std::string Name = "";
	bool IsValid = true;
	std::vector<Setting> Settings{};

	Tab(std::string pName) { Name = pName; }

	Setting& operator[](unsigned int rhs)
	{
		return Settings[rhs];
	}

	Setting& operator[](std::string rhs)
	{
		Setting SettingR("");
		for (auto& SettingV : Settings)
		{
			if (SettingV.Name == rhs)
			{
				return SettingV;
			}
		}
		SettingR.IsValid = false;
		return SettingR;
	}

	void Render(std::string Name, std::map<std::string, std::string>& VarToName)
	{
		using namespace ImGui;

		BeginTabItem(Name.c_str());

		for (auto& SettingV : Settings)
		{
			std::string CName = VarToName[SettingV.Name];
			if (SettingV.Type == "bool")
			{
				Checkbox(CName.c_str(), &SettingV.bValue);
			}
			else if (SettingV.Type == "vec4")
			{
				ColorEdit4(CName.c_str(), (float*)&SettingV.v4Value);
			}
			else
			{
				continue;
			}
		}

		EndTabItem();
	}
};

enum PopulationReason
{
	NoPopulation = 0,
	FileNotReady = 1,
	NoTwinkieSec = 2,
	NoVersionSec = 3,
	VersionNot3C = 4,
	TypeNotFound = 5
};

class SettingMgr
{
public:
	std::vector<Tab> Tabs{};
	std::vector<bool> OpenTabs{};
	mINI::INIFile IniFile = mINI::INIFile("Twinkie.ini");
	mINI::INIStructure IniStruct;

	std::map<std::string, std::string> VarToName{};
	std::map<std::string, std::string> SecToName{};

	PopulationReason Reason = NoPopulation;

	SettingMgr()
	{
		// check if our INI file is valid
		if (!IniFile.read(IniStruct))
		{
			Reason = FileNotReady;
			return;
		}

		if (!IniStruct.has("Twinkie"))
		{
			Reason = NoTwinkieSec;
			return;
		}

		if (!IniStruct["Twinkie"].has("Version"))
		{
			Reason = NoVersionSec;
			return;
		}

		if (IniStruct["Twinkie"]["Version"] != ":3c")
		{
			Reason = VersionNot3C;
			return;
		}

		// ini file is valid, time to load stuffs

		std::map<std::string, std::string> NameToType{};

		for (auto& Section : IniStruct)
		{
			if (Section.first == "types")
			{
				for (auto& Value : Section.second)
				{
					NameToType[Value.first] = Value.second;
				}
			}
		}

		for (auto& Section : IniStruct)
		{
			if (Section.first == "vnames")
			{
				for (auto& Value : Section.second)
				{
					VarToName[Value.first] = Value.second;
				}
			}
		}

		for (auto& Section : IniStruct)
		{
			if (Section.first == "cnames")
			{
				for (auto& Value : Section.second)
				{
					SecToName[Value.first] = Value.second;
				}
			}
		}

		for (auto& Section : IniStruct)
		{
			// Section here is a std::pair<std::string, std::pair<std::string, std::string>>
			// first gets the str
			// second gets the section itself

			if (!IsSecUsable(Section.first)) continue;

			Tab NewTab = Tab(Section.first);

			for (auto& Value : NameToType)
			{
				Setting NewSetting = Setting(Value.first);

				if (!NameToType.contains(NewSetting.Name))
				{
					if (NewSetting.Name.find(".") == std::string::npos)
					{
						Reason = TypeNotFound;
						return;
					}
					else
					{
						if (!NameToType.contains(NewSetting.Name + ".x"))
						{
							Reason = TypeNotFound;
							return;
						}
					}
				}

				std::string Type = NameToType[NewSetting.Name];

				if (Type == "bool")
				{
					NewSetting.bValue = Section.second.get(NewSetting.Name) == "true" ? true : false;
				}
				else if (Type == "vec4")
				{
					NewSetting.v4Value.x = atof(Section.second.get(NewSetting.Name + ".x").c_str());
					NewSetting.v4Value.y = atof(Section.second.get(NewSetting.Name + ".y").c_str());
					NewSetting.v4Value.z = atof(Section.second.get(NewSetting.Name + ".z").c_str());
					NewSetting.v4Value.w = atof(Section.second.get(NewSetting.Name + ".w").c_str());
				}

				NewSetting.Type = Type;

				NewTab.Settings.push_back(NewSetting);
			}

			NewTab.Name = Section.first;

			Tabs.push_back(NewTab);
		}
	}

	Setting& Index(unsigned int TabIdx, unsigned int SettingIdx)
	{
		return Tabs[TabIdx].Settings[SettingIdx];
	}

	bool IsSecUsable(std::string Name)
	{
		return !(Name == "twinkie" or Name == "types" or Name == "vnames" or Name == "cnames");
	}

	void PopulateIniStruct()
	{
		IniStruct["Twinkie"]["Version"] = ":3c";

		for (auto& TabV : Tabs)
		{
			IniStruct["cnames"][TabV.Name] = SecToName[TabV.Name];
			for (auto& SettingV : TabV.Settings)
			{
				IniStruct["vnames"][SettingV.Name] = VarToName[SettingV.Name];
				if (SettingV.Type == "bool")
				{
					IniStruct[TabV.Name][SettingV.Name] = SettingV.bValue ? "true" : "false";

					IniStruct["types"][SettingV.Name] = "bool";
				}
				else if (SettingV.Type == "vec4")
				{
					float xv = SettingV.v4Value.x;
					std::string x = std::to_string(xv);
					float yv = SettingV.v4Value.y;
					std::string y = std::to_string(yv);
					float zv = SettingV.v4Value.z;
					std::string z = std::to_string(zv);
					float wv = SettingV.v4Value.w;
					std::string w = std::to_string(wv);

					IniStruct[TabV.Name][SettingV.Name + ".x"] = x;
					IniStruct[TabV.Name][SettingV.Name + ".y"] = y;
					IniStruct[TabV.Name][SettingV.Name + ".z"] = z;
					IniStruct[TabV.Name][SettingV.Name + ".w"] = w;

					IniStruct["types"][SettingV.Name] = "vec4";
				}
				else
				{
					continue;
				}
			}
		}

		IniFile.generate(IniStruct, true);
	}

	Setting& operator[](std::string rhs)
	{
		Setting SettingR("");
		for (auto& TabV : Tabs)
		{
			if (TabV[rhs])
			{
				return TabV[rhs];
			}
		}
		SettingR.IsValid = false;
		return SettingR;
	}

	Setting Get(std::string Name, std::string Type)
	{
		Setting SettingR("");
		for (auto& TabV : Tabs)
		{
			if (TabV[Name])
			{
				if (TabV[Name].Type == Type)
					return TabV[Name];
			}
		}
		SettingR.IsValid = false;
		return SettingR;
	}

	void Render()
	{
		using namespace ImGui;

		BeginTabBar("##TwinkieTabBar");

		for (auto& TabV : Tabs)
		{
			if (!IsSecUsable(TabV.Name))
				continue;
			TabV.Render(SecToName[TabV.Name], VarToName);
		}

		EndTabBar();
	}
};