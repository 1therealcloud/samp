#ifndef SAMP_GAME_TASK_EVENTS_H
#define SAMP_GAME_TASK_EVENTS_H
#include "game/hooks_control.h"
#include "game/entity.h"
#include "net/vehiclepool.h"
#include "net/localplayer.h"
// Native policy helpers remain explicit pool/network dependencies.
// samp.dll RVA: 0x000a3430
BOOL GameNotifyEnterVehicle();
// samp.dll RVA: 0x000a3910
BOOL __stdcall GameNotifyPedDamage(void *response,void *ped);
// samp.dll RVA: 0x0018f4a8
extern DWORD g_hookIgnoreNextExit;
// samp.dll RVA: 0x0018e550
extern DWORD g_hookExitTask;
// samp.dll RVA: 0x0018e5d4
extern CVehiclePool *g_hookEventVehiclePool;
// samp.dll RVA: 0x0018e568
extern WORD g_hookEventVehicleId;
// samp.dll RVA: 0x0018e868
extern CVehicle *g_hookExitVehicle;
// samp.dll RVA: 0x0018eb18
extern CLocalPlayer *g_hookEventLocalPlayer;
// samp.dll RVA: 0x0018eb30
extern CMatrix g_hookExitMatrix;
// samp.dll RVA: 0x00151aa0
extern DWORD g_hookExitTaskArgument;
// samp.dll RVA: 0x0011c65c
extern const float g_hookTrainExitOffset;
// samp.dll RVA: 0x0018e5bc
extern DWORD g_hookQuitDeadline;
// samp.dll RVA: 0x0018f538
extern DWORD g_hookDamageResponse;
// samp.dll RVA: 0x0018f53c
extern DWORD g_hookDamagePed;
#endif
