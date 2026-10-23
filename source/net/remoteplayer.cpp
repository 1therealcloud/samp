#include "net/remoteplayer.h"
#include "net/vehiclepool.h"
#include "net/objectpool.h"
#include "game/playerped.h"
#include "game/util.h"
#include "main.h"
#include <string.h>
// samp.dll RVA: 0x000144d0
void CRemotePlayer::ApplySurfingMatrix() {
    BYTE *state=(BYTE*)this;
    if(state[0x1a]!=17)return;
    WORD id=*(WORD*)(state+0x7a);
    if(!id||id==0xffff||id>=4100)return;
    BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);
    CVehiclePool *vehicles=*(CVehiclePool**)(pools+0xc);
    CObjectPool *objects=*(CObjectPool**)(pools+0x14);
    CEntity *entity;
    if(id<2000)entity=vehicles->Get(id);
    else entity=objects->Get((WORD)(id-2000));
    if(!entity)return;
    for(int axis=0;axis<3;axis++) {
        float *component=(float*)(state+0x6e)+axis;BYTE accepted;
        // Preserve FCOMP unordered acceptance and native +/-100 bounds.
        __asm {
            mov edx,component
            fld dword ptr [edx]
            fcomp g_vehicleTrainSpeedMaximum
            fnstsw ax
            test ah,41h
            jz invalid_offset
            fld dword ptr [edx]
            fcomp g_vehicleTrainSpeedMinimum
            fnstsw ax
            test ah,5
            jnp invalid_offset
            mov accepted,1
            jmp checked_offset
        invalid_offset:
            mov accepted,0
        checked_offset:
        }
        if(!accepted)return;
    }
    CMatrix entityMatrix,pedMatrix;CVector speed,turnSpeed,position;
    entity->GetMatrix(&entityMatrix);
    entity->GetSpeed(&speed);entity->GetTurnSpeed(&turnSpeed);
    (*(CPed**)(state+4))->GetMatrix(&pedMatrix);
    GameTransformPoint(&position,&entityMatrix,(CVector*)(state+0x6e));
    memcpy(&pedMatrix.position,&position,12);
    (*(CPed**)(state+4))->ApplyMatrix(pedMatrix);
}

// samp.dll RVA: 0x00001080
BOOL CRemotePlayer::IsActive() {
    BYTE *state=(BYTE*)this;
    return *(CPed**)(state+4)&&state[0x1a]!=0;
}
