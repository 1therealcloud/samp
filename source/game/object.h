#ifndef OBJECT_H
#define OBJECT_H

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

    friend void* __cdecl ObjectApplyMaterials(void* atomic, void* object);

    friend void* __cdecl ObjectSaveMaterials(void* atomic, void* object);

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

    CVector* GetRotation();

    void DisableCameraCollision();
    // Native eleven-word entry; lightingRequest retains its raw byte.

    CObject(int model, float x, float y, float z, CVector rotation, float drawDistance, BYTE lightingRequest, BYTE firstHour, BYTE lastHour);

    virtual ~CObject();

    virtual void Add();

    virtual void Remove();

    void AttachToBackend(void* target, const CVector* offset, const CVector* rotation);

    void ProcessFrameObjectsOnce();
    // Render/Draw names follow the DL API map; original SA-MP spelling unconfirmed.

    void Render();

    void Draw();

    void UpdateAttachedObjectRotation();

    void SetAttachedVehicle(WORD id, const CVector* offset, const CVector* rotation);

    void SetAttachedObject(WORD id, const CVector* offset, const CVector* rotation, BYTE flag);

    BOOL IsMoving();

    BOOL IsAdded();

    void SetupLighting();

    void RemoveLighting();

    bool HasRwFrameFlag8();

    void UpdateCachedCentre();

    bool IsTimeVisible();

    void ClearModelFlag0();

    void ProcessStreaming();

    float GetDistanceToTarget(const CMatrix* matrix);

    void AttachToVehicle(CVehicle* vehicle);

    void AttachToObject(CObject* object);

    void Process(float elapsedSeconds);

    void SetMaterial(int model, int index, const char* dictionary, const char* texture, DWORD color);

    void SetMaterialText(int index, const char* text, BYTE size, const char* font, BYTE fontSize, BYTE bold, DWORD fontColor, DWORD backgroundColor, BYTE alignment);

    void BeforeMaterialRender();

    void AfterMaterialRender();

    void ProcessMaterials();

    bool GetMaterialSize(int code, int* width, int* height);

    void ConstructMaterialText();

    void ShutdownMaterialText();

    void SetRotation(const CVector* rotation);

    void Stop();
    // Native angle helper ignores this but consumes two float words.

    float GetAngleDifference(float from, float to);

    void Rotate(float x, float y, float z);
    // Original consumes seven float words.

    void Move(CVector position, float speed, CVector rotation);
};
#pragma pack(pop)
static_assert(sizeof(ObjectMaterialTextInfo) == 0xd7);
static_assert(offsetof(ObjectMaterialTextInfo, size) == 0x8a);
static_assert(offsetof(ObjectMaterialTextInfo, font) == 0x8b);
static_assert(offsetof(ObjectMaterialTextInfo, fontColor) == 0xce);
static_assert(offsetof(ObjectMaterialTextInfo, alignment) == 0xd6);
static_assert(sizeof(ObjectMaterialState) == 0xfc8);
static_assert(offsetof(ObjectMaterialState, textures) == 0x54);
static_assert(offsetof(ObjectMaterialState, colors) == 0x94);
static_assert(offsetof(ObjectMaterialState, types) == 0x118);
static_assert(offsetof(ObjectMaterialState, textInfo) == 0x198);
static_assert(offsetof(ObjectMaterialState, text) == 0xf08);
static_assert(offsetof(ObjectMaterialState, rasterField34) == 0xf48);
static_assert(offsetof(ObjectMaterialState, renderedText) == 0xf88);
static_assert(sizeof(CObject) == 0x119c);

extern const float g_objectRenderDistanceThreshold;

extern const float g_objectStreamingNearDistance;

extern const float g_objectStreamingVehicleMargin;
// Shared native0.01 constant, also used by remote movement gates.

extern const float g_objectTurnVelocityScale;

extern BYTE g_objectRenderFlag;

extern CMatrix g_objectAttachmentMatrix;

extern CVector g_objectAttachmentZeroSpeed;

extern void* g_objectSharedMaterialTexture;

void* __stdcall GamePoolObjectGetAt(int handle);

void* __cdecl ObjectFrameIdentity(void* object, void* data);
// Raw render traversal callbacks return the input atomic pointer.

void* __cdecl ObjectApplyMaterials(void* atomic, void* object);

void* __cdecl ObjectSaveMaterials(void* atomic, void* object);

void* __cdecl ObjectRestoreMaterials(void* atomic, void* object);

#endif // OBJECT_H
