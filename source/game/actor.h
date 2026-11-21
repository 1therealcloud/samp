#ifndef ACTOR_H
#define ACTOR_H

#include "game/entity.h"

#include <static_assert.h>

#pragma pack(push, 1)
// Observed GTA SA ped-backend prefix, not the complete GTA allocation/type.
// Field names describe only the accesses used by this SA-MP wrapper.
struct ActorBackendPrefix
{
    DWORD* vtable;
    BYTE unknown04[0x468];
    DWORD flags;
    BYTE unknown470[0x0c];
    void* tasks;
    BYTE unknown480[0xb0];
    DWORD state530;
    BYTE unknown534[0x0c];
    float health;
    BYTE unknown544[4];
    float armour;
    BYTE unknown54c[0x10];
    float heading;
};
STATIC_ASSERT(offsetof(ActorBackendPrefix, flags) == 0x46c);
STATIC_ASSERT(offsetof(ActorBackendPrefix, tasks) == 0x47c);
STATIC_ASSERT(offsetof(ActorBackendPrefix, state530) == 0x530);
STATIC_ASSERT(offsetof(ActorBackendPrefix, health) == 0x540);
STATIC_ASSERT(offsetof(ActorBackendPrefix, armour) == 0x548);
STATIC_ASSERT(offsetof(ActorBackendPrefix, heading) == 0x55c);
// Native SA-MP allocation is0x56; bytes4C..54 remain untouched/unresolved.
// samp.dll vtable RVA: 0x0011e318
class CActor : public CEntity
{
    friend class CActorPool;
    ActorBackendPrefix* actorBackend;
    BYTE unknown4c[9];
    BYTE invulnerable;

public:
    // Five native stack DWORDs; scalar coordinates preserve the 20-byte ABI.
    // 0x0009c2b0
    CActor(int model, float x, float y, float z, float heading);
    // 0x0009c460
    void PerformAnimation(const char* name, const char* library, float speed, DWORD loop, DWORD lockX, DWORD lockY, DWORD freeze, int time);
    // R5 address to verify
    void ClearTasks();
    // R5 address to verify
    virtual ~CActor();
    // 0x0009c400
    void Destroy();
    // 0x0009c570
    void SetRotation(float heading);
    // 0x0009c5b0
    float GetHealth();
    // 0x0009c5d0
    void SetHealth(float health);
    // R5 address to verify
    float GetArmour();
    // R5 address to verify
    void SetArmour(float armour);
    // R5 address to verify
    DWORD GetBackendFlags();
    // R5 address to verify
    void SetBackendFlags(DWORD flags);
    // R5 address to verify
    BOOL IsDead();
    // R5 address to verify
    BYTE GetBackendByte530();
    // R5 address to verify
    void SetBackendByte530(BYTE value);
    // R5 address to verify
    DWORD HasBackendFlag100();
    // 0x0009c700
    void SetInvulnerable(BYTE enabled);
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CActor) == 0x56);
// R5 address to verify
extern void* g_actorTaskBackend;
// 0x0009c530
DWORD GameClearActorTasks();

#endif // ACTOR_H
