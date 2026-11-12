#include "vector.h"
#include <math.h>

// 0x00002c50
float DotProduct(const CVector* A, const CVector* B)
{
    return A->x * B->x + A->y * B->y + A->z * B->z;
}

// 0x00002c70
void CrossProduct(CVector* Result, const CVector* A, const CVector* B)
{
    Result->x = B->z * A->y - B->y * A->z;
    Result->y = B->x * A->z - B->z * A->x;
    Result->z = B->y * A->x - B->x * A->y;
}

// 0x00002cb0
void Subtract(CVector* Result, const CVector* A, const CVector* B)
{
    Result->x = A->x - B->x;
    Result->y = A->y - B->y;
    Result->z = A->z - B->z;
}

// 0x00002ce0
void Add(CVector* Result, const CVector* A, const CVector* B)
{
    Result->x = A->x + B->x;
    Result->y = A->y + B->y;
    Result->z = A->z + B->z;
}

// 0x00002d10
void Normalize(CVector* Result, const CVector* Value)
{
    float Scale = 1.0f / static_cast<float>(sqrt(Value->z * Value->z +
        Value->y * Value->y + Value->x * Value->x));
    Result->x = Scale * Value->x;
    Result->y = Scale * Value->y;
    Result->z = Scale * Value->z;
}
