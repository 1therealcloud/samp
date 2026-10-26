#ifndef SAMP_RECOVERED_GAME_PATCHES_H
#define SAMP_RECOVERED_GAME_PATCHES_H
#include <windows.h>
// samp.dll RVA: 0x000a6f00
void GameInstallVehicleDamageFilter();
// samp.dll RVA: 0x000a6d50
void GameInstallShotHooks();
// samp.dll RVA: 0x002a9278
extern DWORD g_gameVehicleControlPatched;
// samp.dll RVA: 0x002a927c
extern DWORD g_gameManualVehicleControl;
// Descriptive name: native settings +0x2B selects this patch set.
// samp.dll RVA: 0x000aa800
void GameApplyManualVehicleControlPatches();
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

#ifndef SAMP_RECOVERED_GAME_LIMITS_H
#define SAMP_RECOVERED_GAME_LIMITS_H
#include "game/limit_data.h"
#pragma pack(push,1)
struct GamePedModelRecord { DWORD vtable;BYTE data[64]; };
struct GameModelRecord32 { DWORD vtable;BYTE data[28]; };
#pragma pack(pop)
// samp.dll RVA: 0x001e04b0
extern DWORD g_gameScanList[40000];
// samp.dll RVA: 0x002075b0
extern GamePedModelRecord g_gamePedModels[319];
// samp.dll RVA: 0x0020ca70
extern GameModelRecord32 g_gameModelRecords[20000];
// samp.dll RVA: 0x002a8e70
extern BYTE g_gameModelFlags[1024];
// samp.dll RVA: 0x000aa6a0
void GameRelocateScanList();
// samp.dll RVA: 0x000aa5e0
void GameRelocatePedModels();
// samp.dll RVA: 0x000aa630
void GameRelocateModelRecords();
// samp.dll RVA: 0x000aa980
void GameRelocateModelFlags();
// samp.dll RVA: 0x000aa180
void GameIncreasePoolLimits();
#endif

// samp.dll RVA: 0x000a9d10
void _patch(DWORD address,BYTE *data,int byteCount);
