#ifndef SAMP_RW_HELPERS_H
#define SAMP_RW_HELPERS_H
#include "game/entity.h"
typedef CVector *(__cdecl *GameRwPointTransformBackend)(CVector*,const CVector*,int,const CMatrix*);
typedef void (__cdecl *GameRwAxisRotateBackend)(CMatrix*,const CVector*,float,int);
// samp.dll RVA: 0x001553d4
extern GameRwPointTransformBackend g_rwPointTransformBackend;
// samp.dll RVA: 0x001553fc
extern GameRwAxisRotateBackend g_rwAxisRotateBackend;
// samp.dll RVA: 0x0011f58c
extern const float g_headingInverseHalfTurn;
// samp.dll RVA: 0x0011f590
extern const float g_headingPi;
// samp.dll RVA: 0x000b5210
CVector *GameTransformRwVector(CVector *out,const CVector *vector,const CMatrix *matrix);
// samp.dll RVA: 0x000b5230
CVector *GameTransformRwPoint(CVector *out,const CVector *point,const CMatrix *matrix);
// samp.dll RVA: 0x000b5260
void GameRepeatRwMatrixInvert();
// samp.dll RVA: 0x000b5280
void GameInvertRwMatrix(CMatrix *out,const CMatrix *matrix);
// samp.dll RVA: 0x000b52d0
void GameRotateRwMatrixOnAxis(CMatrix *matrix,int axis,float angle);
// samp.dll RVA: 0x000b54b0
float GameHeadingToRadians(float heading);
#endif
