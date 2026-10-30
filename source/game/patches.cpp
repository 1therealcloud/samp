#include "game/patches.h"
#include "game/game.h"
#include "game/hooks.h"
#include <string.h>
// 0x000a6f00
void GameInstallVehicleDamageFilter()
{
    BOOL(WINAPI * protect)(LPVOID, SIZE_T, DWORD, PDWORD) = VirtualProtect;
    DWORD oldProtection, ignoredProtection;
    BYTE* site = (BYTE*)0x6d7c90;
    protect(site, 5, PAGE_EXECUTE_READWRITE, &oldProtection);
    site[0] = 0xe9;
    *(DWORD*)(site + 1) = (DWORD)GameHook_000a5090 - 0x6d7c95;
    protect(site, 5, oldProtection, &ignoredProtection);
}
// 0x000a6d50
void GameInstallShotHooks()
{
    BOOL(WINAPI * protect)(LPVOID, SIZE_T, DWORD, PDWORD) = VirtualProtect;
    DWORD oldProtection, ignoredProtection;
    // GTA SA virtual addresses; each original call has its own protection pair.
    const DWORD sites[] = {0x742495, 0x7424ec, 0x742548, 0x740721, 0x740b69, 0x736247, 0x7424cb, 0x73ac4b};
    const DWORD targets[] = {(DWORD)GameHook_000a5540,          (DWORD)GameHook_000a5540,          (DWORD)GameHook_000a5540, (DWORD)GameProcessShotLineOfSight,
                             (DWORD)GameProcessShotLineOfSight, (DWORD)GameProcessShotLineOfSight, (DWORD)GameHook_000a5600, (DWORD)GameHook_000a5650};
    for (int i = 0; i < 8; ++i)
    {
        BYTE* site = (BYTE*)sites[i];
        protect(site, 5, PAGE_EXECUTE_READWRITE, &oldProtection);
        site[0] = 0xe8;
        *(DWORD*)(site + 1) = targets[i] - sites[i] - 5;
        protect(site, 5, oldProtection, &ignoredProtection);
    }
}
// 0x002a9278
DWORD g_gameVehicleControlPatched = 0;
// 0x002a927c
DWORD g_gameManualVehicleControl = 0;
// 0x000aa800
void GameApplyManualVehicleControlPatches()
{
    if (g_gameVehicleControlPatched)
        return;
    BOOL(WINAPI * protect)(LPVOID, SIZE_T, DWORD, PDWORD) = VirtualProtect;
    DWORD oldProtection;
    // GTA SA virtual addresses. Native leaves these pages writable.
    protect((void*)0x6e1be3, 37, PAGE_EXECUTE_READWRITE, &oldProtection);
    memset((void*)0x6e1be3, 0x90, 37);
    protect((void*)0x6e1c38, 8, PAGE_EXECUTE_READWRITE, &oldProtection);
    memset((void*)0x6e1c38, 0x90, 8);
    protect((void*)0x6e1d98, 15, PAGE_EXECUTE_READWRITE, &oldProtection);
    memset((void*)0x6e1d98, 0x90, 15);
    protect((void*)0x6e1dbc, 8, PAGE_EXECUTE_READWRITE, &oldProtection);
    memset((void*)0x6e1dbc, 0x90, 8);
    protect((void*)0x6e1ba0, 6, PAGE_EXECUTE_READWRITE, &oldProtection);
    memset((void*)0x6e1ba0, 0x90, 6);
    protect((void*)0x6e1bb1, 6, PAGE_EXECUTE_READWRITE, &oldProtection);
    memset((void*)0x6e1bb1, 0x90, 6);
    protect((void*)0x6e1bd2, 7, PAGE_EXECUTE_READWRITE, &oldProtection);
    memset((void*)0x6e1bd2, 0x90, 7);
    const BYTE branch[] = {0x8a, 0x86, 0x28, 0x04, 0, 0, 0xa8, 0x40, 0x74, 0x28};
    memcpy((void*)0x6e1be5, branch, sizeof(branch));
    g_gameVehicleControlPatched = 1;
    g_gameManualVehicleControl = 1;
}
// 0x001546c0
BYTE g_gamePatchStreamBytes[7] = {0xb8, 0x89, 0x8f, 0x6f, 0x0, 0xff, 0xe0};
// 0x001546c8
DWORD g_gamePatchValue1546c8 = 0x469c4000;
// 0x001546cc
DWORD g_gamePatchValue1546cc = 0xc69c4000;
// 0x001546f0
DWORD g_gamePatchValue1546f0 = 0x40000000;
// 0x001546f4
DWORD g_gamePatchValue1546f4 = 0x461c4000;
// 0x0018f498
DWORD g_gameVersionAddressA;
// 0x0018f49c
DWORD g_gameVersionAddressB;
// 0x000aaad0
void GameApplyInGamePatches()
{
    DWORD* patch;
    int remaining;

    if (g_gameVersion == 1)
    {
        g_gameVersionAddressA = 0x7ee2a0;
        g_gameVersionAddressB = 0x7ee2a9;
    }
    else
    {
        g_gameVersionAddressA = 0x7ee2e0;
        g_gameVersionAddressB = 0x7ee2e9;
    }
    GameRelocateScanList();
    GameRelocatePedModels();
    GameRelocateModelFlags();
    GameRelocateModelRecords();
    GameIncreasePoolLimits();
    UnFuck((void*)0x004df7e2, 2);
    *(DWORD*)0x004df7e2 = 0x90909090;
    *(BYTE*)0x004df7e6 = 0x90;
    UnFuck((void*)0x005b8fe4, 1);
    *(BYTE*)0x005b8fe4 = 0x7f;
    UnFuck((void*)0x005720a5, 5);
    *(DWORD*)0x005720a5 = 0x90909090;
    *(BYTE*)0x005720a9 = 0x90;
    UnFuck((void*)0x0053c159, 5);
    *(DWORD*)0x0053c159 = 0x90909090;
    *(BYTE*)0x0053c15d = 0x90;
    UnFuck((void*)0x0053c136, 5);
    *(DWORD*)0x0053c136 = 0x90909090;
    *(BYTE*)0x0053c13a = 0x90;
    if (g_gameVersion == 1)
    {
        UnFuck((void*)0x00748063, 5);
        patch = (DWORD*)(void*)0x00748063;
    }
    else
    {
        UnFuck((void*)0x007480b3, 5);
        patch = (DWORD*)(void*)0x007480b3;
    }
    *patch = 0x90909090;
    *(BYTE*)(patch + 1) = 0x90;
    UnFuck((void*)0x0058fbe9, 5);
    *(DWORD*)0x0058fbe9 = 0x90909090;
    *(BYTE*)0x0058fbed = 0x90;
    UnFuck((void*)0x0086d1ec, 1);
    *(BYTE*)0x0086d1ec = 0;
    UnFuck((void*)0x0053c090, 5);
    *(DWORD*)0x0053c090 = 0x90909090;
    *(BYTE*)0x0053c094 = 0x90;
    UnFuck((void*)0x00440833, 8);
    *(DWORD*)0x00440833 = 0x90909090;
    *(DWORD*)0x00440837 = 0x90909090;
    UnFuck((void*)0x0053ea08, 10);
    *(DWORD*)0x0053ea08 = 0x90909090;
    *(DWORD*)0x0053ea0c = 0x90909090;
    *(WORD*)0x0053ea10 = 0x9090;
    UnFuck((void*)0x0071162c, 1);
    *(BYTE*)0x0071162c = 0x50;
    UnFuck((void*)0x00609c08, 0x27);
    patch = (DWORD*)(void*)0x00609c08;
    for (remaining = 9; remaining != 0; remaining = remaining + -1)
    {
        *patch = 0x90909090;
        patch = patch + 1;
    }
    *(WORD*)patch = 0x9090;
    *(BYTE*)((int)patch + 2) = 0x90;
    UnFuck((void*)0x0056e0fa, 0x12);
    *(DWORD*)0x0056e0fa = 0x90909090;
    *(DWORD*)0x0056e0fe = 0x90909090;
    *(DWORD*)0x0056e102 = 0x90909090;
    *(DWORD*)0x0056e106 = 0x90909090;
    *(WORD*)0x0056e10a = 0x9090;
    UnFuck((void*)0x006bc9eb, 2);
    *(WORD*)0x006bc9eb = 0x9090;
    UnFuck((void*)0x004bc6c1, 3);
    *(BYTE*)0x004bc6c1 = 0xb0;
    *(BYTE*)0x004bc6c2 = 0;
    *(BYTE*)0x004bc6c3 = 0x90;
    UnFuck((void*)0x0044ac75, 5);
    *(BYTE*)0x0044ac79 = 0x90;
    *(DWORD*)0x0044ac75 = 0x909001b0;
    UnFuck((void*)0x005b47b0, 1);
    *(BYTE*)0x005b47b0 = 0xc3;
    UnFuck((void*)0x005e63a6, 0x13);
    *(DWORD*)0x005e63a6 = 0x90909090;
    *(DWORD*)0x005e63aa = 0x90909090;
    *(DWORD*)0x005e63ae = 0x90909090;
    *(DWORD*)0x005e63b2 = 0x90909090;
    *(WORD*)0x005e63b6 = 0x9090;
    *(BYTE*)0x005e63b8 = 0x90;
    UnFuck((void*)0x00621aea, 0xc);
    *(DWORD*)0x00621aea = 0x90909090;
    *(DWORD*)0x00621aee = 0x90909090;
    *(DWORD*)0x00621af2 = 0x90909090;
    UnFuck((void*)0x0062d331, 0xb);
    *(DWORD*)0x0062d331 = 0x90909090;
    *(DWORD*)0x0062d335 = 0x90909090;
    *(WORD*)0x0062d339 = 0x9090;
    *(BYTE*)0x0062d33b = 0x90;
    UnFuck((void*)0x00741fff, 0x1b);
    *(DWORD*)0x00741fff = 0x90909090;
    *(DWORD*)0x00742003 = 0x90909090;
    *(DWORD*)0x00742007 = 0x90909090;
    *(DWORD*)0x0074200b = 0x90909090;
    *(DWORD*)0x0074200f = 0x90909090;
    *(DWORD*)0x00742013 = 0x90909090;
    *(WORD*)0x00742017 = 0x9090;
    *(BYTE*)0x00742019 = 0x90;
    UnFuck((void*)0x00704e8a, 5);
    *(DWORD*)0x00704e8a = 0x90909090;
    *(BYTE*)0x00704e8e = 0x90;
    UnFuck((void*)0x004090a0, 1);
    *(BYTE*)0x004090a0 = 0xc3;
    UnFuck((void*)0x00441482, 5);
    *(DWORD*)0x00441482 = 0x90909090;
    *(BYTE*)0x00441486 = 0x90;
    UnFuck((void*)0x00588be0, 1);
    *(BYTE*)0x00588be0 = 0xc3;
    UnFuck((void*)0x0053c06a, 5);
    *(DWORD*)0x0053c06a = 0x90909090;
    *(BYTE*)0x0053c06e = 0x90;
    UnFuck((void*)0x00434272, 5);
    *(DWORD*)0x00434272 = 0x90909090;
    *(BYTE*)0x00434276 = 0x90;
    UnFuck((void*)0x60d64d, 2);
    *(BYTE*)0x0060d64e = 0x84;
    UnFuck((void*)0x00542485, 0xb);
    *(DWORD*)0x00542485 = 0x90909090;
    *(DWORD*)0x00542489 = 0x90909090;
    *(WORD*)0x0054248d = 0x9090;
    *(BYTE*)0x0054248f = 0x90;
    UnFuck((void*)0x00612710, 3);
    *(BYTE*)0x00612710 = 0x33;
    *(BYTE*)0x00612711 = 0xc0;
    *(BYTE*)0x00612712 = 0xc3;
    UnFuck((void*)0x00613ba7, 5);
    *(DWORD*)0x00613ba7 = 0x90909090;
    *(BYTE*)0x00613bab = 0x90;
    UnFuck((void*)0x00609a4e, 6);
    *(DWORD*)0x00609a4e = 0x90909090;
    *(WORD*)0x00609a52 = 0x9090;
    UnFuck((void*)0x006f8cf8, 0xc);
    *(DWORD*)0x006f8cf8 = 0x90909090;
    *(BYTE*)0x006f8cfc = 0x90;
    memcpy((void*)0x6f8cfd, g_gamePatchStreamBytes, 7);
    UnFuck((void*)0x0053c1c1, 5);
    *(DWORD*)0x0053c1c1 = 0x90909090;
    *(BYTE*)0x0053c1c5 = 0x90;
    UnFuck((void*)0x00540040, 10);
    *(DWORD*)0x00540040 = 0x90909090;
    *(WORD*)0x00540044 = 0x9090;
    *(BYTE*)0x00540046 = 0x85;
    *(BYTE*)0x00540047 = 0xc9;
    *(BYTE*)0x00540048 = 0x74;
    UnFuck((void*)0x0056e5ad, 5);
    *(DWORD*)0x0056e5ad = 0x90909090;
    *(BYTE*)0x0056e5b1 = 0x90;
    UnFuck((void*)0x00541df5, 5);
    UnFuck((void*)0x00609cb4, 5);
    *(DWORD*)0x00609cb4 = 0x90909090;
    *(BYTE*)0x00609cb8 = 0x90;
    UnFuck((void*)0x0060f2c4, 0x19);
    *(DWORD*)0x0060f2c4 = 0x90909090;
    *(DWORD*)0x0060f2c8 = 0x90909090;
    *(DWORD*)0x0060f2cc = 0x90909090;
    *(DWORD*)0x0060f2d0 = 0x90909090;
    *(DWORD*)0x0060f2d4 = 0x90909090;
    *(DWORD*)0x0060f2d8 = 0x90909090;
    *(BYTE*)0x0060f2dc = 0x90;
    UnFuck((void*)0x006b18f1, 5);
    *(DWORD*)0x006b18f1 = 0x90909090;
    *(BYTE*)0x006b18f5 = 0x90;
    UnFuck((void*)0x006b9298, 5);
    *(DWORD*)0x006b9298 = 0x90909090;
    *(BYTE*)0x006b929c = 0x90;
    UnFuck((void*)0x006f1793, 5);
    *(DWORD*)0x006f1793 = 0x90909090;
    *(BYTE*)0x006f1797 = 0x90;
    UnFuck((void*)0x006f86b6, 5);
    *(DWORD*)0x006f86b6 = 0x90909090;
    *(BYTE*)0x006f86ba = 0x90;
    UnFuck((void*)0x0047c477, 1);
    *(BYTE*)0x0047c477 = 0xeb;
    UnFuck((void*)0x0053a984, 2);
    *(BYTE*)0x0053a984 = 0xeb;
    *(BYTE*)0x0053a985 = 0x77;
    UnFuck((void*)0x0060f289, 8);
    *(DWORD*)0x0060f289 = 0x90909090;
    *(DWORD*)0x0060f28d = 0x90909090;
    UnFuck((void*)0x0060f29d, 0x13);
    *(DWORD*)0x0060f29d = 0x90909090;
    *(DWORD*)0x0060f2a1 = 0x90909090;
    *(DWORD*)0x0060f2a5 = 0x90909090;
    *(DWORD*)0x0060f2a9 = 0x90909090;
    *(WORD*)0x0060f2ad = 0x9090;
    *(BYTE*)0x0060f2af = 0x90;
    UnFuck((void*)0x0058db5f, 9);
    *(BYTE*)0x0058db5f = 0xbd;
    *(BYTE*)0x0058db60 = 0;
    *(BYTE*)0x0058db61 = 0;
    *(BYTE*)0x0058db62 = 0;
    *(BYTE*)0x0058db63 = 0;
    *(BYTE*)0x0058db64 = 0x90;
    *(BYTE*)0x0058db65 = 0x90;
    *(BYTE*)0x0058db66 = 0x90;
    *(BYTE*)0x0058db67 = 0x90;
    UnFuck((void*)0x00575b0e, 5);
    *(DWORD*)0x00575b0e = 0x90909090;
    *(BYTE*)0x00575b12 = 0x90;
    UnFuck((void*)0x0063adc8, 6);
    *(DWORD*)0x0063adc8 = 0x90909090;
    *(WORD*)0x0063adcc = 0x9090;
    UnFuck((void*)0x0058fc2e, 0x1e);
    *(DWORD*)0x0058fc2e = 0x90909090;
    *(DWORD*)0x0058fc32 = 0x90909090;
    *(DWORD*)0x0058fc36 = 0x90909090;
    *(DWORD*)0x0058fc3a = 0x90909090;
    *(DWORD*)0x0058fc3e = 0x90909090;
    *(DWORD*)0x0058fc42 = 0x90909090;
    *(DWORD*)0x0058fc46 = 0x90909090;
    *(WORD*)0x0058fc4a = 0x9090;
    UnFuck((void*)0x005e1e70, 2);
    *(BYTE*)0x005e1e70 = 0x33;
    *(BYTE*)0x005e1e71 = 0xc0;
    UnFuck((void*)0x006a8500, 0x77);
    patch = (DWORD*)(void*)0x006a8500;
    for (remaining = 0x1d; remaining != 0; remaining = remaining + -1)
    {
        *patch = 0xc3c3c3c3;
        patch = patch + 1;
    }
    *(WORD*)patch = 0xc3c3;
    *(BYTE*)((int)patch + 2) = 0xc3;
    UnFuck((void*)0x00593d7c, 5);
    *(DWORD*)0x00593d7c = 0x90909090;
    *(BYTE*)0x00593d80 = 0x90;
    if (g_gameVersion == 1)
    {
        UnFuck((void*)0x007469af, 2);
        *(BYTE*)0x007469af = 0x33;
        *(BYTE*)0x007469b0 = 0xc0;
    }
    else
    {
        UnFuck((void*)0x007469ff, 2);
        *(BYTE*)0x007469ff = 0x33;
        *(BYTE*)0x00746a00 = 0xc0;
    }
    UnFuck((void*)0x0061e0c2, 4);
    *(DWORD*)0x0061e0c2 = (DWORD)&g_gamePatchValue1546f0;
    UnFuck((void*)0x00454cc9, 4);
    *(DWORD*)0x00454cc9 = (DWORD)&g_gamePatchValue1546f4;
    UnFuck((void*)0x005952a6, 5);
    *(DWORD*)0x005952a6 = 0x90909090;
    *(BYTE*)0x005952aa = 0x90;
    UnFuck((void*)0x006884c4, 6);
    UnFuck((void*)0x00688200, 6);
    *(DWORD*)0x006884c4 = 0x90909090;
    *(WORD*)0x006884c8 = 0x9090;
    *(DWORD*)0x00688200 = 0x90909090;
    *(WORD*)0x00688204 = 0x9090;
    UnFuck((void*)0x00570535, 0xf);
    *(DWORD*)0x00570535 = 0x90909090;
    *(DWORD*)0x00570539 = 0x90909090;
    *(DWORD*)0x0057053d = 0x90909090;
    *(WORD*)0x00570541 = 0x9090;
    *(BYTE*)0x00570543 = 0x90;
    UnFuck((void*)0x005e7847, 0x11);
    *(DWORD*)0x005e7847 = 0x90909090;
    *(DWORD*)0x005e784b = 0x90909090;
    *(DWORD*)0x005e784f = 0x90909090;
    *(DWORD*)0x005e7853 = 0x90909090;
    *(BYTE*)0x005e7857 = 0x90;
    UnFuck((void*)0x00570546, 0xaf);
    patch = (DWORD*)(void*)0x00570546;
    for (remaining = 0x2b; remaining != 0; remaining = remaining + -1)
    {
        *patch = 0x90909090;
        patch = patch + 1;
    }
    *(WORD*)patch = 0x9090;
    *(BYTE*)((int)patch + 2) = 0x90;
    UnFuck((void*)0x006d1874, 0x18);
    *(DWORD*)0x006d1874 = 0x6d17d5;
    *(DWORD*)0x006d1878 = 0x6d17d5;
    *(DWORD*)0x006d187c = 0x6d17d5;
    *(DWORD*)0x006d1880 = 0x6d17d5;
    *(DWORD*)0x006d1884 = 0x6d17d5;
    *(DWORD*)0x006d1888 = 0x6d17d5;
    UnFuck((void*)0x0064db49, 9);
    *(DWORD*)0x0064db49 = 0x90909090;
    *(DWORD*)0x0064db4d = 0x90909090;
    *(BYTE*)0x0064db51 = 0x90;
    UnFuck((void*)0x0064bc9f, 9);
    *(DWORD*)0x0064bc9f = 0x90909090;
    *(DWORD*)0x0064bca3 = 0x90909090;
    *(BYTE*)0x0064bca7 = 0x90;
    UnFuck((void*)0x0064b872, 9);
    *(DWORD*)0x0064b872 = 0x90909090;
    *(DWORD*)0x0064b876 = 0x90909090;
    *(BYTE*)0x0064b87a = 0x90;
    UnFuck((void*)0x005e7d62, 5);
    *(BYTE*)0x005e7d62 = 0x5e;
    *(BYTE*)0x005e7d63 = 0x5d;
    *(BYTE*)0x005e7d64 = 0xc2;
    *(BYTE*)0x005e7d65 = 0x1c;
    *(BYTE*)0x005e7d66 = 0;
    UnFuck((void*)0x006348f6, 4);
    *(DWORD*)0x006348f6 = 0xffffffff;
    UnFuck((void*)0x00634de2, 4);
    *(DWORD*)0x00634de2 = 0xffffffff;
    UnFuck((void*)0x71a220, 0x400);
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
    UnFuck((void*)0x007361b2, 4);
    *(DWORD*)0x007361b2 = (DWORD)&g_gamePatchValue1546cc;
    UnFuck((void*)0x007361cb, 4);
    *(DWORD*)0x007361cb = (DWORD)&g_gamePatchValue1546c8;
    UnFuck((void*)0x007361e0, 4);
    *(DWORD*)0x007361e0 = (DWORD)&g_gamePatchValue1546cc;
    UnFuck((void*)0x007361f5, 4);
    *(DWORD*)0x007361f5 = (DWORD)&g_gamePatchValue1546c8;
    UnFuck((void*)0x00524582, 5);
    *(DWORD*)0x00524582 = 0x90909090;
    *(BYTE*)0x00524586 = 0x90;
    UnFuck((void*)0x00712025, 1);
    *(BYTE*)0x00712025 = 1;
    UnFuck((void*)0x858b90, 4);
    UnFuck((void*)0x0053bf28, 5);
    *(DWORD*)0x0053bf28 = 0x90909090;
    *(BYTE*)0x0053bf2c = 0x90;
    UnFuck((void*)0x00859520, 8);
    UnFuck((void*)0x00c17044, 8);
    UnFuck((void*)0x004c3a5b, 10);
    *(DWORD*)0x004c3a5b = 0x90909090;
    *(DWORD*)0x004c3a5f = 0x90909090;
    *(WORD*)0x004c3a63 = 0x9090;
    UnFuck((void*)0x0047bf54, 4);
    GameInstallEventOpcodes();
    return;
}

