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
