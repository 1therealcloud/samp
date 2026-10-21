#include "game/object.h"
#include "game/scripting.h"
// samp.dll RVA: 0x0011edfc
const Scripting::OpcodeInfo g_objectSetRotationCommand={0x453,"ifff"};
// samp.dll RVA: 0x000b3830
void *__stdcall GamePoolObjectGetAt(int handle) {
    void *result;
    // GTA SA virtual addresses: object pool pointer B7449C, lookup 465040.
    __asm {
        mov ecx,dword ptr ds:[0xb7449c]
        push handle
        mov eax,0x465040
        call eax
        mov result,eax
    }
    return result;
}
// samp.dll RVA: 0x000a7c90
void CObject::SetRotation(const CVector *rotation) {
    if(m_pGameEntity&&GamePoolObjectGetAt(m_nHandle)) {
        Scripting::ProcessCommand(&g_objectSetRotationCommand,m_nHandle,(double)rotation->x,(double)rotation->y,(double)rotation->z);
        BYTE *state=(BYTE*)this;*(float*)(state+0xad)=rotation->x;*(float*)(state+0xb1)=rotation->y;*(float*)(state+0xb5)=rotation->z;
    }
}

#include "game/util.h"
#include "game/quaternion.h"
#include "net/localplayer.h"
#include "main.h"
#include <GetTime.h>
#include <string.h>
// samp.dll RVA: 0x0011ee7c
extern const float g_objectMotionRotationSentinel=-999.0f;
// samp.dll RVA: 0x000a7c20
void CObject::Stop(){CVector zero={0,0,0};SetSpeed(zero);SetTurnSpeed(zero);((BYTE*)this)[0x1af]&=0xfe;}
// samp.dll RVA: 0x000a7bf0
float CObject::GetAngleDifference(float from,float to){return GameNormalizeAngle(GameNormalizeAngle(to)-from);}
// samp.dll RVA: 0x0011e648
extern const float g_objectDegreesToRadians=0.01745329238474369f;
// Keep the native x87 trigonometry and float spill boundaries. Only the nine
// rotation coefficients are written; matrix padding and position are untouched.
// samp.dll RVA: 0x000a7fb0
void CObject::Rotate(CVector rotation) {
    BYTE *state=(BYTE*)this;
    __asm {
        mov esi,state
        lea edx,rotation
        fld dword ptr [edx]
        mov ebx, dword ptr [edx + 4]
        fmul g_objectDegreesToRadians
        mov eax, dword ptr [edx]
        fld dword ptr [edx + 4]
        mov dword ptr [esi + 0xad], eax
        fmul g_objectDegreesToRadians
        mov eax, dword ptr [edx + 8]
        mov dword ptr [esi + 0xb1], ebx
        mov dword ptr [esi + 0xb5], eax
        fstp dword ptr [edx + 4]
        fld dword ptr [edx + 8]
        fmul g_objectDegreesToRadians
        fstp dword ptr [edx + 8]
        fld st(0)
        fcos 
        fstp dword ptr [edx]
        fsin 
        fld dword ptr [edx + 4]
        fcos 
        fld dword ptr [edx + 4]
        fsin 
        fld dword ptr [edx + 8]
        fcos 
        fld dword ptr [edx + 8]
        fsin 
        fst dword ptr [edx + 4]
        fmul st,st(4)
        fld st(1)
        fmul st,st(5)
        fstp dword ptr [edx + 8]
        fld st(1)
        fmul st,st(4)
        fld st(1)
        fmul st,st(4)
        fsubp st(1),st
        fstp dword ptr [esi + 0xdb]
        fld dword ptr [edx + 8]
        fmul st,st(3)
        fld dword ptr [edx + 4]
        fmul st,st(5)
        faddp st(1),st
        fstp dword ptr [esi + 0xdf]
        fld st(2)
        fmul dword ptr [edx]
        fchs 
        fstp dword ptr [esi + 0xe3]
        fld dword ptr [edx + 4]
        fmul dword ptr [edx]
        fchs 
        fstp dword ptr [esi + 0xeb]
        fld st(1)
        fmul dword ptr [edx]
        fstp dword ptr [esi + 0xef]
        fxch st(4)
        fstp dword ptr [esi + 0xf3]
        fxch st(3)
        fmul st,st(2)
        fxch st(3)
        fmul st,st(1)
        faddp st(3),st
        fxch st(2)
        fstp dword ptr [esi + 0xfb]
        fld dword ptr [edx + 4]
        fmul st,st(2)
        fld dword ptr [edx + 8]
        fmul st,st(2)
        fsubp st(1),st
        fstp dword ptr [esi + 0xff]
        fstp st(1)
        fmul dword ptr [edx]
        fstp dword ptr [esi + 0x103]
    }
}

