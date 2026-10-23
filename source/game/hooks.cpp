#include "game/hooks.h"
#include "game/game.h"

// Raw installed callback: EAX returns the preview-ped backing storage.
// samp.dll RVA: 0x000a3ec0
__declspec(naked) void GameHook_000a3ec0() {
    __asm {
        mov eax,offset g_gamePreviewPed
        ret
    }
}
#include <string.h>
// samp.dll RVA: 0x002ac9cc
DWORD g_gameLoopAddress;
// samp.dll RVA: 0x0018f56c
DWORD g_gameHookFarClipReturn;
// samp.dll RVA: 0x00151a10
BYTE g_gameHookJump00151a10[10]={0xff,0x25,0x34,0x39,0x4d,0x00,0x90,0x90,0x90,0x90};
// samp.dll RVA: 0x00151a1c
BYTE g_gameHookJump00151a1c[7]={0xff,0x25,0x09,0x46,0x4d,0x00,0x90};
// samp.dll RVA: 0x0015197c
BYTE g_gameHookJump0015197c[7]={0xff,0x25,0xbb,0x19,0x69,0x00,0x90};
// samp.dll RVA: 0x00151984
BYTE g_gameHookJump00151984[7]={0xff,0x25,0xba,0xb8,0x63,0x00,0x90};
// samp.dll RVA: 0x00151994
BYTE g_gameHookJump00151994[7]={0xff,0x25,0xaa,0x85,0x43,0x00,0x90};
// samp.dll RVA: 0x0015198c
BYTE g_gameHookJump0015198c[7]={0xff,0x25,0x79,0x4a,0x58,0x00,0x90};
// samp.dll RVA: 0x001519b0
BYTE g_gameHookJump001519b0[7]={0xff,0x25,0xf1,0xc8,0x53,0x00,0x90};
// samp.dll RVA: 0x001519b8
BYTE g_gameHookJump001519b8[6]={0xff,0x25,0xbc,0x5a,0x4b,0x00};
// samp.dll RVA: 0x001519e8
BYTE g_gameHookJump001519e8[6]={0xff,0x25,0x1b,0x8b,0x73,0x00};
// samp.dll RVA: 0x001519f0
BYTE g_gameHookJump001519f0[6]={0xff,0x25,0x5b,0x88,0x73,0x00};
// samp.dll RVA: 0x001519d4
BYTE g_gameHookJump001519d4[9]={0xff,0x25,0x43,0x00,0x6a,0x00,0x90,0x90,0x90};
// samp.dll RVA: 0x00151a34
BYTE g_gameHookJump00151a34[6]={0xff,0x25,0x21,0xc7,0x59,0x00};
// samp.dll RVA: 0x00151a3c
BYTE g_gameHookJump00151a3c[7]={0xff,0x25,0x84,0x80,0x53,0x00,0x90};
// samp.dll RVA: 0x00151a44
BYTE g_gameHookJump00151a44[9]={0xff,0x25,0xa6,0x34,0x55,0x00,0x90,0x90,0x90};
// samp.dll RVA: 0x00151a50
BYTE g_gameHookJump00151a50[6]={0xff,0x25,0x33,0x34,0x4b,0x00};
// samp.dll RVA: 0x00151a68
BYTE g_gameHookJump00151a68[6]={0xff,0x25,0xdb,0x74,0x56,0x00};
// samp.dll RVA: 0x000a5d80
void GameInstallHook(DWORD address,DWORD function,DWORD storage,const BYTE *jumpCode,int size) {
    DWORD oldProtect,ignored;
    VirtualProtect((void*)storage,4,PAGE_EXECUTE_READWRITE,&oldProtect);
    *(DWORD*)storage=function;
    VirtualProtect((void*)storage,4,oldProtect,&ignored);
    VirtualProtect((void*)address,size,PAGE_EXECUTE_READWRITE,&oldProtect);
    memcpy((void*)address,jumpCode,size);
    VirtualProtect((void*)address,size,oldProtect,&ignored);
}
// samp.dll RVA: 0x000a5e30
void GamePrepareHooks()
{
  DWORD local_8;
  DWORD local_4;
  
  GameUnprotect((void*)0x53eb13,4);
  *(DWORD*)0x0053eb13 = g_gameLoopAddress - 0x53eb17;
  VirtualProtect((void*)0x0058fc53,5,0x40,&local_8);
  *(BYTE*)0x0058fc53 = 0xe8;
  *(DWORD*)0x0058fc54 = (DWORD)GameHook_000a22c0 - 0x58fc58;
  VirtualProtect((void*)0x0058fc53,5,local_8,&local_4);
  VirtualProtect((void*)0x0058fbbf,5,0x40,&local_8);
  *(BYTE*)0x0058fbbf = 0xe8;
  *(DWORD*)0x0058fbc0 = (DWORD)GameHook_000a22f0 - 0x58fbc4;
  VirtualProtect((void*)0x0058fbbf,5,local_8,&local_4);
  VirtualProtect((void*)0x0053c104,5,0x40,&local_8);
  *(BYTE*)0x0053c104 = 0xe8;
  *(DWORD*)0x0053c105 = (DWORD)GameHook_000a23a0 - 0x53c109;
  VirtualProtect((void*)0x0053c104,5,local_8,&local_4);
  VirtualProtect((void*)0x0053e981,5,0x40,&local_8);
  *(BYTE*)0x0053e981 = 0xe8;
  *(DWORD*)0x0053e982 = (DWORD)GameHook_000a23d0 - 0x53e986;
  VirtualProtect((void*)0x0053e981,5,local_8,&local_4);
  VirtualProtect((void*)0x0086d1b0,4,0x40,&local_8);
  *(DWORD*)0x0086d1b0 = (DWORD)GameHook_000a3b90;
  VirtualProtect((void*)0x0086d1b0,4,local_8,&local_4);
  VirtualProtect((void*)0x0086c0f0,4,0x40,&local_8);
  *(DWORD*)0x0086c0f0 = (DWORD)GameHook_000a3b90;
  VirtualProtect((void*)0x0086c0f0,4,local_8,&local_4);
  VirtualProtect((void*)0x0086c168,4,0x40,&local_8);
  *(DWORD*)0x0086c168 = (DWORD)GameHook_000a3b90;
  VirtualProtect((void*)0x0086c168,4,local_8,&local_4);
  VirtualProtect((void*)0x0086c248,4,0x40,&local_8);
  *(DWORD*)0x0086c248 = (DWORD)GameHook_000a3b90;
  VirtualProtect((void*)0x0086c248,4,local_8,&local_4);
  VirtualProtect((void*)0x0086c3a0,4,0x40,&local_8);
  *(DWORD*)0x0086c3a0 = (DWORD)GameHook_000a3b90;
  VirtualProtect((void*)0x0086c3a0,4,local_8,&local_4);
  VirtualProtect((void*)0x0053e930,5,0x40,&local_8);
  *(BYTE*)0x0053e930 = 0xe8;
  *(DWORD*)0x0053e931 = (DWORD)GameHook_000a2200 - 0x53e935;
  VirtualProtect((void*)0x0053e930,5,local_8,&local_4);
  return;
}


