#ifndef SAMP_RECOVERED_VEHICLEPOOL_H
#define SAMP_RECOVERED_VEHICLEPOOL_H
#include "game/vehicle.h"
#pragma pack(push,1)
// Observed prefix only; the remaining pool fields and complete extent are open.
class CVehiclePool {
public:
    // Native ID overload has no bounds check.
    // samp.dll RVA: 0x0001e680
    DWORD GetRef(int id);
    // No this data is consumed by the backend overload.
    // samp.dll RVA: 0x0001e6a0
    DWORD GetRef(void *backend);
    // Native unbounded live-largest-ID scan; returns bool in AL.
    // samp.dll RVA: 0x0001e8e0
    bool TestCollisionShapes(const GameEntityCollisionShape *first,const GameEntityCollisionShape *second);
    int m_nLargestId;
    BYTE unknown04[0x1130];
    CVehicle *m_pObjects[2000];
    BOOL m_bNotEmpty[2000];
    void *m_pGameObjects[2000];
    // samp.dll RVA: 0x00001150
    BOOL DoesExist(WORD id);
    // samp.dll RVA: 0x0001e650
    WORD Find(void *gameObject);
    // samp.dll RVA: 0x00001120
    CVehicle *Get(WORD id);
    // Native unbounded largest-ID loop; complete pool extent remains open.
    // samp.dll RVA: 0x0001e8a0
    void ShutdownLicensePlates();
    // Native readiness gate and largest-ID loop.
    // samp.dll RVA: 0x0001e830
    void ConstructLicensePlates();
};
#pragma pack(pop)
typedef char VehiclePoolObjectOffset[(offsetof(CVehiclePool,m_pObjects)==0x1134)?1:-1];
typedef char VehiclePoolStateOffset[(offsetof(CVehiclePool,m_bNotEmpty)==0x3074)?1:-1];
typedef char VehiclePoolGtaOffset[(offsetof(CVehiclePool,m_pGameObjects)==0x4fb4)?1:-1];
#endif
