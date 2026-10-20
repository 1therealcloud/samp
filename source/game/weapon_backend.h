#ifndef SAMP_WEAPON_BACKEND_H
#define SAMP_WEAPON_BACKEND_H
#include "game/entity.h"
// Opaque GTA backend objects, not maintained CEntity/CPed wrapper instances.
// Flag arguments retain full native DWORD words while the backend consumes bools.
// samp.dll RVA: 0x000b6020
bool __stdcall GameWeaponFireInstantHit(void *weapon,void *shooter,const CVector *origin,
    const CVector *muzzle,void *victim,const CVector *target,const CVector *driveBy,
    DWORD crossHair,DWORD effects);
// samp.dll RVA: 0x000b6050
bool __stdcall GameWeaponFireSniper(void *weapon,void *shooter);
#endif
