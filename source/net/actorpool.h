#ifndef SAMP_ACTOR_POOL_H
#define SAMP_ACTOR_POOL_H
#include <windows.h>
class CActor;
#pragma pack(push,1)
class CActorPool {
    int largestId;
    CActor *actors[1000];
    DWORD active[1000];
    void *peds[1000];
    // Native allocation extends beyond initialized lookup tables; tail meaning unresolved.
    BYTE unknown2ee4[0x1f40];
public:
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
