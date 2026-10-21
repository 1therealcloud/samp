#ifndef SAMP_RECOVERED_ENTITY_H
#define SAMP_RECOVERED_ENTITY_H
#include <windows.h>
#include <stddef.h>
// Five raw DWORDs consumed by the native collision adapter; semantic fields unresolved.
struct GameEntityCollisionShape { DWORD words[5]; };
struct CVector { float x,y,z; };
class CMatrix { public: CVector right;DWORD flags;CVector up;DWORD pad1;CVector at;DWORD pad2;CVector position;DWORD pad3; };
// samp.dll RVA: 0x000b3990
void GameChangeBackendEntityModel(void *entity,DWORD model);
// Raw helper; no full object type or vtable meaning established.
// samp.dll RVA: 0x000b4120
DWORD __stdcall GameReadBackendDword(const void *backend);
#pragma pack(push,1)
// samp.dll vtable RVA: 0x0011e30c
class CEntity {
public:
    BYTE unknown04[60];
    void *m_pGameEntity;
    DWORD m_nHandle;
    // samp.dll RVA: 0x0009bce0
    CEntity();
    // samp.dll RVA: unknown (inline empty base destructor/vptr reset; scalar deleting entry RVA 0x0009bd10, standalone destructor entry not established)
    virtual ~CEntity() {}
    // samp.dll RVA: 0x0009f140
    virtual void Add();
    // samp.dll RVA: 0x0009f240
    virtual void Remove();
    // Broader flag meaning is not established; clears backend DWORD +40 mask 0x00000008.
    // samp.dll RVA: 0x0009f5c0
    void ClearBackendFlag8();
    // samp.dll RVA: 0x0009e950
    void GetMatrix(CMatrix *matrix);
    // samp.dll RVA: 0x0009fb40
    void ApplyMatrix(CMatrix matrix);
    // samp.dll RVA: 0x0009f0f0
    float GetDistanceToPoint(float x,float y,float z);
    // samp.dll RVA: 0x0009ea00
    void SetMatrix(CMatrix matrix);
    // samp.dll RVA: 0x0009eb20
    void GetSpeed(CVector *speed);
    // samp.dll RVA: 0x0009eb50
    void SetSpeed(CVector speed);
    // samp.dll RVA: 0x0009ec70
    void GetTurnSpeed(CVector *speed);
    // samp.dll RVA: 0x0009eca0
    void SetTurnSpeed(CVector speed);
    // samp.dll RVA: 0x0009ecd0
    void ApplyTurnSpeed();
    // samp.dll RVA: 0x0009ecf0
    float GetDistanceFromCentreOfMassToBaseOfModel();
    // samp.dll RVA: 0x0009ed30
    void GetBoundCentre(CVector *centre);
    // samp.dll RVA: 0x0009ee70
    int GetModelIndex();
    // Uses camera position for the two observed local-player mode flags.
    // samp.dll RVA: 0x0009ef00
    float GetDistanceToLocalPlayer();
    // samp.dll RVA: 0x0009efd0
    float GetDistanceToCamera();
    // samp.dll RVA: 0x0009f030
    float GetDistanceToLocalPlayerNoHeight();
    // samp.dll RVA: 0x0009f560
    void SetBackendHighFlag();
    // samp.dll RVA: 0x0009fa20
    void ProcessControl();
    // Additional native wrappers; original names/semantic argument types are unresolved.
    // samp.dll RVA: 0x0009ed60
    void Native009ed60(DWORD argument);
    // samp.dll RVA: 0x0009f5d0
    void Native009f5d0(DWORD argument);
    // samp.dll RVA: 0x0009f810
    void Native009f810(DWORD a1,DWORD a2,DWORD a3,DWORD a4,DWORD a5,DWORD a6);
    // samp.dll RVA: 0x0009f840
    void Native009f840(DWORD argument);
    // samp.dll RVA: 0x0009f960
    void Native009f960();
    // samp.dll RVA: 0x0009fba0
    void ApplyLinearSpeed();
    // samp.dll RVA: 0x0009fa40
    bool TestCollisionShapes(const GameEntityCollisionShape *first,const GameEntityCollisionShape *second,BYTE mode);
    // Returns TRUE only when this call requested/loaded the model, not merely
    // when the entity's model was successfully changed.
    // samp.dll RVA: 0x0009ed90
    BOOL SetModelIndex(int model);
    // samp.dll RVA: 0x0009f210
    BOOL DoesExist();
    // samp.dll RVA: 0x0009f260
    BOOL EnforceWorldBoundries(float positiveX,float negativeX,float positiveY,float negativeY);
    // samp.dll RVA: 0x0009f400
    BOOL HasExceededWorldBoundries(float positiveX,float negativeX,float positiveY,float negativeY);
    // Native results use AL; the SAMP-API BOOL signature is not ABI evidence.
    // samp.dll RVA: 0x0009f4a0
    bool GetCollisionFlag();
    // samp.dll RVA: 0x0009f470
    void SetCollisionFlag(BOOL enabled);
    // samp.dll RVA: 0x0009f4c0
    void SetCollisionProcessed(BOOL processed);
    // samp.dll RVA: 0x0009f730
    void GetEulerInverted(float *x,float *y,float *z);
    // samp.dll RVA: 0x0009f8a0
    void *GetRwObject();
    // samp.dll RVA: 0x0009f9f0
    void DeleteRwObject();
    // samp.dll RVA: 0x0009fb20
    bool IsIgnored();
    // samp.dll RVA: 0x0009fc20
    BOOL IsStationary();
    // samp.dll RVA: 0x0009eac0
    void UpdateRwFrame();
    // samp.dll RVA: 0x0009ee80
    void Teleport(CVector position);
};
#pragma pack(pop)
typedef char EntityExtent[(sizeof(CEntity)==72)?1:-1];
typedef char EntityBackendOffset[(offsetof(CEntity,m_pGameEntity)==0x40)?1:-1];
// samp.dll RVA: 0x000b3720
void __stdcall GameWorldAdd(void *entity);
// samp.dll RVA: 0x000b3740
void __stdcall GameWorldRemove(void *entity);
// samp.dll RVA: 0x001181b8
extern const float g_entityRadiansToDegrees;
// samp.dll RVA: 0x0011c660
extern const float g_entityEulerSign;
// samp.dll RVA: 0x0011b31c
extern const float g_entityBoundaryLift;
// samp.dll RVA: 0x0011e644
extern const float g_entityUnavailableDistance;
#endif

