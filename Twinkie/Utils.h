#pragma once

#include <string>
#include <sstream>
#include <iomanip>

template <typename T>
std::string ToHex(T Value) 
{
    static_assert(std::is_integral<T>::value, "T must be an integral type");

    std::stringstream StringStream;
    StringStream << std::hex << std::uppercase << Value;
    return "0x" + StringStream.str();
}

template <typename T>
std::wstring ToHexW(T Value)
{
    static_assert(std::is_integral<T>::value, "T must be an integral type");

    std::wstringstream StringStream;
    StringStream << std::hex << std::uppercase << Value;
    return L"0x" + StringStream.str();
}

std::string WStringToUTF8(const std::wstring& wstr) {
    if (wstr.empty()) return {};

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &strTo[0], size_needed, nullptr, nullptr);
    return strTo;
}