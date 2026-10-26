#include "game/util.h"
#include "main.h"
#include "chatwindow.h"
// samp.dll RVA: 0x0011e3a0
const double g_aimVectorHalfPi=1.5707963705062866;
// samp.dll RVA: 0x000b6440
__declspec(naked) void __stdcall GameBuildAimUpVector(const CVector *direction,CVector *up) {
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
}
// samp.dll RVA: 0x001181ac
extern const float g_objectMutationStep=0.1f;
// samp.dll RVA: 0x001181c4
extern const float g_objectRotationFactor=10.0f;
// samp.dll RVA: 0x0011f578
extern const float g_angleUpperBound=180.0f;
// samp.dll RVA: 0x0011f574
extern const float g_angleLowerBound=-180.0f;
// samp.dll RVA: 0x001181b4
extern const float g_anglePeriod=360.0f;
// samp.dll RVA: 0x001179b0
extern const float g_objectScaleLowerBound=0.0f;
// samp.dll RVA: 0x000b44e0
__declspec(naked) float __cdecl GameNormalizeAngle(float angle) {
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
}
// samp.dll RVA: 0x000b4850
void GameTransformPoint(CVector *out,const CMatrix *matrix,const CVector *delta) {
    out->x=((matrix->at.x*delta->z+matrix->up.x*delta->y)+matrix->right.x*delta->x)+matrix->position.x;
    out->y=((matrix->at.y*delta->z+matrix->up.y*delta->y)+matrix->right.y*delta->x)+matrix->position.y;
    out->z=((matrix->at.z*delta->z+matrix->up.z*delta->y)+matrix->right.z*delta->x)+matrix->position.z;
}

// Native 0.2x name retained; result stays extended in x87 ST0.
// samp.dll RVA: 0x000b44b0
float FloatOffset(float first,float second) {
    if(first>=second)return first-second;
    return second-first;
}

#include "game/game.h"
// samp.dll RVA: 0x001553d8
const CVector g_rwRotationAxes[3]={{1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f},{0.0f,0.0f,1.0f}};
// samp.dll RVA: 0x002aae14
float g_rwQueryAngle;

// GTA SA virtual addresses: USA 7EB7C0; all other version values use 7EB800.
// samp.dll RVA: 0x000b5320
void __cdecl GameRotateRwQuaternion(void *quaternion,int axis,float angle,int combine) {
    const CVector *direction=&g_rwRotationAxes[axis];
    DWORD target=g_gameVersion==1?0x7eb7c0:0x7eb800;
    ((void(__cdecl*)(void*,const CVector*,float,int))target)(quaternion,direction,angle,combine);
}
// Shared fields are views into the existing native backing span, not new globals.
// GTA SA virtual addresses: USA 7F2720; all other version values use 7F2760.
// samp.dll RVA: 0x000b5370
void __cdecl GameQueryRwMatrixAngle(const CMatrix *matrix,int axis,float *angle) {
    const DWORD *direction=(const DWORD*)&g_rwRotationAxes[axis];
    DWORD x=direction[0],y=direction[1],z=direction[2];
    // Shared native fields: samp.dll RVAs 002ABE34 and 002ABE28.
    CVector *queryAxis=(CVector*)(g_entityCollisionScratch+0x101c);
    CVector *centre=(CVector*)(g_entityCollisionScratch+0x1010);
    *(DWORD*)&queryAxis->x=x;
    *(DWORD*)&queryAxis->y=y;
    *(DWORD*)&queryAxis->z=z;
    DWORD target=g_gameVersion==1?0x7f2720:0x7f2760;
    ((void(__cdecl*)(const CMatrix*,CVector*,float*,CVector*))target)(matrix,queryAxis,&g_rwQueryAngle,centre);
    // Native publishes through x87, including its float-load/store NaN behavior.
    __asm {
        mov eax,angle
        fld g_rwQueryAngle
        fstp dword ptr [eax]
    }
}
// GTA SA virtual addresses: USA 7EBA80; all other version values use 7EBAC0.
// samp.dll RVA: 0x000b53e0
void __cdecl GameQueryRwQuaternionAngle(const void *quaternion,CVector *axis,float *angle) {
    DWORD target=g_gameVersion==1?0x7eba80:0x7ebac0;
    ((void(__cdecl*)(const void*,CVector*,float*))target)(quaternion,axis,angle);
}
// GTA SA virtual addresses: USA 7F1FD0; all other version values use 7F2010.
// samp.dll RVA: 0x000b5410
void __cdecl GameRotateRwMatrix(CMatrix *matrix,const CVector *axis,float angle) {
    DWORD target=g_gameVersion==1?0x7f1fd0:0x7f2010;
    ((void(__cdecl*)(CMatrix*,const CVector*,float,int))target)(matrix,axis,angle,1);
}
// GTA SA virtual addresses: USA 7F1920; all other version values use 7F1960.
// samp.dll RVA: 0x000b5450
void __cdecl GameOrthoNormalizeRwMatrix(CMatrix *out,const CMatrix *matrix) {
    DWORD target=g_gameVersion==1?0x7f1920:0x7f1960;
    ((void(__cdecl*)(CMatrix*,const CMatrix*))target)(out,matrix);
}
// GTA SA virtual addresses: USA 7F22C0; all other version values use 7F2300.
// samp.dll RVA: 0x000b5480
void __cdecl GameScaleRwMatrix(CMatrix *matrix,const CVector *scale) {
    DWORD target=g_gameVersion==1?0x7f22c0:0x7f2300;
    ((void(__cdecl*)(CMatrix*,const CVector*,int))target)(matrix,scale,1);
}


