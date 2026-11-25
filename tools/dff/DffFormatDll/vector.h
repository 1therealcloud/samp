#ifndef KDFF_VECTOR_H
#define KDFF_VECTOR_H

struct CVector
{
    float x, y, z;
};

float DotProduct(const CVector* A, const CVector* B);

void CrossProduct(CVector* Result, const CVector* A, const CVector* B);

void Subtract(CVector* Result, const CVector* A, const CVector* B);

void Add(CVector* Result, const CVector* A, const CVector* B);

void Normalize(CVector* Result, const CVector* Value);

#endif
