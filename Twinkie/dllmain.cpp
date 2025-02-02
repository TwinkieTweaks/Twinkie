// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <iostream>
#include "Twink.h"

TM::GmVec3 HsvToRgb(TM::GmVec3 input)
{
    float hh, p, q, t, ff;
    long i;
    TM::GmVec3 output;

    if (input.y <= 0.0f) {
        output.x = input.z;
        output.y = input.z;
        output.z = input.z;
        return output;
    }

    hh = input.x;
    if (hh >= 360.0) hh = 0.0f;
    hh /= 60.0f;
    i = (long)hh;
    ff = hh - i;
    p = input.z * (1.0f - input.y);
    q = input.z * (1.0f - (input.y * ff));
    t = input.z * (1.0f - (input.y * (1.0f - ff)));

    switch (i) {
    case 0:
        output.x = input.z;
        output.y = t;
        output.z = p;
        break;
    case 1:
        output.x = q;
        output.y = input.z;
        output.z = p;
        break;
    case 2:
        output.x = p;
        output.y = input.z;
        output.z = t;
        break;
    case 3:
        output.x = p;
        output.y = q;
        output.z = input.z;
        break;
    case 4:
        output.x = t;
        output.y = p;
        output.z = input.z;
        break;
    case 5:
    default:
        output.x = input.z;
        output.y = p;
        output.z = q;
        break;
    }
    return output;
}

void Setup(const HMODULE Instance)
{
    Twink Twinkie;

    while (!Twinkie.GetTrackmania())
    {
        continue;
    }

    if (Twinkie.GetTrackmania())
    {
        while (!GetKeyState(VK_F3) & 1)
        {
            static float Hue = 0;
            Hue += 0.0001f;
            if (Hue > 360)
            {
                Hue = 0;
            }
            TM::GmVec3 TrailColor = HsvToRgb(TM::GmVec3{ Hue, 1, 1 });
            Twinkie.SetTrailColorRace(TrailColor);
        }
    }

    FreeLibraryAndExitThread(Instance, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);

        const auto Thread = CreateThread(
            nullptr,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup),
            hModule,
            0,
            nullptr
        );

        if (Thread) CloseHandle(Thread);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