// GTA SA virtual address: 0x0059C910, ECX receives the vector.
// samp.dll RVA: 0x000b4630
void GameNormalizeVector(CVector *vector) {
    __asm {
        mov ecx,vector
        mov edx,0x59c910
        call edx
    }
}
// GTA SA virtual address: 0x0059C730, cdecl out/left/right, returns out in EAX.
// samp.dll RVA: 0x000b4750
CVector *GameVectorCrossProduct(CVector *out,const CVector *left,const CVector *right) {
    return ((CVector *(__cdecl*)(CVector*,const CVector*,const CVector*))0x59c730)(out,left,right);
}
// GTA SA virtual address: 0x0059C810, cdecl out/vector/matrix.
// samp.dll RVA: 0x000b4790
CVector *GameTransformVectorByTranspose(CVector *out,const CVector *vector,const CMatrix *matrix) {
    return ((CVector *(__cdecl*)(CVector*,const CVector*,const CMatrix*))0x59c810)(out,vector,matrix);
}
// Backend quaternion layout is deliberately opaque, as in the other RW adapters.
// GTA SA virtual addresses: USA 0x007EB5C0; all other values use 0x007EB600.
// samp.dll RVA: 0x000b4d20
void GameRwQuaternionFromMatrix(void *quaternion,const CMatrix *matrix) {
    DWORD target=g_gameVersion==1?0x7eb5c0:0x7eb600;
    ((void(__cdecl*)(void*,const CMatrix*))target)(quaternion,matrix);
}


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


// GTA SA virtual address: 0x007498E0 (fixed).
// samp.dll RVA: 0x000b1010
int GameRwCountClumpAtomics(void *clump) {
    return ((int(__cdecl*)(void*))0x7498e0)(clump);
}

// GTA SA virtual address: 0x00749B70 (fixed).
// samp.dll RVA: 0x000b1090
void *GameRwForAllAtomics(void *clump,GameRwObjectCallback callback,void *data) {
    return ((void*(__cdecl*)(void*,GameRwObjectCallback,void*))0x749b70)(clump,callback,data);
}

// GTA SA virtual address: 0x007F1200; every non-USA version uses +0x40.
// samp.dll RVA: 0x000b10c0
void *GameRwForAllFrameObjects(void *frame,GameRwObjectCallback callback,void *data) {
    DWORD target=g_gameVersion==1?0x7f1200:0x7f1240;
    return ((void*(__cdecl*)(void*,GameRwObjectCallback,void*))target)(frame,callback,data);
}

