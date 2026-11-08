#ifndef ENTITY_H
#define ENTITY_H

#include <stddef.h>
#include <string.h>
#include <windows.h>

#include <static_assert.h>
// Five raw DWORDs consumed by the native collision adapter; semantic fields unresolved.
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
// 0x000b3990
void GameChangeBackendEntityModel(void* entity, DWORD model);
// Raw helper; no full object type or vtable meaning established.
// 0x000b4120
DWORD __stdcall GameReadBackendDword(const void* backend);
#pragma pack(push, 1)
// samp.dll vtable RVA: 0x0011e30c
class CEntity
{
public:
    // Five history vectors occupy existing bytes04..3F; in-place shift order retained.
    // 0x0009eb80
    void SetAveragedSpeed(CVector speed);
    // Native x87 sine/cosine; writes nine coefficients, preserving matrix padding/position.
    // 0x0009f630
    void SetEulerRotation(float x, float y, float z);

    // Descriptive raw flag names; invalid GTA backend vtable is excluded.
    // Clearing bit40000 invokes virtual Add afterwards.
    // 0x0009f500
    void SetBackendFlag40000(BOOL enabled);
    // 0x0009f540
    void SetBackendFlags80400();
    // Nonzero argument clears bit2000; zero sets it.
    // 0x0009f580
    void SetBackendFlag2000(BOOL clear);
    // 0x0009f610
    DWORD GetRwObjectWord38();
    // 0x0009f7a0
    void GetRwMatrixAngles(float* x, float* y, float* z);
    // 0x0009f870
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
    // 0x0009bce0
    CEntity()
    {
        memset(m_aSpeedHistory, 0, sizeof(m_aSpeedHistory));
    }
    // Native only resets the vptr; backend/history storage is untouched.
    // 0x0009bd00
    virtual ~CEntity() {}
    // 0x0009f140
    virtual void Add();
    // 0x0009f240
    virtual void Remove();
    // Broader flag meaning is not established; clears backend DWORD +40 mask 0x00000008.
    // 0x0009f5c0
    void ClearBackendFlag8();
    // 0x0009e950
    void GetMatrix(CMatrix* matrix);
    // 0x0009fb40
    void ApplyMatrix(CMatrix matrix);
    // 0x0009f0f0
    float GetDistanceToPoint(float x, float y, float z);
    // 0x0009ea00
    void SetMatrix(CMatrix matrix);
    // 0x0009eb20
    void GetSpeed(CVector* speed);
    // 0x0009eb50
    void SetSpeed(CVector speed);
    // 0x0009ec70
    void GetTurnSpeed(CVector* speed);
    // 0x0009eca0
    void SetTurnSpeed(CVector speed);
    // 0x0009ecd0
    void ApplyTurnSpeed();
    // 0x0009ecf0
    float GetDistanceFromCentreOfMassToBaseOfModel();
    // 0x0009ed30
    void GetBoundCentre(CVector* centre);
    // 0x0009ee70
    int GetModelIndex();
    // Uses camera position for the two observed local-player mode flags.
    // 0x0009ef00
    float GetDistanceToLocalPlayer();
    // 0x0009efd0
    float GetDistanceToCamera();
    // 0x0009f030
    float GetDistanceToLocalPlayerNoHeight();
    // 0x0009f560
    void SetBackendHighFlag();
    // 0x0009fa20
    void ProcessControl();
    // Additional native wrappers; original names/semantic argument types are unresolved.
    // 0x0009ed60
    void Native009ed60(DWORD argument);
    // 0x0009f5d0
    void Native009f5d0(DWORD argument);
    // 0x0009f810
    void Native009f810(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5, DWORD a6);
    // 0x0009f840
    void Native009f840(DWORD argument);
    // 0x0009f960
    void Native009f960();
    // 0x0009fba0
    void ApplyLinearSpeed();
    // 0x0009fa40
    bool TestCollisionShapes(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second, BYTE mode);
    // Returns TRUE only when this call requested/loaded the model, not merely
    // when the entity's model was successfully changed.
    // 0x0009ed90
    BOOL SetModelIndex(int model);
    // 0x0009f210
    BOOL DoesExist();
    // 0x0009f260
    BOOL EnforceWorldBoundries(float positiveX, float negativeX, float positiveY, float negativeY);
    // 0x0009f400
    BOOL HasExceededWorldBoundries(float positiveX, float negativeX, float positiveY, float negativeY);
    // Native returns the complete masked EAX word (0 or 1).
    // 0x0009f4a0
    DWORD GetCollisionFlag();
    // 0x0009f470
    void SetCollisionFlag(BOOL enabled);
    // 0x0009f4c0
    void SetCollisionProcessed(BOOL processed);
    // 0x0009f730
    void GetEulerInverted(float* x, float* y, float* z);
    // 0x0009f8a0
    DWORD* GetRwObject();
    // 0x0009f9f0
    void DeleteRwObject();
    // 0x0009fb20
    bool IsIgnored();
    // 0x0009fc20
    BOOL IsStationary();
    // 0x0009eac0
    void UpdateRwFrame();
    // 0x0009ee80
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
// 0x000b3720
void __stdcall GameWorldAdd(void* entity);
// 0x000b3740
void __stdcall GameWorldRemove(void* entity);
// 0x001181b8
extern const float g_entityRadiansToDegrees;
// 0x0011c660
extern const float g_entityEulerSign;
// 0x0011b31c
extern const float g_entityBoundaryLift;
// 0x0011e644
extern const float g_entityUnavailableDistance;
#endif

#ifndef SAMP_ENTITY_COLLISION_H
#define SAMP_ENTITY_COLLISION_H
#include "game/util.h"
typedef bool(__cdecl* GameEntityCollisionBackend)(const GameEntityCollisionShape*, const GameEntityCollisionShape*, void*);
// Opaque native scratch span ends at saved-context RVA 002ABE5C.
// No typed collision-object layout or broader initialization completeness is claimed.
// 0x0018de60
extern GameEntityCollisionShape g_entityCollisionShapeA;
// 0x0018de4c
extern GameEntityCollisionShape g_entityCollisionShapeB;
// 0x0018de7c
extern void* g_entityCollisionEntity;
// 0x00155404
extern GameEntityCollisionBackend g_entityCollisionBackend;
// 0x00155408
extern void** g_entityCollisionContextSlot;
// 0x002abe5c
extern void* g_entityCollisionSavedContext;
// 0x002abe60
extern BYTE g_entityCollisionResult;
// 0x000b5ea0
float __cdecl GameGetModelCollisionRadius(int model);
// 0x000b6070
bool __stdcall GameEntityCollisionQueryDirect(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second, void* entity);
// 0x000b6090
bool __stdcall GameEntityCollisionQueryScoped(const GameEntityCollisionShape* first, const GameEntityCollisionShape* second, void* entity);
// Only the four-float sphere prefix is read by this GTA query.
struct GameCollisionSphere
{
    CVector centre;
    float radius;
};
// 0x000b60e0
BOOL GameTestCollisionSpheres(const GameCollisionSphere* first, const GameCollisionSphere* second);
// Semantics unresolved; this is the observed pointer/zero backend adapter.
// 0x000b43f0
BYTE GameQueryBackend410ce0(const void* data);
// Raw native flag words are forwarded without bool normalization; result is AL.
// 0x000b3710
BYTE GameProcessLineOfSightRaw(const CVector* origin, const CVector* target, void* colPoint, void** entity, DWORD buildings, DWORD vehicles, DWORD peds, DWORD objects,
                               DWORD dummies, DWORD seeThrough, DWORD cameraIgnore, DWORD shootThrough);
#endif

#ifndef SAMP_ENTITY_DETACH_H
#define SAMP_ENTITY_DETACH_H
// Shared original constants; definitions already maintained by chatbubble/object.
// 0x0011bcc8
extern const float g_chatBubbleHeightLift;
// 0x0011e648
extern const float g_objectDegreesToRadians;
// 0x0011e64c
extern const char g_entityDetachExceptionLabel[5];
// Original owner/name unknown; ECX unused, two cdecl stack arguments.
// 0x0009f8b0
void __cdecl GameDetachBackendRwObject(void* entity, void* object);

#endif // ENTITY_H
