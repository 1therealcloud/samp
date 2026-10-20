#include "game/quaternion.h"
#include "game/input.h"
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