// GTA SA virtual address: 0x007F3820; every non-USA version uses +0x40.
// samp.dll RVA: 0x000b1360
void GameRwDestroyTexture(void *texture) {
    DWORD target=g_gameVersion==1?0x7f3820:0x7f3860;
    ((BOOL(__cdecl*)(void*))target)(texture);
}

// GTA SA virtual address: 0x007F05A0; every non-USA version uses +0x40.
// samp.dll RVA: 0x000b13d0
void GameRwDestroyFrame(void *frame) {
    DWORD target=g_gameVersion==1?0x7f05a0:0x7f05e0;
    ((BOOL(__cdecl*)(void*))target)(frame);
}

// GTA SA virtual address: 0x00749DC0; every non-USA version uses +0x50.
// samp.dll RVA: 0x000b1400
void GameRwDestroyAtomic(void *atomic) {
    DWORD target=g_gameVersion==1?0x749dc0:0x749e10;
    ((BOOL(__cdecl*)(void*))target)(atomic);
}

// GTA SA virtual address: 0x007FB020; every non-USA version uses +0x40.
// samp.dll RVA: 0x000b1470
void GameRwDestroyRaster(void *raster) {
    DWORD target=g_gameVersion==1?0x7fb020:0x7fb060;
    ((BOOL(__cdecl*)(void*))target)(raster);
}

// GTA SA virtual address: 0x007EE340; every non-USA version uses +0x40.
// samp.dll RVA: 0x000b14f0
void GameRwClearCamera(void *camera,const void *color,DWORD flags) {
    DWORD target=g_gameVersion==1?0x7ee340:0x7ee380;
    ((void*(__cdecl*)(void*,const void*,DWORD))target)(camera,color,flags);
}

// GTA SA virtual address: 0x007EE190; every non-USA version uses +0x40.
// samp.dll RVA: 0x000b1520
void GameRwBeginCameraUpdate(void *camera) {
    DWORD target=g_gameVersion==1?0x7ee190:0x7ee1d0;
    ((void*(__cdecl*)(void*))target)(camera);
}

// GTA SA virtual address: 0x007EE180; every non-USA version uses +0x40.
// samp.dll RVA: 0x000b1550
void GameRwEndCameraUpdate(void *camera) {
    DWORD target=g_gameVersion==1?0x7ee180:0x7ee1c0;
    ((void*(__cdecl*)(void*))target)(camera);
}

// GTA SA virtual address: 0x00749B20; every non-USA version uses +0x50.
// samp.dll RVA: 0x000b15d0
void GameRwRenderClump(void *clump) {
    DWORD target=g_gameVersion==1?0x749b20:0x749b70;
    ((void*(__cdecl*)(void*))target)(clump);
}

// GTA SA virtual address: 0x0074A4F0; every non-USA version uses +0x50.
// samp.dll RVA: 0x000b1740
void GameRwAddClumpLight(void *clump,void *light) {
    DWORD target=g_gameVersion==1?0x74a4f0:0x74a540;
    ((void*(__cdecl*)(void*,void*))target)(clump,light);
}

// GTA SA virtual address: 0x00751A70 (fixed).
// samp.dll RVA: 0x000b17a0
void GameRwSetLightRadius(void *light,float radius) {
    ((void*(__cdecl*)(void*,float))0x751a70)(light,radius);
}

// GTA SA virtual address: 0x0074A310; every non-USA version uses +0x50.
// samp.dll RVA: 0x000b1880
BOOL GameRwDestroyClump(void *clump) {
    DWORD target=g_gameVersion==1?0x74a310:0x74a360;
    return ((BOOL(__cdecl*)(void*))target)(clump);
}

// Native object tag must be1 before the raw +48 field is read.
// B5E30 establishes this field as the atomic render callback; keep raw view.
// samp.dll RVA: 0x000b1030
void *GameRwQueryAtomic48(const void *object) {
    const BYTE *bytes=(const BYTE*)object;
    return bytes && *bytes==1?*(void* const*)(bytes+0x48):0;
}

