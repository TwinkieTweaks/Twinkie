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

class __declspec(dllexport) TwinkLogs
{
public:
	bool EnableLog = false;
	std::string LogStr = "";

    TwinkLogs()
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::out);
        if (!LogFile)
        {
            LogStr = LogStr + "[L] LogFile could not be opened!\n";
        }
        LogFile.close();
    }

    void PrintInternal(const char* Str)
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::app);
        LogFile << "[T] " << Str << "\n";
        LogStr = LogStr + "[T] " + Str + "\n";
#ifdef TT_EXTERNAL_CONSOLE
        std::cout << "[T] " << Str << "\n";
#endif
        LogFile.close();
    }

    void PrintWarn(const char* Str)
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::app);
        LogFile << "[W] " << Str << "\n";
        LogStr = LogStr + "[W] " + Str + "\n";
#ifdef TT_EXTERNAL_CONSOLE
        std::cout << "[W] " << Str << "\n";
#endif
        LogFile.close();
    }

    void PrintError(const char* Str)
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::app);
        LogFile << "[E] " << Str << "\n";
        LogStr = LogStr + "[E] " + Str + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[E] " << Str << "\n";
#endif
        LogFile.close();
    }

    void Print(const char* Str)
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::app);
        LogFile << "[L] " << Str << "\n";
        LogStr = LogStr + "[L] " + Str + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[L] " << Str << "\n";
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
        LogFile << "[L] " << std::vformat(Str, std::make_format_args(args...)) << "\n";
        LogStr = LogStr + "[L] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[L] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#endif
        LogFile.close();
    }

    template<typename... Args>
    void PrintErrorArgs(const char* Str, Args&&... args)
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::app);
        LogFile << "[E] " << std::vformat(Str, std::make_format_args(args...)) << "\n";
        LogStr = LogStr + "[E] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[E] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#endif
    }

    template<typename... Args>
    void PrintInternalArgs(const char* Str, Args&&... args)
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::app);
        LogFile << "[T] " << std::vformat(Str, std::make_format_args(args...)) << "\n";
        LogStr = LogStr + "[T] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[T] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#endif
        LogFile.close();
    }

    template<typename... Args>
    void PrintWarnArgs(const char* Str, Args&&... args)
    {
        std::ofstream LogFile(AndGetDocumentsFolder() + "\\Twinkie\\Twinkie.log", std::ios::app);
        LogFile << "[W] " << std::vformat(Str, std::make_format_args(args...)) << "\n";
        LogStr = LogStr + "[W] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
#ifdef BUILD_EXTERNAL_CONSOLE
        std::cout << "[W] " + std::vformat(Str, std::make_format_args(args...)) + "\n";
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