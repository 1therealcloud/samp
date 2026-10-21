#ifndef SAMP_GAME_HOOKS_CONTROL_H
#define SAMP_GAME_HOOKS_CONTROL_H
#include "game/hooks_render.h"
#include "exceptions.h"
#include "gui/textdraw_texture.h"
#include "game/playerped.h"
#include "game/aimstuff.h"
#include "game/keystuff.h"
// samp.dll RVA: 0x00151964
extern WORD * g_hookCameraModeWord;
// samp.dll RVA: 0x0018f4ac
extern BYTE g_hookSavedPlayer;
// samp.dll RVA: 0x0018f4b4
extern BYTE g_hookPlayerIndex;
// samp.dll RVA: 0x0018f4c0
extern DWORD g_hookPedControlThis;
// samp.dll RVA: 0x0018f51c
extern DWORD g_hookWeaponResult;
// samp.dll RVA: 0x0018f520
extern DWORD g_hookRestoreSkills;
// samp.dll RVA: 0x0018f524
extern DWORD g_hookLocalPedControl;
// samp.dll RVA: 0x0018f528
extern DWORD g_hookRestoreKeys;
// samp.dll RVA: 0x0018ea58
extern BYTE g_hookSavedCameraMode;
// samp.dll RVA: 0x0018e5c8
extern WORD g_hookSavedCameraModeWord;
// samp.dll RVA: 0x0018e810
extern DWORD g_hookVehicleProcess;
// samp.dll RVA: 0x0018e8d0
extern DWORD g_hookControlVehicle;
// samp.dll RVA: 0x0018ea54
extern DWORD g_hookVehicleVtable;
// samp.dll RVA: 0x0018e934
extern DWORD g_hookVehicleKeys;
// samp.dll RVA: 0x0018e938
extern BYTE g_hookVehicleStatus;
// samp.dll RVA: 0x0018eb20
extern DWORD g_hookVehicleLean;
// samp.dll RVA: 0x0018e8cc
extern DWORD g_hookVehicleLeanTarget;
// samp.dll RVA: 0x002a9a44
extern int g_gameVehicleModel;
// samp.dll RVA: 0x002a9a48
extern DWORD g_gameLinkedVehicle;
// samp.dll RVA: 0x002a9a4c
extern float g_gameVehicleDistanceSquared;
// samp.dll RVA: 0x002a9da0
extern DWORD g_gameLocalPedMatrix;
// samp.dll RVA: 0x002a9da4
extern DWORD g_gameVehicleMatrix;
// samp.dll RVA: 0x0011f5dc
extern const float g_gameStreamingDistanceSquared;
// samp.dll RVA: 0x000b3f60
BYTE __stdcall GameFindPlayerPedIndex(void *ped);
// samp.dll RVA: 0x000b4410
BOOL __stdcall GameIsTrainVehicle(void *vehicle);
// samp.dll RVA: 0x000b4460
BOOL __stdcall GameHasOccupiedLinkedVehicle(void *vehicle);
// samp.dll RVA: 0x000b6490
BOOL __stdcall GameIsVehicleInStreamingArea(void *vehicle);
#endif
