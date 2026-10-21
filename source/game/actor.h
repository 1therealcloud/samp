#ifndef SAMP_ACTOR_H
#define SAMP_ACTOR_H
#include "game/entity.h"
#pragma pack(push,1)
// Observed prefix through byte55; complete allocation/lifecycle not established.
class CActor : public CEntity {
    void *actorBackend;
    BYTE unknown4c[10];
public:
    // samp.dll RVA: 0x0009c030
    float GetHealth();
    // samp.dll RVA: 0x0009c050
    void SetHealth(float health);
    // samp.dll RVA: 0x0009c090
    float GetArmour();
    // samp.dll RVA: 0x0009c0b0
    void SetArmour(float armour);
    // samp.dll RVA: 0x0009c0d0
    DWORD GetBackendFlags();
    // samp.dll RVA: 0x0009c0e0
    void SetBackendFlags(DWORD flags);
    // samp.dll RVA: 0x0009c100
    BOOL IsDead();
    // samp.dll RVA: 0x0009c130
    BYTE GetBackendByte530();
    // samp.dll RVA: 0x0009c140
    void SetBackendByte530(BYTE value);
    // samp.dll RVA: 0x0009c160
    DWORD HasBackendFlag100();
    // samp.dll RVA: 0x0009c180
    void SetInvulnerable(BYTE enabled);
};
#pragma pack(pop)
#endif
