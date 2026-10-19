#include "game/quaternion.h"
#include "game/input.h"
#include <d3dx9.h>
#include <string.h>
// samp.dll RVA: 0x000b4d50
void GameQuaternionFromMatrix(const CMatrix *matrix,GameQuaternion *out) {
    D3DXQUATERNION quaternion;
    // GTA SA virtual addresses: USA 7EB5C0; all other version values use EU 7EB600.
    DWORD target=g_gameVersion==1?0x7eb5c0:0x7eb600;
    ((void(__cdecl*)(D3DXQUATERNION*,const CMatrix*))target)(&quaternion,matrix);
    // Native w publication is an x87 float load/store; xyz are raw DWORD copies.
    __asm {
        mov eax,out
        fld quaternion.w
        fstp dword ptr [eax]
    }
    memcpy(&out->x,&quaternion.x,12);
}
// samp.dll RVA: 0x000b5040
void GameQuaternionNormalize(GameQuaternion *quaternion) {
    D3DXQUATERNION input,output;
    memcpy(&input.x,&quaternion->x,12);memcpy(&input.w,&quaternion->w,4);
    D3DXQuaternionNormalize(&output,&input);
    memcpy(&quaternion->w,&output.w,4);memcpy(&quaternion->x,&output.x,12);
}
