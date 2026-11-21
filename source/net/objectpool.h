#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include "game/object.h"

#include <windows.h>

#include <static_assert.h>

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
    // 0x00019ff0
    friend void ScrAttachCameraToObject(RPCParameters* parameters);
    int largestId;
    DWORD active[2100];
    CObject* objects[2100];

public:
    // R5 address to verify
    int GetLargestId();
    // R5 address to verify
    DWORD IsActive(WORD id);
    // R5 address to verify
    void UpdateAttachedObjectRotations();

    // 0x00012e70
    WORD FindInRay(float maxDistance, const CVector* origin, const CVector* end);
    // R5 address to verify
    bool TestCollisionShapes(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second);
    // 0x00012b90
    void Draw();
    // 0x00012bd0
    void DrawLast();
    // Native Create has no ID bounds guard.
    // 0x000128d0
    BOOL Create(WORD id, int model, CVector position, CVector rotation, float drawDistance);
    // 0x00012800
    CObjectPool();
    // 0x00013160
    ~CObjectPool();
    // 0x000127a0
    void UpdateLargestId();
    // 0x00012830
    int GetCount();
    // The original also accepts object ID 2100.
    // 0x00012850
    BOOL Delete(WORD id);
    // Starts at slot1, snapshots largest ID and ignores active flags.
    // 0x000129d0
    CObject* Find(void* backend);
    // 0x00012a10
    WORD GetId(void* backend);
    // 0x00002de0
    CEntity* Get(WORD id);
    // 0x00012a50
    void Process();
    // 0x00012b10
    void ConstructMaterials();
    // 0x00012b50
    void ShutdownMaterials();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CObjectPool) == 0x41a4);
// Native copies all five words; the fifth is intentionally uninitialized stack data.
// R5 address to verify
extern GameEntityCollisionShape g_objectPoolRayOrigin;
// R5 address to verify
extern GameEntityCollisionShape g_objectPoolRayEnd;
// R5 address to verify
extern CObject* g_objectPoolRayDebugObject;
// 0x000e60b0
extern const char g_objectPoolRayOriginText[8];
// 0x000e60a8
extern const char g_objectPoolRayEndText[8];
// No this pointer is consumed; class ownership and callers are not established.
// R5 address to verify
void DrawObjectPoolCollisionDebug();
// R5 address to verify
extern DWORD g_objectPoolLastTick;
// 0x000e60a4
extern const float g_objectPoolSecondsPerTick;

#endif // OBJECTPOOL_H
