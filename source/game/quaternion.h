#ifndef SAMP_GAME_QUATERNION_H
#define SAMP_GAME_QUATERNION_H
#include "game/entity.h"
struct GameQuaternion
{
    float w, x, y, z;
};
// 0x000b5210
void GameQuaternionFromMatrix(const CMatrix* matrix, GameQuaternion* out);
// 0x000b5500
void GameQuaternionNormalize(GameQuaternion* quaternion);
// Writes the nine basis floats only; position and padding remain untouched.
// 0x000b5300
void GameQuaternionToMatrixBody(const GameQuaternion* quaternion, CMatrix* matrix);
// Native forwarding entry.
// R5 address to verify
void GameQuaternionToMatrix(const GameQuaternion* quaternion, CMatrix* matrix);
// 0x000e6118
extern const float g_quaternionMatrixTwo;
// 0x000e5b2c
extern const float g_quaternionMatrixOne;
// 0x000b5480
void GameQuaternionSlerp(GameQuaternion* out, const GameQuaternion* from, const GameQuaternion* to, float amount);
// 0x000b5560
void GameQuaternionFromEuler(const CVector* angles, GameQuaternion* quaternion);
// 0x000b5620
void GameQuaternionToEuler(const GameQuaternion* quaternion, CVector* angles);
// Shared native constant, existing storage in game/textdraw.cpp.
// 0x000e5814
extern const float g_textDrawHalf;
// 0x000eb7a0
extern const float g_quaternionEulerNegativeTwo;
// Scalar-component conversion; no implicit normalization.
// 0x000b4f10
void GameQuaternionComponentsToMatrix(float w, float x, float y, float z, float* m00, float* m01, float* m02, float* m10, float* m11, float* m12, float* m20, float* m21,
                                      float* m22);
// Native adapter to the scalar-component builder; no implicit normalization.
// R5 address to verify
void GameQuaternionToMatrixComponents(const GameQuaternion* quaternion, CMatrix* matrix);
// 0x000b4d70
void GameMatrixComponentsToQuaternion(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float* w, float* x, float* y, float* z);
// R5 address to verify
void GameQuaternionFromMatrixComponents(const CMatrix* matrix, GameQuaternion* out);
// 0x000b5020
void GameQuaternionFromVectors(GameQuaternion* out, const CVector* from, const CVector* to);
// 0x000ed478
extern const float g_quaternionOppositeThreshold;
#endif

// Shared original double1 storage from ped code.
// 0x000e8898
extern const double g_pedParachuteOne;
// Native double scalar helper; saturation intentionally uses FSIN, not copysign.
// R5 address to verify
void GameQuaternionDoubleToEuler(double x, double y, double z, double w, double* roll, double* pitch, double* yaw);

// No near-opposite fallback; input snapshot precedes output and normalization.
// 0x000b69f0
void GameQuaternionFromVectorsHalf(GameQuaternion* out, const CVector* from, const CVector* to);

// x squared plus y squared plus z squared then w squared.
// R5 address to verify
float GameQuaternionLengthSquared(const GameQuaternion* quaternion);

// R5 address to verify
void GameQuaternionFromMatrixComponents4DA0(const CMatrix* matrix, GameQuaternion* out);
// R5 address to verify
void GameQuaternionToMatrixComponents4F60(const GameQuaternion* quaternion, CMatrix* matrix);
