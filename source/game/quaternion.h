#ifndef SAMP_GAME_QUATERNION_H
#define SAMP_GAME_QUATERNION_H
#include "game/entity.h"
struct GameQuaternion {float w,x,y,z;};
// samp.dll RVA: 0x000b4d50
void GameQuaternionFromMatrix(const CMatrix *matrix,GameQuaternion *out);
// samp.dll RVA: 0x000b5040
void GameQuaternionNormalize(GameQuaternion *quaternion);
// Writes the nine basis floats only; position and padding remain untouched.
// samp.dll RVA: 0x000b4e40
void GameQuaternionToMatrixBody(const GameQuaternion *quaternion,CMatrix *matrix);
// Native forwarding entry.
// samp.dll RVA: 0x000b4fb0
void GameQuaternionToMatrix(const GameQuaternion *quaternion,CMatrix *matrix);
// samp.dll RVA: 0x001181a8
extern const float g_quaternionMatrixTwo;
// samp.dll RVA: 0x00117b9c
extern const float g_quaternionMatrixOne;
// samp.dll RVA: 0x000b4fc0
void GameQuaternionSlerp(GameQuaternion *out,const GameQuaternion *from,const GameQuaternion *to,float amount);
// samp.dll RVA: 0x000b50a0
void GameQuaternionFromEuler(const CVector *angles,GameQuaternion *quaternion);
// samp.dll RVA: 0x000b5160
void GameQuaternionToEuler(const GameQuaternion *quaternion,CVector *angles);
// Shared native constant, existing storage in gui/textdraw_render.cpp.
// samp.dll RVA: 0x00117884
extern const float g_textDrawHalf;
// samp.dll RVA: 0x0011d7c8
extern const float g_quaternionEulerNegativeTwo;
#endif