// samp.dll RVA: 0x000a6020
void GameInstallHooks()
{
  DWORD local_8;
  DWORD local_4;
  
  GamePrepareHooks();
  GameInstallHook(0x4d3aa0,(DWORD)GameHook_000a3f30,0x4d3934,g_gameHookJump00151a10,10);
  GameInstallHook(0x4d4610,(DWORD)GameHook_000a3fe0,0x4d4609,g_gameHookJump00151a1c,7);
  VirtualProtect((void*)0x0086d190,4,0x40,&local_8);
  *(DWORD*)0x0086d190 = (DWORD)GameHook_000a26d0;
  VirtualProtect((void*)0x0086d190,4,local_8,&local_4);
  VirtualProtect((void*)0x0086c0d0,4,0x40,&local_8);
  *(DWORD*)0x0086c0d0 = (DWORD)GameHook_000a29d0;
  VirtualProtect((void*)0x0086c0d0,4,local_8,&local_4);
  VirtualProtect((void*)0x0086d744,4,0x40,&local_8);
  *(DWORD*)0x0086d744 = (DWORD)GameHook_000a2b50;
  VirtualProtect((void*)0x0086d744,4,local_8,&local_4);
  VirtualProtect((void*)0x007330a2,5,0x40,&local_8);
  *(BYTE*)0x007330a2 = 0xe8;
  *(DWORD*)0x007330a3 = (DWORD)GameHook_000a2ab0 - 0x7330a7;
  VirtualProtect((void*)0x007330a2,5,local_8,&local_4);
  VirtualProtect((void*)0x0086d194,4,0x40,&local_8);
  *(DWORD*)0x0086d194 = (DWORD)GameHook_000a3e50;
  VirtualProtect((void*)0x0086d194,4,local_8,&local_4);
  VirtualProtect((void*)0x005689fd,5,0x40,&local_8);
  *(BYTE*)0x005689fd = 0xe8;
  *(DWORD*)0x005689fe = (DWORD)GameHook_000a46b0 - 0x568a02;
  VirtualProtect((void*)0x005689fd,5,local_8,&local_4);
  VirtualProtect((void*)0x0053ea03,5,0x40,&local_8);
  *(BYTE*)0x0053ea03 = 0xe8;
  *(DWORD*)0x0053ea04 = (DWORD)GameHook_000a2370 - 0x53ea08;
  VirtualProtect((void*)0x0053ea03,5,local_8,&local_4);
  VirtualProtect((void*)0x00871148,4,0x40,&local_8);
  *(DWORD*)0x00871148 = (DWORD)GameHook_000a2e70;
  VirtualProtect((void*)0x00871148,4,local_8,&local_4);
  VirtualProtect((void*)0x008721c8,4,0x40,&local_8);
  *(DWORD*)0x008721c8 = (DWORD)GameHook_000a2e70;
  VirtualProtect((void*)0x008721c8,4,local_8,&local_4);
  VirtualProtect((void*)0x00871388,4,0x40,&local_8);
  *(DWORD*)0x00871388 = (DWORD)GameHook_000a2e70;
  VirtualProtect((void*)0x00871388,4,local_8,&local_4);
  VirtualProtect((void*)0x00871970,4,0x40,&local_8);
  *(DWORD*)0x00871970 = (DWORD)GameHook_000a2e70;
  VirtualProtect((void*)0x00871970,4,local_8,&local_4);
  VirtualProtect((void*)0x008716a8,4,0x40,&local_8);
  *(DWORD*)0x008716a8 = (DWORD)GameHook_000a2e70;
  VirtualProtect((void*)0x008716a8,4,local_8,&local_4);
  VirtualProtect((void*)0x00871550,4,0x40,&local_8);
  *(DWORD*)0x00871550 = (DWORD)GameHook_000a2e70;
  VirtualProtect((void*)0x00871550,4,local_8,&local_4);
  VirtualProtect((void*)0x00871800,4,0x40,&local_8);
  *(DWORD*)0x00871800 = (DWORD)GameHook_000a2e70;
  VirtualProtect((void*)0x00871800,4,local_8,&local_4);
  VirtualProtect((void*)0x00871b10,4,0x40,&local_8);
  *(DWORD*)0x00871b10 = (DWORD)GameHook_000a2e70;
  VirtualProtect((void*)0x00871b10,4,local_8,&local_4);
  VirtualProtect((void*)0x00872398,4,0x40,&local_8);
  *(DWORD*)0x00872398 = (DWORD)GameHook_000a2e70;
  VirtualProtect((void*)0x00872398,4,local_8,&local_4);
  VirtualProtect((void*)0x00871c50,4,0x40,&local_8);
  *(DWORD*)0x00871c50 = (DWORD)GameHook_000a2e70;
  VirtualProtect((void*)0x00871c50,4,local_8,&local_4);
  VirtualProtect((void*)0x00501b1d,5,0x40,&local_8);
  *(BYTE*)0x00501b1d = 0xe8;
  *(DWORD*)0x00501b1e = (DWORD)GameHook_000a33c0 - 0x501b22;
  VirtualProtect((void*)0x00501b1d,5,local_8,&local_4);
  VirtualProtect((void*)0x00501b42,5,0x40,&local_8);
  *(BYTE*)0x00501b42 = 0xe8;
  *(DWORD*)0x00501b43 = (DWORD)GameHook_000a33c0 - 0x501b47;
  VirtualProtect((void*)0x00501b42,5,local_8,&local_4);
  VirtualProtect((void*)0x00501fc2,5,0x40,&local_8);
  *(BYTE*)0x00501fc2 = 0xe8;
  *(DWORD*)0x00501fc3 = (DWORD)GameHook_000a33c0 - 0x501fc7;
  VirtualProtect((void*)0x00501fc2,5,local_8,&local_4);
  VirtualProtect((void*)0x00502067,5,0x40,&local_8);
  *(BYTE*)0x00502067 = 0xe8;
  *(DWORD*)0x00502068 = (DWORD)GameHook_000a33c0 - 0x50206c;
  VirtualProtect((void*)0x00502067,5,local_8,&local_4);
  VirtualProtect((void*)0x005021ae,5,0x40,&local_8);
  *(BYTE*)0x005021ae = 0xe8;
  *(DWORD*)0x005021af = (DWORD)GameHook_000a33c0 - 0x5021b3;
  VirtualProtect((void*)0x005021ae,5,local_8,&local_4);
  VirtualProtect((void*)0x005869bf,5,0x40,&local_8);
  *(BYTE*)0x005869bf = 0xe8;
  *(DWORD*)0x005869c0 = (DWORD)GameHook_000a3c60 - 0x5869c4;
  VirtualProtect((void*)0x005869bf,5,local_8,&local_4);
  VirtualProtect((void*)0x005759e4,5,0x40,&local_8);
  *(BYTE*)0x005759e4 = 0xe8;
  *(DWORD*)0x005759e5 = (DWORD)GameHook_000a3c60 - 0x5759e9;
  VirtualProtect((void*)0x005759e4,5,local_8,&local_4);
  VirtualProtect((void*)0x00609a4e,5,0x40,&local_8);
  *(BYTE*)0x00609a4e = 0xe8;
  *(DWORD*)0x00609a4f = (DWORD)GameHook_000a3c80 - 0x609a53;
  VirtualProtect((void*)0x00609a4e,5,local_8,&local_4);
  VirtualProtect((void*)0x004579c6,5,0x40,&local_8);
  *(BYTE*)0x004579c6 = 0xe9;
  *(DWORD*)0x004579c7 = (DWORD)GameHook_000a3ca0 - 0x4579cb;
  VirtualProtect((void*)0x004579c6,5,local_8,&local_4);
  VirtualProtect((void*)0x0073c252,5,0x40,&local_8);
  *(BYTE*)0x0073c252 = 0xe9;
  *(DWORD*)0x0073c253 = (DWORD)GameHook_000a3ad0 - 0x73c257;
  VirtualProtect((void*)0x0073c252,5,local_8,&local_4);
  VirtualProtect((void*)0x0073ace2,5,0x40,&local_8);
  *(BYTE*)0x0073ace2 = 0xe8;
  *(DWORD*)0x0073ace3 = (DWORD)GameHook_000a3ac0 - 0x73ace7;
  VirtualProtect((void*)0x0073ace2,5,local_8,&local_4);
  VirtualProtect((void*)0x006f8cf8,5,0x40,&local_8);
  *(BYTE*)0x006f8cf8 = 0xe8;
  *(DWORD*)0x006f8cf9 = (DWORD)GameHook_000a3b00 - 0x6f8cfd;
  VirtualProtect((void*)0x006f8cf8,5,local_8,&local_4);
  GameInstallHook(0x6402f0,(DWORD)GameHook_000a3580,0x6919bb,g_gameHookJump0015197c,7);
  GameInstallHook(0x63b8c0,(DWORD)GameHook_000a35e0,0x63b8ba,g_gameHookJump00151984,7);
  GameInstallHook(0x438576,(DWORD)GameHook_000a37c0,0x4385aa,g_gameHookJump00151994,7);
  GameInstallHook(0x584770,(DWORD)GameHook_000a37a0,0x584a79,g_gameHookJump0015198c,7);
  GameInstallHook(0x53c900,(DWORD)GameHook_000a3800,0x53c8f1,g_gameHookJump001519b0,7);
  GameInstallHook(0x4b5ac0,(DWORD)GameHook_000a3a70,0x4b5abc,g_gameHookJump001519b8,6);
  VirtualProtect((void*)0x004d41c0,5,0x40,&local_8);
  *(BYTE*)0x004d41c0 = 0xe9;
  *(DWORD*)0x004d41c1 = (DWORD)GameHook_000a3b20 - 0x4d41c5;
  VirtualProtect((void*)0x004d41c0,5,local_8,&local_4);
  VirtualProtect((void*)0x004e7427,5,0x40,&local_8);
  *(BYTE*)0x004e7427 = 0xe8;
  *(DWORD*)0x004e7428 = (DWORD)GameHook_000a3ec0 - 0x4e742c;
  VirtualProtect((void*)0x004e7427,5,local_8,&local_4);
  GameInstallHook(0x738f3a,(DWORD)GameHook_000a3ce0,0x738b1b,g_gameHookJump001519e8,6);
  GameInstallHook(0x738877,(DWORD)GameHook_000a3d20,0x73885b,g_gameHookJump001519f0,6);
  GameInstallHook(0x6a0050,(DWORD)GameHook_000a3d90,0x6a0043,g_gameHookJump001519d4,9);
  VirtualProtect((void*)0x006fded6,5,0x40,&local_8);
  *(BYTE*)0x006fded6 = 0xe8;
  *(DWORD*)0x006fded7 = (DWORD)GameHook_000a4280 - 0x6fdedb;
  VirtualProtect((void*)0x006fded6,5,local_8,&local_4);
  if (g_gameVersion == 1) {
    GameInstallHook(0x7fb020,(DWORD)GameHook_000a4190,0x59c721,g_gameHookJump00151a34,6);
    g_gameHookFarClipReturn = 0x7fb026;
  }
  else {
    GameInstallHook(0x7fb060,(DWORD)GameHook_000a4190,0x59c721,g_gameHookJump00151a34,6);
    g_gameHookFarClipReturn = 0x7fb066;
  }
  VirtualProtect((void*)0x006d0e7e,5,0x40,&local_8);
  *(BYTE*)0x006d0e7e = 0xe8;
  *(DWORD*)0x006d0e7f = (DWORD)GameHook_000a42c0 - 0x6d0e83;
  VirtualProtect((void*)0x006d0e7e,5,local_8,&local_4);
  VirtualProtect((void*)0x00866fa8,4,0x40,&local_8);
  *(DWORD*)0x00866fa8 = (DWORD)GameHook_000a42d0;
  VirtualProtect((void*)0x00866fa8,4,local_8,&local_4);
  VirtualProtect((void*)0x00586c0a,5,0x40,&local_8);
  *(BYTE*)0x00586c0a = 0xe8;
  *(DWORD*)0x00586c0b = (DWORD)GameHook_000a4770 - 0x586c0f;
  VirtualProtect((void*)0x00586c0a,5,local_8,&local_4);
  GameInstallHook(0x538090,(DWORD)GameHook_000a47d0,0x538084,g_gameHookJump00151a3c,7);
  VirtualProtect((void*)0x00718599,5,0x40,&local_8);
  *(BYTE*)0x00718599 = 0xe8;
  *(DWORD*)0x0071859a = (DWORD)GameHook_000a4960 - 0x71859e;
  VirtualProtect((void*)0x00718599,5,local_8,&local_4);
  VirtualProtect((void*)0x00866f7c,4,0x40,&local_8);
  *(DWORD*)0x00866f7c = (DWORD)GameHook_000a4850;
  VirtualProtect((void*)0x00866f7c,4,local_8,&local_4);
  VirtualProtect((void*)0x00866f80,4,0x40,&local_8);
  *(DWORD*)0x00866f80 = (DWORD)GameHook_000a48c0;
  VirtualProtect((void*)0x00866f80,4,local_8,&local_4);
  VirtualProtect((void*)0x008585e8,4,0x40,&local_8);
  *(DWORD*)0x008585e8 = (DWORD)GameHook_000a48c0;
  VirtualProtect((void*)0x008585e8,4,local_8,&local_4);
  VirtualProtect((void*)0x00871218,4,0x40,&local_8);
  *(DWORD*)0x00871218 = (DWORD)GameHook_000a49d0;
  VirtualProtect((void*)0x00871218,4,local_8,&local_4);
  VirtualProtect((void*)0x00871778,4,0x40,&local_8);
  *(DWORD*)0x00871778 = (DWORD)GameHook_000a49d0;
  VirtualProtect((void*)0x00871778,4,local_8,&local_4);
  VirtualProtect((void*)0x008718d0,4,0x40,&local_8);
  *(DWORD*)0x008718d0 = (DWORD)GameHook_000a49d0;
  VirtualProtect((void*)0x008718d0,4,local_8,&local_4);
  VirtualProtect((void*)0x00871a40,4,0x40,&local_8);
  *(DWORD*)0x00871a40 = (DWORD)GameHook_000a49d0;
  VirtualProtect((void*)0x00871a40,4,local_8,&local_4);
  VirtualProtect((void*)0x00871be0,4,0x40,&local_8);
  *(DWORD*)0x00871be0 = (DWORD)GameHook_000a49d0;
  VirtualProtect((void*)0x00871be0,4,local_8,&local_4);
  VirtualProtect((void*)0x005648d3,5,0x40,&local_8);
  *(BYTE*)0x005648d3 = 0xe8;
  *(DWORD*)0x005648d4 = (DWORD)GameHook_000a4c30 - 0x5648d8;
  VirtualProtect((void*)0x005648d3,5,local_8,&local_4);
  VirtualProtect((void*)0x0053dfdd,5,0x40,&local_8);
  *(BYTE*)0x0053dfdd = 0xe8;
  *(DWORD*)0x0053dfde = (DWORD)GameHook_000a4df0 - 0x53dfe2;
  VirtualProtect((void*)0x0053dfdd,5,local_8,&local_4);
  VirtualProtect((void*)0x0053e019,5,0x40,&local_8);
  *(BYTE*)0x0053e019 = 0xe8;
  *(DWORD*)0x0053e01a = (DWORD)GameHook_000a4e30 - 0x53e01e;
  VirtualProtect((void*)0x0053e019,5,local_8,&local_4);
  GameInstallHook(0x5534b0,(DWORD)GameHook_000a4e70,0x5534a6,g_gameHookJump00151a44,9);
  VirtualProtect((void*)0x005342f9,5,0x40,&local_8);
  *(BYTE*)0x005342f9 = 0xe8;
  *(DWORD*)0x005342fa = (DWORD)GameHook_000a5150 - 0x5342fe;
  VirtualProtect((void*)0x005342f9,5,local_8,&local_4);
  GameInstallHook(0x4b35a0,(DWORD)GameHook_000a5100,0x4b3433,g_gameHookJump00151a50,6);
  VirtualProtect((void*)0x0041b02e,5,0x40,&local_8);
  *(BYTE*)0x0041b02e = 0xe8;
  *(DWORD*)0x0041b02f = (DWORD)GameHook_000a56b0 - 0x41b033;
  VirtualProtect((void*)0x0041b02e,5,local_8,&local_4);
  VirtualProtect((void*)0x0041af80,5,0x40,&local_8);
  *(BYTE*)0x0041af80 = 0xe8;
  *(DWORD*)0x0041af81 = (DWORD)GameHook_000a5820 - 0x41af85;
  VirtualProtect((void*)0x0041af80,5,local_8,&local_4);
  VirtualProtect((void*)0x0041ab78,5,0x40,&local_8);
  *(BYTE*)0x0041ab78 = 0xe8;
  *(DWORD*)0x0041ab79 = (DWORD)GameHook_000a5850 - 0x41ab7d;
  VirtualProtect((void*)0x0041ab78,5,local_8,&local_4);
  VirtualProtect((void*)0x00871178,4,0x40,&local_8);
  *(DWORD*)0x00871178 = (DWORD)GameHook_000a5870;
  VirtualProtect((void*)0x00871178,4,local_8,&local_4);
  VirtualProtect((void*)0x008716d8,4,0x40,&local_8);
  *(DWORD*)0x008716d8 = (DWORD)GameHook_000a5870;
  VirtualProtect((void*)0x008716d8,4,local_8,&local_4);
  VirtualProtect((void*)0x008719a0,4,0x40,&local_8);
  *(DWORD*)0x008719a0 = (DWORD)GameHook_000a5870;
  VirtualProtect((void*)0x008719a0,4,local_8,&local_4);
  VirtualProtect((void*)0x00871b40,4,0x40,&local_8);
  *(DWORD*)0x00871b40 = (DWORD)GameHook_000a5870;
  VirtualProtect((void*)0x00871b40,4,local_8,&local_4);
  VirtualProtect((void*)0x008713b8,4,0x40,&local_8);
  *(DWORD*)0x008713b8 = (DWORD)GameHook_000a5910;
  VirtualProtect((void*)0x008713b8,4,local_8,&local_4);
  VirtualProtect((void*)0x00871580,4,0x40,&local_8);
  *(DWORD*)0x00871580 = (DWORD)GameHook_000a5910;
  VirtualProtect((void*)0x00871580,4,local_8,&local_4);
  VirtualProtect((void*)0x00871830,4,0x40,&local_8);
  *(DWORD*)0x00871830 = (DWORD)GameHook_000a59b0;
  VirtualProtect((void*)0x00871830,4,local_8,&local_4);
  VirtualProtect((void*)0x008721f8,4,0x40,&local_8);
  *(DWORD*)0x008721f8 = (DWORD)GameHook_000a5a50;
  VirtualProtect((void*)0x008721f8,4,local_8,&local_4);
  VirtualProtect((void*)0x006e0954,5,0x40,&local_8);
  *(BYTE*)0x006e0954 = 0xe8;
  *(DWORD*)0x006e0955 = (DWORD)GameHook_000a5af0 - 0x6e0959;
  VirtualProtect((void*)0x006e0954,5,local_8,&local_4);
  VirtualProtect((void*)0x006b2bcb,5,0x40,&local_8);
  *(BYTE*)0x006b2bcb = 0xe8;
  *(DWORD*)0x006b2bcc = (DWORD)GameHook_000a5af0 - 0x6b2bd0;
  VirtualProtect((void*)0x006b2bcb,5,local_8,&local_4);
  VirtualProtect((void*)0x004f77da,5,0x40,&local_8);
  *(BYTE*)0x004f77da = 0xe8;
  *(DWORD*)0x004f77db = (DWORD)GameHook_000a5af0 - 0x4f77df;
  VirtualProtect((void*)0x004f77da,5,local_8,&local_4);
  VirtualProtect((void*)0x00872a74,4,0x40,&local_8);
  *(DWORD*)0x00872a74 = (DWORD)GameHook_000a5b90;
  VirtualProtect((void*)0x00872a74,4,local_8,&local_4);
  GameInstallHook(0x5674e0,(DWORD)GameHook_000a5d20,0x5674db,g_gameHookJump00151a68,6);
  return;
}

#include "game/hooks_independent.h"
#include "game/playerped.h"
#include "game/player_colors.h"
// samp.dll RVA: 0x0018f4c8
DWORD g_hookRadarIndex;
// samp.dll RVA: 0x0018f4cc
DWORD g_hookCheatFunction;
// samp.dll RVA: 0x0018f4b0
DWORD g_hookCameraPed;
// samp.dll RVA: 0x0018f540
DWORD g_hookRenderEsi;
// samp.dll RVA: 0x0018f544
DWORD g_hookRenderEbx;
// samp.dll RVA: 0x0018f568
DWORD g_hookFreedBuffer;
// samp.dll RVA: 0x00151ab8
DWORD g_hookKeepBuffer=1;
// samp.dll RVA: 0x0018f560
DWORD g_hookCachedBuffer;
// samp.dll RVA: 0x00151abc
DWORD g_hookAllocateFunction=0x7fb270;
// samp.dll RVA: 0x0018f570
DWORD g_hookSavedEsi;
// samp.dll RVA: 0x0018f5c4
DWORD g_hookRenderReturn;
// samp.dll RVA: 0x00151ac8
BYTE g_hookRenderRestore[3]={0xff,0x52,0x20};
// samp.dll RVA: 0x0018f5d4
DWORD g_hookSavedFontA;
// samp.dll RVA: 0x0018f5d8
DWORD g_hookSavedFontB;

