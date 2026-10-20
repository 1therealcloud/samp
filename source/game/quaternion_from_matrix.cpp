#include "game/quaternion.h"
#include "game/object_math.h"

// Selected static CRT helper, not a recovered SA-MP implementation.
// samp.dll RVA: 0x000fd048
extern "C" double __cdecl _copysign(double magnitude,double sign);

// Native register/spill boundaries and unordered comparisons are observable.
// Square-root terms are clamped with the original x87 status tests, then signs
// are copied from the three antisymmetric differences by the selected CRT.
// samp.dll RVA: 0x000b48b0
__declspec(naked) void GameMatrixComponentsToQuaternion(float m00,float m01,float m02,
    float m10,float m11,float m12,float m20,float m21,float m22,
    float *w,float *x,float *y,float *z) {
    __asm {
        push ecx
        fld float ptr [esp + 0x8]
        fadd float ptr [esp + 0x18]
        fadd float ptr [esp + 0x28]
        fadd float ptr g_quaternionMatrixOne
        fcom float ptr g_objectScaleLowerBound
        fnstsw ax
        test ah,0x5
        jp q_100b48d8
        fstp st(0)
        fld float ptr g_objectScaleLowerBound
q_100b48d8:
        fsqrt
        fmul float ptr g_textDrawHalf
        fstp float ptr [esp]
        fld float ptr [esp + 0x8]
        fadd float ptr g_quaternionMatrixOne
        fsub float ptr [esp + 0x18]
        fsub float ptr [esp + 0x28]
        fcom float ptr g_objectScaleLowerBound
        fnstsw ax
        test ah,0x5
        jp q_100b490a
        fstp st(0)
        fld float ptr g_objectScaleLowerBound
q_100b490a:
        fsqrt
        fmul float ptr g_textDrawHalf
        fld float ptr g_quaternionMatrixOne
        fsub float ptr [esp + 0x8]
        fld float ptr [esp + 0x18]
        fadd st(0),st(1)
        fsub float ptr [esp + 0x28]
        fcom float ptr g_objectScaleLowerBound
        fnstsw ax
        test ah,0x5
        jp q_100b493b
        fstp st(0)
        fld float ptr g_objectScaleLowerBound
q_100b493b:
        fsqrt
        fmul float ptr g_textDrawHalf
        fstp float ptr [esp + 0x8]
        fsub float ptr [esp + 0x18]
        fadd float ptr [esp + 0x28]
        fcom float ptr g_objectScaleLowerBound
        fnstsw ax
        test ah,0x5
        jp q_100b4964
        fstp st(0)
        fld float ptr g_objectScaleLowerBound
q_100b4964:
        fsqrt
        fmul float ptr g_textDrawHalf
        fstp float ptr [esp + 0x18]
        fld float ptr [esp]
        fcomp float ptr g_objectScaleLowerBound
        fnstsw ax
        test ah,0x5
        jp q_100b4987
        mov dword ptr [esp],0x0
q_100b4987:
        fcom float ptr g_objectScaleLowerBound
        fnstsw ax
        test ah,0x5
        jp q_100b499c
        fstp st(0)
        fld float ptr g_objectScaleLowerBound
q_100b499c:
        fld float ptr [esp + 0x8]
        fcomp float ptr g_objectScaleLowerBound
        fnstsw ax
        test ah,0x5
        jp q_100b49b5
        mov dword ptr [esp + 0x8],0x0
q_100b49b5:
        fld float ptr [esp + 0x18]
        fcomp float ptr g_objectScaleLowerBound
        fnstsw ax
        test ah,0x5
        jp q_100b49ce
        mov dword ptr [esp + 0x18],0x0
q_100b49ce:
        fld float ptr [esp + 0x24]
        sub esp,0x10
        fsub float ptr [esp + 0x2c]
        fstp double ptr [esp + 0x8]
        fstp double ptr [esp]
        call _copysign
        fstp float ptr [esp + 0x38]
        fld float ptr [esp + 0x20]
        fsub float ptr [esp + 0x30]
        fstp double ptr [esp + 0x8]
        fld float ptr [esp + 0x18]
        fstp double ptr [esp]
        call _copysign
        fstp float ptr [esp + 0x18]
        fld float ptr [esp + 0x24]
        fsub float ptr [esp + 0x1c]
        fstp double ptr [esp + 0x8]
        fld float ptr [esp + 0x28]
        fstp double ptr [esp]
        call _copysign
        mov eax,dword ptr [esp + 0x3c]
        fld float ptr [esp + 0x10]
        mov ecx,dword ptr [esp + 0x40]
        fstp float ptr [eax]
        fld float ptr [esp + 0x38]
        mov edx,dword ptr [esp + 0x44]
        mov eax,dword ptr [esp + 0x48]
        fstp float ptr [ecx]
        fld float ptr [esp + 0x18]
        fstp float ptr [edx]
        fstp float ptr [eax]
        add esp,0x14
        ret
    }
}

