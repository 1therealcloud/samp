#include "game/rw_helpers.h"
#include "game/rw_transform.h"
#include "game/input.h"
#include "game/object_math.h"
#include "game/native_scratch.h"

// samp.dll RVA: 0x001553d4
GameRwPointTransformBackend g_rwPointTransformBackend=(GameRwPointTransformBackend)0x7edd90;
// samp.dll RVA: 0x001553fc
GameRwAxisRotateBackend g_rwAxisRotateBackend=(GameRwAxisRotateBackend)0x7f1fd0;
// samp.dll RVA: 0x0011f58c
const float g_headingInverseHalfTurn=0.0055555556900799274f;
// samp.dll RVA: 0x0011f590
const float g_headingPi=3.1415927410125732f;

// GTA SA virtual address: 0x007EDDC0, fixed vector transform (no translation).
// samp.dll RVA: 0x000b5210
CVector *GameTransformRwVector(CVector *out,const CVector *vector,const CMatrix *matrix) {
    return ((CVector *(__cdecl*)(CVector*,const CVector*,const CMatrix*))0x7eddc0)(out,vector,matrix);
}

// The version-2 switch is sticky: other versions do not reset this pointer.
// GTA SA virtual addresses: initial 0x007EDD90, version-2 target 0x007EDDD0.
// samp.dll RVA: 0x000b5230
CVector *GameTransformRwPoint(CVector *out,const CVector *point,const CMatrix *matrix) {
    if(g_gameVersion==2)g_rwPointTransformBackend=(GameRwPointTransformBackend)0x7eddd0;
    return g_rwPointTransformBackend(out,point,1,matrix);
}

// Raw register-preserving native ABI; shared fields are existing arena views.
// Input +1000, output +1004, target +1008 (native RVAs 2ABE18/1C/20).
// samp.dll RVA: 0x000b5260
__declspec(naked) void GameRepeatRwMatrixInvert() {
    __asm {
        push ebx
        push esi
        push edi
        pushad
        push dword ptr [g_entityCollisionScratch + 0x1000]
        push dword ptr [g_entityCollisionScratch + 0x1004]
        mov edx,dword ptr [g_entityCollisionScratch + 0x1008]
        call edx
        pop edx
        pop edx
        popad
        pop edi
        pop esi
        pop ebx
        ret
    }
}

// Raw register-preserving native ABI; shared fields are existing arena views.
// Input +1000, output +1004, target +1008 (native RVAs 2ABE18/1C/20).
// samp.dll RVA: 0x000b5280
__declspec(naked) void GameInvertRwMatrix(CMatrix *out,const CMatrix *matrix) {
    __asm {
        push ebp
        mov ebp,esp
        mov eax,g_gameVersion
        mov ecx,dword ptr [ebp + 0xc]
        mov edx,dword ptr [ebp + 0x8]
        dec eax
        neg eax
        sbb eax,eax
        push ebx
        and eax,0x40
        push esi
        add eax,0x7f2070
        push edi
        mov dword ptr [g_entityCollisionScratch + 0x1008],eax
        mov dword ptr [g_entityCollisionScratch + 0x1000],ecx
        mov dword ptr [g_entityCollisionScratch + 0x1004],edx
        pushad
        push dword ptr [g_entityCollisionScratch + 0x1000]
        push dword ptr [g_entityCollisionScratch + 0x1004]
        mov edx,dword ptr [g_entityCollisionScratch + 0x1008]
        call edx
        pop edx
        pop edx
        popad
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}

// Native x87 comparisons, arithmetic order and extended ST0 return retained.
// samp.dll RVA: 0x000b54b0
__declspec(naked) float GameHeadingToRadians(float heading) {
    __asm {
        fld float ptr [esp + 0x4]
        fcomp float ptr g_anglePeriod
        fnstsw ax
        test ah,0x41
        jz rw_100b550f
        fld float ptr [esp + 0x4]
        fcomp float ptr g_objectScaleLowerBound
        fnstsw ax
        test ah,0x5
        jnp rw_100b550f
        fld float ptr [esp + 0x4]
        fcomp float ptr g_angleUpperBound
        fld float ptr [esp + 0x4]
        fnstsw ax
        test ah,0x41
        jnz rw_100b5502
        fsub float ptr g_angleUpperBound
        fmul float ptr g_headingPi
        fmul float ptr g_headingInverseHalfTurn
        fsubr float ptr g_headingPi
        fchs
        ret
rw_100b5502:
        fmul float ptr g_headingPi
        fmul float ptr g_headingInverseHalfTurn
        ret
rw_100b550f:
        fld float ptr g_objectScaleLowerBound
        ret
    }
}

// Native unbounded axis lookup; target publication occurs before the call.
// GTA SA virtual addresses: USA 0x007F1FD0, fallback 0x007F2010.
// samp.dll RVA: 0x000b52d0
void GameRotateRwMatrixOnAxis(CMatrix *matrix,int axis,float angle) {
    const CVector *direction=&g_rwRotationAxes[axis];
    g_rwAxisRotateBackend=(GameRwAxisRotateBackend)(g_gameVersion==1?0x7f1fd0:0x7f2010);
    g_rwAxisRotateBackend(matrix,direction,angle,1);
}