// Yield before entering the original game update routine.
// samp.dll RVA: 0x000a2200
__declspec(naked) void GameHook_000a2200() {
    __asm { push 1
        call dword ptr [Sleep]
        mov edx,0x561a80
        jmp edx }
}
// The original argument remains on the caller's stack; Get consumes its copy.
// samp.dll RVA: 0x000a37a0
__declspec(naked) void GameHook_000a37a0() {
    __asm { mov eax,[esp+4]
        mov g_hookRadarIndex,eax
        mov eax,g_hookRadarIndex
        push eax
        call PlayerColors::Get
        ret }
}
// samp.dll RVA: 0x000a37c0
__declspec(naked) void GameHook_000a37c0() {
    __asm { mov g_hookCheatFunction,eax
        pushad
        mov edx,0x96918c
        mov byte ptr [edx],1
        popad
        mov edx,0x438581
        cmp eax,0
        jmp edx }
}
// Intentional native one-RET callback, not a replacement for an unknown body.
// samp.dll RVA: 0x000a3ac0
__declspec(naked) void GameHook_000a3ac0() { __asm ret }
// samp.dll RVA: 0x000a3ad0
__declspec(naked) void GameHook_000a3ad0() {
    __asm { mov ebx,[esp+8]
        mov g_hookCameraPed,ebx
        pushad
        call GameFindLocalPed
        cmp g_hookCameraPed,eax
        jne original
        mov byte ptr ds:[0xc8a7c0],1
        mov byte ptr ds:[0xc8a7c1],1
    original:
        popad
        mov ebx,0x73c260
        jmp ebx }
}
// Preserve the sign bit while replacing the remaining float bits by 1.0f.
// samp.dll RVA: 0x000a3b00
__declspec(naked) void GameHook_000a3b00() {
    __asm { mov eax,[esi+0x5a4]
        and eax,0x80000000
        or eax,0x3f800000
        mov [esi+0x5a4],eax
        ret }
}
// The non-null branch continues after GTA's original PUSH EDI prologue.
// samp.dll RVA: 0x000a3b20
__declspec(naked) void GameHook_000a3b20() {
    __asm { push edi
        mov edi,[esp+8]
        test edi,edi
        jz missing
        mov eax,0x4d41c5
        jmp eax
    missing:
        pop edi
        ret }
}
// samp.dll RVA: 0x000a3ce0
__declspec(naked) void GameHook_000a3ce0() {
    __asm { mov g_hookRenderEsi,esi
        mov g_hookRenderEbx,ebx
        pushad
        mov eax,g_hookRenderEbx
        mov ecx,[eax+4]
        test ecx,ecx
        jnz present
        push g_hookRenderEsi
        mov eax,0x739a40
        call eax
        add esp,4
        popad
        mov dl,0xff
        jmp resume
    present:
        popad
        mov ecx,[ebx+4]
        mov dl,[ecx+0x36]
    resume:
        push 0x738f40
        ret }
}
// EBP is an incoming GTA frame pointer, not a compiler-owned C++ frame.
// samp.dll RVA: 0x000a3d20
__declspec(naked) void GameHook_000a3d20() {
    __asm { cmp dword ptr [esi-0x20],0x27
        mov edi,[ebp]
        jne fallback
        mov eax,[esp]
        cmp [esi-0x1c],eax
        jne fallback
        mov eax,0x738880
        jmp eax
    fallback:
        mov eax,0x7388db
        jmp eax }
}
// samp.dll RVA: 0x000a3e50
__declspec(naked) void GameHook_000a3e50() {
    __asm { test ecx,ecx
        jnz present
        ret
    present:
        mov eax,[ecx+0x1c]
        shr eax,31
        cmp eax,1
        jne original
        ret
    original:
        mov edx,0x54dfb0
        jmp edx }
}
// samp.dll RVA: 0x000a4190
__declspec(naked) void GameHook_000a4190() {
    __asm { mov eax,[esp+4]
        mov g_hookFreedBuffer,eax
        pushad
        cmp g_hookKeepBuffer,0
        je original
        cmp g_hookCachedBuffer,0
        je original
        mov eax,g_hookFreedBuffer
        cmp eax,g_hookCachedBuffer
        jne original
        popad
        mov eax,1
        ret
    original:
        popad
        push esi
        mov esi,[esp+8]
        push esi
        mov edx,g_gameHookFarClipReturn
        jmp edx }
}
// Preserve the native sticky allocator selection and four POP EBX operations.
// samp.dll RVA: 0x000a4280
__declspec(naked) void GameHook_000a4280() {
    __asm { mov eax,g_hookCachedBuffer
        test eax,eax
        jnz cached
        cmp g_gameVersion,1
        jne allocate
        mov g_hookAllocateFunction,0x7fb230
    allocate:
        push 0x604
        push 0x20
        push 0x10
        push 0x40
        mov ebx,g_hookAllocateFunction
        call ebx
        mov g_hookCachedBuffer,eax
        pop ebx
        pop ebx
        pop ebx
        pop ebx
    cached:
        mov eax,g_hookCachedBuffer
        ret }
}
// samp.dll RVA: 0x000a42c0
__declspec(naked) void GameHook_000a42c0() { __asm { mov g_hookSavedEsi,esi
        ret } }
// samp.dll RVA: 0x000a4850
__declspec(naked) void GameHook_000a4850() {
    __asm { mov eax,[esp]
        mov g_hookRenderReturn,eax
        pushad
        push 3
        push 0x4096aa
        call GameUnprotect
        mov eax,0x90909090
        mov ecx,0x4096aa
        mov word ptr [ecx],ax
        add esp,8
        mov byte ptr [ecx+2],al
        popad
        mov eax,0x533d30
        call eax
        pushad
        mov ax,word ptr g_hookRenderRestore
        mov edx,0x4096aa
        mov word ptr [edx],ax
        mov cl,byte ptr g_hookRenderRestore[2]
        mov byte ptr [edx+2],cl
        popad
        ret }
}
// samp.dll RVA: 0x000a4960
__declspec(naked) void GameHook_000a4960() {
    __asm { pushad
        push 4
        push 0x858ba4
        call GameUnprotect
        push 4
        push 0x858f84
        call GameUnprotect
        mov eax,dword ptr ds:[0x858ba4]
        mov g_hookSavedFontA,eax
        mov ecx,dword ptr ds:[0x858f84]
        mov g_hookSavedFontB,ecx
        mov dword ptr ds:[0x858ba4],0x42200000
        add esp,16
        mov dword ptr ds:[0x858f84],0x42480000
        popad
        mov eax,0x717d30
        call eax
        pushad
        mov edx,g_hookSavedFontA
        mov dword ptr ds:[0x858ba4],edx
        mov eax,g_hookSavedFontB
        mov dword ptr ds:[0x858f84],eax
        popad
        ret }
}
// samp.dll RVA: 0x000a5150
__declspec(naked) void GameHook_000a5150() {
    __asm { mov eax,[esp+4]
        test eax,eax
        jz missing
        mov eax,0x6ff350
        jmp eax
    missing:
        ret }
}
// samp.dll RVA: 0x000a5850
__declspec(naked) void GameHook_000a5850() {
    __asm { mov eax,[esp+4]
        test eax,eax
        jz skip
        mov ecx,[eax+0xfc]
        cmp ecx,0
        jne skip
        mov edx,0x563f40
        jmp edx
    skip:
        mov eax,1
        ret }
}
// Continue into GTA after its 44-byte local-frame allocation, forwarding the
// observed argument-byte override rather than translating this into a C++ call.
// samp.dll RVA: 0x000a5d20
__declspec(naked) void GameHook_000a5d20() {
    __asm { mov eax,[esp+0x14]
        test eax,eax
        jz original
        mov byte ptr [esp+0x20],1
    original:
        sub esp,0x2c
        mov eax,0x407c37
        jmp eax }
}

#include "game/hooks_render.h"
#include "game/building_removal.h"
// samp.dll RVA: 0x0018f504
DWORD g_hookHudScaleX;
// samp.dll RVA: 0x0018f508
DWORD g_hookHudScaleY;
// samp.dll RVA: 0x0018f50c
float g_hookHudWidth;
// samp.dll RVA: 0x0018f510
float g_hookHudHeight;
// samp.dll RVA: 0x00151a6e
BYTE g_hookHudAspectFix=1;
// samp.dll RVA: 0x0011c728
const float g_hookHudAspectThreshold=1.6f;
// samp.dll RVA: 0x0011eb68
const float g_hookHudScaleNumerator=0.00221999991f;
// samp.dll RVA: 0x002aca6c
DWORD g_hookFogEnabled;
// samp.dll RVA: 0x002aca70
DWORD g_hookDisableFog;
// samp.dll RVA: 0x0018e560
DWORD g_hookCollisionPed;
// samp.dll RVA: 0x0018e8e0
DWORD g_hookSavedMatrix[16];
// samp.dll RVA: 0x0018eab4
DWORD g_hookSavedSpeed[3];
// samp.dll RVA: 0x0018f52c
DWORD g_hookHornArgument;
// samp.dll RVA: 0x0018f530
DWORD g_hookHornThis;
// samp.dll RVA: 0x0018eb24
DWORD g_hookHornVehicle;
// samp.dll RVA: 0x0018f534
BYTE g_hookHornStatus;
// samp.dll RVA: 0x0018f4c4
DWORD g_hookAnimationReturn;
// samp.dll RVA: 0x0018e584
DWORD g_hookAnimationClump;
// samp.dll RVA: 0x0018f548
DWORD g_hookAnimationGroup;
// samp.dll RVA: 0x0018f54c
DWORD g_hookAnimationId;
// samp.dll RVA: 0x0018f550
DWORD g_hookAnimationBlend;
// samp.dll RVA: 0x0018eb90
DWORD g_hookAnimationPed;
// samp.dll RVA: 0x0018eb88
DWORD g_hookAnimationLocalClump;
// samp.dll RVA: 0x0018f4ec
DWORD g_hookAnimationType;
// samp.dll RVA: 0x0018f4f0
DWORD g_hookQueuedAnimationGroup;
// samp.dll RVA: 0x0018f4f4
DWORD g_hookQueuedAnimationId;
// samp.dll RVA: 0x0018f4f8
DWORD g_hookQueuedAnimationBlend;
// samp.dll RVA: 0x00151aa4
DWORD g_hookAnimationContinueA=0x4d3aaa;
// samp.dll RVA: 0x00151aa8
DWORD g_hookAnimationContinueB=0x4d4617;
// samp.dll RVA: 0x0018f5cc
DWORD g_hookRenderObject;
// samp.dll RVA: 0x00151acc
BYTE g_hookObjectRestore[5]={0xe8,0x3c,0xff,0xff,0xff};
// samp.dll RVA: 0x0018f5b4
DWORD g_hookMarker;
// samp.dll RVA: 0x0018f5b8
DWORD g_hookMarkerArgument;
// samp.dll RVA: 0x0018f5bc
DWORD g_hookSuppressAllMarkers;
// The comparison and repeated division retain x87 precision until the final
// float store. A rounded float ratio changes the native 16:10 boundary.
// samp.dll RVA: 0x000a2220
void GamePrepareHudDraw() {
    g_hookHudScaleX=*(DWORD*)0x859520;g_hookHudScaleY=*(DWORD*)0x859524;
    BYTE enabled=g_hookHudAspectFix;
    g_hookHudWidth=(float)*(int*)0xc17044;g_hookHudHeight=(float)*(int*)0xc17048;
    if(enabled) {
        __asm {
            fld g_hookHudWidth
            fdiv g_hookHudHeight
            fcomp g_hookHudAspectThreshold
            fnstsw ax
            test ah,1
            jnz done
            mov dword ptr ds:[0x859524],0x3b1e98dd
            fld g_hookHudWidth
            fdiv g_hookHudHeight
            fdivr g_hookHudScaleNumerator
            fstp dword ptr ds:[0x859520]
        done:
        }
    }
}
// samp.dll RVA: 0x000c5400
void GameSetupD3DFog(DWORD enabled) {
    if(g_hookDisableFog)enabled=0;
    if(g_gameD3DDevice) {
        g_gameD3DDevice->SetRenderState(D3DRS_FOGENABLE,enabled);
        g_gameD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE,D3DFOG_NONE);
        g_gameD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);
    }
}
// samp.dll RVA: 0x000a22c0
__declspec(naked) void GameHook_000a22c0() {
    __asm {
        pushad
        call GamePrepareHudDraw
        popad
        mov edx,0x58a330
        call edx
        pushad
        mov eax,g_hookHudScaleX
        mov dword ptr ds:[0x859520],eax
        mov ecx,g_hookHudScaleY
        mov dword ptr ds:[0x859524],ecx
        popad
        ret
    }
}
// samp.dll RVA: 0x000a29d0
__declspec(naked) void GameHook_000a29d0() {
    __asm {
        mov g_hookCameraPed,ecx
        pushad
        mov ecx,g_hookCameraPed
        mov edx,ecx
        mov g_hookCollisionPed,edx
        mov eax,[ecx+0x40]
        xor ebx,ebx
        test eax,0x40000
        jz save
        test eax,0x800000
        jz save
        test eax,0x200000
        jz save
        mov [ecx+0x44],ebx
        mov eax,g_hookCollisionPed
        mov [eax+0x48],ebx
        mov edx,g_hookCollisionPed
    save:
        mov esi,[edx+0x14]
        cmp esi,ebx
        jz original
        mov ecx,16
        mov edi,offset g_hookSavedMatrix
        rep movsd
        mov ecx,[edx+0x44]
        mov g_hookSavedSpeed[0],ecx
        mov eax,[edx+0x48]
        mov g_hookSavedSpeed[4],eax
        mov ecx,[edx+0x4c]
        mov g_hookSavedSpeed[8],ecx
    original:
        mov ecx,g_hookCameraPed
        mov edx,0x5ddbf0
        call edx
        mov ecx,g_hookCollisionPed
        mov eax,[ecx+0x40]
        test eax,0x40000
        jz done
        test eax,0x800000
        jz done
        test eax,0x200000
        jz done
        mov eax,[ecx+0x14]
        cmp eax,ebx
        jz zero_speed
        mov edx,g_hookSavedMatrix[48]
        mov [eax+0x30],edx
        mov eax,g_hookCollisionPed
        mov ecx,[eax+0x14]
        mov edx,g_hookSavedMatrix[52]
        mov [ecx+0x34],edx
        mov eax,g_hookCollisionPed
        mov ecx,[eax+0x14]
        mov edx,g_hookSavedMatrix[56]
        mov [ecx+0x38],edx
        mov ecx,g_hookCollisionPed
    zero_speed:
        mov [ecx+0x44],ebx
        mov eax,g_hookCollisionPed
        mov [eax+0x48],ebx
    done:
        popad
        ret
    }
}
// Original thiscall consumes one stack argument and restores only the status
// byte it saved, leaving the GTA callee's other results intact.
// samp.dll RVA: 0x000a33c0
__declspec(naked) void GameHook_000a33c0() {
    __asm {
        mov g_hookHornThis,ecx
        mov edx,[esp+4]
        mov g_hookHornArgument,edx
        mov eax,[edx+0x10]
        mov g_hookHornVehicle,eax
        mov eax,g_hookHornVehicle
        mov cl,[eax+0x36]
        mov g_hookHornStatus,cl
        mov ecx,[eax+0x460]
        test ecx,ecx
        jz non_player
        mov edx,[ecx+0x46c]
        test dh,1
        jz non_player
        mov byte ptr [eax+0x36],2
        jmp original
    non_player:
        mov byte ptr [eax+0x36],0x22
    original:
        push g_hookHornArgument
        mov ecx,g_hookHornThis
        mov edx,0x5002c0
        call edx
        mov edx,g_hookHornVehicle
        mov al,g_hookHornStatus
        mov [edx+0x36],al
        ret 4
    }
}
// samp.dll RVA: 0x000a3b90
__declspec(naked) void GameHook_000a3b90() {
    __asm {
        pushad
        push 0
        call GameSetupD3DFog
        add esp,4
        popad
        mov edx,0x5e7680
        call edx
        pushad
        mov eax,g_hookFogEnabled
        test eax,eax
        jz disabled
        push 1
        jmp restore
    disabled:
        push 0
    restore:
        call GameSetupD3DFog
        add esp,4
        popad
        ret
    }
}
// samp.dll RVA: 0x000a3f30
__declspec(naked) void GameHook_000a3f30() {
    __asm {
        mov edx,[esp]
        mov g_hookAnimationReturn,edx
        mov edx,[esp+4]
        mov g_hookAnimationClump,edx
        mov edx,[esp+8]
        mov g_hookAnimationGroup,edx
        mov edx,[esp+12]
        mov g_hookAnimationId,edx
        pushad
        call GameFindLocalPed
        mov g_hookAnimationPed,eax
        cmp g_hookAnimationPed,0
        jz missing
        mov eax,g_hookAnimationPed
        mov ecx,[eax+0x18]
        mov g_hookAnimationLocalClump,ecx
        jmp test_clump
    missing:
        mov g_hookAnimationLocalClump,0
    test_clump:
        mov edx,g_hookAnimationClump
        cmp edx,g_hookAnimationLocalClump
        jne original
        cmp g_hookAnimationGroup,0
        jne queue
        cmp g_hookAnimationId,0xa0
        je original
    queue:
        mov g_hookAnimationType,1
        mov eax,g_hookAnimationGroup
        mov g_hookQueuedAnimationGroup,eax
        mov ecx,g_hookAnimationId
        mov g_hookQueuedAnimationId,ecx
    original:
        popad
        mov eax,[esp+12]
        mov edx,dword ptr ds:[0xb4ea34]
        jmp dword ptr [g_hookAnimationContinueA]
    }
}
// samp.dll RVA: 0x000a3fe0
__declspec(naked) void GameHook_000a3fe0() {
    __asm {
        mov edx,[esp]
        mov g_hookAnimationReturn,edx
        mov edx,[esp+4]
        mov g_hookAnimationClump,edx
        mov edx,[esp+8]
        mov g_hookAnimationGroup,edx
        mov edx,[esp+12]
        mov g_hookAnimationId,edx
        mov edx,[esp+16]
        mov g_hookAnimationBlend,edx
        pushad
        call GameFindLocalPed
        mov g_hookAnimationPed,eax
        cmp g_hookAnimationPed,0
        jz missing
        mov eax,g_hookAnimationPed
        mov ecx,[eax+0x18]
        mov g_hookAnimationLocalClump,ecx
        jmp test_clump
    missing:
        mov g_hookAnimationLocalClump,0
    test_clump:
        mov edx,g_hookAnimationClump
        cmp edx,g_hookAnimationLocalClump
        jne original
        cmp g_hookAnimationGroup,0
        jne queue
        cmp g_hookAnimationId,0xa0
        je original
    queue:
        mov g_hookAnimationType,2
        mov eax,g_hookAnimationGroup
        mov g_hookQueuedAnimationGroup,eax
        mov ecx,g_hookAnimationId
        mov g_hookQueuedAnimationId,ecx
        mov edx,g_hookAnimationBlend
        mov g_hookQueuedAnimationBlend,edx
    original:
        popad
        sub esp,0x14
        mov ecx,[esp+0x18]
        jmp dword ptr [g_hookAnimationContinueB]
    }
}
// samp.dll RVA: 0x000a47d0
__declspec(naked) void GameHook_000a47d0() {
    __asm {
        mov eax,[esp+4]
        mov g_hookMarker,eax
        mov eax,[esp+8]
        mov g_hookMarkerArgument,eax
        pushad
        cmp g_hookMarker,0
        je override_all
        mov eax,g_hookMarker
        mov ecx,[eax+8]
        push ecx
        mov edx,g_hookMarker
        mov eax,[edx+4]
        push eax
        mov ecx,g_hookMarker
        mov edx,[ecx]
        push edx
        mov eax,g_hookMarker
        mov ecx,[eax+0x1c]
        push ecx
        call GameShouldSuppressMarker
        add esp,16
        test eax,eax
        je override_all
        mov edx,g_hookMarker
        mov dword ptr [edx+0x1c],0x4b64
    override_all:
        cmp g_hookSuppressAllMarkers,0
        je original
        mov eax,g_hookMarker
        mov dword ptr [eax+0x1c],0x4b64
    original:
        popad
        push -1
        push 0x83c931
        mov eax,0x538097
        jmp eax
    }
}
// samp.dll RVA: 0x000a48c0
__declspec(naked) void GameHook_000a48c0() {
    __asm {
        mov eax,[esp]
        mov g_hookRenderReturn,eax
        mov g_hookRenderObject,ecx
        pushad
        mov eax,g_hookRenderObject
        movsx ecx,word ptr [eax+0x22]
        cmp ecx,0xe02
        je patch
        mov edx,g_hookRenderObject
        movsx eax,word ptr [edx+0x22]
        cmp eax,0xe9f
        je patch
        mov ecx,g_hookRenderObject
        movsx edx,word ptr [ecx+0x22]
        cmp edx,0x2314
        je patch
        mov eax,g_hookRenderObject
        movsx ecx,word ptr [eax+0x22]
        cmp ecx,0x2313
        jne original
    patch:
        push 5
        push 0x71791f
        call GameUnprotect
        add esp,8
        mov edx,0x90909090
        mov eax,0x71791f
        mov [eax],edx
        mov [eax+4],dl
        popad
        mov eax,0x534030
        call eax
        pushad
        mov ecx,0x71791f
        mov edx,dword ptr g_hookObjectRestore
        mov [ecx],edx
        mov al,byte ptr g_hookObjectRestore[4]
        mov [ecx+4],al
        popad
        ret
    original:
        popad
        mov eax,0x534030
        jmp eax
    }
}

