#ifndef PATCHES_H
#define PATCHES_H

#include <windows.h>

void GameInstallVehicleDamageFilter();

void GameInstallShotHooks();

extern DWORD g_gameVehicleControlPatched;

extern DWORD g_gameManualVehicleControl;

void GameApplyManualVehicleControlPatches();

extern BYTE g_gamePatchStreamBytes[7];

extern DWORD g_gamePatchValue1546c8;

extern DWORD g_gamePatchValue1546cc;

extern float g_gamePatchValue1546f0;

extern DWORD g_gamePatchValue1546f4;

extern DWORD g_gameVersionAddressA;

extern DWORD g_gameVersionAddressB;

void GameApplyInGamePatches();

void GameInstallEventOpcodes();
// Native opcode callbacks; the driver path also constructs the local CPed lazily.

void GameSetEventOpcode();

void GamePlayerDriverOpcode();

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

static_assert(sizeof(GamePedModelRecord) == 68);
static_assert(sizeof(GameModelRecord32) == 32);

extern DWORD g_gameScanList[40000];

extern GamePedModelRecord g_gamePedModels[319];

extern GameModelRecord32 g_gameModelRecords[20000];

extern BYTE g_gameModelFlags[1024];

void GameRelocateScanList();

void GameRelocatePedModels();

void GameRelocateModelRecords();

void GameRelocateModelFlags();

void GameIncreasePoolLimits();

extern BYTE g_gamePoolPatchOne[7];

extern BYTE g_gamePoolPatchTwo[5];

extern DWORD g_gameModelReferenceSites[14];

extern DWORD g_gameScanDirectUSA[14];

extern DWORD g_gameScanDirectEU[14];

extern DWORD g_gameScanOperandUSA[56];

extern DWORD g_gameScanOperandEU[56];

extern DWORD g_gameScanSecondSites[11];

extern DWORD g_gamePedReferenceSites[4];

extern BYTE g_gameDefaultModelFlags[1024];

void _patch(DWORD address, BYTE* data, int byteCount);

// Legacy entry: patches the raw IPL allocation pointer at native GTA sites.

void SetIPLs(int iIPLs, int iGtaVersion);
// Native36-byte records; allocation, initialization and pointer/count patches.

void SetTimedObjects(int iTimedObjects);

// Mutable native operand address table; seven entries.

extern DWORD g_gameCount44OperandSites[7];

void GamePatchCount44(DWORD count);

void GameSetFontPatchBytes3();

void GameApplyDebugInitializationPatches();

#endif // PATCHES_H