// Nullable raw +34 read; no full backend object layout is inferred.
// samp.dll RVA: 0x000b1130
void *__stdcall GameRwReadField34(const void *object) {
    return object?*(void* const*)((const BYTE*)object+0x34):0;
}

// Each entry snapshots GTA world slot C17038 once, and tests only that world.
// GTA SA virtual backend: 0x00750F20; non-USA fallback +0x50.
// samp.dll RVA: 0x000b14b0
void GameRwWorldAddCamera(void *camera) {
    void *world=*(void**)0xc17038;
    if(world) {
        DWORD target=g_gameVersion==1?0x750f20:0x750f70;
        ((void*(__cdecl*)(void*,void*))target)(world,camera);
    }
}
// GTA SA virtual backend: 0x00751300 (fixed for every version).
// samp.dll RVA: 0x000b1600
void GameRwWorldAddClump(void *clump) {
    void *world=*(void**)0xc17038;
    if(world) {
        ((void*(__cdecl*)(void*,void*))0x751300)(world,clump);
    }
}
// GTA SA virtual backend: 0x00751460 (fixed for every version).
// samp.dll RVA: 0x000b1630
void GameRwWorldRemoveClump(void *clump) {
    void *world=*(void**)0xc17038;
    if(world) {
        ((void*(__cdecl*)(void*,void*))0x751460)(world,clump);
    }
}
// GTA SA virtual backend: 0x0074F570 (fixed for every version).
// samp.dll RVA: 0x000b1660
void GameRwWorldRender() {
    void *world=*(void**)0xc17038;
    if(world) {
        ((void*(__cdecl*)(void*))0x74f570)(world);
    }
}
// GTA SA virtual backend: 0x00751910; non-USA fallback +0x50.
// samp.dll RVA: 0x000b17c0
void GameRwWorldAddLight(void *light) {
    void *world=*(void**)0xc17038;
    if(world) {
        DWORD target=g_gameVersion==1?0x751910:0x751960;
        ((void*(__cdecl*)(void*,void*))target)(world,light);
    }
}
// GTA SA virtual backend: 0x00751960; non-USA fallback +0x50.
// samp.dll RVA: 0x000b1800
void GameRwWorldRemoveLight(void *light) {
    void *world=*(void**)0xc17038;
    if(world) {
        DWORD target=g_gameVersion==1?0x751960:0x7519b0;
        ((void*(__cdecl*)(void*,void*))target)(world,light);
    }
}

// GTA SA virtual address: 0x00749F70, fixed even for non-USA version values.
// samp.dll RVA: 0x000b1850
void *GameRwCloneClump(void *clump) {
    if(!clump)return 0;
    return ((void*(__cdecl*)(void*))0x749f70)(clump);
}

// Raw return ABI: stack cleanup POPs intentionally overwrite backend EAX.
// GTA SA engine slot C97B24, entry+20; slot purpose remains unresolved.
// Two raw words are passed, and the final popped second word is returned.
// samp.dll RVA: 0x000b0ff0
__declspec(naked) DWORD GameRwInvokeEngineSlot20(DWORD first,DWORD second) {
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
}
// GTA SA virtual address74C800. Return input word, not backend result.
// samp.dll RVA: 0x000b1100
__declspec(naked) void *GameRwUnlockGeometry(void *geometry) {
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
}
// GTA SA virtual address74C7D0. Pass lock maskFFF, then return that word.
// samp.dll RVA: 0x000b1110
__declspec(naked) DWORD GameRwLockGeometry(void *geometry) {
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
}

