#include "game/object.h"
#include <string.h>
// samp.dll RVA: 0x000715c0
CVector *CObject::GetRotation() {return &eulerRotation;}
// samp.dll RVA: 0x00017750
void CObject::DisableCameraCollision() {cameraCollisionDisabled=1;}
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
        memcpy(&eulerRotation.x,&rotation->x,4);memcpy(&eulerRotation.y,&rotation->y,4);memcpy(&eulerRotation.z,&rotation->z,4);
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
void CObject::Stop(){CVector zero={0,0,0};SetSpeed(zero);SetTurnSpeed(zero);motionFlags&=0xfe;}
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
        Scripting::ProcessCommand(&g_objectAttachVehicleCommand,m_nHandle,((CEntity*)vehicle)->m_nHandle,
            (double)attachmentOffset.x,(double)attachmentOffset.y,(double)attachmentOffset.z,
            (double)attachmentRotation.x,(double)attachmentRotation.y,(double)attachmentRotation.z);
    }
}
// samp.dll RVA: 0x000a7eb0
void CObject::AttachToObject(CObject *object) {
    if(!Scripting::ProcessCommand(&g_objectIsAttachedCommand,m_nHandle)) {
        Scripting::ProcessCommand(&g_objectAttachObjectCommand,m_nHandle,object->m_nHandle,
            (double)attachmentOffset.x,(double)attachmentOffset.y,(double)attachmentOffset.z,
            (double)attachmentRotation.x,(double)attachmentRotation.y,(double)attachmentRotation.z);
    }
}

// samp.dll RVA: 0x000a97c0
void CObject::Move(CVector position,float speed,CVector rotation) {
    CMatrix matrix;GetMatrix(&matrix);
    if(motionFlags&1) {
        Stop();memcpy(&matrix.position,&targetMatrix.position,12);
        if(rotationEnabled)GameQuaternionToMatrix(&targetQuaternion,&matrix);
        ApplyMatrix(matrix);
    }
    DWORD tick=RakNet::GetTime();moveStartTick=tick;
    memcpy(&targetMatrix.position.x,&position.x,4);memcpy(&moveSpeed,&speed,4);motionFlags|=1;
    memcpy(&targetMatrix.position.y,&position.y,4);memcpy(&targetMatrix.position.z,&position.z,4);
    if(rotation.x>g_objectMotionRotationSentinel&&rotation.y>g_objectMotionRotationSentinel&&rotation.z>g_objectMotionRotationSentinel) {
        rotationEnabled=1;float x,y,z;GetEulerInverted(&x,&y,&z);
        targetEuler.x=GameNormalizeAngle(rotation.y);
        targetEuler.y=GameNormalizeAngle(rotation.x);
        targetEuler.z=GameNormalizeAngle(rotation.z);
        angleDelta.x=GetAngleDifference(x,targetEuler.x);
        angleDelta.y=GetAngleDifference(y,targetEuler.y);
        angleDelta.z=GetAngleDifference(z,targetEuler.z);
        Rotate(rotation);CMatrix rotated;GetMatrix(&rotated);
        GameQuaternionFromMatrix(&rotated,&startQuaternion);
        GameQuaternionFromMatrix(&targetMatrix,&targetQuaternion);
        GameQuaternionNormalize(&startQuaternion);GameQuaternionNormalize(&targetQuaternion);
    }else rotationEnabled=0;
    moveDistance=GetDistanceToPoint(targetMatrix.position.x,targetMatrix.position.y,targetMatrix.position.z);
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
    for(int i=0;i<16;++i) {
        ObjectMaterialTextInfo &info=materials.textInfo[i];
        if(materials.textures[i] && materials.types[i]==2 && !materials.textProcessed[i]) {
            int width,height;
            if(GetMaterialSize(info.size,&width,&height)) {
                materials.renderedText[i]=g_deviceMaterialText->Create(materials.text[i],info.font,
                    info.fontSize,width,height,info.fontColor,info.backgroundColor,info.bold,info.alignment);
                // Native records the attempt even if Create returned NULL.
                materials.textProcessed[i]=1;
            }
        }
    }
}
// samp.dll RVA: 0x000a8ac0
void CObject::ShutdownMaterialText() {
    for(int i=0;i<16;++i) {
        IDirect3DTexture9 *&texture=materials.renderedText[i];
        if(texture) {
            texture->Release();texture=0;
            materials.textProcessed[i]=0;
        }
    }
}

