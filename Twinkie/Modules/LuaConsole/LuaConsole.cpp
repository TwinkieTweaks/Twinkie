#include "LuaConsole.h"

#ifdef BUILD_DEBUG
namespace Filesystem = std::filesystem;
inline std::string AlsoGetDocumentsFolder()
{
    std::string path;

    char szPath[MAX_PATH + 1] = {};
    if (SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szPath) == S_OK)
        path = szPath;

    return path;
}

void LuaConsoleModule::Render()
{
    using namespace ImGui;
    Begin(ICON_FK_CODE " Lua Console", &Enabled);

    char ErrorBuffer[512];
    memset(ErrorBuffer, 0, 512);

    char LuaStringBuffer[512];
    memset(LuaStringBuffer, 0, 512);

    BeginChild("LuaConsoleScrollable", { 0, 0 }, ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysAutoResize);

    std::istringstream Stream(ConsoleStr);
    std::string Line;
    while (std::getline(Stream, Line))
    {
        Text(Line.c_str());
    }

    EndChild();

    InputText("##LuaInputString", LuaStringBuffer, 512);
    SameLine();
    if (Button("Run"))
    {
        ConsoleStr = ConsoleStr + ">>> " + LuaStringBuffer + "\n";
        RunLua("TwinkieLuaConsole", LuaStringBuffer, ErrorBuffer, 512);
        if (strcmp(ErrorBuffer, "OK") != 0)
        {
            Logger->PrintErrorArgs("Error while running lua string: {}", ErrorBuffer);
            lua_settop(GetLuaState("TwinkieLuaConsole"), 0);
        }
    }

    End();
}
#endif