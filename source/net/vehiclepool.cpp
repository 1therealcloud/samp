#include "net/vehiclepool.h"
// samp.dll RVA: 0x0001e650
WORD CVehiclePool::Find(void *gameObject) {
    for(int i=1;i<=m_nLargestId;i++)if(m_pGameObjects[i]==gameObject)return (WORD)i;
    return 0xffff;
}
// samp.dll RVA: 0x00001120
CVehicle *CVehiclePool::Get(WORD id) { return id<2000 && m_bNotEmpty[id]?m_pObjects[id]:0; }