// samp.dll RVA: 0x0011ee24
extern const Scripting::OpcodeInfo g_objectIsAttachedCommand={0x685,"i"};
// samp.dll RVA: 0x0011ee10
extern const Scripting::OpcodeInfo g_objectAttachVehicleCommand={0x681,"iiffffff"};
// samp.dll RVA: 0x0011ee38
extern const Scripting::OpcodeInfo g_objectAttachObjectCommand={0x69a,"iiffffff"};
// Keep differences/products in x87 registers through sqrt, returning ST0.
// samp.dll RVA: 0x000a7bb0
float CObject::GetDistanceToTarget(const CMatrix *matrix) {
    BYTE *state=(BYTE*)this;
    __asm {
        mov ecx,state
        mov eax,matrix
        fld dword ptr [eax+0x38]
        fsub dword ptr [ecx+0x113]
        fld dword ptr [eax+0x34]
        fsub dword ptr [ecx+0x10f]
        fld dword ptr [eax+0x30]
        fsub dword ptr [ecx+0x10b]
        fld st
        fmul st,st(1)
        fld st(2)
        fmul st,st(3)
        faddp st(1),st
        fld st(3)
        fmul st,st(4)
        faddp st(1),st
        fsqrt
        fstp st(3)
        fstp st
        fstp st
    }
}
// samp.dll RVA: 0x000a7e30
void CObject::AttachToVehicle(CVehicle *vehicle) {
    if(!Scripting::ProcessCommand(&g_objectIsAttachedCommand,m_nHandle)) {
        BYTE *state=(BYTE*)this;
        Scripting::ProcessCommand(&g_objectAttachVehicleCommand,m_nHandle,((CEntity*)vehicle)->m_nHandle,
            (double)*(float*)(state+0xc2),(double)*(float*)(state+0xc6),(double)*(float*)(state+0xca),
            (double)*(float*)(state+0xce),(double)*(float*)(state+0xd2),(double)*(float*)(state+0xd6));
    }
}
// samp.dll RVA: 0x000a7eb0
void CObject::AttachToObject(CObject *object) {
    if(!Scripting::ProcessCommand(&g_objectIsAttachedCommand,m_nHandle)) {
        BYTE *state=(BYTE*)this;
        Scripting::ProcessCommand(&g_objectAttachObjectCommand,m_nHandle,object->m_nHandle,
            (double)*(float*)(state+0xc2),(double)*(float*)(state+0xc6),(double)*(float*)(state+0xca),
            (double)*(float*)(state+0xce),(double)*(float*)(state+0xd2),(double)*(float*)(state+0xd6));
    }
}

