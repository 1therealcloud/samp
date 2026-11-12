#ifndef KDFF_DLL_H
#define KDFF_DLL_H

extern "C"
{
    // 0x000025a0
    int __cdecl GenerateColFromDff(const char* SourceDff, int Mode, const char* DestinationCol);
    // 0x00002780
    int __cdecl AppendColToDff(const char* SourceDff, const char* SourceCol, const char* DestinationDff);
    // 0x000028d0
    int __cdecl ImportNVCFromDff(const char* SourceDff, const char* NightDff, const char* DestinationDff);
    // 0x00002a70
    int __cdecl EasyVertexColors(const char* SourceDff, unsigned int DayColor, unsigned int NightColor, float DayX, float DayY, float DayZ, float NightX, float NightY,
                                 float NightZ, float Minimum, float Maximum, const char* DestinationDff);
}

#endif
