#ifndef SAMP_GAME_DEBUG_H
#define SAMP_GAME_DEBUG_H
#include <windows.h>
struct KeyPad;
class CCamera;
class CVehicle;
// Descriptive recovery names for the native debug-state setters.
// samp.dll RVA: 0x0009dee0
void GameSetDebugState(DWORD first,DWORD second,DWORD stage);
// samp.dll RVA: 0x0009df00
void GameClearDebugState();
// samp.dll RVA: 0x001518d4
extern DWORD g_debugSelectedVehicleModel;
// samp.dll RVA: 0x0018dd48
extern KeyPad *g_debugVehicleKeys;
// samp.dll RVA: 0x0018dd4c
extern CCamera *g_debugVehicleCamera;
// samp.dll RVA: 0x0018de30
extern DWORD g_debugVehicleSelectionActive;
// samp.dll RVA: 0x0018de34
extern CVehicle *g_debugSelectedVehicle;
// samp.dll RVA: 0x0018ddec
extern DWORD g_debugSelectionFlagA;
// samp.dll RVA: 0x0018ddf0
extern DWORD g_debugSelectionFlagB;
// samp.dll RVA: 0x0011e530
extern const char g_debugVehicleSelectText[15];
// samp.dll RVA: 0x0011c198
extern const char g_debugVehicleTypeFormat[8];
// samp.dll RVA: 0x0009df10
void GameRebuildDebugVehicle();
// Historical public name; the body is the native vehicle selector.
// samp.dll RVA: 0x0009df50
void GameDrawDebugEntities();
// samp.dll RVA: 0x000b3a00
DWORD __stdcall GameCountUnusedModelSlots();
#endif
