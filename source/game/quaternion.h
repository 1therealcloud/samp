#ifndef SAMP_GAME_QUATERNION_H
#define SAMP_GAME_QUATERNION_H
#include "game/entity.h"
struct GameQuaternion {float w,x,y,z;};
// samp.dll RVA: 0x000b4d50
void GameQuaternionFromMatrix(const CMatrix *matrix,GameQuaternion *out);
// samp.dll RVA: 0x000b5040
void GameQuaternionNormalize(GameQuaternion *quaternion);
// Native forwarding entry; full conversion body at RVA B4E40 remains open.
// samp.dll RVA: 0x000b4fb0
void GameQuaternionToMatrix(const GameQuaternion *quaternion,CMatrix *matrix);
#endif