#include "game/hooks_boot.h"
#include "game/aimstuff.h"
#include "net/playerpool.h"
#include "main.h"

// samp.dll RVA: 0x0018f514
BYTE g_hookCameraHudPrepared;
// samp.dll RVA: 0x0018f6a8
DWORD g_hookAudioSavedState;
// samp.dll RVA: 0x0018f6ac
DWORD g_hookAudioArgument0;
// samp.dll RVA: 0x0018f6b0
DWORD g_hookAudioArgument1;

// Raw GTA entry ABIs: preserve live registers and native CALL/JMP/RET behavior.
// samp.dll RVA: 0x000a22f0
__declspec(naked) void GameHook_000a22f0() {
    __asm {
        pushad
        mov ecx,g_pGame
        test ecx,ecx
        mov g_hookCameraHudPrepared,0
        jz draw
        call CGame::GetPlayerPed
        test eax,eax
        jz draw
        mov ecx,g_pGame
        call CGame::GetPlayerPed
        mov al,[eax+0x2b0]
        test al,al
        jnz player_mode
        call AimStuff::GetCameraMode
        jmp check_mode
    player_mode:
        push eax
        call AimStuff::GetPlayerCameraMode
    check_mode:
        cmp al,0x35
        jne draw
        call GamePrepareHudDraw
        mov g_hookCameraHudPrepared,1
    draw:
        popad
        mov edx,0x58e020
        call edx
        pushad
        mov al,g_hookCameraHudPrepared
        test al,al
        jz finish
        mov eax,g_hookHudScaleX
        mov dword ptr ds:[0x859520],eax
        mov ecx,g_hookHudScaleY
        mov dword ptr ds:[0x859524],ecx
        mov g_hookCameraHudPrepared,0
    finish:
        popad
        ret
    }
}
// samp.dll RVA: 0x000a2370
__declspec(naked) void GameHook_000a2370() {
    __asm {
        mov edx,0x563430
        call edx
        pushad
        mov eax,g_pNetGame
        test eax,eax
        jz finish
        mov eax,[eax+0x3de]
        mov ecx,[eax+8]
        test ecx,ecx
        jz finish
        call CPlayerPool::Native013840
    finish:
        popad
        ret
    }
}
// samp.dll RVA: 0x000a23a0
__declspec(naked) void GameHook_000a23a0() {
    __asm {
        pushad
        cmp g_pGame,0
        je continue_game
        mov ecx,g_pGame
        call CGame::GetCamera
        mov ecx,eax
        call CCamera::SetToOwner
    continue_game:
        popad
        mov eax,0x52b730
        jmp eax
    }
}
// samp.dll RVA: 0x000a5b90
__declspec(naked) void GameHook_000a5b90() {
    __asm {
        mov edx,[esp+4]
        mov g_hookAudioArgument0,edx
        mov edx,[esp+8]
        mov g_hookAudioArgument1,edx
        mov eax,dword ptr ds:[0xb72914]
        mov g_hookAudioSavedState,eax
        mov eax,g_pGame
        test eax,eax
        jz play
        mov ecx,[eax]
        mov al,[ecx+4]
        test al,al
        jz play
        mov dword ptr ds:[0xb72914],1
    play:
        push g_hookAudioArgument1
        push g_hookAudioArgument0
        mov edx,0x505a00
        call edx
        mov edx,g_hookAudioSavedState
        mov dword ptr ds:[0xb72914],edx
        ret 8
    }
}

#include "game/hooks_prerender.h"
#include "game/scripting.h"
#include "net/vehiclepool.h"
#include "gui/device_objects.h"
#include "chatwindow.h"

// samp.dll RVA: 0x0018de44
CObjectPool *g_debugObjectPool;

// samp.dll RVA: 0x000a23d0
void GameHook_000a23d0() {
    if(g_pGame)g_pGame->ProcessFrameLimiter();
    if(g_pNetGame) {
        CObjectPool *objects=*(CObjectPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0x14);
        if(objects) {
            __try { objects->Process(); }
            __except(GameExceptionFilter(GetExceptionCode(),GetExceptionInformation(),"s003")) {}
        }
    }
    if(g_debugObjectPool) {
        __try { g_debugObjectPool->Process(); }
        __except(GameExceptionFilter(GetExceptionCode(),GetExceptionInformation(),"s003d")) {}
    }
    if(g_pNetGame) {
        CVehiclePool *vehicles=*(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0xc);
        if(vehicles)vehicles->ConstructLicensePlates();
    }
    if(g_pNetGame) {
        CObjectPool *objects=*(CObjectPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0x14);
        if(objects)objects->ConstructMaterials();
    }
    if(g_pChat)g_pChat->RenderToSurface();
    __try { ((void (__cdecl*)())0x53bee0)(); }
    __except(GameExceptionFilter(GetExceptionCode(),GetExceptionInformation(),"s007")) {}
    if(g_pNetGame) {
        CTextDrawPool *textdraws=*(CTextDrawPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0x20);
        if(textdraws)textdraws->Native01E330();
    }
}

