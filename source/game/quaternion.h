#ifndef SAMP_GAME_QUATERNION_H
#define SAMP_GAME_QUATERNION_H
#include "game/entity.h"
struct GameQuaternion
{
    float w, x, y, z;
};

void GameQuaternionFromMatrix(const CMatrix* matrix, GameQuaternion* out);

void GameQuaternionNormalize(GameQuaternion* quaternion);
// Writes the nine basis floats only; position and padding remain untouched.

void GameQuaternionToMatrixBody(const GameQuaternion* quaternion, CMatrix* matrix);
// Native forwarding entry.

void GameQuaternionToMatrix(const GameQuaternion* quaternion, CMatrix* matrix);

extern const float g_quaternionMatrixTwo;

extern const float g_quaternionMatrixOne;

void GameQuaternionSlerp(GameQuaternion* out, const GameQuaternion* from, const GameQuaternion* to, float amount);

void GameQuaternionFromEuler(const CVector* angles, GameQuaternion* quaternion);

void GameQuaternionToEuler(const GameQuaternion* quaternion, CVector* angles);
// Shared native constant, existing storage in game/textdraw.cpp.

extern const float g_textDrawHalf;

extern const float g_quaternionEulerNegativeTwo;
// Scalar-component conversion; no implicit normalization.

void GameQuaternionComponentsToMatrix(float w, float x, float y, float z, float* m00, float* m01, float* m02, float* m10, float* m11, float* m12, float* m20, float* m21,
                                      float* m22);
// Native adapter to the scalar-component builder; no implicit normalization.

void GameQuaternionToMatrixComponents(const GameQuaternion* quaternion, CMatrix* matrix);

void GameMatrixComponentsToQuaternion(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float* w, float* x, float* y, float* z);

void GameQuaternionFromMatrixComponents(const CMatrix* matrix, GameQuaternion* out);

void GameQuaternionFromVectors(GameQuaternion* out, const CVector* from, const CVector* to);

extern const float g_quaternionOppositeThreshold;
#endif

// Shared original double1 storage from ped code.

extern const double g_pedParachuteOne;
// Native double scalar helper; saturation intentionally uses FSIN, not copysign.

void GameQuaternionDoubleToEuler(double x, double y, double z, double w, double* roll, double* pitch, double* yaw);

// No near-opposite fallback; input snapshot precedes output and normalization.

void GameQuaternionFromVectorsHalf(GameQuaternion* out, const CVector* from, const CVector* to);

// x squared plus y squared plus z squared then w squared.

float GameQuaternionLengthSquared(const GameQuaternion* quaternion);

void GameQuaternionFromMatrixComponents4DA0(const CMatrix* matrix, GameQuaternion* out);

void GameQuaternionToMatrixComponents4F60(const GameQuaternion* quaternion, CMatrix* matrix);