// samp.dll RVA: 0x000a97c0
void CObject::Move(CVector position,float speed,CVector rotation) {
    BYTE *state=(BYTE*)this;CMatrix matrix;GetMatrix(&matrix);
    if(state[0x1af]&1) {
        Stop();memcpy(&matrix.position,state+0x10b,12);
        if(state[0x1b5])GameQuaternionToMatrix((GameQuaternion*)(state+0x167),&matrix);
        ApplyMatrix(matrix);
    }
    DWORD tick=RakNet::GetTime();*(DWORD*)(state+0x15f)=tick;
    memcpy(state+0x10b,&position.x,4);memcpy(state+0x1b0,&speed,4);state[0x1af]|=1;
    memcpy(state+0x10f,&position.y,4);memcpy(state+0x113,&position.z,4);
    if(rotation.x>g_objectMotionRotationSentinel&&rotation.y>g_objectMotionRotationSentinel&&rotation.z>g_objectMotionRotationSentinel) {
        state[0x1b5]=1;float x,y,z;GetEulerInverted(&x,&y,&z);
        *(float*)(state+0x197)=GameNormalizeAngle(rotation.y);
        *(float*)(state+0x19b)=GameNormalizeAngle(rotation.x);
        *(float*)(state+0x19f)=GameNormalizeAngle(rotation.z);
        *(float*)(state+0x1a3)=GetAngleDifference(x,*(float*)(state+0x197));
        *(float*)(state+0x1a7)=GetAngleDifference(y,*(float*)(state+0x19b));
        *(float*)(state+0x1ab)=GetAngleDifference(z,*(float*)(state+0x19f));
        Rotate(rotation);CMatrix rotated;GetMatrix(&rotated);
        GameQuaternionFromMatrix(&rotated,(GameQuaternion*)(state+0x187));
        GameQuaternionFromMatrix((CMatrix*)(state+0xdb),(GameQuaternion*)(state+0x167));
        GameQuaternionNormalize((GameQuaternion*)(state+0x187));GameQuaternionNormalize((GameQuaternion*)(state+0x167));
    }else state[0x1b5]=0;
    *(float*)(state+0x15b)=GetDistanceToPoint(*(float*)(state+0x10b),*(float*)(state+0x10f),*(float*)(state+0x113));
    if(g_pNetGame){BYTE*pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);BYTE*players=*(BYTE**)(pools+8);if(players)(*(CLocalPlayer**)(players+0x1e))->ProcessSurfing();}
    ClearBackendFlag8();
}

#include "gui/device_lifecycle.h"

// samp.dll RVA: 0x000a8870
bool CObject::GetMaterialSize(int code,int *width,int *height) {
    switch(code) {
    case 10:*width=32;*height=32;return true;
    case 20:*width=64;*height=32;return true;
    case 30:*width=64;*height=64;return true;
    case 40:*width=128;*height=32;return true;
    case 50:*width=128;*height=64;return true;
    case 60:*width=128;*height=128;return true;
    case 70:*width=256;*height=32;return true;
    case 80:*width=256;*height=64;return true;
    case 90:*width=256;*height=128;return true;
    case 100:*width=256;*height=256;return true;
    case 110:*width=512;*height=64;return true;
    case 120:*width=512;*height=128;return true;
    case 130:*width=512;*height=256;return true;
    case 140:*width=512;*height=512;return true;
    default:return false;
    }
}
// samp.dll RVA: 0x000a9ad0
void CObject::ConstructMaterialText() {
    BYTE *state=(BYTE*)this;
    for(int i=0;i<16;i++) {
        BYTE *type=state+0x2db+i*4;
        BYTE *record=state+0x431+i*0xd7;
        if(*(DWORD*)(type-0xc4) && *(DWORD*)type==2 && !*(DWORD*)(type+0x40)) {
            int width,height;
            if(GetMaterialSize(record[-0x4c],&width,&height)) {
                IDirect3DTexture9 *created=g_deviceMaterialText->Create(*(char**)(type+0xdf0),(char*)record-0x4b,record[-10],width,height,*(DWORD*)(record-8),*(DWORD*)(record-4),record[-9],*record);
                *(IDirect3DTexture9**)(type+0xe70)=created;
                *(DWORD*)(type+0x40)=1;
            }
        }
    }
}
// samp.dll RVA: 0x000a8ac0
void CObject::ShutdownMaterialText() {
    BYTE *state=(BYTE*)this;
    for(int i=0;i<16;i++) {
        IDirect3DTexture9 **texture=(IDirect3DTexture9**)(state+0x114b+i*4);
        if(*texture) {
            (*texture)->Release();*texture=0;
            *(DWORD*)(state+0x31b+i*4)=0;
        }
    }
}