#include "game/hooks_control.h"
using AimStuff::UpdateLocalSkills;
using AimStuff::ApplyPlayerSkills;
using AimStuff::ApplyLocalSkills;
using KeyStuff::UpdateKeys;
using KeyStuff::ApplyPlayerKeys;
using AimStuff::GetPlayerCameraMode;
using AimStuff::UpdateCamera;
using AimStuff::ApplyPlayerCamera;
using AimStuff::UpdateAim;
using AimStuff::ApplyPlayerAim;
using AimStuff::ApplyLocalCamera;
using AimStuff::ApplyLocalAim;
using KeyStuff::ApplyLocalKeys;
using KeyStuff::GetInternalKeys;
// Raw entry ABI: weapon ECX=ped; ped control ECX=this, [ESP+4]=ped;
// vehicle ECX=vehicle. Vehicle control preserves PUSHAD registers.
// samp.dll RVA: 0x00151964
WORD * g_hookCameraModeWord=(WORD*)0xb6f858;
// samp.dll RVA: 0x0018f4ac
BYTE g_hookSavedPlayer;
// samp.dll RVA: 0x0018f4b4
BYTE g_hookPlayerIndex;
// samp.dll RVA: 0x0018f4c0
DWORD g_hookPedControlThis;
// samp.dll RVA: 0x0018f51c
DWORD g_hookWeaponResult;
// samp.dll RVA: 0x0018f520
DWORD g_hookRestoreSkills;
// samp.dll RVA: 0x0018f524
DWORD g_hookLocalPedControl;
// samp.dll RVA: 0x0018f528
DWORD g_hookRestoreKeys;
// samp.dll RVA: 0x0018ea58
BYTE g_hookSavedCameraMode;
// samp.dll RVA: 0x0018e5c8
WORD g_hookSavedCameraModeWord;
// samp.dll RVA: 0x0018e810
DWORD g_hookVehicleProcess;
// samp.dll RVA: 0x0018e8d0
DWORD g_hookControlVehicle;
// samp.dll RVA: 0x0018ea54
DWORD g_hookVehicleVtable;
// samp.dll RVA: 0x0018e934
DWORD g_hookVehicleKeys;
// samp.dll RVA: 0x0018e938
BYTE g_hookVehicleStatus;
// samp.dll RVA: 0x0018eb20
DWORD g_hookVehicleLean;
// samp.dll RVA: 0x0018e8cc
DWORD g_hookVehicleLeanTarget;
// samp.dll RVA: 0x002a9a44
int g_gameVehicleModel;
// samp.dll RVA: 0x002a9a48
DWORD g_gameLinkedVehicle;
// samp.dll RVA: 0x002a9a4c
float g_gameVehicleDistanceSquared;
// samp.dll RVA: 0x002a9da0
DWORD g_gameLocalPedMatrix;
// samp.dll RVA: 0x002a9da4
DWORD g_gameVehicleMatrix;
// samp.dll RVA: 0x0011f5dc
const float g_gameStreamingDistanceSquared=40000.0f;
// samp.dll RVA: 0x000b3f60
BYTE __stdcall GameFindPlayerPedIndex(void *ped) {
    for(BYTE player=0;player<210;++player)if(g_gamePlayerPeds[player]==ped)return player;
    return 0;
}
// samp.dll RVA: 0x000b4410
BOOL __stdcall GameIsTrainVehicle(void *vehicle) {
    if(!vehicle)return FALSE;
    g_gameVehicleModel=*(short*)((BYTE*)vehicle+0x22);
    return g_gameVehicleModel==0x21a || g_gameVehicleModel==0x23a || g_gameVehicleModel==0x219 || g_gameVehicleModel==0x239 || g_gameVehicleModel==0x1c1;
}
// samp.dll RVA: 0x000b4460
BOOL __stdcall GameHasOccupiedLinkedVehicle(void *vehicle) {
    if(!vehicle)return FALSE;
    BYTE *linked=*(BYTE**)((BYTE*)vehicle+0x4c4);
    if(!linked)return FALSE;
    g_gameLinkedVehicle=(DWORD)linked;
    BYTE *driver=*(BYTE**)(linked+0x460);
    return driver && (*(DWORD*)(driver+0x46c)&0x100)?TRUE:FALSE;
}
// samp.dll RVA: 0x000a2ab0
__declspec(naked) void GameHook_000a2ab0() {
    __asm {
        mov dword ptr [g_hookCameraPed],ecx
        mov eax,[g_hookCameraPed]
        mov ecx,dword ptr [g_clientCrashPedContext]
        mov [g_hookCollisionPed],eax
        mov dl,byte ptr [ecx]
        push eax
        mov byte ptr [g_hookSavedPlayer],dl
        call GameFindPlayerPedIndex
        mov ecx,dword ptr [g_hookCameraPed]
        test ecx,ecx
        mov [g_hookPlayerIndex],al
        jz branch_100a2b0c
        test al,al
        jz branch_100a2b0c
        mov al,[g_hookSavedPlayer]
        test al,al
        jnz branch_100a2b0c
        call UpdateLocalSkills
        movzx eax,byte ptr [g_hookPlayerIndex]
        push eax
        call ApplyPlayerSkills
        mov dword ptr [g_hookRestoreSkills],0x1
    branch_100a2b0c:
        mov ecx,dword ptr [g_hookCameraPed]
        movsx eax,byte ptr [ecx + 0x718]
        imul eax,eax,0x1c
        mov eax,dword ptr [eax + ecx*0x1 + 0x5a0]
        push eax
        mov edx,0x5e3b60
        call edx
        mov [g_hookWeaponResult],eax
        mov eax,[g_hookRestoreSkills]
        test eax,eax
        jz branch_100a2b48
        call ApplyLocalSkills
        mov dword ptr [g_hookRestoreSkills],0x0
    branch_100a2b48:
        mov eax,[g_hookWeaponResult]
        ret
    }
}
// samp.dll RVA: 0x000a2b50
__declspec(naked) void GameHook_000a2b50() {
    __asm {
        mov dword ptr [g_hookPedControlThis],ecx
        mov eax,dword ptr [esp + 0x4]
        mov [g_hookCameraPed],eax
        mov eax,[g_clientCrashPedContext]
        mov edx,dword ptr [g_hookCameraPed]
        mov cl,byte ptr [eax]
        push edx
        mov byte ptr [g_hookSavedPlayer],cl
        call GameFindPlayerPedIndex
        mov ecx,dword ptr [g_hookCameraPed]
        test ecx,ecx
        mov [g_hookPlayerIndex],al
        jz branch_100a2ca4
        test al,al
        jz branch_100a2ca4
        mov al,[g_hookSavedPlayer]
        test al,al
        jnz branch_100a2ca4
        call UpdateKeys
        movzx eax,byte ptr [g_hookPlayerIndex]
        push eax
        call ApplyPlayerKeys
        mov ecx,dword ptr [g_aimInternalCameraMode]
        mov dl,byte ptr [ecx]
        xor eax,eax
        mov al,[g_hookPlayerIndex]
        add esp,0x4
        mov byte ptr [g_hookSavedCameraMode],dl
        push eax
        call GetPlayerCameraMode
        mov ecx,dword ptr [g_aimInternalCameraMode]
        mov byte ptr [ecx],al
        mov edx,dword ptr [g_hookCameraModeWord]
        mov ax,word ptr [edx]
        xor ecx,ecx
        mov cl,byte ptr [g_hookPlayerIndex]
        mov [g_hookSavedCameraModeWord],ax
        push ecx
        call GetPlayerCameraMode
        movzx dx,al
        mov eax,[g_hookCameraModeWord]
        mov word ptr [eax],dx
        mov eax,[g_hookCameraModeWord]
        cmp word ptr [eax],0x4
        jnz branch_100a2c11
        mov word ptr [eax],0x0
    branch_100a2c11:
        call UpdateCamera
        xor ecx,ecx
        mov cl,byte ptr [g_hookPlayerIndex]
        push ecx
        call ApplyPlayerCamera
        call UpdateAim
        movzx edx,byte ptr [g_hookPlayerIndex]
        push edx
        call ApplyPlayerAim
        call UpdateLocalSkills
        movzx eax,byte ptr [g_hookPlayerIndex]
        push eax
        call ApplyPlayerSkills
        mov ecx,dword ptr [g_clientCrashPedContext]
        mov dl,byte ptr [g_hookPlayerIndex]
        mov byte ptr [ecx],dl
        mov ecx,dword ptr [g_hookPedControlThis]
        push dword ptr [g_hookCameraPed]
        mov eax,0x624ed0
        call eax
        mov al,[g_hookSavedCameraMode]
        mov ecx,dword ptr [g_aimInternalCameraMode]
        mov byte ptr [ecx],al
        mov dx,word ptr [g_hookSavedCameraModeWord]
        mov eax,[g_hookCameraModeWord]
        mov word ptr [eax],dx
        call ApplyLocalSkills
        call ApplyLocalCamera
        mov ecx,dword ptr [g_clientCrashPedContext]
        mov byte ptr [ecx],0x0
        call ApplyLocalAim
        call ApplyLocalKeys
        jmp branch_100a2ccb
    branch_100a2ca4:
        mov dword ptr [g_hookLocalPedControl],0x1
        mov ecx,dword ptr [g_hookPedControlThis]
        push dword ptr [g_hookCameraPed]
        mov eax,0x624ed0
        call eax
        mov dword ptr [g_hookLocalPedControl],0x0
    branch_100a2ccb:
        ret 0x4
    }
}
// samp.dll RVA: 0x000a2e70
__declspec(naked) void GameHook_000a2e70() {
    __asm {
        mov dword ptr [g_hookControlVehicle],ecx
        mov eax,dword ptr [ecx]
        mov [g_hookVehicleVtable],eax
        pushad
        mov eax,[g_hookVehicleVtable]
        cmp eax,0x871120
        mov edi,0x871680
        jnz branch_100a2e9e
        mov dword ptr [g_hookVehicleProcess],0x6b1880
        jmp branch_100a2f4a
    branch_100a2e9e:
        cmp eax,0x8721a0
        jnz branch_100a2eb4
        mov dword ptr [g_hookVehicleProcess],0x6f1770
        jmp branch_100a2f4a
    branch_100a2eb4:
        cmp eax,0x871360
        jnz branch_100a2eca
        mov dword ptr [g_hookVehicleProcess],0x6b9250
        jmp branch_100a2f4a
    branch_100a2eca:
        cmp eax,0x871948
        jnz branch_100a2edd
        mov dword ptr [g_hookVehicleProcess],0x6c9260
        jmp branch_100a2f4a
    branch_100a2edd:
        cmp eax,edi
        jnz branch_100a2eed
        mov dword ptr [g_hookVehicleProcess],0x6c7050
        jmp branch_100a2f4a
    branch_100a2eed:
        cmp eax,0x871528
        jnz branch_100a2f00
        mov dword ptr [g_hookVehicleProcess],0x6bfa30
        jmp branch_100a2f4a
    branch_100a2f00:
        cmp eax,0x8717d8
        jnz branch_100a2f13
        mov dword ptr [g_hookVehicleProcess],0x6c8250
        jmp branch_100a2f4a
    branch_100a2f13:
        cmp eax,0x871ae8
        jnz branch_100a2f26
        mov dword ptr [g_hookVehicleProcess],0x6cdcc0
        jmp branch_100a2f4a
    branch_100a2f26:
        cmp eax,0x872370
        jnz branch_100a2f39
        mov dword ptr [g_hookVehicleProcess],0x6f86a0
        jmp branch_100a2f4a
    branch_100a2f39:
        cmp eax,0x871c28
        jnz branch_100a2f4a
        mov dword ptr [g_hookVehicleProcess],0x6ced20
    branch_100a2f4a:
        mov al,ds:[0x00b7cd74]
        mov ecx,dword ptr [g_hookControlVehicle]
        mov [g_hookSavedPlayer],al
        mov esi,dword ptr [ecx + 0x460]
        xor ebx,ebx
        cmp esi,ebx
        jz branch_100a3331
        cmp dword ptr [esi + 0x598],ebx
        jnz branch_100a327a
        call GameFindLocalPed
        cmp esi,eax
        jz branch_100a3274
        cmp byte ptr [g_hookSavedPlayer],bl
        jnz branch_100a3274
        push esi
        call GameFindPlayerPedIndex
        mov [g_hookPlayerIndex],al
        call UpdateKeys
        movzx ecx,byte ptr [g_hookPlayerIndex]
        push ecx
        call ApplyPlayerKeys
        mov edx,dword ptr [g_aimInternalCameraMode]
        mov al,byte ptr [edx]
        xor ecx,ecx
        mov cl,byte ptr [g_hookPlayerIndex]
        add esp,0x4
        mov [g_hookSavedCameraMode],al
        push ecx
        call GetPlayerCameraMode
        mov edx,dword ptr [g_aimInternalCameraMode]
        mov byte ptr [edx],al
        mov eax,[g_hookCameraModeWord]
        mov cx,word ptr [eax]
        xor edx,edx
        mov dl,byte ptr [g_hookPlayerIndex]
        mov word ptr [g_hookSavedCameraModeWord],cx
        push edx
        call GetPlayerCameraMode
        mov ecx,dword ptr [g_hookCameraModeWord]
        movzx ax,al
        mov word ptr [ecx],ax
        mov eax,[g_hookCameraModeWord]
        cmp word ptr [eax],0x4
        jnz branch_100a300a
        mov word ptr [eax],bx
    branch_100a300a:
        call UpdateAim
        movzx edx,byte ptr [g_hookPlayerIndex]
        push edx
        call ApplyPlayerAim
        mov eax,[g_hookControlVehicle]
        test byte ptr [eax + 0x428],0x10
        jnz branch_100a3063
        cmp dword ptr [g_hookVehicleVtable],edi
        jnz branch_100a3036
        or byte ptr [eax + 0x36],0x20
    branch_100a3036:
        call GetInternalKeys
        mov [g_hookVehicleKeys],eax
        mov word ptr [eax + 0x1c],bx
        mov eax,[g_hookVehicleKeys]
        mov word ptr [eax + 0x4c],bx
        mov ecx,dword ptr [g_hookVehicleKeys]
        mov word ptr [ecx + 0x20],bx
        mov edx,dword ptr [g_hookVehicleKeys]
        mov word ptr [edx + 0x50],bx
        jmp branch_100a3067
    branch_100a3063:
        and byte ptr [eax + 0x36],0xdf
    branch_100a3067:
        mov eax,[g_hookControlVehicle]
        mov ecx,dword ptr [eax + 0x648]
        mov dword ptr [g_hookVehicleLean],ecx
        mov edx,dword ptr [eax + 0x64c]
        mov eax,[g_clientCrashPedContext]
        mov dword ptr [g_hookVehicleLeanTarget],edx
        mov byte ptr [eax],bl
        mov ecx,dword ptr [g_hookControlVehicle]
        mov edx,dword ptr [ecx + 0x460]
        mov dword ptr [edx + 0x598],0x4
        mov eax,[g_hookControlVehicle]
        mov cl,byte ptr [eax + 0x36]
        mov byte ptr [g_hookVehicleStatus],cl
        mov byte ptr [eax + 0x36],0x1a
        mov edx,dword ptr [g_hookControlVehicle]
        lea ecx,[edx + 0x138]
        mov edx,0x502280
        call edx
        mov al,[g_hookVehicleStatus]
        mov edx,dword ptr [g_hookControlVehicle]
        mov byte ptr [edx + 0x36],al
        mov ecx,dword ptr [g_hookControlVehicle]
        mov edx,dword ptr [ecx + 0x460]
        mov dword ptr [edx + 0x598],ebx
        mov al,[g_hookPlayerIndex]
        mov ecx,dword ptr [g_clientCrashPedContext]
        mov byte ptr [ecx],al
        mov eax,[g_hookControlVehicle]
        mov esi,dword ptr [eax + 0x14]
        cmp esi,ebx
        jz branch_100a3126
        mov ecx,0x10
        mov edi,offset g_hookSavedMatrix
        rep movsd
        mov edx,dword ptr [eax + 0x44]
        mov dword ptr [g_hookSavedSpeed],edx
        mov ecx,dword ptr [eax + 0x48]
        mov dword ptr [g_hookSavedSpeed+4],ecx
        mov edx,dword ptr [eax + 0x4c]
        mov dword ptr [g_hookSavedSpeed+8],edx
    branch_100a3126:
        mov ecx,dword ptr [g_hookControlVehicle]
        mov eax,[g_hookVehicleProcess]
        call eax
        mov eax,[g_hookControlVehicle]
        cmp dword ptr [eax + 0x14],ebx
        jz branch_100a3169
        push eax
        call GameIsVehicleInStreamingArea
        test eax,eax
        jnz branch_100a3169
        mov eax,[g_hookControlVehicle]
        push eax
        call GameIsTrainVehicle
        test eax,eax
        jnz branch_100a3169
        mov ecx,dword ptr [g_hookControlVehicle]
        mov edx,dword ptr [ecx + 0x14]
        mov eax,[g_hookSavedMatrix+56]
        mov dword ptr [edx + 0x38],eax
        jmp branch_100a3198
    branch_100a3169:
        mov edx,dword ptr [g_hookControlVehicle]
        mov ecx,dword ptr [edx + 0x14]
        cmp ecx,ebx
        jz branch_100a31a7
        mov eax,[g_hookVehicleProcess]
        cmp eax,0x6f1770
        jz branch_100a3190
        cmp eax,0x6c9260
        jz branch_100a3190
        cmp eax,0x6c7050
        jnz branch_100a31ac
    branch_100a3190:
        mov eax,[g_hookSavedMatrix+56]
        mov dword ptr [ecx + 0x38],eax
    branch_100a3198:
        fld dword ptr [g_hookSavedSpeed+8]
        mov ecx,dword ptr [g_hookControlVehicle]
        fstp dword ptr [ecx + 0x4c]
    branch_100a31a7:
        mov eax,[g_hookVehicleProcess]
    branch_100a31ac:
        cmp eax,0x6bfa30
        jz branch_100a31be
        cmp eax,0x6b9250
        jnz branch_100a323f
    branch_100a31be:
        mov eax,[g_hookControlVehicle]
        mov edx,dword ptr [g_hookVehicleLean]
        mov dword ptr [eax + 0x648],edx
        fld dword ptr [g_hookVehicleLeanTarget]
        mov ecx,dword ptr [g_hookControlVehicle]
        fstp dword ptr [ecx + 0x64c]
        mov ecx,dword ptr [g_hookControlVehicle]
        fld dword ptr [ecx + 0x64c]
        fcomp dword ptr [ecx + 0x648]
        fld dword ptr [ecx + 0x64c]
        fnstsw ax
        test ah,0x41
        jnz branch_100a3214
        fsub dword ptr [ecx + 0x648]
        fmul dword ptr [g_textDrawHalf]
        fadd dword ptr [ecx + 0x648]
        jmp branch_100a3239
    branch_100a3214:
        fcomp dword ptr [ecx + 0x648]
        fnstsw ax
        test ah,0x5
        jp branch_100a323f
        fld dword ptr [ecx + 0x648]
        fsub dword ptr [ecx + 0x64c]
        fmul dword ptr [g_textDrawHalf]
        fsubr dword ptr [ecx + 0x648]
    branch_100a3239:
        fstp dword ptr [ecx + 0x648]
    branch_100a323f:
        mov edx,dword ptr [g_clientCrashPedContext]
        mov byte ptr [edx],bl
        call ApplyLocalKeys
        mov al,[g_hookSavedCameraMode]
        mov ecx,dword ptr [g_aimInternalCameraMode]
        mov byte ptr [ecx],al
        mov dx,word ptr [g_hookSavedCameraModeWord]
        mov eax,[g_hookCameraModeWord]
        mov word ptr [eax],dx
        call ApplyLocalAim
        popad
        ret
    branch_100a3274:
        mov ecx,dword ptr [g_hookControlVehicle]
    branch_100a327a:
        cmp esi,ebx
        jz branch_100a3331
        cmp dword ptr [esi + 0x598],ebx
        jnz branch_100a3331
        call GameFindLocalPed
        mov ecx,dword ptr [g_hookControlVehicle]
        cmp dword ptr [ecx + 0x460],eax
        jnz branch_100a3331
        test byte ptr [ecx + 0x428],0x10
        jnz branch_100a32f6
        cmp dword ptr [g_hookVehicleVtable],edi
        jnz branch_100a32ba
        or byte ptr [ecx + 0x36],0x20
    branch_100a32ba:
        mov dword ptr [g_hookRestoreKeys],0x1
        call UpdateKeys
        call GetInternalKeys
        mov [g_hookVehicleKeys],eax
        mov word ptr [eax + 0x1c],bx
        mov ecx,dword ptr [g_hookVehicleKeys]
        mov word ptr [ecx + 0x4c],bx
        mov edx,dword ptr [g_hookVehicleKeys]
        mov word ptr [edx + 0x20],bx
        mov eax,[g_hookVehicleKeys]
        mov word ptr [eax + 0x50],bx
        jmp branch_100a32fa
    branch_100a32f6:
        and byte ptr [ecx + 0x36],0xdf
    branch_100a32fa:
        mov edx,dword ptr [g_hookControlVehicle]
        lea ecx,[edx + 0x138]
        mov edx,0x502280
        call edx
        mov ecx,dword ptr [g_hookControlVehicle]
        mov eax,[g_hookVehicleProcess]
        call eax
        cmp dword ptr [g_hookRestoreKeys],ebx
        jz branch_100a332d
        mov dword ptr [g_hookRestoreKeys],ebx
        call ApplyLocalKeys
    branch_100a332d:
        popad
        ret
    branch_100a3331:
        push ecx
        call GameHasOccupiedLinkedVehicle
        test eax,eax
        jnz branch_100a335b
        mov ecx,dword ptr [g_hookControlVehicle]
        push ecx
        call GameIsTrainVehicle
        test eax,eax
        jnz branch_100a335b
        mov edx,dword ptr [g_hookControlVehicle]
        push edx
        call GameIsVehicleInStreamingArea
        test eax,eax
        jz branch_100a33ad
    branch_100a335b:
        mov eax,[g_hookControlVehicle]
        mov esi,dword ptr [eax + 0x14]
        cmp esi,ebx
        jz branch_100a338d
        mov ecx,0x10
        mov edi,offset g_hookSavedMatrix
        rep movsd
        mov ecx,dword ptr [eax + 0x44]
        mov dword ptr [g_hookSavedSpeed],ecx
        mov edx,dword ptr [eax + 0x48]
        mov dword ptr [g_hookSavedSpeed+4],edx
        mov eax,dword ptr [eax + 0x4c]
        mov [g_hookSavedSpeed+8],eax
    branch_100a338d:
        mov edx,dword ptr [g_hookControlVehicle]
        lea ecx,[edx + 0x138]
        mov edx,0x502280
        call edx
        mov ecx,dword ptr [g_hookControlVehicle]
        mov eax,[g_hookVehicleProcess]
        call eax
    branch_100a33ad:
        popad
        ret
    }
}
// Retain the unrounded x87 threshold test and native argument cleanup.
// samp.dll RVA: 0x000b6490
__declspec(naked) BOOL __stdcall GameIsVehicleInStreamingArea(void *vehicle) {
    __asm {
        push ebp
        mov ebp,esp
        push ecx
        mov eax,ds:[0x00b7cd98]
        mov eax,dword ptr [eax + 0x14]
        test eax,eax
        mov ecx,dword ptr [ebp + 0x8]
        mov [g_gameLocalPedMatrix],eax
        mov ecx,dword ptr [ecx + 0x14]
        mov dword ptr [g_gameVehicleMatrix],ecx
        jz branch_100b6519
        test ecx,ecx
        jz branch_100b6519
        fld dword ptr [ecx + 0x34]
        mov edx,dword ptr [eax + 0x34]
        fld dword ptr [ecx + 0x30]
        add ecx,0x30
        fsubr dword ptr [eax + 0x30]
        mov dword ptr [ebp + 0x8],edx
        fld dword ptr [ebp + 0x8]
        fsub st(0),st(2)
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fst dword ptr [g_gameVehicleDistanceSquared]
        fstp st(3)
        fstp st(0)
        fstp st(0)
        fcomp dword ptr [g_gameStreamingDistanceSquared]
        fnstsw ax
        test ah,0x41
        jp branch_100b6519
        mov dword ptr [ebp + -0x4],ecx
        mov byte ptr [ebp + 0xb],0x0
        push 0x0
        push dword ptr [ebp + -0x4]
        mov edx,0x410ce0
        call edx
        mov byte ptr [ebp + 0xb],al
        pop edx
        pop edx
        mov al,byte ptr [ebp + 0xb]
        test al,al
        jz branch_100b6519
        mov eax,0x1
        mov esp,ebp
        pop ebp
        ret 0x4
    branch_100b6519:
        xor eax,eax
        mov esp,ebp
        pop ebp
        ret 0x4
    }
}

