#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>

#ifndef I_UTILS
#define I_UTILS

template <typename T>
std::string ToHex(T Value) 
{
    static_assert(std::is_integral<T>::value, "T must be an integral type");

    std::stringstream StringStream;
    StringStream << std::hex << std::uppercase << Value;
    return "0x" + StringStream.str();
}

#endif