// GTA SA virtual address: 0x007328C0, static visibility camera setter.
// samp.dll RVA: 0x000b1840
void GameRwSetVisibilityCamera(void *camera) {
    ((void(__cdecl*)(void*))0x7328c0)(camera);
}
// GTA SA virtual addresses: hierarchy lookup734A40, matrix update7C51D0.
// No additional null hierarchy guard is native.
// samp.dll RVA: 0x000b1980
void GameRwUpdateClumpHierarchy(void *clump) {
    void *hierarchy=((void*(__cdecl*)(void*))0x734a40)(clump);
    ((BOOL(__cdecl*)(void*))0x7c51d0)(hierarchy);
}
// GTA SA virtual address: 0x004D34F0; full native on-screen word is forwarded.
// samp.dll RVA: 0x000b19a0
void GameRwUpdateClumpAnimations(void *clump,float timeStep,DWORD onScreen) {
    ((void(__cdecl*)(void*,float,DWORD))0x4d34f0)(clump,timeStep,onScreen);
}

// Return the address at +7C, not the pointer stored there; tag1 only.
// samp.dll RVA: 0x000b1050
void *GameRwQueryAtomic7c(void *object) {
    BYTE *bytes=(BYTE*)object;
    return bytes && *bytes==1?bytes+0x7c:0;
}

// Snapshot frame before atomic destruction; clump path has no frame teardown.
// samp.dll RVA: 0x000b5df0
void GameRwDestroyObject(void *object) {
    if(!object)return;
    BYTE type=*(BYTE*)object;
    if(type==1) {
        void *frame=*(void**)((BYTE*)object+4);
        GameRwDestroyAtomic(object);
        if(frame)GameRwDestroyFrame(frame);
    } else if(type==2)GameRwDestroyClump(object);
}

// Native callback is cdecl, unchecked, and its return value is ignored.
// samp.dll RVA: 0x000b5e30
BOOL __stdcall GameRwRenderAtomic(void *object) {
    if(!object || *(BYTE*)object!=1)return FALSE;
    void *callback=GameRwQueryAtomic48(object);
    ((void*(__cdecl*)(void*))callback)(object);
    return TRUE;
}

// Dispatch opaque backend tags1/2 only; native entry pops its one argument.
// samp.dll RVA: 0x000b5e70
void __stdcall GameRwRenderObject(void *object) {
    if(!object)return;
    BYTE type=*(BYTE*)object;
    if(type==1)GameRwRenderAtomic(object);
    else if(type==2)GameRwRenderClump(object);
}


// GTA SA virtual addresses: pool slot B74490, thiscall backend 4442D0.
// samp.dll RVA: 0x000b37a0
int __stdcall GamePoolPedGetIndex(void *ped) {
    int result;
    __asm {
        mov ebx,0xb74490
        mov ecx,dword ptr [ebx]
        push ped
        mov ebx,0x4442d0
        call ebx
        mov result,eax
    }
    return result;
}

// GTA SA virtual addresses: pool slot B74494, thiscall backend 424160.
// samp.dll RVA: 0x000b3800
DWORD __stdcall GamePoolVehicleGetIndex(void *vehicle) {
    DWORD result=0;
    __asm {
        mov eax,0xb74494
        mov ecx,dword ptr [eax]
        push vehicle
        mov edx,0x424160
        call edx
        mov result,eax
    }
    return result;
}

// GTA SA virtual pool-pointer address: 0x00B74498.
// samp.dll RVA: 0x000b3870
void *GamePoolBuildingGetArray() { return **(void***)0xb74498; }

// GTA SA virtual pool-pointer address: 0x00B744A0.
// samp.dll RVA: 0x000b3890
void *GamePoolDummyGetArray() { return **(void***)0xb744a0; }

// GTA SA virtual pool-pointer address: 0x00B7449C.
// samp.dll RVA: 0x000b38b0
void *GamePoolObjectGetArray() { return **(void***)0xb7449c; }

// Native scans exactly 3100 records of 412 bytes, independent of pool metadata.
// Preserve the signed model-word comparison and the 863C40 vtable exclusion.
// GTA SA virtual address: object-pool pointer B7449C.
// samp.dll RVA: 0x000b3930
bool GamePoolHasObjectModel(int model) {
    DWORD address=(DWORD)**(void***)0xb7449c;
    for(DWORD index=0;index<3100;++index,address+=0x19c) {
        if(address && *(DWORD*)address && *(DWORD*)address!=0x863c40 &&
            *(short*)(address+0x22)==model)return true;
    }
    return false;
}