// 0x00002130
void GameInstallEventOpcodes()
{
    *(DWORD*)0x47bf54 = (DWORD)GameSetEventOpcode;
    *(DWORD*)0x47bf88 = (DWORD)GamePlayerDriverOpcode;
}

#include "game/limit_values.inc"
// 0x001e04b0
DWORD g_gameScanList[40000];
// 0x002075b0
GamePedModelRecord g_gamePedModels[319];
// 0x0020ca70
GameModelRecord32 g_gameModelRecords[20000];
// 0x002a8e70
BYTE g_gameModelFlags[1024];
// 0x000aa6a0
void GameRelocateScanList()
{
    memset(g_gameScanList, 0, sizeof(g_gameScanList));
    for (int i = 0; i < 14; i++)
    {
        if (g_gameVersion == 1)
        {
            UnFuck((void*)g_gameScanDirectUSA[i], 4);
            *(DWORD*)g_gameScanDirectUSA[i] = (DWORD)g_gameScanList;
        }
        else if (g_gameVersion == 2)
        {
            UnFuck((void*)g_gameScanDirectEU[i], 4);
            *(DWORD*)g_gameScanDirectEU[i] = (DWORD)g_gameScanList;
        }
    }
    for (int j = 0; j < 56; j++)
    {
        if (g_gameVersion == 1)
        {
            UnFuck((void*)g_gameScanOperandUSA[j], 8);
            *(DWORD*)(g_gameScanOperandUSA[j] + 3) = (DWORD)g_gameScanList;
        }
        else if (g_gameVersion == 2)
        {
            UnFuck((void*)g_gameScanOperandEU[j], 8);
            *(DWORD*)(g_gameScanOperandEU[j] + 3) = (DWORD)g_gameScanList;
        }
    }
    for (int k = 0; k < 11; k++)
    {
        UnFuck((void*)g_gameScanSecondSites[k], 8);
        *(DWORD*)(g_gameScanSecondSites[k] + 3) = (DWORD)(g_gameScanList + 1);
    }
    // In the native DLL the ped store begins exactly one-past the 40000-DWORD scan list.
    // These four operands terminate scan traversal; they do not reference ped records.
    for (int p = 0; p < 4; p++)
    {
        UnFuck((void*)g_gamePedReferenceSites[p], 4);
        *(DWORD*)g_gamePedReferenceSites[p] = (DWORD)(g_gameScanList + 40000);
    }
    UnFuck((void*)0x564dc7, 4);
    *(DWORD*)0x564dc7 = (DWORD)((BYTE*)g_gameScanList + 0x1c200);
    UnFuck((void*)0x40936a, 4);
    *(DWORD*)0x40936a = (DWORD)(g_gameScanList + 1);
    memset((void*)0xb7d0b8, 0, 0x1c200);
}
// 0x000aa5e0
void GameRelocatePedModels()
{
    for (int i = 0; i < 319; i++)
    {
        g_gamePedModels[i].vtable = 0x85bdc0;
        memset(g_gamePedModels[i].data, 0, 64);
    }
    UnFuck((void*)0x4c67ad, 4);
    *(DWORD*)0x4c67ad = (DWORD)g_gamePedModels;
}
// 0x000aa630
void GameRelocateModelRecords()
{
    for (int i = 0; i < 20000; i++)
    {
        g_gameModelRecords[i].vtable = 0x85bbf0;
        memset(g_gameModelRecords[i].data, 0, 28);
    }
    for (int j = 0; j < 14; j++)
    {
        UnFuck((void*)g_gameModelReferenceSites[j], 4);
        *(DWORD*)g_gameModelReferenceSites[j] = (DWORD)g_gameModelRecords;
    }
}
// 0x000aa980
void GameRelocateModelFlags()
{
    memcpy(g_gameModelFlags, g_gameDefaultModelFlags, 1024);
    UnFuck((void*)0x44b1c1, 4);
    *(DWORD*)0x44b1c1 = (DWORD)g_gameModelFlags;
    UnFuck((void*)0x4c8390, 4);
    *(DWORD*)0x4c8390 = (DWORD)g_gameModelFlags;
    UnFuck((void*)0x4c8399, 4);
    *(DWORD*)0x4c8399 = (DWORD)(g_gameModelFlags + 1);
    UnFuck((void*)0x4c83a3, 4);
    *(DWORD*)0x4c83a3 = (DWORD)(g_gameModelFlags + 2);
    UnFuck((void*)0x5817cc, 4);
    *(DWORD*)0x5817cc = (DWORD)g_gameModelFlags;
    UnFuck((void*)0x582176, 4);
    *(DWORD*)0x582176 = (DWORD)g_gameModelFlags;
    UnFuck((void*)0x6a6ffa, 4);
    *(DWORD*)0x6a6ffa = (DWORD)g_gameModelFlags;
}
// 0x000aa180
void GameIncreasePoolLimits()
{
    GameWaitUnprotect((BYTE*)0x551024, 7, 0x68);
    memcpy((void*)0x551024, g_gamePoolPatchOne, 7);
    UnFuck((void*)0x55105f, 4);
    *(DWORD*)0x55105f = 20000;
    UnFuck((void*)0x5510cf, 4);
    *(DWORD*)0x5510cf = 16000;
    UnFuck((void*)0x550f46, 4);
    *(DWORD*)0x550f46 = 300000;
    UnFuck((void*)0x550f82, 4);
    *(DWORD*)0x550f82 = 210000;
    UnFuck((void*)0x550fba, 4);
    *(DWORD*)0x550fba = 50000;
    UnFuck((void*)0x551097, 4);
    *(DWORD*)0x551097 = 0xc1c;
    UnFuck((void*)0x550ff2, 1);
    *(BYTE*)0x550ff2 = 0xf0;
    UnFuck((void*)0x551283, 1);
    *(BYTE*)0x551283 = 0xf0;
    UnFuck((void*)0x551140, 1);
    *(BYTE*)0x551140 = 5;
    UnFuck((void*)0x551178, 1);
    *(BYTE*)0x551178 = 1;
    UnFuck((void*)0x54f3a1, 4);
    *(DWORD*)0x54f3a1 = 20000;
    UnFuck((void*)0x551106, 5);
    memcpy((void*)0x551106, g_gamePoolPatchTwo, 5);
}