#include "game/hooks_vehicle.h"
#include "exceptions.h"
// samp.dll RVA: 0x0018f690
DWORD g_hookContactThis;
// samp.dll RVA: 0x0018f694
DWORD g_hookContactOther;
// samp.dll RVA: 0x0018f698
DWORD g_hookContactReturn;
// samp.dll RVA: 0x0018f69c
DWORD g_hookSirenVehicle;
// samp.dll RVA: 0x0018f6a0
BYTE g_hookSirenResult;
// samp.dll RVA: 0x0018f6a4
DWORD g_hookSirenOnce;
// samp.dll RVA: 0x00151b0c
WORD g_hookSirenVehicleId=0xffff;
// samp.dll RVA: 0x0018f63c
DWORD g_hookPedQueryThis;
// samp.dll RVA: 0x0018f640
DWORD g_hookPedQueryPed;

// Raw GTA collision method ABI; suppression returns with RET8.
// samp.dll RVA: 0x000a5870
__declspec(naked) void GameHook_000a5870() {
    __asm {
        mov g_hookContactThis,ecx
        mov edx,[esp]
        mov g_hookContactReturn,edx
        mov edx,[esp+4]
        mov g_hookContactOther,edx
        pushad
        cmp g_pNetGame,0
        je original
        mov eax,g_pNetGame
        movzx ecx,byte ptr [eax+0x232]
        test ecx,ecx
        je original
        cmp g_hookContactThis,0
        je original
        cmp g_hookContactOther,0
        je original
        mov edx,g_hookContactOther
        movsx eax,word ptr [edx+0x22]
        cmp eax,400
        jl original
        mov ecx,g_hookContactOther
        movsx edx,word ptr [ecx+0x22]
        cmp edx,611
        jg original
        mov eax,g_hookContactThis
        cmp dword ptr [eax+0x460],0
        je original
        mov ecx,g_hookContactOther
        cmp dword ptr [ecx+0x460],0
        je original
        popad
        mov eax,0
        ret 8
    original:
        popad
        mov edx,0x6ace70
        jmp edx
    }
}

// Raw GTA collision method ABI; suppression returns with RET8.
// samp.dll RVA: 0x000a5910
__declspec(naked) void GameHook_000a5910() {
    __asm {
        mov g_hookContactThis,ecx
        mov edx,[esp]
        mov g_hookContactReturn,edx
        mov edx,[esp+4]
        mov g_hookContactOther,edx
        pushad
        cmp g_pNetGame,0
        je original
        mov eax,g_pNetGame
        movzx ecx,byte ptr [eax+0x232]
        test ecx,ecx
        je original
        cmp g_hookContactThis,0
        je original
        cmp g_hookContactOther,0
        je original
        mov edx,g_hookContactOther
        movsx eax,word ptr [edx+0x22]
        cmp eax,400
        jl original
        mov ecx,g_hookContactOther
        movsx edx,word ptr [ecx+0x22]
        cmp edx,611
        jg original
        mov eax,g_hookContactThis
        cmp dword ptr [eax+0x460],0
        je original
        mov ecx,g_hookContactOther
        cmp dword ptr [ecx+0x460],0
        je original
        popad
        mov eax,0
        ret 8
    original:
        popad
        mov edx,0x6bdea0
        jmp edx
    }
}

// Raw GTA collision method ABI; suppression returns with RET8.
// samp.dll RVA: 0x000a59b0
__declspec(naked) void GameHook_000a59b0() {
    __asm {
        mov g_hookContactThis,ecx
        mov edx,[esp]
        mov g_hookContactReturn,edx
        mov edx,[esp+4]
        mov g_hookContactOther,edx
        pushad
        cmp g_pNetGame,0
        je original
        mov eax,g_pNetGame
        movzx ecx,byte ptr [eax+0x232]
        test ecx,ecx
        je original
        cmp g_hookContactThis,0
        je original
        cmp g_hookContactOther,0
        je original
        mov edx,g_hookContactOther
        movsx eax,word ptr [edx+0x22]
        cmp eax,400
        jl original
        mov ecx,g_hookContactOther
        movsx edx,word ptr [ecx+0x22]
        cmp edx,611
        jg original
        mov eax,g_hookContactThis
        cmp dword ptr [eax+0x460],0
        je original
        mov ecx,g_hookContactOther
        cmp dword ptr [ecx+0x460],0
        je original
        popad
        mov eax,0
        ret 8
    original:
        popad
        mov edx,0x6c8ae0
        jmp edx
    }
}

// Raw GTA collision method ABI; suppression returns with RET8.
// samp.dll RVA: 0x000a5a50
__declspec(naked) void GameHook_000a5a50() {
    __asm {
        mov g_hookContactThis,ecx
        mov edx,[esp]
        mov g_hookContactReturn,edx
        mov edx,[esp+4]
        mov g_hookContactOther,edx
        pushad
        cmp g_pNetGame,0
        je original
        mov eax,g_pNetGame
        movzx ecx,byte ptr [eax+0x232]
        test ecx,ecx
        je original
        cmp g_hookContactThis,0
        je original
        cmp g_hookContactOther,0
        je original
        mov edx,g_hookContactOther
        movsx eax,word ptr [edx+0x22]
        cmp eax,400
        jl original
        mov ecx,g_hookContactOther
        movsx edx,word ptr [ecx+0x22]
        cmp edx,611
        jg original
        mov eax,g_hookContactThis
        cmp dword ptr [eax+0x460],0
        je original
        mov ecx,g_hookContactOther
        cmp dword ptr [ecx+0x460],0
        je original
        popad
        mov eax,0
        ret 8
    original:
        popad
        mov edx,0x546d00
        jmp edx
    }
}
// samp.dll RVA: 0x000a5820
__declspec(naked) void GameHook_000a5820() {
    __asm {
        pushad
        cmp g_pNetGame,0
        je original
        mov eax,g_pNetGame
        movzx ecx,byte ptr [eax+0x232]
        test ecx,ecx
        je original
        popad
        xor eax,eax
        ret
    original:
        popad
        mov eax,0x41a990
        jmp eax
    }
}
// samp.dll RVA: 0x000a5100
__declspec(naked) void GameHook_000a5100() {
    __asm {
        mov g_hookPedQueryThis,ecx
        mov eax,[esp+4]
        mov g_hookPedQueryPed,eax
        pushad
        cmp g_pNetGame,0
        je original
        call GameFindLocalPed
        cmp g_hookPedQueryPed,eax
        jne original
        mov eax,g_clientCrashPedContext
        movzx ecx,byte ptr [eax]
        test ecx,ecx
        je original
        popad
        xor eax,eax
        mov al,1
        ret 4
    original:
        popad
        sub esp,12
        push esi
        mov esi,ecx
        mov eax,0x4b35a6
        jmp eax
    }
}
// Raw siren query returns AL. The one-shot path deliberately leaves upper EAX.
// samp.dll RVA: 0x000a5af0
__declspec(naked) void GameHook_000a5af0() {
    __asm {
        mov eax,g_hookSirenOnce
        test eax,eax
        je backend
        mov g_hookSirenOnce,0
        mov al,1
        ret
    backend:
        mov g_hookSirenVehicle,ecx
        mov edx,0x6d8470
        call edx
        mov g_hookSirenResult,al
        mov al,g_hookSirenResult
        test al,al
        jne finish
        pushad
        mov eax,g_pNetGame
        test eax,eax
        je restore
        mov eax,[eax+0x3de]
        mov ecx,[eax+0xc]
        test ecx,ecx
        je restore
        mov edx,g_hookSirenVehicle
        push edx
        call CVehiclePool::Find
        cmp ax,0xffff
        mov g_hookSirenVehicleId,ax
        je restore
        mov ecx,g_pNetGame
        mov edx,[ecx+0x3de]
        mov ecx,[edx+0xc]
        push eax
        call CVehiclePool::Get
        mov ecx,eax
        call CVehicle::HasSiren
        test eax,eax
        je restore
        mov g_hookSirenResult,1
    restore:
        popad
    finish:
        mov al,g_hookSirenResult
        ret
    }
}

