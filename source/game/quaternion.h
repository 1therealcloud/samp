#ifndef SAMP_GAME_QUATERNION_H
#define SAMP_GAME_QUATERNION_H
#include "game/entity.h"
struct GameQuaternion
{
    float w, x, y, z;
};
// 0x000b4d50
void GameQuaternionFromMatrix(const CMatrix* matrix, GameQuaternion* out);
// 0x000b5040
void GameQuaternionNormalize(GameQuaternion* quaternion);
// Writes the nine basis floats only; position and padding remain untouched.
// 0x000b4e40
void GameQuaternionToMatrixBody(const GameQuaternion* quaternion, CMatrix* matrix);
// Native forwarding entry.
// 0x000b4fb0
void GameQuaternionToMatrix(const GameQuaternion* quaternion, CMatrix* matrix);
// 0x001181a8
extern const float g_quaternionMatrixTwo;
// 0x00117b9c
extern const float g_quaternionMatrixOne;
// 0x000b4fc0
void GameQuaternionSlerp(GameQuaternion* out, const GameQuaternion* from, const GameQuaternion* to, float amount);
// 0x000b50a0
void GameQuaternionFromEuler(const CVector* angles, GameQuaternion* quaternion);
// 0x000b5160
void GameQuaternionToEuler(const GameQuaternion* quaternion, CVector* angles);
// Shared native constant, existing storage in gui/textdraw_render.cpp.
// 0x00117884
extern const float g_textDrawHalf;
// 0x0011d7c8
extern const float g_quaternionEulerNegativeTwo;
// Scalar-component conversion; no implicit normalization.
// 0x000b4a50
void GameQuaternionComponentsToMatrix(float w, float x, float y, float z, float* m00, float* m01, float* m02, float* m10, float* m11, float* m12, float* m20, float* m21,
                                      float* m22);
// Native adapter to the scalar-component builder; no implicit normalization.
// 0x000b65c0
void GameQuaternionToMatrixComponents(const GameQuaternion* quaternion, CMatrix* matrix);
// 0x000b48b0
void GameMatrixComponentsToQuaternion(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float* w, float* x, float* y, float* z);
// 0x000b4df0
void GameQuaternionFromMatrixComponents(const CMatrix* matrix, GameQuaternion* out);
// 0x000b4b60
void GameQuaternionFromVectors(GameQuaternion* out, const CVector* from, const CVector* to);
// 0x0011f588
extern const float g_quaternionOppositeThreshold;
#endif

// Shared original double1 storage from ped code.
// 0x0011a928
extern const double g_pedParachuteOne;
// Native double scalar helper; saturation intentionally uses FSIN, not copysign.
// 0x0009d040
void GameQuaternionDoubleToEuler(double x, double y, double z, double w, double* roll, double* pitch, double* yaw);

// No near-opposite fallback; input snapshot precedes output and normalization.
// 0x000b6530
void GameQuaternionFromVectorsHalf(GameQuaternion* out, const CVector* from, const CVector* to);

// x squared plus y squared plus z squared then w squared.
// 0x000b4cc0
float GameQuaternionLengthSquared(const GameQuaternion* quaternion);

// Separate original adapters; distinct original names are unresolved.
// 0x000b4da0
void GameQuaternionFromMatrixComponents4DA0(const CMatrix* matrix, GameQuaternion* out);
// 0x000b4f60
void GameQuaternionToMatrixComponents4F60(const GameQuaternion* quaternion, CMatrix* matrix);
