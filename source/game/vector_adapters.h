#ifndef SAMP_VECTOR_ADAPTERS_H
#define SAMP_VECTOR_ADAPTERS_H
#include "game/entity.h"
// samp.dll RVA: 0x000b4630
void GameNormalizeVector(CVector *vector);
// samp.dll RVA: 0x000b4750
CVector *GameVectorCrossProduct(CVector *out,const CVector *left,const CVector *right);
// Transposed basis only; no translation.
// samp.dll RVA: 0x000b4790
CVector *GameTransformVectorByTranspose(CVector *out,const CVector *vector,const CMatrix *matrix);
#endif