#include "game/task_events.h"
#include "net/actorpool.h"
#include "net/remoteplayer.h"
#include "gui/render_dependencies.h"
// samp.dll RVA: 0x0018e81c
CVehicle *g_hookEnterVehicle;
// samp.dll RVA: 0x000a3430
BOOL GameNotifyEnterVehicle() {
    if(!g_pNetGame)return TRUE;
    BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);
    g_hookEventVehiclePool=*(CVehiclePool**)(pools+0xc);
    CPlayerPool *players=*(CPlayerPool**)(pools+8);
    g_hookEventLocalPlayer=*(CLocalPlayer**)((BYTE*)players+0x1e);
    g_hookEventVehicleId=g_hookEventVehiclePool->Find((void*)g_hookControlVehicle);
    WORD id=g_hookEventVehicleId;
    if(id==0xffff || id>=2000 || !g_hookEventVehiclePool->m_bNotEmpty[id])return FALSE;
    g_hookEnterVehicle=g_hookEventVehiclePool->m_pObjects[id];
    if(*(DWORD*)((BYTE*)g_hookEnterVehicle+0x60) ||
        *(short*)((BYTE*)g_hookEnterVehicle->m_pGameVehicle+0x22)==0x23a)return FALSE;
    if(g_hookEventLocalPlayer) {
        CPed *ped=*(CPed**)g_hookEventLocalPlayer;
        if(ped && ped->HasDanceState())return FALSE;
        if(g_hookEventLocalPlayer) {
            ped=*(CPed**)g_hookEventLocalPlayer;
            if(ped && ped->HasDanceStyle())return FALSE;
        }
    }
    BYTE *driver=*(BYTE**)((BYTE*)g_hookEnterVehicle->m_pGameVehicle+0x460);
    if(driver) {
        if(*(DWORD*)(driver+0x598))return FALSE;
        WORD player=players->Find(driver);
        if(players->IsConnected(player) && players->GetPlayerInfoFlag(player))return FALSE;
    }
    // Native deliberately has no local-player null guard on this final path.
    CPed *ped=*(CPed**)g_hookEventLocalPlayer;
    if(ped && ped->GetCurrentWeapon()==46) {
        ped=*(CPed**)g_hookEventLocalPlayer;ped->SetArmedWeapon(0,0);
    }
    g_hookEventLocalPlayer->EnterVehicle(g_hookEventVehicleId,FALSE);
    return TRUE;
}
// samp.dll RVA: 0x000a3840
WORD GameResolveDamagePlayer(void *entity) {
    if(!entity)return 0xffff;
    CPlayerPool *players=*(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+8);
    WORD result=players->Find(entity);
    if(result==0xffff) {
        CVehiclePool *vehicles=*(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0xc);
        WORD vehicle=vehicles->Find(entity);
        if(vehicle!=0xffff) {
            BYTE *state=(BYTE*)players;LONG last=*(LONG*)(state+0x22);
            for(DWORD slot=0;(LONG)slot<=last;++slot) {
                WORD player=(WORD)slot;
                if(player<1004 && *(DWORD*)(state+0xfd6+player*4)) {
                    BYTE *entry=*(BYTE**)(state+0x26+player*4);
                    BYTE *remote=entry?*(BYTE**)(entry+8):0;
                    if(*(WORD*)(remote+2)==vehicle){result=player;break;}
                }
            }
        }
    }
    return result;
}
// samp.dll RVA: 0x000a38e0
DWORD GameResolveDamageActor(void *entity) {
    if(!entity)return 0xffff;
    CActorPool *actors=*(CActorPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+4);
    return actors->Find(entity);
}
// Observed first four DWORDs only, not a complete native allocation layout.
struct GameDamageResponsePrefix { void *entity;float damage;DWORD bodyPart,weapon; };
// samp.dll RVA: 0x000a3910
BOOL __stdcall GameNotifyPedDamage(void *response,void *ped) {
    void *local=GameFindLocalPed();
    if(!g_pNetGame)return FALSE;
    GameDamageResponsePrefix *damage=(GameDamageResponsePrefix*)response;
    void *attacker=damage->entity;
    if(attacker!=local && ped && *(DWORD*)ped==0x86c0a8)return TRUE;
    bool taken=ped==local;
    if(!taken && attacker!=local)return FALSE;
    CPlayerPool *players=*(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+8);
    WORD player;
    if(taken) {
        BYTE *localPlayer=*(BYTE**)((BYTE*)players+0x1e);
        BYTE *protectedObject=*(BYTE**)(localPlayer+0x2de);
        if(protectedObject) {
            if(*(void**)(protectedObject+0x40)==attacker || damage->weapon==54)return TRUE;
        }
        player=GameResolveDamagePlayer(attacker);
        (*(CLocalPlayer**)((BYTE*)players+0x1e))->SendTakeDamage(player,damage->damage,damage->weapon,damage->bodyPart);
    } else {
        player=GameResolveDamagePlayer(ped);
        if(player==0xffff) {
            DWORD actor=GameResolveDamageActor(ped);
            if((WORD)actor!=0xffff) {
                (*(CLocalPlayer**)((BYTE*)players+0x1e))->SendActorDamage((WORD)actor,damage->damage,damage->weapon,damage->bodyPart);
                return TRUE;
            }
        } else {
            (*(CLocalPlayer**)((BYTE*)players+0x1e))->SendGiveDamage(player,damage->damage,damage->weapon,damage->bodyPart);
            if(*(DWORD*)((BYTE*)players->GetPlayer(player)+0x1c))return TRUE;
        }
    }
    BYTE *settings=*(BYTE**)((BYTE*)g_pNetGame+0x3d5);
    if(settings[0x22]) {
        BYTE team=(*(BYTE**)((BYTE*)players+0x1e))[0x130];
        if(team!=0xff && player!=0xffff) {
            CPlayerPool *reloaded=*(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+8);
            if(((BYTE*)reloaded->GetPlayer(player))[0x19]==team)return TRUE;
        }
    }
    return FALSE;
}

#include "net/netgame.h"
#include "net/gangzonepool.h"
#include "net/pickuppool.h"
#include "net/menupool.h"
#include "game/util.h"

// samp.dll RVA: 0x0018e7f8
DWORD g_hookPickupBackend;
// samp.dll RVA: 0x0018e5cc
DWORD g_hookMenuTextResult;
// samp.dll RVA: 0x0018f5a8
CVector g_hookPassengerCameraPosition;
// samp.dll RVA: 0x0018e580
DWORD g_hookTrailerBackend;
// samp.dll RVA: 0x0018e7fc
CVehiclePool * g_hookTrailerPool;
// samp.dll RVA: 0x0018e7f0
WORD g_hookTrailerId;
// samp.dll RVA: 0x0018f5e0
CVehicle * g_hookTrailerVehicle;
// samp.dll RVA: 0x0018f600
DWORD g_hookObjectSavedStack;
// samp.dll RVA: 0x0018f5f8
DWORD g_hookObjectBackend;
// samp.dll RVA: 0x0018f604
DWORD g_hookObjectResult;
// samp.dll RVA: 0x0018f5fc
CObject * g_hookObjectRotationWrapper;
// samp.dll RVA: 0x0018f614
CObjectPool * g_hookObjectDrawPool;
// samp.dll RVA: 0x0018f618
DWORD g_hookObjectDrawBackend;
// samp.dll RVA: 0x0018f578
CObject * g_hookObjectDrawWrapper;

// Raw installed GTA ABI; void() is an address declaration only.
// samp.dll RVA: 0x000a3c60
__declspec(naked) void GameHook_000a3c60() {
    __asm {
        pushad
        mov eax, dword ptr [g_pNetGame]
        test eax, eax
        je hook_a3c60_a3c7c
        mov eax, dword ptr [eax + 0x3de]
        mov ecx, dword ptr [eax + 0x18]
        test ecx, ecx
        je hook_a3c60_a3c7c
        call CGangZonePool::Draw
    hook_a3c60_a3c7c:
        popad
        ret
    }
}

// Raw installed GTA ABI; void() is an address declaration only.
// samp.dll RVA: 0x000a3c80
__declspec(naked) void GameHook_000a3c80() {
    __asm {
        pushad
        mov eax, dword ptr [g_pNetGame]
        test eax, eax
        je hook_a3c80_a3c9e
        mov eax, dword ptr [eax + 0x3d5]
        cmp byte ptr [eax], 0
        je hook_a3c80_a3c9e
        popad
        mov dword ptr [esi + 0x4d4], eax
        jmp hook_a3c80_a3c9f
    hook_a3c80_a3c9e:
        popad
    hook_a3c80_a3c9f:
        ret
    }
}

// Raw installed GTA ABI; void() is an address declaration only.
// samp.dll RVA: 0x000a3ca0
__declspec(naked) void GameHook_000a3ca0() {
    __asm {
        mov dword ptr [g_hookPickupBackend], esi
        pushad
        mov eax, dword ptr [g_pNetGame]
        test eax, eax
        je hook_a3ca0_a3cd2
        mov eax, dword ptr [eax + 0x3de]
        mov ecx, dword ptr [eax + 0x10]
        test ecx, ecx
        je hook_a3ca0_a3cd2
        mov edx, dword ptr [g_hookPickupBackend]
        add edx, 0xff687740
        shr edx, 5
        push edx
        call CPickupPool::SendNotification
    hook_a3ca0_a3cd2:
        popad
        mov al, byte ptr [esi + 0x1c]
        cmp al, 6
        push 0x4579cb
        ret
    }
}

// Raw installed GTA ABI; void() is an address declaration only.
// samp.dll RVA: 0x000a3d90
__declspec(naked) void GameHook_000a3d90() {
    __asm {
        sub esp, 0x20
        push esi
        push edi
        mov edi, dword ptr [esp + 0x2c]
        pushad
        mov dword ptr [g_hookPedControlThis], edi
        mov dword ptr [g_hookMenuTextResult], 0
        cmp dword ptr [g_pNetGame], 0
        je hook_a3d90_a3e1c
        mov eax, dword ptr [g_hookPedControlThis]
        movsx ecx, byte ptr [eax]
        cmp ecx, 0x53
        jne hook_a3d90_a3e1c
        mov edx, dword ptr [g_hookPedControlThis]
        movsx eax, byte ptr [edx + 1]
        cmp eax, 0x41
        jne hook_a3d90_a3e1c
        mov ecx, dword ptr [g_hookPedControlThis]
        movsx edx, byte ptr [ecx + 2]
        cmp edx, 0x4d
        jne hook_a3d90_a3e1c
        mov eax, dword ptr [g_hookPedControlThis]
        movsx ecx, byte ptr [eax + 3]
        cmp ecx, 0x50
        jne hook_a3d90_a3e1c
        mov ecx, dword ptr [g_pNetGame]
        call CNetGame::GetMenuPool
        test eax, eax
        je hook_a3d90_a3e1c
        mov edx, dword ptr [g_hookPedControlThis]
        add edx, 4
        push edx
        mov ecx, dword ptr [g_pNetGame]
        call CNetGame::GetMenuPool
        mov ecx, eax
        call CMenuPool::GetTextPointer
        mov dword ptr [g_hookMenuTextResult], eax
    hook_a3d90_a3e1c:
        popad
        cmp dword ptr [g_hookMenuTextResult], 0
        je hook_a3d90_a3e33
        mov eax, dword ptr [g_hookMenuTextResult]
        pop edi
        pop esi
        add esp, 0x20
        ret 4
    hook_a3d90_a3e33:
        mov eax, 0x6a0059
        jmp eax
    }
}

// Raw installed GTA ABI; void() is an address declaration only.
// samp.dll RVA: 0x000a4770
__declspec(naked) void GameHook_000a4770() {
    __asm {
        cmp dword ptr [g_pGame], 0
        je hook_a4770_a47bb
        mov ecx, dword ptr [g_pGame]
        call CGame::GetPlayerPed
        test eax, eax
        je hook_a4770_a47bb
        mov ecx, dword ptr [g_pGame]
        call CGame::GetPlayerPed
        mov ecx, eax
        call CPed::IsInVehicle
        test eax, eax
        je hook_a4770_a47bb
        mov ecx, dword ptr [g_pGame]
        call CGame::GetPlayerPed
        mov ecx, eax
        call CPed::IsPassenger
        test eax, eax
        je hook_a4770_a47bb
        lea eax, [g_hookPassengerCameraPosition]
        ret
    hook_a4770_a47bb:
        mov edx, 0x56e090
        jmp edx
    }
}

// Raw installed GTA ABI; void() is an address declaration only.
// samp.dll RVA: 0x000a49d0
__declspec(naked) void GameHook_000a49d0() {
    __asm {
        mov dword ptr [g_hookTrailerBackend], ecx
        pushad
        cmp dword ptr [g_pNetGame], 0
        je hook_a49d0_a4a90
        cmp dword ptr [g_vehicleDetachingTrailer], 0
        jne hook_a49d0_a4a90
        cmp dword ptr [g_hookTrailerBackend], 0
        je hook_a49d0_a4a90
        mov eax, dword ptr [g_hookTrailerBackend]
        fld dword ptr [eax + 0x4c0]
        fcomp dword ptr [g_objectScaleLowerBound]
        fnstsw ax
        test ah, 0x41
        jne hook_a49d0_a4a90
        mov ecx, dword ptr [g_pNetGame]
        call CNetGame::GetVehiclePool
        mov dword ptr [g_hookTrailerPool], eax
        mov ecx, dword ptr [g_hookTrailerBackend]
        mov edx, dword ptr [ecx + 0x4c4]
        push edx
        mov ecx, dword ptr [g_hookTrailerPool]
        call CVehiclePool::Find
        mov word ptr [g_hookTrailerId], ax
        movzx eax, word ptr [g_hookTrailerId]
        cmp eax, 0xffff
        je hook_a49d0_a4a90
        mov cx, word ptr [g_hookTrailerId]
        push ecx
        mov ecx, dword ptr [g_hookTrailerPool]
        call CVehiclePool::Get
        mov dword ptr [g_hookTrailerVehicle], eax
        cmp dword ptr [g_hookTrailerVehicle], 0
        je hook_a49d0_a4a90
        mov ecx, dword ptr [g_hookTrailerVehicle]
        call CVehicle::DriverIsPlayerPed
        test eax, eax
        jne hook_a49d0_a4a90
        mov edx, dword ptr [g_hookTrailerVehicle]
        cmp dword ptr [edx + 0x48], 0
        je hook_a49d0_a4a90
        popad
        ret
    hook_a49d0_a4a90:
        popad
        mov eax, 0x6a4400
        jmp eax
    }
}

// Raw installed GTA ABI; void() is an address declaration only.
// samp.dll RVA: 0x000a4c30
__declspec(naked) void GameHook_000a4c30() {
    __asm {
        mov dword ptr [g_hookObjectSavedStack], esp
        mov dword ptr [g_hookObjectBackend], ecx
        pushad
        mov ecx, dword ptr [g_hookObjectBackend]
        mov ebx, 0x546ff0
        call ebx
        mov dword ptr [g_hookObjectResult], eax
        mov eax, dword ptr [g_pNetGame]
        test eax, eax
        je hook_a4c30_a4c81
        mov eax, dword ptr [eax + 0x3de]
        mov ecx, dword ptr [eax + 0x14]
        test ecx, ecx
        je hook_a4c30_a4c81
        mov edx, dword ptr [g_hookObjectBackend]
        push edx
        call CObjectPool::Find
        test eax, eax
        mov dword ptr [g_hookObjectRotationWrapper], eax
        je hook_a4c30_a4c81
        mov ecx, eax
        call CObject::UpdateAttachedObjectRotation
    hook_a4c30_a4c81:
        popad
        mov eax, dword ptr [g_hookObjectResult]
        ret
    }
}

// Raw installed GTA ABI; void() is an address declaration only.
// samp.dll RVA: 0x000a4df0
__declspec(naked) void GameHook_000a4df0() {
    __asm {
        mov eax, 0x553aa0
        call eax
        mov eax, dword ptr [g_pNetGame]
        test eax, eax
        je hook_a4df0_a4e18
        mov eax, dword ptr [eax + 0x3de]
        mov ecx, dword ptr [eax + 0x14]
        test ecx, ecx
        mov dword ptr [g_hookObjectDrawPool], ecx
        je hook_a4df0_a4e18
        call CObjectPool::Draw
    hook_a4df0_a4e18:
        mov ecx, dword ptr [g_debugObjectPool]
        test ecx, ecx
        je hook_a4df0_a4e27
        call CObjectPool::Draw
    hook_a4df0_a4e27:
        ret
    }
}

// Raw installed GTA ABI; void() is an address declaration only.
// samp.dll RVA: 0x000a4e30
__declspec(naked) void GameHook_000a4e30() {
    __asm {
        mov eax, 0x5531e0
        call eax
        mov eax, dword ptr [g_pNetGame]
        test eax, eax
        je hook_a4e30_a4e58
        mov eax, dword ptr [eax + 0x3de]
        mov ecx, dword ptr [eax + 0x14]
        test ecx, ecx
        mov dword ptr [g_hookObjectDrawPool], ecx
        je hook_a4e30_a4e58
        call CObjectPool::DrawLast
    hook_a4e30_a4e58:
        mov ecx, dword ptr [g_debugObjectPool]
        test ecx, ecx
        je hook_a4e30_a4e67
        call CObjectPool::DrawLast
    hook_a4e30_a4e67:
        ret
    }
}

