#include "game/quaternion.h"

// The four input components are passed by value, without normalization.
// Keep products in x87 registers until the native output stores. Output order
// is 00,11,22,10,01,20,02,21,12; output pointers may alias.
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
