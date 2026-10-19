#include "gui/object_edit.h"
#include "gui/device_objects.h"
#include "game/object.h"
#include "game/object_math.h"
#include "game/playerped.h"
#include "main.h"
#include <string.h>
// samp.dll RVA: 0x00072cb0
void CObjectEdit::ApplyObjectChanges(int axis,float difference) {
    CObjectPool *pool=*(CObjectPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0x14);
    CObject *object=(CObject*)pool->Get(objectId);if(!object)return;
    float x,y,z;object->GetEulerInverted(&x,&y,&z);object->ClearBackendFlag8();
    CMatrix matrix;memcpy(&matrix,(BYTE*)this+0xc3,64);
    CVector position=matrix.position,delta={0,0,0};
    if(axis>=0&&axis<3) {
        if(unknown7c==0) {
            ((float*)&delta)[axis]=difference;GameTransformPoint(&position,&matrix,&delta);
            matrix.position=position;CVector noRotation={-1000.0f,-1000.0f,-1000.0f};object->Move(position,1.0f,noRotation);
        } else if(unknown7c==1) {
            float *angle=(float*)((BYTE*)this+0x92+axis*4);*angle=GameNormalizeAngle(difference*g_objectRotationFactor+*angle);
            object->SetRotation((CVector*)((BYTE*)this+0x92));
        }
    }
    SendEditEndNotification(2);memcpy((BYTE*)this+0xc3,&matrix,64);
}
// samp.dll RVA: 0x00071cb0
void CObjectEdit::ApplyAttachedChanges(int axis,float difference) {
    int slot=*(int*)((BYTE*)this+0x8a);CPed *ped=g_pGame->GetPlayerPed();
    if(slot<0||slot>=10||!ped->m_accessoryOccupied[slot])return;
    BYTE *record=ped->m_accessoryInfo[slot].opaque;if(!record)return;
    if(axis<0||axis>2)return;
    DWORD mode=unknown7c;
    if(mode==0){float *value=(float*)(record+8+axis*4);*value=difference*g_objectMutationStep+*value;}
    else if(mode==1){float *value=(float*)(record+0x14+axis*4);*value=GameNormalizeAngle(difference*g_objectRotationFactor+*value);}
    else if(mode==2) {
        float *value=(float*)(record+0x20+axis*4);
        // FST then compare the unrounded x87 result, including its unordered behavior.
        __asm {
            mov edx,value
            fld difference
            fmul g_objectMutationStep
            fadd dword ptr [edx]
            fst dword ptr [edx]
            fcomp g_objectScaleLowerBound
            fnstsw ax
            test ah,5
            jp done
            mov dword ptr [edx],0
        done:
        }
    }
}
