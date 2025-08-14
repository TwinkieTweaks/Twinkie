#include "LuaConsole.h"
#include "../../Version.h"
#include "../../imgui-dx9/imgui_internal.h"
#include <cstdlib>

#define LuaConsoleStateName "TwinkieLuaConsole"
#define LuaConsoleStrBootup "Lua console for Twinkie " TwinkieVersion "\nRunning " LUA_VERSION "\nAvailable globals: App\nAvailable functions: print(), clear(), cls()\n"

extern void AddToLuaPackagePath(lua_State* L, const std::string& Directory);

extern "C"
{
    int LuaConsolePrintOverride(lua_State* L)
    {
        int NumberOfElems = lua_gettop(L);
        for (int Idx = 1; Idx <= NumberOfElems; Idx++)
        {
            const char* LuaStr = lua_tostring(L, Idx);
            if (LuaStr)
            {
                g_LuaConsoleModuleOutputStr = g_LuaConsoleModuleOutputStr + LuaStr;
            }
            else
            {
                if (lua_isnil(L, Idx))
                {
                    g_LuaConsoleModuleOutputStr = g_LuaConsoleModuleOutputStr + "nil";
                }
                else if (lua_isboolean(L, Idx))
                {
                    g_LuaConsoleModuleOutputStr = g_LuaConsoleModuleOutputStr + (lua_toboolean(L, Idx) ? "true" : "false");
                }
            }
            if (Idx < NumberOfElems) g_LuaConsoleModuleOutputStr = g_LuaConsoleModuleOutputStr + "\t";
        }
        g_LuaConsoleModuleOutputStr = g_LuaConsoleModuleOutputStr + "\n";
        return 0;
    }

    int LuaConsoleClear(lua_State* L)
    {
        g_LuaConsoleModuleOutputStr = LuaConsoleStrBootup;
        g_LuaConsoleModulePreviousStatements.clear();
        return 0;
    }
}

void LuaConsoleModule::Render()
{
    using namespace ImGui;

    bool EnterPressed = false;

    if (!AreBuffersZeroed)
    {
        memset(LuaStringBuffer, 0, StringBufferMaxSize);
        memset(ErrorBuffer, 0, StringBufferMaxSize);
        AreBuffersZeroed = true;
    }

    if (!LuaInited(LuaConsoleStateName))
    {
        g_LuaConsoleModuleOutputStr = LuaConsoleStrBootup;
        InitLua(LuaConsoleStateName);
        AddToLuaPackagePath(GetLuaState(LuaConsoleStateName), ";" + GetDocumentsFolder() + "\\Twinkie\\LuaScripts\\?.lua");
        SetLuaPrintFunctionForState(LuaConsoleStateName, LuaConsolePrintOverride);
        UpdatePrintFunction(LuaConsoleStateName);

        lua_pushcfunction(GetLuaState(LuaConsoleStateName), LuaConsoleClear);
        lua_setglobal(GetLuaState(LuaConsoleStateName), "clear");
        lua_getglobal(GetLuaState(LuaConsoleStateName), "clear");
        lua_setglobal(GetLuaState(LuaConsoleStateName), "cls");
    }

    Begin(ICON_FK_CODE " Lua Console", &Enabled);

    BeginChild("LuaConsoleScrollable", { 0, 0 }, ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY);

    std::istringstream Stream(g_LuaConsoleModuleOutputStr);
    std::string Line;
    while (std::getline(Stream, Line))
    {
        TextUnformatted(Line.c_str());
    }

    EndChild();

    if (PreviousFrameWantsTextInputFocus)
    {
        PreviousFrameWantsTextInputFocus = false;
#define NextInputTextItem 0
        SetKeyboardFocusHere(NextInputTextItem);
    }

    if (!WasPreviouslyWrittenStatementCopied)
    {
        InputTextID = rand();
        WasPreviouslyWrittenStatementCopied = true;
        SetKeyboardFocusHere(NextInputTextItem);
    }

    PushID(InputTextID);
    InputText("##LuaInputString", LuaStringBuffer, StringBufferMaxSize);
    PopID();

    if (IsItemFocused())
    {
#define NoRepeat false
#define StringsAreEqual 0
        if (IsKeyPressed(ImGuiKey_Enter, NoRepeat) and strcmp(LuaStringBuffer, "") != StringsAreEqual)
        {
            EnterPressed = true;
            PreviousFrameWantsTextInputFocus = true;
            SetNextFrameWantCaptureKeyboard(true);
        }
        if (IsKeyPressed(ImGuiKey_UpArrow, NoRepeat))
        {
            PreviouslyWrittenStatementIndex++;
            WasPreviouslyWrittenStatementCopied = false;
        }
        if (IsKeyPressed(ImGuiKey_DownArrow, NoRepeat))
        {
            PreviouslyWrittenStatementIndex--;
            WasPreviouslyWrittenStatementCopied = false;
        }
    }

    if (PreviouslyWrittenStatementIndex < -1)
    {
        PreviouslyWrittenStatementIndex = -1;
    }
    if (PreviouslyWrittenStatementIndex >= (int)g_LuaConsoleModulePreviousStatements.size())
    {
        PreviouslyWrittenStatementIndex = ((int)g_LuaConsoleModulePreviousStatements.size()) - 1;
    }

    if (!WasPreviouslyWrittenStatementCopied and g_LuaConsoleModulePreviousStatements.size() != 0)
    {
        if (PreviouslyWrittenStatementIndex != -1)
        {
            size_t PreviousStatementsLength = g_LuaConsoleModulePreviousStatements.size() - 1;
            std::string PreviousStatement = g_LuaConsoleModulePreviousStatements[PreviousStatementsLength - PreviouslyWrittenStatementIndex];
            strcpy_s(LuaStringBuffer, StringBufferMaxSize, PreviousStatement.c_str());
        }
        else
        {
            // strcpy_s(LuaStringBuffer, StringBufferMaxSize, "");
            LuaStringBuffer[0] = '\0';
        }
    }

    SameLine();
    if (Button("Run") or EnterPressed)
    {
        g_LuaConsoleModulePreviousStatements.push_back(LuaStringBuffer);
        g_LuaConsoleModuleOutputStr = g_LuaConsoleModuleOutputStr + "> " + LuaStringBuffer + "\n";
        RunLua(LuaConsoleStateName, LuaStringBuffer, ErrorBuffer, StringBufferMaxSize);
        if (strcmp(ErrorBuffer, "OK") != StringsAreEqual)
        {
            g_LuaConsoleModuleOutputStr = g_LuaConsoleModuleOutputStr + ErrorBuffer + "\n";
            memset(ErrorBuffer, 0, StringBufferMaxSize);
        }
        AreBuffersZeroed = false;
        EnterPressed = false;
        PreviouslyWrittenStatementIndex = -1;
    }

    End();
}