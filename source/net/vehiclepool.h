#ifndef VEHICLEPOOL_H
#define VEHICLEPOOL_H

#include "game/vehicle.h"

#include <static_assert.h>

#pragma pack(push, 1)
// Packed 40-byte record.
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
    // R5 address to verify
    void SetUnoccupiedSyncOwner(WORD vehicle, WORD player);
    // First native stack word unused; endpoints are the second and third words.
    // 0x0001eeb0
    WORD FindInLine(DWORD unused, const CVector* origin, const CVector* end);

    // Fourth raw word is copied into both native ray records.
    // 0x0001ef30
    WORD FindInRay(float maxCameraDistance, const CVector* origin, const CVector* end, DWORD rayWord);
    // 0x0001f3c0
    void Process();
    // Native Create indexes the WORD ID without a bounds guard.
    // 0x0001f080
    BOOL Create(const VehiclePoolQueueEntry* entry);
    // Mutates queued train IDs/models in place and clears active even on failure.
    // 0x0001f300
    void ProcessWaitingList();
    // Does not consume this; original stack argument is a WORD vehicle ID.
    // 0x0001f230
    void SendDestroyNotification(WORD id);
    // 0x0001e9b0
    void UpdateLargestId();
    // 0x0001ea10
    CVehiclePool();
    // 0x0001f060
    ~CVehiclePool();
    // 0x0001ea80
    BOOL Delete(WORD id);
    // Native unchecked WORD index.
    // 0x0001eb00
    void ChangeInterior(WORD id, int interior);
    // 0x0001eb30
    void AssignSpecialParams(WORD id, BYTE objective, BYTE locked);
    // 0x0001ec00
    WORD FindNearestToLocalPlayer();
    // 0x0001ec70
    WORD FindNearestToPoint(float x, float y, float z);
    // 0x0001ed10
    void Queue(const VehiclePoolQueueEntry* entry);
    // Native ID overload has no bounds check.
    // 0x0001ebc0
    DWORD GetRef(int id);
    // No this data is consumed by the backend overload.
    // 0x0001ebe0
    DWORD GetRef(void* backend);
    // Native unbounded live-largest-ID scan; returns bool in AL.
    // 0x0001ee20
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
    // 0x0001eb90
    WORD Find(void* gameObject);
    // 0x00001120
    CVehicle* Get(WORD id);
    // Native unbounded largest-ID loop; complete pool extent remains open.
    // 0x0001ede0
    void ShutdownLicensePlates();
    // Native readiness gate and largest-ID loop.
    // 0x0001ed70
    void ConstructLicensePlates();
};
#pragma pack(pop)
STATIC_ASSERT(offsetof(CVehiclePool, m_pObjects) == 0x1134);
STATIC_ASSERT(offsetof(CVehiclePool, m_bNotEmpty) == 0x3074);
STATIC_ASSERT(offsetof(CVehiclePool, m_pGameObjects) == 0x4fb4);
STATIC_ASSERT(offsetof(CVehiclePool, queueActive) == 0xfa4);
STATIC_ASSERT(offsetof(CVehiclePool, lastTick) == 0x9dd4);
STATIC_ASSERT(offsetof(CVehiclePool, stateBD14) == 0xbd14);
STATIC_ASSERT(offsetof(CVehiclePool, licensePlatesReady) == 0x17894);
STATIC_ASSERT(sizeof(CVehiclePool) == 0x17898);
STATIC_ASSERT(sizeof(VehiclePoolQueueEntry) == 40);
STATIC_ASSERT(offsetof(VehiclePoolQueueEntry, model) == 2);
STATIC_ASSERT(offsetof(VehiclePoolQueueEntry, health) == 0x18);
STATIC_ASSERT(offsetof(VehiclePoolQueueEntry, siren) == 0x27);
// 0x000e6468
extern const char g_vehiclePoolNotDeletedMessage[];
// R5 address to verify
extern int g_vehiclePoolDestroyRpc;
// 0x000e6434
extern const char g_vehiclePoolQueueFullMessage[];

#endif // VEHICLEPOOL_H