// Legacy helper name retained from the 0.2x source; API failures are ignored.
// 0x000a9d10
void _patch(DWORD address, BYTE* data, int byteCount)
{
    DWORD oldProtection, ignoredProtection;
    VirtualProtect((void*)address, byteCount, PAGE_EXECUTE_READWRITE, &oldProtection);
    // clang-format off
    // Native forward DWORD/byte copy, including its behavior for overlap.
    __asm {
        mov esi,data
        mov edi,address
        mov edx,byteCount
        mov ecx,edx
        shr ecx,2
        rep movsd
        mov ecx,edx
        and ecx,3
        rep movsb
    }
    // clang-format on
    VirtualProtect((void*)address, byteCount, oldProtection, &ignoredProtection);
}

#include <stdlib.h>
// Legacy allocation patches; native retains these allocations indefinitely.
#define patch(a, b) _patch(a, b, sizeof(b))
#define memadd(num, arr)                                                                                                                                                           \
    dwAlloc += num;                                                                                                                                                                \
    cBytes = (char*)&dwAlloc;                                                                                                                                                      \
    for (int i = 0; i < 4; i++)                                                                                                                                                    \
    arr[i] = cBytes[i]
// 0x000a9d60
void SetIPLs(int iIPLs, int iGtaVersion)
{
    DWORD dwAlloc = (DWORD)malloc((DWORD)iIPLs + 4);
    if (dwAlloc)
    {
        memset((void*)dwAlloc, 0x0, (DWORD)iIPLs + 4);
        char* cBytes = (char*)&dwAlloc;

        BYTE bIPLs[] = {cBytes[0], cBytes[1], cBytes[2], cBytes[3]};

        if (iGtaVersion == 1)
        {
            patch(0x1569777, bIPLs);
            patch(0x15649FA, bIPLs);
            patch(0x1561160, bIPLs);
        }
        else
        {
            patch(0x1569717, bIPLs);
            patch(0x156495A, bIPLs);
            patch(0x156115C, bIPLs);
        }
        patch(0x40619B, bIPLs);
        patch(0x405C3D, bIPLs);
    }
}

