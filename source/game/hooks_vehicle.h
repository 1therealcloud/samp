#ifndef SAMP_VEHICLE_HOOK_CALLBACKS_H
#define SAMP_VEHICLE_HOOK_CALLBACKS_H
#include "game/hooks.h"
// samp.dll RVA: 0x0018f690
extern DWORD g_hookContactThis;
// samp.dll RVA: 0x0018f694
extern DWORD g_hookContactOther;
// samp.dll RVA: 0x0018f698
extern DWORD g_hookContactReturn;
// samp.dll RVA: 0x0018f69c
extern DWORD g_hookSirenVehicle;
// samp.dll RVA: 0x0018f6a0
extern BYTE g_hookSirenResult;
// samp.dll RVA: 0x0018f6a4
extern DWORD g_hookSirenOnce;
// samp.dll RVA: 0x00151b0c
extern WORD g_hookSirenVehicleId;
// samp.dll RVA: 0x0018f63c
extern DWORD g_hookPedQueryThis;
// samp.dll RVA: 0x0018f640
extern DWORD g_hookPedQueryPed;
#endif
