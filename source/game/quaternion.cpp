#include "game/quaternion.h"
// samp.dll RVA: 0x000b65c0
void GameQuaternionToMatrixComponents(const GameQuaternion *quaternion,CMatrix *matrix) {
    GameQuaternionComponentsToMatrix(quaternion->w,quaternion->x,quaternion->y,quaternion->z,
        &matrix->right.x,&matrix->right.y,&matrix->right.z,&matrix->up.x,&matrix->up.y,&matrix->up.z,
        &matrix->at.x,&matrix->at.y,&matrix->at.z);
}
#include "game/game.h"
#include <d3dx9.h>
#include <string.h>
// samp.dll RVA: 0x000b4d50
void GameQuaternionFromMatrix(const CMatrix *matrix,GameQuaternion *out) {
    D3DXQUATERNION quaternion;
    // GTA SA virtual addresses: USA 7EB5C0; all other version values use EU 7EB600.
    DWORD target=g_gameVersion==1?0x7eb5c0:0x7eb600;
    ((void(__cdecl*)(D3DXQUATERNION*,const CMatrix*))target)(&quaternion,matrix);
    // Native w publication is an x87 float load/store; xyz are raw DWORD copies.
    __asm {
        mov eax,out
        fld quaternion.w
        fstp dword ptr [eax]
    }
    memcpy(&out->x,&quaternion.x,12);
}
// samp.dll RVA: 0x000b5040
void GameQuaternionNormalize(GameQuaternion *quaternion) {
    D3DXQUATERNION input,output;
    memcpy(&input.x,&quaternion->x,12);memcpy(&input.w,&quaternion->w,4);
    D3DXQuaternionNormalize(&output,&input);
    memcpy(&quaternion->w,&output.w,4);memcpy(&quaternion->x,&output.x,12);
}

// samp.dll RVA: 0x001181a8
const float g_quaternionMatrixTwo=2.0f;
// samp.dll RVA: 0x00117b9c
const float g_quaternionMatrixOne=1.0f;

