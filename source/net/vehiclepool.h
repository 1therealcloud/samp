#ifndef SAMP_RECOVERED_VEHICLEPOOL_H
#define SAMP_RECOVERED_VEHICLEPOOL_H
#include "game/vehicle.h"
#pragma pack(push, 1)
// Native40-byte record; raw words retained for existing callers.
struct VehiclePoolQueueEntry
{
    union
    {
        DWORD words[10];
        struct
        {
            WORD id;
            int model;
            CVector position;
            float rotation;
            signed char primaryColor, secondaryColor;
            float health;
            BYTE interior;
            DWORD doors, panels;
            BYTE lights, tires, siren;
        };
    };
};
// Allocation extent established; unreviewed tail fields remain opaque.
class CVehiclePool
{
public:
    // Unchecked WORD index into the native unoccupied-sync owner table.
    // 0x00002df0
    void SetUnoccupiedSyncOwner(WORD vehicle, WORD player);
    // First native stack word unused; endpoints are the second and third words.
    // 0x0001e970
    WORD FindInLine(DWORD unused, const CVector* origin, const CVector* end);

    // Fourth raw word is copied into both native ray records.
    // 0x0001e9f0
    WORD FindInRay(float maxCameraDistance, const CVector* origin, const CVector* end, DWORD rayWord);
    // 0x0001ee80
    void Process();
    // Native Create indexes the WORD ID without a bounds guard.
    // 0x0001eb40
    BOOL Create(const VehiclePoolQueueEntry* entry);
    // Mutates queued train IDs/models in place and clears active even on failure.
    // 0x0001edc0
    void ProcessWaitingList();
    // Does not consume this; original stack argument is a WORD vehicle ID.
    // 0x0001ecf0
    void SendDestroyNotification(WORD id);
    // 0x0001e470
    void UpdateLargestId();
    // 0x0001e4d0
    CVehiclePool();
    // 0x0001eb20
    ~CVehiclePool();
    // 0x0001e540
    BOOL Delete(WORD id);
    // Native unchecked WORD index.
    // 0x0001e5c0
    void ChangeInterior(WORD id, int interior);
    // 0x0001e5f0
    void AssignSpecialParams(WORD id, BYTE objective, BYTE locked);
    // 0x0001e6c0
    WORD FindNearestToLocalPlayer();
    // 0x0001e730
    WORD FindNearestToPoint(float x, float y, float z);
    // 0x0001e7d0
    void Queue(const VehiclePoolQueueEntry* entry);
    // Native ID overload has no bounds check.
    // 0x0001e680
    DWORD GetRef(int id);
    // No this data is consumed by the backend overload.
    // 0x0001e6a0
    DWORD GetRef(void* backend);
    // Native unbounded live-largest-ID scan; returns bool in AL.
    // 0x0001e8e0
    bool TestCollisionShapes(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second);
    int m_nLargestId;
    VehiclePoolQueueEntry queue[100];
    DWORD queueActive[100];
    CVehicle* m_pObjects[2000];
    BOOL m_bNotEmpty[2000];
    void* m_pGameObjects[2000];
    BYTE unknown6ef4[0x2ee0];
    DWORD lastTick[2000];
    DWORD stateBD14[2000];
    BYTE unknownDC54[0x9c40];
    DWORD licensePlatesReady;
    // 0x00001150
    BOOL DoesExist(WORD id);
    // 0x0001e650
    WORD Find(void* gameObject);
    // 0x00001120
    CVehicle* Get(WORD id);
    // Native unbounded largest-ID loop; complete pool extent remains open.
    // 0x0001e8a0
    void ShutdownLicensePlates();
    // Native readiness gate and largest-ID loop.
    // 0x0001e830
    void ConstructLicensePlates();
};
#pragma pack(pop)
typedef char VehiclePoolObjectOffset[(offsetof(CVehiclePool, m_pObjects) == 0x1134) ? 1 : -1];
typedef char VehiclePoolStateOffset[(offsetof(CVehiclePool, m_bNotEmpty) == 0x3074) ? 1 : -1];
typedef char VehiclePoolGtaOffset[(offsetof(CVehiclePool, m_pGameObjects) == 0x4fb4) ? 1 : -1];
typedef char VehiclePoolQueueFlagsOffset[(offsetof(CVehiclePool, queueActive) == 0xfa4) ? 1 : -1];
typedef char VehiclePoolTickOffset[(offsetof(CVehiclePool, lastTick) == 0x9dd4) ? 1 : -1];
typedef char VehiclePoolStateBD14Offset[(offsetof(CVehiclePool, stateBD14) == 0xbd14) ? 1 : -1];
typedef char VehiclePoolPlateReadyOffset[(offsetof(CVehiclePool, licensePlatesReady) == 0x17894) ? 1 : -1];
typedef char VehiclePoolExtent[(sizeof(CVehiclePool) == 0x17898) ? 1 : -1];
typedef char VehiclePoolQueueExtent[(sizeof(VehiclePoolQueueEntry) == 40) ? 1 : -1];
typedef char VehiclePoolQueueModelOffset[(offsetof(VehiclePoolQueueEntry, model) == 2) ? 1 : -1];
typedef char VehiclePoolQueueHealthOffset[(offsetof(VehiclePoolQueueEntry, health) == 0x18) ? 1 : -1];
typedef char VehiclePoolQueueSirenOffset[(offsetof(VehiclePoolQueueEntry, siren) == 0x27) ? 1 : -1];
// 0x001184f8
extern const char g_vehiclePoolNotDeletedMessage[];
// 0x0011851c
extern int g_vehiclePoolDestroyRpc;
// 0x001184c4
extern const char g_vehiclePoolQueueFullMessage[];
#endif
