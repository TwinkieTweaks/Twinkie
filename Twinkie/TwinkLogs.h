#pragma once

#include <string>
#include "imgui-dx9/imgui.h"

class TwinkLogs
{
public:
	bool EnableLog = false;
	std::string LogStr = "";

    void PrintInternal(const char* Str)
    {
        LogStr = LogStr + "[TWINK] " + Str + "\n";
#ifdef TT_EXTERNAL_CONSOLE
        std::cout << "[TWINK] " << Str << "\n";
#endif
    }

    void PrintError(const char* Str)
    {
        LogStr = LogStr + "[ERR] " + Str + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[ERR] " << Str << "\n";
#endif
    }

    void Print(const char* Str)
    {
        LogStr = LogStr + "[LOG] " + Str + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[LOG] " << Str << "\n";
#endif
    }

    template<typename... Args>
    void PrintArgs(const char* Str, Args&&... args)
    {
        LogStr = LogStr + "[LOG] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[LOG] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#endif
    }

    template<typename... Args>
    void PrintErrorArgs(const char* Str, Args&&... args)
    {
        LogStr = LogStr + "[ERR] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[ERR] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#endif
    }

    template<typename... Args>
    void PrintInternalArgs(const char* Str, Args&&... args)
    {
        LogStr = LogStr + "[TWINK] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[TWINK] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#endif
    }

    void RenderLog()
    {
        using namespace ImGui;
        if (Begin("Logs", &EnableLog, ImGuiWindowFlags_MenuBar))
        {
            if (BeginMenuBar())
            {
                if (MenuItem("Clear"))
                {
                    LogStr = "";
                }
                EndMenuBar();
            }
            TextWrapped(LogStr.c_str());
        }
        End();
    }
};