// Raw installed GTA ABI; void() is an address declaration only.
// samp.dll RVA: 0x000a4e70
__declspec(naked) void GameHook_000a4e70() {
    __asm {
        push esi
        mov esi, dword ptr [esp + 8]
        mov dword ptr [g_hookObjectDrawBackend], esi
        pushad
        cmp dword ptr [g_pNetGame], 0
        je hook_a4e70_a4f0e
        cmp dword ptr [g_hookObjectDrawBackend], 0
        je hook_a4e70_a4f0e
        mov eax, dword ptr [g_hookObjectDrawBackend]
        cmp dword ptr [eax], 0x866f60
        jne hook_a4e70_a4f0e
        mov ecx, dword ptr [g_hookObjectDrawBackend]
        mov edx, dword ptr [ecx + 0x1c]
        and edx, 0x80000000
        je hook_a4e70_a4f0e
        mov eax, dword ptr [g_hookObjectDrawBackend]
        push eax
        mov ecx, dword ptr [g_pNetGame]
        call CNetGame::GetObjectPool
        mov ecx, eax
        call CObjectPool::Find
        mov dword ptr [g_hookObjectDrawWrapper], eax
        cmp dword ptr [g_hookObjectDrawWrapper], 0
        je hook_a4e70_a4f0e
        mov ecx, dword ptr [g_hookObjectDrawWrapper]
        cmp dword ptr [ecx + 0x118b], 0
        jne hook_a4e70_a4f0b
        mov edx, dword ptr [g_hookObjectDrawWrapper]
        fld dword ptr [edx + 0x54]
        fcomp dword ptr [g_objectStreamingNearDistance]
        fnstsw ax
        test ah, 0x41
        je hook_a4e70_a4f0b
        mov eax, dword ptr [g_hookObjectDrawWrapper]
        movzx ecx, byte ptr [eax + 0x1199]
        test ecx, ecx
        je hook_a4e70_a4f0e
    hook_a4e70_a4f0b:
        popad
        pop esi
        ret
    hook_a4e70_a4f0e:
        cmp dword ptr [g_debugObjectPool], 0
        je hook_a4e70_a4f85
        cmp dword ptr [g_hookObjectDrawBackend], 0
        je hook_a4e70_a4f85
        mov edx, dword ptr [g_hookObjectDrawBackend]
        cmp dword ptr [edx], 0x866f60
        jne hook_a4e70_a4f85
        mov eax, dword ptr [g_hookObjectDrawBackend]
        mov ecx, dword ptr [eax + 0x1c]
        and ecx, 0x80000000
        je hook_a4e70_a4f85
        mov edx, dword ptr [g_hookObjectDrawBackend]
        push edx
        mov ecx, dword ptr [g_debugObjectPool]
        call CObjectPool::Find
        mov dword ptr [g_hookObjectDrawWrapper], eax
        cmp dword ptr [g_hookObjectDrawWrapper], 0
        je hook_a4e70_a4f85
        mov eax, dword ptr [g_hookObjectDrawWrapper]
        cmp dword ptr [eax + 0x118b], 0
        jne hook_a4e70_a4f82
        mov ecx, dword ptr [g_hookObjectDrawWrapper]
        fld dword ptr [ecx + 0x54]
        fcomp dword ptr [g_objectStreamingNearDistance]
        fnstsw ax
        test ah, 0x41
        jne hook_a4e70_a4f85
    hook_a4e70_a4f82:
        popad
        pop esi
        ret
    hook_a4e70_a4f85:
        popad
        mov esi, dword ptr [g_hookObjectDrawBackend]
        movsx eax, word ptr [esi + 0x22]
        mov edx, 0x5534b9
        jmp edx
    }
}

#include "net/remoteplayer.h"
#include "net/localplayer.h"
// samp.dll RVA: 0x0018f580
DWORD g_hookMaterialReturn;
// samp.dll RVA: 0x0018f574
DWORD g_hookMaterialBackend;
// samp.dll RVA: 0x0018f57c
CPlayerPool * g_hookMaterialPlayerPool;
// samp.dll RVA: 0x0018f5a4
DWORD g_hookSurfingIndex;
// samp.dll RVA: 0x0018e7f4
CRemotePlayer * g_hookSurfingRemote;
// samp.dll RVA: 0x0018eac0
GameEntityCollisionShape g_hookCollisionFirst;
// samp.dll RVA: 0x0018e56c
GameEntityCollisionShape g_hookCollisionSecond;
// samp.dll RVA: 0x0018f68c
BYTE g_hookCollisionResult;
// samp.dll RVA: 0x0011e508
const float g_hookCollisionBoundHigh=3000.0f;
// samp.dll RVA: 0x0011ec3c
const float g_hookCollisionBoundLow=-3000.0f;

// Raw installed GTA ABI; void() is an installation-address type only.
// samp.dll RVA: 0x000a42d0
__declspec(naked) void GameHook_000a42d0() {
    __asm {
        mov eax, dword ptr [esp]
        mov dword ptr [g_hookMaterialReturn], eax
        mov dword ptr [g_hookMaterialBackend], ecx
        pushad
        mov eax, dword ptr [g_hookMaterialBackend]
        test eax, eax
        je hook_a42d0_a42f3
        mov cx, word ptr [eax + 0x22]
        mov word ptr [g_clientCrashLastRenderedObject], cx
    hook_a42d0_a42f3:
        mov ecx, dword ptr [g_pNetGame]
        test ecx, ecx
        mov dword ptr [g_hookObjectDrawWrapper], 0
        mov esi, 0x80000000
        je hook_a42d0_a4366
        test eax, eax
        je hook_a42d0_a4366
        test dword ptr [eax + 0x1c], esi
        je hook_a42d0_a4366
        mov edx, dword ptr [ecx + 0x3de]
        mov ecx, dword ptr [edx + 0x14]
        push eax
        call CObjectPool::Find
        test eax, eax
        mov dword ptr [g_hookObjectDrawWrapper], eax
        jne hook_a42d0_a435a
        mov eax, dword ptr [g_pNetGame]
        mov ecx, dword ptr [eax + 0x3de]
        mov ecx, dword ptr [ecx + 8]
        test ecx, ecx
        mov dword ptr [g_hookMaterialPlayerPool], ecx
        je hook_a42d0_a4361
        mov edx, dword ptr [g_hookMaterialBackend]
        push edx
        call CPlayerPool::FindAccessory
        test eax, eax
        mov dword ptr [g_hookObjectDrawWrapper], eax
        je hook_a42d0_a4361
    hook_a42d0_a435a:
        mov ecx, eax
        call CObject::BeforeMaterialRender
    hook_a42d0_a4361:
        mov eax, dword ptr [g_hookMaterialBackend]
    hook_a42d0_a4366:
        mov ecx, dword ptr [g_debugObjectPool]
        test ecx, ecx
        je hook_a42d0_a438f
        test eax, eax
        je hook_a42d0_a438f
        test dword ptr [eax + 0x1c], esi
        je hook_a42d0_a438f
        push eax
        call CObjectPool::Find
        test eax, eax
        mov dword ptr [g_hookObjectDrawWrapper], eax
        je hook_a42d0_a438f
        mov ecx, eax
        call CObject::BeforeMaterialRender
    hook_a42d0_a438f:
        popad
        mov eax, 0x59f180
        call eax
        pushad
        mov eax, dword ptr [g_pNetGame]
        test eax, eax
        mov ecx, dword ptr [g_hookObjectDrawWrapper]
        je hook_a42d0_a43b6
        test ecx, ecx
        je hook_a42d0_a43b6
        call CObject::AfterMaterialRender
        mov ecx, dword ptr [g_hookObjectDrawWrapper]
    hook_a42d0_a43b6:
        mov eax, dword ptr [g_debugObjectPool]
        test eax, eax
        je hook_a42d0_a43c8
        test ecx, ecx
        je hook_a42d0_a43c8
        call CObject::AfterMaterialRender
    hook_a42d0_a43c8:
        popad
        ret
    }
}

// Raw installed GTA ABI; void() is an installation-address type only.
// samp.dll RVA: 0x000a46b0
__declspec(naked) void GameHook_000a46b0() {
    __asm {
        mov edx, 0x5647f0
        call edx
        pushad
        mov esi, dword ptr [g_pNetGame]
        test esi, esi
        je hook_a46b0_a4768
        mov eax, dword ptr [esi + 0x3de]
        mov eax, dword ptr [eax + 8]
        test eax, eax
        je hook_a46b0_a46e1
        mov ecx, dword ptr [eax + 0x1e]
        call CLocalPlayer::ApplySurfingPhysics
        mov esi, dword ptr [g_pNetGame]
    hook_a46b0_a46e1:
        test esi, esi
        je hook_a46b0_a4768
        mov ecx, dword ptr [esi + 0x3de]
        mov eax, dword ptr [ecx + 8]
        test eax, eax
        je hook_a46b0_a4768
        xor edx, edx
        mov dword ptr [g_hookSurfingIndex], edx
        mov edi, edi
    hook_a46b0_a4700:
        cmp dx, 0x3ec
        jae hook_a46b0_a4759
        mov eax, dword ptr [esi + 0x3de]
        mov ecx, dword ptr [eax + 8]
        movzx eax, dx
        shl eax, 2
        mov edi, dword ptr [eax + ecx + 0xfd6]
        test edi, edi
        je hook_a46b0_a4759
        mov eax, dword ptr [eax + ecx + 0x26]
        test eax, eax
        je hook_a46b0_a472e
        mov ecx, dword ptr [eax + 8]
        jmp hook_a46b0_a4730
    hook_a46b0_a472e:
        xor ecx, ecx
    hook_a46b0_a4730:
        test ecx, ecx
        mov dword ptr [g_hookSurfingRemote], ecx
        je hook_a46b0_a4759
        mov eax, dword ptr [ecx + 4]
        test eax, eax
        je hook_a46b0_a4759
        mov al, byte ptr [ecx + 0x1a]
        test al, al
        je hook_a46b0_a4759
        call CRemotePlayer::ApplySurfingMatrix
        mov esi, dword ptr [g_pNetGame]
        mov edx, dword ptr [g_hookSurfingIndex]
    hook_a46b0_a4759:
        inc edx
        cmp edx, 0x3ec
        mov dword ptr [g_hookSurfingIndex], edx
        jne hook_a46b0_a4700
    hook_a46b0_a4768:
        popad
        ret
    }
}

// Raw installed GTA ABI; void() is an installation-address type only.
// samp.dll RVA: 0x000a56b0
__declspec(naked) void GameHook_000a56b0() {
    __asm {
        push ebp
        mov ebp, esp
        mov eax, dword ptr [ebp + 8]
        mov ecx, dword ptr [eax]
        mov dword ptr [g_hookCollisionFirst], ecx
        mov edx, dword ptr [eax + 4]
        mov dword ptr [g_hookCollisionFirst+4], edx
        mov ecx, dword ptr [eax + 8]
        mov dword ptr [g_hookCollisionFirst+8], ecx
        mov edx, dword ptr [eax + 0xc]
        mov dword ptr [g_hookCollisionFirst+12], edx
        mov eax, dword ptr [eax + 0x10]
        push esi
        mov esi, dword ptr [ebp + 0xc]
        mov dword ptr [g_hookCollisionFirst+16], eax
        mov ecx, esi
        mov edx, dword ptr [ecx]
        mov dword ptr [g_hookCollisionSecond], edx
        mov eax, dword ptr [ecx + 4]
        mov dword ptr [g_hookCollisionSecond+4], eax
        mov edx, dword ptr [ecx + 8]
        mov dword ptr [g_hookCollisionSecond+8], edx
        mov eax, dword ptr [ecx + 0xc]
        mov dword ptr [g_hookCollisionSecond+12], eax
        mov ecx, dword ptr [ecx + 0x10]
        mov dword ptr [g_hookCollisionSecond+16], ecx
        mov byte ptr [g_hookCollisionResult], 0
        push dword ptr [ebp + 0xc]
        push dword ptr [ebp + 8]
        mov edx, 0x41ac40
        call edx
        mov byte ptr [g_hookCollisionResult], al
        pop edx
        pop edx
        fld dword ptr [esi]
        fcomp dword ptr [g_hookCollisionBoundHigh]
        fnstsw ax
        test ah, 0x41
        je hook_a56b0_a576e
        fld dword ptr [esi]
        fcomp dword ptr [g_hookCollisionBoundLow]
        fnstsw ax
        test ah, 5
        jnp hook_a56b0_a576e
        fld dword ptr [esi + 4]
        fcomp dword ptr [g_hookCollisionBoundHigh]
        fnstsw ax
        test ah, 0x41
        je hook_a56b0_a576e
        fld dword ptr [esi + 4]
        fcomp dword ptr [g_hookCollisionBoundLow]
        fnstsw ax
        test ah, 5
        jp hook_a56b0_a5818
    hook_a56b0_a576e:
        mov ecx, dword ptr [g_pNetGame]
        test ecx, ecx
        mov al, byte ptr [g_hookCollisionResult]
        je hook_a56b0_a57ab
        test al, al
        jne hook_a56b0_a57ab
        mov edx, dword ptr [ecx + 0x3de]
        mov ecx, dword ptr [edx + 0x14]
        push g_hookCollisionSecond
        push g_hookCollisionFirst
        call CObjectPool::TestCollisionShapes
        test al, al
        je hook_a56b0_a57a6
        mov al, 1
        mov byte ptr [g_hookCollisionResult], al
        jmp hook_a56b0_a57ab
    hook_a56b0_a57a6:
        mov al, byte ptr [g_hookCollisionResult]
    hook_a56b0_a57ab:
        mov ecx, dword ptr [g_debugObjectPool]
        test ecx, ecx
        je hook_a56b0_a57da
        test al, al
        jne hook_a56b0_a57da
        push g_hookCollisionSecond
        push g_hookCollisionFirst
        call CObjectPool::TestCollisionShapes
        test al, al
        je hook_a56b0_a57d5
        mov al, 1
        mov byte ptr [g_hookCollisionResult], al
        jmp hook_a56b0_a57da
    hook_a56b0_a57d5:
        mov al, byte ptr [g_hookCollisionResult]
    hook_a56b0_a57da:
        mov ecx, dword ptr [g_pNetGame]
        test ecx, ecx
        je hook_a56b0_a581d
        mov dl, byte ptr [ecx + 0x232]
        test dl, dl
        jne hook_a56b0_a581d
        test al, al
        jne hook_a56b0_a581d
        mov eax, dword ptr [ecx + 0x3de]
        mov ecx, dword ptr [eax + 0xc]
        push g_hookCollisionSecond
        push g_hookCollisionFirst
        call CVehiclePool::TestCollisionShapes
        test al, al
        je hook_a56b0_a5818
        mov al, 1
        mov byte ptr [g_hookCollisionResult], al
        pop esi
        pop ebp
        ret
    hook_a56b0_a5818:
        mov al, byte ptr [g_hookCollisionResult]
    hook_a56b0_a581d:
        pop esi
        pop ebp
        ret
    }
}
