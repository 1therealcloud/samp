#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include "game/object.h"

#include <windows.h>

struct RPCParameters;
#pragma pack(push, 1)
class CObjectPool
{
    static_assert(offsetof(CObject, renderReady) == 0x49);
    static_assert(offsetof(CObject, cameraCollisionDisabled) == 0x53);
    static_assert(offsetof(CObject, drawLast) == 0x6c);
    static_assert(offsetof(CObject, eulerRotation) == 0xad);
    static_assert(offsetof(CObject, attachedVehicle) == 0xbe);
    static_assert(offsetof(CObject, attachedObject) == 0xc0);
    static_assert(offsetof(CObject, attachmentOffset) == 0xc2);
    static_assert(offsetof(CObject, attachmentRotation) == 0xce);
    static_assert(offsetof(CObject, attachmentRotationSync) == 0xda);
    static_assert(offsetof(CObject, targetMatrix) == 0xdb);
    static_assert(offsetof(CObject, baseMatrix) == 0x11b);
    static_assert(offsetof(CObject, moveDistance) == 0x15b);
    static_assert(offsetof(CObject, moveStartTick) == 0x15f);
    static_assert(offsetof(CObject, lastMoveTick) == 0x163);
    static_assert(offsetof(CObject, targetQuaternion) == 0x167);
    static_assert(offsetof(CObject, interpolatedQuaternion) == 0x177);
    static_assert(offsetof(CObject, startQuaternion) == 0x187);
    static_assert(offsetof(CObject, targetEuler) == 0x197);
    static_assert(offsetof(CObject, angleDelta) == 0x1a3);
    static_assert(offsetof(CObject, motionFlags) == 0x1af);
    static_assert(offsetof(CObject, moveSpeed) == 0x1b0);
    static_assert(offsetof(CObject, rotationEnabled) == 0x1b5);
    static_assert(offsetof(CObject, streamingHideCount) == 0x4a);
    static_assert(offsetof(CObject, modelIndex) == 0x4e);
    static_assert(offsetof(CObject, usesModelDrawDistance) == 0x52);
    static_assert(offsetof(CObject, streamingDistance) == 0x54);
    static_assert(offsetof(CObject, collisionRadius) == 0x58);
    static_assert(offsetof(CObject, worldCentre) == 0x5c);
    static_assert(offsetof(CObject, cameraDistance) == 0x68);
    static_assert(offsetof(CObject, timeVisibilityEnabled) == 0x1199);
    static_assert(offsetof(CObject, visibilityStartHour) == 0x119a);
    static_assert(offsetof(CObject, visibilityEndHour) == 0x119b);
    static_assert(offsetof(CObject, materials) == 0x1c3);
    static_assert(offsetof(CObject, materialPassState) == 0x118f);
    static_assert(offsetof(CObject, hasMaterials) == 0x118b);
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

    // 0x00012d70
    WORD FindInRay(float maxDistance, const CVector* origin, const CVector* end);
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
    // The original also accepts object ID 2100.
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
static_assert(sizeof(CObjectPool) == 0x41a4);
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

#endif // OBJECTPOOL_H
