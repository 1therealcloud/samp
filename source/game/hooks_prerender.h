#ifndef SAMP_PRERENDER_HOOK_H
#define SAMP_PRERENDER_HOOK_H
#include "game/hooks.h"
class CObjectPool;
// samp.dll RVA: 0x0018de44
extern CObjectPool* g_debugObjectPool;
class CVehicle;
class CVehiclePool;
class CObject;
struct CVector;
// samp.dll RVA: 0x0018e7f8
extern DWORD g_hookPickupBackend;
// samp.dll RVA: 0x0018e5cc
extern DWORD g_hookMenuTextResult;
// samp.dll RVA: 0x0018f5a8
extern CVector g_hookPassengerCameraPosition;
// samp.dll RVA: 0x0018e580
extern DWORD g_hookTrailerBackend;
// samp.dll RVA: 0x0018e7fc
extern CVehiclePool* g_hookTrailerPool;
// samp.dll RVA: 0x0018e7f0
extern WORD g_hookTrailerId;
// samp.dll RVA: 0x0018f5e0
extern CVehicle* g_hookTrailerVehicle;
// samp.dll RVA: 0x0018f600
extern DWORD g_hookObjectSavedStack;
// samp.dll RVA: 0x0018f5f8
extern DWORD g_hookObjectBackend;
// samp.dll RVA: 0x0018f604
extern DWORD g_hookObjectResult;
// samp.dll RVA: 0x0018f5fc
extern CObject* g_hookObjectRotationWrapper;
// samp.dll RVA: 0x0018f614
extern CObjectPool* g_hookObjectDrawPool;
// samp.dll RVA: 0x0018f618
extern DWORD g_hookObjectDrawBackend;
// samp.dll RVA: 0x0018f578
extern CObject* g_hookObjectDrawWrapper;
class CPlayerPool;
class CRemotePlayer;
struct GameEntityCollisionShape;
// samp.dll RVA: 0x0018f580
extern DWORD g_hookMaterialReturn;
// samp.dll RVA: 0x0018f574
extern DWORD g_hookMaterialBackend;
// samp.dll RVA: 0x0018f57c
extern CPlayerPool* g_hookMaterialPlayerPool;
// samp.dll RVA: 0x0018f5a4
extern DWORD g_hookSurfingIndex;
// samp.dll RVA: 0x0018e7f4
extern CRemotePlayer* g_hookSurfingRemote;
// samp.dll RVA: 0x0018eac0
extern GameEntityCollisionShape g_hookCollisionFirst;
// samp.dll RVA: 0x0018e56c
extern GameEntityCollisionShape g_hookCollisionSecond;
// samp.dll RVA: 0x0018f68c
extern BYTE g_hookCollisionResult;
// samp.dll RVA: 0x0011e508
extern const float g_hookCollisionBoundHigh;
// samp.dll RVA: 0x0011ec3c
extern const float g_hookCollisionBoundLow;
#endif
