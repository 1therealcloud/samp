#ifndef SAMP_OBJECT_POOL_H
#define SAMP_OBJECT_POOL_H
#include "game/object.h"
#pragma pack(push,1)
class CObjectPool {
    int largestId;
    DWORD active[2100];
    CObject *objects[2100];
public:
    // Descriptive name: tests eligible pool objects, returns on first collision.
    // samp.dll RVA: 0x00012ce0
    bool TestCollisionShapes(const GameEntityCollisionShape *first,const GameEntityCollisionShape *second);
    // samp.dll RVA: 0x00012a90
    void Draw();
    // samp.dll RVA: 0x00012ad0
    void DrawLast();
    // Native Create has no ID bounds guard.
    // samp.dll RVA: 0x000127d0
    BOOL Create(WORD id,int model,CVector position,CVector rotation,float drawDistance);
    // samp.dll RVA: 0x00012700
    CObjectPool();
    // samp.dll RVA: 0x00013060
    ~CObjectPool();
    // samp.dll RVA: 0x000126a0
    void UpdateLargestId();
    // samp.dll RVA: 0x00012730
    int GetCount();
    // Native permits ID2100; adjacent allocation layout is not established.
    // samp.dll RVA: 0x00012750
    BOOL Delete(WORD id);
    // Starts at slot1, snapshots largest ID and ignores active flags.
    // samp.dll RVA: 0x000128d0
    CObject *Find(void *backend);
    // samp.dll RVA: 0x00012910
    WORD GetId(void *backend);
    // samp.dll RVA: 0x00002e10
    CEntity *Get(WORD id);
    // samp.dll RVA: 0x00012950
    void Process();
    // samp.dll RVA: 0x00012a10
    void ConstructMaterials();
    // samp.dll RVA: 0x00012a50
    void ShutdownMaterials();
};
#pragma pack(pop)
typedef char NativeObjectPoolExtent[(sizeof(CObjectPool)==0x41a4)?1:-1];
#endif
