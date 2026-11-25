#ifndef ENTITY_H
#define ENTITY_H

#include <stddef.h>
#include <string.h>
#include <windows.h>

// Five DWORDs passed to the collision adapter.
struct GameEntityCollisionShape
{
    DWORD words[5];
};
struct CVector
{
    float x, y, z;
};
class CMatrix
{
public:
    CVector right;
    DWORD flags;
    CVector up;
    DWORD pad1;
    CVector at;
    DWORD pad2;
    CVector position;
    DWORD pad3;
};
// Observed GTA backend prefix only; unknown ranges are not reconstructed fields.
struct EntityBackendPrefix
{
    // Raw address and RW word view follow the supplied 0.2x backend representation.
    DWORD vtable;
    BYTE unknown04[16];
    CMatrix* matrix;
    DWORD* rwObject;
    DWORD flags;
    BYTE unknown20[2];
    short model;
    BYTE unknown24[28];
    DWORD rawFlags40;
    CVector speed;
    CVector turnSpeed;
};
static_assert(offsetof(EntityBackendPrefix, matrix) == 0x14);
static_assert(offsetof(EntityBackendPrefix, speed) == 0x44);
static_assert(offsetof(EntityBackendPrefix, turnSpeed) == 0x50);
static_assert(sizeof(EntityBackendPrefix) == 0x5c);

void GameChangeBackendEntityModel(void* entity, DWORD model);
// Raw helper; no full object type or vtable meaning established.

DWORD __stdcall GameReadBackendDword(const void* backend);
// samp.dll vtable RVA: 0x0011e30c
class CEntity
{
public:
    // Five history vectors occupy existing bytes04..3F; in-place shift order retained.

    void SetAveragedSpeed(CVector speed);
    // Native x87 sine/cosine; writes nine coefficients, preserving matrix padding/position.

    void SetEulerRotation(float x, float y, float z);

    // Descriptive raw flag names; invalid GTA backend vtable is excluded.
    // Clearing bit40000 invokes virtual Add afterwards.

    void SetStaticWaitingForCollision(BOOL enabled);

    void DisableStreamingAndDeletion();
    // Nonzero argument clears bit2000; zero sets it.

    void SetSpeedApplicationEnabled(BOOL clear);

    DWORD GetRwObjectWord38();

    void GetRwMatrixAngles(float* x, float* y, float* z);

    int GetModelAtomicCount();

    union
    {
        CVector m_aSpeedHistory[5];
        // Raw view retained for historical native fixtures.
        BYTE unknown04[60];
    };
    union
    {
        void* m_pGameEntity;
        EntityBackendPrefix* m_pEntity;
    };
    DWORD m_nHandle;

    CEntity()
    {
        memset(m_aSpeedHistory, 0, sizeof(m_aSpeedHistory));
    }

    // Native only resets the vptr; backend/history storage is untouched.

    virtual ~CEntity() {}

    virtual void Add();

    virtual void Remove();
    // Clears mask 0x00000008 in the backend DWORD at offset 0x40.

    void ClearBackendFlag8();

    void GetMatrix(CMatrix* matrix);

    void ApplyMatrix(CMatrix matrix);

    float GetDistanceToPoint(float x, float y, float z);

    void SetMatrix(CMatrix matrix);

    void GetSpeed(CVector* speed);

    void SetSpeed(CVector speed);

    void GetTurnSpeed(CVector* speed);

    void SetTurnSpeed(CVector speed);

    void ApplyTurnSpeed();

    float GetDistanceFromCentreOfMassToBaseOfModel();

    void GetBoundCentre(CVector* centre);

    int GetModelIndex();
    // Uses camera position for the two observed local-player mode flags.

    float GetDistanceToLocalPlayer();

    float GetDistanceToCamera();

    float GetDistanceToLocalPlayerNoHeight();

    void SetBackendHighFlag();

    void ProcessControl();
    // Additional native wrappers; original names/semantic argument types are unresolved.

    void GetBackendBoundRect(DWORD argument);

    void SetRenderClumpAlpha(DWORD argument);

