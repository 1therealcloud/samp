#include "game/hooks.h"
#include "game/input.h"
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

