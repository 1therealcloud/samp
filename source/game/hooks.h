#ifndef SAMP_GAME_HOOKS_H
#define SAMP_GAME_HOOKS_H
#include <windows.h>
// samp.dll RVA: 0x002ac9cc
extern DWORD g_gameLoopAddress;
// samp.dll RVA: 0x0018f56c
extern DWORD g_gameHookFarClipReturn;
// samp.dll RVA: 0x00151a10
extern BYTE g_gameHookJump00151a10[10];
// samp.dll RVA: 0x00151a1c
extern BYTE g_gameHookJump00151a1c[7];
// samp.dll RVA: 0x0015197c
extern BYTE g_gameHookJump0015197c[7];
// samp.dll RVA: 0x00151984
extern BYTE g_gameHookJump00151984[7];
// samp.dll RVA: 0x00151994
extern BYTE g_gameHookJump00151994[7];
// samp.dll RVA: 0x0015198c
extern BYTE g_gameHookJump0015198c[7];
// samp.dll RVA: 0x001519b0
extern BYTE g_gameHookJump001519b0[7];
// samp.dll RVA: 0x001519b8
extern BYTE g_gameHookJump001519b8[6];
// samp.dll RVA: 0x001519e8
extern BYTE g_gameHookJump001519e8[6];
// samp.dll RVA: 0x001519f0
extern BYTE g_gameHookJump001519f0[6];
// samp.dll RVA: 0x001519d4
extern BYTE g_gameHookJump001519d4[9];
// samp.dll RVA: 0x00151a34
extern BYTE g_gameHookJump00151a34[6];
// samp.dll RVA: 0x00151a3c
extern BYTE g_gameHookJump00151a3c[7];
// samp.dll RVA: 0x00151a44
extern BYTE g_gameHookJump00151a44[9];
// samp.dll RVA: 0x00151a50
extern BYTE g_gameHookJump00151a50[6];
// samp.dll RVA: 0x00151a68
extern BYTE g_gameHookJump00151a68[6];
// Raw callback declarations for installing addresses. Recovered bodies and exact
// entry ABIs are tracked in the callback review; remaining bodies stay open.
// Do not call these through the provisional void() types from ordinary C++.
// samp.dll RVA: 0x000a2200
void GameHook_000a2200();
// samp.dll RVA: 0x000a22c0
void GameHook_000a22c0();
// samp.dll RVA: 0x000a22f0
void GameHook_000a22f0();
// samp.dll RVA: 0x000a2370
void GameHook_000a2370();
// samp.dll RVA: 0x000a23a0
void GameHook_000a23a0();
// samp.dll RVA: 0x000a23d0
void GameHook_000a23d0();
// samp.dll RVA: 0x000a26d0
void GameHook_000a26d0();
// samp.dll RVA: 0x000a29d0
void GameHook_000a29d0();
// samp.dll RVA: 0x000a2ab0
void GameHook_000a2ab0();
// samp.dll RVA: 0x000a2b50
void GameHook_000a2b50();
// samp.dll RVA: 0x000a2e70
void GameHook_000a2e70();
// samp.dll RVA: 0x000a33c0
void GameHook_000a33c0();
// samp.dll RVA: 0x000a3580
void GameHook_000a3580();
// samp.dll RVA: 0x000a35e0
void GameHook_000a35e0();
// samp.dll RVA: 0x000a37a0
void GameHook_000a37a0();
// samp.dll RVA: 0x000a37c0
void GameHook_000a37c0();
// samp.dll RVA: 0x000a3800
void GameHook_000a3800();
// samp.dll RVA: 0x000a3a70
void GameHook_000a3a70();
// samp.dll RVA: 0x000a3ac0
void GameHook_000a3ac0();
// samp.dll RVA: 0x000a3ad0
void GameHook_000a3ad0();
// samp.dll RVA: 0x000a3b00
void GameHook_000a3b00();
// samp.dll RVA: 0x000a3b20
void GameHook_000a3b20();
// samp.dll RVA: 0x000a3b90
void GameHook_000a3b90();
// samp.dll RVA: 0x000a3c60
void GameHook_000a3c60();
// samp.dll RVA: 0x000a3c80
void GameHook_000a3c80();
// samp.dll RVA: 0x000a3ca0
void GameHook_000a3ca0();
// samp.dll RVA: 0x000a3ce0
void GameHook_000a3ce0();
// samp.dll RVA: 0x000a3d20
void GameHook_000a3d20();
// samp.dll RVA: 0x000a3d90
void GameHook_000a3d90();
// samp.dll RVA: 0x000a3e50
void GameHook_000a3e50();
// Raw installed ABI: no arguments, returns the preview backing pointer in EAX.
// The void() declaration is an installation-address type, not a calling prototype.
// samp.dll RVA: 0x000a3ec0
void GameHook_000a3ec0();
// samp.dll RVA: 0x000a3f30
void GameHook_000a3f30();
// samp.dll RVA: 0x000a3fe0
void GameHook_000a3fe0();
// samp.dll RVA: 0x000a4190
void GameHook_000a4190();
// samp.dll RVA: 0x000a4280
void GameHook_000a4280();
// samp.dll RVA: 0x000a42c0
void GameHook_000a42c0();
// samp.dll RVA: 0x000a42d0
void GameHook_000a42d0();
// samp.dll RVA: 0x000a46b0
void GameHook_000a46b0();
// samp.dll RVA: 0x000a4770
void GameHook_000a4770();
// samp.dll RVA: 0x000a47d0
void GameHook_000a47d0();
// samp.dll RVA: 0x000a4850
void GameHook_000a4850();
// samp.dll RVA: 0x000a48c0
void GameHook_000a48c0();
// samp.dll RVA: 0x000a4960
void GameHook_000a4960();
// samp.dll RVA: 0x000a49d0
void GameHook_000a49d0();
// samp.dll RVA: 0x000a4c30
void GameHook_000a4c30();
// samp.dll RVA: 0x000a4df0
void GameHook_000a4df0();
// samp.dll RVA: 0x000a4e30
void GameHook_000a4e30();
// samp.dll RVA: 0x000a4e70
void GameHook_000a4e70();
// samp.dll RVA: 0x000a5100
void GameHook_000a5100();
// samp.dll RVA: 0x000a5150
void GameHook_000a5150();
// samp.dll RVA: 0x000a56b0
void GameHook_000a56b0();
// samp.dll RVA: 0x000a5820
void GameHook_000a5820();
// samp.dll RVA: 0x000a5850
void GameHook_000a5850();
// samp.dll RVA: 0x000a5870
void GameHook_000a5870();
// samp.dll RVA: 0x000a5910
void GameHook_000a5910();
// samp.dll RVA: 0x000a59b0
void GameHook_000a59b0();
// samp.dll RVA: 0x000a5a50
void GameHook_000a5a50();
// samp.dll RVA: 0x000a5af0
void GameHook_000a5af0();
// samp.dll RVA: 0x000a5b90
void GameHook_000a5b90();
// samp.dll RVA: 0x000a5d20
void GameHook_000a5d20();
// samp.dll RVA: 0x000a5d80
void GameInstallHook(DWORD address,DWORD function,DWORD storage,const BYTE *jumpCode,int size);
// samp.dll RVA: 0x000a5e30
void GamePrepareHooks();
// samp.dll RVA: 0x000a6020
void GameInstallHooks();
#endif
