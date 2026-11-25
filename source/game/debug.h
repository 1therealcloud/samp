#ifndef SAMP_GAME_DEBUG_H
#define SAMP_GAME_DEBUG_H
#include <windows.h>
// Names follow the 0.2x task helper family; DL name lookup is disabled natively.
// The unused task parameter follows that reference; DL consumes no argument.

const char* GameGetTaskName(void* task);

extern const char g_taskNoneName[5];

DWORD GameGetTaskType(void* task);
// Validates the raw GTA vtable range before calling virtual slot +8.

void* GameGetTaskChild(void* task);
struct CVector;
struct KeyPad;
class CCamera;
class CVehicle;

void GameSetDebugState(DWORD first, DWORD second, DWORD stage);

void GameClearDebugState();

extern DWORD g_debugSelectedVehicleModel;

extern KeyPad* g_debugVehicleKeys;

extern CCamera* g_debugVehicleCamera;

extern DWORD g_debugVehicleSelectionActive;

extern CVehicle* g_debugSelectedVehicle;

extern DWORD g_debugSelectionFlagA;

extern DWORD g_debugSelectionFlagB;

extern const char g_debugVehicleSelectText[15];

void GameRebuildDebugVehicle();
// Historical public name; the body is the native vehicle selector.

void GameDrawDebugEntities();

DWORD __stdcall GameCountUnusedModelSlots();

extern BYTE g_nativeDebugFlag18DC20;

void __stdcall GameSetNativeDebugFlag18DC20(BYTE value);

float GameDebugGroundHeight(const CVector* position);

void GameDebugBindRasterResult();

#endif
