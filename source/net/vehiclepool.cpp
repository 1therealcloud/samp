#include "net/vehiclepool.h"
// samp.dll RVA: 0x0001e650
WORD CVehiclePool::Find(void *gameObject) {
    for(int i=1;i<=m_nLargestId;i++)if(m_pGameObjects[i]==gameObject)return (WORD)i;
    return 0xffff;
}
// samp.dll RVA: 0x00001120
CVehicle *CVehiclePool::Get(WORD id) { return id<2000 && m_bNotEmpty[id]?m_pObjects[id]:0; }

// Native returns the raw state DWORD; do not normalize it to bool.
// samp.dll RVA: 0x00001150
BOOL CVehiclePool::DoesExist(WORD id) { return id<2000?m_bNotEmpty[id]:FALSE; }

#include "game/object.h"
// samp.dll RVA: 0x0001e8e0
bool CVehiclePool::TestCollisionShapes(const GameEntityCollisionShape *first,const GameEntityCollisionShape *second) {
    for(int index=0;index<=m_nLargestId;index++) {
        if(!m_bNotEmpty[index])continue;
        CVehicle *vehicle=m_pObjects[index];
        if(!vehicle||!vehicle->DoesExist())continue;
        BYTE nearby;
        __asm {
            mov ecx,vehicle
            call CEntity::GetDistanceToCamera
            fcomp g_objectStreamingNearDistance
            fnstsw ax
            test ah,5
            setnp nearby
        }
        if(!nearby||vehicle->IsIgnored())continue;
        if(vehicle->TestCollisionShapes(first,second,1))return true;
    }
    return false;
}
