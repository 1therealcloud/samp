#include "game/entity.h"
#include <string.h>
// samp.dll RVA: 0x001181b8
extern const float g_entityRadiansToDegrees=57.2957763671875f;
// samp.dll RVA: 0x0011c660
extern const float g_entityEulerSign=-1.0f;
// samp.dll RVA: 0x0011b31c
extern const float g_entityBoundaryLift=0.03999999910593033f;
// samp.dll RVA: 0x0009ea00
void CEntity::SetMatrix(CMatrix matrix) {
    if(m_pGameEntity && *(void**)((BYTE*)m_pGameEntity+0x14)) {
        for(int row=0;row<4;row++)for(int word=0;word<3;word++) {
            BYTE *target=*(BYTE**)((BYTE*)m_pGameEntity+0x14);
            *(DWORD*)(target+row*16+word*4)=*(DWORD*)((BYTE*)&matrix+row*16+word*4);
        }
    }
}
// samp.dll RVA: 0x0009eb20
void CEntity::GetSpeed(CVector *speed) { memcpy(speed,(BYTE*)m_pGameEntity+0x44,12); }
// samp.dll RVA: 0x0009eb50
void CEntity::SetSpeed(CVector speed) { memcpy((BYTE*)m_pGameEntity+0x44,&speed,12); }
// samp.dll RVA: 0x0009ec70
void CEntity::GetTurnSpeed(CVector *speed) { memcpy(speed,(BYTE*)m_pGameEntity+0x50,12); }
// samp.dll RVA: 0x0009eca0
void CEntity::SetTurnSpeed(CVector speed) { memcpy((BYTE*)m_pGameEntity+0x50,&speed,12); }
// samp.dll RVA: 0x0009ecd0
void CEntity::ApplyTurnSpeed() { if(m_pGameEntity)((void (__fastcall*)(void*,void*))0x542e20)(m_pGameEntity,0); }
// GTA returns raw float bits in EAX, not an ST0 float result.
// samp.dll RVA: 0x0009ecf0
float CEntity::GetDistanceFromCentreOfMassToBaseOfModel() {
    if(!m_pGameEntity)return 0.0f;
    DWORD bits=((DWORD (__fastcall*)(void*,void*))0x536be0)(m_pGameEntity,0);
    float distance;memcpy(&distance,&bits,4);return distance;
}
// samp.dll RVA: 0x0009ed30
void CEntity::GetBoundCentre(CVector *centre) { if(m_pGameEntity)((void (__fastcall*)(void*,void*,CVector*))0x534250)(m_pGameEntity,0,centre); }
// samp.dll RVA: 0x0009ee70
int CEntity::GetModelIndex() { return (int)*(short*)((BYTE*)m_pGameEntity+0x22); }
// samp.dll RVA: 0x0009f210
BOOL CEntity::DoesExist() { return m_pGameEntity && *(DWORD*)m_pGameEntity!=0x863c40 && *(DWORD*)((BYTE*)m_pGameEntity+0xb4)?TRUE:FALSE; }
// samp.dll RVA: 0x0009f260
BOOL CEntity::EnforceWorldBoundries(float positiveX,float negativeX,float positiveY,float negativeY) {
    if(!m_pGameEntity)return FALSE;
    CMatrix matrix;GetMatrix(&matrix);CVector speed;GetSpeed(&speed);
    if(matrix.position.x>positiveX) { if(speed.x!=0.0f){speed.x=-0.2f;speed.z=0.1f;} }
    else if(matrix.position.x<negativeX) { if(speed.x!=0.0f){speed.x=0.2f;speed.z=0.1f;} }
    else if(matrix.position.y>positiveY) { if(speed.y!=0.0f){speed.y=-0.2f;speed.z=0.1f;} }
    else if(matrix.position.y<negativeY) { if(speed.y!=0.0f){speed.y=0.2f;speed.z=0.1f;} }
    else return FALSE;
    SetSpeed(speed);matrix.position.z+=g_entityBoundaryLift;SetMatrix(matrix);return TRUE;
}
// samp.dll RVA: 0x0009f400
BOOL CEntity::HasExceededWorldBoundries(float positiveX,float negativeX,float positiveY,float negativeY) {
    if(!m_pGameEntity)return FALSE;
    CMatrix matrix;GetMatrix(&matrix);
    return matrix.position.x>positiveX || matrix.position.x<negativeX || matrix.position.y>positiveY || matrix.position.y<negativeY?TRUE:FALSE;
}
// samp.dll RVA: 0x0009f4a0
bool CEntity::GetCollisionFlag() { return !m_pGameEntity || *(DWORD*)m_pGameEntity==0x863c40 || (*(DWORD*)((BYTE*)m_pGameEntity+0x1c)&1)!=0; }
// samp.dll RVA: 0x0009f470
void CEntity::SetCollisionFlag(BOOL enabled) {
    if(m_pGameEntity && *(DWORD*)m_pGameEntity!=0x863c40) {
        DWORD &flags=*(DWORD*)((BYTE*)m_pGameEntity+0x1c);
        if(enabled)flags|=1;else flags&=~1u;
    }
}
// samp.dll RVA: 0x0009f4c0
void CEntity::SetCollisionProcessed(BOOL processed) {
    if(m_pGameEntity && *(DWORD*)m_pGameEntity!=0x863c40) {
        DWORD &flags=*(DWORD*)((BYTE*)m_pGameEntity+0x1c);
        if(processed)flags&=0x7ffffffd;else flags|=0x80000002;
    }
}
// samp.dll RVA: 0x0009f730
void CEntity::GetEulerInverted(float *x,float *y,float *z) {
    if(m_pGameEntity) {
        void *matrix=*(void**)((BYTE*)m_pGameEntity+0x14);
        if(matrix)((void (__fastcall*)(void*,void*,float*,float*,float*,int))0x59a840)(matrix,0,x,y,z,0x15);
        // Keep the two native x87 multiplies and the store only at the end.
        __asm {
            mov eax,x
            fld dword ptr [eax]
            fmul g_entityRadiansToDegrees
            fmul g_entityEulerSign
            fstp dword ptr [eax]
            mov eax,y
            fld dword ptr [eax]
            fmul g_entityRadiansToDegrees
            fmul g_entityEulerSign
            fstp dword ptr [eax]
            mov eax,z
            fld dword ptr [eax]
            fmul g_entityRadiansToDegrees
            fmul g_entityEulerSign
            fstp dword ptr [eax]
        }
    }
}
// samp.dll RVA: 0x0009f8a0
void *CEntity::GetRwObject() { return m_pGameEntity?*(void**)((BYTE*)m_pGameEntity+0x18):0; }
// samp.dll RVA: 0x0009f9f0
void CEntity::DeleteRwObject() { if(m_pGameEntity && *(void**)((BYTE*)m_pGameEntity+0x18))((void (__fastcall*)(void*,void*))(*(DWORD**)m_pGameEntity)[8])(m_pGameEntity,0); }
// samp.dll RVA: 0x0009fb20
bool CEntity::IsIgnored() { return m_pGameEntity && m_pGameEntity==*(void**)0xb7cd68; }
// samp.dll RVA: 0x0009fc20
BOOL CEntity::IsStationary() {
    if(!DoesExist())return FALSE;
    CVector speed;GetSpeed(&speed);return speed.x==0.0f && speed.y==0.0f && speed.z==0.0f?TRUE:FALSE;
}
// samp.dll RVA: 0x0009eac0
void CEntity::UpdateRwFrame() {
    void *entity=m_pGameEntity;
    if(entity && *(DWORD*)entity!=0x863c40) {
        BYTE *object=*(BYTE**)((BYTE*)entity+0x18);void *matrix=*(void**)((BYTE*)entity+0x14);
        if(object && matrix) {
            ((void (__fastcall*)(void*,void*,void*))0x59ad70)(matrix,0,*(BYTE**)(object+4)+0x10);
            ((void (__fastcall*)(void*,void*))0x532b00)(entity,0);
        }
    }
}