// 0x000a9e70
void SetTimedObjects(int iTimedObjects)
{
    DWORD dwAlloc = (DWORD)malloc((0x24 * (DWORD)iTimedObjects) + 4);
    if (dwAlloc)
    {
        memset((LPVOID)dwAlloc, 0x00, (0x24 * (DWORD)iTimedObjects) + 4);
        for (DWORD i = dwAlloc + 4; i < (dwAlloc + 4 + (0x24 * (DWORD)iTimedObjects)); i += 0x24)
        {
            *(BYTE*)i = 0xB0;
            *(BYTE*)(i + 1) = 0xBC;
            *(BYTE*)(i + 2) = 0x85;
            *(BYTE*)(i + 10) = 0xFF;
            *(BYTE*)(i + 11) = 0xFF;
            *(BYTE*)(i + 34) = 0xFF;
            *(BYTE*)(i + 35) = 0xFF;
        }
        char* cBytes = (char*)&dwAlloc;
        BYTE bTimedObjects[] = {cBytes[0], cBytes[1], cBytes[2], cBytes[3]};
        patch(0x4C66B1, bTimedObjects);
        patch(0x4C66C2, bTimedObjects);
        patch(0x84BC51, bTimedObjects);
        patch(0x856261, bTimedObjects);
        patch(0x4C683B, bTimedObjects);
        memadd(4, bTimedObjects);
        patch(0x4C6464, bTimedObjects);
        patch(0x4C66BD, bTimedObjects);
        cBytes = (char*)&iTimedObjects;
        BYTE pushTimedObjects[] = {0x68, cBytes[0], cBytes[1], cBytes[2], cBytes[3]};
        patch(0x4C58A5, pushTimedObjects);
    }
}