#include "game/game.h"
// samp.dll RVA: 0x000a7b40
bool CObject::IsTimeVisible() {
    BYTE *state=(BYTE*)this;BYTE hour=0,minute=0;
    if(!state[0x1199])return false;
    if(g_pGame)g_pGame->GetWorldTime(&hour,&minute);
    BYTE first=state[0x119a],last=state[0x119b];
    if(last>first)return hour>=first && hour<last;
    if(last<first)return hour>=first || hour<last;
    return false;
}
// samp.dll RVA: 0x000a8b90
void CObject::ClearModelFlag0() {
    if(m_pGameEntity && *(DWORD*)m_pGameEntity!=0x863c40) {
        BYTE *info=(BYTE*)GameGetModelInfo(*(int*)((BYTE*)this+0x4e));
        if(info)*(DWORD*)(info+0x12)&=0xfffffffe;
    }
}
// samp.dll RVA: 0x000a9150
void CObject::ProcessStreaming() {
    BYTE *state=(BYTE*)this;state[0x49]=0;
    bool timeHidden=state[0x1199] && !IsTimeVisible();
    bool outside;
    // Native caches a float but compares the unspilled ST0 result.
    __asm {
        mov ecx,state
        call CEntity::GetDistanceToCamera
        mov edx,state
        fst dword ptr [edx+0x68]
        fcomp dword ptr [edx+0x54]
        fnstsw ax
        test ah,0x41
        sete outside
    }
    if(!outside && !timeHidden) {
        if(!GetRwObject()) {
            if(!g_pGame->IsModelAvailable(*(int*)(state+0x4e))) {
                g_pGame->RequestModel(*(int*)(state+0x4e),2);return;
            }
            SetModelIndex(*(int*)(state+0x4e));
        }
        if(GetRwObject()) {
            state[0x49]=1;*(DWORD*)(state+0x4a)=0;ClearModelFlag0();
        }
    } else if(GetRwObject()) {
        DWORD count=++*(DWORD*)(state+0x4a);
        if(count==1000) {
            DeleteRwObject();*(DWORD*)(state+0x4a)=0;
            int model=*(int*)(state+0x4e);
            if(model>0 && !GameGetModelReferenceCount(model))
                g_pGame->ReleaseModel(*(int*)(state+0x4e),0);
        }
    }
}

