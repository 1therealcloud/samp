#ifndef SAMP_RECOVERED_VEHICLEPOOL_H
#define SAMP_RECOVERED_VEHICLEPOOL_H
#include "game/vehicle.h"
#pragma pack(push,1)
// Observed prefix only; the remaining pool fields and complete extent are open.
class CVehiclePool {
public:
    int m_nLargestId;
    BYTE unknown04[0x1130];
    CVehicle *m_pObjects[2000];
    BOOL m_bNotEmpty[2000];
    void *m_pGameObjects[2000];
    // samp.dll RVA: 0x0001e650
    WORD Find(void *gameObject);
    // samp.dll RVA: 0x00001120
    CVehicle *Get(WORD id);
};
#pragma pack(pop)
typedef char VehiclePoolObjectOffset[(offsetof(CVehiclePool,m_pObjects)==0x1134)?1:-1];
typedef char VehiclePoolStateOffset[(offsetof(CVehiclePool,m_bNotEmpty)==0x3074)?1:-1];
typedef char VehiclePoolGtaOffset[(offsetof(CVehiclePool,m_pGameObjects)==0x4fb4)?1:-1];
#endif
