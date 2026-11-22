#ifndef SAMP_GAME_DEBUG_H
#define SAMP_GAME_DEBUG_H
#include <windows.h>
// Names follow the 0.2x task helper family; DL name lookup is disabled natively.
// The unused task parameter follows that reference; DL consumes no argument.
// 0x000b3cf0
const char* GameGetTaskName(void* task);
// 0x0011f56c
extern const char g_taskNoneName[5];
// 0x000b3ca0
DWORD GameGetTaskType(void* task);
// Validates the raw GTA vtable range before calling virtual slot +8.
// 0x000b3c50
void* GameGetTaskChild(void* task);
struct CVector;
struct KeyPad;
class CCamera;
class CVehicle;
// 0x0009dee0
void GameSetDebugState(DWORD first, DWORD second, DWORD stage);
// 0x0009df00
void GameClearDebugState();
// 0x001518d4
extern DWORD g_debugSelectedVehicleModel;
// 0x0018dd48
extern KeyPad* g_debugVehicleKeys;
// 0x0018dd4c
extern CCamera* g_debugVehicleCamera;
// 0x0018de30
extern DWORD g_debugVehicleSelectionActive;
// 0x0018de34
extern CVehicle* g_debugSelectedVehicle;
// 0x0018ddec
extern DWORD g_debugSelectionFlagA;
// 0x0018ddf0
extern DWORD g_debugSelectionFlagB;
// 0x0011e530
extern const char g_debugVehicleSelectText[15];
// 0x0009df10
void GameRebuildDebugVehicle();
// Historical public name; the body is the native vehicle selector.
// 0x0009df50
void GameDrawDebugEntities();
// 0x000b3a00
DWORD __stdcall GameCountUnusedModelSlots();
// 0x0018dc20
extern BYTE g_nativeDebugFlag18DC20;
// 0x0009d3a0
void __stdcall GameSetNativeDebugFlag18DC20(BYTE value);
// 0x0009dc80
float GameDebugGroundHeight(const CVector* position);
// 0x000b5610
void GameDebugBindRasterResult();

#endif
