#include <math.h>
#include <stdio.h>

#include <d3dx9math.h>

#include "game/util.h"

#include "game/custommodels.h"
#include "game/game.h"
#include "game/modelfile.h"
#include "main.h"
#include <static_assert.h>
STATIC_ASSERT(sizeof(CVector) == sizeof(D3DXVECTOR3));
STATIC_ASSERT(offsetof(CVector, x) == offsetof(D3DXVECTOR3, x));
STATIC_ASSERT(offsetof(CVector, y) == offsetof(D3DXVECTOR3, y));
STATIC_ASSERT(offsetof(CVector, z) == offsetof(D3DXVECTOR3, z));
// 0x000c5110
IDirect3DDevice9* GameGetCurrentD3DDevice()
{
    return *(IDirect3DDevice9**)0xc97c28;
}
// 0x000c5510
__declspec(naked) DWORD GameRwInvokeEngineSlot24(DWORD first, DWORD second)
{
    // clang-format off
    __asm {
        push ebp
        mov ebp,esp
        push ebx
        push dword ptr [ebp+0xc]
        push dword ptr [ebp+8]
        mov ebx,0xc97b24
        mov eax,dword ptr [ebx]
        call dword ptr [eax+0x24]
        add esp,8
        pop ebx
        pop ebp
        ret
    }
    // clang-format on
}
#include "chatwindow.h"
// 0x0011e3a0
const double g_aimVectorHalfPi = 1.5707963705062866;
// 0x000b6440
__declspec(naked) void __stdcall GameBuildAimUpVector(const CVector* direction, CVector* up)
{
    // clang-format off
    __asm {
        mov ecx,[esp+4]
        fld dword ptr [ecx]
        mov eax,[esp+8]
        fld dword ptr [ecx+4]
        fpatan
        fsub qword ptr [g_aimVectorHalfPi]
        fld st(0)
        fsin
        fxch st(1)
        fcos
        fld dword ptr [ecx+4]
        fmul dword ptr [g_objectScaleLowerBound]
        fld st(1)
        fmul dword ptr [ecx+8]
        fsubp st(1),st(0)
        fstp dword ptr [eax]
        fld st(1)
        fmul dword ptr [ecx+8]
        fld dword ptr [ecx]
        fmul dword ptr [g_objectScaleLowerBound]
        fsubp st(1),st(0)
        fstp dword ptr [eax+4]
        fmul dword ptr [ecx]
        fxch st(1)
        fmul dword ptr [ecx+4]
        fsubp st(1),st(0)
        fstp dword ptr [eax+8]
        ret 8
    }
    // clang-format on
}
// 0x001181ac
extern const float g_objectMutationStep = 0.1f;
// 0x001181c4
extern const float g_objectRotationFactor = 10.0f;
// 0x0011f578
extern const float g_angleUpperBound = 180.0f;
// 0x0011f574
extern const float g_angleLowerBound = -180.0f;
// 0x001181b4
extern const float g_anglePeriod = 360.0f;
// 0x001179b0
extern const float g_objectScaleLowerBound = 0.0f;
// 0x000b44e0
__declspec(naked) float __cdecl GameNormalizeAngle(float angle)
{
    // clang-format off
    // Explicit x87 preserves the native extended ST0 result and unordered comparisons.
    __asm {
        fld dword ptr [esp+4]
        fcom g_angleUpperBound
        fnstsw ax
        test ah,41h
        jnz lower
        fsub g_anglePeriod
    lower:
        fcom g_angleLowerBound
        fnstsw ax
        test ah,5
        jp done
        fadd g_anglePeriod
    done:
        ret
    }
    // clang-format on
}
// 0x000b4850
void GameTransformPoint(CVector* out, const CMatrix* matrix, const CVector* delta)
{
    out->x = ((matrix->at.x * delta->z + matrix->up.x * delta->y) + matrix->right.x * delta->x) + matrix->position.x;
    out->y = ((matrix->at.y * delta->z + matrix->up.y * delta->y) + matrix->right.y * delta->x) + matrix->position.y;
    out->z = ((matrix->at.z * delta->z + matrix->up.z * delta->y) + matrix->right.z * delta->x) + matrix->position.z;
}

// Native 0.2x name retained; result stays extended in x87 ST0.
// 0x000b44b0
float FloatOffset(float first, float second)
{
    if (first >= second)
        return first - second;
    return second - first;
}

