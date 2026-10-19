#ifndef SAMP_RECOVERED_ENTITY_H
#define SAMP_RECOVERED_ENTITY_H
#include <windows.h>
#include <stddef.h>
struct CVector { float x,y,z; };
class CMatrix { public: CVector right;DWORD flags;CVector up;DWORD pad1;CVector at;DWORD pad2;CVector position;DWORD pad3; };
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
    // samp.dll RVA: 0x0009e950
    void GetMatrix(CMatrix *matrix);
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
    // Stage 2 distance wrapper dependency.
    // samp.dll RVA: 0x0009ef00
    float GetDistanceToLocalPlayer();
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
#endif
