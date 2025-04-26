# Getting started
In order to get started with making your own modules in your build of Twinkie, you need to first understand some basic C++ class inheritance (including virtual functions). Once you understand that, you can go ahead and make a new .h file in the Modules directory.

# `IModule`
`IModule` is the class that all modules inherit from, and your module is required to adhere to the specifications of `IModule` and inherit from it.\
Let's try making a new module called `MyModule`:
```c++
#pragma once

#include "../IModule.h"

class MyModule : public IModule
{
}
```
Now, we can use this module by adding it to the [`std::vector<IModule*> TwinkUi.Modules`](https://github.com/TwinkieTweaks/Twinkie/blob/3cd0b6b7dc8dbe171ed962d503350a81b64d50c0/Twinkie/TwinkUi.h#L88) (after inlcuding our MyModule.h file):
```c++
Modules.push_back(new MyModule(TrackmaniaMgr, Logger, &DoRender));
```
Now, how do we make this module do stuff?
# Virtual functions
Before implementing these functions, remember to make them **`public`**!
## `virtual void Render/RenderInactive/RenderAnyways()`
`Render()` is used to render ImGui/nanovg draw calls while the main UI is visible (`assert(*UiRenderEnabled)`).\
`RenderAnyways()` renders regardless of the current UI visiblity state, but it only render if the module is active (`assert(Enabled)`).\
`RenderInactive()` always renders, even if the module is inactive.
## `virtual void RenderSettings()`
`RenderSettings()` is used to render the inside of the settings tab for the module.
## `virtual void RenderMenuItem()`
`RenderMenuItem()` is called to render the Menu item used to enable/disable the module. Try only using `ImGui::MenuItem` or similar calls when implementing this function.
## `virtual void SettingsInit/SettingsSave(SettingMgr&)`
`SettingsInit(SettingMgr&)` is called when Twinkie is launched, to load the values in the `Twinkie.ini` file used to store modules' settings.\
We will now get into loading and saving settings using `SettingMgr`.
### `.ini`?
`.ini` files are structured this way:
```ini
[My Module]
My String = hello guys
My Vec4 = 1.0,1.0,0.5,3.14159
My Bool = false
```
where all values are loaded as strings by default, however Twinkie is able to understand these as the datatypes you request.
### Loading
To get the `MyModule` set of values from the above example `.ini` file:
```c++
virtual void SettingsInit(SettingMgr& Settings)
{
  Tab& MyModuleTab = Settings["My Module"];
}
```
The reason for naming the class `Tab` is for future purposes, where Tabs and Settings will be directly rendered instead of being managed by the module.\
Now that we have the Tab ready for use, we can access our values:
```c++
Tab& MyModuleTab = Settings["My Module"];
Setting& MyStringSetting = MyModuleTab["My String"];
```
Since it's a string, you can directly use the `std::string Setting.Value` to get the value you want.\
Or if you don't want the possiblity of the string being empty by default, you can use the `void Setting.GetAsString(std::string*)` which will not overwrite the value living at the provided pointer if the string is empty.
```c++
Setting& MyVec4Setting = MyModuleTab["My Vec4"];
ImVec4 Vector4 = ImVec4(0.0, 1.0, 0.5, 0.0);
MyVec4Setting.GetAsVec4(&Vector4);
```
Again, this will not overwrite the value of `Vector4` if the setting does not exist/is an empty string.
### Saving
You can use the `Setting.Set(auto)` function to set a setting to a specified value.
```c++
Settings["My Module"]["My Vec4"].Set(Vector4);
```
Do keep in mind that the `operator[]` on the `SettingMgr` and `Tab` will create the `Tab` or `Setting` respectively if they don't exist at all.
# Flags
There are some custom flags you can set to your module:
- `virtual bool IsDebug()`: this makes all `RenderMenuItem` calls render inside the Debug menu item in the bar.
- `virtual bool HasSettings()`: this makes the module not have a tab at all in the settings menu.
