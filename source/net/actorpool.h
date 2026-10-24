#ifndef SAMP_ACTOR_POOL_H
#define SAMP_ACTOR_POOL_H
#include <windows.h>
#include "game/entity.h"
class CActor;
#pragma pack(push,1)
// Packed native creation request; custom model is separate from constructor model.
struct ActorCreateInfo { WORD id; int model,customModel; CVector position; float heading,health; BYTE invulnerable; };
typedef char NativeActorCreateInfoExtent[(sizeof(ActorCreateInfo)==31)?1:-1];
class CActorPool {
    int largestId;
    CActor *actors[1000];
    DWORD active[1000];
    void *peds[1000];
    // Observed per-slot DWORDs; semantic roles remain unresolved. Constructor leaves both untouched.
    DWORD state2ee4[1000];
    DWORD state3e84[1000];
public:
    // Verified entry; external SAMP-API1615 points inside its first comparison.
    // samp.dll RVA: 0x00001610
    CActor *Get(WORD id);
    // Raw active DWORD, not normalized to bool.
    // samp.dll RVA: 0x00001640
    BOOL DoesExist(WORD id);
    // Fourth parameter is copied into native ray records; semantic role unresolved.
    // samp.dll RVA: 0x00001770
    WORD FindInRay(float maxCameraDistance,const CVector *origin,const CVector *end,DWORD rayWord);
    // samp.dll RVA: 0x00001900
    BOOL Create(const ActorCreateInfo *info);
    // samp.dll RVA: 0x000016c0
    CActorPool();
    // samp.dll RVA: 0x000018e0
    ~CActorPool();
    // samp.dll RVA: 0x00001660
    void UpdateLargestId();
    // samp.dll RVA: 0x000016f0
    BOOL Delete(WORD id);
    // samp.dll RVA: 0x000018b0
    DWORD Find(void *ped);
};
#pragma pack(pop)
typedef char NativeActorPoolExtent[(sizeof(CActorPool)==0x4e24)?1:-1];
#endif
