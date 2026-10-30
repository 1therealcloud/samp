#ifndef SAMP_RECOVERED_GAME_PATCHES_H
#define SAMP_RECOVERED_GAME_PATCHES_H
#include <windows.h>
// 0x000a6f00
void GameInstallVehicleDamageFilter();
// 0x000a6d50
void GameInstallShotHooks();
// 0x002a9278
extern DWORD g_gameVehicleControlPatched;
// 0x002a927c
extern DWORD g_gameManualVehicleControl;
// Descriptive name: native settings +0x2B selects this patch set.
// 0x000aa800
void GameApplyManualVehicleControlPatches();
// 0x001546c0
extern BYTE g_gamePatchStreamBytes[7];
// 0x001546c8
extern DWORD g_gamePatchValue1546c8;
// 0x001546cc
extern DWORD g_gamePatchValue1546cc;
// 0x001546f0
extern DWORD g_gamePatchValue1546f0;
// 0x001546f4
extern DWORD g_gamePatchValue1546f4;
// 0x0018f498
extern DWORD g_gameVersionAddressA;
// 0x0018f49c
extern DWORD g_gameVersionAddressB;
// 0x000aaad0
void GameApplyInGamePatches();
// 0x00002130
void GameInstallEventOpcodes();
// Native opcode callbacks; the driver path also constructs the local CPed lazily.
// 0x000020c0
void GameSetEventOpcode();
// 0x00001c60
void GamePlayerDriverOpcode();

#pragma pack(push, 1)
struct GamePedModelRecord
{
    DWORD vtable;
    BYTE data[64];
};
struct GameModelRecord32
{
    DWORD vtable;
    BYTE data[28];
};
#pragma pack(pop)
// 0x001e04b0
extern DWORD g_gameScanList[40000];
// 0x002075b0
extern GamePedModelRecord g_gamePedModels[319];
// 0x0020ca70
extern GameModelRecord32 g_gameModelRecords[20000];
// 0x002a8e70
extern BYTE g_gameModelFlags[1024];
// 0x000aa6a0
void GameRelocateScanList();
// 0x000aa5e0
void GameRelocatePedModels();
// 0x000aa630
void GameRelocateModelRecords();
// 0x000aa980
void GameRelocateModelFlags();
// 0x000aa180
void GameIncreasePoolLimits();
// 0x001546b0
extern BYTE g_gamePoolPatchOne[7];
// 0x001546b8
extern BYTE g_gamePoolPatchTwo[5];
// 0x001546f8
extern DWORD g_gameModelReferenceSites[14];
// 0x00154730
extern DWORD g_gameScanDirectUSA[14];
// 0x00154768
extern DWORD g_gameScanDirectEU[14];
// 0x001547a0
extern DWORD g_gameScanOperandUSA[56];
// 0x00154880
extern DWORD g_gameScanOperandEU[56];
// 0x00154960
extern DWORD g_gameScanSecondSites[11];
// 0x0015498c
extern DWORD g_gamePedReferenceSites[4];
// 0x001549a0
extern BYTE g_gameDefaultModelFlags[1024];

// 0x000a9d10
void _patch(DWORD address, BYTE* data, int byteCount);

// Legacy entry: patches the raw IPL allocation pointer at native GTA sites.
// 0x000a9d60
void SetIPLs(int iIPLs, int iGtaVersion);
// Native36-byte records; allocation, initialization and pointer/count patches.
// 0x000a9e70
void SetTimedObjects(int iTimedObjects);

// Mutable native operand address table; seven entries.
// 0x001546d0
extern DWORD g_gameCount44OperandSites[7];
// 0x000aa0d0
void GamePatchCount44(DWORD count);
// 0x000aa130
void GameSetFontPatchBytes3();
// 0x000aaa50
void GameApplyDebugInitializationPatches();
#endif