#include "game/playerped.h"
#include "net/pool_frame.h"
// samp.dll RVA: 0x00118200
extern const float g_objectStreamingNearDistance=300.0f;
// samp.dll RVA: 0x00117878
extern const float g_objectStreamingVehicleMargin=30.0f;
// samp.dll RVA: 0x001181b0
extern const float g_objectTurnVelocityScale=0.01f;
// samp.dll RVA: 0x0011ee78
extern const float g_objectNegativeTurnVelocityLimit=-0.001f;
// samp.dll RVA: 0x000a9240
void CObject::Process(float elapsedSeconds) {
    BYTE *state=(BYTE*)this;
    if(!*(DWORD*)(state+0x118b) && !(*(float*)(state+0x54)>g_objectStreamingNearDistance) && !state[0x1199]) {
        if(!GetRwObject() && g_pGame->GetPlayerPed()->IsInVehicle()) {
            bool within;
            __asm {
                mov ecx,state
                call CEntity::GetDistanceToCamera
                mov edx,state
                fld dword ptr [edx+0x54]
                fsub g_objectStreamingVehicleMargin
                fcompp
                fnstsw ax
                test ah,0x41
                sete within
            }
            if(within && !g_pGame->IsModelAvailable(*(int*)(state+0x4e)))
                g_pGame->RequestModel(*(int*)(state+0x4e),2);
        }
    } else ProcessStreaming();
    WORD vehicle=*(WORD*)(state+0xbe);
    if(vehicle!=0xffff) {
        if(!g_pNetGame)return;
        BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de),*pool=*(BYTE**)(pools+0xc);
        if(!pool || vehicle>=2000 || !*(DWORD*)(pool+0x3074+vehicle*4))return;
        CVehicle *attached=*(CVehicle**)(pool+0x1134+vehicle*4);
        if(attached && ((CEntity*)attached)->DoesExist())AttachToVehicle(attached);
        return;
    }
    WORD object=*(WORD*)(state+0xc0);
    if(object!=0xffff) {
        if(!g_pNetGame)return;
        BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);CObjectPool *pool=*(CObjectPool**)(pools+0x14);
        if(!pool)return;CObject *attached=(CObject*)pool->Get(object);
        if(attached)AttachToObject(attached);return;
    }
    if(!(state[0x1af]&1))return;
    CVector velocity={0,0,0};CMatrix matrix;GetMatrix(&matrix);
    float step=elapsedSeconds**(float*)(state+0x1b0);
    float remaining=GetDistanceToTarget(&matrix);
    DWORD now=RakNet::GetTime();int elapsedTicks=(int)(now-*(DWORD*)(state+0x15f));
    float clockDistance;
    __asm {
        fild elapsedTicks
        fmul g_objectPoolSecondsPerTick
        mov eax,now
        mov edx,state
        mov dword ptr [edx+0x163],eax
        fmul dword ptr [edx+0x1b0]
        fstp clockDistance
    }
    CVector old=matrix.position;
    float travelled=*(float*)(state+0x15b)-remaining;
    if(step>=remaining) {
        SetSpeed(velocity);SetTurnSpeed(velocity);memcpy(&matrix.position,state+0x10b,12);
        if(state[0x1b5])GameQuaternionToMatrix((GameQuaternion*)(state+0x167),&matrix);
        ApplyMatrix(matrix);Stop();return;
    }
    if(!(elapsedSeconds>0.0f))return;
    float remainingFraction,progress;CMatrix *moving=&matrix;CVector *previous=&old;
    // Native retains reciprocal/dx intermediates in ST0 while spilling dy/dz.
    __asm {
        mov esi,state
        mov edx,moving
        fld remaining
        fdiv step
        fdivr g_quaternionMatrixOne
        fld dword ptr [esi+0x10b]
        fsub dword ptr [edx+0x30]
        fmul st,st(1)
        fld dword ptr [esi+0x10f]
        fsub dword ptr [edx+0x34]
        fmul st,st(2)
        fstp velocity.y
        fld dword ptr [esi+0x113]
        fsub dword ptr [edx+0x38]
        fmul st,st(2)
        fstp velocity.z
        fadd dword ptr [edx+0x30]
        fstp dword ptr [edx+0x30]
        fstp st
        fld velocity.y
        fadd dword ptr [edx+0x34]
        fstp dword ptr [edx+0x34]
        fld velocity.z
        fadd dword ptr [edx+0x38]
        fstp dword ptr [edx+0x38]
        fld remaining
        fdiv dword ptr [esi+0x15b]
        fld g_quaternionMatrixOne
        fsub st,st(1)
        fstp progress
        fstp remainingFraction
        mov eax,previous
        fld g_quaternionMatrixOne
        fdiv elapsedSeconds
        fld dword ptr [edx+0x30]
        fsub dword ptr [eax]
        fmul st,st(1)
        fld dword ptr [edx+0x34]
        fsub dword ptr [eax+4]
        fmul st,st(2)
        fstp velocity.y
        fld dword ptr [edx+0x38]
        fsub dword ptr [eax+8]
        fmul st,st(2)
        fstp velocity.z
        fmul g_pedDrunkShakeScale
        fstp velocity.x
        fstp st
        fld velocity.y
        fmul g_pedDrunkShakeScale
        fstp velocity.y
        fld velocity.z
        fmul g_pedDrunkShakeScale
        fstp velocity.z
    }
    bool correct;
    __asm {
        push travelled
        push clockDistance
        call FloatOffset
        add esp,8
        fcomp g_objectMutationStep
        fnstsw ax
        test ah,0x41
        sete correct
    }
    if(correct && (clockDistance>travelled || travelled>clockDistance)) {
        if(clockDistance>travelled) {
            __asm {
                fld clockDistance
                fsub travelled
                fmul g_objectMutationStep
                fadd g_quaternionMatrixOne
            }
        } else {
            __asm {
                fld travelled
                fsub clockDistance
                fmul g_objectMutationStep
                fsubr g_quaternionMatrixOne
            }
        }
        __asm {
            fld velocity.x
            fmul st,st(1)
            fstp velocity.x
            fld velocity.y
            fmul st,st(1)
            fstp velocity.y
            fmul velocity.z
            fstp velocity.z
        }
    }
    SetSpeed(velocity);ApplyLinearSpeed();CMatrix current;
    if(state[0x1b5]) {
        float x,y,z;GetEulerInverted(&x,&y,&z);
        float desired=*(float*)(state+0x19f)-remainingFraction**(float*)(state+0x1ab);
        CVector turn={0,0,GetAngleDifference(z,desired)*g_objectTurnVelocityScale};
        if(turn.z>g_objectPoolSecondsPerTick)turn.z=g_objectPoolSecondsPerTick;
        else if(turn.z<g_objectNegativeTurnVelocityLimit)turn.z=g_objectNegativeTurnVelocityLimit;
        SetTurnSpeed(turn);GetMatrix(&current);
        GameQuaternionSlerp((GameQuaternion*)(state+0x177),(GameQuaternion*)(state+0x187),
            (GameQuaternion*)(state+0x167),progress);
        GameQuaternionNormalize((GameQuaternion*)(state+0x177));
        GameQuaternionToMatrix((GameQuaternion*)(state+0x177),&current);
    } else GetMatrix(&current);
    ApplyMatrix(current);
}