// The original keeps sx, sz, xz and xy in x87 registers but spills other
// products to float. VC7 C++ reschedules/spills these differently; explicit x87
// preserves the native rounding boundaries and caller precision control.
// Flags, padding and translation are untouched; zero input is not special-cased.
// samp.dll RVA: 0x000b4e40
__declspec(naked) void GameQuaternionToMatrixBody(const GameQuaternion *quaternion,CMatrix *matrix) {
    __asm {
        // Raw cdecl frame; x,y,z are retained while scale is computed.
        sub esp,0x30
        mov eax,dword ptr [esp + 0x34]
        fld float ptr [eax + 0x4]
        mov ecx,dword ptr [eax]
        fld float ptr [eax + 0x8]
        mov dword ptr [esp + 0x34],ecx
        fld float ptr [eax + 0xc]
        fld float ptr [eax + 0xc]
        fld float ptr [eax + 0x8]
        fld float ptr [eax + 0x4]
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fld st(3)
        fmul st(0),st(4)
        faddp st(1),st(0)
        fld float ptr [esp + 0x34]
        fmul float ptr [esp + 0x34]
        // scale = float(2 / (((x*x+y*y)+z*z)+w*w)).
        faddp st(1),st(0)
        fdivr float ptr g_quaternionMatrixTwo
        fstp float ptr [esp + 0x34]
        fstp st(0)
        fstp st(0)
        fstp st(0)
        // sx and sz remain extended; only sy is rounded to float here.
        fld float ptr [esp + 0x34]
        fmul st(0),st(3)
        fld float ptr [esp + 0x34]
        fmul st(0),st(3)
        fstp float ptr [esp + 0x1c]
        fld float ptr [esp + 0x34]
        fmul st(0),st(2)
        fld float ptr [eax]
        mov eax,dword ptr [esp + 0x38]
        fld st(0)
        // wx,wy,wz,xx,yy,zz,yz have native float-store boundaries.
        fmul st(0),st(3)
        fstp float ptr [esp]
        fld float ptr [esp + 0x1c]
        fmul st(0),st(1)
        fstp float ptr [esp + 0x4]
        fmul st(0),st(1)
        fstp float ptr [esp + 0x8]
        fld st(1)
        fmul st(0),st(5)
        fstp float ptr [esp + 0xc]
        fld float ptr [esp + 0x1c]
        fmul st(0),st(4)
        fstp float ptr [esp + 0x10]
        fld st(0)
        fmul st(0),st(3)
        fstp float ptr [esp + 0x14]
        fmul st(0),st(3)
        fstp float ptr [esp + 0x24]
        fmul st(0),st(1)
        fstp st(2)
        fstp st(0)
        fld float ptr [esp + 0x1c]
        fmul st(0),st(2)
        fld float ptr [esp + 0x14]
        fadd float ptr [esp + 0x10]
        fsubr float ptr g_quaternionMatrixOne
        // Publish nine basis components in native order; xz and xy stay extended.
        fstp float ptr [eax]
        fld float ptr [esp + 0x8]
        fadd st(0),st(1)
        fstp float ptr [eax + 0x4]
        fld st(1)
        fsub float ptr [esp + 0x4]
        fstp float ptr [eax + 0x8]
        fsub float ptr [esp + 0x8]
        fstp float ptr [eax + 0x10]
        fld float ptr [esp + 0x14]
        fadd float ptr [esp + 0xc]
        fsubr float ptr g_quaternionMatrixOne
        fstp float ptr [eax + 0x14]
        fld float ptr [esp]
        fadd float ptr [esp + 0x24]
        fstp float ptr [eax + 0x18]
        fld float ptr [esp + 0x4]
        fadd st(0),st(1)
        fstp float ptr [eax + 0x20]
        fstp st(0)
        fstp st(0)
        fld float ptr [esp + 0x24]
        fsub float ptr [esp]
        fstp float ptr [eax + 0x24]
        fld float ptr [esp + 0x10]
        fadd float ptr [esp + 0xc]
        fsubr float ptr g_quaternionMatrixOne
        fstp float ptr [eax + 0x28]
        add esp,0x30
        ret
    }
}
// Native forwarding entry shares the cdecl argument frame with the body.
// samp.dll RVA: 0x000b4fb0
__declspec(naked) void GameQuaternionToMatrix(const GameQuaternion *quaternion,CMatrix *matrix) {
    __asm { jmp GameQuaternionToMatrixBody }
}

// CRT helper uses x87 ST0 for its argument/result, rather than a C stack argument.
// samp.dll RVA: 0x000fce60
extern "C" void __cdecl _CIasin();
// samp.dll RVA: 0x0011d7c8
const float g_quaternionEulerNegativeTwo=-2.0f;

// Snapshot both inputs in D3DX x,y,z,w order before publishing the result.
// samp.dll RVA: 0x000b4fc0
void GameQuaternionSlerp(GameQuaternion *out,const GameQuaternion *from,const GameQuaternion *to,float amount) {
    D3DXQUATERNION first,second,result;
    memcpy(&first.w,&from->w,4);memcpy(&first.x,&from->x,12);
    memcpy(&second.w,&to->w,4);memcpy(&second.x,&to->x,12);
    D3DXQuaternionSlerp(&result,&first,&second,amount);
    __asm {
        mov eax,out
        fld result.w
        fstp dword ptr [eax]
    }
    memcpy(&out->x,&result.x,12);
}