// GTA SA virtual addresses: scan A9C068 inclusive through AAE948 exclusive.
// Exactly 19000 DWORD slots; do not use the relocated/custom SA-MP bank here.
// samp.dll RVA: 0x000b3a00
int GameCountEmptyBackendModelSlots() {
    int count=0;
    for(DWORD *slot=(DWORD*)0xa9c068;slot!=(DWORD*)0xaae948;++slot) {
        if(*slot==0)++count;
    }
    return count;
}


// samp.dll RVA: 0x002a9d98
int g_rwImageDepth;
// samp.dll RVA: 0x002a9d9c
int g_rwImageWidth;
// samp.dll RVA: 0x002a9a40
int g_rwImageHeight;
// samp.dll RVA: 0x002aae10
void * g_rwImageScratch;
// samp.dll RVA: 0x00155400
void ** g_rwSceneCameraSlot=(void**)0xc1703c;

// Shared scratch views only; no independent global storage is introduced.
// RVAs 2ABE40, 2ABE48 and 2ABE4C.
#define SCENE_SOURCE_RASTER (*(void**)(g_entityCollisionScratch+0x1028))
#define SCENE_COLOR_RASTER (*(void**)(g_entityCollisionScratch+0x1030))
#define SCENE_DEPTH_RASTER (*(void**)(g_entityCollisionScratch+0x1034))

// Native ignores all failures and reloads the published image between calls.
// GTA SA virtual addresses: image create/allocate/from-raster/BMP write/destroy.
// samp.dll RVA: 0x000b5690
void GameWriteRasterBmp(void *raster,const char *filename) {
    g_rwImageScratch=((void*(__cdecl*)(int,int,int))0x8026e0)(g_rwImageWidth,g_rwImageHeight,g_rwImageDepth);
    ((void*(__cdecl*)(void*))0x8027a0)(g_rwImageScratch);
    ((void*(__cdecl*)(void*,void*))0x804250)(g_rwImageScratch,raster);
    ((void*(__cdecl*)(void*,const char*))0x7ce990)(g_rwImageScratch,filename);
    ((BOOL(__cdecl*)(void*))0x802740)(g_rwImageScratch);
}

// Allocate only when both raster slots are null. A partial success is retained.
// GTA SA virtual addresses: source raster slot C402D8; RwRasterCreate 7FB230.
// samp.dll RVA: 0x000b57d0
void GameCreateSceneRasters() {
    if(SCENE_COLOR_RASTER || SCENE_DEPTH_RASTER)return;
    SCENE_SOURCE_RASTER=*(void**)0xc402d8;
    BYTE *source=(BYTE*)SCENE_SOURCE_RASTER;
    g_rwImageDepth=*(int*)(source+0x14);
    g_rwImageHeight=*(int*)(source+0x10);
    g_rwImageWidth=*(int*)(source+0xc);
    SCENE_COLOR_RASTER=((void*(__cdecl*)(int,int,int,int))0x7fb230)(2048,1024,g_rwImageDepth,5);
    SCENE_DEPTH_RASTER=((void*(__cdecl*)(int,int,int,int))0x7fb230)(2048,1024,g_rwImageDepth,1);
}

// Preserve all PUSHAD registers, camera publication, call order and saved-slot
// reloads. Shared saved slots at RVAs 2ABE50/54 make this path non-reentrant.
// GTA SA virtual address C9BCC0 is written but not restored by the native body.
// samp.dll RVA: 0x000b5860
__declspec(naked) void GameRenderWithSceneRasters() {
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
}

// samp.dll RVA: 0x002aae18
BYTE g_entityCollisionScratch[0x1044];

