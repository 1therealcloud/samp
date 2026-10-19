#include "game/game.h"
#include "game/input.h"
// samp.dll RVA: 0x000a0a20
void CGame::SetMaxStats(){
    ((void(__cdecl*)())0x4399d0)();((void(__cdecl*)())0x439940)();
    GameUnprotect((void*)0x55a070,1);*(BYTE*)0x55a070=0xc3;
}
// samp.dll RVA: 0x000a0940
void CGame::LetTimeGo(char enabled){
    GameUnprotect((void*)0x52cf10,1);
    if(enabled){*(BYTE*)0x52cf10=0x56;m_bClockEnabled=TRUE;}
    else{*(BYTE*)0x52cf10=0xc3;m_bClockEnabled=FALSE;}
}
