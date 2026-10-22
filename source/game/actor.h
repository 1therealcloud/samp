#ifndef SAMP_ACTOR_H
#define SAMP_ACTOR_H
#include "game/entity.h"
#pragma pack(push,1)
// Native allocation is 0x56 bytes; unresolved tail fields retain native untouched bytes.
class CActor : public CEntity {
    void *actorBackend;
    BYTE unknown4c[10];
public:
    // samp.dll RVA: 0x0009bd30
    CActor(int model,CVector position,float heading);
    // samp.dll RVA: 0x0009bee0
    void PerformAnimation(const char *name,const char *library,float speed,DWORD loop,DWORD lockX,DWORD lockY,DWORD freeze,int time);
    // samp.dll RVA: 0x0009bfd0
    void ClearTasks();
    // samp.dll RVA: 0x0009c1d0
    virtual ~CActor();
    // samp.dll RVA: 0x0009be80
    void Destroy();
    // samp.dll RVA: 0x0009bff0
    void SetRotation(float heading);
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
typedef char NativeActorExtent[(sizeof(CActor)==0x56)?1:-1];
// samp.dll RVA: 0x00181e5c
extern void *g_actorTaskBackend;
// samp.dll RVA: 0x0009bfb0
DWORD GameClearActorTasks();
#endif
