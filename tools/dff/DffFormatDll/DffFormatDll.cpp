#include "DffFormatDll.h"
#include "clump.h"
#include "collision.h"
#include <stdio.h>
#include <windows.h>

// 0x000022f0
BOOL WINAPI DllMain(HINSTANCE Instance, DWORD Reason, LPVOID Reserved)
{
    return TRUE;
}

// 0x000025a0
int __cdecl GenerateColFromDff(const char* SourceDff, int Mode, const char* DestinationCol)
{
    CClump Clump;
    CColModel Col;
    if (Clump.Load(SourceDff) < 1)
    {
        OutputDebugStringA("[kdffdll:error:read] Could not load the dff file.\n");
        return 1;
    }
    if (!Clump.ReadGeometry())
    {
        OutputDebugStringA("[kdffdll:error:read] Couldn't read the geometry.\n");
        return 2;
    }
    switch (Mode)
    {
        case 0:
            if (!Col.GenerateEmpty(static_cast<CGeometry*>(Clump.Geometries.Get(0))))
            {
                OutputDebugStringA("[kdffdll:error:genempty] Couldn't generate the col.\n");
                return 4;
            }
            break;
        case 1:
            if (!Col.GenerateBox(static_cast<CGeometry*>(Clump.Geometries.Get(0))))
            {
                OutputDebugStringA("[kdffdll:error:genbox] Couldn't generate the col.\n");
                return 4;
            }
            break;
        case 2:
            if (!Col.GenerateFaces(static_cast<CGeometry*>(Clump.Geometries.Get(0))))
            {
                OutputDebugStringA("[kdffdll:error:genfaces] Couldn't generate the col.\n");
                return 4;
            }
            break;
    }
    if (!Col.Save(DestinationCol, "samp"))
    {
        OutputDebugStringA("[kdffdll:error:save] Couldn't save the col.\n");
        return 5;
    }
    return 0;
}

// 0x00002780
int __cdecl AppendColToDff(const char* SourceDff, const char* SourceCol, const char* DestinationDff)
{
    CClump Clump;
    if (Clump.Load(SourceDff) < 1)
    {
        OutputDebugStringA("[kdffdll:error:read] Could not load the dff file.\n");
        return 1;
    }
    if (Clump.HasCol)
    {
        OutputDebugStringA("[kdffdll:error:read] File already has a col.\n");
        return 6;
    }
    int Result = Clump.AppendCol(SourceCol);
    if (Result < 1)
    {
        char Buffer[512];
        _snprintf(Buffer, sizeof(Buffer), "[kdffdll:error:appendcol] Could not append col to file. Error: %d\n", Result);
        OutputDebugStringA(Buffer);
        return 7;
    }
    if (static_cast<int>(Clump.Save(DestinationDff)) < 1)
    {
        printf("[kdffdll:error:savedff] Could not save clump to file.\n");
        return 5;
    }
    return 0;
}

// 0x000028d0
int __cdecl ImportNVCFromDff(const char* SourceDff, const char* NightDff, const char* DestinationDff)
{
    CClump Source, Night;
    if (Source.Load(SourceDff) < 1)
        return 1;
    if (Night.Load(NightDff) < 1)
        return 2;
    if (!Source.ReadGeometry())
        return 4;
    if (!Night.ReadGeometry())
        return 5;
    CGeometry* Geometry = static_cast<CGeometry*>(Source.Geometries.Get(0));
    CGeometry* NightGeometry = static_cast<CGeometry*>(Night.Geometries.Get(0));
    if (!Geometry)
        return 4;
    if (!NightGeometry)
        return 5;
    if (!Geometry->ImportNightColors(NightGeometry))
        return 9;
    if (!Source.RebuildGeometry())
        return 7;
    if (static_cast<int>(Source.Save(DestinationDff)) < 1)
        return 8;
    return 0;
}

// 0x00002a70
int __cdecl EasyVertexColors(const char* SourceDff, unsigned int DayColor, unsigned int NightColor, float DayX, float DayY, float DayZ, float NightX, float NightY, float NightZ,
                             float Minimum, float Maximum, const char* DestinationDff)
{
    CClump Source, Unused;
    RwColor Day = {static_cast<unsigned char>(DayColor), static_cast<unsigned char>(DayColor >> 8), static_cast<unsigned char>(DayColor >> 16), 0xff};
    RwColor Night = {static_cast<unsigned char>(NightColor), static_cast<unsigned char>(NightColor >> 8), static_cast<unsigned char>(NightColor >> 16), 0xff};
    if (Source.Load(SourceDff) < 1)
        return 1;
    if (!Source.ReadGeometry())
        return 4;
    CGeometry* Geometry = static_cast<CGeometry*>(Source.Geometries.Get(0));
    if (!Geometry)
        return 4;
    CVector DayDirection = {DayX, DayY, DayZ};
    CVector NightDirection = {NightX, NightY, NightZ};
    Geometry->GenerateNormals();
    Geometry->SetColors(&Day, &Night, &DayDirection, &NightDirection, Minimum, Maximum);
    if (!Source.RebuildGeometry())
        return 7;
    if (static_cast<int>(Source.Save(DestinationDff)) < 1)
        return 8;
    return 0;
}