// samp.dll RVA: 0x000b3e40
DWORD __stdcall GameTestCameraSphere(const CVector *centre,float radius) {
    DWORD sphere[4];memcpy(sphere,centre,12);memcpy(sphere+3,&radius,4);
    // GTA SA virtual address: current RenderWare camera pointer.
    void *camera=*(void**)0xc1703c;if(!camera)return 0;
    DWORD target=g_gameVersion==1?0x7ee2d0:0x7ee310;
    return ((DWORD(__cdecl*)(void*,const void*))target)(camera,sphere);
}

// samp.dll RVA: 0x000b1170
void __stdcall GameRwReplaceField34(void *object,void *replacement) {
    if(!object)return;
    void *previous=*(void**)((BYTE*)object+0x34);
    if(!previous)return;
    DWORD target=*(DWORD*)(*(BYTE**)previous+8);
    ((DWORD(__stdcall*)(void*))target)(previous);
    *(void**)((BYTE*)object+0x34)=replacement;
}
// samp.dll RVA: 0x000b11c0
void * GameRwDumpAtomicMaterials(void *object,void *data) {
    BYTE *atomic=(BYTE*)object;
    if(atomic[0]!=1||!*(void**)(atomic+0x18))return object;
    int count=*(int*)(*(BYTE**)(atomic+0x18)+0x24);
    CChat *chat=g_pChat;
    if(chat){chat->AddDebugMessage(g_rwMaterialCountText,count);chat=g_pChat;}
    for(int i=0;i<count;++i) {
        BYTE *geometry=*(BYTE**)(atomic+0x18);
        BYTE *material=(*(BYTE***)(geometry+0x20))[i];
        if(!material)continue;
        BYTE *texture=*(BYTE**)material;
        if(chat) {
            if(texture)chat->AddDebugMessage(g_rwMaterialNameText,i,(char*)texture+0x10);
            else chat->AddDebugMessage(g_rwMaterialNullText,i);
            chat=g_pChat;
        }
    }
    return object;
}
// samp.dll RVA: 0x000b1260
void * __stdcall GameRwDumpObjectFrameMaterials(void *object) {
    void *frame=*(void**)((BYTE*)object+4);
    return frame?GameRwForAllFrameObjects(frame,GameRwDumpAtomicMaterials,0):0;
}
// samp.dll RVA: 0x000b1280
void * GameRwKeepType1Object(void *object,void *data) {
    return *(BYTE*)object==1?object:0;
}
// samp.dll RVA: 0x000b12a0
void * __stdcall GameRwEnumerateObjectFrameType1(void *object) {
    void *frame=*(void**)((BYTE*)object+4);
    return frame?GameRwForAllFrameObjects(frame,GameRwKeepType1Object,0):0;
}
// samp.dll RVA: 0x000b1970
void * GameRwGetObjectFrame(const void *object) {
    return object?*(void**)((const BYTE*)object+4):0;
}
// samp.dll RVA: 0x000b1e90
void * GameRwGetClumpWord8Minus64(void *clump,int index) {
    int count=GameRwCountClumpAtomics(clump);
    if(!count||index>count)return 0;
    return (void*)(*(DWORD*)((BYTE*)clump+8)-0x40);
}

// samp.dll RVA: 0x0011f414
const char g_rwMaterialCountText[]="NumMaterials = %d";
// samp.dll RVA: 0x0011f3f0
const char g_rwMaterialNameText[]="Material Index: %d TextureName: %s";
// samp.dll RVA: 0x0011f3cc
const char g_rwMaterialNullText[]="Material Index: %d Texture: NULL";
// Native x87 intermediate precision, spill locations and input bit pattern matter.
// samp.dll RVA: 0x000b55a0
__declspec(naked) float GameApproximateSqrt(float value) {
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
}

// GTA SA virtual address: 0x00859524; raw float semantics unresolved.
// samp.dll RVA: 0x000b26d0
float GameReadFloat859524() {
    return *(const float*)0x859524;
}
// GTA SA virtual address: 0x00859520; raw float semantics unresolved.
// samp.dll RVA: 0x000b26e0
float GameReadFloat859520() {
    return *(const float*)0x859520;
}