#include "game/game.h"
// 0x001553d8
const CVector g_rwRotationAxes[3] = {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
#pragma bss_seg(".pstate$h")
// 0x002aae14
float g_rwQueryAngle;
#pragma bss_seg()

// GTA SA virtual addresses: USA 7EB7C0; all other version values use 7EB800.
// 0x000b5320
void __cdecl GameRotateRwQuaternion(void* quaternion, int axis, float angle, int combine)
{
    const CVector* direction = &g_rwRotationAxes[axis];
    DWORD target = g_gameVersion == 1 ? 0x7eb7c0 : 0x7eb800;
    // Native cdecl GTA bridge keeps the four argument words and EDX call target.
    // clang-format off
    __asm {
        push combine
        push angle
        push direction
        push quaternion
        mov edx,target
        call edx
        pop edx
        pop edx
        pop edx
        pop edx
    }
    // clang-format on
}
// Shared fields are views into the existing native backing span, not new globals.
// GTA SA virtual addresses: USA 7F2720; all other version values use 7F2760.
// 0x000b5370
void __cdecl GameQueryRwMatrixAngle(const CMatrix* matrix, int axis, float* angle)
{
    CVector* queryAxis = (CVector*)(g_entityCollisionScratch + 0x101c);
    *queryAxis = g_rwRotationAxes[axis];
    DWORD target = g_gameVersion == 1 ? 0x7f2720 : 0x7f2760;
    // Shared axis/centre/result storage and native x87 publication are retained.
    // clang-format off
    __asm {
        lea eax,[g_entityCollisionScratch + 0x1010]
        push eax
        lea eax,g_rwQueryAngle
        push eax
        lea eax,[g_entityCollisionScratch + 0x101c]
        push eax
        push matrix
        mov edx,target
        call edx
        pop edx
        pop edx
        pop edx
        pop edx
        fld g_rwQueryAngle
        mov eax,angle
        fstp dword ptr [eax]
    }
    // clang-format on
}
// GTA SA virtual addresses: USA 7EBA80; all other version values use 7EBAC0.
// 0x000b53e0
void __cdecl GameQueryRwQuaternionAngle(const void* quaternion, CVector* axis, float* angle)
{
    DWORD target = g_gameVersion == 1 ? 0x7eba80 : 0x7ebac0;
    // Recovered cdecl GTA call and caller word cleanup.
    // clang-format off
    __asm {
        push angle
        push axis
        push quaternion
        mov edx,target
        call edx
        pop edx
        pop edx
        pop edx
    }
    // clang-format on
}
// GTA SA virtual addresses: USA 7F1FD0; all other version values use 7F2010.
// 0x000b5410
void __cdecl GameRotateRwMatrix(CMatrix* matrix, const CVector* axis, float angle)
{
    const CVector* direction = axis;
    DWORD target = g_gameVersion == 1 ? 0x7f1fd0 : 0x7f2010;
    // Native captures the axis before reusing its argument home for the target.
    // clang-format off
    __asm {
        push 1
        push angle
        push direction
        push matrix
        mov edx,target
        call edx
        pop edx
        pop edx
        pop edx
        pop edx
    }
    // clang-format on
}
// GTA SA virtual addresses: USA 7F1920; all other version values use 7F1960.
// 0x000b5450
void __cdecl GameOrthoNormalizeRwMatrix(CMatrix* out, const CMatrix* matrix)
{
    DWORD target = g_gameVersion == 1 ? 0x7f1920 : 0x7f1960;
    // Recovered cdecl GTA call and caller word cleanup.
    // clang-format off
    __asm {
        push matrix
        push out
        mov edx,target
        call edx
        pop edx
        pop edx
    }
    // clang-format on
}
// GTA SA virtual addresses: USA 7F22C0; all other version values use 7F2300.
// 0x000b5480
void __cdecl GameScaleRwMatrix(CMatrix* matrix, const CVector* scale)
{
    DWORD target = g_gameVersion == 1 ? 0x7f22c0 : 0x7f2300;
    // Recovered cdecl GTA call and caller word cleanup.
    // clang-format off
    __asm {
        push 1
        push scale
        push matrix
        mov edx,target
        call edx
        pop edx
        pop edx
        pop edx
    }
    // clang-format on
}

// GTA SA virtual address: 0x0059C910, ECX receives the vector.
// 0x000b4630
void GameNormalizeVector(CVector* vector)
{
    // clang-format off
    __asm {
        mov ecx,vector
        mov edx,0x59c910
        call edx
    }
    // clang-format on
}
// GTA SA virtual address: 0x0059C730, cdecl out/left/right, returns out in EAX.
// 0x000b4750
CVector* GameVectorCrossProduct(CVector* out, const CVector* left, const CVector* right)
{
    return ((CVector * (__cdecl*)(CVector*, const CVector*, const CVector*))0x59c730)(out, left, right);
}
// GTA SA virtual address: 0x0059C810, cdecl out/vector/matrix.
// 0x000b4790
CVector* GameTransformVectorByTranspose(CVector* out, const CVector* vector, const CMatrix* matrix)
{
    return ((CVector * (__cdecl*)(CVector*, const CVector*, const CMatrix*))0x59c810)(out, vector, matrix);
}
// Backend quaternion layout is deliberately opaque, as in the other RW adapters.
// GTA SA virtual addresses: USA 0x007EB5C0; all other values use 0x007EB600.
// 0x000b4d20
void GameRwQuaternionFromMatrix(void* quaternion, const CMatrix* matrix)
{
    DWORD target = g_gameVersion == 1 ? 0x7eb5c0 : 0x7eb600;
    // clang-format off
    __asm {
        push matrix
        push quaternion
        mov eax,target
        call eax
        pop edx
        pop edx
    }
    // clang-format on
}

// 0x001553d4
GameRwPointTransformBackend g_rwPointTransformBackend = (GameRwPointTransformBackend)0x7edd90;
// 0x001553fc
GameRwAxisRotateBackend g_rwAxisRotateBackend = (GameRwAxisRotateBackend)0x7f1fd0;
// 0x0011f58c
const float g_headingInverseHalfTurn = 0.0055555556900799274f;
// 0x0011f590
const float g_headingPi = 3.1415927410125732f;

// GTA SA virtual address: 0x007EDDC0, fixed vector transform (no translation).
// 0x000b5210
CVector* GameTransformRwVector(CVector* out, const CVector* vector, const CMatrix* matrix)
{
    return ((CVector * (__cdecl*)(CVector*, const CVector*, const CMatrix*))0x7eddc0)(out, vector, matrix);
}

// The version-2 switch is sticky: other versions do not reset this pointer.
// GTA SA virtual addresses: initial 0x007EDD90, version-2 target 0x007EDDD0.
// 0x000b5230
CVector* GameTransformRwPoint(CVector* out, const CVector* point, const CMatrix* matrix)
{
    if (g_gameVersion == 2)
        g_rwPointTransformBackend = (GameRwPointTransformBackend)0x7eddd0;
    return g_rwPointTransformBackend(out, point, 1, matrix);
}

// Raw register-preserving native ABI; shared fields are existing arena views.
// Input +1000, output +1004, target +1008 (native RVAs 2ABE18/1C/20).
// 0x000b5260
__declspec(naked) void GameRepeatRwMatrixInvert()
{
    // clang-format off
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
    // clang-format on
}

// Raw register-preserving native ABI; shared fields are existing arena views.
// Input +1000, output +1004, target +1008 (native RVAs 2ABE18/1C/20).
// 0x000b5280
__declspec(naked) void GameInvertRwMatrix(CMatrix* out, const CMatrix* matrix)
{
    // clang-format off
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
    // clang-format on
}

// Native x87 comparisons, arithmetic order and extended ST0 return retained.
// 0x000b54b0
__declspec(naked) float GameHeadingToRadians(float heading)
{
    // clang-format off
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
    // clang-format on
}

// Native unbounded axis lookup; target publication occurs before the call.
// GTA SA virtual addresses: USA 0x007F1FD0, fallback 0x007F2010.
// 0x000b52d0
void GameRotateRwMatrixOnAxis(CMatrix* matrix, int axis, float angle)
{
    const CVector* direction = &g_rwRotationAxes[axis];
    g_rwAxisRotateBackend = (GameRwAxisRotateBackend)(g_gameVersion == 1 ? 0x7f1fd0 : 0x7f2010);
    // Native reloads the published target into saved EBX after pushing arguments.
    // clang-format off
    __asm {
        push 1
        push angle
        push direction
        push matrix
        mov ebx,g_rwAxisRotateBackend
        call ebx
        pop ebx
        pop ebx
        pop ebx
        pop ebx
    }
    // clang-format on
}

// GTA SA virtual address: 0x007498E0 (fixed).
// 0x000b1010
int GameRwCountClumpAtomics(void* clump)
{
    int result = 0;
    // clang-format off
    __asm {
        push clump
        mov edx,0x7498e0
        call edx
        pop edx
        mov result,eax
    }
    // clang-format on
    return result;
}

// GTA SA virtual address: 0x00749B70 (fixed).
// 0x000b1090
void* GameRwForAllAtomics(void* clump, GameRwObjectCallback callback, void* data)
{
    void* result = 0;
    DWORD target = 0x749b70;
    // clang-format off
    __asm {
        push data
        push callback
        push clump
        mov eax,target
        call eax
        mov result,eax
        pop eax
        pop eax
        pop eax
    }
    // clang-format on
    return result;
}

// GTA SA virtual address: 0x007F1200; every non-USA version uses +0x40.
// 0x000b10c0
void* GameRwForAllFrameObjects(void* frame, GameRwObjectCallback callback, void* data)
{
    DWORD target = g_gameVersion == 1 ? 0x7f1200 : 0x7f1240;
    void* result;
    // clang-format off
    __asm {
        push data
        push callback
        push frame
        mov eax,target
        call eax
        mov result,eax
        pop eax
        pop eax
        pop eax
    }
    // clang-format on
    return result;
}

// GTA SA virtual address: 0x007F3820; every non-USA version uses +0x40.
// 0x000b1360
void GameRwDestroyTexture(void* texture)
{
    DWORD target = g_gameVersion == 1 ? 0x7f3820 : 0x7f3860;
    // clang-format off
    __asm {
        push texture
        mov edx,target
        call edx
        pop edx
    }
    // clang-format on
}

// GTA SA virtual address: 0x007F05A0; every non-USA version uses +0x40.
// 0x000b13d0
void GameRwDestroyFrame(void* frame)
{
    DWORD target = g_gameVersion == 1 ? 0x7f05a0 : 0x7f05e0;
    // clang-format off
    __asm {
        push frame
        mov edx,target
        call edx
        pop edx
    }
    // clang-format on
}

// GTA SA virtual address: 0x00749DC0; every non-USA version uses +0x50.
// 0x000b1400
void GameRwDestroyAtomic(void* atomic)
{
    DWORD target = g_gameVersion == 1 ? 0x749dc0 : 0x749e10;
    // clang-format off
    __asm {
        push atomic
        mov edx,target
        call edx
        pop edx
    }
    // clang-format on
}

// GTA SA virtual address: 0x007FB020; every non-USA version uses +0x40.
// 0x000b1470
void GameRwDestroyRaster(void* raster)
{
    DWORD target = g_gameVersion == 1 ? 0x7fb020 : 0x7fb060;
    // clang-format off
    __asm {
        push raster
        mov edx,target
        call edx
        pop edx
    }
    // clang-format on
}

// GTA SA virtual address: 0x007EE340; every non-USA version uses +0x40.
// 0x000b14f0
void GameRwClearCamera(void* camera, const void* color, DWORD flags)
{
    DWORD target = g_gameVersion == 1 ? 0x7ee340 : 0x7ee380;
    // clang-format off
    __asm {
        push flags
        push color
        push camera
        mov edx,target
        call edx
        pop edx
        pop edx
        pop edx
    }
    // clang-format on
}

// GTA SA virtual address: 0x007EE190; every non-USA version uses +0x40.
// 0x000b1520
void GameRwBeginCameraUpdate(void* camera)
{
    DWORD target = g_gameVersion == 1 ? 0x7ee190 : 0x7ee1d0;
    // clang-format off
    __asm {
        push camera
        mov edx,target
        call edx
        pop edx
    }
    // clang-format on
}

// GTA SA virtual address: 0x007EE180; every non-USA version uses +0x40.
// 0x000b1550
void GameRwEndCameraUpdate(void* camera)
{
    DWORD target = g_gameVersion == 1 ? 0x7ee180 : 0x7ee1c0;
    // clang-format off
    __asm {
        push camera
        mov edx,target
        call edx
        pop edx
    }
    // clang-format on
}

// GTA SA virtual address: 0x00749B20; every non-USA version uses +0x50.
// 0x000b15d0
void GameRwRenderClump(void* clump)
{
    DWORD target = g_gameVersion == 1 ? 0x749b20 : 0x749b70;
    // clang-format off
    __asm {
        push clump
        mov edx,target
        call edx
        pop edx
    }
    // clang-format on
}

// GTA SA virtual address: 0x0074A4F0; every non-USA version uses +0x50.
// 0x000b1740
void GameRwAddClumpLight(void* clump, void* light)
{
    DWORD target = g_gameVersion == 1 ? 0x74a4f0 : 0x74a540;
    void* result;
    // Native captures the backend return word but does not expose it.
    // clang-format off
    __asm {
        push light
        push clump
        mov edx,target
        call edx
        pop edx
        pop edx
        mov result,eax
    }
    // clang-format on
}

// GTA SA virtual address: 0x00751A70 (fixed).
// 0x000b17a0
void GameRwSetLightRadius(void* light, float radius)
{
    DWORD target = 0x751a70;
    // clang-format off
    __asm {
        push radius
        push light
        mov edx,target
        call edx
        pop edx
        pop edx
    }
    // clang-format on
}

// GTA SA virtual address: 0x0074A310; every non-USA version uses +0x50.
// 0x000b1880
BOOL GameRwDestroyClump(void* clump)
{
    int result = 0;
    DWORD target = g_gameVersion == 1 ? 0x74a310 : 0x74a360;
    // clang-format off
    __asm {
        push clump
        mov eax,target
        call eax
        mov result,eax
        pop eax
    }
    // clang-format on
    return result;
}

// Native object tag must be1 before the raw +48 field is read.
// B5E30 establishes this field as the atomic render callback; keep raw view.
// 0x000b1030
void* GameRwQueryAtomic48(const void* object)
{
    const BYTE* bytes = (const BYTE*)object;
    return bytes && *bytes == 1 ? *(void* const*)(bytes + 0x48) : 0;
}

// Nullable raw +34 read; no full backend object layout is inferred.
// 0x000b1130
void* __stdcall GameRwReadField34(const void* object)
{
    void* result = 0;
    if (object)
    {
        // Native RenderWare memory adapter; the backend slot is still opaque.
        // clang-format off
        __asm {
            mov eax,object
            mov edx,0x34
            lea esi,[edx+eax]
            mov eax,[esi]
            mov result,eax
        }
        // clang-format on
        return result;
    }
    return 0;
}

// Each entry snapshots GTA world slot C17038 once, and tests only that world.
// GTA SA virtual backend: 0x00750F20; non-USA fallback +0x50.
// 0x000b14b0
void GameRwWorldAddCamera(void* camera)
{
    void* world = *(void**)0xc17038;
    if (world)
    {
        DWORD target = g_gameVersion == 1 ? 0x750f20 : 0x750f70;
        // clang-format off
        __asm {
            push camera
            push world
            mov edx,target
            call edx
            pop edx
            pop edx
        }
        // clang-format on
    }
}
// GTA SA virtual backend: 0x00751300 (fixed for every version).
// 0x000b1600
void GameRwWorldAddClump(void* clump)
{
    void* world = *(void**)0xc17038;
    if (world)
    {
        // clang-format off
        __asm {
            push clump
            push world
            mov edx,0x751300
            call edx
            pop edx
            pop edx
        }
        // clang-format on
    }
}
// GTA SA virtual backend: 0x00751460 (fixed for every version).
// 0x000b1630
void GameRwWorldRemoveClump(void* clump)
{
    void* world = *(void**)0xc17038;
    if (world)
    {
        // clang-format off
        __asm {
            push clump
            push world
            mov edx,0x751460
            call edx
            pop edx
            pop edx
        }
        // clang-format on
    }
}
// GTA SA virtual backend: 0x0074F570 (fixed for every version).
// 0x000b1660
void GameRwWorldRender()
{
    void* world = *(void**)0xc17038;
    if (world)
    {
        // clang-format off
        __asm {
            push world
            mov edx,0x74f570
            call edx
            pop edx
        }
        // clang-format on
    }
}
// GTA SA virtual backend: 0x00751910; non-USA fallback +0x50.
// 0x000b17c0
void GameRwWorldAddLight(void* light)
{
    void* world = *(void**)0xc17038;
    if (world)
    {
        DWORD target = g_gameVersion == 1 ? 0x751910 : 0x751960;
        // clang-format off
        __asm {
            push light
            push world
            mov edx,target
            call edx
            pop edx
            pop edx
        }
        // clang-format on
    }
}
// GTA SA virtual backend: 0x00751960; non-USA fallback +0x50.
// 0x000b1800
void GameRwWorldRemoveLight(void* light)
{
    void* world = *(void**)0xc17038;
    if (world)
    {
        DWORD target = g_gameVersion == 1 ? 0x751960 : 0x7519b0;
        // clang-format off
        __asm {
            push light
            push world
            mov edx,target
            call edx
            pop edx
            pop edx
        }
        // clang-format on
    }
}

// GTA SA virtual address: 0x00749F70, fixed even for non-USA version values.
// 0x000b1850
void* GameRwCloneClump(void* clump)
{
    void* result = 0;
    if (!clump)
        return 0;
    // clang-format off
    __asm {
        push clump
        mov edx,0x749f70
        call edx
        pop edx
        mov result,eax
    }
    // clang-format on
    return result;
}

// Raw return ABI: stack cleanup POPs intentionally overwrite backend EAX.
// GTA SA engine slot C97B24, entry+20; slot purpose remains unresolved.
// Two raw words are passed, and the final popped second word is returned.
// 0x000b0ff0
__declspec(naked) DWORD GameRwInvokeEngineSlot20(DWORD first, DWORD second)
{
    // clang-format off
    __asm {
        push ebp
        mov ebp,esp
        mov edx,0xc97b24
        mov eax,dword ptr [edx]
        push dword ptr [ebp + 0xc]
        push dword ptr [ebp + 0x8]
        call dword ptr [eax + 0x20]
        pop eax
        pop eax
        pop ebp
        ret
    }
    // clang-format on
}
// GTA SA virtual address74C800. Return input word, not backend result.
// 0x000b1100
__declspec(naked) void* GameRwUnlockGeometry(void* geometry)
{
    // clang-format off
    __asm {
        push ebp
        mov ebp,esp
        push dword ptr [ebp + 0x8]
        mov eax,0x74c800
        call eax
        pop eax
        pop ebp
        ret
    }
    // clang-format on
}
// GTA SA virtual address74C7D0. Pass lock maskFFF, then return that word.
// 0x000b1110
__declspec(naked) DWORD GameRwLockGeometry(void* geometry)
{
    // clang-format off
    __asm {
        push ebp
        mov ebp,esp
        push 0xfff
        push dword ptr [ebp + 0x8]
        mov eax,0x74c7d0
        call eax
        pop eax
        pop eax
        pop ebp
        ret
    }
    // clang-format on
}

// GTA SA virtual address: 0x007328C0, static visibility camera setter.
// 0x000b1840
void GameRwSetVisibilityCamera(void* camera)
{
    // clang-format off
    __asm {
        push camera
        mov edx,0x7328c0
        call edx
        pop edx
    }
    // clang-format on
}
// GTA SA virtual addresses: hierarchy lookup734A40, matrix update7C51D0.
// No additional null hierarchy guard is native.
// 0x000b1980
void GameRwUpdateClumpHierarchy(void* clump)
{
    // clang-format off
    __asm {
        push clump
        mov edx,0x734a40
        call edx
        pop edx
        push eax
        mov edx,0x7c51d0
        call edx
        pop edx
    }
    // clang-format on
}
// GTA SA virtual address: 0x004D34F0; full native on-screen word is forwarded.
// 0x000b19a0
void GameRwUpdateClumpAnimations(void* clump, float timeStep, DWORD onScreen)
{
    // clang-format off
    __asm {
        push onScreen
        push timeStep
        push clump
        mov edx,0x4d34f0
        call edx
        pop edx
        pop edx
        pop edx
    }
    // clang-format on
}

// Return the address at +7C, not the pointer stored there; tag1 only.
// 0x000b1050
void* GameRwQueryAtomic7c(void* object)
{
    BYTE* bytes = (BYTE*)object;
    return bytes && *bytes == 1 ? bytes + 0x7c : 0;
}

// Snapshot frame before atomic destruction; clump path has no frame teardown.
// 0x000b5df0
void GameRwDestroyObject(void* object)
{
    if (!object)
        return;
    BYTE type = *(BYTE*)object;
    if (type == 1)
    {
        void* frame = *(void**)((BYTE*)object + 4);
        GameRwDestroyAtomic(object);
        if (frame)
            GameRwDestroyFrame(frame);
    }
    else if (type == 2)
        GameRwDestroyClump(object);
}

// Native callback is cdecl, unchecked, and its return value is ignored.
// 0x000b5e30
BOOL __stdcall GameRwRenderAtomic(void* object)
{
    if (!object || *(BYTE*)object != 1)
        return FALSE;
    void* callback = GameRwQueryAtomic48(object);
    // clang-format off
    __asm {
        push object
        mov ebx,callback
        call ebx
        pop ebx
    }
    // clang-format on
    return TRUE;
}

// Dispatch opaque backend tags1/2 only; native entry pops its one argument.
// 0x000b5e70
void __stdcall GameRwRenderObject(void* object)
{
    if (!object)
        return;
    BYTE type = *(BYTE*)object;
    if (type == 1)
        GameRwRenderAtomic(object);
    else if (type == 2)
        GameRwRenderClump(object);
}

// GTA SA virtual addresses: pool slot B74490, thiscall backend 4442D0.
// 0x000b37a0
int __stdcall GamePoolPedGetIndex(void* ped)
{
    int result;
    // clang-format off
    __asm {
        mov ebx,0xb74490
        mov ecx,dword ptr [ebx]
        push ped
        mov ebx,0x4442d0
        call ebx
        mov result,eax
    }
    // clang-format on
    return result;
}

// GTA SA virtual addresses: pool slot B74494, thiscall backend 424160.
// 0x000b3800
DWORD __stdcall GamePoolVehicleGetIndex(void* vehicle)
{
    DWORD result = 0;
    // clang-format off
    __asm {
        mov eax,0xb74494
        mov ecx,dword ptr [eax]
        push vehicle
        mov edx,0x424160
        call edx
        mov result,eax
    }
    // clang-format on
    return result;
}

// GTA SA virtual pool-pointer address: 0x00B74498.
// 0x000b3870
void* GamePoolBuildingGetArray()
{
    return **(void***)0xb74498;
}

// GTA SA virtual pool-pointer address: 0x00B744A0.
// 0x000b3890
void* GamePoolDummyGetArray()
{
    return **(void***)0xb744a0;
}

// GTA SA virtual pool-pointer address: 0x00B7449C.
// 0x000b38b0
void* GamePoolObjectGetArray()
{
    return **(void***)0xb7449c;
}

// Native scans exactly 3100 records of 412 bytes, independent of pool metadata.
// Preserve the signed model-word comparison and the 863C40 vtable exclusion.
// GTA SA virtual address: object-pool pointer B7449C.
// 0x000b3930
bool GamePoolHasObjectModel(int model)
{
    DWORD address = (DWORD) * *(void***)0xb7449c;
    for (DWORD index = 0; index < 3100; ++index, address += 0x19c)
    {
        if (address && *(DWORD*)address && *(DWORD*)address != 0x863c40 && *(short*)(address + 0x22) == model)
            return true;
    }
    return false;
}

// GTA SA virtual addresses: scan A9C068 inclusive through AAE948 exclusive.
// Exactly 19000 DWORD slots; do not use the relocated/custom SA-MP bank here.
// 0x000b3a00
int GameCountEmptyBackendModelSlots()
{
    int count = 0;
    for (DWORD* slot = (DWORD*)0xa9c068; slot != (DWORD*)0xaae948; ++slot)
    {
        if (*slot == 0)
            ++count;
    }
    return count;
}

// 0x002a9d98
int g_rwImageDepth;
// 0x002a9d9c
int g_rwImageWidth;
// 0x002a9a40
int g_rwImageHeight;
// 0x002aae10
void* g_rwImageScratch;
// 0x00155400
void** g_rwSceneCameraSlot = (void**)0xc1703c;

// Shared scratch views only; no independent global storage is introduced.
// RVAs 2ABE40, 2ABE48 and 2ABE4C.
#define SCENE_SOURCE_RASTER (*(void**)(g_entityCollisionScratch + 0x1028))
#define SCENE_COLOR_RASTER (*(void**)(g_entityCollisionScratch + 0x1030))
#define SCENE_DEPTH_RASTER (*(void**)(g_entityCollisionScratch + 0x1034))

// Native ignores all failures and reloads the published image between calls.
// GTA SA virtual addresses: image create/allocate/from-raster/BMP write/destroy.
// 0x000b5690
void GameWriteRasterBmp(void* raster, const char* filename)
{
    g_rwImageScratch = ((void*(__cdecl*)(int, int, int))0x8026e0)(g_rwImageWidth, g_rwImageHeight, g_rwImageDepth);
    ((void*(__cdecl*)(void*))0x8027a0)(g_rwImageScratch);
    ((void*(__cdecl*)(void*, void*))0x804250)(g_rwImageScratch, raster);
    ((void*(__cdecl*)(void*, const char*))0x7ce990)(g_rwImageScratch, filename);
    ((BOOL(__cdecl*)(void*))0x802740)(g_rwImageScratch);
}

// Allocate only when both raster slots are null. A partial success is retained.
// GTA SA virtual addresses: source raster slot C402D8; RwRasterCreate 7FB230.
// 0x000b57d0
void GameCreateSceneRasters()
{
    if (SCENE_COLOR_RASTER || SCENE_DEPTH_RASTER)
        return;
    SCENE_SOURCE_RASTER = *(void**)0xc402d8;
    BYTE* source = (BYTE*)SCENE_SOURCE_RASTER;
    g_rwImageDepth = *(int*)(source + 0x14);
    g_rwImageHeight = *(int*)(source + 0x10);
    g_rwImageWidth = *(int*)(source + 0xc);
    SCENE_COLOR_RASTER = ((void*(__cdecl*)(int, int, int, int))0x7fb230)(2048, 1024, g_rwImageDepth, 5);
    SCENE_DEPTH_RASTER = ((void*(__cdecl*)(int, int, int, int))0x7fb230)(2048, 1024, g_rwImageDepth, 1);
}

// Preserve all PUSHAD registers, camera publication, call order and saved-slot
// reloads. Shared saved slots at RVAs 2ABE50/54 make this path non-reentrant.
// GTA SA virtual address C9BCC0 is written but not restored by the native body.
// 0x000b5860
__declspec(naked) void GameRenderWithSceneRasters()
{
    // clang-format off
    __asm {
        push ebp
        mov ebp,esp
        push ecx
        push ebx
        push esi
        push edi
        pushad
        call GameCreateSceneRasters
        mov eax,g_rwSceneCameraSlot
        mov eax,dword ptr [eax]
        mov dword ptr [ebp + -0x4],eax
        mov dword ptr ds:[0x00c9bcc0],eax
        mov ebx,dword ptr [ebp + -0x4]
        mov edx,dword ptr [ebx + 0x60]
        mov dword ptr [g_entityCollisionScratch+0x1038],edx
        mov edx,dword ptr [ebx + 0x64]
        mov dword ptr [g_entityCollisionScratch+0x103c],edx
        mov edx,dword ptr [g_entityCollisionScratch+0x1030]
        mov dword ptr [ebx + 0x60],edx
        mov edx,dword ptr [g_entityCollisionScratch+0x1034]
        mov dword ptr [ebx + 0x64],edx
        mov edx,0x734650
        call edx
        mov edx,0x53df40
        call edx
        mov edx,0x732f30
        call edx
        mov ebx,dword ptr [ebp + -0x4]
        mov edx,dword ptr [g_entityCollisionScratch+0x1038]
        mov dword ptr [ebx + 0x60],edx
        mov edx,dword ptr [g_entityCollisionScratch+0x103c]
        mov dword ptr [ebx + 0x64],edx
        popad
        pop edi
        pop esi
        pop ebx
        mov esp,ebp
        pop ebp
        ret
    }
    // clang-format on
}

// 0x002aae18
BYTE g_entityCollisionScratch[0x1044];

// 0x000b3e40
DWORD __stdcall GameTestCameraSphere(const CVector* centre, float radius)
{
    DWORD sphere[4];
    memcpy(sphere, centre, 12);
    memcpy(sphere + 3, &radius, 4);
    // GTA SA virtual address: current RenderWare camera pointer.
    void* camera = *(void**)0xc1703c;
    if (!camera)
        return 0;
    DWORD target = g_gameVersion == 1 ? 0x7ee2d0 : 0x7ee310;
    return ((DWORD(__cdecl*)(void*, const void*))target)(camera, sphere);
}

// 0x000b1170
void __stdcall GameRwReplaceField34(void* object, void* replacement)
{
    IUnknown* previous = 0;
    if (object)
    {
        // clang-format off
        __asm {
            mov eax,object
            mov edx,0x34
            lea esi,[edx+eax]
            mov eax,[esi]
            mov previous,eax
        }
        // clang-format on
        if (previous)
        {
            previous->Release();
            // clang-format off
            __asm {
                mov eax,object
                mov edx,0x34
                lea esi,[edx+eax]
                mov eax,replacement
                mov [esi],eax
            }
            // clang-format on
        }
    }
}
// 0x000b11c0
void* GameRwDumpAtomicMaterials(void* object, void* data)
{
    BYTE* atomic = (BYTE*)object;
    if (atomic[0] == 1)
    {
        if (*(void**)(atomic + 0x18))
        {
            int count = *(int*)(*(BYTE**)(atomic + 0x18) + 0x24);
            if (g_pChat)
                g_pChat->AddDebugMessage(g_rwMaterialCountText, count);
            for (int i = 0; i < count; ++i)
            {
                BYTE* geometry = *(BYTE**)(atomic + 0x18);
                BYTE* material = (*(BYTE***)(geometry + 0x20))[i];
                if (material)
                {
                    BYTE* texture = *(BYTE**)material;
                    if (texture)
                    {
                        if (g_pChat)
                            g_pChat->AddDebugMessage(g_rwMaterialNameText, i, (char*)texture + 0x10);
                    }
                    else if (g_pChat)
                        g_pChat->AddDebugMessage(g_rwMaterialNullText, i);
                }
            }
        }
    }
    return object;
}
// 0x000b1260
void* __stdcall GameRwDumpObjectFrameMaterials(void* object)
{
    void* frame = *(void**)((BYTE*)object + 4);
    if (frame)
        frame = GameRwForAllFrameObjects(frame, GameRwDumpAtomicMaterials, 0);
    return frame;
}
// 0x000b1280
void* GameRwKeepType1Object(void* object, void* data)
{
    return *(BYTE*)object == 1 ? object : 0;
}
// 0x000b12a0
void* __stdcall GameRwEnumerateObjectFrameType1(void* object)
{
    void* frame = *(void**)((BYTE*)object + 4);
    if (!frame)
        return frame;
    return GameRwForAllFrameObjects(frame, GameRwKeepType1Object, 0);
}
// 0x000b1970
void* GameRwGetObjectFrame(const void* object)
{
    if (!object)
        return (void*)object;
    return *(void**)((const BYTE*)object + 4);
}
// 0x000b1e90
void* GameRwGetClumpWord8Minus64(void* clump, int index)
{
    int count = GameRwCountClumpAtomics(clump);
    if (!count || index > count)
        return 0;
    void* result = 0;
    // clang-format off
    __asm {
        mov eax,clump
        mov edx,[eax+8]
        lea eax,[edx-0x40]
        mov result,eax
    }
    // clang-format on
    return result;
}

// 0x0011f414
const char g_rwMaterialCountText[] = "NumMaterials = %d";
// 0x0011f3f0
const char g_rwMaterialNameText[] = "Material Index: %d TextureName: %s";
// 0x0011f3cc
const char g_rwMaterialNullText[] = "Material Index: %d Texture: NULL";
// Native x87 intermediate precision, spill locations and input bit pattern matter.
// 0x000b55a0
__declspec(naked) float GameApproximateSqrt(float value)
{
    // clang-format off
    __asm {
        fld dword ptr [esp+4]
        mov eax,dword ptr [esp+4]
        fld dword ptr [esp+4]
        sar eax,1
        fmul g_textDrawHalf
        mov ecx,0x5f3759d5
        sub ecx,eax
        mov dword ptr [esp+4],ecx
        fmul dword ptr [esp+4]
        fmul dword ptr [esp+4]
        fsubr g_dialogLineScale
        fmul dword ptr [esp+4]
        fmulp st(1),st
        ret
    }
    // clang-format on
}

// 0x000b62f0
BOOL GameCopyBackendBoneMatrix(void* entity, CMatrix* out, int bone)
{
    if (!entity || *(DWORD*)entity == 0x863c40)
        return FALSE;
    void* clump = *(void**)((BYTE*)entity + 0x18);
    if (!clump)
        return FALSE;
    // GTA SA virtual addresses; select index backend before the first callback.
    DWORD indexTarget = g_gameVersion == 1 ? 0x7c51a0 : 0x7c51e0;
    void* hierarchy = ((void*(__cdecl*)(void*))0x734a40)(clump);
    DWORD index = ((DWORD(__cdecl*)(void*, int))indexTarget)(hierarchy, bone);
    DWORD matrices = *(DWORD*)((BYTE*)hierarchy + 8);
    memcpy(out, (void*)(matrices + (index << 6)), 64);
    return TRUE;
}

// Preserves raw EAX from the fixed GTA backend; argument meaning unresolved.
// GTA SA virtual address: 0x00705B60.
// 0x000b14a0
DWORD GameCall705B60(DWORD argument)
{
    return ((DWORD(__cdecl*)(DWORD))0x705b60)(argument);
}
// Backend consumes three stack DWORDs (RET12); semantic types are unknown.
// GTA SA virtual address: 0x0059C790.
// 0x000b4770
DWORD GameCall59C790(DWORD first, DWORD second, DWORD third)
{
    return ((DWORD(__stdcall*)(DWORD, DWORD, DWORD))0x59c790)(first, second, third);
}
// 0x000b4360
void GameReplaceBuildingPoolModels(int model, int replacement)
{
    CEntity temporary;
    // GTA SA virtual address: pool global0x00B74498; native starts at pool+4.
    // Preserve this observed slot instead of substituting the pool array getter.
    DWORD cursor = *(DWORD*)(*(DWORD*)0xb74498 + 4);
    for (int remaining = 0x36b0; remaining; --remaining, cursor += 0x38)
    {
        if (cursor && *(short*)(cursor + 0x22) == model)
        {
            temporary.m_pGameEntity = (void*)cursor;
            temporary.Remove();
            temporary.SetModelIndex(replacement);
        }
    }
}

#include "game/quaternion.h"

// Returns the unspilled extended difference.
// 0x000b44a0
float GameSubtractFloats(float first, float second)
{
    return first - second;
}

// dy squared plus dx squared, no intermediate float spill.
// 0x000b4510
float __stdcall GameSquaredDistance2D(float x, float y, float otherX, float otherY)
{
    return (y - otherY) * (y - otherY) + (x - otherX) * (x - otherX);
}

// Square root of dy squared plus dx squared.
// 0x000b4540
float __stdcall GameDistance2D(float x, float y, float otherX, float otherY)
{
    return (float)sqrt((y - otherY) * (y - otherY) + (x - otherX) * (x - otherX));
}

// dz squared plus dy squared then dx squared.
// 0x000b45b0
__declspec(naked) float __stdcall GameSquaredDistance3D(float x, float y, float z, float otherX, float otherY, float otherZ)
{
    // clang-format off
    __asm {
        fld dword ptr [esp + 4]
        fsub dword ptr [esp + 0x10]
        fld dword ptr [esp + 8]
        fsub dword ptr [esp + 0x14]
        fld dword ptr [esp + 0xc]
        fsub dword ptr [esp + 0x18]
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st
        fld st(3)
        fmul st(0),st(4)
        faddp st(1),st
        fstp st(3)
        fstp st(0)
        fstp st(0)
        ret 0x18
    }
    // clang-format on
}

// dx squared plus dy squared then dz squared; RET8.
// 0x000b45f0
__declspec(naked) float __stdcall GameSquaredDistanceVectors(const CVector* first, const CVector* second)
{
    // clang-format off
    __asm {
        mov eax, dword ptr [esp + 4]
        mov ecx, dword ptr [esp + 8]
        fld dword ptr [eax + 8]
        fsub dword ptr [ecx + 8]
        fld dword ptr [eax + 4]
        fsub dword ptr [ecx + 4]
        fld dword ptr [eax]
        fsub dword ptr [ecx]
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st
        fld st(3)
        fmul st(0),st(4)
        faddp st(1),st
        fstp st(3)
        fstp st(0)
        fstp st(0)
        ret 8
    }
    // clang-format on
}

// x squared plus y squared then z squared.
// 0x000b4640
float GameVectorLengthSquared(const CVector* vector)
{
    return D3DXVec3LengthSq((const D3DXVECTOR3*)vector);
}

// Inclusive signed unit bounds for all components; unordered values rejected.
// 0x000b4670
bool GameIsVectorWithinUnitBounds(const CVector* vector)
{
    return vector->x <= 1.0f && vector->x >= -1.0f && vector->y <= 1.0f && vector->y >= -1.0f && vector->z <= 1.0f && vector->z >= -1.0f;
}

// Sequential stores and input reloads preserve native alias behavior.
// 0x000b47b0
__declspec(naked) void GameVectorCrossProductLocal(CVector* out, const CVector* left, const CVector* right)
{
    // clang-format off
    __asm {
        mov ecx, dword ptr [esp + 0xc]
        mov eax, dword ptr [esp + 8]
        fld dword ptr [ecx + 8]
        fmul dword ptr [eax + 4]
        mov edx, dword ptr [esp + 4]
        fld dword ptr [ecx + 4]
        fmul dword ptr [eax + 8]
        fsubp st(1),st
        fstp dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [eax + 8]
        fld dword ptr [ecx + 8]
        fmul dword ptr [eax]
        fsubp st(1),st
        fstp dword ptr [edx + 4]
        fld dword ptr [ecx + 4]
        fmul dword ptr [eax]
        fld dword ptr [ecx]
        fmul dword ptr [eax + 4]
        fsubp st(1),st
        fstp dword ptr [edx + 8]
        ret
    }
    // clang-format on
}

// Square root of x squared plus y squared then z squared.
// 0x000b47f0
float GameVectorLength(const CVector* vector)
{
    return D3DXVec3Length((const D3DXVECTOR3*)vector);
}

// Separate native entry, same arithmetic as RVA000B4640; original name unknown.
// 0x000b4820
float GameVectorLengthSquared4820(const CVector* vector)
{
    return D3DXVec3LengthSq((const D3DXVECTOR3*)vector);
}

// x squared plus y squared then z squared; cdecl.
// 0x000b4b20
float GameLengthSquared3D(float x, float y, float z)
{
    return (x * x + y * y) + z * z;
}

// z products plus y products then x products.
// 0x000b4b40
float GameVectorDotProduct(const CVector* first, const CVector* second)
{
    return D3DXVec3Dot((const D3DXVECTOR3*)first, (const D3DXVECTOR3*)second);
}

// Three sequential component stores; native input/output alias behavior.
// 0x000b4cf0
void GameScaleVector(CVector* out, const CVector* vector, float scale)
{
    out->x = scale * vector->x;
    out->y = scale * vector->y;
    out->z = scale * vector->z;
}

// Native inclusive0..360 gate accepts NaN; preserves branch arithmetic.
// 0x000b5520
__declspec(naked) float GameHeadingToRadiansBounded(float heading)
{
    // clang-format off
    __asm {
        fld dword ptr [esp + 4]
        fcomp g_anglePeriod
        fnstsw ax
        test ah, 0x41
        je math_b558b
        fld dword ptr [esp + 4]
        fcomp g_objectScaleLowerBound
        fnstsw ax
        test ah, 5
        jnp math_b558b
        fld dword ptr [esp + 4]
        fcomp g_angleUpperBound
        fld dword ptr [esp + 4]
        fnstsw ax
        test ah, 0x41
        jne math_b5578
        fsub g_angleUpperBound
        fmul g_headingPi
        fmul g_headingInverseHalfTurn
        fsubr g_headingPi
        fchs
        fmul g_entityEulerSign
        ret
    math_b5578:
        fmul g_headingPi
        fmul g_headingInverseHalfTurn
        fmul g_entityEulerSign
        ret
    math_b558b:
        fld g_objectScaleLowerBound
        ret
    }
    // clang-format on
}

// 0x000b38d0
DWORD GameCountNonzeroPoolBytesB7449C()
{
    // GTA SA virtual address: pool global. Bytes are inline at +4, not indirect.
    BYTE* pool = *(BYTE**)0xb7449c;
    int capacity = *(int*)(pool + 8);
    DWORD count = 0;
    for (int index = 0; index < capacity; ++index)
        if (pool[index + 4])
            ++count;
    return count;
}
// 0x000b3900
DWORD GameCountNonzeroPoolBytesC8800C()
{
    // GTA SA virtual address: pool global. Preserve the same raw inline-byte scan.
    BYTE* pool = *(BYTE**)0xc8800c;
    int capacity = *(int*)(pool + 8);
    DWORD count = 0;
    for (int index = 0; index < capacity; ++index)
        if (pool[index + 4])
            ++count;
    return count;
}
// 0x000b4130
void __stdcall GameReplaceBackendVtable85BBF0(void* entity)
{
    // GTA SA virtual addresses: source and replacement vtables.
    if (entity && *(DWORD*)entity == 0x85bbf0)
        *(DWORD*)entity = 0x85bd30;
}
// 0x000b42c0
void GameWriteGlobal9689E0(DWORD value)
{
    // GTA SA virtual address; semantic meaning of this raw DWORD is unresolved.
    *(DWORD*)0x9689e0 = value;
}
// CRT helper consumes and returns ST0, not a C stack argument.
// 0x000fce60
extern "C" void __cdecl _CIasin();
// 0x000b3620
float GameAsin(float value)
{
    return (float)asin(value);
}

// Raw GTA backend, sequential position publication and live matrix reloads.
// 0x000b6260
__declspec(naked) void GameGetBackendPositionAndHeading(void* entity, CVector* position, float* heading)
{
    // clang-format off
    __asm {
        mov eax, dword ptr [esp + 4]
        mov ecx, dword ptr [eax + 0x14]
        test ecx, ecx
        je placement_b62cd
        mov edx, dword ptr [ecx + 0x30]
        mov ecx, dword ptr [esp + 8]
        mov dword ptr [ecx], edx
        mov edx, dword ptr [eax + 0x14]
        mov edx, dword ptr [edx + 0x34]
        mov dword ptr [ecx + 4], edx
        mov edx, dword ptr [eax + 0x14]
        mov edx, dword ptr [edx + 0x38]
        mov dword ptr [ecx + 8], edx
        mov eax, dword ptr [eax + 0x14]
        fld dword ptr [eax + 0x10]
        fchs
        fld dword ptr [eax + 0x14]
        fpatan
        fmul g_entityRadiansToDegrees
        fcom g_objectScaleLowerBound
        fnstsw ax
        test ah, 5
        jp placement_b62b3
        fadd g_anglePeriod
        mov eax, dword ptr [esp + 0xc]
        fstp dword ptr [eax]
        ret
    placement_b62b3:
        fcom g_anglePeriod
        fnstsw ax
        test ah, 1
        jne placement_b62c6
        fsub g_anglePeriod
    placement_b62c6:
        mov eax, dword ptr [esp + 0xc]
        fstp dword ptr [eax]
        ret
    placement_b62cd:
        mov edx, dword ptr [eax + 4]
        mov ecx, dword ptr [esp + 8]
        mov dword ptr [ecx], edx
        mov edx, dword ptr [eax + 8]
        mov dword ptr [ecx + 4], edx
        mov edx, dword ptr [eax + 0xc]
        mov dword ptr [ecx + 8], edx
        mov eax, dword ptr [eax + 0x10]
        mov ecx, dword ptr [esp + 0xc]
        mov dword ptr [ecx], eax
        ret
    }
    // clang-format on
}

// 0x000b3720
void __stdcall GameWorldAdd(void* entity)
{
    // GTA SA cdecl backend, one caller-popped pointer.
    // clang-format off
    __asm {
        push entity
        mov ebx,0x563220
        call ebx
        pop ebx
    }
    // clang-format on
}
// 0x000b3740
void __stdcall GameWorldRemove(void* entity)
{
    // GTA SA cdecl backend, one caller-popped pointer.
    // clang-format off
    __asm {
        push entity
        mov ebx,0x563280
        call ebx
        pop ebx
    }
    // clang-format on
}

// 0x000b3830
void* __stdcall GamePoolObjectGetAt(int handle)
{
    void* result;
    // GTA SA object pool B7449C and lookup465040, native thiscall bridge.
    // clang-format off
    __asm {
        mov ebx,0xb7449c
        mov ecx,[ebx]
        push handle
        mov ebx,0x465040
        call ebx
        mov result,eax
    }
    // clang-format on
    return result;
}

#include "game/util.h"
// GTA SA virtual addresses: GetPad 0x0053FB70; Clear 0x00541A70.
// VC7 ABI bridge: GetPad(0), then ECX=pad and two unmodified argument words.
// 0x000b58e0
__declspec(naked) void GameClearLocalPad(DWORD first, DWORD second)
{
    // clang-format off
    __asm {
        push ebp
        mov ebp,esp
        push 0x0
        mov edx,0x53fb70
        call edx
        mov ecx,eax
        pop edx
        push dword ptr [ebp + 0xc]
        push dword ptr [ebp + 0x8]
        mov edx,0x541a70
        call edx
        pop ebp
        ret
    }
    // clang-format on
}

#include "game/util.h"

// VC7 cannot express a free-function __thiscall pointer. These thin native
// adapters set ECX and keep the backend callee-pop and raw AL return intact.

// GTA SA virtual address: 0x0073FB10.
// 0x000b6020
__declspec(naked) bool __stdcall GameWeaponFireInstantHit(void* weapon, void* shooter, const CVector* origin, const CVector* muzzle, void* victim, const CVector* target,
                                                          const CVector* driveBy, DWORD crossHair, DWORD effects)
{
    // clang-format off
    __asm {
        push ebp
        mov ebp,esp
        push dword ptr [ebp + 0x28]
        push dword ptr [ebp + 0x24]
        push dword ptr [ebp + 0x20]
        push dword ptr [ebp + 0x1c]
        push dword ptr [ebp + 0x18]
        push dword ptr [ebp + 0x14]
        push dword ptr [ebp + 0x10]
        push dword ptr [ebp + 0xc]
        mov ecx,dword ptr [ebp + 0x8]
        mov edx,0x73fb10
        call edx
        pop ebp
        ret 0x24
    }
    // clang-format on
}

// GTA SA virtual address: 0x0073AAC0.
// 0x000b6050
__declspec(naked) bool __stdcall GameWeaponFireSniper(void* weapon, void* shooter)
{
    // clang-format off
    __asm {
        push ebp
        mov ebp,esp
        push 0x0
        push 0x0
        push dword ptr [ebp + 0xc]
        mov ecx,dword ptr [ebp + 0x8]
        mov edx,0x73aac0
        call edx
        pop ebp
        ret 0x8
    }
    // clang-format on
}

#include "game/util.h"
#include <string.h>
// 0x00151b20
DWORD g_playerDefaultColors[1000] = {
#include "game/player_color_values.inc"
};
// 0x0018f6c0
DWORD g_playerColors[1000];
// 0x000a6fc0
void PlayerColors::Initialize()
{
    memcpy(g_playerColors, g_playerDefaultColors, sizeof(g_playerColors));
}
// 0x000a6f50
void __stdcall PlayerColors::Set(DWORD index, DWORD color)
{
    if (index < sizeof(g_playerColors))
        g_playerColors[index] = color;
}
// 0x000a6f70
DWORD __stdcall PlayerColors::Get(DWORD index)
{
    if (index == 1004)
        return 0x89a4c440;
    if (index == 1005)
        return 0xaa0000ff;
    if (index == 1006)
        return 0xe2c063ff;
    if (index < sizeof(g_playerColors))
        return g_playerColors[index];
    return index;
}

#include "game/game.h"
#include "game/util.h"
// 0x0015535c
const CVector g_rwPreviewAxes[3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
// 0x000b1700
void* GameRwCreateLight(int lightType)
{
    void* result = 0;
    DWORD target = g_gameVersion == 1 ? 0x752110 : 0x752160;
    // clang-format off
    __asm {
        push lightType
        mov edx,target
        call edx
        pop edx
        mov result,eax
    }
    // clang-format on
    return result;
}
// 0x000b1770
void GameRwSetLightColor(void* light, const float* rgba)
{
    DWORD target = g_gameVersion == 1 ? 0x751a90 : 0x751ae0;
    // clang-format off
    __asm {
        push rgba
        push light
        mov edx,target
        call edx
        pop edx
        pop edx
    }
    // clang-format on
}
// 0x000b12d0
void* GameRwCreateRaster(int width, int height, int depth, int flags)
{
    void* result = 0;
    DWORD target = g_gameVersion == 1 ? 0x7fb230 : 0x7fb270;
    // clang-format off
    __asm {
        push flags
        push depth
        push height
        push width
        mov edx,target
        call edx
        mov result,eax
        pop edx
        pop edx
        pop edx
        pop edx
    }
    // clang-format on
    return result;
}
// 0x000b1430
void* GameRwCreateCamera()
{
    void* result = 0;
    DWORD target = g_gameVersion == 1 ? 0x7ee4f0 : 0x7ee530;
    // clang-format off
    __asm {
        mov edx,target
        call edx
        mov result,eax
    }
    // clang-format on
    return result;
}
// 0x000b1390
void* GameRwCreateFrame()
{
    void* result = 0;
    DWORD target = g_gameVersion == 1 ? 0x7f0410 : 0x7f0450;
    // clang-format off
    __asm {
        mov edx,target
        call edx
        mov result,eax
    }
    // clang-format on
    return result;
}
// 0x000b18f0
void GameRwTranslateFrame(void* frame, const CVector* position, int combine)
{
    DWORD target = g_gameVersion == 1 ? 0x7f0e30 : 0x7f0e70;
    // clang-format off
    __asm {
        push combine
        push position
        push frame
        mov edx,target
        call edx
        pop edx
        pop edx
        pop edx
    }
    // clang-format on
}
// 0x000b1920
void GameRwRotateFrame(void* frame, int axis, float angle)
{
    const CVector* direction = g_rwPreviewAxes + axis;
    DWORD target = g_gameVersion == 1 ? 0x7f1010 : 0x7f1050;
    // clang-format off
    __asm {
        push 1
        push angle
        push direction
        push frame
        mov edx,target
        call edx
        pop edx
        pop edx
        pop edx
        pop edx
    }
    // clang-format on
}
// 0x000b15b0
void GameRwSetCameraRasters(void* camera, void* color, void* depth)
{
    // clang-format off
    __asm {
        mov ebx,camera
        mov edx,color
        mov [ebx+0x60],edx
        mov edx,depth
        mov [ebx+0x64],edx
    }
    // clang-format on
}
// 0x000b1580
void GameRwSetObjectFrame(void* object, void* frame)
{
    DWORD target = g_gameVersion == 1 ? 0x804ef0 : 0x804f30;
    // clang-format off
    __asm {
        push frame
        push object
        mov edx,target
        call edx
        pop edx
        pop edx
    }
    // clang-format on
}
// 0x000b1680
void GameRwSetClipDistances(void* camera, float farClip, float nearClip)
{
    DWORD farAddress, nearAddress;
    if (g_gameVersion == 1)
    {
        farAddress = 0x7ee2a0;
        nearAddress = 0x7ee1d0;
    }
    else
    {
        farAddress = 0x7ee2e0;
        nearAddress = 0x7ee210;
    }
    // clang-format off
    __asm {
        push farClip
        push camera
        mov edx,farAddress
        call edx
        pop edx
        pop edx
        push nearClip
        push camera
        mov edx,nearAddress
        call edx
        pop edx
        pop edx
    }
    // clang-format on
}
// 0x000b16d0
void GameRwSetViewWindow(void* camera, const float* window)
{
    DWORD target = g_gameVersion == 1 ? 0x7ee410 : 0x7ee450;
    // clang-format off
    __asm {
        push window
        push camera
        mov edx,target
        call edx
        pop edx
        pop edx
    }
    // clang-format on
}
// 0x000b18c0
void GameRwSetProjection(void* camera, int projection)
{
    DWORD target = g_gameVersion == 1 ? 0x7ee3a0 : 0x7ee3e0;
    // clang-format off
    __asm {
        push projection
        push camera
        mov edx,target
        call edx
        pop edx
        pop edx
    }
    // clang-format on
}
// 0x000b1320
void* GameRwCreateTexture(void* raster)
{
    void* result = 0;
    DWORD target = g_gameVersion == 1 ? 0x7f37c0 : 0x7f3800;
    // clang-format off
    __asm {
        push raster
        mov edx,target
        call edx
        mov result,eax
        pop edx
    }
    // clang-format on
    return result;
}

// 0x000b2790
BOOL GameLoadTextureDictionary(const char* name, const char* filename, BYTE fromSamp)
{
    char directory[65], path[129];
    if (fromSamp)
        strcpy(directory, "SAMP\\");
    else
        strcpy(directory, "models\\txd");
    int index = GameTxdFind(name);
    if (index != -1)
        return TRUE;
    index = GameTxdAdd(name);
    sprintf(path, "%s\\%s.txd", directory, filename);
    if (GameFileExists(path))
    {
        GameTxdLoad(index, path);
        GameTxdAddRef(index);
        return TRUE;
    }
    return FALSE;
}

// 0x000b2860
void* GameLoadDictionaryTexture(const char* dictionary, const char* name)
{
    int index = GameTxdFind(dictionary);
    if (index == -1)
        return 0;
    GameTxdPush();
    GameTxdSetCurrent(index);
    void* texture = GameTxdReadTexture(name);
    GameTxdPop();
    return texture;
}

// 0x000b3480
int GameTxdFind(const char* name)
{
    int result;
    // Native GTA adapter: preserve the EDX call target and caller cleanup.
    // clang-format off
    __asm {
        push name
        mov edx,0x731850
        call edx
        pop edx
        mov result,eax
    }
    // clang-format on
    return result;
}

// 0x000b3510
void GameTxdPush()
{
    // clang-format off
    __asm {
        mov edx,0x7316a0
        call edx
    }
    // clang-format on
}

// 0x000b3520
void GameTxdPop()
{
    // clang-format off
    __asm {
        mov edx,0x7316b0
        call edx
    }
    // clang-format on
}

// 0x000b3530
void GameTxdSetCurrent(int index)
{
    // clang-format off
    __asm {
        push index
        mov edx,0x7319c0
        call edx
        pop edx
    }
    // clang-format on
}

// 0x000b3540
void* GameTxdReadTexture(const char* name)
{
    // Native constructs a one-pointer CSprite2d on the stack; this is an out-object, not the EAX result.
    void* texture = 0;
    // clang-format off
    __asm push name
    __asm lea ecx,texture
    __asm mov edx,0x727270
    __asm call edx
        // clang-format on
        return texture;
}

// Texture dictionary reference/slot adapters. Only the native -1 gates are
// applied; other indices pass through unchanged to their GTA backends.
// 0x000b34f0
void GameTxdAddRef(int index)
{
    // clang-format off
    __asm {
        push index
        mov edx,0x731a00
        call edx
        pop edx
    }
    // clang-format on
}

// 0x000b3500
void GameTxdRemoveRef(int index)
{
    // clang-format off
    __asm {
        push index
        mov edx,0x731a30
        call edx
        pop edx
    }
    // clang-format on
}

// 0x000b35b0
void GameTxdRemove(int index)
{
    if (index != -1)
        ((void(__cdecl*)(int))0x731e90)(index);
}

// 0x000b35d0
void GameTxdRemoveSlot(int index)
{
    if (index != -1)
        ((void(__cdecl*)(int))0x731cd0)(index);
}

// 0x000b35f0
DWORD GameTxdGetReferenceCount(int index)
{
    return index == -1 ? 0xffffffffu : ((DWORD(__cdecl*)(int))0x731aa0)(index);
}

// Native performs name lookup, then removes the slot only when it is not -1.
// 0x000b3580
void GameTxdRemoveSlotByName(const char* name)
{
    int index = GameTxdFind(name);
    if (index != -1)
        ((void(__cdecl*)(int))0x731cd0)(index);
}

// GTA SA virtual addresses: USA7C7550, fallback7C7590. Exactly one argument.
// 0x000b1ce0
void* GameRwGetGeometrySkin(void* geometry)
{
    DWORD target = g_gameVersion == 1 ? 0x7c7550 : 0x7c7590;
    return ((void*(__cdecl*)(void*))target)(geometry);
}

// GTA SA virtual addresses: backend74BDD0/+50; initialization flag C97B20.
// Callback addresses are opaque GTA code pointers; no invented callable types.
// 0x000b1da0
int GameRwRegisterClumpPlugin(DWORD pluginId)
{
    DWORD target = g_gameVersion == 1 ? 0x74bdd0 : 0x74be20;
    *(DWORD*)0xc97b20 = 0;
    int result = ((int(__cdecl*)(DWORD, DWORD, DWORD, DWORD, DWORD))target)(0, pluginId, 0x41b1a0, 0x41b1c0, 0x41b1b0);
    *(DWORD*)0xc97b20 = 1;
    return result;
}

// GTA SA virtual addresses: backend74BE70/+50, callbacks41B2F0/41B300.
// Native forces the flag back to1, not to its previous value, even on failure.
// 0x000b1e00
int GameRwRegisterClumpPluginStream(DWORD pluginId, DWORD readCallback)
{
    DWORD target = g_gameVersion == 1 ? 0x74be70 : 0x74bec0;
    *(DWORD*)0xc97b20 = 0;
    int result = ((int(__cdecl*)(DWORD, DWORD, DWORD, DWORD))target)(pluginId, readCallback, 0x41b2f0, 0x41b300);
    *(DWORD*)0xc97b20 = 1;
    return result;
}

// GTA SA virtual addresses: USA7F3560, fallback7F35A0; fixed enable argument1.
// 0x000b1e60
void GameRwEnableAutoMipmapping()
{
    DWORD target = g_gameVersion == 1 ? 0x7f3560 : 0x7f35a0;
    ((void(__cdecl*)(DWORD))target)(1);
}

// GTA SA virtual addresses: USA734820, all other version values734880 (+60).
// 0x000b1ca0
void* GameRwGetFirstAtomic(void* clump)
{
    DWORD target = g_gameVersion == 1 ? 0x734820 : 0x734880;
    return ((void*(__cdecl*)(void*))target)(clump);
}

// GTA SA virtual address: 0x00734B10, exactly one clump argument.
// 0x000b1d40
void* GameRwGetClumpAnimHierarchy(void* clump)
{
    return ((void*(__cdecl*)(void*))0x734b10)(clump);
}

// GTA SA virtual address: 0x004D60E0, exactly one hierarchy argument.
// 0x000b1d70
void* GameRwCreateHierarchyAnimation(void* hierarchy)
{
    return ((void*(__cdecl*)(void*))0x4d60e0)(hierarchy);
}

// 0x000b1d20
void* GameRwGetClumpSkin(void* clump)
{
    void* atomic = GameRwGetFirstAtomic(clump);
    return GameRwGetGeometrySkin(*(void**)((BYTE*)atomic + 0x18));
}
