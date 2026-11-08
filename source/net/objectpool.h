#ifndef SAMP_OBJECT_POOL_H
#define SAMP_OBJECT_POOL_H
#include <static_assert.h>
#include "game/object.h"
#include <windows.h>
struct RPCParameters;
#pragma pack(push, 1)
class CObjectPool
{
    STATIC_ASSERT(offsetof(CObject, renderReady) == 0x49);
    STATIC_ASSERT(offsetof(CObject, cameraCollisionDisabled) == 0x53);
    STATIC_ASSERT(offsetof(CObject, drawLast) == 0x6c);
    STATIC_ASSERT(offsetof(CObject, eulerRotation) == 0xad);
    STATIC_ASSERT(offsetof(CObject, attachedVehicle) == 0xbe);
    STATIC_ASSERT(offsetof(CObject, attachedObject) == 0xc0);
    STATIC_ASSERT(offsetof(CObject, attachmentOffset) == 0xc2);
    STATIC_ASSERT(offsetof(CObject, attachmentRotation) == 0xce);
    STATIC_ASSERT(offsetof(CObject, attachmentRotationSync) == 0xda);
    STATIC_ASSERT(offsetof(CObject, targetMatrix) == 0xdb);
    STATIC_ASSERT(offsetof(CObject, baseMatrix) == 0x11b);
    STATIC_ASSERT(offsetof(CObject, moveDistance) == 0x15b);
    STATIC_ASSERT(offsetof(CObject, moveStartTick) == 0x15f);
    STATIC_ASSERT(offsetof(CObject, lastMoveTick) == 0x163);
    STATIC_ASSERT(offsetof(CObject, targetQuaternion) == 0x167);
    STATIC_ASSERT(offsetof(CObject, interpolatedQuaternion) == 0x177);
    STATIC_ASSERT(offsetof(CObject, startQuaternion) == 0x187);
    STATIC_ASSERT(offsetof(CObject, targetEuler) == 0x197);
    STATIC_ASSERT(offsetof(CObject, angleDelta) == 0x1a3);
    STATIC_ASSERT(offsetof(CObject, motionFlags) == 0x1af);
    STATIC_ASSERT(offsetof(CObject, moveSpeed) == 0x1b0);
    STATIC_ASSERT(offsetof(CObject, rotationEnabled) == 0x1b5);
    STATIC_ASSERT(offsetof(CObject, streamingHideCount) == 0x4a);
    STATIC_ASSERT(offsetof(CObject, modelIndex) == 0x4e);
    STATIC_ASSERT(offsetof(CObject, usesModelDrawDistance) == 0x52);
    STATIC_ASSERT(offsetof(CObject, streamingDistance) == 0x54);
    STATIC_ASSERT(offsetof(CObject, collisionRadius) == 0x58);
    STATIC_ASSERT(offsetof(CObject, worldCentre) == 0x5c);
    STATIC_ASSERT(offsetof(CObject, cameraDistance) == 0x68);
    STATIC_ASSERT(offsetof(CObject, timeVisibilityEnabled) == 0x1199);
    STATIC_ASSERT(offsetof(CObject, visibilityStartHour) == 0x119a);
    STATIC_ASSERT(offsetof(CObject, visibilityEndHour) == 0x119b);
    STATIC_ASSERT(offsetof(CObject, materials) == 0x1c3);
    STATIC_ASSERT(offsetof(CObject, materialPassState) == 0x118f);
    STATIC_ASSERT(offsetof(CObject, hasMaterials) == 0x118b);
    // 0x00019b20
    friend void ScrAttachCameraToObject(RPCParameters* parameters);
    int largestId;
    DWORD active[2100];
    CObject* objects[2100];

public:
    // 0x00012670
    int GetLargestId();
    // 0x00012680
    DWORD IsActive(WORD id);
    // 0x000129d0
    void UpdateAttachedObjectRotations();

    // Descriptive name. Native snapshots the ray, but reloads origin for distances.
    // 0x00012d70
    WORD FindInRay(float maxDistance, const CVector* origin, const CVector* end);
    // Descriptive name: tests eligible pool objects, returns on first collision.
    // 0x00012ce0
    bool TestCollisionShapes(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second);
    // 0x00012a90
    void Draw();
    // 0x00012ad0
    void DrawLast();
    // Native Create has no ID bounds guard.
    // 0x000127d0
    BOOL Create(WORD id, int model, CVector position, CVector rotation, float drawDistance);
    // 0x00012700
    CObjectPool();
    // 0x00013060
    ~CObjectPool();
    // 0x000126a0
    void UpdateLargestId();
    // 0x00012730
    int GetCount();
    // Native permits ID2100; adjacent allocation layout is not established.
    // 0x00012750
    BOOL Delete(WORD id);
    // Starts at slot1, snapshots largest ID and ignores active flags.
    // 0x000128d0
    CObject* Find(void* backend);
    // 0x00012910
    WORD GetId(void* backend);
    // 0x00002e10
    CEntity* Get(WORD id);
    // 0x00012950
    void Process();
    // 0x00012a10
    void ConstructMaterials();
    // 0x00012a50
    void ShutdownMaterials();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CObjectPool) == 0x41a4);
// Native copies all five words; the fifth is intentionally uninitialized stack data.
// 0x00156b5c
extern GameEntityCollisionShape g_objectPoolRayOrigin;
// 0x00156b48
extern GameEntityCollisionShape g_objectPoolRayEnd;
// 0x00156b70
extern CObject* g_objectPoolRayDebugObject;
// 0x0011813c
extern const char g_objectPoolRayOriginText[8];
// 0x00118134
extern const char g_objectPoolRayEndText[8];
// No this pointer is consumed; class ownership and callers are not established.
// 0x00012f50
void DrawObjectPoolCollisionDebug();
// 0x00156b74
extern DWORD g_objectPoolLastTick;
// 0x00118130
extern const float g_objectPoolSecondsPerTick;
#endif