#include "gui/textdraw_texture.h"
#include "d3dhook/texture_binding.h"
#include <stdlib.h>
// samp.dll RVA: 0x001e0498
void *g_objectSharedMaterialTexture=0;
// samp.dll RVA: 0x0011ee4c
extern const Scripting::OpcodeInfo g_objectDestroyCommand={0x108,"i"};
// samp.dll RVA: 0x000a79a0
void CObject::Add() {
    // GTA SA virtual address: excluded backend vtable 0x00863c40.
    if(m_pGameEntity && *(DWORD*)m_pGameEntity!=0x863c40)GameWorldAdd(m_pGameEntity);
}
// samp.dll RVA: 0x000a79f0
void CObject::Remove() {
    // GTA SA virtual address: excluded backend vtable 0x00863c40.
    if(m_pGameEntity && *(DWORD*)m_pGameEntity!=0x863c40)GameWorldRemove(m_pGameEntity);
}
// samp.dll RVA: 0x000a9020
CObject::~CObject() {
    BYTE *state=(BYTE*)this;m_pGameEntity=GamePoolObjectGetAt(m_nHandle);
    // Native deliberately skips resource cleanup with no valid backend or on quit.
    if(!m_pGameEntity || *(DWORD*)m_pGameEntity==0x863c40 || g_clientQuitRequested)return;
    if(GetRwObject())DeleteRwObject();
    Scripting::ProcessCommand(&g_objectDestroyCommand,m_nHandle);
    int model=*(int*)(state+0x4e);
    if(model<0 || !GameGetModelReferenceCount(model))g_pGame->ReleaseModel(*(int*)(state+0x4e),0);
    if(*(DWORD*)(state+0x118b)) {
        ShutdownMaterialText();
        for(int i=0;i<16;++i) {
            void **texture=(void**)(state+0x217+i*4);
            if(*texture && *texture!=g_objectSharedMaterialTexture) {
                GameDestroyTextDrawTexture(*texture);*texture=0;
                if(*(DWORD*)(state+0x110b+i*4))*(DWORD*)(state+0x110b+i*4)=0;
            }
            void **text=(void**)(state+0x10cb+i*4);
            if(*text){free(*text);*text=0;}
        }
        if(g_deviceMaterialOverrides==state) {
            g_deviceMaterialOverrides=0;g_deviceMaterialOverrideEnabled=0;
        }
    }
}

