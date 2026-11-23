#include "game/quaternion.h"

#include "game/game.h"
#include "game/util.h"

#include <d3dx9.h>

#include <math.h>
#include <string.h>
// 0x001181a8
const float g_quaternionMatrixTwo = 2.0f;
// 0x00117b9c
const float g_quaternionMatrixOne = 1.0f;
// 0x0011f588
const float g_quaternionOppositeThreshold = 0.0001f;

// 0x0011d7c8
const float g_quaternionEulerNegativeTwo = -2.0f;

// 0x000b65c0
void GameQuaternionToMatrixComponents(const GameQuaternion* quaternion, CMatrix* matrix)
{
    GameQuaternionComponentsToMatrix(quaternion->w, quaternion->x, quaternion->y, quaternion->z, &matrix->right.x, &matrix->right.y, &matrix->right.z, &matrix->up.x, &matrix->up.y,
                                     &matrix->up.z, &matrix->at.x, &matrix->at.y, &matrix->at.z);
}

// 0x000b4d50
void GameQuaternionFromMatrix(const CMatrix* matrix, GameQuaternion* out)
{
    D3DXQUATERNION quaternion;
    // GTA SA virtual addresses: USA 7EB5C0; all other version values use EU 7EB600.
    DWORD target = g_gameVersion == 1 ? 0x7eb5c0 : 0x7eb600;
    ((void(__cdecl*)(D3DXQUATERNION*, const CMatrix*))target)(&quaternion, matrix);
    out->w = quaternion.w;
    memcpy(&out->x, &quaternion.x, 12);
}

// 0x000b5040
void GameQuaternionNormalize(GameQuaternion* quaternion)
{
    D3DXQUATERNION input, output;
    memcpy(&input.x, &quaternion->x, 12);
    memcpy(&input.w, &quaternion->w, 4);
    D3DXQuaternionNormalize(&output, &input);
    memcpy(&quaternion->w, &output.w, 4);
    memcpy(&quaternion->x, &output.x, 12);
}

// Round the scale and selected products before assembling the basis.
// Flags, padding and translation are untouched; zero input is not special-cased.
// 0x000b4e40
void GameQuaternionToMatrixBody(const GameQuaternion* quaternion, CMatrix* matrix)
{
    const double w = quaternion->w, x = quaternion->x, y = quaternion->y, z = quaternion->z;
    const volatile float scale = (float)(2.0 / (x * x + y * y + z * z + w * w));
    const double sx = scale * x;
    const volatile float sy = (float)(scale * y);
    const double sz = scale * z;
    const volatile float wx = (float)(w * sx), wy = (float)(w * sy), wz = (float)(w * sz);
    const volatile float xx = (float)(x * sx), yy = (float)(y * sy), zz = (float)(z * sz);
    const volatile float yz = (float)(y * sz);
    const double xy = x * sy, xz = x * sz;
    matrix->right.x = (float)(1.0 - ((double)zz + yy));
    matrix->right.y = (float)(wz + xy);
    matrix->right.z = (float)(xz - wy);
    matrix->up.x = (float)(xy - wz);
    matrix->up.y = (float)(1.0 - ((double)zz + xx));
    matrix->up.z = (float)((double)wx + yz);
    matrix->at.x = (float)(wy + xz);
    matrix->at.y = (float)((double)yz - wx);
    matrix->at.z = (float)(1.0 - ((double)yy + xx));
}

// Native forwarding entry shares the cdecl argument frame with the body.
// 0x000b4fb0
__declspec(naked) void GameQuaternionToMatrix(const GameQuaternion* quaternion, CMatrix* matrix)
{
    // clang-format off
    __asm { jmp GameQuaternionToMatrixBody }
    // clang-format on
}

// Snapshot both inputs in D3DX x,y,z,w order before publishing the result.
// 0x000b4fc0
void GameQuaternionSlerp(GameQuaternion* out, const GameQuaternion* from, const GameQuaternion* to, float amount)
{
    D3DXQUATERNION first, second, result;
    memcpy(&first.w, &from->w, 4);
    memcpy(&first.x, &from->x, 12);
    memcpy(&second.w, &to->w, 4);
    memcpy(&second.x, &to->x, 12);
    D3DXQuaternionSlerp(&result, &first, &second, amount);
    out->w = result.w;
    memcpy(&out->x, &result.x, 12);
}

