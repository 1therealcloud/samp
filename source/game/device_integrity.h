#ifndef SAMP_DEVICE_INTEGRITY_H
#define SAMP_DEVICE_INTEGRITY_H
#include <windows.h>
// samp.dll RVA: 0x0018dc24
extern DWORD g_integrityVehicleChecksum;
// samp.dll RVA: 0x0018dc28
extern DWORD g_integrityPedChecksum;
// samp.dll RVA: 0x0018dc2c
extern DWORD g_integrityComparisons;
// samp.dll RVA: 0x0018dc30
extern DWORD g_integrityPending;
// samp.dll RVA: 0x0018dc34
extern BYTE g_integritySuppressCount;
// samp.dll RVA: 0x0009d690
DWORD *GameIntegrityVehicleArray();
// samp.dll RVA: 0x0009d6b0
DWORD *GameIntegrityPedArray();
// samp.dll RVA: 0x0009d6d0
DWORD GameIntegrityXor(const DWORD *data,DWORD count);
// Seven DWORD words are supplied by reviewed callers; native body ignores them.
// samp.dll RVA: 0x000c5c10
DWORD __stdcall GameIntegrityMismatch(DWORD code,DWORD a,DWORD b,DWORD c,DWORD d,DWORD e,DWORD f);
// samp.dll RVA: 0x0009d6f0
void GameDeviceIntegrityCycle();
#endif
