#include "LuaConsole.h"
#include "../../Version.h"
#include "../../imgui-dx9/imgui_internal.h"

#define StringBufferMaxSize 512
#define LuaConsoleStateName "TwinkieLuaConsole"
#define LuaConsoleStrBootup "Lua console for Twinkie " TwinkieVersion "\nRunning Lua " LuaVersion "\nAvailable globals: App\nAvailable functions: print(), clear(), cls()\n"
#ifdef BUILD_DEBUG

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
                g_LuaConsoleModuleOutputStr = g_LuaConsoleModuleOutputStr + lua_typename(L, Idx);
            }
            if (Idx < NumberOfElems) g_LuaConsoleModuleOutputStr = g_LuaConsoleModuleOutputStr + "\t";
        }
        g_LuaConsoleModuleOutputStr = g_LuaConsoleModuleOutputStr + "\n";
        return 0;
    }

    int LuaConsoleClear(lua_State* L)
    {
        g_LuaConsoleModuleOutputStr = LuaConsoleStrBootup;
        return 0;
    }
}

void LuaConsoleModule::Render()
{
    using namespace ImGui;

    static char LuaStringBuffer[StringBufferMaxSize];
    static char ErrorBuffer[StringBufferMaxSize];
    static bool AreBuffersZeroed = false;

    static bool PreviousFrameWantsTextInputFocus = false;
    static int PreviouslyWrittenStatementIndex = -1;

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
        Text(("> " + Line).c_str());
    }

    EndChild();

    if (PreviousFrameWantsTextInputFocus)
    {
        PreviousFrameWantsTextInputFocus = false;
#define NextInputTextItem 0
        SetKeyboardFocusHere(NextInputTextItem);
    }

    InputText("##LuaInputString", LuaStringBuffer, StringBufferMaxSize);

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
    }

    SameLine();
    if (Button("Run") or EnterPressed)
    {
        g_LuaConsoleModuleOutputStr = g_LuaConsoleModuleOutputStr + LuaStringBuffer + "\n";
        RunLua(LuaConsoleStateName, LuaStringBuffer, ErrorBuffer, StringBufferMaxSize);
        if (strcmp(ErrorBuffer, "OK") != StringsAreEqual)
        {
            g_LuaConsoleModuleOutputStr = g_LuaConsoleModuleOutputStr + ErrorBuffer + "\n";
            memset(ErrorBuffer, 0, StringBufferMaxSize);
        }
        AreBuffersZeroed = false;
        EnterPressed = false;
    }

    End();
}

#endif