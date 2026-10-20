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
#endif