    void ApplyBackendTurnForce(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5, DWORD a6);

    void AddVehicleAudioEvent(DWORD argument);

    void RenderForModelPreview();

    void ApplyLinearSpeed();

    bool TestCollisionShapes(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second, BYTE mode);
    // Returns TRUE only when this call requested/loaded the model, not merely
    // when the entity's model was successfully changed.

    BOOL SetModelIndex(int model);

    BOOL DoesExist();

    BOOL EnforceWorldBoundries(float positiveX, float negativeX, float positiveY, float negativeY);

    BOOL HasExceededWorldBoundries(float positiveX, float negativeX, float positiveY, float negativeY);
    // Native returns the complete masked EAX word (0 or 1).

    DWORD GetCollisionFlag();

    void SetCollisionFlag(BOOL enabled);

    void SetCollisionProcessed(BOOL processed);

    void GetEulerInverted(float* x, float* y, float* z);

    DWORD* GetRwObject();

    void DeleteRwObject();

    bool IsIgnored();

    BOOL IsStationary();

    void UpdateRwFrame();

    void Teleport(float x, float y, float z);
    // Source adapter only; no established separate native entry.
    void Teleport(CVector position)
    {
        Teleport(position.x, position.y, position.z);
    }
};
static_assert(sizeof(CEntity) == 72);
static_assert(offsetof(CEntity, m_aSpeedHistory) == 4);
static_assert(offsetof(CEntity, m_pGameEntity) == 0x40);

void __stdcall GameWorldAdd(void* entity);

void __stdcall GameWorldRemove(void* entity);

extern const float g_entityRadiansToDegrees;

extern const float g_entityEulerSign;

extern const float g_entityBoundaryLift;

extern const float g_entityUnavailableDistance;
#endif

#ifndef SAMP_ENTITY_COLLISION_H
#define SAMP_ENTITY_COLLISION_H
#include "game/util.h"
typedef bool(__cdecl* GameEntityCollisionBackend)(const GameEntityCollisionShape*, const GameEntityCollisionShape*, void*);
// Opaque native scratch span ends at saved-context RVA 002ABE5C.
// No typed collision-object layout or broader initialization completeness is claimed.

extern GameEntityCollisionShape g_entityCollisionShapeA;

extern GameEntityCollisionShape g_entityCollisionShapeB;

extern void* g_entityCollisionEntity;

extern GameEntityCollisionBackend g_entityCollisionBackend;

extern void** g_entityCollisionContextSlot;

extern void* g_entityCollisionSavedContext;

extern BYTE g_entityCollisionResult;

float __cdecl GameGetModelCollisionRadius(int model);

bool __stdcall GameEntityCollisionQueryDirect(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second, void* entity);

bool __stdcall GameEntityCollisionQueryScoped(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second, void* entity);
// Only the four-float sphere prefix is read by this GTA query.
struct GameCollisionSphere
{
    CVector centre;
    float radius;
};

BOOL GameTestCollisionSpheres(const GameCollisionSphere* first, const GameCollisionSphere* second);
// Semantics unresolved; this is the observed pointer/zero backend adapter.

BYTE GameHasCollisionLoaded(const void* data);
// Raw native flag words are forwarded without bool normalization; result is AL.

BYTE GameProcessLineOfSightRaw(const CVector* origin, const CVector* target, void* colPoint, void** entity, DWORD buildings, DWORD vehicles, DWORD peds, DWORD objects,
                               DWORD dummies, DWORD seeThrough, DWORD cameraIgnore, DWORD shootThrough);
#endif

#ifndef SAMP_ENTITY_DETACH_H
#define SAMP_ENTITY_DETACH_H
// Shared original constants; definitions already maintained by chatbubble/object.

extern const float g_chatBubbleHeightLift;

extern const float g_objectDegreesToRadians;

extern const char g_entityDetachExceptionLabel[5];
// Original owner/name unknown; ECX unused, two cdecl stack arguments.

void __cdecl GameDetachBackendRwObject(void* entity, void* object);

#endif // ENTITY_H
