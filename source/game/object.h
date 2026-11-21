#ifndef OBJECT_H
#define OBJECT_H

#include "game/entity.h"
#include "game/quaternion.h"

#include <static_assert.h>

// Native allocation extent is established; observed pool flags are named below.
// Remaining storage and its untouched bytes stay explicitly unknown.
class CVehicle;
struct IDirect3DTexture9;
#pragma pack(push, 1)
// Observed contiguous native records; unknown bytes retain their original storage.
struct ObjectMaterialTextInfo
{
    BYTE unknown00, index;
    BYTE unknown02[0x88];
    BYTE size;
    char font[65];
    BYTE fontSize, bold;
    DWORD fontColor, backgroundColor;
    BYTE alignment;
};
struct ObjectMaterialState
{
    DWORD frameObjectsProcessed, snapshotValid, texturesChanged, colorsChanged;
    void* rwObject;
    void* savedTextures[16];
    void* textures[16];
    DWORD colors[16], savedColors[16], savedGeometryFlags;
    int types[16];
    DWORD textProcessed[16];
    ObjectMaterialTextInfo textInfo[16];
    char* text[16];
    // Non-owning pointer view copied from the RenderWare raster's +0x34 slot.
    IDirect3DTexture9* rasterField34[16];
    IDirect3DTexture9* renderedText[16];
};
class CObject : public CEntity
{
    friend class CObjectPool;
    // R5 address to verify
    friend void* __cdecl ObjectApplyMaterials(void* atomic, void* object);
    // R5 address to verify
    friend void* __cdecl ObjectSaveMaterials(void* atomic, void* object);
    // R5 address to verify
    friend void* __cdecl ObjectRestoreMaterials(void* atomic, void* object);
    // Set after successful backend lookup; IsAdded tests this raw byte.
    BYTE backendCreationMarker;
    BYTE renderReady;
    DWORD streamingHideCount;
    int modelIndex;
    BYTE usesModelDrawDistance;
    BYTE cameraCollisionDisabled;
    float streamingDistance, collisionRadius;
    CVector worldCentre;
    float cameraDistance;
    BYTE drawLast;
    BYTE unknown6d[0x40];
    CVector eulerRotation;
    BYTE unknownb9;
    DWORD unknownba;
    WORD attachedVehicle, attachedObject;
    CVector attachmentOffset, attachmentRotation;
    BYTE attachmentRotationSync;
    CMatrix targetMatrix, baseMatrix;
    float moveDistance;
    DWORD moveStartTick, lastMoveTick;
    GameQuaternion targetQuaternion, interpolatedQuaternion, startQuaternion;
    // Native GetEulerInverted ordering: normalized input y, x, z.
    CVector targetEuler, angleDelta;
    BYTE motionFlags;
    float moveSpeed;
    BYTE unknown1b4, rotationEnabled;
    BYTE unknown1b6[0x0d];
    ObjectMaterialState materials;
    // Native raw DWORD gate shared by ordinary and text materials.
    BOOL hasMaterials;
    DWORD materialPassState;
    // Raw lighting request and call-entry marker; original names remain unknown.
    BYTE lightingPassRequested, lightingPassEntered;
    DWORD unknown1195;
    BYTE timeVisibilityEnabled, visibilityStartHour, visibilityEndHour;

public:
    // Direct pointer to the internal Euler vector; no copy or backend query.
    // R5 address to verify
    CVector* GetRotation();
    // R5 address to verify
    void DisableCameraCollision();
    // Native eleven-word entry; lightingRequest retains its raw byte.
    // 0x000a9110
    CObject(int model, float x, float y, float z, CVector rotation, float drawDistance, BYTE lightingRequest, BYTE firstHour, BYTE lastHour);
    // 0x000a9430
    virtual ~CObject();
    // R5 address to verify
    virtual void Add();
    // R5 address to verify
    virtual void Remove();
    // 0x000a8340
    void AttachToBackend(void* target, const CVector* offset, const CVector* rotation);
    // 0x000a8f20
    void ProcessFrameObjectsOnce();
    // Render/Draw names follow the DL API map; original SA-MP spelling unconfirmed.
    // 0x000a8f60
    void Render();
    // 0x000a9f90
    void Draw();
    // 0x000a7e20
    void UpdateAttachedObjectRotation();
    // 0x000a8110
    void SetAttachedVehicle(WORD id, const CVector* offset, const CVector* rotation);
    // 0x000a81a0
    void SetAttachedObject(WORD id, const CVector* offset, const CVector* rotation, BYTE flag);
    // 0x000a84c0
    BOOL IsMoving();
    // 0x000a7dd0
    BOOL IsAdded();
    // 0x000a9040
    void SetupLighting();
    // 0x000a9080
    void RemoveLighting();
    // R5 address to verify
    bool HasRwFrameFlag8();
    // 0x000a8fd0
    void UpdateCachedCentre();
    // 0x000a7f50
    bool IsTimeVisible();
    // 0x000a8fa0
    void ClearModelFlag0();
    // 0x000a9560
    void ProcessStreaming();
    // 0x000a7fc0
    float GetDistanceToTarget(const CMatrix* matrix);
    // 0x000a8240
    void AttachToVehicle(CVehicle* vehicle);
    // 0x000a82c0
    void AttachToObject(CObject* object);
    // 0x000a9650
    void Process(float elapsedSeconds);
    // 0x000a8530
    void SetMaterial(int model, int index, const char* dictionary, const char* texture, DWORD color);
    // 0x000a86b0
    void SetMaterialText(int index, const char* text, BYTE size, const char* font, BYTE fontSize, BYTE bold, DWORD fontColor, DWORD backgroundColor, BYTE alignment);
    // 0x000a9e30
    void BeforeMaterialRender();
    // 0x000a9e70
    void AfterMaterialRender();
    // 0x000a8c00
    void ProcessMaterials();
    // 0x000a8c80
    bool GetMaterialSize(int code, int* width, int* height);
    // 0x000a9ee0
    void ConstructMaterialText();
    // 0x000a8ed0
    void ShutdownMaterialText();
    // 0x000a80a0
    void SetRotation(const CVector* rotation);
    // 0x000a8030
    void Stop();
    // Native angle helper ignores this but consumes two float words.
    // 0x000a8000
    float GetAngleDifference(float from, float to);
    // 0x000a83c0
    void Rotate(float x, float y, float z);
    // Original consumes seven float words.
    // 0x000a9bd0
    void Move(CVector position, float speed, CVector rotation);
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(ObjectMaterialTextInfo) == 0xd7);
STATIC_ASSERT(offsetof(ObjectMaterialTextInfo, size) == 0x8a);
STATIC_ASSERT(offsetof(ObjectMaterialTextInfo, font) == 0x8b);
STATIC_ASSERT(offsetof(ObjectMaterialTextInfo, fontColor) == 0xce);
STATIC_ASSERT(offsetof(ObjectMaterialTextInfo, alignment) == 0xd6);
STATIC_ASSERT(sizeof(ObjectMaterialState) == 0xfc8);
STATIC_ASSERT(offsetof(ObjectMaterialState, textures) == 0x54);
STATIC_ASSERT(offsetof(ObjectMaterialState, colors) == 0x94);
STATIC_ASSERT(offsetof(ObjectMaterialState, types) == 0x118);
STATIC_ASSERT(offsetof(ObjectMaterialState, textInfo) == 0x198);
STATIC_ASSERT(offsetof(ObjectMaterialState, text) == 0xf08);
STATIC_ASSERT(offsetof(ObjectMaterialState, rasterField34) == 0xf48);
STATIC_ASSERT(offsetof(ObjectMaterialState, renderedText) == 0xf88);
STATIC_ASSERT(sizeof(CObject) == 0x119c);
// 0x000ecd88
extern const float g_objectRenderDistanceThreshold;
// 0x000e6170
extern const float g_objectStreamingNearDistance;
// R5 address to verify
extern const float g_objectStreamingVehicleMargin;
// Shared native0.01 constant, also used by remote movement gates.
// 0x000e6120
extern const float g_objectTurnVelocityScale;
// R5 address to verify
extern BYTE g_objectRenderFlag;
// R5 address to verify
extern CMatrix g_objectAttachmentMatrix;
// R5 address to verify
extern CVector g_objectAttachmentZeroSpeed;
// 0x001a2600
extern void* g_objectSharedMaterialTexture;
// 0x000b3c30
void* __stdcall GamePoolObjectGetAt(int handle);
// R5 address to verify
void* __cdecl ObjectFrameIdentity(void* object, void* data);
// Raw render traversal callbacks return the input atomic pointer.
// R5 address to verify
void* __cdecl ObjectApplyMaterials(void* atomic, void* object);
// R5 address to verify
void* __cdecl ObjectSaveMaterials(void* atomic, void* object);
// R5 address to verify
void* __cdecl ObjectRestoreMaterials(void* atomic, void* object);

#endif // OBJECT_H