#undef patch
#undef memadd

// 0x001546d0
DWORD g_gameCount44OperandSites[7] = {0x410dbb, 0x410de5, 0x410f68, 0x410aa4, 0x410cc4, 0x410ead, 0x411505};
// 0x000aa0d0
void GamePatchCount44(DWORD count)
{
    BOOL(WINAPI * protect)(LPVOID, SIZE_T, DWORD, PDWORD) = VirtualProtect;
    DWORD ignoredProtection;
    protect((void*)0x411458, 4, PAGE_EXECUTE_READWRITE, &ignoredProtection);
    *(DWORD*)0x411458 = count;
    DWORD bytes = count * 44;
    for (int index = 0; index < 7; ++index)
    {
        protect((void*)(g_gameCount44OperandSites[index] + 2), 4, PAGE_EXECUTE_READWRITE, &ignoredProtection);
        // Original reloads the mutable table entry after VirtualProtect.
        *(DWORD*)(g_gameCount44OperandSites[index] + 2) = bytes;
    }
}
// 0x000aa130
void GameSetFontPatchBytes3()
{
    // GTA SA virtual addresses; native does not call VirtualProtect here.
    *(BYTE*)0x71a223 = 3;
    *(BYTE*)0x71a5d5 = 3;
    *(BYTE*)0x71a247 = 3;
    *(BYTE*)0x71a282 = 3;
    *(BYTE*)0x71a26a = 3;
    *(BYTE*)0x71a2e5 = 3;
    *(BYTE*)0x71a3ac = 3;
    *(BYTE*)0x71a4b9 = 3;
    *(BYTE*)0x71a53c = 3;
    *(BYTE*)0x71a37f = 3;
    *(BYTE*)0x71a518 = 3;
    *(BYTE*)0x71a525 = 3;
    *(BYTE*)0x71a54b = 3;
    *(BYTE*)0x71a5b6 = 3;
    *(BYTE*)0x71a3bb = 3;
}
// 0x000aaa50
void GameApplyDebugInitializationPatches()
{
    BOOL(WINAPI * protect)(LPVOID, SIZE_T, DWORD, PDWORD) = VirtualProtect;
    DWORD ignoredProtection;
    protect((void*)0x704e8a, 5, PAGE_EXECUTE_READWRITE, &ignoredProtection);
    *(DWORD*)0x704e8a = 0x90909090;
    *(BYTE*)0x704e8e = 0x90;
    protect((void*)0x609a4e, 6, PAGE_EXECUTE_READWRITE, &ignoredProtection);
    *(DWORD*)0x609a4e = 0x90909090;
    *(WORD*)0x609a52 = 0x9090;
    GamePrepareHooks();
    GameRelocateScanList();
    GameRelocatePedModels();
    GameRelocateModelFlags();
    GameRelocateModelRecords();
    GameIncreasePoolLimits();
    protect((void*)0x47bf54, 4, PAGE_EXECUTE_READWRITE, &ignoredProtection);
    GameInstallEventOpcodes();
}
