#ifndef ENTITY_H
#define ENTITY_H

#include <stddef.h>
#include <string.h>
#include <windows.h>

#include <static_assert.h>

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
#pragma pack(push, 1)
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
#pragma pack(pop)
STATIC_ASSERT(offsetof(EntityBackendPrefix, matrix) == 0x14);
STATIC_ASSERT(offsetof(EntityBackendPrefix, speed) == 0x44);
STATIC_ASSERT(offsetof(EntityBackendPrefix, turnSpeed) == 0x50);
STATIC_ASSERT(sizeof(EntityBackendPrefix) == 0x5c);
// 0x000b3d90
void GameChangeBackendEntityModel(void* entity, DWORD model);
// Raw helper; no full object type or vtable meaning established.
// R5 address to verify
DWORD __stdcall GameReadBackendDword(const void* backend);
#pragma pack(push, 1)
// samp.dll vtable RVA: 0x0011e30c
class CEntity
{
public:
    // Five history vectors occupy existing bytes04..3F; in-place shift order retained.
    // R5 address to verify
    void SetAveragedSpeed(CVector speed);
    // Native x87 sine/cosine; writes nine coefficients, preserving matrix padding/position.
    // R5 address to verify
    void SetEulerRotation(float x, float y, float z);

    // Descriptive raw flag names; invalid GTA backend vtable is excluded.
    // Clearing bit40000 invokes virtual Add afterwards.
    // R5 address to verify
    void SetBackendFlag40000(BOOL enabled);
    // R5 address to verify
    void SetBackendFlags80400();
    // Nonzero argument clears bit2000; zero sets it.
    // R5 address to verify
    void SetBackendFlag2000(BOOL clear);
    // R5 address to verify
    DWORD GetRwObjectWord38();
    // R5 address to verify
    void GetRwMatrixAngles(float* x, float* y, float* z);
    // R5 address to verify
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
    // 0x0009c260
    CEntity()
    {
        memset(m_aSpeedHistory, 0, sizeof(m_aSpeedHistory));
    }

    // Native only resets the vptr; backend/history storage is untouched.
    // R5 address to verify
    virtual ~CEntity() {}
    // 0x0009f300
    virtual void Add();
    // 0x0009f400
    virtual void Remove();
    // Clears mask 0x00000008 in the backend DWORD at offset 0x40.
    // 0x0009f780
    void ClearBackendFlag8();
    // 0x0009eb10
    void GetMatrix(CMatrix* matrix);
    // 0x0009fd00
    void ApplyMatrix(CMatrix matrix);
    // 0x0009f2b0
    float GetDistanceToPoint(float x, float y, float z);
    // 0x0009ebc0
    void SetMatrix(CMatrix matrix);
    // 0x0009ece0
    void GetSpeed(CVector* speed);
    // 0x0009ed10
    void SetSpeed(CVector speed);
    // 0x0009ee30
    void GetTurnSpeed(CVector* speed);
    // 0x0009ee60
    void SetTurnSpeed(CVector speed);
    // 0x0009ee90
    void ApplyTurnSpeed();
    // 0x0009eeb0
    float GetDistanceFromCentreOfMassToBaseOfModel();
    // 0x0009eef0
    void GetBoundCentre(CVector* centre);
    // 0x0009f030
    int GetModelIndex();
    // Uses camera position for the two observed local-player mode flags.
    // 0x0009f0c0
    float GetDistanceToLocalPlayer();
    // 0x0009f190
    float GetDistanceToCamera();
    // 0x0009f1f0
    float GetDistanceToLocalPlayerNoHeight();
    // 0x0009f720
    void SetBackendHighFlag();
    // 0x0009fbe0
    void ProcessControl();
    // Additional native wrappers; original names/semantic argument types are unresolved.
    // 0x0009ef20
    void Native009ed60(DWORD argument);
    // 0x0009f790
    void Native009f5d0(DWORD argument);
    // 0x0009f9d0
    void Native009f810(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5, DWORD a6);
    // 0x0009fa00
    void Native009f840(DWORD argument);
    // 0x0009fb20
    void Native009f960();
    // 0x0009fd60
    void ApplyLinearSpeed();
    // R5 address to verify
    bool TestCollisionShapes(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second, BYTE mode);
    // Returns TRUE only when this call requested/loaded the model, not merely
    // when the entity's model was successfully changed.
    // 0x0009ef50
    BOOL SetModelIndex(int model);
    // 0x0009f3d0
    BOOL DoesExist();
    // 0x0009f420
    BOOL EnforceWorldBoundries(float positiveX, float negativeX, float positiveY, float negativeY);
    // 0x0009f5c0
    BOOL HasExceededWorldBoundries(float positiveX, float negativeX, float positiveY, float negativeY);
    // Native returns the complete masked EAX word (0 or 1).
    // 0x0009f660
    DWORD GetCollisionFlag();
    // 0x0009f630
    void SetCollisionFlag(BOOL enabled);
    // 0x0009f680
    void SetCollisionProcessed(BOOL processed);
    // 0x0009f8f0
    void GetEulerInverted(float* x, float* y, float* z);
    // 0x0009fa60
    DWORD* GetRwObject();
    // 0x0009fbb0
    void DeleteRwObject();
    // 0x0009fce0
    bool IsIgnored();
    // 0x0009fde0
    BOOL IsStationary();
    // 0x0009ec80
    void UpdateRwFrame();
    // 0x0009f040
    void Teleport(float x, float y, float z);
    // Source adapter only; no established separate native entry.
    void Teleport(CVector position)
    {
        Teleport(position.x, position.y, position.z);
    }
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CEntity) == 72);
STATIC_ASSERT(offsetof(CEntity, m_aSpeedHistory) == 4);
STATIC_ASSERT(offsetof(CEntity, m_pGameEntity) == 0x40);
// 0x000b3b20
void __stdcall GameWorldAdd(void* entity);
// 0x000b3b40
void __stdcall GameWorldRemove(void* entity);
// 0x000e6128
extern const float g_entityRadiansToDegrees;
// 0x000ea638
extern const float g_entityEulerSign;
// 0x000e928c
extern const float g_entityBoundaryLift;
// 0x000ec54c
extern const float g_entityUnavailableDistance;
#endif