#include "game/custom_model_io.h"
// samp.dll RVA: 0x0011ede8
extern const Scripting::OpcodeInfo g_objectCreateCommand={0x107,"ifffv"};
// samp.dll RVA: 0x000a8bc0
void CObject::UpdateCachedCentre() {
    // GTA SA virtual address: excluded backend vtable 0x00863c40.
    if(m_pGameEntity && *(DWORD*)m_pGameEntity!=0x863c40) {
        CMatrix matrix;GetMatrix(&matrix);CVector centre;
        if(GameGetModelCollisionCentre(*(int*)((BYTE*)this+0x4e),&centre))
            GameTransformPoint((CVector*)((BYTE*)this+0x5c),&matrix,&centre);
        else memcpy((BYTE*)this+0x5c,&matrix.position,12);
    }
}
// samp.dll RVA: 0x000a8d00
CObject::CObject(int model,CVector position,CVector rotation,float drawDistance,
    BYTE flag1193,BYTE firstHour,BYTE lastHour):CEntity() {
    BYTE *state=(BYTE*)this;DWORD handle=0;
    *(WORD*)(state+0xbe)=*(WORD*)(state+0xc0)=0xffff;
    state[0x1193]=flag1193;m_pGameEntity=0;m_nHandle=0;
    memset(state+0xc2,0,24);state[0xda]=1;state[0x1b4]=0;state[0x1194]=0;
    state[0xb9]=0;*(DWORD*)(state+0xba)=0;state[0x53]=0;
    state[0x1199]=state[0x119a]=state[0x119b]=0;
    if(firstHour || lastHour){state[0x1199]=1;state[0x119a]=firstHour;state[0x119b]=lastHour;}
    if(!GameModelQueryB4170(model))model=18631;
    void *info=GameGetModelInfo(model);state[0x6c]=(GameGetModelFlagsByte(model)>>1)&1;
    if(drawDistance!=g_objectScaleLowerBound && info && GameIsObjectModelInfo(info))
        GameSetModelDrawDistance(info,drawDistance);
    if(model<0) {
        // GTA SA virtual address: native negative-model create patch.
        GameUnprotect((void*)0x46977f,1);*(BYTE*)0x46977f=0x75;
    }
    Scripting::ProcessCommand(&g_objectCreateCommand,model,(double)position.x,
        (double)position.y,(double)position.z,&handle);
    if(model<0)*(BYTE*)0x46977f=0x7d;
    if(handle && GamePoolObjectGetAt(handle)) {
        m_pGameEntity=GamePoolObjectGetAt(handle);m_nHandle=handle;
        state[0x48]=1;state[0x1af]=0;*(DWORD*)(state+0x1b0)=0;
        state[0x1b6]=0;*(DWORD*)(state+0x163)=0;*(int*)(state+0x4e)=model;
        if(GameIsObjectModelInfo(info)){state[0x52]=1;*(float*)(state+0x54)=GameGetModelDrawDistance(info);}
        else{state[0x52]=0;memcpy(state+0x54,&drawDistance,4);}
        GetMatrix((CMatrix*)(state+0x11b));memcpy(state+0x14b,&position,12);
        SetMatrix(*(CMatrix*)(state+0x11b));SetRotation(&rotation);SetBackendHighFlag();
        state[0x49]=0;*(DWORD*)(state+0x4a)=0;
        *(float*)(state+0x58)=GameGetModelCollisionRadius(model);UpdateCachedCentre();
    }
    *(DWORD*)(state+0x118f)=0;*(DWORD*)(state+0x1c3)=0;*(DWORD*)(state+0x118b)=0;
    *(DWORD*)(state+0x1c7)=0;*(DWORD*)(state+0x1d3)=0;*(DWORD*)(state+0x1cb)=0;
    *(DWORD*)(state+0x1cf)=0;*(DWORD*)(state+0x2d7)=0;*(DWORD*)(state+0x1195)=0;
    for(int i=0;i<16;++i) {
        *(DWORD*)(state+0x1d7+i*4)=0;*(DWORD*)(state+0x217+i*4)=0;
        *(DWORD*)(state+0x257+i*4)=0;*(DWORD*)(state+0x297+i*4)=0;
        *(DWORD*)(state+0x2db+i*4)=0;*(DWORD*)(state+0x31b+i*4)=0;
        memset(state+0x35b+i*0xd7,0,0xd7);
        *(DWORD*)(state+0x10cb+i*4)=0;*(DWORD*)(state+0x110b+i*4)=0;*(DWORD*)(state+0x114b+i*4)=0;
    }
}

