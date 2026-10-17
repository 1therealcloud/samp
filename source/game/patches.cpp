#include "game/patches.h"
#include "game/limits.h"
#include "game/input.h"
#include <string.h>
// samp.dll RVA: 0x001546c0
BYTE g_gamePatchStreamBytes[7]={0xb8,0x89,0x8f,0x6f,0x0,0xff,0xe0};
// samp.dll RVA: 0x001546c8
DWORD g_gamePatchValue1546c8=0x469c4000;
// samp.dll RVA: 0x001546cc
DWORD g_gamePatchValue1546cc=0xc69c4000;
// samp.dll RVA: 0x001546f0
DWORD g_gamePatchValue1546f0=0x40000000;
// samp.dll RVA: 0x001546f4
DWORD g_gamePatchValue1546f4=0x461c4000;
// samp.dll RVA: 0x0018f498
DWORD g_gameVersionAddressA;
// samp.dll RVA: 0x0018f49c
DWORD g_gameVersionAddressB;
// samp.dll RVA: 0x000aaad0
void GameApplyInGamePatches()
{
  DWORD *patch;
  int remaining;
  
  if (g_gameVersion == 1) {
    g_gameVersionAddressA = 0x7ee2a0;
    g_gameVersionAddressB = 0x7ee2a9;
  }
  else {
    g_gameVersionAddressA = 0x7ee2e0;
    g_gameVersionAddressB = 0x7ee2e9;
  }
  GameRelocateScanList();
  GameRelocatePedModels();
  GameRelocateModelFlags();
  GameRelocateModelRecords();
  GameIncreasePoolLimits();
  GameUnprotect((void*)0x004df7e2,2);
  *(DWORD*)0x004df7e2 = 0x90909090;
  *(BYTE*)0x004df7e6 = 0x90;
  GameUnprotect((void*)0x005b8fe4,1);
  *(BYTE*)0x005b8fe4 = 0x7f;
  GameUnprotect((void*)0x005720a5,5);
  *(DWORD*)0x005720a5 = 0x90909090;
  *(BYTE*)0x005720a9 = 0x90;
  GameUnprotect((void*)0x0053c159,5);
  *(DWORD*)0x0053c159 = 0x90909090;
  *(BYTE*)0x0053c15d = 0x90;
  GameUnprotect((void*)0x0053c136,5);
  *(DWORD*)0x0053c136 = 0x90909090;
  *(BYTE*)0x0053c13a = 0x90;
  if (g_gameVersion == 1) {
    GameUnprotect((void*)0x00748063,5);
    patch = (DWORD *)(void*)0x00748063;
  }
  else {
    GameUnprotect((void*)0x007480b3,5);
    patch = (DWORD *)(void*)0x007480b3;
  }
  *patch = 0x90909090;
  *(BYTE *)(patch + 1) = 0x90;
  GameUnprotect((void*)0x0058fbe9,5);
  *(DWORD*)0x0058fbe9 = 0x90909090;
  *(BYTE*)0x0058fbed = 0x90;
  GameUnprotect((void*)0x0086d1ec,1);
  *(BYTE*)0x0086d1ec = 0;
  GameUnprotect((void*)0x0053c090,5);
  *(DWORD*)0x0053c090 = 0x90909090;
  *(BYTE*)0x0053c094 = 0x90;
  GameUnprotect((void*)0x00440833,8);
  *(DWORD*)0x00440833 = 0x90909090;
  *(DWORD*)0x00440837 = 0x90909090;
  GameUnprotect((void*)0x0053ea08,10);
  *(DWORD*)0x0053ea08 = 0x90909090;
  *(DWORD*)0x0053ea0c = 0x90909090;
  *(WORD*)0x0053ea10 = 0x9090;
  GameUnprotect((void*)0x0071162c,1);
  *(BYTE*)0x0071162c = 0x50;
  GameUnprotect((void*)0x00609c08,0x27);
  patch = (DWORD *)(void*)0x00609c08;
  for (remaining = 9; remaining != 0; remaining = remaining + -1) {
    *patch = 0x90909090;
    patch = patch + 1;
  }
  *(WORD *)patch = 0x9090;
  *(BYTE *)((int)patch + 2) = 0x90;
  GameUnprotect((void*)0x0056e0fa,0x12);
  *(DWORD*)0x0056e0fa = 0x90909090;
  *(DWORD*)0x0056e0fe = 0x90909090;
  *(DWORD*)0x0056e102 = 0x90909090;
  *(DWORD*)0x0056e106 = 0x90909090;
  *(WORD*)0x0056e10a = 0x9090;
  GameUnprotect((void*)0x006bc9eb,2);
  *(WORD*)0x006bc9eb = 0x9090;
  GameUnprotect((void*)0x004bc6c1,3);
  *(BYTE*)0x004bc6c1 = 0xb0;
  *(BYTE*)0x004bc6c2 = 0;
  *(BYTE*)0x004bc6c3 = 0x90;
  GameUnprotect((void*)0x0044ac75,5);
  *(BYTE*)0x0044ac79 = 0x90;
  *(DWORD*)0x0044ac75 = 0x909001b0;
  GameUnprotect((void*)0x005b47b0,1);
  *(BYTE*)0x005b47b0 = 0xc3;
  GameUnprotect((void*)0x005e63a6,0x13);
  *(DWORD*)0x005e63a6 = 0x90909090;
  *(DWORD*)0x005e63aa = 0x90909090;
  *(DWORD*)0x005e63ae = 0x90909090;
  *(DWORD*)0x005e63b2 = 0x90909090;
  *(WORD*)0x005e63b6 = 0x9090;
  *(BYTE*)0x005e63b8 = 0x90;
  GameUnprotect((void*)0x00621aea,0xc);
  *(DWORD*)0x00621aea = 0x90909090;
  *(DWORD*)0x00621aee = 0x90909090;
  *(DWORD*)0x00621af2 = 0x90909090;
  GameUnprotect((void*)0x0062d331,0xb);
  *(DWORD*)0x0062d331 = 0x90909090;
  *(DWORD*)0x0062d335 = 0x90909090;
  *(WORD*)0x0062d339 = 0x9090;
  *(BYTE*)0x0062d33b = 0x90;
  GameUnprotect((void*)0x00741fff,0x1b);
  *(DWORD*)0x00741fff = 0x90909090;
  *(DWORD*)0x00742003 = 0x90909090;
  *(DWORD*)0x00742007 = 0x90909090;
  *(DWORD*)0x0074200b = 0x90909090;
  *(DWORD*)0x0074200f = 0x90909090;
  *(DWORD*)0x00742013 = 0x90909090;
  *(WORD*)0x00742017 = 0x9090;
  *(BYTE*)0x00742019 = 0x90;
  GameUnprotect((void*)0x00704e8a,5);
  *(DWORD*)0x00704e8a = 0x90909090;
  *(BYTE*)0x00704e8e = 0x90;
  GameUnprotect((void*)0x004090a0,1);
  *(BYTE*)0x004090a0 = 0xc3;
  GameUnprotect((void*)0x00441482,5);
  *(DWORD*)0x00441482 = 0x90909090;
  *(BYTE*)0x00441486 = 0x90;
  GameUnprotect((void*)0x00588be0,1);
  *(BYTE*)0x00588be0 = 0xc3;
  GameUnprotect((void*)0x0053c06a,5);
  *(DWORD*)0x0053c06a = 0x90909090;
  *(BYTE*)0x0053c06e = 0x90;
  GameUnprotect((void*)0x00434272,5);
  *(DWORD*)0x00434272 = 0x90909090;
  *(BYTE*)0x00434276 = 0x90;
  GameUnprotect((void*)0x60d64d,2);
  *(BYTE*)0x0060d64e = 0x84;
  GameUnprotect((void*)0x00542485,0xb);
  *(DWORD*)0x00542485 = 0x90909090;
  *(DWORD*)0x00542489 = 0x90909090;
  *(WORD*)0x0054248d = 0x9090;
  *(BYTE*)0x0054248f = 0x90;
  GameUnprotect((void*)0x00612710,3);
  *(BYTE*)0x00612710 = 0x33;
  *(BYTE*)0x00612711 = 0xc0;
  *(BYTE*)0x00612712 = 0xc3;
  GameUnprotect((void*)0x00613ba7,5);
  *(DWORD*)0x00613ba7 = 0x90909090;
  *(BYTE*)0x00613bab = 0x90;
  GameUnprotect((void*)0x00609a4e,6);
  *(DWORD*)0x00609a4e = 0x90909090;
  *(WORD*)0x00609a52 = 0x9090;
  GameUnprotect((void*)0x006f8cf8,0xc);
  *(DWORD*)0x006f8cf8 = 0x90909090;
  *(BYTE*)0x006f8cfc = 0x90;
  memcpy((void*)0x6f8cfd,g_gamePatchStreamBytes,7);
  GameUnprotect((void*)0x0053c1c1,5);
  *(DWORD*)0x0053c1c1 = 0x90909090;
  *(BYTE*)0x0053c1c5 = 0x90;
  GameUnprotect((void*)0x00540040,10);
  *(DWORD*)0x00540040 = 0x90909090;
  *(WORD*)0x00540044 = 0x9090;
  *(BYTE*)0x00540046 = 0x85;
  *(BYTE*)0x00540047 = 0xc9;
  *(BYTE*)0x00540048 = 0x74;
  GameUnprotect((void*)0x0056e5ad,5);
  *(DWORD*)0x0056e5ad = 0x90909090;
  *(BYTE*)0x0056e5b1 = 0x90;
  GameUnprotect((void*)0x00541df5,5);
  GameUnprotect((void*)0x00609cb4,5);
  *(DWORD*)0x00609cb4 = 0x90909090;
  *(BYTE*)0x00609cb8 = 0x90;
  GameUnprotect((void*)0x0060f2c4,0x19);
  *(DWORD*)0x0060f2c4 = 0x90909090;
  *(DWORD*)0x0060f2c8 = 0x90909090;
  *(DWORD*)0x0060f2cc = 0x90909090;
  *(DWORD*)0x0060f2d0 = 0x90909090;
  *(DWORD*)0x0060f2d4 = 0x90909090;
  *(DWORD*)0x0060f2d8 = 0x90909090;
  *(BYTE*)0x0060f2dc = 0x90;
  GameUnprotect((void*)0x006b18f1,5);
  *(DWORD*)0x006b18f1 = 0x90909090;
  *(BYTE*)0x006b18f5 = 0x90;
  GameUnprotect((void*)0x006b9298,5);
  *(DWORD*)0x006b9298 = 0x90909090;
  *(BYTE*)0x006b929c = 0x90;
  GameUnprotect((void*)0x006f1793,5);
  *(DWORD*)0x006f1793 = 0x90909090;
  *(BYTE*)0x006f1797 = 0x90;
  GameUnprotect((void*)0x006f86b6,5);
  *(DWORD*)0x006f86b6 = 0x90909090;
  *(BYTE*)0x006f86ba = 0x90;
  GameUnprotect((void*)0x0047c477,1);
  *(BYTE*)0x0047c477 = 0xeb;
  GameUnprotect((void*)0x0053a984,2);
  *(BYTE*)0x0053a984 = 0xeb;
  *(BYTE*)0x0053a985 = 0x77;
  GameUnprotect((void*)0x0060f289,8);
  *(DWORD*)0x0060f289 = 0x90909090;
  *(DWORD*)0x0060f28d = 0x90909090;
  GameUnprotect((void*)0x0060f29d,0x13);
  *(DWORD*)0x0060f29d = 0x90909090;
  *(DWORD*)0x0060f2a1 = 0x90909090;
  *(DWORD*)0x0060f2a5 = 0x90909090;
  *(DWORD*)0x0060f2a9 = 0x90909090;
  *(WORD*)0x0060f2ad = 0x9090;
  *(BYTE*)0x0060f2af = 0x90;
  GameUnprotect((void*)0x0058db5f,9);
  *(BYTE*)0x0058db5f = 0xbd;
  *(BYTE*)0x0058db60 = 0;
  *(BYTE*)0x0058db61 = 0;
  *(BYTE*)0x0058db62 = 0;
  *(BYTE*)0x0058db63 = 0;
  *(BYTE*)0x0058db64 = 0x90;
  *(BYTE*)0x0058db65 = 0x90;
  *(BYTE*)0x0058db66 = 0x90;
  *(BYTE*)0x0058db67 = 0x90;
  GameUnprotect((void*)0x00575b0e,5);
  *(DWORD*)0x00575b0e = 0x90909090;
  *(BYTE*)0x00575b12 = 0x90;
  GameUnprotect((void*)0x0063adc8,6);
  *(DWORD*)0x0063adc8 = 0x90909090;
  *(WORD*)0x0063adcc = 0x9090;
  GameUnprotect((void*)0x0058fc2e,0x1e);
  *(DWORD*)0x0058fc2e = 0x90909090;
  *(DWORD*)0x0058fc32 = 0x90909090;
  *(DWORD*)0x0058fc36 = 0x90909090;
  *(DWORD*)0x0058fc3a = 0x90909090;
  *(DWORD*)0x0058fc3e = 0x90909090;
  *(DWORD*)0x0058fc42 = 0x90909090;
  *(DWORD*)0x0058fc46 = 0x90909090;
  *(WORD*)0x0058fc4a = 0x9090;
  GameUnprotect((void*)0x005e1e70,2);
  *(BYTE*)0x005e1e70 = 0x33;
  *(BYTE*)0x005e1e71 = 0xc0;
  GameUnprotect((void*)0x006a8500,0x77);
  patch = (DWORD *)(void*)0x006a8500;
  for (remaining = 0x1d; remaining != 0; remaining = remaining + -1) {
    *patch = 0xc3c3c3c3;
    patch = patch + 1;
  }
  *(WORD *)patch = 0xc3c3;
  *(BYTE *)((int)patch + 2) = 0xc3;
  GameUnprotect((void*)0x00593d7c,5);
  *(DWORD*)0x00593d7c = 0x90909090;
  *(BYTE*)0x00593d80 = 0x90;
  if (g_gameVersion == 1) {
    GameUnprotect((void*)0x007469af,2);
    *(BYTE*)0x007469af = 0x33;
    *(BYTE*)0x007469b0 = 0xc0;
  }
  else {
    GameUnprotect((void*)0x007469ff,2);
    *(BYTE*)0x007469ff = 0x33;
    *(BYTE*)0x00746a00 = 0xc0;
  }
  GameUnprotect((void*)0x0061e0c2,4);
  *(DWORD*)0x0061e0c2 = (DWORD)&g_gamePatchValue1546f0;
  GameUnprotect((void*)0x00454cc9,4);
  *(DWORD*)0x00454cc9 = (DWORD)&g_gamePatchValue1546f4;
  GameUnprotect((void*)0x005952a6,5);
  *(DWORD*)0x005952a6 = 0x90909090;
  *(BYTE*)0x005952aa = 0x90;
  GameUnprotect((void*)0x006884c4,6);
  GameUnprotect((void*)0x00688200,6);
  *(DWORD*)0x006884c4 = 0x90909090;
  *(WORD*)0x006884c8 = 0x9090;
  *(DWORD*)0x00688200 = 0x90909090;
  *(WORD*)0x00688204 = 0x9090;
  GameUnprotect((void*)0x00570535,0xf);
  *(DWORD*)0x00570535 = 0x90909090;
  *(DWORD*)0x00570539 = 0x90909090;
  *(DWORD*)0x0057053d = 0x90909090;
  *(WORD*)0x00570541 = 0x9090;
  *(BYTE*)0x00570543 = 0x90;
  GameUnprotect((void*)0x005e7847,0x11);
  *(DWORD*)0x005e7847 = 0x90909090;
  *(DWORD*)0x005e784b = 0x90909090;
  *(DWORD*)0x005e784f = 0x90909090;
  *(DWORD*)0x005e7853 = 0x90909090;
  *(BYTE*)0x005e7857 = 0x90;
  GameUnprotect((void*)0x00570546,0xaf);
  patch = (DWORD *)(void*)0x00570546;
  for (remaining = 0x2b; remaining != 0; remaining = remaining + -1) {
    *patch = 0x90909090;
    patch = patch + 1;
  }
  *(WORD *)patch = 0x9090;
  *(BYTE *)((int)patch + 2) = 0x90;
  GameUnprotect((void*)0x006d1874,0x18);
  *(DWORD*)0x006d1874 = 0x6d17d5;
  *(DWORD*)0x006d1878 = 0x6d17d5;
  *(DWORD*)0x006d187c = 0x6d17d5;
  *(DWORD*)0x006d1880 = 0x6d17d5;
  *(DWORD*)0x006d1884 = 0x6d17d5;
  *(DWORD*)0x006d1888 = 0x6d17d5;
  GameUnprotect((void*)0x0064db49,9);
  *(DWORD*)0x0064db49 = 0x90909090;
  *(DWORD*)0x0064db4d = 0x90909090;
  *(BYTE*)0x0064db51 = 0x90;
  GameUnprotect((void*)0x0064bc9f,9);
  *(DWORD*)0x0064bc9f = 0x90909090;
  *(DWORD*)0x0064bca3 = 0x90909090;
  *(BYTE*)0x0064bca7 = 0x90;
  GameUnprotect((void*)0x0064b872,9);
  *(DWORD*)0x0064b872 = 0x90909090;
  *(DWORD*)0x0064b876 = 0x90909090;
  *(BYTE*)0x0064b87a = 0x90;
  GameUnprotect((void*)0x005e7d62,5);
  *(BYTE*)0x005e7d62 = 0x5e;
  *(BYTE*)0x005e7d63 = 0x5d;
  *(BYTE*)0x005e7d64 = 0xc2;
  *(BYTE*)0x005e7d65 = 0x1c;
  *(BYTE*)0x005e7d66 = 0;
  GameUnprotect((void*)0x006348f6,4);
  *(DWORD*)0x006348f6 = 0xffffffff;
  GameUnprotect((void*)0x00634de2,4);
  *(DWORD*)0x00634de2 = 0xffffffff;
  GameUnprotect((void*)0x71a220,0x400);
  *(BYTE*)0x0071a223 = 3;
  *(BYTE*)0x0071a5d5 = 3;
  *(BYTE*)0x0071a247 = 3;
  *(BYTE*)0x0071a282 = 3;
  *(BYTE*)0x0071a26a = 3;
  *(BYTE*)0x0071a2e5 = 3;
  *(BYTE*)0x0071a3ac = 3;
  *(BYTE*)0x0071a4b9 = 3;
  *(BYTE*)0x0071a53c = 3;
  *(BYTE*)0x0071a37f = 3;
  *(BYTE*)0x0071a518 = 3;
  *(BYTE*)0x0071a525 = 3;
  *(BYTE*)0x0071a54b = 3;
  *(BYTE*)0x0071a5b6 = 3;
  *(BYTE*)0x0071a3bb = 3;
  GameUnprotect((void*)0x007361b2,4);
  *(DWORD*)0x007361b2 = (DWORD)&g_gamePatchValue1546cc;
  GameUnprotect((void*)0x007361cb,4);
  *(DWORD*)0x007361cb = (DWORD)&g_gamePatchValue1546c8;
  GameUnprotect((void*)0x007361e0,4);
  *(DWORD*)0x007361e0 = (DWORD)&g_gamePatchValue1546cc;
  GameUnprotect((void*)0x007361f5,4);
  *(DWORD*)0x007361f5 = (DWORD)&g_gamePatchValue1546c8;
  GameUnprotect((void*)0x00524582,5);
  *(DWORD*)0x00524582 = 0x90909090;
  *(BYTE*)0x00524586 = 0x90;
  GameUnprotect((void*)0x00712025,1);
  *(BYTE*)0x00712025 = 1;
  GameUnprotect((void*)0x858b90,4);
  GameUnprotect((void*)0x0053bf28,5);
  *(DWORD*)0x0053bf28 = 0x90909090;
  *(BYTE*)0x0053bf2c = 0x90;
  GameUnprotect((void*)0x00859520,8);
  GameUnprotect((void*)0x00c17044,8);
  GameUnprotect((void*)0x004c3a5b,10);
  *(DWORD*)0x004c3a5b = 0x90909090;
  *(DWORD*)0x004c3a5f = 0x90909090;
  *(WORD*)0x004c3a63 = 0x9090;
  GameUnprotect((void*)0x0047bf54,4);
  GameInstallEventOpcodes();
  return;
}


// samp.dll RVA: 0x00002130
void GameInstallEventOpcodes() {
    *(DWORD*)0x47bf54=(DWORD)GameSetEventOpcode;
    *(DWORD*)0x47bf88=(DWORD)GamePlayerDriverOpcode;
}
