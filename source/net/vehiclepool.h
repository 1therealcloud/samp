#ifndef SAMP_RECOVERED_VEHICLEPOOL_H
#define SAMP_RECOVERED_VEHICLEPOOL_H
#include "game/vehicle.h"
#pragma pack(push,1)
// Native queue record extent established; semantic fields await the spawn path.
struct VehiclePoolQueueEntry {DWORD words[10];};
// Allocation extent established; unreviewed tail fields remain opaque.
class CVehiclePool {
public:
    // samp.dll RVA: 0x0001e470
    void UpdateLargestId();
    // samp.dll RVA: 0x0001e4d0
    CVehiclePool();
    // samp.dll RVA: 0x0001eb20
    ~CVehiclePool();
    // samp.dll RVA: 0x0001e540
    BOOL Delete(WORD id);
    // Native unchecked WORD index.
    // samp.dll RVA: 0x0001e5c0
    void ChangeInterior(WORD id,int interior);
    // samp.dll RVA: 0x0001e5f0
    void AssignSpecialParams(WORD id,BYTE objective,BYTE locked);
    // samp.dll RVA: 0x0001e6c0
    WORD FindNearestToLocalPlayer();
    // samp.dll RVA: 0x0001e730
    WORD FindNearestToPoint(float x,float y,float z);
    // samp.dll RVA: 0x0001e7d0
    void Queue(const VehiclePoolQueueEntry *entry);
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
    VehiclePoolQueueEntry queue[100];
    DWORD queueActive[100];
    CVehicle *m_pObjects[2000];
    BOOL m_bNotEmpty[2000];
    void *m_pGameObjects[2000];
    BYTE unknown6ef4[0x2ee0];
    DWORD lastTick[2000];
    DWORD stateBD14[2000];
    BYTE unknownDC54[0x9c40];
    DWORD licensePlatesReady;
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
typedef char VehiclePoolQueueFlagsOffset[(offsetof(CVehiclePool,queueActive)==0xfa4)?1:-1];
typedef char VehiclePoolTickOffset[(offsetof(CVehiclePool,lastTick)==0x9dd4)?1:-1];
typedef char VehiclePoolStateBD14Offset[(offsetof(CVehiclePool,stateBD14)==0xbd14)?1:-1];
typedef char VehiclePoolPlateReadyOffset[(offsetof(CVehiclePool,licensePlatesReady)==0x17894)?1:-1];
typedef char VehiclePoolExtent[(sizeof(CVehiclePool)==0x17898)?1:-1];
// samp.dll RVA: 0x001184c4
extern const char g_vehiclePoolQueueFullMessage[];
#endif