#ifndef SAMP_ENTITY_COLLISION_H
#define SAMP_ENTITY_COLLISION_H
#include "game/util.h"
typedef bool (__cdecl *GameEntityCollisionBackend)(const GameEntityCollisionShape*,const GameEntityCollisionShape*,void*);
// Opaque native scratch span ends at saved-context RVA 002ABE5C.
// No typed collision-object layout or broader initialization completeness is claimed.
// samp.dll RVA: 0x0018de60
extern GameEntityCollisionShape g_entityCollisionShapeA;
// samp.dll RVA: 0x0018de4c
extern GameEntityCollisionShape g_entityCollisionShapeB;
// samp.dll RVA: 0x0018de7c
extern void *g_entityCollisionEntity;
// samp.dll RVA: 0x00155404
extern GameEntityCollisionBackend g_entityCollisionBackend;
// samp.dll RVA: 0x00155408
extern void **g_entityCollisionContextSlot;
// samp.dll RVA: 0x002abe5c
extern void *g_entityCollisionSavedContext;
// samp.dll RVA: 0x002abe60
extern BYTE g_entityCollisionResult;
// samp.dll RVA: 0x000b5ea0
float __cdecl GameGetModelCollisionRadius(int model);
// samp.dll RVA: 0x000b6070
bool __stdcall GameEntityCollisionQueryDirect(const GameEntityCollisionShape *first,const GameEntityCollisionShape *second,void *entity);
// samp.dll RVA: 0x000b6090
bool __stdcall GameEntityCollisionQueryScoped(const GameEntityCollisionShape *first,const GameEntityCollisionShape *second,void *entity);
// Only the four-float sphere prefix is read by this GTA query.
struct GameCollisionSphere { CVector centre; float radius; };
// samp.dll RVA: 0x000b60e0
BOOL GameTestCollisionSpheres(const GameCollisionSphere *first,const GameCollisionSphere *second);
// Semantics unresolved; this is the observed pointer/zero backend adapter.
// samp.dll RVA: 0x000b43f0
BYTE GameQueryBackend410ce0(const void *data);
// Raw native flag words are forwarded without bool normalization; result is AL.
// samp.dll RVA: 0x000b3710
BYTE GameProcessLineOfSightRaw(const CVector *origin,const CVector *target,void *colPoint,
    void **entity,DWORD buildings,DWORD vehicles,DWORD peds,DWORD objects,
    DWORD dummies,DWORD seeThrough,DWORD cameraIgnore,DWORD shootThrough);
#endif
