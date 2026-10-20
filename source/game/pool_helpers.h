#ifndef SAMP_GAME_POOL_HELPERS_H
#define SAMP_GAME_POOL_HELPERS_H
#include <windows.h>
// Opaque GTA backend pointers; these are not SA-MP pool/wrapper objects.
// samp.dll RVA: 0x000b37a0
int __stdcall GamePoolPedGetIndex(void *ped);
// samp.dll RVA: 0x000b3800
DWORD __stdcall GamePoolVehicleGetIndex(void *vehicle);
// samp.dll RVA: 0x000b3870
void *GamePoolBuildingGetArray();
// samp.dll RVA: 0x000b3890
void *GamePoolDummyGetArray();
// samp.dll RVA: 0x000b38b0
void *GamePoolObjectGetArray();
// samp.dll RVA: 0x000b3930
bool GamePoolHasObjectModel(int model);
#endif
