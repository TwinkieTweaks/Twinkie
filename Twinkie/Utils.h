#pragma once

#include <string>
#include <sstream>
#include <iomanip>

template <typename T>
std::string ToHex(T Value) {
    // imma be honest ChatGPT wrote this shit
    // idk why but std::hex just doesn't like working with me
    // someone must've           HEXED ME LMAOAMOAMAOMOA,AK;AA;AA;;SM,A;OSMD;ASJKFOIUPOIERY9WRQOJGKSFJIKILLED17PEOPLEAIJDFNAOSGHAI
    static_assert(std::is_integral<T>::value, "T must be an integral type");

    std::stringstream StringStream;
    StringStream << std::hex << std::uppercase << Value;
    return "0x" + StringStream.str();
}