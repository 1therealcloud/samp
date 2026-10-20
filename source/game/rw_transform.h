#ifndef SAMP_RW_TRANSFORM_H
#define SAMP_RW_TRANSFORM_H
#include "game/entity.h"
// Native axis table is indexed without a bounds check.
// samp.dll RVA: 0x001553d8
extern const CVector g_rwRotationAxes[3];
// samp.dll RVA: 0x002aae14
extern float g_rwQueryAngle;
// Quaternion layout here belongs to the GTA/RenderWare backend; it is opaque.
// Do not substitute the maintained w,x,y,z GameQuaternion layout implicitly.
// samp.dll RVA: 0x000b5320
void __cdecl GameRotateRwQuaternion(void *quaternion,int axis,float angle,int combine);
// samp.dll RVA: 0x000b5370
void __cdecl GameQueryRwMatrixAngle(const CMatrix *matrix,int axis,float *angle);
// samp.dll RVA: 0x000b53e0
void __cdecl GameQueryRwQuaternionAngle(const void *quaternion,CVector *axis,float *angle);
// samp.dll RVA: 0x000b5410
void __cdecl GameRotateRwMatrix(CMatrix *matrix,const CVector *axis,float angle);
// samp.dll RVA: 0x000b5450
void __cdecl GameOrthoNormalizeRwMatrix(CMatrix *out,const CMatrix *matrix);
// samp.dll RVA: 0x000b5480
void __cdecl GameScaleRwMatrix(CMatrix *matrix,const CVector *scale);
// Raw backend layout; unlike GameQuaternionFromMatrix, no XYZW/WXYZ reorder.
// samp.dll RVA: 0x000b4d20
void GameRwQuaternionFromMatrix(void *quaternion,const CMatrix *matrix);
#endif
