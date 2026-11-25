#include "vector.h"

#include <math.h>

float DotProduct(const CVector* A, const CVector* B)
{
    return A->x * B->x + A->y * B->y + A->z * B->z;
}

void CrossProduct(CVector* Result, const CVector* A, const CVector* B)
{
    Result->x = B->z * A->y - B->y * A->z;
    Result->y = B->x * A->z - B->z * A->x;
    Result->z = B->y * A->x - B->x * A->y;
}

void Subtract(CVector* Result, const CVector* A, const CVector* B)
{
    Result->x = A->x - B->x;
    Result->y = A->y - B->y;
    Result->z = A->z - B->z;
}

void Add(CVector* Result, const CVector* A, const CVector* B)
{
    Result->x = A->x + B->x;
    Result->y = A->y + B->y;
    Result->z = A->z + B->z;
}

void Normalize(CVector* Result, const CVector* Value)
{
    float Scale = 1.0f / static_cast<float>(sqrt(Value->z * Value->z + Value->y * Value->y + Value->x * Value->x));
    Result->x = Scale * Value->x;
    Result->y = Scale * Value->y;
    Result->z = Scale * Value->z;
}
