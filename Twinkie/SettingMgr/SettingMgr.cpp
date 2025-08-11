#include "SettingMgr.h"

Setting::Setting(std::string Name, std::string Value)
{
	this->Name = Name;
	this->Value = Value;
}

void Setting::GetAsVec4(ImVec4* DefaultValue)
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
		return;
	}

	*DefaultValue = { Values[0], Values[1], Values[2], Values[3] };
}

std::vector<float> Setting::GetAsFloats()
{
	// FORMAT: "f,f,f,...,f"
	std::stringstream Stream(Value);
	std::vector<float> Values = {};
	std::string Token = "";

	while (std::getline(Stream, Token, ','))
	{
		Values.push_back(std::stof(Token));
	}

	return Values;
}

void Setting::GetAsBool(bool* DefaultValue)
{
	// FORMAT: "true|false"
	*DefaultValue = Value == "true" ? true : (Value == "false" ? false : *DefaultValue);
}

void Setting::GetAsFloat(float* DefaultValue)
{
	// FORMAT: "f"
	try
	{
		*DefaultValue = Value == "" ? *DefaultValue : std::stof(Value);
	}
	catch (std::exception e)
	{
		return;
	}
}

void Setting::GetAsString(std::string* DefaultValue)
{
	*DefaultValue = Value == "" ? *DefaultValue : Value;
}

void Setting::GetAsInt(int* DefaultValue)
{
	// FORMAT: "i"
	try
	{
		*DefaultValue = Value == "" ? *DefaultValue : std::stoi(Value);
	}
	catch (std::exception e)
	{
		return;
	}
}

void Setting::Set(ImVec4 Value)
{
	this->Value = std::to_string(Value.x) + "," + std::to_string(Value.y) + "," + std::to_string(Value.z) + "," + std::to_string(Value.w);
}

void Setting::Set(bool Value)
{
	this->Value = Value ? "true" : "false";
}

// TODO: turn this into a template (declspec?)
void Setting::Set(float Value)
{
	this->Value = std::to_string(Value);
}

void Setting::Set(int Value)
{
	this->Value = std::to_string(Value);
}

void Setting::Set(std::string Value)
{
	this->Value = Value;
}

void Setting::Set(std::vector<float>& Value)
{
	std::string NewValue = "";

	size_t Idx = 0;
	for (float& IndexedValue : Value)
	{
		NewValue = NewValue + std::to_string(IndexedValue) + (Idx == Value.size() - 1 ? "" : ",");
		Idx++;
	}

	this->Value = NewValue;
}

void Tab::AddSetting(std::string Name, std::string Value)
{
	Settings.push_back(Setting(Name, Value));
}

Setting& Tab::operator[](std::string Name)
{
	for (auto& Setting : Settings)
	{
		if (Setting.Name == Name) return Setting;
	}
	Settings.push_back(Setting(Name, ""));
	return Settings.back();
}

SettingMgr::SettingMgr()
{
	if (!IniFile.read(IniStruct))
	{
		Status = -1;
		GenerateIni();
		if (!IniFile.generate(IniStruct, true))
		{
			Filesystem::create_directories(GetDocumentsFolder() + "\\Twinkie\\");
			IniFile.generate(IniStruct, true);
		}
	}
	else
	{
		LoadIni();
	}
}

Tab& SettingMgr::operator[](std::string Name)
{
	for (auto& Tab : Tabs)
	{
		if (Tab.Name == Name) return Tab;
	}
	Tabs.push_back(Tab(Name));
	return Tabs.back();
}

Setting& SettingMgr::operator()(std::string TabName, std::string SettingName)
{
	Tab& FoundTab = this->operator[](TabName);
	for (auto& Setting : FoundTab.Settings)
	{
		if (Setting.Name == SettingName)
		{
			return Setting;
		}
	}
	FoundTab.Settings.push_back(Setting(SettingName, ""));
	return FoundTab.Settings.back();
}

void SettingMgr::LoadIni()
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

void SettingMgr::GenerateIni()
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

void SettingMgr::Save()
{
	GenerateIni();
	if (!IniFile.write(IniStruct, true))
	{
		Error = IniFile.err;
		Status = -2;
	}
}