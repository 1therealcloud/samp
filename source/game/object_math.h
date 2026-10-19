#ifndef SAMP_OBJECT_MATH_H
#define SAMP_OBJECT_MATH_H
#include "game/entity.h"
// samp.dll RVA: 0x0011ee7c
extern const float g_objectMotionRotationSentinel;
// samp.dll RVA: 0x001181ac
extern const float g_objectMutationStep;
// samp.dll RVA: 0x001181c4
extern const float g_objectRotationFactor;
// samp.dll RVA: 0x0011f578
extern const float g_angleUpperBound;
// samp.dll RVA: 0x0011f574
extern const float g_angleLowerBound;
// samp.dll RVA: 0x001181b4
extern const float g_anglePeriod;
// samp.dll RVA: 0x001179b0
extern const float g_objectScaleLowerBound;
// Native ST0 retains extended precision until the caller chooses its store.
// samp.dll RVA: 0x000b44e0
float __cdecl GameNormalizeAngle(float angle);
// Output may alias the matrix or input; native stores each component before reading the next.
// samp.dll RVA: 0x000b4850
void GameTransformPoint(CVector *out,const CMatrix *matrix,const CVector *delta);
#endif