#ifndef SAMP_ENTITY_COLLISION_H
#define SAMP_ENTITY_COLLISION_H
#include "game/util.h"
typedef bool(__cdecl* GameEntityCollisionBackend)(const GameEntityCollisionShape*, const GameEntityCollisionShape*, void*);
// Opaque native scratch span ends at saved-context RVA 002ABE5C.
// No typed collision-object layout or broader initialization completeness is claimed.
// R5 address to verify
extern GameEntityCollisionShape g_entityCollisionShapeA;
// R5 address to verify
extern GameEntityCollisionShape g_entityCollisionShapeB;
// R5 address to verify
extern void* g_entityCollisionEntity;
// 0x0011742c
extern GameEntityCollisionBackend g_entityCollisionBackend;
// 0x00117430
extern void** g_entityCollisionContextSlot;
// R5 address to verify
extern void* g_entityCollisionSavedContext;
// R5 address to verify
extern BYTE g_entityCollisionResult;
// 0x000b6360
float __cdecl GameGetModelCollisionRadius(int model);
// 0x000b6530
bool __stdcall GameEntityCollisionQueryDirect(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second, void* entity);
// 0x000b6550
bool __stdcall GameEntityCollisionQueryScoped(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second, void* entity);
// Only the four-float sphere prefix is read by this GTA query.
struct GameCollisionSphere
{
    CVector centre;
    float radius;
};
// 0x000b65a0
BOOL GameTestCollisionSpheres(const GameCollisionSphere* first, const GameCollisionSphere* second);
// Semantics unresolved; this is the observed pointer/zero backend adapter.
// 0x000b47f0
BYTE GameQueryBackend410ce0(const void* data);
// Raw native flag words are forwarded without bool normalization; result is AL.
// 0x000b3b10
BYTE GameProcessLineOfSightRaw(const CVector* origin, const CVector* target, void* colPoint, void** entity, DWORD buildings, DWORD vehicles, DWORD peds, DWORD objects,
                               DWORD dummies, DWORD seeThrough, DWORD cameraIgnore, DWORD shootThrough);
#endif

#ifndef SAMP_ENTITY_DETACH_H
#define SAMP_ENTITY_DETACH_H
// Shared original constants; definitions already maintained by chatbubble/object.
// 0x000e9c90
extern const float g_chatBubbleHeightLift;
// 0x000ec550
extern const float g_objectDegreesToRadians;
// 0x000ec554
extern const char g_entityDetachExceptionLabel[5];
// Original owner/name unknown; ECX unused, two cdecl stack arguments.
// 0x0009fa70
void __cdecl GameDetachBackendRwObject(void* entity, void* object);

#endif // ENTITY_H
