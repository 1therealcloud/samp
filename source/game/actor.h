#ifndef ACTOR_H
#define ACTOR_H

#include "game/entity.h"

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
static_assert(offsetof(ActorBackendPrefix, flags) == 0x46c);
static_assert(offsetof(ActorBackendPrefix, tasks) == 0x47c);
static_assert(offsetof(ActorBackendPrefix, state530) == 0x530);
static_assert(offsetof(ActorBackendPrefix, health) == 0x540);
static_assert(offsetof(ActorBackendPrefix, armour) == 0x548);
static_assert(offsetof(ActorBackendPrefix, heading) == 0x55c);
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

    CActor(int model, float x, float y, float z, float heading);

    void PerformAnimation(const char* name, const char* library, float speed, DWORD loop, DWORD lockX, DWORD lockY, DWORD freeze, int time);

    void ClearTasks();

    virtual ~CActor();

    void Destroy();

    void SetRotation(float heading);

    float GetHealth();

    void SetHealth(float health);

    float GetArmour();

    void SetArmour(float armour);

    DWORD GetBackendFlags();

    void SetBackendFlags(DWORD flags);

    BOOL IsDead();

    BYTE GetPedState();

    void SetPedState(BYTE value);

    DWORD IsInVehicle();

    void SetInvulnerable(BYTE enabled);
};
#pragma pack(pop)
static_assert(sizeof(CActor) == 0x56);

extern void* g_actorTaskBackend;

DWORD GameClearActorTasks();

#endif // ACTOR_H
