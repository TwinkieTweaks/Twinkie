#pragma once

#include <filesystem>
#include <string>
#include "imgui-dx9/imgui.h"

namespace Filesystem = std::filesystem;
inline std::string AndGetDocumentsFolder()
{
    std::string path;

    char szPath[MAX_PATH + 1] = {};
    if (SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szPath) == S_OK)
        path = szPath;

    return path;
}

class TwinkLogs
{
public:
	bool EnableLog = false;
	std::string LogStr = "";

    TwinkLogs()
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::out);
        if (!LogFile)
        {
            LogStr = LogStr + "[LOG] LogFile could not be opened!\n";
        }
        LogFile.close();
    }

    void PrintInternal(const char* Str)
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::app);
        LogFile << "[TWINK] " << Str << "\n";
        LogStr = LogStr + "[TWINK] " + Str + "\n";
#ifdef TT_EXTERNAL_CONSOLE
        std::cout << "[TWINK] " << Str << "\n";
#endif
        LogFile.close();
    }

    void PrintError(const char* Str)
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::app);
        LogFile << "[ERR] " << Str << "\n";
        LogStr = LogStr + "[ERR] " + Str + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[ERR] " << Str << "\n";
#endif
        LogFile.close();
    }

    void Print(const char* Str)
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::app);
        LogFile << "[LOG] " << Str << "\n";
        LogStr = LogStr + "[LOG] " + Str + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[LOG] " << Str << "\n";
#endif
        LogFile.close();
    }

    void PrintCustom(const char* Str)
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::app);
        LogFile << Str;
        LogStr = LogStr + Str;
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << Str;
#endif
        LogFile.close();
    }

    template<typename... Args>
    void PrintArgs(const char* Str, Args&&... args)
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::app);
        LogFile << "[LOG] " << std::vformat(Str, std::make_format_args(args...)) << "\n";
        LogStr = LogStr + "[LOG] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[LOG] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#endif
        LogFile.close();
    }

    template<typename... Args>
    void PrintErrorArgs(const char* Str, Args&&... args)
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::app);
        LogFile << "[ERR] " << std::vformat(Str, std::make_format_args(args...)) << "\n";
        LogStr = LogStr + "[ERR] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[ERR] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#endif
    }

    template<typename... Args>
    void PrintInternalArgs(const char* Str, Args&&... args)
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::app);
        LogFile << "[TWINK] " << std::vformat(Str, std::make_format_args(args...)) << "\n";
        LogStr = LogStr + "[TWINK] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[TWINK] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#endif
        LogFile.close();
    }

    template<typename... Args>
    void PrintCustomArgs(const char* Str, Args&&... args)
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::app);
        LogFile << std::vformat(Str, std::make_format_args(args...));
        LogStr = LogStr + std::vformat(Str, std::make_format_args(args...));
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << std::vformat(Str, std::make_format_args(args...));
#endif
        LogFile.close();
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
            // TextWrapped(LogStr.c_str());
            std::istringstream Stream(LogStr);
            std::string Line;
            while (std::getline(Stream, Line))
            {
                Text(Line.c_str());
            }
        }
        End();
    }
};