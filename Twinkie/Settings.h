#pragma once

#include <typeinfo>
#include <string>
#include <vector>
#include "imgui-dx9/imgui.h"
#include "mINI/ini.h"

class Setting
{
public:
	std::string Name = "";
	void* Value = nullptr;
	bool IsValid = false;

	Setting(std::string pName, void* pValue)
	{
		Name = pName;
		Value = pValue;
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

	void AddSetting(std::string SettingName, void* SettingValue)
	{
		Settings.push_back(Setting(SettingName, SettingValue));
	}

	void* GetSetting(std::string SettingName)
	{
		for (auto& SettingV : Settings)
		{
			if (SettingV.Name == SettingName)
			{
				return SettingV.Value;
			}
		}
		return nullptr;
	}

	template <typename T>
	T* GetSetting(std::string SettingName)
	{
		for (auto& SettingV : Settings)
		{
			if (SettingV.Name == SettingName)
			{
				return (T*)SettingV.Value;
			}
		}
		return nullptr;
	}
	
	void SetSetting(std::string SettingName, void* SettingValue)
	{
		for (auto& SettingV : Settings)
		{
			if (SettingV.Name == SettingName)
			{
				SettingV.Value = SettingValue;
			}
		}
	}

	Setting& operator[](unsigned int rhs)
	{
		return Settings[rhs];
	}

	Setting& operator[](std::string rhs)
	{
		Setting SettingR("", nullptr);
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

	void Render()
	{
		using namespace ImGui;

		BeginTabItem(Name.c_str());

		for (auto& SettingV : Settings)
		{
			const char* CName = SettingV.Name.c_str();
			void* Val = SettingV.Value;
			if (typeid(SettingV.Value) == typeid(bool*))
			{
				Checkbox(CName, (bool*)Val);
			}
			else if (typeid(SettingV.Value) == typeid(ImVec4*))
			{
				ColorEdit4(CName, (float*)Val);
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
	UnableToRead = 1,
	NoTwinkieSec = 2,
	NoVersionSec = 3,
	VersionNot3C = 4,
	TypeNotFound = 5
};

class SettingMgr
{
public:
	std::vector<Tab> Tabs{};
	mINI::INIFile IniFile = mINI::INIFile("Twinkie.ini");
	mINI::INIStructure IniStruct;
	PopulationReason Reason = NoPopulation;

	SettingMgr()
	{
		// check if our INI file is valid
		if (!IniFile.read(IniStruct))
		{
			PopulateIniStruct();
			Reason = UnableToRead;
			return;
		}

		if (!IniStruct.has("Twinkie"))
		{
			PopulateIniStruct();
			Reason = NoTwinkieSec;
			return;
		}

		if (!IniStruct["Twinkie"].has("Version"))
		{
			PopulateIniStruct();
			Reason = NoVersionSec;
			return;
		}

		if (IniStruct["Twinkie"]["Version"] != ":3c")
		{
			PopulateIniStruct();
			Reason = VersionNot3C;
			return;
		}

		// ini file is valid, time to load stuffs

		std::unordered_map<std::string, std::string> NameToType{};

		for (auto& Section : IniStruct)
		{
			if (Section.first == "types")
			{
				for (auto& Value : Section.second)
				{
					static int Idx = 0;
					NameToType[Value.first] = Value.second;
					Idx++;
				}
			}
		}

		for (auto& Section : IniStruct)
		{
			// Section here is a std::pair<std::string, INIMap>
			// first gets the str
			// second gets the section itself

			if (Section.first == "twinkie") continue;

			Tab NewTab = Tab(Section.first);

			for (auto& Value : Section.second)
			{
				Setting NewSetting = Setting(Value.first, nullptr);

				if (!NameToType.contains(NewSetting.Name))
				{
					if (NewSetting.Name.find(".") == std::string::npos)
					{
						PopulateIniStruct();
						Reason = TypeNotFound;
						return;
					}
					else
					{
						if (!NameToType.contains(NewSetting.Name + ".x"))
						{
							PopulateIniStruct();
							Reason = TypeNotFound;
							return;
						}
					}
				}

				std::string Type = NameToType[NewSetting.Name];

				if (Type == "bool")
				{
					NewSetting.Value = new bool{Value.second == "true" ? true : false};
				}
				else if (Type == "vec4")
				{
					NewSetting.Value = new ImVec4{}; // TODO
				}

				NewTab.Settings.push_back(NewSetting);
			}

			Tabs.push_back(NewTab);
		}
	}

	void PopulateIniStruct()
	{
		IniStruct["Twinkie"]["Version"] = ":3c";

		for (auto& TabV : Tabs)
		{
			for (auto& SettingV : TabV.Settings)
			{
				if (typeid(SettingV.Value) == typeid(bool*))
				{
					IniStruct[TabV.Name][SettingV.Name] = *(bool*)SettingV.Value ? "true" : "false";

					IniStruct["Types"][SettingV.Name] = "bool";
				}
				else if (typeid(SettingV.Value) == typeid(ImVec4*))
				{
					float xv = ((ImVec4*)SettingV.Value)->x;
					std::string x = std::to_string(xv);
					float yv = ((ImVec4*)SettingV.Value)->y;
					std::string y = std::to_string(yv);
					float zv = ((ImVec4*)SettingV.Value)->z;
					std::string z = std::to_string(zv);
					float wv = ((ImVec4*)SettingV.Value)->w;
					std::string w = std::to_string(wv);

					IniStruct[TabV.Name][SettingV.Name + ".x"] = x;
					IniStruct[TabV.Name][SettingV.Name + ".y"] = y;
					IniStruct[TabV.Name][SettingV.Name + ".z"] = z;
					IniStruct[TabV.Name][SettingV.Name + ".w"] = w;

					IniStruct["Types"][SettingV.Name] = "vec4";
				}
				else
				{
					continue;
				}
			}
		}

		IniFile.generate(IniStruct, true);
	}

	~SettingMgr()
	{
		for (auto& TabV : Tabs)
		{
			for (auto& SettingV : TabV.Settings)
			{
				delete SettingV.Value;
				SettingV.Value = nullptr;
			}
		}
	}

	Setting& operator[](std::string rhs)
	{
		Setting SettingR("", nullptr);
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

	void Render()
	{
		using namespace ImGui;

		BeginTabBar("##TwinkieTabBar");

		for (auto& TabV : Tabs)
		{
			TabV.Render();
		}

		EndTabBar();
	}
};