#include "game/game.h"
// samp.dll RVA: 0x000a7b40
bool CObject::IsTimeVisible() {
    BYTE hour=0,minute=0;
    if(!timeVisibilityEnabled)return false;
    if(g_pGame)g_pGame->GetWorldTime(&hour,&minute);
    BYTE first=visibilityStartHour,last=visibilityEndHour;
    if(last>first)return hour>=first && hour<last;
    if(last<first)return hour>=first || hour<last;
    return false;
}
// samp.dll RVA: 0x000a8b90
void CObject::ClearModelFlag0() {
    if(m_pGameEntity && *(DWORD*)m_pGameEntity!=0x863c40) {
        BYTE *info=(BYTE*)GameGetModelInfo(modelIndex);
        if(info)*(DWORD*)(info+0x12)&=0xfffffffe;
    }
}
// samp.dll RVA: 0x000a9150
void CObject::ProcessStreaming() {
    BYTE *state=(BYTE*)this;renderReady=0;
    bool timeHidden=timeVisibilityEnabled && !IsTimeVisible();
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
            if(!g_pGame->IsModelAvailable(modelIndex)) {
                g_pGame->RequestModel(modelIndex,2);return;
            }
            SetModelIndex(modelIndex);
        }
        if(GetRwObject()) {
            renderReady=1;streamingHideCount=0;ClearModelFlag0();
        }
    } else if(GetRwObject()) {
        DWORD count=++streamingHideCount;
        if(count==1000) {
            DeleteRwObject();streamingHideCount=0;
            int model=modelIndex;
            if(model>0 && !GameGetModelReferenceCount(model))
                g_pGame->ReleaseModel(modelIndex,0);
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
    if(!hasMaterials && !(streamingDistance>g_objectStreamingNearDistance) && !timeVisibilityEnabled) {
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
            if(within && !g_pGame->IsModelAvailable(modelIndex))
                g_pGame->RequestModel(modelIndex,2);
        }
    } else ProcessStreaming();
    WORD vehicle=attachedVehicle;
    if(vehicle!=0xffff) {
        if(!g_pNetGame)return;
        BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de),*pool=*(BYTE**)(pools+0xc);
        if(!pool || vehicle>=2000 || !*(DWORD*)(pool+0x3074+vehicle*4))return;
        CVehicle *attached=*(CVehicle**)(pool+0x1134+vehicle*4);
        if(attached && ((CEntity*)attached)->DoesExist())AttachToVehicle(attached);
        return;
    }
    WORD object=attachedObject;
    if(object!=0xffff) {
        if(!g_pNetGame)return;
        BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);CObjectPool *pool=*(CObjectPool**)(pools+0x14);
        if(!pool)return;CObject *attached=(CObject*)pool->Get(object);
        if(attached)AttachToObject(attached);return;
    }
    if(!(motionFlags&1))return;
    CVector velocity={0,0,0};CMatrix matrix;GetMatrix(&matrix);
    float step=elapsedSeconds*moveSpeed;
    float remaining=GetDistanceToTarget(&matrix);
    DWORD now=RakNet::GetTime();int elapsedTicks=(int)(now-moveStartTick);
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
    float travelled=moveDistance-remaining;
    if(step>=remaining) {
        SetSpeed(velocity);SetTurnSpeed(velocity);memcpy(&matrix.position,&targetMatrix.position,12);
        if(rotationEnabled)GameQuaternionToMatrix(&targetQuaternion,&matrix);
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
        // Native A9571 runs with two deferred FloatOffset arguments on stack:
        // its [esp+28] is the remaining distance, not travelled distance.
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
    if(rotationEnabled) {
        float x,y,z;GetEulerInverted(&x,&y,&z);
        float desired=targetEuler.z-remainingFraction*angleDelta.z;
        CVector turn={0,0,GetAngleDifference(z,desired)*g_objectTurnVelocityScale};
        if(turn.z>g_objectPoolSecondsPerTick)turn.z=g_objectPoolSecondsPerTick;
        else if(turn.z<g_objectNegativeTurnVelocityLimit)turn.z=g_objectNegativeTurnVelocityLimit;
        SetTurnSpeed(turn);GetMatrix(&current);
        GameQuaternionSlerp(&interpolatedQuaternion,&startQuaternion,
            &targetQuaternion,progress);
        GameQuaternionNormalize(&interpolatedQuaternion);
        GameQuaternionToMatrix(&interpolatedQuaternion,&current);
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
    int model=modelIndex;
    if(model<0 || !GameGetModelReferenceCount(model))g_pGame->ReleaseModel(modelIndex,0);
    if(hasMaterials) {
        ShutdownMaterialText();
        for(int i=0;i<16;++i) {
            void **texture=&materials.textures[i];
            if(*texture && *texture!=g_objectSharedMaterialTexture) {
                GameDestroyTextDrawTexture(*texture);*texture=0;
                if(materials.rasterField34[i])materials.rasterField34[i]=0;
            }
            char **text=&materials.text[i];
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
        if(GameGetModelCollisionCentre(modelIndex,&centre))
            GameTransformPoint(&worldCentre,&matrix,&centre);
        else memcpy(&worldCentre,&matrix.position,12);
    }
}
// samp.dll RVA: 0x000a8d00
CObject::CObject(int model,CVector position,CVector rotation,float drawDistance,
    BYTE flag1193,BYTE firstHour,BYTE lastHour):CEntity() {
    BYTE *state=(BYTE*)this;DWORD handle=0;
    attachedVehicle=attachedObject=0xffff;
    state[0x1193]=flag1193;m_pGameEntity=0;m_nHandle=0;
    memset(&attachmentOffset,0,24);attachmentRotationSync=1;state[0x1b4]=0;state[0x1194]=0;
    state[0xb9]=0;*(DWORD*)(state+0xba)=0;state[0x53]=0;
    timeVisibilityEnabled=visibilityStartHour=visibilityEndHour=0;
    if(firstHour || lastHour){timeVisibilityEnabled=1;visibilityStartHour=firstHour;visibilityEndHour=lastHour;}
    if(!GameModelQueryB4170(model))model=18631;
    void *info=GameGetModelInfo(model);drawLast=(GameGetModelFlagsByte(model)>>1)&1;
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
        state[0x48]=1;motionFlags=0;moveSpeed=0.0f;
        state[0x1b6]=0;lastMoveTick=0;modelIndex=model;
        if(GameIsObjectModelInfo(info)){usesModelDrawDistance=1;streamingDistance=GameGetModelDrawDistance(info);}
        else{usesModelDrawDistance=0;memcpy(&streamingDistance,&drawDistance,4);}
        GetMatrix(&baseMatrix);memcpy(&baseMatrix.position,&position,12);
        SetMatrix(baseMatrix);SetRotation(&rotation);SetBackendHighFlag();
        renderReady=0;streamingHideCount=0;
        collisionRadius=GameGetModelCollisionRadius(model);UpdateCachedCentre();
    }
    materialPassState=0;materials.frameObjectsProcessed=0;hasMaterials=0;
    materials.snapshotValid=0;materials.rwObject=0;materials.texturesChanged=0;
    materials.colorsChanged=0;materials.savedGeometryFlags=0;*(DWORD*)(state+0x1195)=0;
    for(int i=0;i<16;++i) {
        materials.savedTextures[i]=0;materials.textures[i]=0;
        materials.colors[i]=0;materials.savedColors[i]=0;
        materials.types[i]=0;materials.textProcessed[i]=0;
        memset(&materials.textInfo[i],0,sizeof(materials.textInfo[i]));
        materials.text[i]=0;materials.rasterField34[i]=0;materials.renderedText[i]=0;
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
    attachedVehicle=id;
    if(id==0xffff){for(int i=0;i<6;++i)((DWORD*)&attachmentOffset)[i]=0;return;}
    for(int i=0;i<3;++i)((DWORD*)&attachmentOffset)[i]=*(const DWORD*)((const BYTE*)offset+i*4);
    for(int i=0;i<3;++i)((DWORD*)&attachmentRotation)[i]=*(const DWORD*)((const BYTE*)rotation+i*4);
}
// samp.dll RVA: 0x000a7d90
void CObject::SetAttachedObject(WORD id,const CVector *offset,const CVector *rotation,BYTE flag) {
    attachedObject=id;
    if(id==0xffff) {
        for(int i=0;i<5;++i)((DWORD*)&attachmentOffset)[i]=0;
        attachmentRotationSync=1;*(DWORD*)&attachmentRotation.z=0;return;
    }
    for(int i=0;i<3;++i)((DWORD*)&attachmentOffset)[i]=*(const DWORD*)((const BYTE*)offset+i*4);
    for(int i=0;i<2;++i)((DWORD*)&attachmentRotation)[i]=*(const DWORD*)((const BYTE*)rotation+i*4);
    DWORD last=*(const DWORD*)((const BYTE*)rotation+8);attachmentRotationSync=flag;*(DWORD*)&attachmentRotation.z=last;
}
// samp.dll RVA: 0x000a80b0
BOOL CObject::IsMoving() {
    WORD object=attachedObject;
    if(object!=0xffff) {
        if(!g_pNetGame)return FALSE;
        BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de),*pool=*(BYTE**)(pools+0x14);
        // Native accepts id2100 as well; the adjacent-state layout is unproven.
        if(!pool || object>2100 || !*(DWORD*)(pool+4+object*4))return FALSE;
        BYTE *attached=*(BYTE**)(pool+0x20d4+object*4);
        return attached && (((CObject*)attached)->motionFlags&1)?TRUE:FALSE;
    }
    if(attachedVehicle!=0xffff)return TRUE;
    return motionFlags&1;
}

// samp.dll RVA: 0x000a8430
void *__cdecl ObjectApplyMaterials(void *atomic,void *object) {
    BYTE *a=(BYTE*)atomic,*state=(BYTE*)object;
    if(*a!=1 || !*(void**)(a+0x18) || !state)return atomic;
    BYTE *geometry=*(BYTE**)(a+0x18);int count=*(int*)(geometry+0x24);if(count>16)count=16;
    for(int i=0;i<count;++i) {
        BYTE *material=(*(BYTE***)(*(BYTE**)(a+0x18)+0x20))[i];if(!material)continue;
        DWORD texture=*(DWORD*)(state+0x217+i*4);
        if(texture){*(DWORD*)material=texture;*(DWORD*)(state+0x1cb)=1;}
        if(*(DWORD*)(state+0x257+i*4)) {
            *(DWORD*)(*(BYTE**)(a+0x18)+8)|=0x40;
            *(DWORD*)(*(BYTE**)(a+0x18)+8)&=~8;
            *(DWORD*)(material+0xc)=0x3f800000;*(DWORD*)(material+0x10)=0;*(DWORD*)(material+0x14)=0x3f800000;
            *(DWORD*)(material+4)=*(DWORD*)(state+0x257+i*4);*(DWORD*)(state+0x1cf)=1;
        }
    }
    return atomic;
}
// samp.dll RVA: 0x000a86c0
void *__cdecl ObjectSaveMaterials(void *atomic,void *object) {
    BYTE *a=(BYTE*)atomic,*state=(BYTE*)object;
    if(*a!=1 || !*(void**)(a+0x18) || !state)return atomic;
    BYTE *geometry=*(BYTE**)(a+0x18);int count=*(int*)(geometry+0x24);if(count>16)count=16;
    *(DWORD*)(state+0x2d7)=*(DWORD*)(geometry+8);
    for(int i=0;i<count;++i) {
        BYTE *material=(*(BYTE***)(*(BYTE**)(a+0x18)+0x20))[i];if(!material)continue;
        *(DWORD*)(state+0x1d7+i*4)=*(DWORD*)material;
        *(DWORD*)(state+0x297+i*4)=*(DWORD*)(material+4);
    }
    *(DWORD*)(state+0x1c7)=1;
    return atomic;
}
// samp.dll RVA: 0x000a8740
void *__cdecl ObjectRestoreMaterials(void *atomic,void *object) {
    BYTE *a=(BYTE*)atomic,*state=(BYTE*)object;
    if(*a!=1 || !*(void**)(a+0x18) || !state)return atomic;
    BYTE *geometry=*(BYTE**)(a+0x18);int count=*(int*)(geometry+0x24);if(count>16)count=16;
    for(int i=0;i<count;++i) {
        BYTE *material=(*(BYTE***)(*(BYTE**)(a+0x18)+0x20))[i];if(!material)continue;
        if(*(DWORD*)(state+0x217+i*4) && *(DWORD*)material!=*(DWORD*)(state+0x1d7+i*4))
            *(DWORD*)material=*(DWORD*)(state+0x1d7+i*4);
        if(*(DWORD*)(state+0x1cf) && *(DWORD*)(state+0x257+i*4))
            *(DWORD*)(material+4)=*(DWORD*)(state+0x297+i*4);
    }
    *(DWORD*)(*(BYTE**)(a+0x18)+8)=*(DWORD*)(state+0x2d7);
    *(DWORD*)(state+0x1cf)=0;*(DWORD*)(state+0x1cb)=0;
    return atomic;
}

// samp.dll RVA: 0x000a87f0
void CObject::ProcessMaterials() {
    BYTE *state=(BYTE*)this;
    if(!hasMaterials)return;
    BYTE *rw=(BYTE*)GetRwObject();if(!rw)return;
    if(materials.rwObject!=rw) {
        materials.snapshotValid=0;
        void *frame=*(void**)(rw+4);if(!frame)return;
        GameRwForAllFrameObjects(frame,ObjectSaveMaterials,this);
        materials.rwObject=rw;materialPassState=0;
        return;
    }
    g_deviceMaterialOverrideEnabled=1;g_deviceMaterialOverrides=state;
    void *frame=*(void**)(rw+4);if(frame)GameRwForAllFrameObjects(frame,ObjectApplyMaterials,this);
}

#include "game/rw_preview.h"
// samp.dll RVA: 0x000a82a0
void CObject::SetMaterialText(int index,const char *text,BYTE size,const char *font,
    BYTE fontSize,BYTE bold,DWORD fontColor,DWORD backgroundColor,BYTE alignment) {
    if(index>16)return;
    BYTE *state=(BYTE*)this;
    void **texture=(void**)(state+0x217+index*4);
    if(*texture){GameDestroyTextDrawTexture(*texture);*texture=0;
        if(*(DWORD*)(state+0x110b+index*4))*(DWORD*)(state+0x110b+index*4)=0;}
    IUnknown **rendered=(IUnknown**)(state+0x114b+index*4);
    if(*rendered){(*rendered)->Release();*rendered=0;*(DWORD*)(state+0x31b+index*4)=0;}
    char **stored=(char**)(state+0x10cb+index*4);
    if(*stored){free(*stored);*stored=0;}
    *texture=GameRwCreateTexture(GameRwCreateRaster(2,2,0,0));if(!*texture)return;
    ++*(DWORD*)((BYTE*)*texture+0x50);
    unsigned length=0;while(text[length])++length;
    *stored=(char*)calloc(1,length+1);
    char *destination=*stored;do{*destination++=*text;}while(*text++);
    BYTE *record=state+index*0xd7;
    record[0x35c]=(BYTE)index;record[0x3e5]=size;
    destination=(char*)(record+0x3e6);do{*destination++=*font;}while(*font++);
    record[0x427]=fontSize;record[0x428]=bold!=0;
    *(DWORD*)(record+0x429)=fontColor;*(DWORD*)(record+0x42d)=backgroundColor;
    record[0x431]=alignment;
    *(DWORD*)(state+0x257+index*4)=0;*(DWORD*)(state+0x2db+index*4)=2;
    void *raster=*(void**)*texture;
    if(raster)*(void**)(state+0x110b+index*4)=GameRwReadField34(raster);
    hasMaterials=1;
}

#include "game/effects.h"
// samp.dll RVA: 0x000a8120
void CObject::SetMaterial(int model,int index,const char *dictionary,const char *texture,DWORD color) {
    if(index>16)return;
    if(model>0 && !GameModelQueryB4170(model))return;
    BYTE *state=(BYTE*)this;void **slot=(void**)(state+0x217+index*4);
    if(*slot){GameDestroyTextDrawTexture(*slot);*slot=0;
        if(*(DWORD*)(state+0x110b+index*4))*(DWORD*)(state+0x110b+index*4)=0;}
    if(model>0) {
        BOOL ready=g_pGame->IsModelAvailable(model);
        if(!ready){g_pGame->RequestModel(model,2);g_pGame->LoadRequestedModels();ready=g_pGame->IsModelAvailable(model);
            int waits=0;while(!ready){if(++waits>250)break;Sleep(2);ready=g_pGame->IsModelAvailable(model);}}
        *slot=ready?GameLoadDictionaryTexture(dictionary,texture):0;
        if(!GameGetModelReferenceCount(model) && g_pGame->IsModelAvailable(model))g_pGame->ReleaseModel(model,0);
    } else if(model==0) {
        if(!g_objectSharedMaterialTexture){GameLoadTextureDictionary("blanktex","blanktex",1);
            g_objectSharedMaterialTexture=GameLoadDictionaryTexture("blanktex","blanktex");}
        *slot=g_objectSharedMaterialTexture;
    }
    *(DWORD*)(state+0x257+index*4)=color;
    *(DWORD*)(state+0x2db+index*4)=1;hasMaterials=1;
}

// samp.dll RVA: 0x000a9a20
void CObject::BeforeMaterialRender() {
    BYTE *state=(BYTE*)this;
    if(hasMaterials && materialPassState) {
        ProcessMaterials();if(state[0x1193])NativeA8C30();
    }
    materialPassState=1;
}
// samp.dll RVA: 0x000a9a60
void CObject::AfterMaterialRender() {
    BYTE *state=(BYTE*)this;if(!hasMaterials)return;
    if(state[0x1193])NativeA8C70();
    g_deviceMaterialOverrideEnabled=0;g_deviceMaterialOverrides=0;
    if(!materials.texturesChanged && !materials.colorsChanged)return;
    BYTE *rw=(BYTE*)GetRwObject();if(!rw || materials.rwObject!=rw)return;
    void *frame=*(void**)(rw+4);if(frame)GameRwForAllFrameObjects(frame,ObjectRestoreMaterials,this);
}

// samp.dll RVA: 0x001e0450
CMatrix g_objectAttachmentMatrix;
// samp.dll RVA: 0x001e049c
CVector g_objectAttachmentZeroSpeed={0,0,0};
// samp.dll RVA: 0x000a7a10
void CObject::UpdateAttachedObjectRotation() {
    if(attachedObject==0xffff || attachmentRotationSync)return;
    GetMatrix(&g_objectAttachmentMatrix);
    DWORD *out=(DWORD*)&g_objectAttachmentMatrix,*in=(DWORD*)&baseMatrix;
    for(int column=0;column<3;++column)for(int row=0;row<3;++row)out[column*4+row]=in[column*4+row];
    GameRotateRwMatrixOnAxis(&g_objectAttachmentMatrix,0,attachmentRotation.x);
    GameRotateRwMatrixOnAxis(&g_objectAttachmentMatrix,1,attachmentRotation.y);
    GameRotateRwMatrixOnAxis(&g_objectAttachmentMatrix,2,attachmentRotation.z);
    SetMatrix(g_objectAttachmentMatrix);
    SetTurnSpeed(g_objectAttachmentZeroSpeed);SetSpeed(g_objectAttachmentZeroSpeed);
}

#include "game/camera.h"
#include "game/hooks_render.h"
// samp.dll RVA: 0x0011ee80
extern const float g_objectRenderDistanceThreshold=600.0f;
// samp.dll RVA: 0x001e04a8
BYTE g_objectRenderFlag;
// samp.dll RVA: 0x000a8b50
void CObject::NativeA8B50() {
    void *backend=m_pGameEntity;
    // GTA SA virtual addresses: excluded vtable, backend post-call helper.
    if(!backend || *(DWORD*)backend==0x863c40)return;
    DWORD *table=*(DWORD**)backend;
    __asm {mov ecx,backend}
    __asm {mov eax,table}
    __asm {call dword ptr [eax+0x44]}
    ((void(__cdecl*)(void*))0x553260)(backend);
}
// samp.dll RVA: 0x000a9b80
void CObject::Render() {
    // GTA SA virtual address: excluded backend vtable.
    if(!m_pGameEntity || *(DWORD*)m_pGameEntity==0x863c40 || !GetRwObject())return;
    UpdateCachedCentre();
    CCamera *camera=*(CCamera**)((BYTE*)g_pGame+4);CVector *centre=&worldCentre;
    float *radius=&collisionRadius;BYTE closer;
    // Compare the camera's unspilled ST0 result, as in the original body.
    __asm {mov ecx,camera}
    __asm {push centre}
    __asm {call CCamera::GetDistanceToPoint}
    __asm {mov edx,radius}
    __asm {fcomp dword ptr [edx]}
    __asm {fnstsw ax}
    __asm {test ah,5}
    __asm {setnp closer}
    if(!closer && !GameTestCameraSphere(centre,*radius))return;
    if(drawLast) {
        // GTA SA virtual address: native weather/render state gate.
        DWORD alpha=(*(DWORD*)0xb72914 || (GameGetModelFlagsByte(modelIndex)&8))?0:100;
        ClientSetRenderState(30,alpha);
    }
    ClientSetRenderState(20,(GameGetModelFlagsByte(modelIndex)&0x40)?2:1);
    if(hasMaterials && materialPassState)ProcessMaterials();
    GameSetupD3DFog(1);
    if(hasMaterials && materials.colorsChanged) {
        if(cameraDistance<g_objectRenderDistanceThreshold) {
            NativeA8C30();GameRwRenderObject(GetRwObject());NativeA8C70();
        } else GameRwRenderObject(GetRwObject());
    } else if(cameraDistance<g_objectRenderDistanceThreshold)NativeA8B50();
    else GameRwRenderObject(GetRwObject());
    if(hasMaterials && materialPassState)AfterMaterialRender();
    g_objectRenderFlag=0;materialPassState=1;
}

// samp.dll RVA: 0x000a8b00
void *__cdecl ObjectFrameIdentity(void *object,void *) {return object;}
// samp.dll RVA: 0x000a8b10
void CObject::ProcessFrameObjectsOnce() {
    BYTE *rw=(BYTE*)GetRwObject();if(!rw || materials.frameObjectsProcessed)return;
    void *frame=*(void**)(rw+4);if(!frame)return;
    GameRwForAllFrameObjects(frame,ObjectFrameIdentity,this);
    materials.frameObjectsProcessed=1;
}
// samp.dll RVA: 0x000a7f30
void CObject::AttachToBackend(void *target,const CVector *offset,const CVector *rotation) {
    void *backend=m_pGameEntity;if(!backend || !target)return;
    float x,y,z;
    __asm {mov eax,rotation}
    __asm {fld dword ptr [eax]}
    __asm {fmul g_objectDegreesToRadians}
    __asm {fstp x}
    __asm {fld dword ptr [eax+4]}
    __asm {fmul g_objectDegreesToRadians}
    __asm {fstp y}
    __asm {fld dword ptr [eax+8]}
    __asm {fmul g_objectDegreesToRadians}
    __asm {fstp z}
    // GTA SA virtual address: native entity attachment backend (thiscall).
    ((void(__fastcall*)(void*,void*,void*,float,float,float,float,float,float))0x54d570)
        (backend,0,target,offset->x,offset->y,offset->z,x,y,z);
}
