#ifndef SAMP_ACTOR_H
#define SAMP_ACTOR_H
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
typedef char ActorBackendFlagsOffset[(offsetof(ActorBackendPrefix, flags) == 0x46c) ? 1 : -1];
typedef char ActorBackendTasksOffset[(offsetof(ActorBackendPrefix, tasks) == 0x47c) ? 1 : -1];
typedef char ActorBackendStateOffset[(offsetof(ActorBackendPrefix, state530) == 0x530) ? 1 : -1];
typedef char ActorBackendHealthOffset[(offsetof(ActorBackendPrefix, health) == 0x540) ? 1 : -1];
typedef char ActorBackendArmourOffset[(offsetof(ActorBackendPrefix, armour) == 0x548) ? 1 : -1];
typedef char ActorBackendHeadingOffset[(offsetof(ActorBackendPrefix, heading) == 0x55c) ? 1 : -1];
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
    // 0x0009bd30
    CActor(int model, float x, float y, float z, float heading);
    // 0x0009bee0
    void PerformAnimation(const char* name, const char* library, float speed, DWORD loop, DWORD lockX, DWORD lockY, DWORD freeze, int time);
    // 0x0009bfd0
    void ClearTasks();
    // 0x0009c1d0
    virtual ~CActor();
    // 0x0009be80
    void Destroy();
    // 0x0009bff0
    void SetRotation(float heading);
    // 0x0009c030
    float GetHealth();
    // 0x0009c050
    void SetHealth(float health);
    // 0x0009c090
    float GetArmour();
    // 0x0009c0b0
    void SetArmour(float armour);
    // 0x0009c0d0
    DWORD GetBackendFlags();
    // 0x0009c0e0
    void SetBackendFlags(DWORD flags);
    // 0x0009c100
    BOOL IsDead();
    // 0x0009c130
    BYTE GetBackendByte530();
    // 0x0009c140
    void SetBackendByte530(BYTE value);
    // 0x0009c160
    DWORD HasBackendFlag100();
    // 0x0009c180
    void SetInvulnerable(BYTE enabled);
};
#pragma pack(pop)
typedef char NativeActorExtent[(sizeof(CActor) == 0x56) ? 1 : -1];
// 0x00181e5c
extern void* g_actorTaskBackend;
// 0x0009bfb0
DWORD GameClearActorTasks();
#endif
