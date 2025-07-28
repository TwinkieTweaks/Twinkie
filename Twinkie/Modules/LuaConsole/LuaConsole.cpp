#include "LuaConsole.h"
#include "../../imgui-dx9/imgui_internal.h"

#define StringBufferMaxSize 512
#ifdef BUILD_DEBUG

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
        Text(("> " + Line).c_str());
    }

    EndChild();

    if (PreviousFrameWantsTextInputFocus)
    {
        PreviousFrameWantsTextInputFocus = false;
#define NextInputTextItem
        SetKeyboardFocusHere(NextInputTextItem);
    }

    InputText("##LuaInputString", LuaStringBuffer, StringBufferMaxSize);

    if (IsItemFocused())
    {
#define NoRepeat false
        if (IsKeyPressed(ImGuiKey_Enter, NoRepeat) and strcmp(LuaStringBuffer, "") != 0)
        {
            EnterPressed = true;
            PreviousFrameWantsTextInputFocus = true;
        }
    }

    SameLine();
    if (Button("Run") or EnterPressed)
    {
        g_LuaConsoleModuleOutputStr = g_LuaConsoleModuleOutputStr + LuaStringBuffer + "\n";
        RunLua("TwinkieLuaConsole", LuaStringBuffer, ErrorBuffer, StringBufferMaxSize);
        if (strcmp(ErrorBuffer, "OK") != 0)
        {
            Logger->PrintErrorArgs("Error while running lua string: {}", ErrorBuffer);
            memset(ErrorBuffer, 0, StringBufferMaxSize);
        }
        AreBuffersZeroed = false;
        EnterPressed = false;
    }

    End();
}

#endif