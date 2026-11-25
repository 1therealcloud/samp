#ifndef VEHICLEPOOL_H
#define VEHICLEPOOL_H

#include "game/vehicle.h"

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

    void SetUnoccupiedSyncOwner(WORD vehicle, WORD player);
    // First native stack word unused; endpoints are the second and third words.

    WORD FindInLine(DWORD unused, const CVector* origin, const CVector* end);

    // Fourth raw word is copied into both native ray records.

    WORD FindInRay(float maxCameraDistance, const CVector* origin, const CVector* end, DWORD rayWord);

    void Process();
    // Native Create indexes the WORD ID without a bounds guard.

    BOOL Create(const VehiclePoolQueueEntry* entry);
    // Mutates queued train IDs/models in place and clears active even on failure.

    void ProcessWaitingList();
    // Does not consume this; original stack argument is a WORD vehicle ID.

    void SendDestroyNotification(WORD id);

    void UpdateLargestId();

    CVehiclePool();

    ~CVehiclePool();

    BOOL Delete(WORD id);
    // Native unchecked WORD index.

    void ChangeInterior(WORD id, int interior);

    void AssignSpecialParams(WORD id, BYTE objective, BYTE locked);

    WORD FindNearestToLocalPlayer();

    WORD FindNearestToPoint(float x, float y, float z);

    void Queue(const VehiclePoolQueueEntry* entry);
    // Native ID overload has no bounds check.

    DWORD GetRef(int id);
    // No this data is consumed by the backend overload.

    DWORD GetRef(void* backend);
    // Native unbounded live-largest-ID scan; returns bool in AL.

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

    BOOL DoesExist(WORD id);

    WORD Find(void* gameObject);

    CVehicle* Get(WORD id);
    // Native unbounded largest-ID loop; complete pool extent remains open.

    void ShutdownLicensePlates();
    // Native readiness gate and largest-ID loop.

    void ConstructLicensePlates();
};
#pragma pack(pop)
static_assert(offsetof(CVehiclePool, m_pObjects) == 0x1134);
static_assert(offsetof(CVehiclePool, m_bNotEmpty) == 0x3074);
static_assert(offsetof(CVehiclePool, m_pGameObjects) == 0x4fb4);
static_assert(offsetof(CVehiclePool, queueActive) == 0xfa4);
static_assert(offsetof(CVehiclePool, lastTick) == 0x9dd4);
static_assert(offsetof(CVehiclePool, stateBD14) == 0xbd14);
static_assert(offsetof(CVehiclePool, licensePlatesReady) == 0x17894);
static_assert(sizeof(CVehiclePool) == 0x17898);
static_assert(sizeof(VehiclePoolQueueEntry) == 40);
static_assert(offsetof(VehiclePoolQueueEntry, model) == 2);
static_assert(offsetof(VehiclePoolQueueEntry, health) == 0x18);
static_assert(offsetof(VehiclePoolQueueEntry, siren) == 0x27);

extern const char g_vehiclePoolNotDeletedMessage[];

extern int g_vehiclePoolDestroyRpc;

extern const char g_vehiclePoolQueueFullMessage[];

#endif // VEHICLEPOOL_H
