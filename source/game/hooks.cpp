#include "game/hooks.h"
#include "chatwindow.h"
#include "cmdprocs.h"
#include "exceptions.h"
#include "game/aimstuff.h"
#include "game/animation.h"
#include "game/building_removal.h"
#include "game/frame_limit.h"
#include "game/game.h"
#include "game/keystuff.h"
#include "game/patches.h"
#include "game/player_colors.h"
#include "game/playerped.h"
#include "game/scripting.h"
#include "game/util.h"
#include "gui/device_objects.h"
#include "gui/render_dependencies.h"
#include "gui/textdraw_texture.h"
#include "main.h"
#include "net/actorpool.h"
#include "net/gangzonepool.h"
#include "net/localplayer.h"
#include "net/menupool.h"
#include "net/netgame.h"
#include "net/pickuppool.h"
#include "net/playerpool.h"
#include "net/remoteplayer.h"
#include "net/vehiclepool.h"
#include <stdlib.h>
#include <string.h>

// 0x00151a60
BYTE g_gameCustomModelHookJump[6] = {0xff, 0x25, 0xd7, 0x87, 0x40, 0x00};
// 0x000a5bf0
__declspec(naked) void GameHook_000a5bf0()
{
    // clang-format off
    __asm {
        cmp ebp,0
        jl custom_model_negative
        push esi
        push edi
        lea edi,[ebp+ebp*4]
        mov eax,0x4087f0
        jmp eax
    custom_model_negative:
        pop ebp
        pop ebx
        ret
    }
    // clang-format on
}
// 0x000a6d30
void GameInstallModelInfoHook()
{
    GameInstallHook(0x4087ea, (DWORD)GameHook_000a5bf0, 0x4087d7, g_gameCustomModelHookJump, 6);
}
// 0x0018f624
CPlayerPool* g_hookDamagePlayerPool;
// 0x00151afc
WORD g_hookDamagePlayerId = 0xffff;
// 0x0018f628
DWORD g_hookVehicleDamageAttacker;
// 0x0018f62c
DWORD g_hookVehicleDamageReceiver;
// 0x0018f630
DWORD g_hookVehicleDamageWord;
// 0x0018f634
DWORD g_hookVehicleDamageBlocked;
// 0x0018f638
DWORD g_hookVehicleDamageReturn;
// 0x0018f644
void* g_hookBulletHitEntity;
// 0x0018e590
PedBulletData g_hookBulletHitData;
// 0x0018e828
CMatrix g_hookBulletInverseMatrix;
// 0x0018ea60
CMatrix g_hookBulletOrthoMatrix;
// 0x0018f66c
DWORD g_hookShotArguments[8];
// 0x0018eb7c
CVector g_hookAdjustedShotPoint;
// 0x0018f648
DWORD g_hookShotLineResult;

