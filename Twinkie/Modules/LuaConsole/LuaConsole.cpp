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

    static char LuaStringBuffer[512];
    static char ErrorBuffer[512];
    static bool AreBuffersZeroed = false;

    if (!AreBuffersZeroed)
    {
        memset(LuaStringBuffer, 0, 512);
        memset(ErrorBuffer, 0, 512);
        AreBuffersZeroed = true;
    }

    if (!LuaInited("TwinkieLuaConsole"))
    {
        InitLua("TwinkieLuaConsole");
    }

    Begin(ICON_FK_CODE " Lua Console", &Enabled);

    BeginChild("LuaConsoleScrollable", { 0, 0 }, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY);

    std::istringstream Stream(g_LuaConsoleModuleOutputStr);
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
        g_LuaConsoleModuleOutputStr = g_LuaConsoleModuleOutputStr + ">>> " + LuaStringBuffer + "\n";
        RunLua("TwinkieLuaConsole", LuaStringBuffer, ErrorBuffer, 512);
        if (strcmp(ErrorBuffer, "OK") != 0)
        {
            Logger->PrintErrorArgs("Error while running lua string: {}", ErrorBuffer);
            memset(ErrorBuffer, 0, 512);
        }
    }

    End();
}
#endif