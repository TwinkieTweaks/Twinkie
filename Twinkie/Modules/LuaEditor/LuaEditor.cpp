#include "LuaEditor.h"

namespace Filesystem = std::filesystem;
inline std::string AlsoGetDocumentsFolder()
{
    std::string path;

    char szPath[MAX_PATH + 1] = {};
    if (SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szPath) == S_OK)
        path = szPath;

    return path;
}

void LuaEditorModule::Render()
{
    using namespace ImGui;
    Begin(ICON_FK_CODE " Lua Editor", &Enabled);

    char ErrorBuffer[512];
    memset(ErrorBuffer, 0, 512);

    std::string ActiveLuaScript = "Script.lua";

    if ((Button("Run") and !LuaFileInitialized) or LuaFileReload)
    {
        Logger->PrintArgs("Running lua script {}", ActiveLuaScript);
        LuaErrorOccured = false;
        RunLuaFile(std::string(AlsoGetDocumentsFolder() + "\\Twinkie\\LuaScripts\\" + ActiveLuaScript).c_str(), ErrorBuffer, 512);
        LuaFileInitialized = true;
        LuaFileReload = false;
        if (strcmp(ErrorBuffer, "OK") != 0)
        {
            Logger->PrintErrorArgs("Error while running lua script: {}", ErrorBuffer);
            Logger->PrintArgs("Stopping lua script {}", ActiveLuaScript);
            LuaErrorOccured = true;
            LuaFileInitialized = false;
            LuaFileReload = false;
            lua_settop(GetLuaState(), 0);
        }
    }

    SameLine();

    if (Button("Stop") and LuaFileInitialized)
    {
        Logger->PrintArgs("Stopping lua script {}", ActiveLuaScript);
        LuaFileInitialized = false;
        LuaErrorOccured = false;
        End();
        return;
    }

    SameLine();

    if (Button("Reload") and LuaFileInitialized)
    {
        Logger->PrintArgs("Stopping lua script {}", ActiveLuaScript);
        LuaFileReload = true;
        LuaErrorOccured = false;
        End();
        return;
    }

    if (LuaFileInitialized and !LuaErrorOccured)
    {
        LuaErrorOccured = false;
        RunRender(ErrorBuffer, 512);
        if (strcmp(ErrorBuffer, "OK") != 0)
        {
            Logger->PrintErrorArgs("Error while running lua script: {}", ErrorBuffer);
            Logger->PrintArgs("Stopping lua script {}", ActiveLuaScript);
            LuaErrorOccured = true;
            LuaFileInitialized = false;
            LuaFileReload = false;
            lua_settop(GetLuaState(), 0);
        }
    }

    End();
}