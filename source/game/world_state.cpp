#include "game/game.h"
#include "game/wrapper_commands.h"
// samp.dll RVA: 0x0011e68c
const Scripting::OpcodeInfo g_gameSetWorldTime={0xc0,"ii"};
// samp.dll RVA: 0x000a0980
void CGame::SetWorldWeather(int weather){
    BOOL clockEnabled=m_bClockEnabled;
    // GTA SA virtual addresses: 0x00c81318, 0x00c8131c, 0x00c81320.
    *(DWORD*)0xc81318=weather;
    if(!clockEnabled){*(DWORD*)0xc8131c=weather;*(DWORD*)0xc81320=weather;}
}
// samp.dll RVA: 0x000a08f0
void CGame::SetWorldTime(int hour,int minute){
    // GTA SA virtual addresses: minute 0x00b70152, hour 0x00b70153.
    *(BYTE*)0xb70152=(BYTE)minute;*(BYTE*)0xb70153=(BYTE)hour;
    Scripting::ProcessCommand(&g_gameSetWorldTime,hour,minute);
}
