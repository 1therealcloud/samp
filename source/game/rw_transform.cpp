#include "game/rw_transform.h"
#include "game/input.h"
#include "game/native_scratch.h"
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
