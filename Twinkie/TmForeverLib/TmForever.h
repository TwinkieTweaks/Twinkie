#pragma once

#pragma comment(lib, "TmForeverLib/TmForever.lib")

#define TMF_API __declspec( dllimport )

class CFastStringInt
{
public:
    int Size;
    wchar_t* Cstr;

    __declspec(dllimport) __thiscall CFastStringInt(wchar_t const*);
};

class CMwNod {
public:
    void** vftable;
    unsigned int m_ReferenceCount;
    uintptr_t m_SystemFid;
    TM::CFastBuffer<CMwNod*>* m_Dependants;
    unsigned int u1;
};

class CSystemFids : public CMwNod
{

};

class CClassicArchive : public CMwNod
{
};

class __declspec(dllimport) CSystemArchiveNod : public CClassicArchive
{
public:
    enum EArchive
    {
        ZERO,
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN // <-- always use this one
    };

    static int __cdecl LoadFileFrom(CFastStringInt const&, CMwNod*&, CSystemFids*, EArchive);
};

class __declspec(dllimport) CSystemEngine : public CMwNod
{
public:
    CSystemFids* __thiscall GetLocationData();
};