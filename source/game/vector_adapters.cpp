#include "game/vector_adapters.h"
#include "game/rw_transform.h"
#include "game/input.h"

// GTA SA virtual address: 0x0059C910, ECX receives the vector.
// samp.dll RVA: 0x000b4630
void GameNormalizeVector(CVector *vector) {
    __asm {
        mov ecx,vector
        mov edx,0x59c910
        call edx
    }
}
// GTA SA virtual address: 0x0059C730, cdecl out/left/right, returns out in EAX.
// samp.dll RVA: 0x000b4750
CVector *GameVectorCrossProduct(CVector *out,const CVector *left,const CVector *right) {
    return ((CVector *(__cdecl*)(CVector*,const CVector*,const CVector*))0x59c730)(out,left,right);
}
// GTA SA virtual address: 0x0059C810, cdecl out/vector/matrix.
// samp.dll RVA: 0x000b4790
CVector *GameTransformVectorByTranspose(CVector *out,const CVector *vector,const CMatrix *matrix) {
    return ((CVector *(__cdecl*)(CVector*,const CVector*,const CMatrix*))0x59c810)(out,vector,matrix);
}
// Backend quaternion layout is deliberately opaque, as in the other RW adapters.
// GTA SA virtual addresses: USA 0x007EB5C0; all other values use 0x007EB600.
// samp.dll RVA: 0x000b4d20
void GameRwQuaternionFromMatrix(void *quaternion,const CMatrix *matrix) {
    DWORD target=g_gameVersion==1?0x7eb5c0:0x7eb600;
    ((void(__cdecl*)(void*,const CMatrix*))target)(quaternion,matrix);
}