// Raw installed callback: EAX returns the preview-ped backing storage.
// 0x000a3ec0
__declspec(naked) void GameHook_000a3ec0()
{
    // clang-format off
    __asm {
        mov eax,offset g_gamePreviewPed
        ret
    }
    // clang-format on
}
// 0x002ac9cc
DWORD g_gameLoopAddress;
// 0x0018f56c
DWORD g_gameHookFarClipReturn;
// 0x00151a10
BYTE g_gameHookJump00151a10[10] = {0xff, 0x25, 0x34, 0x39, 0x4d, 0x00, 0x90, 0x90, 0x90, 0x90};
// 0x00151a1c
BYTE g_gameHookJump00151a1c[7] = {0xff, 0x25, 0x09, 0x46, 0x4d, 0x00, 0x90};
// 0x0015197c
BYTE g_gameHookJump0015197c[7] = {0xff, 0x25, 0xbb, 0x19, 0x69, 0x00, 0x90};
// 0x00151984
BYTE g_gameHookJump00151984[7] = {0xff, 0x25, 0xba, 0xb8, 0x63, 0x00, 0x90};
// 0x00151994
BYTE g_gameHookJump00151994[7] = {0xff, 0x25, 0xaa, 0x85, 0x43, 0x00, 0x90};
// 0x0015198c
BYTE g_gameHookJump0015198c[7] = {0xff, 0x25, 0x79, 0x4a, 0x58, 0x00, 0x90};
// 0x001519b0
BYTE g_gameHookJump001519b0[7] = {0xff, 0x25, 0xf1, 0xc8, 0x53, 0x00, 0x90};
// 0x001519b8
BYTE g_gameHookJump001519b8[6] = {0xff, 0x25, 0xbc, 0x5a, 0x4b, 0x00};
// 0x001519e8
BYTE g_gameHookJump001519e8[6] = {0xff, 0x25, 0x1b, 0x8b, 0x73, 0x00};
// 0x001519f0
BYTE g_gameHookJump001519f0[6] = {0xff, 0x25, 0x5b, 0x88, 0x73, 0x00};
// 0x001519d4
BYTE g_gameHookJump001519d4[9] = {0xff, 0x25, 0x43, 0x00, 0x6a, 0x00, 0x90, 0x90, 0x90};
// 0x00151a34
BYTE g_gameHookJump00151a34[6] = {0xff, 0x25, 0x21, 0xc7, 0x59, 0x00};
// 0x00151a3c
BYTE g_gameHookJump00151a3c[7] = {0xff, 0x25, 0x84, 0x80, 0x53, 0x00, 0x90};
// 0x00151a44
BYTE g_gameHookJump00151a44[9] = {0xff, 0x25, 0xa6, 0x34, 0x55, 0x00, 0x90, 0x90, 0x90};
// 0x00151a50
BYTE g_gameHookJump00151a50[6] = {0xff, 0x25, 0x33, 0x34, 0x4b, 0x00};
// 0x00151a68
BYTE g_gameHookJump00151a68[6] = {0xff, 0x25, 0xdb, 0x74, 0x56, 0x00};
// 0x000a5d80
void GameInstallHook(DWORD address, DWORD function, DWORD storage, const BYTE* jumpCode, int size)
{
    DWORD oldProtect, ignored;
    VirtualProtect((void*)storage, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(DWORD*)storage = function;
    VirtualProtect((void*)storage, 4, oldProtect, &ignored);
    VirtualProtect((void*)address, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy((void*)address, jumpCode, size);
    VirtualProtect((void*)address, size, oldProtect, &ignored);
}
// 0x000a5e30
void GamePrepareHooks()
{
    DWORD local_8;
    DWORD local_4;

    GameUnprotect((void*)0x53eb13, 4);
    *(DWORD*)0x0053eb13 = g_gameLoopAddress - 0x53eb17;
    VirtualProtect((void*)0x0058fc53, 5, 0x40, &local_8);
    *(BYTE*)0x0058fc53 = 0xe8;
    *(DWORD*)0x0058fc54 = (DWORD)GameHook_000a22c0 - 0x58fc58;
    VirtualProtect((void*)0x0058fc53, 5, local_8, &local_4);
    VirtualProtect((void*)0x0058fbbf, 5, 0x40, &local_8);
    *(BYTE*)0x0058fbbf = 0xe8;
    *(DWORD*)0x0058fbc0 = (DWORD)GameHook_000a22f0 - 0x58fbc4;
    VirtualProtect((void*)0x0058fbbf, 5, local_8, &local_4);
    VirtualProtect((void*)0x0053c104, 5, 0x40, &local_8);
    *(BYTE*)0x0053c104 = 0xe8;
    *(DWORD*)0x0053c105 = (DWORD)GameHook_000a23a0 - 0x53c109;
    VirtualProtect((void*)0x0053c104, 5, local_8, &local_4);
    VirtualProtect((void*)0x0053e981, 5, 0x40, &local_8);
    *(BYTE*)0x0053e981 = 0xe8;
    *(DWORD*)0x0053e982 = (DWORD)GameHook_000a23d0 - 0x53e986;
    VirtualProtect((void*)0x0053e981, 5, local_8, &local_4);
    VirtualProtect((void*)0x0086d1b0, 4, 0x40, &local_8);
    *(DWORD*)0x0086d1b0 = (DWORD)GameHook_000a3b90;
    VirtualProtect((void*)0x0086d1b0, 4, local_8, &local_4);
    VirtualProtect((void*)0x0086c0f0, 4, 0x40, &local_8);
    *(DWORD*)0x0086c0f0 = (DWORD)GameHook_000a3b90;
    VirtualProtect((void*)0x0086c0f0, 4, local_8, &local_4);
    VirtualProtect((void*)0x0086c168, 4, 0x40, &local_8);
    *(DWORD*)0x0086c168 = (DWORD)GameHook_000a3b90;
    VirtualProtect((void*)0x0086c168, 4, local_8, &local_4);
    VirtualProtect((void*)0x0086c248, 4, 0x40, &local_8);
    *(DWORD*)0x0086c248 = (DWORD)GameHook_000a3b90;
    VirtualProtect((void*)0x0086c248, 4, local_8, &local_4);
    VirtualProtect((void*)0x0086c3a0, 4, 0x40, &local_8);
    *(DWORD*)0x0086c3a0 = (DWORD)GameHook_000a3b90;
    VirtualProtect((void*)0x0086c3a0, 4, local_8, &local_4);
    VirtualProtect((void*)0x0053e930, 5, 0x40, &local_8);
    *(BYTE*)0x0053e930 = 0xe8;
    *(DWORD*)0x0053e931 = (DWORD)GameHook_000a2200 - 0x53e935;
    VirtualProtect((void*)0x0053e930, 5, local_8, &local_4);
    return;
}

// 0x000a6020
void GameInstallHooks()
{
    DWORD local_8;
    DWORD local_4;

    GamePrepareHooks();
    GameInstallHook(0x4d3aa0, (DWORD)GameHook_000a3f30, 0x4d3934, g_gameHookJump00151a10, 10);
    GameInstallHook(0x4d4610, (DWORD)GameHook_000a3fe0, 0x4d4609, g_gameHookJump00151a1c, 7);
    VirtualProtect((void*)0x0086d190, 4, 0x40, &local_8);
    *(DWORD*)0x0086d190 = (DWORD)GameHook_000a26d0;
    VirtualProtect((void*)0x0086d190, 4, local_8, &local_4);
    VirtualProtect((void*)0x0086c0d0, 4, 0x40, &local_8);
    *(DWORD*)0x0086c0d0 = (DWORD)GameHook_000a29d0;
    VirtualProtect((void*)0x0086c0d0, 4, local_8, &local_4);
    VirtualProtect((void*)0x0086d744, 4, 0x40, &local_8);
    *(DWORD*)0x0086d744 = (DWORD)GameHook_000a2b50;
    VirtualProtect((void*)0x0086d744, 4, local_8, &local_4);
    VirtualProtect((void*)0x007330a2, 5, 0x40, &local_8);
    *(BYTE*)0x007330a2 = 0xe8;
    *(DWORD*)0x007330a3 = (DWORD)GameHook_000a2ab0 - 0x7330a7;
    VirtualProtect((void*)0x007330a2, 5, local_8, &local_4);
    VirtualProtect((void*)0x0086d194, 4, 0x40, &local_8);
    *(DWORD*)0x0086d194 = (DWORD)GameHook_000a3e50;
    VirtualProtect((void*)0x0086d194, 4, local_8, &local_4);
    VirtualProtect((void*)0x005689fd, 5, 0x40, &local_8);
    *(BYTE*)0x005689fd = 0xe8;
    *(DWORD*)0x005689fe = (DWORD)GameHook_000a46b0 - 0x568a02;
    VirtualProtect((void*)0x005689fd, 5, local_8, &local_4);
    VirtualProtect((void*)0x0053ea03, 5, 0x40, &local_8);
    *(BYTE*)0x0053ea03 = 0xe8;
    *(DWORD*)0x0053ea04 = (DWORD)GameHook_000a2370 - 0x53ea08;
    VirtualProtect((void*)0x0053ea03, 5, local_8, &local_4);
    VirtualProtect((void*)0x00871148, 4, 0x40, &local_8);
    *(DWORD*)0x00871148 = (DWORD)GameHook_000a2e70;
    VirtualProtect((void*)0x00871148, 4, local_8, &local_4);
    VirtualProtect((void*)0x008721c8, 4, 0x40, &local_8);
    *(DWORD*)0x008721c8 = (DWORD)GameHook_000a2e70;
    VirtualProtect((void*)0x008721c8, 4, local_8, &local_4);
    VirtualProtect((void*)0x00871388, 4, 0x40, &local_8);
    *(DWORD*)0x00871388 = (DWORD)GameHook_000a2e70;
    VirtualProtect((void*)0x00871388, 4, local_8, &local_4);
    VirtualProtect((void*)0x00871970, 4, 0x40, &local_8);
    *(DWORD*)0x00871970 = (DWORD)GameHook_000a2e70;
    VirtualProtect((void*)0x00871970, 4, local_8, &local_4);
    VirtualProtect((void*)0x008716a8, 4, 0x40, &local_8);
    *(DWORD*)0x008716a8 = (DWORD)GameHook_000a2e70;
    VirtualProtect((void*)0x008716a8, 4, local_8, &local_4);
    VirtualProtect((void*)0x00871550, 4, 0x40, &local_8);
    *(DWORD*)0x00871550 = (DWORD)GameHook_000a2e70;
    VirtualProtect((void*)0x00871550, 4, local_8, &local_4);
    VirtualProtect((void*)0x00871800, 4, 0x40, &local_8);
    *(DWORD*)0x00871800 = (DWORD)GameHook_000a2e70;
    VirtualProtect((void*)0x00871800, 4, local_8, &local_4);
    VirtualProtect((void*)0x00871b10, 4, 0x40, &local_8);
    *(DWORD*)0x00871b10 = (DWORD)GameHook_000a2e70;
    VirtualProtect((void*)0x00871b10, 4, local_8, &local_4);
    VirtualProtect((void*)0x00872398, 4, 0x40, &local_8);
    *(DWORD*)0x00872398 = (DWORD)GameHook_000a2e70;
    VirtualProtect((void*)0x00872398, 4, local_8, &local_4);
    VirtualProtect((void*)0x00871c50, 4, 0x40, &local_8);
    *(DWORD*)0x00871c50 = (DWORD)GameHook_000a2e70;
    VirtualProtect((void*)0x00871c50, 4, local_8, &local_4);
    VirtualProtect((void*)0x00501b1d, 5, 0x40, &local_8);
    *(BYTE*)0x00501b1d = 0xe8;
    *(DWORD*)0x00501b1e = (DWORD)GameHook_000a33c0 - 0x501b22;
    VirtualProtect((void*)0x00501b1d, 5, local_8, &local_4);
    VirtualProtect((void*)0x00501b42, 5, 0x40, &local_8);
    *(BYTE*)0x00501b42 = 0xe8;
    *(DWORD*)0x00501b43 = (DWORD)GameHook_000a33c0 - 0x501b47;
    VirtualProtect((void*)0x00501b42, 5, local_8, &local_4);
    VirtualProtect((void*)0x00501fc2, 5, 0x40, &local_8);
    *(BYTE*)0x00501fc2 = 0xe8;
    *(DWORD*)0x00501fc3 = (DWORD)GameHook_000a33c0 - 0x501fc7;
    VirtualProtect((void*)0x00501fc2, 5, local_8, &local_4);
    VirtualProtect((void*)0x00502067, 5, 0x40, &local_8);
    *(BYTE*)0x00502067 = 0xe8;
    *(DWORD*)0x00502068 = (DWORD)GameHook_000a33c0 - 0x50206c;
    VirtualProtect((void*)0x00502067, 5, local_8, &local_4);
    VirtualProtect((void*)0x005021ae, 5, 0x40, &local_8);
    *(BYTE*)0x005021ae = 0xe8;
    *(DWORD*)0x005021af = (DWORD)GameHook_000a33c0 - 0x5021b3;
    VirtualProtect((void*)0x005021ae, 5, local_8, &local_4);
    VirtualProtect((void*)0x005869bf, 5, 0x40, &local_8);
    *(BYTE*)0x005869bf = 0xe8;
    *(DWORD*)0x005869c0 = (DWORD)GameHook_000a3c60 - 0x5869c4;
    VirtualProtect((void*)0x005869bf, 5, local_8, &local_4);
    VirtualProtect((void*)0x005759e4, 5, 0x40, &local_8);
    *(BYTE*)0x005759e4 = 0xe8;
    *(DWORD*)0x005759e5 = (DWORD)GameHook_000a3c60 - 0x5759e9;
    VirtualProtect((void*)0x005759e4, 5, local_8, &local_4);
    VirtualProtect((void*)0x00609a4e, 5, 0x40, &local_8);
    *(BYTE*)0x00609a4e = 0xe8;
    *(DWORD*)0x00609a4f = (DWORD)GameHook_000a3c80 - 0x609a53;
    VirtualProtect((void*)0x00609a4e, 5, local_8, &local_4);
    VirtualProtect((void*)0x004579c6, 5, 0x40, &local_8);
    *(BYTE*)0x004579c6 = 0xe9;
    *(DWORD*)0x004579c7 = (DWORD)GameHook_000a3ca0 - 0x4579cb;
    VirtualProtect((void*)0x004579c6, 5, local_8, &local_4);
    VirtualProtect((void*)0x0073c252, 5, 0x40, &local_8);
    *(BYTE*)0x0073c252 = 0xe9;
    *(DWORD*)0x0073c253 = (DWORD)GameHook_000a3ad0 - 0x73c257;
    VirtualProtect((void*)0x0073c252, 5, local_8, &local_4);
    VirtualProtect((void*)0x0073ace2, 5, 0x40, &local_8);
    *(BYTE*)0x0073ace2 = 0xe8;
    *(DWORD*)0x0073ace3 = (DWORD)GameHook_000a3ac0 - 0x73ace7;
    VirtualProtect((void*)0x0073ace2, 5, local_8, &local_4);
    VirtualProtect((void*)0x006f8cf8, 5, 0x40, &local_8);
    *(BYTE*)0x006f8cf8 = 0xe8;
    *(DWORD*)0x006f8cf9 = (DWORD)GameHook_000a3b00 - 0x6f8cfd;
    VirtualProtect((void*)0x006f8cf8, 5, local_8, &local_4);
    GameInstallHook(0x6402f0, (DWORD)GameHook_000a3580, 0x6919bb, g_gameHookJump0015197c, 7);
    GameInstallHook(0x63b8c0, (DWORD)GameHook_000a35e0, 0x63b8ba, g_gameHookJump00151984, 7);
    GameInstallHook(0x438576, (DWORD)GameHook_000a37c0, 0x4385aa, g_gameHookJump00151994, 7);
    GameInstallHook(0x584770, (DWORD)GameHook_000a37a0, 0x584a79, g_gameHookJump0015198c, 7);
    GameInstallHook(0x53c900, (DWORD)GameHook_000a3800, 0x53c8f1, g_gameHookJump001519b0, 7);
    GameInstallHook(0x4b5ac0, (DWORD)GameHook_000a3a70, 0x4b5abc, g_gameHookJump001519b8, 6);
    VirtualProtect((void*)0x004d41c0, 5, 0x40, &local_8);
    *(BYTE*)0x004d41c0 = 0xe9;
    *(DWORD*)0x004d41c1 = (DWORD)GameHook_000a3b20 - 0x4d41c5;
    VirtualProtect((void*)0x004d41c0, 5, local_8, &local_4);
    VirtualProtect((void*)0x004e7427, 5, 0x40, &local_8);
    *(BYTE*)0x004e7427 = 0xe8;
    *(DWORD*)0x004e7428 = (DWORD)GameHook_000a3ec0 - 0x4e742c;
    VirtualProtect((void*)0x004e7427, 5, local_8, &local_4);
    GameInstallHook(0x738f3a, (DWORD)GameHook_000a3ce0, 0x738b1b, g_gameHookJump001519e8, 6);
    GameInstallHook(0x738877, (DWORD)GameHook_000a3d20, 0x73885b, g_gameHookJump001519f0, 6);
    GameInstallHook(0x6a0050, (DWORD)GameHook_000a3d90, 0x6a0043, g_gameHookJump001519d4, 9);
    VirtualProtect((void*)0x006fded6, 5, 0x40, &local_8);
    *(BYTE*)0x006fded6 = 0xe8;
    *(DWORD*)0x006fded7 = (DWORD)GameHook_000a4280 - 0x6fdedb;
    VirtualProtect((void*)0x006fded6, 5, local_8, &local_4);
    if (g_gameVersion == 1)
    {
        GameInstallHook(0x7fb020, (DWORD)GameHook_000a4190, 0x59c721, g_gameHookJump00151a34, 6);
        g_gameHookFarClipReturn = 0x7fb026;
    }
    else
    {
        GameInstallHook(0x7fb060, (DWORD)GameHook_000a4190, 0x59c721, g_gameHookJump00151a34, 6);
        g_gameHookFarClipReturn = 0x7fb066;
    }
    VirtualProtect((void*)0x006d0e7e, 5, 0x40, &local_8);
    *(BYTE*)0x006d0e7e = 0xe8;
    *(DWORD*)0x006d0e7f = (DWORD)GameHook_000a42c0 - 0x6d0e83;
    VirtualProtect((void*)0x006d0e7e, 5, local_8, &local_4);
    VirtualProtect((void*)0x00866fa8, 4, 0x40, &local_8);
    *(DWORD*)0x00866fa8 = (DWORD)GameHook_000a42d0;
    VirtualProtect((void*)0x00866fa8, 4, local_8, &local_4);
    VirtualProtect((void*)0x00586c0a, 5, 0x40, &local_8);
    *(BYTE*)0x00586c0a = 0xe8;
    *(DWORD*)0x00586c0b = (DWORD)GameHook_000a4770 - 0x586c0f;
    VirtualProtect((void*)0x00586c0a, 5, local_8, &local_4);
    GameInstallHook(0x538090, (DWORD)GameHook_000a47d0, 0x538084, g_gameHookJump00151a3c, 7);
    VirtualProtect((void*)0x00718599, 5, 0x40, &local_8);
    *(BYTE*)0x00718599 = 0xe8;
    *(DWORD*)0x0071859a = (DWORD)GameHook_000a4960 - 0x71859e;
    VirtualProtect((void*)0x00718599, 5, local_8, &local_4);
    VirtualProtect((void*)0x00866f7c, 4, 0x40, &local_8);
    *(DWORD*)0x00866f7c = (DWORD)GameHook_000a4850;
    VirtualProtect((void*)0x00866f7c, 4, local_8, &local_4);
    VirtualProtect((void*)0x00866f80, 4, 0x40, &local_8);
    *(DWORD*)0x00866f80 = (DWORD)GameHook_000a48c0;
    VirtualProtect((void*)0x00866f80, 4, local_8, &local_4);
    VirtualProtect((void*)0x008585e8, 4, 0x40, &local_8);
    *(DWORD*)0x008585e8 = (DWORD)GameHook_000a48c0;
    VirtualProtect((void*)0x008585e8, 4, local_8, &local_4);
    VirtualProtect((void*)0x00871218, 4, 0x40, &local_8);
    *(DWORD*)0x00871218 = (DWORD)GameHook_000a49d0;
    VirtualProtect((void*)0x00871218, 4, local_8, &local_4);
    VirtualProtect((void*)0x00871778, 4, 0x40, &local_8);
    *(DWORD*)0x00871778 = (DWORD)GameHook_000a49d0;
    VirtualProtect((void*)0x00871778, 4, local_8, &local_4);
    VirtualProtect((void*)0x008718d0, 4, 0x40, &local_8);
    *(DWORD*)0x008718d0 = (DWORD)GameHook_000a49d0;
    VirtualProtect((void*)0x008718d0, 4, local_8, &local_4);
    VirtualProtect((void*)0x00871a40, 4, 0x40, &local_8);
    *(DWORD*)0x00871a40 = (DWORD)GameHook_000a49d0;
    VirtualProtect((void*)0x00871a40, 4, local_8, &local_4);
    VirtualProtect((void*)0x00871be0, 4, 0x40, &local_8);
    *(DWORD*)0x00871be0 = (DWORD)GameHook_000a49d0;
    VirtualProtect((void*)0x00871be0, 4, local_8, &local_4);
    VirtualProtect((void*)0x005648d3, 5, 0x40, &local_8);
    *(BYTE*)0x005648d3 = 0xe8;
    *(DWORD*)0x005648d4 = (DWORD)GameHook_000a4c30 - 0x5648d8;
    VirtualProtect((void*)0x005648d3, 5, local_8, &local_4);
    VirtualProtect((void*)0x0053dfdd, 5, 0x40, &local_8);
    *(BYTE*)0x0053dfdd = 0xe8;
    *(DWORD*)0x0053dfde = (DWORD)GameHook_000a4df0 - 0x53dfe2;
    VirtualProtect((void*)0x0053dfdd, 5, local_8, &local_4);
    VirtualProtect((void*)0x0053e019, 5, 0x40, &local_8);
    *(BYTE*)0x0053e019 = 0xe8;
    *(DWORD*)0x0053e01a = (DWORD)GameHook_000a4e30 - 0x53e01e;
    VirtualProtect((void*)0x0053e019, 5, local_8, &local_4);
    GameInstallHook(0x5534b0, (DWORD)GameHook_000a4e70, 0x5534a6, g_gameHookJump00151a44, 9);
    VirtualProtect((void*)0x005342f9, 5, 0x40, &local_8);
    *(BYTE*)0x005342f9 = 0xe8;
    *(DWORD*)0x005342fa = (DWORD)GameHook_000a5150 - 0x5342fe;
    VirtualProtect((void*)0x005342f9, 5, local_8, &local_4);
    GameInstallHook(0x4b35a0, (DWORD)GameHook_000a5100, 0x4b3433, g_gameHookJump00151a50, 6);
    VirtualProtect((void*)0x0041b02e, 5, 0x40, &local_8);
    *(BYTE*)0x0041b02e = 0xe8;
    *(DWORD*)0x0041b02f = (DWORD)GameHook_000a56b0 - 0x41b033;
    VirtualProtect((void*)0x0041b02e, 5, local_8, &local_4);
    VirtualProtect((void*)0x0041af80, 5, 0x40, &local_8);
    *(BYTE*)0x0041af80 = 0xe8;
    *(DWORD*)0x0041af81 = (DWORD)GameHook_000a5820 - 0x41af85;
    VirtualProtect((void*)0x0041af80, 5, local_8, &local_4);
    VirtualProtect((void*)0x0041ab78, 5, 0x40, &local_8);
    *(BYTE*)0x0041ab78 = 0xe8;
    *(DWORD*)0x0041ab79 = (DWORD)GameHook_000a5850 - 0x41ab7d;
    VirtualProtect((void*)0x0041ab78, 5, local_8, &local_4);
    VirtualProtect((void*)0x00871178, 4, 0x40, &local_8);
    *(DWORD*)0x00871178 = (DWORD)GameHook_000a5870;
    VirtualProtect((void*)0x00871178, 4, local_8, &local_4);
    VirtualProtect((void*)0x008716d8, 4, 0x40, &local_8);
    *(DWORD*)0x008716d8 = (DWORD)GameHook_000a5870;
    VirtualProtect((void*)0x008716d8, 4, local_8, &local_4);
    VirtualProtect((void*)0x008719a0, 4, 0x40, &local_8);
    *(DWORD*)0x008719a0 = (DWORD)GameHook_000a5870;
    VirtualProtect((void*)0x008719a0, 4, local_8, &local_4);
    VirtualProtect((void*)0x00871b40, 4, 0x40, &local_8);
    *(DWORD*)0x00871b40 = (DWORD)GameHook_000a5870;
    VirtualProtect((void*)0x00871b40, 4, local_8, &local_4);
    VirtualProtect((void*)0x008713b8, 4, 0x40, &local_8);
    *(DWORD*)0x008713b8 = (DWORD)GameHook_000a5910;
    VirtualProtect((void*)0x008713b8, 4, local_8, &local_4);
    VirtualProtect((void*)0x00871580, 4, 0x40, &local_8);
    *(DWORD*)0x00871580 = (DWORD)GameHook_000a5910;
    VirtualProtect((void*)0x00871580, 4, local_8, &local_4);
    VirtualProtect((void*)0x00871830, 4, 0x40, &local_8);
    *(DWORD*)0x00871830 = (DWORD)GameHook_000a59b0;
    VirtualProtect((void*)0x00871830, 4, local_8, &local_4);
    VirtualProtect((void*)0x008721f8, 4, 0x40, &local_8);
    *(DWORD*)0x008721f8 = (DWORD)GameHook_000a5a50;
    VirtualProtect((void*)0x008721f8, 4, local_8, &local_4);
    VirtualProtect((void*)0x006e0954, 5, 0x40, &local_8);
    *(BYTE*)0x006e0954 = 0xe8;
    *(DWORD*)0x006e0955 = (DWORD)GameHook_000a5af0 - 0x6e0959;
    VirtualProtect((void*)0x006e0954, 5, local_8, &local_4);
    VirtualProtect((void*)0x006b2bcb, 5, 0x40, &local_8);
    *(BYTE*)0x006b2bcb = 0xe8;
    *(DWORD*)0x006b2bcc = (DWORD)GameHook_000a5af0 - 0x6b2bd0;
    VirtualProtect((void*)0x006b2bcb, 5, local_8, &local_4);
    VirtualProtect((void*)0x004f77da, 5, 0x40, &local_8);
    *(BYTE*)0x004f77da = 0xe8;
    *(DWORD*)0x004f77db = (DWORD)GameHook_000a5af0 - 0x4f77df;
    VirtualProtect((void*)0x004f77da, 5, local_8, &local_4);
    VirtualProtect((void*)0x00872a74, 4, 0x40, &local_8);
    *(DWORD*)0x00872a74 = (DWORD)GameHook_000a5b90;
    VirtualProtect((void*)0x00872a74, 4, local_8, &local_4);
    GameInstallHook(0x5674e0, (DWORD)GameHook_000a5d20, 0x5674db, g_gameHookJump00151a68, 6);
    return;
}

// 0x0018f4c8
DWORD g_hookRadarIndex;
// 0x0018f4cc
DWORD g_hookCheatFunction;
// 0x0018f4b0
DWORD g_hookCameraPed;
// 0x0018f540
DWORD g_hookRenderEsi;
// 0x0018f544
DWORD g_hookRenderEbx;
// 0x0018f568
DWORD g_hookFreedBuffer;
// 0x00151ab8
DWORD g_hookKeepBuffer = 1;
// 0x0018f560
DWORD g_hookCachedBuffer;
// 0x00151abc
DWORD g_hookAllocateFunction = 0x7fb270;
// 0x0018f570
DWORD g_hookSavedEsi;
// 0x0018f5c4
DWORD g_hookRenderReturn;
// 0x00151ac8
BYTE g_hookRenderRestore[3] = {0xff, 0x52, 0x20};
// 0x0018f5d4
DWORD g_hookSavedFontA;
// 0x0018f5d8
DWORD g_hookSavedFontB;

// Yield before entering the original game update routine.
// 0x000a2200
__declspec(naked) void GameHook_000a2200()
{
    // clang-format off
    __asm { push 1
        call dword ptr [Sleep]
        mov edx,0x561a80
        jmp edx }
    // clang-format on
}
// The original argument remains on the caller's stack; Get consumes its copy.
// 0x000a37a0
__declspec(naked) void GameHook_000a37a0()
{
    // clang-format off
    __asm { mov eax,[esp+4]
        mov g_hookRadarIndex,eax
        mov eax,g_hookRadarIndex
        push eax
        call PlayerColors::Get
        ret }
    // clang-format on
}
// 0x000a37c0
__declspec(naked) void GameHook_000a37c0()
{
    // clang-format off
    __asm { mov g_hookCheatFunction,eax
        pushad
        mov edx,0x96918c
        mov byte ptr [edx],1
        popad
        mov edx,0x438581
        cmp eax,0
        jmp edx }
    // clang-format on
}
// clang-format off
// Intentional native one-RET callback, not a replacement for an unknown body.
// 0x000a3ac0
__declspec(naked) void GameHook_000a3ac0() { __asm ret }
// clang-format on
// 0x000a3ad0
__declspec(naked) void GameHook_000a3ad0()
{
    // clang-format off
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
    // clang-format on
}
// Preserve the sign bit while replacing the remaining float bits by 1.0f.
// 0x000a3b00
__declspec(naked) void GameHook_000a3b00()
{
    // clang-format off
    __asm { mov eax,[esi+0x5a4]
        and eax,0x80000000
        or eax,0x3f800000
        mov [esi+0x5a4],eax
        ret }
    // clang-format on
}
// The non-null branch continues after GTA's original PUSH EDI prologue.
// 0x000a3b20
__declspec(naked) void GameHook_000a3b20()
{
    // clang-format off
    __asm { push edi
        mov edi,[esp+8]
        test edi,edi
        jz missing
        mov eax,0x4d41c5
        jmp eax
    missing:
        pop edi
        ret }
    // clang-format on
}
// 0x000a3ce0
__declspec(naked) void GameHook_000a3ce0()
{
    // clang-format off
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
    // clang-format on
}
// EBP is an incoming GTA frame pointer, not a compiler-owned C++ frame.
// 0x000a3d20
__declspec(naked) void GameHook_000a3d20()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a3e50
__declspec(naked) void GameHook_000a3e50()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a4190
__declspec(naked) void GameHook_000a4190()
{
    // clang-format off
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
    // clang-format on
}
// Preserve the native sticky allocator selection and four POP EBX operations.
// 0x000a4280
__declspec(naked) void GameHook_000a4280()
{
    // clang-format off
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
    // clang-format on
}
// clang-format off
// 0x000a42c0
__declspec(naked) void GameHook_000a42c0() { __asm { mov g_hookSavedEsi,esi
        ret } }
// clang-format on
// 0x000a4850
__declspec(naked) void GameHook_000a4850()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a4960
__declspec(naked) void GameHook_000a4960()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a5150
__declspec(naked) void GameHook_000a5150()
{
    // clang-format off
    __asm { mov eax,[esp+4]
        test eax,eax
        jz missing
        mov eax,0x6ff350
        jmp eax
    missing:
        ret }
    // clang-format on
}
// 0x000a5850
__declspec(naked) void GameHook_000a5850()
{
    // clang-format off
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
    // clang-format on
}
// Continue into GTA after its 44-byte local-frame allocation, forwarding the
// observed argument-byte override rather than translating this into a C++ call.
// 0x000a5d20
__declspec(naked) void GameHook_000a5d20()
{
    // clang-format off
    __asm { mov eax,[esp+0x14]
        test eax,eax
        jz original
        mov byte ptr [esp+0x20],1
    original:
        sub esp,0x2c
        mov eax,0x407c37
        jmp eax }
    // clang-format on
}

// 0x0018f504
DWORD g_hookHudScaleX;
// 0x0018f508
DWORD g_hookHudScaleY;
// 0x0018f50c
float g_hookHudWidth;
// 0x0018f510
float g_hookHudHeight;
// 0x00151a6e
BYTE g_hookHudAspectFix = 1;
// 0x0011c728
const float g_hookHudAspectThreshold = 1.6f;
// 0x0011eb68
const float g_hookHudScaleNumerator = 0.00221999991f;
// 0x002aca6c
DWORD g_hookFogEnabled;
// 0x002aca70
DWORD g_hookDisableFog;
// 0x0018e560
DWORD g_hookCollisionPed;
// 0x0018e8e0
DWORD g_hookSavedMatrix[16];
// 0x0018eab4
DWORD g_hookSavedSpeed[3];
// 0x0018f52c
DWORD g_hookHornArgument;
// 0x0018f530
DWORD g_hookHornThis;
// 0x0018eb24
DWORD g_hookHornVehicle;
// 0x0018f534
BYTE g_hookHornStatus;
// 0x0018f4c4
DWORD g_hookAnimationReturn;
// 0x0018e584
DWORD g_hookAnimationClump;
// 0x0018f548
DWORD g_hookAnimationGroup;
// 0x0018f54c
DWORD g_hookAnimationId;
// 0x0018f550
DWORD g_hookAnimationBlend;
// 0x0018eb90
DWORD g_hookAnimationPed;
// 0x0018eb88
DWORD g_hookAnimationLocalClump;
// 0x0018f4ec
DWORD g_hookAnimationType;
// 0x0018f4f0
DWORD g_hookQueuedAnimationGroup;
// 0x0018f4f4
DWORD g_hookQueuedAnimationId;
// 0x0018f4f8
DWORD g_hookQueuedAnimationBlend;
// 0x00151aa4
DWORD g_hookAnimationContinueA = 0x4d3aaa;
// 0x00151aa8
DWORD g_hookAnimationContinueB = 0x4d4617;
// 0x0018f5cc
DWORD g_hookRenderObject;
// 0x00151acc
BYTE g_hookObjectRestore[5] = {0xe8, 0x3c, 0xff, 0xff, 0xff};
// 0x0018f5b4
DWORD g_hookMarker;
// 0x0018f5b8
DWORD g_hookMarkerArgument;
// 0x0018f5bc
DWORD g_hookSuppressAllMarkers;
// The comparison and repeated division retain x87 precision until the final
// float store. A rounded float ratio changes the native 16:10 boundary.
// 0x000a2220
void GamePrepareHudDraw()
{
    g_hookHudScaleX = *(DWORD*)0x859520;
    g_hookHudScaleY = *(DWORD*)0x859524;
    BYTE enabled = g_hookHudAspectFix;
    g_hookHudWidth = (float)*(int*)0xc17044;
    g_hookHudHeight = (float)*(int*)0xc17048;
    if (enabled)
    {
        // clang-format off
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
        // clang-format on
    }
}
// 0x000c5400
void GameSetupD3DFog(DWORD enabled)
{
    if (g_hookDisableFog)
        enabled = 0;
    if (g_gameD3DDevice)
    {
        g_gameD3DDevice->SetRenderState(D3DRS_FOGENABLE, enabled);
        g_gameD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
        g_gameD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
    }
}
// 0x000a22c0
__declspec(naked) void GameHook_000a22c0()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a29d0
__declspec(naked) void GameHook_000a29d0()
{
    // clang-format off
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
    // clang-format on
}
// Original thiscall consumes one stack argument and restores only the status
// byte it saved, leaving the GTA callee's other results intact.
// 0x000a33c0
__declspec(naked) void GameHook_000a33c0()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a3b90
__declspec(naked) void GameHook_000a3b90()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a3f30
__declspec(naked) void GameHook_000a3f30()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a3fe0
__declspec(naked) void GameHook_000a3fe0()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a47d0
__declspec(naked) void GameHook_000a47d0()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a48c0
__declspec(naked) void GameHook_000a48c0()
{
    // clang-format off
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
    // clang-format on
}

// 0x0018f514
BYTE g_hookCameraHudPrepared;
// 0x0018f6a8
DWORD g_hookAudioSavedState;
// 0x0018f6ac
DWORD g_hookAudioArgument0;
// 0x0018f6b0
DWORD g_hookAudioArgument1;

// Raw GTA entry ABIs: preserve live registers and native CALL/JMP/RET behavior.
// 0x000a22f0
__declspec(naked) void GameHook_000a22f0()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a2370
__declspec(naked) void GameHook_000a2370()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a23a0
__declspec(naked) void GameHook_000a23a0()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a5b90
__declspec(naked) void GameHook_000a5b90()
{
    // clang-format off
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
    // clang-format on
}

// 0x0018de44
CObjectPool* g_debugObjectPool;

// 0x000a23d0
void GameHook_000a23d0()
{
    if (g_pGame)
        g_pGame->ProcessFrameLimiter();
    if (g_pNetGame)
    {
        CObjectPool* objects = *(CObjectPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x14);
        if (objects)
        {
            __try
            {
                objects->Process();
            }
            __except (GameExceptionFilter(GetExceptionCode(), GetExceptionInformation(), "s003"))
            {}
        }
    }
    if (g_debugObjectPool)
    {
        __try
        {
            g_debugObjectPool->Process();
        }
        __except (GameExceptionFilter(GetExceptionCode(), GetExceptionInformation(), "s003d"))
        {}
    }
    if (g_pNetGame)
    {
        CVehiclePool* vehicles = *(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0xc);
        if (vehicles)
            vehicles->ConstructLicensePlates();
    }
    if (g_pNetGame)
    {
        CObjectPool* objects = *(CObjectPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x14);
        if (objects)
            objects->ConstructMaterials();
    }
    if (g_pChat)
        g_pChat->RenderToSurface();
    __try
    {
        ((void(__cdecl*)())0x53bee0)();
    }
    __except (GameExceptionFilter(GetExceptionCode(), GetExceptionInformation(), "s007"))
    {}
    if (g_pNetGame)
    {
        CTextDrawPool* textdraws = *(CTextDrawPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0x20);
        if (textdraws)
            textdraws->Native01E330();
    }
}

using AimStuff::ApplyLocalAim;
using AimStuff::ApplyLocalCamera;
using AimStuff::ApplyLocalSkills;
using AimStuff::ApplyPlayerAim;
using AimStuff::ApplyPlayerCamera;
using AimStuff::ApplyPlayerSkills;
using AimStuff::GetPlayerCameraMode;
using AimStuff::UpdateAim;
using AimStuff::UpdateCamera;
using AimStuff::UpdateLocalSkills;
using KeyStuff::ApplyLocalKeys;
using KeyStuff::ApplyPlayerKeys;
using KeyStuff::GetInternalKeys;
using KeyStuff::UpdateKeys;
// Raw entry ABI: weapon ECX=ped; ped control ECX=this, [ESP+4]=ped;
// vehicle ECX=vehicle. Vehicle control preserves PUSHAD registers.
// 0x00151964
WORD* g_hookCameraModeWord = (WORD*)0xb6f858;
// 0x0018f4ac
BYTE g_hookSavedPlayer;
// 0x0018f4b4
BYTE g_hookPlayerIndex;
// 0x0018f4c0
DWORD g_hookPedControlThis;
// 0x0018f51c
DWORD g_hookWeaponResult;
// 0x0018f520
DWORD g_hookRestoreSkills;
// 0x0018f524
DWORD g_hookLocalPedControl;
// 0x0018f528
DWORD g_hookRestoreKeys;
// 0x0018ea58
BYTE g_hookSavedCameraMode;
// 0x0018e5c8
WORD g_hookSavedCameraModeWord;
// 0x0018e810
DWORD g_hookVehicleProcess;
// 0x0018e8d0
DWORD g_hookControlVehicle;
// 0x0018ea54
DWORD g_hookVehicleVtable;
// 0x0018e934
DWORD g_hookVehicleKeys;
// 0x0018e938
BYTE g_hookVehicleStatus;
// 0x0018eb20
DWORD g_hookVehicleLean;
// 0x0018e8cc
DWORD g_hookVehicleLeanTarget;
// 0x002a9a44
int g_gameVehicleModel;
// 0x002a9a48
DWORD g_gameLinkedVehicle;
// 0x002a9a4c
float g_gameVehicleDistanceSquared;

// 0x0011f5dc
const float g_gameStreamingDistanceSquared = 40000.0f;
// 0x000b3f60
BYTE __stdcall GameFindPlayerPedIndex(void* ped)
{
    for (BYTE player = 0; player < 210; ++player)
        if (g_gamePlayerPeds[player] == ped)
            return player;
    return 0;
}
// 0x000b4410
BOOL __stdcall GameIsTrainVehicle(void* vehicle)
{
    if (!vehicle)
        return FALSE;
    g_gameVehicleModel = *(short*)((BYTE*)vehicle + 0x22);
    return g_gameVehicleModel == 0x21a || g_gameVehicleModel == 0x23a || g_gameVehicleModel == 0x219 || g_gameVehicleModel == 0x239 || g_gameVehicleModel == 0x1c1;
}
// 0x000b4460
BOOL __stdcall GameHasOccupiedLinkedVehicle(void* vehicle)
{
    if (!vehicle)
        return FALSE;
    BYTE* linked = *(BYTE**)((BYTE*)vehicle + 0x4c4);
    if (!linked)
        return FALSE;
    g_gameLinkedVehicle = (DWORD)linked;
    BYTE* driver = *(BYTE**)(linked + 0x460);
    return driver && (*(DWORD*)(driver + 0x46c) & 0x100) ? TRUE : FALSE;
}
// 0x000a2ab0
__declspec(naked) void GameHook_000a2ab0()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a2b50
__declspec(naked) void GameHook_000a2b50()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a2e70
__declspec(naked) void GameHook_000a2e70()
{
    // clang-format off
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
    // clang-format on
}
// Retain the unrounded x87 threshold test and native argument cleanup.
// 0x000b6490
__declspec(naked) BOOL __stdcall GameIsVehicleInStreamingArea(void* vehicle)
{
    // clang-format off
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
    // clang-format on
}

// 0x0018f690
DWORD g_hookContactThis;
// 0x0018f694
DWORD g_hookContactOther;
// 0x0018f698
DWORD g_hookContactReturn;
// 0x0018f69c
DWORD g_hookSirenVehicle;
// 0x0018f6a0
BYTE g_hookSirenResult;
// 0x0018f6a4
DWORD g_hookSirenOnce;
// 0x00151b0c
WORD g_hookSirenVehicleId = 0xffff;
// 0x0018f63c
DWORD g_hookPedQueryThis;
// 0x0018f640
DWORD g_hookPedQueryPed;

// Raw GTA collision method ABI; suppression returns with RET8.
// 0x000a5870
__declspec(naked) void GameHook_000a5870()
{
    // clang-format off
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
    // clang-format on
}

// Raw GTA collision method ABI; suppression returns with RET8.
// 0x000a5910
__declspec(naked) void GameHook_000a5910()
{
    // clang-format off
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
    // clang-format on
}

// Raw GTA collision method ABI; suppression returns with RET8.
// 0x000a59b0
__declspec(naked) void GameHook_000a59b0()
{
    // clang-format off
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
    // clang-format on
}

// Raw GTA collision method ABI; suppression returns with RET8.
// 0x000a5a50
__declspec(naked) void GameHook_000a5a50()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a5820
__declspec(naked) void GameHook_000a5820()
{
    // clang-format off
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
    // clang-format on
}
// 0x000a5100
__declspec(naked) void GameHook_000a5100()
{
    // clang-format off
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
    // clang-format on
}
// Raw siren query returns AL. The one-shot path deliberately leaves upper EAX.
// 0x000a5af0
__declspec(naked) void GameHook_000a5af0()
{
    // clang-format off
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
    // clang-format on
}

// 0x0018e81c
CVehicle* g_hookEnterVehicle;
// 0x000a3430
BOOL GameNotifyEnterVehicle()
{
    if (!g_pNetGame)
        return TRUE;
    BYTE* pools = *(BYTE**)((BYTE*)g_pNetGame + 0x3de);
    g_hookEventVehiclePool = *(CVehiclePool**)(pools + 0xc);
    CPlayerPool* players = *(CPlayerPool**)(pools + 8);
    g_hookEventLocalPlayer = *(CLocalPlayer**)((BYTE*)players + 0x1e);
    g_hookEventVehicleId = g_hookEventVehiclePool->Find((void*)g_hookControlVehicle);
    WORD id = g_hookEventVehicleId;
    if (id == 0xffff || id >= 2000 || !g_hookEventVehiclePool->m_bNotEmpty[id])
        return FALSE;
    g_hookEnterVehicle = g_hookEventVehiclePool->m_pObjects[id];
    if (g_hookEnterVehicle->m_dwLocked || *(short*)((BYTE*)g_hookEnterVehicle->m_pGameVehicle + 0x22) == 0x23a)
        return FALSE;
    if (g_hookEventLocalPlayer)
    {
        CPed* ped = *(CPed**)g_hookEventLocalPlayer;
        if (ped && ped->HasDanceState())
            return FALSE;
        if (g_hookEventLocalPlayer)
        {
            ped = *(CPed**)g_hookEventLocalPlayer;
            if (ped && ped->HasDanceStyle())
                return FALSE;
        }
    }
    BYTE* driver = *(BYTE**)((BYTE*)g_hookEnterVehicle->m_pGameVehicle + 0x460);
    if (driver)
    {
        if (*(DWORD*)(driver + 0x598))
            return FALSE;
        WORD player = players->Find(driver);
        if (players->IsConnected(player) && players->GetPlayerInfoFlag(player))
            return FALSE;
    }
    // Native deliberately has no local-player null guard on this final path.
    CPed* ped = *(CPed**)g_hookEventLocalPlayer;
    if (ped && ped->GetCurrentWeapon() == 46)
    {
        ped = *(CPed**)g_hookEventLocalPlayer;
        ped->SetArmedWeapon(0, 0);
    }
    g_hookEventLocalPlayer->EnterVehicle(g_hookEventVehicleId, FALSE);
    return TRUE;
}
// 0x000a3840
WORD GameResolveDamagePlayer(void* entity)
{
    if (!entity)
        return 0xffff;
    CPlayerPool* players = *(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 8);
    WORD result = players->Find(entity);
    if (result == 0xffff)
    {
        CVehiclePool* vehicles = *(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 0xc);
        WORD vehicle = vehicles->Find(entity);
        if (vehicle != 0xffff)
        {
            BYTE* state = (BYTE*)players;
            LONG last = *(LONG*)(state + 0x22);
            for (DWORD slot = 0; (LONG)slot <= last; ++slot)
            {
                WORD player = (WORD)slot;
                if (player < 1004 && *(DWORD*)(state + 0xfd6 + player * 4))
                {
                    BYTE* entry = *(BYTE**)(state + 0x26 + player * 4);
                    BYTE* remote = entry ? *(BYTE**)(entry + 8) : 0;
                    if (*(WORD*)(remote + 2) == vehicle)
                    {
                        result = player;
                        break;
                    }
                }
            }
        }
    }
    return result;
}
// 0x000a38e0
DWORD GameResolveDamageActor(void* entity)
{
    if (!entity)
        return 0xffff;
    CActorPool* actors = *(CActorPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 4);
    return actors->Find(entity);
}
// Observed first four DWORDs only, not a complete native allocation layout.
struct GameDamageResponsePrefix
{
    void* entity;
    float damage;
    DWORD bodyPart, weapon;
};
// 0x000a3910
BOOL __stdcall GameNotifyPedDamage(void* response, void* ped)
{
    void* local = GameFindLocalPed();
    if (!g_pNetGame)
        return FALSE;
    GameDamageResponsePrefix* damage = (GameDamageResponsePrefix*)response;
    void* attacker = damage->entity;
    if (attacker != local && ped && *(DWORD*)ped == 0x86c0a8)
        return TRUE;
    bool taken = ped == local;
    if (!taken && attacker != local)
        return FALSE;
    CPlayerPool* players = *(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 8);
    WORD player;
    if (taken)
    {
        BYTE* localPlayer = *(BYTE**)((BYTE*)players + 0x1e);
        BYTE* protectedObject = *(BYTE**)(localPlayer + 0x2de);
        if (protectedObject)
        {
            if (*(void**)(protectedObject + 0x40) == attacker || damage->weapon == 54)
                return TRUE;
        }
        player = GameResolveDamagePlayer(attacker);
        (*(CLocalPlayer**)((BYTE*)players + 0x1e))->SendTakeDamage(player, damage->damage, damage->weapon, damage->bodyPart);
    }
    else
    {
        player = GameResolveDamagePlayer(ped);
        if (player == 0xffff)
        {
            DWORD actor = GameResolveDamageActor(ped);
            if ((WORD)actor != 0xffff)
            {
                (*(CLocalPlayer**)((BYTE*)players + 0x1e))->SendActorDamage((WORD)actor, damage->damage, damage->weapon, damage->bodyPart);
                return TRUE;
            }
        }
        else
        {
            (*(CLocalPlayer**)((BYTE*)players + 0x1e))->SendGiveDamage(player, damage->damage, damage->weapon, damage->bodyPart);
            if (*(DWORD*)((BYTE*)players->GetPlayer(player) + 0x1c))
                return TRUE;
        }
    }
    BYTE* settings = *(BYTE**)((BYTE*)g_pNetGame + 0x3d5);
    if (settings[0x22])
    {
        BYTE team = (*(BYTE**)((BYTE*)players + 0x1e))[0x130];
        if (team != 0xff && player != 0xffff)
        {
            CPlayerPool* reloaded = *(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 8);
            if (((BYTE*)reloaded->GetPlayer(player))[0x19] == team)
                return TRUE;
        }
    }
    return FALSE;
}

// 0x0018e7f8
DWORD g_hookPickupBackend;
// 0x0018e5cc
DWORD g_hookMenuTextResult;
// 0x0018f5a8
CVector g_hookPassengerCameraPosition;
// 0x0018e580
DWORD g_hookTrailerBackend;
// 0x0018e7fc
CVehiclePool* g_hookTrailerPool;
// 0x0018e7f0
WORD g_hookTrailerId;
// 0x0018f5e0
CVehicle* g_hookTrailerVehicle;
// 0x0018f600
DWORD g_hookObjectSavedStack;
// 0x0018f5f8
DWORD g_hookObjectBackend;
// 0x0018f604
DWORD g_hookObjectResult;
// 0x0018f5fc
CObject* g_hookObjectRotationWrapper;
// 0x0018f614
CObjectPool* g_hookObjectDrawPool;
// 0x0018f618
DWORD g_hookObjectDrawBackend;
// 0x0018f578
CObject* g_hookObjectDrawWrapper;

// Raw installed GTA ABI; void() is an address declaration only.
// 0x000a3c60
__declspec(naked) void GameHook_000a3c60()
{
    // clang-format off
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
    // clang-format on
}

// Raw installed GTA ABI; void() is an address declaration only.
// 0x000a3c80
__declspec(naked) void GameHook_000a3c80()
{
    // clang-format off
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
    // clang-format on
}

// Raw installed GTA ABI; void() is an address declaration only.
// 0x000a3ca0
__declspec(naked) void GameHook_000a3ca0()
{
    // clang-format off
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
    // clang-format on
}

// Raw installed GTA ABI; void() is an address declaration only.
// 0x000a3d90
__declspec(naked) void GameHook_000a3d90()
{
    // clang-format off
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
    // clang-format on
}

// Raw installed GTA ABI; void() is an address declaration only.
// 0x000a4770
__declspec(naked) void GameHook_000a4770()
{
    // clang-format off
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
    // clang-format on
}

// Raw installed GTA ABI; void() is an address declaration only.
// 0x000a49d0
__declspec(naked) void GameHook_000a49d0()
{
    // clang-format off
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
    // clang-format on
}

// Raw installed GTA ABI; void() is an address declaration only.
// 0x000a4c30
__declspec(naked) void GameHook_000a4c30()
{
    // clang-format off
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
    // clang-format on
}

// Raw installed GTA ABI; void() is an address declaration only.
// 0x000a4df0
__declspec(naked) void GameHook_000a4df0()
{
    // clang-format off
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
    // clang-format on
}

// Raw installed GTA ABI; void() is an address declaration only.
// 0x000a4e30
__declspec(naked) void GameHook_000a4e30()
{
    // clang-format off
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
    // clang-format on
}

// Raw installed GTA ABI; void() is an address declaration only.
// 0x000a4e70
__declspec(naked) void GameHook_000a4e70()
{
    // clang-format off
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
    // clang-format on
}

// 0x0018f580
DWORD g_hookMaterialReturn;
// 0x0018f574
DWORD g_hookMaterialBackend;
// 0x0018f57c
CPlayerPool* g_hookMaterialPlayerPool;
// 0x0018f5a4
DWORD g_hookSurfingIndex;
// 0x0018e7f4
CRemotePlayer* g_hookSurfingRemote;
// 0x0018eac0
GameEntityCollisionShape g_hookCollisionFirst;
// 0x0018e56c
GameEntityCollisionShape g_hookCollisionSecond;
// 0x0018f68c
BYTE g_hookCollisionResult;
// 0x0011e508
const float g_hookCollisionBoundHigh = 3000.0f;
// 0x0011ec3c
const float g_hookCollisionBoundLow = -3000.0f;

// Raw installed GTA ABI; void() is an installation-address type only.
// 0x000a42d0
__declspec(naked) void GameHook_000a42d0()
{
    // clang-format off
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
    // clang-format on
}

// Raw installed GTA ABI; void() is an installation-address type only.
// 0x000a46b0
__declspec(naked) void GameHook_000a46b0()
{
    // clang-format off
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
    // clang-format on
}

// Raw installed GTA ABI; void() is an installation-address type only.
// 0x000a56b0
__declspec(naked) void GameHook_000a56b0()
{
    // clang-format off
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
    // clang-format on
}

// 0x000a4fa0
BOOL GameShouldBlockVehicleDamage(void* vehicle, void* attacker, DWORD unused)
{
    (void)unused;
    if (!g_pNetGame || !g_pGame || !g_pGame->GetPlayerPed() || !g_pGame->GetPlayerPed()->IsInVehicle() || g_pGame->GetPlayerPed()->IsPassenger() ||
        g_pGame->GetPlayerPed()->GetVehicle() != vehicle)
        return FALSE;
    WORD id = GameResolveDamagePlayer(attacker);
    g_hookDamagePlayerId = id;
    if (id == 0xffff)
        return FALSE;
    CNetGame* net = (CNetGame*)g_pNetGame;
    CPlayerPool* players = net->pools->players;
    g_hookDamagePlayerPool = players;
    if (!players)
        return FALSE;
    if (!(*(BYTE**)(net->nativeState + 0x3d5))[0x22])
        return FALSE;
    BYTE team = players->local->m_state.team;
    if (team == 0xff)
        return FALSE;
    return net->pools->players->GetPlayer(id)->nativeState[0x19] == team ? TRUE : FALSE;
}
// 0x000a5090
__declspec(naked) void GameHook_000a5090()
{
    // clang-format off
    __asm {
        mov g_hookVehicleDamageReceiver,ecx
        mov eax,[esp]
        mov g_hookVehicleDamageReturn,eax
        mov eax,[esp+4]
        mov g_hookVehicleDamageAttacker,eax
        mov eax,[esp+8]
        mov g_hookVehicleDamageWord,eax
        pushad
        push g_hookVehicleDamageWord
        push g_hookVehicleDamageAttacker
        push g_hookVehicleDamageReceiver
        call GameShouldBlockVehicleDamage
        add esp,0ch
        mov g_hookVehicleDamageBlocked,eax
        popad
        cmp g_hookVehicleDamageBlocked,0
        je vehicle_damage_continue
        xor eax,eax
        ret 18h
    vehicle_damage_continue:
        cmp g_gameVersion,1
        jne vehicle_damage_eu
        push -1
        mov eax,404cdch
        jmp eax
    vehicle_damage_eu:
        push -1
        mov eax,404ce3h
        jmp eax
    }
    // clang-format on
}
// 0x000a5170
void __stdcall GameRecordBulletHit(const CVector* origin, const CVector* target, const CVector* collision, void** entity)
{
    (void)target;
    memset(&g_hookBulletHitData, 0, sizeof(g_hookBulletHitData));
    memcpy(&g_hookBulletHitData.origin, origin, 12);
    memcpy(&g_hookBulletHitData.target, collision, 12);
    if (entity)
    {
        void* hit = *entity;
        g_hookBulletHitEntity = hit;
        if (hit && *(CMatrix**)((BYTE*)hit + 0x14))
        {
            if (!g_netStatsShotCompMode)
            {
                memset(&g_hookBulletInverseMatrix, 0, sizeof(g_hookBulletInverseMatrix));
                memset(&g_hookBulletOrthoMatrix, 0, sizeof(g_hookBulletOrthoMatrix));
                GameOrthoNormalizeRwMatrix(&g_hookBulletOrthoMatrix, *(CMatrix**)((BYTE*)hit + 0x14));
                GameInvertRwMatrix(&g_hookBulletInverseMatrix, &g_hookBulletOrthoMatrix);
                GameTransformPoint(&g_hookBulletHitData.offset, &g_hookBulletInverseMatrix, collision);
                hit = g_hookBulletHitEntity;
            }
            else
            {
                // clang-format off
                __asm {
                    mov edx,hit
                    mov esi,collision
                    mov eax,[edx+14h]
                    fld dword ptr [esi]
                    fsub dword ptr [eax+30h]
                    fstp dword ptr [g_hookBulletHitData+28]
                    mov ecx,[edx+14h]
                    fld dword ptr [esi+4]
                    fsub dword ptr [ecx+34h]
                    fstp dword ptr [g_hookBulletHitData+32]
                    mov eax,[edx+14h]
                    fld dword ptr [esi+8]
                    fsub dword ptr [eax+38h]
                    fstp dword ptr [g_hookBulletHitData+36]
                }
                // clang-format on
            }
            g_hookBulletHitData.entity = hit;
        }
    }
    g_pGame->GetPlayerPed()->SetBulletData(&g_hookBulletHitData);
}
// 0x000a5540
__declspec(naked) void GameHook_000a5540()
{
    // clang-format off
    __asm {
        mov eax,[esp+4]
        mov g_hookShotArguments,eax
        mov eax,[esp+8]
        mov [g_hookShotArguments+4],eax
        mov eax,[esp+0ch]
        mov [g_hookShotArguments+8],eax
        mov eax,[esp+10h]
        mov [g_hookShotArguments+12],eax
        mov eax,[esp+14h]
        mov [g_hookShotArguments+16],eax
        mov eax,[esp+18h]
        mov [g_hookShotArguments+20],eax
        mov eax,[esp+1ch]
        mov [g_hookShotArguments+24],eax
        mov eax,[esp+20h]
        mov [g_hookShotArguments+28],eax
        pushad
        call GameFindLocalPed
        cmp g_hookShotArguments,eax
        je shot_with_collision
        popad
        ret 20h
    shot_with_collision:
        mov eax,g_pNetGame
        test eax,eax
        je shot_fire_local
        mov eax,[eax+3deh]
        mov ecx,[eax+8]
        test ecx,ecx
        je shot_fire_local
        call CPlayerPool::ForceCollision
    shot_fire_local:
        mov ecx,g_pGame
        test ecx,ecx
        je shot_restore_collision
        call CGame::GetPlayerPed
        test eax,eax
        je shot_restore_collision
        mov ecx,g_pGame
        call CGame::GetPlayerPed
        mov ecx,eax
        call CPed::FireWeapon
    shot_restore_collision:
        mov eax,g_pNetGame
        test eax,eax
        je shot_finished
        mov ecx,[eax+3deh]
        mov ecx,[ecx+8]
        test ecx,ecx
        je shot_finished
        call CPlayerPool::RestoreCollision
    shot_finished:
        popad
        ret 20h
    }
    // clang-format on
}
// 0x000a5600
__declspec(naked) void GameHook_000a5600()
{
    // clang-format off
    __asm {
        mov eax,[esp+4]
        mov g_hookShotArguments,eax
        pushad
        call GameFindLocalPed
        cmp g_hookShotArguments,eax
        je shot_three_local
        popad
        ret 0ch
    shot_three_local:
        mov ecx,g_pGame
        test ecx,ecx
        je shot_three_finished
        call CGame::GetPlayerPed
        test eax,eax
        je shot_three_finished
        mov ecx,g_pGame
        call CGame::GetPlayerPed
        mov ecx,eax
        call CPed::FireWeapon
    shot_three_finished:
        popad
        ret 0ch
    }
    // clang-format on
}
// 0x000a5650
__declspec(naked) void GameHook_000a5650()
{
    // clang-format off
    __asm {
        push ebp
        mov ebp,esp
        fld dword ptr [ebp+1ch]
        fmul dword ptr [g_frameScaleNumerator]
        fstp dword ptr [ebp+1ch]
        fld dword ptr [ebp+20h]
        fmul dword ptr [g_frameScaleNumerator]
        fstp dword ptr [ebp+20h]
        fld dword ptr [ebp+24h]
        fmul dword ptr [g_frameScaleNumerator]
        fstp dword ptr [ebp+24h]
        push dword ptr [ebp+24h]
        push dword ptr [ebp+20h]
        push dword ptr [ebp+1ch]
        push dword ptr [ebp+18h]
        push dword ptr [ebp+14h]
        push dword ptr [ebp+10h]
        push dword ptr [ebp+0ch]
        push dword ptr [ebp+8]
        mov edx,736010h
        call edx
        add esp,20h
        mov edx,7360d0h
        call edx
        pop ebp
        ret
    }
    // clang-format on
}
// 0x000a5360
__declspec(naked) DWORD GameProcessShotLineOfSight(const CVector* origin, CVector* target, void* collision, void** entity, DWORD buildings, DWORD vehicles, DWORD peds,
                                                   DWORD objects, DWORD dummies, DWORD seeThrough, DWORD cameraIgnore, DWORD shootThrough)
{
    // clang-format off
    __asm {
        cmp g_netStatsShotCompMode,2
        push ebx
        mov ebx,[esp+0ch]
        push ebp
        mov ebp,[esp+0ch]
        push esi
        push edi
        je shot_los_backend
        mov eax,g_pedFiringContext
        test eax,eax
        je shot_los_backend
        mov ecx,g_pGame
        call CGame::GetPlayerPed
        cmp g_pedFiringContext,eax
        je shot_los_backend
        mov eax,g_pedFiringData
        test eax,eax
        je shot_los_backend
        mov ecx,[eax+28h]
        test ecx,ecx
        je shot_los_backend
        cmp dword ptr [ecx],863c40h
        je shot_los_backend
        mov ecx,[ecx+14h]
        test ecx,ecx
        je shot_los_backend
        mov edx,g_netStatsShotCompMode
        test edx,edx
        jne shot_los_translation
        add eax,1ch
        push eax
        push ecx
        push offset g_hookAdjustedShotPoint
        call GameTransformPoint
        add esp,0ch
        jmp shot_los_endpoint
    shot_los_translation:
        mov ecx,[eax+28h]
        mov edx,[ecx+14h]
        fld dword ptr [edx+30h]
        fadd dword ptr [eax+1ch]
        fstp dword ptr [g_hookAdjustedShotPoint]
        mov ecx,[eax+28h]
        mov edx,[ecx+14h]
        fld dword ptr [edx+34h]
        fadd dword ptr [eax+20h]
        fstp dword ptr [g_hookAdjustedShotPoint+4]
        mov ecx,[eax+28h]
        mov edx,[ecx+14h]
        fld dword ptr [edx+38h]
        fadd dword ptr [eax+24h]
        fstp dword ptr [g_hookAdjustedShotPoint+8]
    shot_los_endpoint:
        fld dword ptr [g_hookAdjustedShotPoint]
        fsub dword ptr [ebp]
        fadd dword ptr [g_hookAdjustedShotPoint]
        fstp dword ptr [ebx]
        fld dword ptr [g_hookAdjustedShotPoint+4]
        fsub dword ptr [ebp+4]
        fadd dword ptr [g_hookAdjustedShotPoint+4]
        fstp dword ptr [ebx+4]
        fld dword ptr [g_hookAdjustedShotPoint+8]
        fsub dword ptr [ebp+8]
        fadd dword ptr [g_hookAdjustedShotPoint+8]
        fstp dword ptr [ebx+8]
    shot_los_backend:
        mov eax,[esp+40h]
        mov ecx,[esp+3ch]
        mov edx,[esp+38h]
        mov edi,[esp+20h]
        mov esi,[esp+1ch]
        push eax
        mov eax,[esp+38h]
        push ecx
        mov ecx,[esp+38h]
        push edx
        mov edx,[esp+38h]
        push eax
        mov eax,[esp+38h]
        push ecx
        mov ecx,[esp+38h]
        push edx
        push eax
        push ecx
        push edi
        push esi
        push ebx
        push ebp
        call GameProcessLineOfSightRaw
        mov g_hookShotLineResult,eax
        mov eax,g_netStatsShotCompMode
        add esp,30h
        cmp eax,2
        je shot_los_record_local
        mov eax,g_pedFiringContext
        test eax,eax
        je shot_los_return
        mov ecx,g_pGame
        call CGame::GetPlayerPed
        cmp g_pedFiringContext,eax
        je shot_los_record_local
        mov eax,g_pedFiringData
        test eax,eax
        je shot_los_record_local
        mov ecx,[eax+28h]
        test ecx,ecx
        jne shot_los_record_local
        call GameFindLocalPed
        cmp [edi],eax
        je shot_los_clear_hit
        call GameFindLocalPed
        mov ecx,[eax+46ch]
        test ch,1
        je shot_los_record_local
        call GameFindLocalPed
        mov edx,[edi]
        cmp edx,[eax+58ch]
        jne shot_los_record_local
    shot_los_clear_hit:
        xor eax,eax
        mov [edi],eax
        pop edi
        mov [esi],eax
        mov [esi+4],eax
        mov [esi+8],eax
        pop esi
        pop ebp
        pop ebx
        ret
    shot_los_record_local:
        mov eax,g_pedFiringContext
        test eax,eax
        je shot_los_return
        mov ecx,g_pGame
        call CGame::GetPlayerPed
        cmp g_pedFiringContext,eax
        jne shot_los_return
        push edi
        push esi
        push ebx
        push ebp
        call GameRecordBulletHit
    shot_los_return:
        mov eax,g_hookShotLineResult
        pop edi
        pop esi
        pop ebp
        pop ebx
        ret
    }
    // clang-format on
}

// Native leaves the cache published during the hooked raster-destroy call.
// 0x000a4200
void GameReleaseCachedRasterBuffer()
{
    DWORD target = g_gameVersion == 1 ? 0x7fb020 : 0x7fb060;
    if (g_hookCachedBuffer)
        ((DWORD(__cdecl*)(DWORD))target)(g_hookCachedBuffer);
    g_hookCachedBuffer = 0;
}

// 0x000a2690
void GameDisablePedRemoteCall()
{
    // GTA SA virtual address: five-byte call site; page remains writable.
    GameUnprotect((void*)0x5e92f4, 5);
    *(DWORD*)0x5e92f4 = 0x90909090;
    *(BYTE*)0x5e92f8 = 0x90;
}
// 0x000a26b0
void GameRestorePedRemoteCall()
{
    // Native restore does not call VirtualProtect or flush the instruction cache.
    *(DWORD*)0x5e92f4 = *(const DWORD*)g_hookPedRemoteRestore;
    *(BYTE*)0x5e92f8 = g_hookPedRemoteRestore[4];
}

// 0x000a5d40
void GameWriteProtectedDword(DWORD address, DWORD value)
{
    BOOL(WINAPI * protect)(LPVOID, SIZE_T, DWORD, PDWORD) = VirtualProtect;
    DWORD oldProtection, ignoredProtection;
    protect((void*)address, 4, PAGE_EXECUTE_READWRITE, &oldProtection);
    *(DWORD*)address = value;
    protect((void*)address, 4, oldProtection, &ignoredProtection);
}
// Opcode byte is passed through without restricting it to CALL/JMP.
// 0x000a5df0
void GameWriteProtectedRelativeBranch(DWORD address, DWORD target, BYTE opcode)
{
    BOOL(WINAPI * protect)(LPVOID, SIZE_T, DWORD, PDWORD) = VirtualProtect;
    DWORD oldProtection, ignoredProtection;
    protect((void*)address, 5, PAGE_EXECUTE_READWRITE, &oldProtection);
    *(BYTE*)address = opcode;
    *(DWORD*)(address + 1) = target - address - 5;
    protect((void*)address, 5, oldProtection, &ignoredProtection);
}

// 0x0018f4d0
DWORD g_hookGate18F4D0;
// 0x0018eb2c
DWORD g_hookRandomReturn;
// 0x0018e8b8
DWORD g_hookRecordReturn18E8B8;
// 0x0018f5c8
DWORD g_hookRecordEcx18F5C8;

// 0x000a22a0
__declspec(naked) void GameRestoreHudDraw()
{
    // clang-format off
    __asm {
        mov eax, dword ptr [g_hookHudScaleX]
        mov dword ptr ds:[0x859520], eax
        mov ecx, dword ptr [g_hookHudScaleY]
        mov dword ptr ds:[0x859524], ecx
        ret
    }
    // clang-format on
}

// 0x000a2630
__declspec(naked) void GameDisablePedControlCalls()
{
    // clang-format off
    __asm {
        mov eax, 0x90909090
        mov ecx, 0x6884c4
        mov dword ptr [ecx], eax
        mov word ptr [ecx + 4], ax
        mov edx, eax
        mov eax, 0x688200
        mov dword ptr [eax], edx
        mov word ptr [eax + 4], dx
        ret
    }
    // clang-format on
}

// 0x000a2650
__declspec(naked) void GameRestorePedControlCalls()
{
    // clang-format off
    __asm {
        mov ecx, dword ptr [g_hookPedRestoreA]
        mov eax, 0x6884c4
        mov dword ptr [eax], ecx
        mov dx, word ptr [g_hookPedRestoreA+4]
        mov word ptr [eax + 4], dx
        mov ecx, dword ptr [g_hookPedRestoreB]
        mov eax, 0x688200
        mov dword ptr [eax], ecx
        mov dx, word ptr [g_hookPedRestoreB+4]
        mov word ptr [eax + 4], dx
        ret
    }
    // clang-format on
}

// 0x000a3780
__declspec(naked) void GameHook_000a3780()
{
    // clang-format off
    __asm {
        pushad
        cmp dword ptr [g_hookGate18F4D0], 0
        jne hook_a378e
        popad
        xor eax, eax
        ret
    hook_a378e:
        popad
        mov eax, 0x421446
        jmp eax
    }
    // clang-format on
}

// 0x000a37e0
__declspec(naked) void GameHook_000a37e0()
{
    // clang-format off
    __asm {
        fld dword ptr [g_gameFarClipDistance]
        push esi
        mov esi, dword ptr [esp + 8]
        mov edx, dword ptr [g_gameVersionAddressB]
        jmp edx
    }
    // clang-format on
}

// 0x000a3b40
__declspec(naked) void GameHook_000a3b40()
{
    // clang-format off
    __asm {
        mov eax, dword ptr ds:[0xc17054]
        test eax, eax
        je hook_a3b50
        push 0x746929
        ret
    hook_a3b50:
        xor eax, eax
        ret
    }
    // clang-format on
}

// 0x000a3d50
__declspec(naked) void GameHook_000a3d50()
{
    // clang-format off
    __asm {
        push esi
        test ecx, ecx
        je hook_a3d64
        mov esi, dword ptr ss:[esp + 8]
        mov eax, dword ptr ds:[esi]
        mov edx, 0x63a047
        jmp edx
    hook_a3d64:
        mov esi, 0x63a04d
        jmp esi
    }
    // clang-format on
}

// 0x000a3d70
__declspec(naked) void GameHook_000a3d70()
{
    // clang-format off
    __asm {
        test eax, eax
        je hook_a3d82
        mov ecx, 0x47eab8
        mov cl, byte ptr ds:[0xa43c7c]
        jmp ecx
    hook_a3d82:
        mov esi, 0x47eaca
        jmp esi
    }
    // clang-format on
}

// 0x000a3e40
__declspec(naked) void GameHook_000a3e40()
{
    // clang-format off
    __asm {
        mov eax, dword ptr [esp]
        mov dword ptr [g_hookRandomReturn], eax
        call rand
        ret
    }
    // clang-format on
}

// 0x000a4240
__declspec(naked) void* GameAllocateHookBuffer()
{
    // clang-format off
    __asm {
        cmp dword ptr [g_gameVersion], 1
        push ebx
        jne hook_a4254
        mov dword ptr [g_hookAllocateFunction], 0x7fb230
    hook_a4254:
        push 0x604
        push 0x20
        push 0x10
        push 0x40
        mov ebx, dword ptr [g_hookAllocateFunction]
        call ebx
        mov dword ptr [g_hookCachedBuffer], eax
        pop ebx
        pop ebx
        pop ebx
        pop ebx
        pop ebx
        ret
    }
    // clang-format on
}

// 0x000a4c10
__declspec(naked) void GameHook_000a4c10()
{
    // clang-format off
    __asm {
        mov eax, dword ptr [esp + 4]
        push eax
        mov eax, 0x5618d0
        call eax
        pop edx
        pushad
        mov ecx, dword ptr [g_pGame]
        test ecx, ecx
        je hook_a4c2d
        call CGame::ProcessFrameLimiter
    hook_a4c2d:
        popad
        ret
    }
    // clang-format on
}

// 0x000a4c90
__declspec(naked) void GameHook_000a4c90()
{
    // clang-format off
    __asm {
        mov ecx, dword ptr [g_pGame]
        call CGame::IsMenuVisible
        test eax, eax
        je hook_a4ca0
        ret
    hook_a4ca0:
        mov edx, 0x69efc0
        jmp edx
    }
    // clang-format on
}

// 0x000a4cb0
__declspec(naked) void GameHook_000a4cb0()
{
    // clang-format off
    __asm {
        pushad
        popad
        mov eax, 0x61e3f0
        jmp eax
    }
    // clang-format on
}

// 0x000a4170
__declspec(naked) void GameHook_000a4170()
{
    // clang-format off
    __asm {
        mov eax, dword ptr [esp]
        mov dword ptr [g_hookRecordReturn18E8B8], eax
        pushad
        popad
        mov eax, 0x4c95c0
        jmp eax
    }
    // clang-format on
}

// 0x000a48a0
__declspec(naked) void GameHook_000a48a0()
{
    // clang-format off
    __asm {
        mov eax, dword ptr [esp]
        mov dword ptr [g_hookRenderReturn], eax
        mov dword ptr [g_hookRecordEcx18F5C8], ecx
        pushad
        popad
        mov eax, 0x5a2bd0
        jmp eax
    }
    // clang-format on
}

// 0x0011ebe4
const char g_hookPedSayFormat[] = "CPed::Say(%u,%u,%f,%u,%u,%u)";
// 0x0011ec04
const char g_hookGamepadAimFormat[] = "Joypad: %d LocalContext: %u UpdateCameraAim: %f %f %f";
// 0x00151aac
DWORD g_hookCapture00151AAC = 0x4d4417;
// 0x00151ab0
DWORD g_hookCapture00151AB0 = 0x4d3b30;
// 0x00151ab4
WORD g_hookCapture00151AB4 = 0xffff;
// 0x00151ac4
WORD g_hookCapture00151AC4 = 0xffff;
// 0x0018e540
DWORD g_hookCapture0018E540;
// 0x0018e544
DWORD g_hookCapture0018E544;
// 0x0018e564
DWORD g_hookCapture0018E564;
// 0x0018e588
DWORD g_hookCapture0018E588;
// 0x0018e58c
DWORD g_hookCapture0018E58C;
// 0x0018e5c4
DWORD g_hookCapture0018E5C4;
// 0x0018e5d8
DWORD g_hookCapture0018E5D8;
// 0x0018e800
CVector g_hookAimRayTarget;
// 0x0018e814
DWORD g_hookCapture0018E814;
// 0x0018e818
DWORD g_hookCapture0018E818;
// 0x0018e86c
DWORD g_hookCapture0018E86C;
// 0x0018e874
DWORD g_hookCapture0018E874;
// 0x0018e8c8
DWORD g_hookCapture0018E8C8;
// 0x0018e8d4
DWORD g_hookCapture0018E8D4;
// 0x0018e924
DWORD g_hookCapture0018E924;
// 0x0018e928
DWORD g_hookCapture0018E928;
// 0x0018ea40
CVector g_hookAimRayOrigin;
// 0x0018eaa0
DWORD g_hookCapture0018EAA0;
// 0x0018eaa4
DWORD g_hookCapture0018EAA4;
// 0x0018eaa8
DWORD g_hookCapture0018EAA8;
// 0x0018eab0
DWORD g_hookCapture0018EAB0;
// 0x0018eb1c
DWORD g_hookCapture0018EB1C;
// 0x0018eb70
DWORD g_hookCapture0018EB70;
// 0x0018eb78
DWORD g_hookCapture0018EB78;
// 0x0018eb8c
DWORD g_hookCapture0018EB8C;
// 0x0018f33c
DWORD g_hookCapture0018F33C;
// 0x0018f340
DWORD g_hookCapture0018F340;
// 0x0018f34c
DWORD g_hookCapture0018F34C;
// 0x0018f350
DWORD g_hookCapture0018F350;
// 0x0018f354
DWORD g_hookCapture0018F354;
// 0x0018f360
DWORD g_hookCapture0018F360;
// 0x0018f368
DWORD g_hookCapture0018F368;
// 0x0018f37c
DWORD g_hookCapture0018F37C;
// 0x0018f558
DWORD g_hookCapture0018F558;
// 0x0018f55c
DWORD g_hookCapture0018F55C;
// 0x0018f594
DWORD g_hookCapture0018F594;
// 0x0018f598
DWORD g_hookCapture0018F598;
// 0x0018f59c
DWORD g_hookCapture0018F59C;
// 0x0018f5a0
DWORD g_hookCapture0018F5A0;
// 0x0018f5e4
DWORD g_hookCapture0018F5E4;
// 0x0018f658
DWORD g_hookCapture0018F658;
// 0x0018f65c
DWORD g_hookCapture0018F65C;
// 0x0018f660
DWORD g_hookCapture0018F660;

// 0x000a2cd0
__declspec(naked) void GameHook_000a2cd0()
{
    // clang-format off
    __asm {
        mov dword ptr [g_hookPedControlThis], ecx
        mov eax, dword ptr [esp + 4]
        mov dword ptr [g_hookCameraPed], eax
        push 4
        push 0x858ef0
        call GameUnprotect
        mov eax, dword ptr ds:[0x858ef0]
        mov dword ptr [g_hookCapture0018E588], eax
        add esp, 8
        mov dword ptr ds:[0x858ef0], 0x40000000
        mov ecx, dword ptr [g_hookPedControlThis]
        push dword ptr [g_hookCameraPed]
        mov eax, 0x62a380
        call eax
        mov dword ptr [g_hookCapture0018F37C], eax
        mov ecx, dword ptr [g_hookCapture0018E588]
        mov dword ptr ds:[0x858ef0], ecx
        mov eax, dword ptr [g_hookCapture0018F37C]
        ret 4
    }
    // clang-format on
}

// 0x000a3b60
__declspec(naked) void GameHook_000a3b60()
{
    // clang-format off
    __asm {
        mov edi, dword ptr [esp + 4]
        mov dword ptr [g_hookPickupBackend], edi
        mov dword ptr [g_hookCapture0018EB8C], ecx
        pushad
        mov edx, dword ptr [edi]
        mov ecx, edi
        call dword ptr [edx + 4]
        mov dword ptr [g_hookCapture0018E58C], eax
        popad
        mov eax, dword ptr fs:[0]
        mov edx, 0x4c3876
        jmp edx
    }
    // clang-format on
}

// 0x000a3bc0
__declspec(naked) void GameHook_000a3bc0()
{
    // clang-format off
    __asm {
        mov eax, dword ptr [esp + 4]
        mov dword ptr [g_hookCapture0018E8C8], eax
        mov eax, dword ptr [esp + 8]
        mov dword ptr [g_hookCapture0018F340], eax
        mov eax, dword ptr [esp + 0xc]
        mov dword ptr [g_hookCapture0018EAA4], eax
        mov eax, dword ptr [esp + 0x10]
        mov dword ptr [g_hookCapture0018F33C], eax
        mov eax, dword ptr [esp + 0x14]
        mov dword ptr [g_hookCapture0018E544], eax
        mov eax, dword ptr [esp + 0x18]
        mov dword ptr [g_hookCapture0018E928], eax
        pushad
        cmp dword ptr [g_hookCapture0018E8C8], 0x2d
        je raw_a3c4a
        cmp dword ptr [g_pChat], 0
        je raw_a3c4a
        mov eax, dword ptr [g_hookCapture0018E928]
        push eax
        mov ecx, dword ptr [g_hookCapture0018E544]
        push ecx
        mov edx, dword ptr [g_hookCapture0018F33C]
        push edx
        fld dword ptr [g_hookCapture0018EAA4]
        sub esp, 8
        fstp qword ptr [esp]
        mov eax, dword ptr [g_hookCapture0018F340]
        push eax
        mov ecx, dword ptr [g_hookCapture0018E8C8]
        push ecx
        push offset g_hookPedSayFormat
        mov edx, dword ptr [g_pChat]
        push edx
        call CChat::AddDebugMessage
        add esp, 0x24
    raw_a3c4a:
        popad
        mov eax, dword ptr [esp + 4]
        test ax, ax
        mov edx, 0x5effe7
        jmp edx
    }
    // clang-format on
}

// 0x000a3e70
__declspec(naked) void GameHook_000a3e70()
{
    // clang-format off
    __asm {
        mov eax, dword ptr [esp + 4]
        mov dword ptr [g_hookCapture0018EAB0], eax
        mov eax, dword ptr [esp + 8]
        mov dword ptr [g_hookCapture0018F350], eax
        push 1
        push 0x6d64f0
        call GameUnprotect
        add esp, 8
        mov byte ptr ds:[0x6d64f0], 0xc3
        push 0
        push dword ptr [g_hookCapture0018EAB0]
        mov edx, 0x811c80
        call edx
        pop edx
        pop edx
        mov edx, 0x4c8848
        cmp byte ptr [esp + 8], 0xff
        jmp edx
    }
    // clang-format on
}

// 0x000a40a0
__declspec(naked) void GameHook_000a40a0()
{
    // clang-format off
    __asm {
        mov edx, dword ptr [esp]
        mov dword ptr [g_hookAnimationReturn], edx
        mov edx, dword ptr [esp + 4]
        mov dword ptr [g_hookAnimationClump], edx
        mov edx, dword ptr [esp + 8]
        mov dword ptr [g_hookCapture0018EB1C], edx
        mov edx, dword ptr [esp + 0xc]
        mov dword ptr [g_hookAnimationId], edx
        mov edx, dword ptr [esp + 0x10]
        mov dword ptr [g_hookAnimationBlend], edx
        pushad
        call GameFindLocalPed
        mov dword ptr [g_hookAnimationPed], eax
        cmp dword ptr [g_hookAnimationPed], 0
        je raw_a40f5
        mov eax, dword ptr [g_hookAnimationPed]
        mov ecx, dword ptr [eax + 0x18]
        mov dword ptr [g_hookAnimationLocalClump], ecx
        jmp raw_a40ff
    raw_a40f5:
        mov dword ptr [g_hookAnimationLocalClump], 0
    raw_a40ff:
        popad
        push -1
        push 0x83bd6b
        jmp dword ptr [g_hookCapture00151AAC]
    }
    // clang-format on
}

// 0x000a4110
__declspec(naked) void GameHook_000a4110()
{
    // clang-format off
    __asm {
        mov edx, dword ptr [esp + 4]
        mov dword ptr [g_hookAnimationClump], edx
        mov edx, dword ptr [esp + 8]
        mov dword ptr [g_hookCapture0018F558], edx
        mov edx, dword ptr [esp + 0xc]
        mov dword ptr [g_hookAnimationGroup], edx
        mov edx, dword ptr [esp + 0x10]
        mov dword ptr [g_hookAnimationId], edx
        pushad
        mov eax, dword ptr [g_hookAnimationId]
        push eax
        mov ecx, dword ptr [g_hookAnimationGroup]
        push ecx
        call GameGetAnimationGroupKey
        mov dword ptr [g_hookCapture0018F55C], eax
        mov edx, dword ptr [g_hookCapture0018F55C]
        push edx
        call GameFindAnimationByKey
        add esp, 4
        mov word ptr [g_hookCapture00151AB4], ax
        popad
        jmp dword ptr [g_hookCapture00151AB0]
    }
    // clang-format on
}

// 0x000a43d0
__declspec(naked) void GameHook_000a43d0()
{
    // clang-format off
    __asm {
        mov eax, dword ptr [esp]
        mov dword ptr [g_hookCapture0018EAA8], eax
        mov eax, dword ptr [esp + 4]
        mov dword ptr [g_hookCapture0018F594], eax
        mov eax, dword ptr [esp + 8]
        mov dword ptr [g_hookCapture0018EB78], eax
        mov eax, dword ptr [esp + 0xc]
        mov dword ptr [g_hookCapture0018E5C4], eax
        mov eax, dword ptr [esp + 0x10]
        mov dword ptr [g_hookCapture0018E5D8], eax
        mov eax, dword ptr [esp + 0x14]
        mov dword ptr [g_hookCapture0018F354], eax
        pushad
        call GameFindLocalPed
        cmp dword ptr [g_hookCapture0018F594], eax
        jne raw_a4444
        cmp dword ptr [g_pNetGame], 0
        je raw_a4444
        mov ecx, dword ptr [g_pNetGame]
        call CNetGame::GetPlayerPool
        mov ecx, eax
        call CPlayerPool::GetLocalPlayer
        mov ecx, eax
        call CLocalPlayer::GetPed
        mov ecx, eax
        call CPed::HasDanceState
        test eax, eax
        je raw_a4444
        popad
        ret 0x14
    raw_a4444:
        popad
        mov eax, 0x6a6ae0
        jmp eax
    }
    // clang-format on
}

// 0x000a4450
__declspec(naked) void GameHook_000a4450()
{
    // clang-format off
    __asm {
        mov dword ptr [g_hookCapture0018F598], esi
        pushad
        cmp dword ptr [g_pNetGame], 0
        je raw_a4505
        mov ecx, dword ptr [g_pNetGame]
        call CNetGame::GetVehiclePool
        mov dword ptr [g_hookCapture0018F59C], eax
        mov eax, dword ptr [g_hookCapture0018F598]
        push eax
        mov ecx, dword ptr [g_hookCapture0018F59C]
        call CVehiclePool::Find
        mov word ptr [g_hookCapture00151AC4], ax
        movzx ecx, word ptr [g_hookCapture00151AC4]
        cmp ecx, 0xffff
        je raw_a4505
        mov dx, word ptr [g_hookCapture00151AC4]
        push edx
        mov ecx, dword ptr [g_hookCapture0018F59C]
        call CVehiclePool::Get
        mov dword ptr [g_hookCapture0018F5A0], eax
        cmp dword ptr [g_hookCapture0018F5A0], 0
        je raw_a4505
        mov ecx, dword ptr [g_hookCapture0018F5A0]
        call CVehicle::GetField54
        test eax, eax
        je raw_a44e7
        mov eax, dword ptr [g_hookCapture0018F598]
        movzx ecx, byte ptr [eax + 0x428]
        or ecx, 0x40
        mov edx, dword ptr [g_hookCapture0018F598]
        mov byte ptr [edx + 0x428], cl
        jmp raw_a4505
    raw_a44e7:
        mov eax, dword ptr [g_hookCapture0018F598]
        movzx ecx, byte ptr [eax + 0x428]
        and ecx, 0xbf
        mov edx, dword ptr [g_hookCapture0018F598]
        mov byte ptr [edx + 0x428], cl
    raw_a4505:
        popad
        mov eax, 0x6d55c0
        jmp eax
    }
    // clang-format on
}

// 0x000a4aa0
__declspec(naked) void GameHook_000a4aa0()
{
    // clang-format off
    __asm {
        mov dword ptr [g_hookCapture0018E540], ecx
        mov eax, dword ptr [esp + 4]
        mov dword ptr [g_hookCapture0018F360], eax
        mov eax, dword ptr [esp + 8]
        mov dword ptr [g_hookCapture0018EAA0], eax
        mov eax, dword ptr [esp + 0xc]
        mov dword ptr [g_hookCapture0018E874], eax
        mov eax, dword ptr [esp + 0x10]
        mov dword ptr [g_hookCapture0018EB70], eax
        mov eax, dword ptr [esp + 0x14]
        mov dword ptr [g_hookCapture0018E818], eax
        mov eax, dword ptr [esp + 0x18]
        mov dword ptr [g_hookCapture0018E924], eax
        pushad
        popad
        push dword ptr [g_hookCapture0018E924]
        push dword ptr [g_hookCapture0018E818]
        push dword ptr [g_hookCapture0018EB70]
        push dword ptr [g_hookCapture0018E874]
        push dword ptr [g_hookCapture0018EAA0]
        push dword ptr [g_hookCapture0018F360]
        mov ecx, dword ptr [g_hookCapture0018E540]
        mov eax, 0x514970
        call eax
        pushad
        popad
        ret 0x18
    }
    // clang-format on
}

// 0x000a4b20
__declspec(naked) void GameHook_000a4b20()
{
    // clang-format off
    __asm {
        mov ebx, dword ptr [esp + 4]
        mov dword ptr [g_hookCapture0018F5E4], ebx
        mov ebx, dword ptr [esp + 8]
        mov dword ptr [g_hookCapture0018E564], ebx
        mov ebx, dword ptr [esp + 0xc]
        mov dword ptr [g_hookCapture0018E814], ebx
        pushad
        cmp dword ptr [g_pGame], 0
        je raw_a4b73
        mov ecx, dword ptr [g_pGame]
        call CGame::GetPlayerPed
        test eax, eax
        je raw_a4b73
        push offset g_hookAimRayTarget
        push offset g_hookAimRayOrigin
        mov ecx, dword ptr [g_pGame]
        call CGame::GetPlayerPed
        mov ecx, eax
        call CPed::GetCameraAimRay
    raw_a4b73:
        popad
        mov ebx, 0x742300
        jmp ebx
    }
    // clang-format on
}

// 0x000a4b80
__declspec(naked) void GameHook_000a4b80()
{
    // clang-format off
    __asm {
        mov ebx, dword ptr [esp + 4]
        mov dword ptr [g_hookCapture0018F5E4], ebx
        mov ebx, dword ptr [esp + 8]
        mov dword ptr [g_hookCapture0018E564], ebx
        mov ebx, dword ptr [esp + 0xc]
        mov dword ptr [g_hookCapture0018E814], ebx
        mov ebx, dword ptr [esp + 0x10]
        mov dword ptr [g_hookCapture0018E8D4], ebx
        mov ebx, dword ptr [esp + 0x14]
        mov dword ptr [g_hookCapture0018F34C], ebx
        mov ebx, dword ptr [esp + 0x18]
        mov dword ptr [g_hookCapture0018E86C], ebx
        mov ebx, dword ptr [esp + 0x1c]
        mov dword ptr [g_hookCapture0018F368], ebx
        pushad
        cmp dword ptr [g_pGame], 0
        je raw_a4bfb
        mov ecx, dword ptr [g_pGame]
        call CGame::GetPlayerPed
        test eax, eax
        je raw_a4bfb
        push offset g_hookAimRayTarget
        push offset g_hookAimRayOrigin
        mov ecx, dword ptr [g_pGame]
        call CGame::GetPlayerPed
        mov ecx, eax
        call CPed::GetCameraAimRay
    raw_a4bfb:
        popad
        mov ebx, 0x73fb10
        jmp ebx
    }
    // clang-format on
}

// 0x000a5290
__declspec(naked) void GameHook_000a5290()
{
    // clang-format off
    __asm {
        mov dword ptr [g_hookCapture0018F658], ecx
        pushad
        mov ecx, dword ptr [g_pGame]
        test ecx, ecx
        je raw_a52b2
        call CGame::UsingGamepad
        test eax, eax
        je raw_a52b2
        popad
        mov ecx, dword ptr [g_hookCapture0018F658]
        ret
    raw_a52b2:
        mov ecx, dword ptr [g_hookCapture0018F658]
        mov edx, 0x60dc50
        call edx
        popad
        mov ecx, dword ptr [g_hookCapture0018F658]
        ret
    }
    // clang-format on
}

// 0x000a52d0
__declspec(naked) void GameHook_000a52d0()
{
    // clang-format off
    __asm {
        mov dword ptr [g_hookCapture0018F660], ecx
        mov eax, dword ptr [esp + 4]
        mov dword ptr [g_hookCapture0018F65C], eax
        pushad
        mov eax, dword ptr [g_pNetGame]
        test eax, eax
        je raw_a5333
        mov eax, dword ptr [g_pChat]
        test eax, eax
        je raw_a5333
        mov eax, dword ptr [g_hookCapture0018F65C]
        fld dword ptr [eax + 8]
        mov ecx, dword ptr [g_pGame]
        sub esp, 0x18
        fstp qword ptr [esp + 0x10]
        fld dword ptr [eax + 4]
        fstp qword ptr [esp + 8]
        fld dword ptr [eax]
        mov eax, dword ptr [g_hookLocalPedControl]
        fstp qword ptr [esp]
        push eax
        call CGame::UsingGamepad
        mov ecx, dword ptr [g_pChat]
        push eax
        push offset g_hookGamepadAimFormat
        push ecx
        call CChat::AddDebugMessage
        add esp, 0x28
    raw_a5333:
        mov ecx, dword ptr [g_hookCapture0018F660]
        push dword ptr [g_hookCapture0018F65C]
        mov eax, 0x50cb10
        call eax
        popad
        ret 4
    }
    // clang-format on
}

// Additional native control/camera/patch callbacks and their shared aggregates.
// 0x0018e878
CMatrix g_hookAimOffsetMatrix;
// 0x0018ead8
CMatrix g_hookMountedMatrix;
// 0x00151ad4
DWORD g_hookPatchOverride151AD4[3] = {0x0, 0x0, 0x41700000};
// 0x00151ae0
BYTE g_hookRestore5543C0[22] = {0x8b, 0xce, 0xe8, 0x79, 0x1, 0xfe, 0xff, 0x84, 0xc0, 0x74, 0xb5, 0x8b, 0xce, 0xe8, 0xe, 0xb7, 0x1c, 0x0, 0x84, 0xc0, 0x75, 0xaa};
// 0x00151af8
WORD g_hookRestore5543DC = 0x4774;
// 0x0018e548
DWORD g_hookCapture0018E548;
// 0x0018e54c
DWORD g_hookCapture0018E54C;
// 0x0018e7e4
DWORD g_hookCapture0018E7E4;
// 0x0018e7e8
DWORD g_hookCapture0018E7E8;
// 0x0018e7ec
DWORD g_hookCapture0018E7EC;
// 0x0018e80c
DWORD g_hookCapture0018E80C;
// 0x0018e92c
DWORD g_hookCapture0018E92C;
// 0x0018eaac
DWORD g_hookCapture0018EAAC;
// 0x0018eb74
DWORD g_hookCapture0018EB74;
// 0x0018f338
DWORD g_hookCapture0018F338;
// 0x0018f344
DWORD g_hookCapture0018F344;
// 0x0018f348
DWORD g_hookCapture0018F348;
// 0x0018f358
DWORD g_hookCapture0018F358;
// 0x0018f35c
DWORD g_hookCapture0018F35C;
// 0x0018f364
DWORD g_hookCapture0018F364;
// 0x0018f380
DWORD g_hookCapture0018F380;
// 0x0018f48c
DWORD g_hookCapture0018F48C;
// 0x0018f490
DWORD g_hookCapture0018F490;
// 0x0018f608
DWORD g_hookCapture0018F608;
// 0x0018f60c
DWORD g_hookCapture0018F60C;
// 0x0018f610
DWORD g_hookCapture0018F610;

// 0x000a2d30
__declspec(naked) void GameHook_000a2d30()
{
    // clang-format off
    __asm {
        mov dword ptr [g_hookCapture0018F48C], ecx
        mov eax, dword ptr [esp + 4]
        mov dword ptr [g_hookCameraPed], eax
        mov eax, dword ptr [esp + 8]
        mov dword ptr [g_hookCapture0018E548], eax
        mov eax, dword ptr [esp + 0xc]
        mov dword ptr [g_hookCapture0018F364], eax
        mov eax, dword ptr [g_clientCrashPedContext]
        mov edx, dword ptr [g_hookCameraPed]
        mov cl, byte ptr [eax]
        push edx
        mov byte ptr [g_hookSavedPlayer], cl
        call GameFindPlayerPedIndex
        mov ecx, dword ptr [g_hookCameraPed]
        test ecx, ecx
        mov byte ptr [g_hookPlayerIndex], al
        je context_a2dbe
        test al, al
        je context_a2dbe
        mov al, byte ptr [g_hookSavedPlayer]
        test al, al
        jne context_a2dbe
        call AimStuff::UpdateLocalSkills
        movzx eax, byte ptr [g_hookPlayerIndex]
        push eax
        call AimStuff::ApplyPlayerSkills
        push dword ptr [g_hookCapture0018F364]
        push dword ptr [g_hookCapture0018E548]
        push dword ptr [g_hookCameraPed]
        mov ecx, dword ptr [g_hookCapture0018F48C]
        mov edx, 0x6857e0
        call edx
        call AimStuff::ApplyLocalSkills
        jmp context_a2ddd
    context_a2dbe:
        push dword ptr [g_hookCapture0018F364]
        push dword ptr [g_hookCapture0018E548]
        push dword ptr [g_hookCameraPed]
        mov ecx, dword ptr [g_hookCapture0018F48C]
        mov edx, 0x6857e0
        call edx
    context_a2ddd:
        ret 0xc
    }
    // clang-format on
}

// 0x000a2de0
__declspec(naked) void GameHook_000a2de0()
{
    // clang-format off
    __asm {
        mov eax, dword ptr [esp + 4]
        mov dword ptr [g_hookCameraPed], eax
        mov dword ptr [g_hookCapture0018F48C], ecx
        mov eax, dword ptr [g_clientCrashPedContext]
        mov edx, dword ptr [g_hookCameraPed]
        mov cl, byte ptr [eax]
        push edx
        mov byte ptr [g_hookSavedPlayer], cl
        call GameFindPlayerPedIndex
        mov ecx, dword ptr [g_hookCameraPed]
        test ecx, ecx
        mov byte ptr [g_hookPlayerIndex], al
        je context_a2e50
        test al, al
        je context_a2e50
        mov al, byte ptr [g_hookSavedPlayer]
        test al, al
        jne context_a2e50
        call AimStuff::UpdateLocalSkills
        movzx eax, byte ptr [g_hookPlayerIndex]
        push eax
        call AimStuff::ApplyPlayerSkills
        push dword ptr [g_hookCameraPed]
        mov ecx, dword ptr [g_hookCapture0018F48C]
        mov edx, 0x688810
        call edx
        call AimStuff::ApplyLocalSkills
        jmp context_a2e63
    context_a2e50:
        push dword ptr [g_hookCameraPed]
        mov ecx, dword ptr [g_hookCapture0018F48C]
        mov edx, 0x688810
        call edx
    context_a2e63:
        ret 4
    }
    // clang-format on
}

// 0x000a4510
__declspec(naked) void GameHook_000a4510()
{
    // clang-format off
    __asm {
        mov dword ptr [g_hookCapture0018E80C], ecx
        mov eax, dword ptr [esp]
        mov dword ptr [g_hookCapture0018F35C], eax
        mov eax, dword ptr [esp + 4]
        mov dword ptr [g_hookCapture0018F358], eax
        mov eax, dword ptr [esp + 8]
        mov dword ptr [g_hookCapture0018E54C], eax
        mov eax, dword ptr [esp + 0xc]
        mov dword ptr [g_hookCapture0018E92C], eax
        mov eax, dword ptr [esp + 0x10]
        mov dword ptr [g_hookCapture0018F338], eax
        pushad
        push dword ptr [g_hookCapture0018F338]
        push dword ptr [g_hookCapture0018E92C]
        push dword ptr [g_hookCapture0018E54C]
        push dword ptr [g_hookCapture0018F358]
        mov ecx, dword ptr [g_hookCapture0018E80C]
        mov edx, 0x521500
        call edx
        mov ecx, dword ptr [g_pGame]
        push offset g_hookAimOffsetMatrix
        call CGame::GetPlayerPed
        mov ecx, eax
        call CPed::GetAimOffsetMatrix
        mov ecx, dword ptr [g_pGame]
        call CGame::GetPlayerPed
        mov ecx, eax
        call CPed::IsInVehicle
        test eax, eax
        je context_a4647
        mov ecx, dword ptr [g_pGame]
        call CGame::GetPlayerPed
        mov ecx, eax
        call CPed::GetVehicle
        test eax, eax
        mov dword ptr [g_hookCapture0018F380], eax
        je context_a4647
        mov esi, dword ptr [eax + 0x14]
        push 0x42b40000
        push 2
        mov ecx, 0x10
        mov edi, offset g_hookMountedMatrix
        push offset g_hookMountedMatrix
        rep movsd
        call GameRotateRwMatrixOnAxis
        push 0x42b40000
        push 0
        push offset g_hookMountedMatrix
        call GameRotateRwMatrixOnAxis
        add esp, 0x18
        call AimStuff::GetInternalAim
        mov ecx, dword ptr [g_hookMountedMatrix]
        mov dword ptr [g_hookCapture0018EB74], eax
        mov dword ptr [eax], ecx
        fld dword ptr [g_hookMountedMatrix+0x4]
        mov edx, dword ptr [g_hookCapture0018EB74]
        fstp dword ptr [edx + 4]
        mov eax, dword ptr [g_hookCapture0018EB74]
        fld dword ptr [g_hookMountedMatrix+0x8]
        fstp dword ptr [eax + 8]
        mov ecx, dword ptr [g_hookCapture0018EB74]
        fld dword ptr [g_hookMountedMatrix+0x10]
        fstp dword ptr [ecx + 0x24]
        mov edx, dword ptr [g_hookCapture0018EB74]
        fld dword ptr [g_hookMountedMatrix+0x14]
        fstp dword ptr [edx + 0x28]
        mov eax, dword ptr [g_hookCapture0018EB74]
        fld dword ptr [g_hookMountedMatrix+0x18]
        fstp dword ptr [eax + 0x2c]
    context_a4647:
        fld dword ptr [g_hookAimOffsetMatrix+0x30]
        mov ecx, dword ptr [g_hookCapture0018E80C]
        fstp dword ptr [ecx + 0x19c]
        mov edx, dword ptr [g_hookCapture0018E80C]
        fld dword ptr [g_hookAimOffsetMatrix+0x34]
        fstp dword ptr [edx + 0x1a0]
        mov eax, dword ptr [g_hookCapture0018E80C]
        fld dword ptr [g_hookAimOffsetMatrix+0x38]
        fstp dword ptr [eax + 0x1a4]
        popad
        ret 0x10
    }
    // clang-format on
}

// 0x000a4680
__declspec(naked) void GameHook_000a4680()
{
    // clang-format off
    __asm {
        mov eax, dword ptr [esp + 4]
        mov dword ptr [g_hookCapture0018EAAC], eax
        mov eax, dword ptr [esp + 8]
        mov dword ptr [g_hookCapture0018F490], eax
        mov eax, dword ptr [esp + 0xc]
        mov dword ptr [g_hookCapture0018F344], eax
        pushad
        popad
        mov eax, 0x40c6b0
        jmp eax
    }
    // clang-format on
}

// 0x000a4cc0
__declspec(naked) void GameHook_000a4cc0()
{
    // clang-format off
    __asm {
        mov eax, dword ptr [esp + 4]
        mov edx, dword ptr [g_hookCapture0018F608]
        cmp eax, edx
        je context_a4cd5
        mov edx, 0x554230
        jmp edx
    context_a4cd5:
        mov edx, dword ptr [esp + 4]
        mov dword ptr [g_hookCapture0018F60C], edx
        mov edx, dword ptr [esp + 8]
        mov dword ptr [g_hookCapture0018F348], edx
        pushad
        mov eax, 0xb76870
        mov ecx, dword ptr [eax]
        mov dword ptr [g_hookCapture0018E7E4], ecx
        mov edx, dword ptr [eax + 4]
        mov dword ptr [g_hookCapture0018E7E8], edx
        mov eax, dword ptr [eax + 8]
        mov dword ptr [g_hookCapture0018E7EC], eax
        mov ecx, 0xb76870
        mov edx, dword ptr [g_hookPatchOverride151AD4]
        mov dword ptr [ecx], edx
        mov eax, dword ptr [g_hookPatchOverride151AD4+0x4]
        mov dword ptr [ecx + 4], eax
        mov edx, dword ptr [g_hookPatchOverride151AD4+0x8]
        mov dword ptr [ecx + 8], edx
        push 0x20
        push 0x5543c0
        call GameUnprotect
        add esp, 8
        mov eax, 0x90909090
        mov ecx, 0x5543c0
        mov dword ptr [ecx], eax
        mov dword ptr [ecx + 4], eax
        mov dword ptr [ecx + 8], eax
        mov dword ptr [ecx + 0xc], eax
        mov dword ptr [ecx + 0x10], eax
        mov word ptr [ecx + 0x14], ax
        mov edx, 0x90909090
        mov eax, 0x5543dc
        mov word ptr [eax], dx
        push dword ptr [g_hookCapture0018F348]
        push dword ptr [g_hookCapture0018F60C]
        mov edx, 0x554230
        call edx
        mov dword ptr [g_hookCapture0018F610], eax
        pop edx
        pop edx
        mov ecx, 0xb76870
        mov edx, dword ptr [g_hookCapture0018E7E4]
        mov dword ptr [ecx], edx
        mov eax, dword ptr [g_hookCapture0018E7E8]
        mov dword ptr [ecx + 4], eax
        mov edx, dword ptr [g_hookCapture0018E7EC]
        mov dword ptr [ecx + 8], edx
        mov eax, 0x5543c0
        mov ecx, dword ptr [g_hookRestore5543C0]
        mov dword ptr [eax], ecx
        mov edx, dword ptr [g_hookRestore5543C0+0x4]
        mov dword ptr [eax + 4], edx
        mov ecx, dword ptr [g_hookRestore5543C0+0x8]
        mov dword ptr [eax + 8], ecx
        mov edx, dword ptr [g_hookRestore5543C0+0xc]
        mov dword ptr [eax + 0xc], edx
        mov ecx, dword ptr [g_hookRestore5543C0+0x10]
        mov dword ptr [eax + 0x10], ecx
        mov dx, word ptr [g_hookRestore5543C0+0x14]
        mov word ptr [eax + 0x14], dx
        mov ax, word ptr [g_hookRestore5543DC]
        mov word ptr ds:[0x5543dc], ax
        popad
        mov eax, dword ptr [g_hookCapture0018F610]
        ret
    }
    // clang-format on
}

// 0x002aca50
DWORD g_clientPreviousRenderState;

// 0x002aca54
DWORD g_clientRenderCaller;

// 0x002aca58
DWORD g_clientRenderState;

// 0x002aca5c
DWORD g_clientRenderValue;

// 0x000c54c0
void ClientInstallRenderStateHook()
{
    // GTA SA VA: 0x00c97b24; capture the engine table once, as native does.
    BYTE* table = *(BYTE**)0xc97b24;
    g_clientPreviousRenderState = *(DWORD*)(table + 0x20);
    *(DWORD*)(table + 0x20) = (DWORD)ClientRenderStateHook;
}

// 0x000c5450
__declspec(naked) void ClientRenderStateHook()
{
    // clang-format off
    __asm {
        mov eax,[esp]
        mov g_clientRenderCaller,eax
        mov eax,[esp+4]
        mov g_clientRenderState,eax
        mov eax,[esp+8]
        mov g_clientRenderValue,eax
        cmp g_clientRenderState,14
        jne forward
        cmp g_clientRenderValue,0
        je fog_off
        push 1
        call GameSetupD3DFog
        add esp,4
        mov g_hookFogEnabled,1
        jmp clear_fog
    fog_off:
        push 0
        call GameSetupD3DFog
        add esp,4
        mov g_hookFogEnabled,0
    clear_fog:
        mov byte ptr [esp+8],0
    forward:
        mov eax,g_clientPreviousRenderState
        jmp eax
    }
    // clang-format on
}

// 0x0018f4a8
DWORD g_hookIgnoreNextExit;
// 0x0018e550
DWORD g_hookExitTask;
// 0x0018e5d4
CVehiclePool* g_hookEventVehiclePool;
// 0x0018e568
WORD g_hookEventVehicleId;
// 0x0018e868
CVehicle* g_hookExitVehicle;
// 0x0018eb18
CLocalPlayer* g_hookEventLocalPlayer;
// 0x0018eb30
CMatrix g_hookExitMatrix;
// 0x00151aa0
DWORD g_hookExitTaskArgument = 0xffffffff;
// 0x0011c65c
const float g_hookTrainExitOffset = 2.5f;
// 0x0018e5bc
DWORD g_hookQuitDeadline;
// 0x0018f538
DWORD g_hookDamageResponse;
// 0x0018f53c
DWORD g_hookDamagePed;
// Raw GTA task ABI: continuation jumps and original constructor stack words.
// 0x000a3580
__declspec(naked) void GameHook_000a3580()
{
    // clang-format off
    __asm {
        mov eax,[esp]
        mov g_hookAnimationReturn,eax
        mov eax,[esp+4]
        mov g_hookControlVehicle,eax
        pushad
        cmp g_hookAnimationReturn,0x570a20
        je notify
        cmp g_hookAnimationReturn,0x570a99
        jne original
    notify:
        call GameNotifyEnterVehicle
        test eax,eax
        jne original
        popad
        mov ebx,[ecx]
        test ebx,ebx
        jz cancelled
        push 1
        call dword ptr [ebx]
    cancelled:
        pop eax
        pop eax
        mov eax,0x570a9e
        jmp eax
    original:
        popad
        mov eax,[esp+4]
        push esi
        push 0
        mov ebx,0x6402f7
        jmp ebx
    }
    // clang-format on
}
// 0x000a35e0
__declspec(naked) void GameHook_000a35e0()
{
    // clang-format off
    __asm {
        mov ebp,esp
        mov g_hookExitTask,ecx
        mov eax,[ebp]
        mov g_hookAnimationReturn,eax
        mov eax,[ebp+4]
        mov g_hookControlVehicle,eax
        mov g_hookExitTaskArgument,0xffffffff
        cmp g_hookIgnoreNextExit,0
        jne clear_ignore
        cmp g_hookAnimationReturn,0x5704a1
        je notify
        cmp g_hookAnimationReturn,0x5703fc
        jne clear_ignore
    notify:
        cmp g_pNetGame,0
        je done
        call GameFindLocalPed
        mov eax,[eax+0x58c]
        cmp eax,g_hookControlVehicle
        jne done
        mov ecx,g_pNetGame
        call CNetGame::GetVehiclePool
        mov g_hookEventVehiclePool,eax
        call GameFindLocalPed
        mov ecx,[eax+0x58c]
        push ecx
        mov ecx,g_hookEventVehiclePool
        call CVehiclePool::Find
        mov g_hookEventVehicleId,ax
        movzx edx,g_hookEventVehicleId
        cmp edx,0xffff
        je done
        mov ax,g_hookEventVehicleId
        push eax
        mov ecx,g_hookEventVehiclePool
        call CVehiclePool::Get
        mov g_hookExitVehicle,eax
        mov ecx,g_pNetGame
        call CNetGame::GetPlayerPool
        mov ecx,eax
        call CPlayerPool::GetLocalPlayer
        mov g_hookEventLocalPlayer,eax
        cmp g_hookExitVehicle,0
        je done
        cmp g_hookEventLocalPlayer,0
        je done
        mov ecx,g_hookExitVehicle
        call CVehicle::IsTrainPart
        test eax,eax
        je send_exit
        mov g_hookExitTaskArgument,1
        push offset g_hookExitMatrix
        mov ecx,g_hookExitVehicle
        call CEntity::GetMatrix
        mov ecx,dword ptr [g_hookExitMatrix+0x38]
        push ecx
        fld dword ptr [g_hookExitMatrix+0x34]
        fadd g_hookTrainExitOffset
        push ecx
        fstp dword ptr [esp]
        fld dword ptr [g_hookExitMatrix+0x30]
        fadd g_hookTrainExitOffset
        push ecx
        fstp dword ptr [esp]
        mov ecx,g_hookEventLocalPlayer
        call CLocalPlayer::GetPed
        mov ecx,eax
        call CPed::WarpFromVehicle
        jmp done
    send_exit:
        mov dx,g_hookEventVehicleId
        push edx
        mov ecx,g_hookEventLocalPlayer
        call CLocalPlayer::ExitVehicle
        jmp done
    clear_ignore:
        mov g_hookIgnoreNextExit,0
    done:
        push g_hookExitTaskArgument
        push 0x841618
        mov ecx,g_hookExitTask
        mov edx,0x63b8c0
        add edx,7
        jmp edx
    }
    // clang-format on
}
// 0x000a3a70
__declspec(naked) void GameHook_000a3a70()
{
    // clang-format off
    __asm {
        mov eax,fs:[0]
        test ecx,ecx
        jz original
        mov g_hookDamageResponse,ecx
        mov ebx,[esp+4]
        test ebx,ebx
        jz original
        mov g_hookDamagePed,ebx
        pushad
        mov eax,g_hookDamagePed
        push eax
        mov ecx,g_hookDamageResponse
        push ecx
        call GameNotifyPedDamage
        test eax,eax
        jz continue_original
        popad
        ret 12
    continue_original:
        popad
    original:
        mov edx,0x4b5ac6
        jmp edx
    }
    // clang-format on
}
// Native does not return. Preserve its unsigned deadline comparison/wrap behavior.
// 0x000a3800
__declspec(naked) void GameHook_000a3800()
{
    // clang-format off
    __asm {
        mov esi,dword ptr [GetTickCount]
        call esi
        add eax,2000
        mov g_hookQuitDeadline,eax
        call ClientRequestQuit
        call esi
        cmp eax,g_hookQuitDeadline
        jae exit_game
        mov edi,dword ptr [Sleep]
    wait_loop:
        push 100
        call edi
        call esi
        cmp eax,g_hookQuitDeadline
        jb wait_loop
    exit_game:
        push 0
        call dword ptr [ExitProcess]
    }
    // clang-format on
}

// 0x0018e8c0
DWORD g_hookPedControlField540;
// 0x0018f378
CPlayerPool* g_hookFaultPlayerPool;
// 0x00151970
WORD g_hookFaultPlayerId = 0xffff;
// 0x0018e824
CRemotePlayer* g_hookFaultRemotePlayer;
// 0x00151a70
const BYTE g_hookPedRestoreA[6] = {0xd9, 0x96, 0x5c, 0x05, 0, 0};
// 0x00151a88
const BYTE g_hookPedRestoreB[6] = {0xd9, 0x96, 0x5c, 0x05, 0, 0};
// 0x00151a90
const BYTE g_hookPedRemoteRestore[5] = {0xe8, 0x17, 0x88, 0xff, 0xff};
// 0x0011ebcc
const char g_hookPedExceptionLabel[10] = "playerped";
// 0x0011ebac
const char g_hookPedRemovedMessage[32] = "Removed player %u due to error.";
// 0x000a2530
void GameRunPedControlBackend()
{
    __try
    {
        // GTA SA thiscall backend, ECX=the current saved ped.
        ((void(__fastcall*)(void*, void*))0x60ea90)((void*)g_hookCameraPed, 0);
    }
    __except (GameExceptionFilter(GetExceptionCode(), GetExceptionInformation(), g_hookPedExceptionLabel))
    {
        if (*g_clientCrashPedContext && g_pNetGame)
        {
            g_hookFaultPlayerPool = *(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 8);
            if (g_hookFaultPlayerPool)
            {
                WORD id = g_hookFaultPlayerPool->Find((void*)g_hookCameraPed);
                g_hookFaultPlayerId = id;
                if (id != 0xffff)
                {
                    g_hookFaultRemotePlayer = g_hookFaultPlayerPool->GetPlayer(id);
                    if (g_hookFaultRemotePlayer)
                    {
                        g_hookFaultRemotePlayer->Remove();
                        id = g_hookFaultPlayerId;
                    }
                    if (g_pChat)
                        g_pChat->AddDebugMessage(g_hookPedRemovedMessage, (unsigned)id);
                }
            }
        }
    }
}
// Extracted readable body; outer native entry preserves PUSHAD state.
// samp.dll RVA: unknown (reconstruction-only body helper extracted from A26D0; no standalone native entry)
void GameProcessPedControlBody()
{
    g_hookCollisionPed = g_hookCameraPed;
    g_hookSavedPlayer = *g_clientCrashPedContext;
    g_hookPlayerIndex = GameFindPlayerPedIndex((void*)g_hookCameraPed);
    if (g_hookCameraPed && g_hookPlayerIndex && !g_hookSavedPlayer)
    {
        KeyStuff::UpdateKeys();
        KeyStuff::ApplyPlayerKeys(g_hookPlayerIndex);
        g_hookSavedCameraMode = *g_aimInternalCameraMode;
        *g_aimInternalCameraMode = AimStuff::GetPlayerCameraMode(g_hookPlayerIndex);
        g_hookSavedCameraModeWord = *g_hookCameraModeWord;
        *g_hookCameraModeWord = AimStuff::GetPlayerCameraMode(g_hookPlayerIndex);
        if (*g_hookCameraModeWord == 4)
            *g_hookCameraModeWord = 0;
        AimStuff::UpdateCamera();
        AimStuff::ApplyPlayerCamera(g_hookPlayerIndex);
        AimStuff::UpdateAim();
        AimStuff::ApplyPlayerAim(g_hookPlayerIndex);
        AimStuff::UpdateLocalSkills();
        AimStuff::ApplyPlayerSkills(g_hookPlayerIndex);
        *g_clientCrashPedContext = g_hookPlayerIndex;
        g_hookPedControlField540 = *(DWORD*)(g_hookCollisionPed + 0x540);
        GameUnprotect((void*)0x5e92f4, 5);
        *(DWORD*)0x5e92f4 = 0x90909090;
        *(BYTE*)0x5e92f8 = 0x90;
        GameRunPedControlBackend();
        memcpy((void*)0x5e92f4, g_hookPedRemoteRestore, 5);
        AimStuff::ApplyLocalSkills();
        *g_aimInternalCameraMode = g_hookSavedCameraMode;
        *g_hookCameraModeWord = g_hookSavedCameraModeWord;
        AimStuff::ApplyLocalCamera();
        *g_clientCrashPedContext = 0;
        KeyStuff::ApplyLocalKeys();
        AimStuff::ApplyLocalAim();
    }
    else
    {
        if (g_pNetGame && *(DWORD*)(*(BYTE**)(*(BYTE**)(*(BYTE**)((BYTE*)g_pNetGame + 0x3de) + 8) + 0x1e) + 0xf4))
        {
            BYTE* settings = *(BYTE**)((BYTE*)g_pNetGame + 0x3d5);
            float positiveX = *(float*)(settings + 5), negativeX = *(float*)(settings + 9);
            float positiveY = *(float*)(settings + 0xd), negativeY = *(float*)(settings + 0x11);
            if (g_pGame->GetPlayerPed()->HasExceededWorldBoundries(positiveX, negativeX, positiveY, negativeY) && !g_pGame->GetCurrentInterior() &&
                !(*(DWORD*)(g_hookCollisionPed + 0x46c) & 0x100) && (*(DWORD*)(g_hookCollisionPed + 0x46c) & 3))
            {
                g_hookVehicleKeys = (DWORD)KeyStuff::GetInternalKeys();
                *(WORD*)(g_hookVehicleKeys + 0x1c) = 0xff;
                *(WORD*)(g_hookVehicleKeys + 0x4c) = 0;
            }
        }
        if (g_pGame->GetPlayerPed())
        {
            if (g_pGame->GetPlayerPed()->unknown2f7)
            {
                g_hookVehicleKeys = (DWORD)KeyStuff::GetInternalKeys();
                *(WORD*)(g_hookVehicleKeys + 0x24) = 0;
                *(WORD*)(g_hookVehicleKeys + 0x54) = 0;
            }
            if (g_pGame->GetPlayerPed()->unknown2f6)
            {
                g_hookVehicleKeys = (DWORD)KeyStuff::GetInternalKeys();
                *(WORD*)(g_hookVehicleKeys + 0x1c) = 0;
                *(WORD*)(g_hookVehicleKeys + 0x4c) = 0;
            }
            if (g_pGame->GetPlayerPed()->unknown2f8)
            {
                g_hookVehicleKeys = (DWORD)KeyStuff::GetInternalKeys();
                *(WORD*)(g_hookVehicleKeys + 0x20) = 0;
                *(WORD*)(g_hookVehicleKeys + 0x50) = 0;
            }
        }
        memcpy((void*)0x6884c4, g_hookPedRestoreA, 6);
        memcpy((void*)0x688200, g_hookPedRestoreB, 6);
        ((void(__fastcall*)(void*, void*))0x60ea90)((void*)g_hookCameraPed, 0);
        memset((void*)0x6884c4, 0x90, 6);
        memset((void*)0x688200, 0x90, 6);
    }
}
// Raw entry: ECX=GTA ped; original preserves all PUSHAD registers and returns.
// 0x000a26d0
__declspec(naked) void GameHook_000a26d0()
{
    // clang-format off
    __asm {
        mov g_hookCameraPed,ecx
        pushad
        call GameProcessPedControlBody
        popad
        ret
    }
    // clang-format on
}
