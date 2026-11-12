#ifndef KDFF_VECTOR_H
#define KDFF_VECTOR_H

struct CVector
{
    float x, y, z;
};

// 0x00002c50
float DotProduct(const CVector* A, const CVector* B);
// 0x00002c70
void CrossProduct(CVector* Result, const CVector* A, const CVector* B);
// 0x00002cb0
void Subtract(CVector* Result, const CVector* A, const CVector* B);
// 0x00002ce0
void Add(CVector* Result, const CVector* A, const CVector* B);
// 0x00002d10
void Normalize(CVector* Result, const CVector* Value);

#endif
