#ifndef SAMP_OBJECT_POOL_H
#define SAMP_OBJECT_POOL_H
#include "game/object.h"
#include <windows.h>
struct RPCParameters;
#pragma pack(push, 1)
class CObjectPool
{
    typedef char ObjectReadyOffset[(offsetof(CObject, renderReady) == 0x49) ? 1 : -1];
    typedef char ObjectCameraCollisionOffset[(offsetof(CObject, cameraCollisionDisabled) == 0x53) ? 1 : -1];
    typedef char ObjectDrawLastOffset[(offsetof(CObject, drawLast) == 0x6c) ? 1 : -1];
    typedef char ObjectAttachmentEulerOffset[(offsetof(CObject, eulerRotation) == 0xad) ? 1 : -1];
    typedef char ObjectAttachmentVehicleOffset[(offsetof(CObject, attachedVehicle) == 0xbe) ? 1 : -1];
    typedef char ObjectAttachmentObjectOffset[(offsetof(CObject, attachedObject) == 0xc0) ? 1 : -1];
    typedef char ObjectAttachmentOffsetOffset[(offsetof(CObject, attachmentOffset) == 0xc2) ? 1 : -1];
    typedef char ObjectAttachmentRotationOffset[(offsetof(CObject, attachmentRotation) == 0xce) ? 1 : -1];
    typedef char ObjectAttachmentSyncOffset[(offsetof(CObject, attachmentRotationSync) == 0xda) ? 1 : -1];
    typedef char ObjectAttachmentTargetMatrixOffset[(offsetof(CObject, targetMatrix) == 0xdb) ? 1 : -1];
    typedef char ObjectAttachmentBaseMatrixOffset[(offsetof(CObject, baseMatrix) == 0x11b) ? 1 : -1];
    typedef char ObjectMoveDistanceOffset[(offsetof(CObject, moveDistance) == 0x15b) ? 1 : -1];
    typedef char ObjectMoveStartTickOffset[(offsetof(CObject, moveStartTick) == 0x15f) ? 1 : -1];
    typedef char ObjectLastMoveTickOffset[(offsetof(CObject, lastMoveTick) == 0x163) ? 1 : -1];
    typedef char ObjectTargetQuaternionOffset[(offsetof(CObject, targetQuaternion) == 0x167) ? 1 : -1];
    typedef char ObjectInterpolatedQuaternionOffset[(offsetof(CObject, interpolatedQuaternion) == 0x177) ? 1 : -1];
    typedef char ObjectStartQuaternionOffset[(offsetof(CObject, startQuaternion) == 0x187) ? 1 : -1];
    typedef char ObjectTargetEulerOffset[(offsetof(CObject, targetEuler) == 0x197) ? 1 : -1];
    typedef char ObjectAngleDeltaOffset[(offsetof(CObject, angleDelta) == 0x1a3) ? 1 : -1];
    typedef char ObjectMotionFlagsOffset[(offsetof(CObject, motionFlags) == 0x1af) ? 1 : -1];
    typedef char ObjectMoveSpeedOffset[(offsetof(CObject, moveSpeed) == 0x1b0) ? 1 : -1];
    typedef char ObjectRotationEnabledOffset[(offsetof(CObject, rotationEnabled) == 0x1b5) ? 1 : -1];
    typedef char ObjectStreamingHideCountOffset[(offsetof(CObject, streamingHideCount) == 0x4a) ? 1 : -1];
    typedef char ObjectModelIndexOffset[(offsetof(CObject, modelIndex) == 0x4e) ? 1 : -1];
    typedef char ObjectUsesModelDrawDistanceOffset[(offsetof(CObject, usesModelDrawDistance) == 0x52) ? 1 : -1];
    typedef char ObjectStreamingDistanceOffset[(offsetof(CObject, streamingDistance) == 0x54) ? 1 : -1];
    typedef char ObjectCollisionRadiusOffset[(offsetof(CObject, collisionRadius) == 0x58) ? 1 : -1];
    typedef char ObjectWorldCentreOffset[(offsetof(CObject, worldCentre) == 0x5c) ? 1 : -1];
    typedef char ObjectCameraDistanceOffset[(offsetof(CObject, cameraDistance) == 0x68) ? 1 : -1];
    typedef char ObjectTimeVisibilityEnabledOffset[(offsetof(CObject, timeVisibilityEnabled) == 0x1199) ? 1 : -1];
    typedef char ObjectVisibilityStartHourOffset[(offsetof(CObject, visibilityStartHour) == 0x119a) ? 1 : -1];
    typedef char ObjectVisibilityEndHourOffset[(offsetof(CObject, visibilityEndHour) == 0x119b) ? 1 : -1];
    typedef char ObjectMaterialStateOffset[(offsetof(CObject, materials) == 0x1c3) ? 1 : -1];
    typedef char ObjectMaterialPassOffset[(offsetof(CObject, materialPassState) == 0x118f) ? 1 : -1];
    typedef char ObjectMaterialGateOffset[(offsetof(CObject, hasMaterials) == 0x118b) ? 1 : -1];
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
typedef char NativeObjectPoolExtent[(sizeof(CObjectPool) == 0x41a4) ? 1 : -1];
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