// samp.dll RVA: 0x000a79c0
BOOL CObject::IsAdded() {
    // GTA SA virtual address: excluded backend vtable 0x00863c40.
    return m_pGameEntity && *(DWORD*)m_pGameEntity!=0x863c40 &&
        *(DWORD*)((BYTE*)m_pGameEntity+0x18) && ((BYTE*)this)[0x48]?TRUE:FALSE;
}
// samp.dll RVA: 0x000a8c30
void CObject::NativeA8C30() {
    // GTA SA virtual address: excluded backend vtable 0x00863c40.
    if(m_pGameEntity && *(DWORD*)m_pGameEntity!=0x863c40) {
        *(DWORD*)((BYTE*)m_pGameEntity+0x1c)|=0x10000000;
        void *backend=m_pGameEntity;DWORD *table=*(DWORD**)backend;
        __asm {mov ecx,backend}
        __asm {mov eax,table}
        __asm {call dword ptr [eax+0x4c]}
        ((BYTE*)this)[0x1194]=1;
    }
}
// samp.dll RVA: 0x000a8c70
void CObject::NativeA8C70() {
    // GTA SA virtual address: excluded backend vtable 0x00863c40.
    if(m_pGameEntity && *(DWORD*)m_pGameEntity!=0x863c40 && ((BYTE*)this)[0x1194]) {
        void *backend=m_pGameEntity;DWORD *table=*(DWORD**)backend;
        __asm {push 1}
        __asm {mov ecx,backend}
        __asm {mov eax,table}
        __asm {call dword ptr [eax+0x50]}
        ((BYTE*)this)[0x1194]=0;
    }
}
// samp.dll RVA: 0x000a8cc0
bool CObject::HasRwFrameFlag8() {
    // GTA SA virtual address: excluded backend vtable 0x00863c40.
    if(!m_pGameEntity || *(DWORD*)m_pGameEntity==0x863c40)return false;
    BYTE *rw=(BYTE*)GetRwObject();if(!rw)return false;
    BYTE *frame=*(BYTE**)(rw+4);if(!frame || *frame!=1)return false;
    BYTE *data=*(BYTE**)(frame+0x18);return data && (data[8]&8)!=0;
}

// samp.dll RVA: 0x000a7d00
void CObject::SetAttachedVehicle(WORD id,const CVector *offset,const CVector *rotation) {
    BYTE *state=(BYTE*)this;*(WORD*)(state+0xbe)=id;
    if(id==0xffff){for(int i=0;i<6;++i)*(DWORD*)(state+0xc2+i*4)=0;return;}
    for(int i=0;i<3;++i)*(DWORD*)(state+0xc2+i*4)=*(const DWORD*)((const BYTE*)offset+i*4);
    for(int i=0;i<3;++i)*(DWORD*)(state+0xce+i*4)=*(const DWORD*)((const BYTE*)rotation+i*4);
}
// samp.dll RVA: 0x000a7d90
void CObject::SetAttachedObject(WORD id,const CVector *offset,const CVector *rotation,BYTE flag) {
    BYTE *state=(BYTE*)this;*(WORD*)(state+0xc0)=id;
    if(id==0xffff) {
        for(int i=0;i<5;++i)*(DWORD*)(state+0xc2+i*4)=0;
        state[0xda]=1;*(DWORD*)(state+0xd6)=0;return;
    }
    for(int i=0;i<3;++i)*(DWORD*)(state+0xc2+i*4)=*(const DWORD*)((const BYTE*)offset+i*4);
    for(int i=0;i<2;++i)*(DWORD*)(state+0xce+i*4)=*(const DWORD*)((const BYTE*)rotation+i*4);
    DWORD last=*(const DWORD*)((const BYTE*)rotation+8);state[0xda]=flag;*(DWORD*)(state+0xd6)=last;
}
// samp.dll RVA: 0x000a80b0
BOOL CObject::IsMoving() {
    BYTE *state=(BYTE*)this;WORD object=*(WORD*)(state+0xc0);
    if(object!=0xffff) {
        if(!g_pNetGame)return FALSE;
        BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de),*pool=*(BYTE**)(pools+0x14);
        // Native accepts id2100 as well; the adjacent-state layout is unproven.
        if(!pool || object>2100 || !*(DWORD*)(pool+4+object*4))return FALSE;
        BYTE *attached=*(BYTE**)(pool+0x20d4+object*4);
        return attached && (attached[0x1af]&1)?TRUE:FALSE;
    }
    if(*(WORD*)(state+0xbe)!=0xffff)return TRUE;
    return state[0x1af]&1;
}
