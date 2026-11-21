#ifndef PATCHES_H
#define PATCHES_H

#include <windows.h>
// 0x000a71c0
void GameInstallVehicleDamageFilter();
// 0x000a7010
void GameInstallShotHooks();
// R5 address to verify
extern DWORD g_gameVehicleControlPatched;
// R5 address to verify
extern DWORD g_gameManualVehicleControl;
// 0x000aabe0
void GameApplyManualVehicleControlPatches();
// 0x001166e8
extern BYTE g_gamePatchStreamBytes[7];
// 0x001166f0
extern DWORD g_gamePatchValue1546c8;
// 0x001166f4
extern DWORD g_gamePatchValue1546cc;
// 0x00116718
extern float g_gamePatchValue1546f0;
// 0x0011671c
extern DWORD g_gamePatchValue1546f4;
// R5 address to verify
extern DWORD g_gameVersionAddressA;
// R5 address to verify
extern DWORD g_gameVersionAddressB;
// 0x000aaeb0
void GameApplyInGamePatches();
// 0x00002100
void GameInstallEventOpcodes();
// Native opcode callbacks; the driver path also constructs the local CPed lazily.
// R5 address to verify
void GameSetEventOpcode();
// R5 address to verify
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
// R5 address to verify
extern DWORD g_gameScanList[40000];
// 0x001c9718
extern GamePedModelRecord g_gamePedModels[319];
// 0x001cebd8
extern GameModelRecord32 g_gameModelRecords[20000];
// R5 address to verify
extern BYTE g_gameModelFlags[1024];
// 0x000aaa80
void GameRelocateScanList();
// 0x000aa9c0
void GameRelocatePedModels();
// 0x000aaa10
void GameRelocateModelRecords();
// 0x000aad60
void GameRelocateModelFlags();
// 0x000aa590
void GameIncreasePoolLimits();
// R5 address to verify
extern BYTE g_gamePoolPatchOne[7];
// R5 address to verify
extern BYTE g_gamePoolPatchTwo[5];
// 0x00116720
extern DWORD g_gameModelReferenceSites[14];
// 0x00116758
extern DWORD g_gameScanDirectUSA[14];
// 0x00116790
extern DWORD g_gameScanDirectEU[14];
// 0x001167c8
extern DWORD g_gameScanOperandUSA[56];
// 0x001168a8
extern DWORD g_gameScanOperandEU[56];
// 0x00116988
extern DWORD g_gameScanSecondSites[11];
// 0x001169b4
extern DWORD g_gamePedReferenceSites[4];
// 0x001169c8
extern BYTE g_gameDefaultModelFlags[1024];

// 0x000aa120
void _patch(DWORD address, BYTE* data, int byteCount);

// Legacy entry: patches the raw IPL allocation pointer at native GTA sites.
// 0x000aa170
void SetIPLs(int iIPLs, int iGtaVersion);
// Native36-byte records; allocation, initialization and pointer/count patches.
// 0x000aa280
void SetTimedObjects(int iTimedObjects);

// Mutable native operand address table; seven entries.
// R5 address to verify
extern DWORD g_gameCount44OperandSites[7];
// R5 address to verify
void GamePatchCount44(DWORD count);
// R5 address to verify
void GameSetFontPatchBytes3();
// R5 address to verify
void GameApplyDebugInitializationPatches();

#endif // PATCHES_H
