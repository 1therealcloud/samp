#ifndef SAMP_ACTOR_POOL_H
#define SAMP_ACTOR_POOL_H
#include "game/actor.h"
#include <windows.h>
#pragma pack(push, 1)
// Packed native creation request; custom model is separate from constructor model.
struct ActorCreateInfo
{
    WORD id;
    int model, customModel;
    CVector position;
    float heading, health;
    BYTE invulnerable;
};
typedef char NativeActorCreateInfoExtent[(sizeof(ActorCreateInfo) == 31) ? 1 : -1];
class CActorPool
{
    typedef char ActorBackendOffset[(offsetof(CActor, actorBackend) == 0x48) ? 1 : -1];
    typedef char ActorInvulnerableOffset[(offsetof(CActor, invulnerable) == 0x55) ? 1 : -1];
    int largestId;
    CActor* actors[1000];
    DWORD active[1000];
    void* peds[1000];
    // Observed per-slot DWORDs; semantic roles remain unresolved. Constructor leaves both untouched.
    DWORD state2ee4[1000];
    DWORD state3e84[1000];

public:
    // Verified entry; external SAMP-API1615 points inside its first comparison.
    // 0x00001610
    CActor* Get(WORD id);
    // Raw active DWORD, not normalized to bool.
    // 0x00001640
    BOOL DoesExist(WORD id);
    // Fourth float is copied raw into native ray records; semantic role unresolved.
    // 0x00001770
    WORD FindInRay(float maxCameraDistance, const CVector* origin, const CVector* end, float rayValue);
    // 0x00001900
    BOOL Create(const ActorCreateInfo* info);
    // 0x000016c0
    CActorPool();
    // 0x000018e0
    ~CActorPool();
    // 0x00001660
    void UpdateLargestId();
    // 0x000016f0
    BOOL Delete(WORD id);
    // ID in AX; native failure preserves the high half of the EAX counter.
    // 0x000018b0
    WORD Find(void* ped);
};
#pragma pack(pop)
typedef char NativeActorPoolExtent[(sizeof(CActorPool) == 0x4e24) ? 1 : -1];
#endif