// Euler inputs are radians. Native FSIN/FCOS and mixed float/x87 intermediates
// implement the half-angle rotation; all inputs are consumed before output stores.
// samp.dll RVA: 0x000b50a0
__declspec(naked) void GameQuaternionFromEuler(const CVector *angles,GameQuaternion *quaternion) {
    __asm {
        sub esp,0x10
        mov eax,dword ptr [esp + 0x14]
        fld float ptr [eax + 0x8]
        fmul float ptr g_textDrawHalf
        fld st(0)
        fcos
        fld float ptr [eax + 0x4]
        fmul float ptr g_textDrawHalf
        fld st(0)
        fcos
        fstp float ptr [esp + 0x4]
        fld float ptr [eax]
        mov eax,dword ptr [esp + 0x18]
        fmul float ptr g_textDrawHalf
        fld st(0)
        fcos
        fstp float ptr [esp]
        fxch st(3)
        fsin
        fstp float ptr [esp + 0x14]
        fsin
        fstp float ptr [esp + 0x8]
        fxch st(1)
        fsin
        fxch st(1)
        fld float ptr [esp + 0x8]
        fmul st(0),st(2)
        fstp float ptr [esp + 0xc]
        fld float ptr [esp]
        fmul float ptr [esp + 0x4]
        fmul st(0),st(1)
        fld float ptr [esp + 0xc]
        fmul float ptr [esp + 0x14]
        faddp st(1),st(0)
        fstp float ptr [eax]
        fld float ptr [esp + 0x4]
        fmul st(0),st(2)
        fmul st(0),st(1)
        fld float ptr [esp + 0x8]
        fmul float ptr [esp + 0x14]
        fmul float ptr [esp]
        fsubp st(1),st(0)
        fstp float ptr [eax + 0x4]
        fxch st(1)
        fmul float ptr [esp + 0x14]
        fmul float ptr [esp + 0x4]
        fld float ptr [esp + 0x8]
        fmul float ptr [esp]
        fmul st(0),st(2)
        faddp st(1),st(0)
        fstp float ptr [eax + 0x8]
        fld float ptr [esp + 0x14]
        fmul float ptr [esp]
        fmul float ptr [esp + 0x4]
        fld float ptr [esp + 0xc]
        fmul st(0),st(2)
        fsubp st(1),st(0)
        fstp float ptr [eax + 0xc]
        fstp st(0)
        add esp,0x10
        ret
    }
}

// Native FPATAN and CRT asin return radians, without clamping or normalization.
// Inputs are re-read after each output store; overlapping buffers follow that order.
// samp.dll RVA: 0x000b5160
__declspec(naked) void GameQuaternionToEuler(const GameQuaternion *quaternion,CVector *angles) {
    __asm {
        sub esp,0xc
        push esi
        mov esi,dword ptr [esp + 0x14]
        fld float ptr [esi]
        push edi
        fld st(0)
        mov edi,dword ptr [esp + 0x1c]
        fmul st(0),st(1)
        fstp float ptr [esp + 0x10]
        fstp st(0)
        fld float ptr [esi + 0x4]
        fld st(0)
        fmul st(0),st(1)
        fstp float ptr [esp + 0x8]
        fstp st(0)
        fld float ptr [esi + 0x8]
        fld st(0)
        fmul st(0),st(1)
        fstp float ptr [esp + 0x18]
        fstp st(0)
        fld float ptr [esi + 0xc]
        fld st(0)
        fmul st(0),st(1)
        fstp float ptr [esp + 0xc]
        fstp st(0)
        fld float ptr [esi + 0x8]
        fmul float ptr [esi + 0x4]
        fld float ptr [esi + 0xc]
        fmul float ptr [esi]
        faddp st(1),st(0)
        fadd st(0),st(0)
        fld float ptr [esp + 0x8]
        fsub float ptr [esp + 0x18]
        fsub float ptr [esp + 0xc]
        fadd float ptr [esp + 0x10]
        fpatan
        fstp float ptr [edi]
        fld float ptr [esi + 0xc]
        fmul float ptr [esi + 0x4]
        fld float ptr [esi + 0x8]
        fmul float ptr [esi]
        fsubp st(1),st(0)
        fmul float ptr g_quaternionEulerNegativeTwo
        call _CIasin
        fstp float ptr [edi + 0x4]
        fld float ptr [esi + 0xc]
        fmul float ptr [esi + 0x8]
        fld float ptr [esi + 0x4]
        fmul float ptr [esi]
        faddp st(1),st(0)
        fadd st(0),st(0)
        fld float ptr [esp + 0x18]
        fadd float ptr [esp + 0x8]
        fsubr float ptr [esp + 0xc]
        fadd float ptr [esp + 0x10]
        fpatan
        fstp float ptr [edi + 0x8]
        pop edi
        pop esi
        add esp,0xc
        ret
    }
}