// Euler inputs are radians. Read them before storing the quaternion.
// 0x000b50a0
void GameQuaternionFromEuler(const CVector* angles, GameQuaternion* quaternion)
{
    const double x = angles->x * 0.5, y = angles->y * 0.5, z = angles->z * 0.5;
    const volatile float cx = (float)cos(x), cy = (float)cos(y);
    const double cz = cos(z);
    const double sx = sin(x);
    const volatile float sy = (float)sin(y), sz = (float)sin(z);
    const volatile float sxsy = (float)(sx * sy);
    quaternion->w = (float)(((double)cx * cy) * cz + (double)sxsy * sz);
    quaternion->x = (float)((cy * sx) * cz - ((double)sy * sz) * cx);
    quaternion->y = (float)((sx * sz) * cy + ((double)sy * cx) * cz);
    quaternion->z = (float)(((double)sz * cx) * cy - sxsy * cz);
}

// Native FPATAN and CRT asin return radians, without clamping or normalization.
// Inputs are re-read after each output store; overlapping buffers follow that order.
// 0x000b5160
void GameQuaternionToEuler(const GameQuaternion* quaternion, CVector* angles)
{
    const volatile float ww = quaternion->w * quaternion->w;
    const volatile float xx = quaternion->x * quaternion->x;
    const volatile float yy = quaternion->y * quaternion->y;
    const volatile float zz = quaternion->z * quaternion->z;
    angles->x = (float)atan2(2.0 * ((double)quaternion->y * quaternion->x + (double)quaternion->z * quaternion->w), ((double)xx - yy) - zz + ww);
    angles->y = (float)asin(-2.0 * ((double)quaternion->z * quaternion->x - (double)quaternion->y * quaternion->w));
    angles->z = (float)atan2(2.0 * ((double)quaternion->z * quaternion->y + (double)quaternion->x * quaternion->w), (double)zz - ((double)yy + xx) + ww);
}

// Selected static CRT helper, not a recovered SA-MP implementation.
// 0x000fd048
extern "C" double __cdecl _copysign(double magnitude, double sign);

// Native register/spill boundaries and unordered comparisons are observable.
// Square-root terms are clamped with the original x87 status tests, then signs
// are copied from the three antisymmetric differences by the selected CRT.
// 0x000b48b0
void GameMatrixComponentsToQuaternion(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22, float* w, float* x, float* y, float* z)
{
    float term = m00 + m11 + m22 + 1.0f;
    if (term < 0.0f)
        term = 0.0f;
    float qw = (float)(sqrt(term) * 0.5f);
    term = m00 + 1.0f - m11 - m22;
    if (term < 0.0f)
        term = 0.0f;
    float qx = (float)(sqrt(term) * 0.5f);
    term = 1.0f - m00 + m11 - m22;
    if (term < 0.0f)
        term = 0.0f;
    float qy = (float)(sqrt(term) * 0.5f);
    term = 1.0f - m00 - m11 + m22;
    if (term < 0.0f)
        term = 0.0f;
    float qz = (float)(sqrt(term) * 0.5f);
    if (qw < 0.0f)
        qw = 0.0f;
    if (qx < 0.0f)
        qx = 0.0f;
    if (qy < 0.0f)
        qy = 0.0f;
    if (qz < 0.0f)
        qz = 0.0f;
    float sx = (float)_copysign(qx, (double)(m21 - m12));
    float sy = (float)_copysign(qy, (double)(m02 - m20));
    float sz = (float)_copysign(qz, (double)(m10 - m01));
    *w = qw;
    *x = sx;
    *y = sy;
    *z = sz;
}

// Inputs are passed by value before the conversion publishes w,x,y,z.
// 0x000b4df0
void GameQuaternionFromMatrixComponents(const CMatrix* matrix, GameQuaternion* out)
{
    GameMatrixComponentsToQuaternion(matrix->right.x, matrix->right.y, matrix->right.z, matrix->up.x, matrix->up.y, matrix->up.z, matrix->at.x, matrix->at.y, matrix->at.z, &out->w,
                                     &out->x, &out->y, &out->z);
}

// Inputs are snapshotted before output. Preserve the original near-opposite
// fallback (-from.z,from.y,from.x), float spills, and final x,y,z,w store order.
// There is no additional zero-length guard.
// 0x000b4b60
void GameQuaternionFromVectors(GameQuaternion* out, const CVector* from, const CVector* to)
{
    const double fx = from->x, fy = from->y, fz = from->z;
    const double tx = to->x, ty = to->y, tz = to->z;
    double x = tz * fy - ty * fz;
    volatile float y = (float)(tx * fz - tz * fx);
    volatile float z = (float)(ty * fx - tx * fy);
    const double lengths = sqrt((tz * tz + ty * ty + tx * tx) * (fz * fz + fy * fy + fx * fx));
    const double dotPlusLengths = lengths + (tz * fz + ty * fy + tx * fx);
    volatile float w = (float)dotPlusLengths;
    if (dotPlusLengths < g_quaternionOppositeThreshold)
    {
        x = -fz;
        y = (float)fy;
        z = (float)fx;
        w = 0.0f;
    }
    const volatile float inverseLength = (float)(1.0 / sqrt((double)w * w + (double)z * z + (double)y * y + x * x));
    out->x = (float)(inverseLength * x);
    out->y = inverseLength * y;
    out->z = inverseLength * z;
    out->w = inverseLength * w;
}

