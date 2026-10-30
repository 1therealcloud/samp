#ifndef SAMP_OBJECT_WRAPPER_H
#define SAMP_OBJECT_WRAPPER_H
#include "game/entity.h"
#include "game/quaternion.h"
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
// Descriptive grouping of the verified render/material table region.
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
    // 0x000a8430
    friend void* __cdecl ObjectApplyMaterials(void* atomic, void* object);
    // 0x000a86c0
    friend void* __cdecl ObjectSaveMaterials(void* atomic, void* object);
    // 0x000a8740
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
    // 0x000715c0
    CVector* GetRotation();
    // 0x00017750
    void DisableCameraCollision();
    // Native eleven-word entry; lightingRequest retains its raw byte.
    // 0x000a8d00
    CObject(int model, CVector position, CVector rotation, float drawDistance, BYTE lightingRequest, BYTE firstHour, BYTE lastHour);
    // 0x000a9020
    virtual ~CObject();
    // 0x000a79a0
    virtual void Add();
    // 0x000a79f0
    virtual void Remove();
    // 0x000a7f30
    void AttachToBackend(void* target, const CVector* offset, const CVector* rotation);
    // 0x000a8b10
    void ProcessFrameObjectsOnce();
    // Render/Draw names follow the DL API map; original SA-MP spelling unconfirmed.
    // 0x000a8b50
    void Render();
    // 0x000a9b80
    void Draw();
    // 0x000a7a10
    void UpdateAttachedObjectRotation();
    // 0x000a7d00
    void SetAttachedVehicle(WORD id, const CVector* offset, const CVector* rotation);
    // 0x000a7d90
    void SetAttachedObject(WORD id, const CVector* offset, const CVector* rotation, BYTE flag);
    // 0x000a80b0
    BOOL IsMoving();
    // 0x000a79c0
    BOOL IsAdded();
    // Descriptive recovered lighting operation; original SA-MP spelling unconfirmed.
    // 0x000a8c30
    void SetupLighting();
    // Descriptive recovered lighting operation; original SA-MP spelling unconfirmed.
    // 0x000a8c70
    void RemoveLighting();
    // 0x000a8cc0
    bool HasRwFrameFlag8();
    // 0x000a8bc0
    void UpdateCachedCentre();
    // 0x000a7b40
    bool IsTimeVisible();
    // 0x000a8b90
    void ClearModelFlag0();
    // 0x000a9150
    void ProcessStreaming();
    // 0x000a7bb0
    float GetDistanceToTarget(const CMatrix* matrix);
    // 0x000a7e30
    void AttachToVehicle(CVehicle* vehicle);
    // 0x000a7eb0
    void AttachToObject(CObject* object);
    // Downstream motion dependency of the recovered pool coordinator.
    // 0x000a9240
    void Process(float elapsedSeconds);
    // 0x000a8120
    void SetMaterial(int model, int index, const char* dictionary, const char* texture, DWORD color);
    // 0x000a82a0
    void SetMaterialText(int index, const char* text, BYTE size, const char* font, BYTE fontSize, BYTE bold, DWORD fontColor, DWORD backgroundColor, BYTE alignment);
    // 0x000a9a20
    void BeforeMaterialRender();
    // 0x000a9a60
    void AfterMaterialRender();
    // 0x000a87f0
    void ProcessMaterials();
    // 0x000a8870
    bool GetMaterialSize(int code, int* width, int* height);
    // 0x000a9ad0
    void ConstructMaterialText();
    // 0x000a8ac0
    void ShutdownMaterialText();
    // 0x000a7c90
    void SetRotation(const CVector* rotation);
    // 0x000a7c20
    void Stop();
    // Native angle helper ignores this but consumes two float words.
    // 0x000a7bf0
    float GetAngleDifference(float from, float to);
    // Rotation matrix builder dependency; not a production stand-in.
    // 0x000a7fb0
    void Rotate(float x, float y, float z);
    // Original consumes seven float words.
    // 0x000a97c0
    void Move(CVector position, float speed, CVector rotation);
};
#pragma pack(pop)
typedef char ObjectTextInfoExtent[(sizeof(ObjectMaterialTextInfo) == 0xd7) ? 1 : -1];
typedef char ObjectTextInfoSize[(offsetof(ObjectMaterialTextInfo, size) == 0x8a) ? 1 : -1];
typedef char ObjectTextInfoFont[(offsetof(ObjectMaterialTextInfo, font) == 0x8b) ? 1 : -1];
typedef char ObjectTextInfoColor[(offsetof(ObjectMaterialTextInfo, fontColor) == 0xce) ? 1 : -1];
typedef char ObjectTextInfoAlignment[(offsetof(ObjectMaterialTextInfo, alignment) == 0xd6) ? 1 : -1];
typedef char ObjectMaterialStateExtent[(sizeof(ObjectMaterialState) == 0xfc8) ? 1 : -1];
typedef char ObjectMaterialTextures[(offsetof(ObjectMaterialState, textures) == 0x54) ? 1 : -1];
typedef char ObjectMaterialColors[(offsetof(ObjectMaterialState, colors) == 0x94) ? 1 : -1];
typedef char ObjectMaterialTypes[(offsetof(ObjectMaterialState, types) == 0x118) ? 1 : -1];
typedef char ObjectMaterialRecords[(offsetof(ObjectMaterialState, textInfo) == 0x198) ? 1 : -1];
typedef char ObjectMaterialText[(offsetof(ObjectMaterialState, text) == 0xf08) ? 1 : -1];
typedef char ObjectMaterialRaster[(offsetof(ObjectMaterialState, rasterField34) == 0xf48) ? 1 : -1];
typedef char ObjectMaterialRendered[(offsetof(ObjectMaterialState, renderedText) == 0xf88) ? 1 : -1];
typedef char NativeObjectAllocationExtent[(sizeof(CObject) == 0x119c) ? 1 : -1];
// 0x0011ee80
extern const float g_objectRenderDistanceThreshold;
// 0x00118200
extern const float g_objectStreamingNearDistance;
// 0x00117878
extern const float g_objectStreamingVehicleMargin;
// Shared native0.01 constant, also used by remote movement gates.
// 0x001181b0
extern const float g_objectTurnVelocityScale;
// 0x001e04a8
extern BYTE g_objectRenderFlag;
// 0x001e0450
extern CMatrix g_objectAttachmentMatrix;
// 0x001e049c
extern CVector g_objectAttachmentZeroSpeed;
// 0x001e0498
extern void* g_objectSharedMaterialTexture;
// 0x000b3830
void* __stdcall GamePoolObjectGetAt(int handle);
// 0x000a8b00
void* __cdecl ObjectFrameIdentity(void* object, void* data);
// Raw render traversal callbacks return the input atomic pointer.
// 0x000a8430
void* __cdecl ObjectApplyMaterials(void* atomic, void* object);
// 0x000a86c0
void* __cdecl ObjectSaveMaterials(void* atomic, void* object);
// 0x000a8740
void* __cdecl ObjectRestoreMaterials(void* atomic, void* object);
#endif
