#ifndef SAMP_RECOVERED_GAME_PATCHES_H
#define SAMP_RECOVERED_GAME_PATCHES_H
#include <windows.h>
// samp.dll RVA: 0x001546c0
extern BYTE g_gamePatchStreamBytes[7];
// samp.dll RVA: 0x001546c8
extern DWORD g_gamePatchValue1546c8;
// samp.dll RVA: 0x001546cc
extern DWORD g_gamePatchValue1546cc;
// samp.dll RVA: 0x001546f0
extern DWORD g_gamePatchValue1546f0;
// samp.dll RVA: 0x001546f4
extern DWORD g_gamePatchValue1546f4;
// samp.dll RVA: 0x0018f498
extern DWORD g_gameVersionAddressA;
// samp.dll RVA: 0x0018f49c
extern DWORD g_gameVersionAddressB;
// samp.dll RVA: 0x000aaad0
void GameApplyInGamePatches();
// samp.dll RVA: 0x00002130
void GameInstallEventOpcodes();
// Native opcode callbacks; the driver path also constructs the local CPed lazily.
// samp.dll RVA: 0x000020c0
void GameSetEventOpcode();
// samp.dll RVA: 0x00001c60
void GamePlayerDriverOpcode();
#endif