// The four input components are passed by value, without normalization.
// Keep products in x87 registers until the native output stores. Output order
// is 00,11,22,10,01,20,02,21,12; output pointers may alias.
// 0x000b4a50
void GameQuaternionComponentsToMatrix(float w, float x, float y, float z, float* m00, float* m01, float* m02, float* m10, float* m11, float* m12, float* m20, float* m21,
                                      float* m22)
{
    float ww = w * w, xx = x * x, yy = y * y, zz = z * z;
    *m00 = ((xx - yy) - zz) + ww;
    *m11 = ((yy - xx) - zz) + ww;
    *m22 = (zz - (xx + yy)) + ww;
    float xy = x * y, wz = w * z;
    *m10 = 2.0f * (wz + xy);
    *m01 = 2.0f * (xy - wz);
    float xz = x * z, wy = w * y;
    *m20 = 2.0f * (xz - wy);
    *m02 = 2.0f * (wy + xz);
    float yz = y * z, wx = w * x;
    *m21 = 2.0f * (wx + yz);
    *m12 = 2.0f * (yz - wx);
}

// Native scalar x,y,z,w inputs; double output order and x87 spill boundaries matter.
// 0x0009d040
void GameQuaternionDoubleToEuler(double x, double y, double z, double w, double* roll, double* pitch, double* yaw)
{
    const volatile double yy = y * y;
    *roll = atan2(2.0 * (y * z + x * w), 1.0 - 2.0 * (x * x + yy));
    const double pitchTerm = 2.0 * (y * w - x * z);
    const volatile double roundedPitchTerm = pitchTerm;
    *pitch = fabs(pitchTerm) < 1.0 ? asin(roundedPitchTerm) : sin(roundedPitchTerm) * g_aimVectorHalfPi;
    *yaw = atan2(2.0 * (x * y + z * w), 1.0 - 2.0 * (z * z + yy));
}

// Half cross-product and half(1+dot), then normalization; no opposite fallback.
// 0x000b6530
void GameQuaternionFromVectorsHalf(GameQuaternion* out, const CVector* from, const CVector* to)
{
    const double x = (double)to->z * from->y - (double)to->y * from->z;
    const double y = (double)to->x * from->z - (double)to->z * from->x;
    const volatile float z = (float)((double)to->y * from->x - (double)to->x * from->y);
    const volatile float dot = (float)((double)from->x * to->x + (double)to->z * from->z + (double)to->y * from->y);
    out->x = (float)(x * 0.5);
    out->y = (float)(y * 0.5);
    out->z = z * 0.5f;
    out->w = (1.0f + dot) * 0.5f;
    GameQuaternionNormalize(out);
}

// x squared plus y squared plus z squared then w squared.
// 0x000b4cc0
float GameQuaternionLengthSquared(const GameQuaternion* quaternion)
{
    return quaternion->x * quaternion->x + quaternion->y * quaternion->y + quaternion->z * quaternion->z + quaternion->w * quaternion->w;
}

// Separate identical native adapter; original distinct name is unresolved.
// 0x000b4da0
void GameQuaternionFromMatrixComponents4DA0(const CMatrix* matrix, GameQuaternion* out)
{
    GameMatrixComponentsToQuaternion(matrix->right.x, matrix->right.y, matrix->right.z, matrix->up.x, matrix->up.y, matrix->up.z, matrix->at.x, matrix->at.y, matrix->at.z, &out->w,
                                     &out->x, &out->y, &out->z);
}

// Separate identical native adapter, no implicit normalization.
// 0x000b4f60
void GameQuaternionToMatrixComponents4F60(const GameQuaternion* quaternion, CMatrix* matrix)
{
    GameQuaternionComponentsToMatrix(quaternion->w, quaternion->x, quaternion->y, quaternion->z, &matrix->right.x, &matrix->right.y, &matrix->right.z, &matrix->up.x, &matrix->up.y,
                                     &matrix->up.z, &matrix->at.x, &matrix->at.y, &matrix->at.z);
}