// Inputs are passed by value before the conversion publishes w,x,y,z.
// samp.dll RVA: 0x000b4df0
void GameQuaternionFromMatrixComponents(const CMatrix *matrix,GameQuaternion *out) {
    GameMatrixComponentsToQuaternion(matrix->right.x,matrix->right.y,matrix->right.z,
        matrix->up.x,matrix->up.y,matrix->up.z,
        matrix->at.x,matrix->at.y,matrix->at.z,
        &out->w,&out->x,&out->y,&out->z);
}

// samp.dll RVA: 0x0011f588
const float g_quaternionOppositeThreshold=0.0001f;

// Inputs are snapshotted before output. Preserve the original near-opposite
// fallback (-from.z,from.y,from.x), float spills, and final x,y,z,w store order.
// There is no additional zero-length guard.
// samp.dll RVA: 0x000b4b60
__declspec(naked) void GameQuaternionFromVectors(GameQuaternion *out,const CVector *from,const CVector *to) {
    __asm {
        sub esp,0x18
        mov eax,dword ptr [esp + 0x20]
        mov ecx,dword ptr [eax]
        mov edx,dword ptr [eax + 0x4]
        mov eax,dword ptr [eax + 0x8]
        mov dword ptr [esp + 0x20],eax
        mov eax,dword ptr [esp + 0x24]
        mov dword ptr [esp + 0x4],ecx
        mov ecx,dword ptr [eax]
        mov dword ptr [esp],edx
        mov edx,dword ptr [eax + 0x4]
        mov eax,dword ptr [eax + 0x8]
        mov dword ptr [esp + 0x24],eax
        fld float ptr [esp + 0x24]
        mov dword ptr [esp + 0x8],edx
        fmul float ptr [esp]
        mov dword ptr [esp + 0xc],ecx
        fld float ptr [esp + 0x8]
        fmul float ptr [esp + 0x20]
        fsubp st(1),st(0)
        fld float ptr [esp + 0xc]
        fmul float ptr [esp + 0x20]
        fld float ptr [esp + 0x24]
        fmul float ptr [esp + 0x4]
        fsubp st(1),st(0)
        fstp float ptr [esp + 0x10]
        fld float ptr [esp + 0x8]
        fmul float ptr [esp + 0x4]
        fld float ptr [esp + 0xc]
        fmul float ptr [esp]
        fsubp st(1),st(0)
        fstp float ptr [esp + 0x14]
        fld float ptr [esp + 0x24]
        fmul float ptr [esp + 0x24]
        fld float ptr [esp + 0x8]
        fmul float ptr [esp + 0x8]
        faddp st(1),st(0)
        fld float ptr [esp + 0xc]
        fmul float ptr [esp + 0xc]
        faddp st(1),st(0)
        fld float ptr [esp + 0x20]
        fmul float ptr [esp + 0x20]
        fld float ptr [esp]
        fmul float ptr [esp]
        faddp st(1),st(0)
        fld float ptr [esp + 0x4]
        fmul float ptr [esp + 0x4]
        faddp st(1),st(0)
        fmulp st(1),st(0)
        fsqrt
        fld float ptr [esp + 0x24]
        fmul float ptr [esp + 0x20]
        fld float ptr [esp + 0x8]
        fmul float ptr [esp]
        faddp st(1),st(0)
        fld float ptr [esp + 0xc]
        fmul float ptr [esp + 0x4]
        faddp st(1),st(0)
        faddp st(1),st(0)
        fst float ptr [esp + 0x24]
        fcomp float ptr g_quaternionOppositeThreshold
        fnstsw ax
        test ah,0x5
        jp q_vectors_normalize
        mov ecx,dword ptr [esp]
        fstp st(0)
        fld float ptr [esp + 0x20]
        mov edx,dword ptr [esp + 0x4]
        fchs
        mov dword ptr [esp + 0x10],ecx
        mov dword ptr [esp + 0x14],edx
        mov dword ptr [esp + 0x24],0x0
q_vectors_normalize:
        fld float ptr [esp + 0x24]
        mov eax,dword ptr [esp + 0x1c]
        fmul float ptr [esp + 0x24]
        fld float ptr [esp + 0x14]
        fmul float ptr [esp + 0x14]
        faddp st(1),st(0)
        fld float ptr [esp + 0x10]
        fmul float ptr [esp + 0x10]
        faddp st(1),st(0)
        fld st(1)
        fmul st(0),st(2)
        faddp st(1),st(0)
        fsqrt
        fld float ptr g_quaternionMatrixOne
        fdiv st(0),st(1)
        fstp float ptr [esp + 0x20]
        fstp st(0)
        fld float ptr [esp + 0x20]
        fmul st(0),st(1)
        fstp float ptr [eax + 0x4]
        fstp st(0)
        fld float ptr [esp + 0x20]
        fmul float ptr [esp + 0x10]
        fstp float ptr [eax + 0x8]
        fld float ptr [esp + 0x20]
        fmul float ptr [esp + 0x14]
        fstp float ptr [eax + 0xc]
        fld float ptr [esp + 0x20]
        fmul float ptr [esp + 0x24]
        fstp float ptr [eax]
        add esp,0x18
        ret
    }
}