#include "game/util.h"

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


// The four input components are passed by value, without normalization.
// Keep products in x87 registers until the native output stores. Output order
// is 00,11,22,10,01,20,02,21,12; output pointers may alias.
// samp.dll RVA: 0x000b4a50
__declspec(naked) void GameQuaternionComponentsToMatrix(float w,float x,float y,float z,
    float *m00,float *m01,float *m02,float *m10,float *m11,float *m12,
    float *m20,float *m21,float *m22) {
    __asm {
        fld float ptr [esp + 0x4]
        mov eax,dword ptr [esp + 0x14]
        fmul float ptr [esp + 0x4]
        mov ecx,dword ptr [esp + 0x24]
        fld float ptr [esp + 0x8]
        mov edx,dword ptr [esp + 0x34]
        fmul float ptr [esp + 0x8]
        fld float ptr [esp + 0xc]
        fmul float ptr [esp + 0xc]
        fld float ptr [esp + 0x10]
        fmul float ptr [esp + 0x10]
        fld st(2)
        fsub st(0),st(2)
        fsub st(0),st(1)
        fadd st(0),st(4)
        fstp float ptr [eax]
        fld st(1)
        mov eax,dword ptr [esp + 0x20]
        fsub st(0),st(3)
        fsub st(0),st(1)
        fadd st(0),st(4)
        fstp float ptr [ecx]
        fxch st(1)
        mov ecx,dword ptr [esp + 0x18]
        fadd st(0),st(2)
        fsubr st(0),st(1)
        fadd st(0),st(3)
        fstp float ptr [edx]
        mov edx,dword ptr [esp + 0x2c]
        fstp st(0)
        fstp st(0)
        fstp st(0)
        fld float ptr [esp + 0x8]
        fmul float ptr [esp + 0xc]
        fld float ptr [esp + 0x4]
        fmul float ptr [esp + 0x10]
        fld st(0)
        fadd st(0),st(2)
        fadd st(0),st(0)
        fstp float ptr [eax]
        fxch st(1)
        mov eax,dword ptr [esp + 0x1c]
        fsub st(0),st(1)
        fadd st(0),st(0)
        fstp float ptr [ecx]
        mov ecx,dword ptr [esp + 0x30]
        fstp st(0)
        fld float ptr [esp + 0x8]
        fmul float ptr [esp + 0x10]
        fld float ptr [esp + 0x4]
        fmul float ptr [esp + 0xc]
        fld st(1)
        fsub st(0),st(1)
        fadd st(0),st(0)
        fstp float ptr [edx]
        fadd st(0),st(1)
        fadd st(0),st(0)
        fstp float ptr [eax]
        fstp st(0)
        fld float ptr [esp + 0xc]
        fmul float ptr [esp + 0x10]
        fld float ptr [esp + 0x4]
        fmul float ptr [esp + 0x8]
        fld st(0)
        fadd st(0),st(2)
        fadd st(0),st(0)
        fstp float ptr [ecx]
        fxch st(1)
        fsub st(0),st(1)
        fadd st(0),st(0)
        mov edx,dword ptr [esp + 0x28]
        fstp float ptr [edx]
        fstp st(0)
        ret
    }
}
