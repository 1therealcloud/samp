#include "game/entity.h"
#include <math.h>
// samp.dll RVA: 0x0009fb40
void CEntity::ApplyMatrix(CMatrix matrix) {
    BYTE *entity=(BYTE*)m_pGameEntity;
    if(entity&&*(void**)(entity+0x14)) {
        DWORD *vtable=*(DWORD**)entity;
        ((void(__fastcall*)(void*,void*))vtable[3])(entity,0);
        SetMatrix(matrix);UpdateRwFrame();
        ((void(__fastcall*)(void*,void*))vtable[2])(entity,0);
    }
}
// samp.dll RVA: 0x0009f0f0
float CEntity::GetDistanceToPoint(float x,float y,float z) {
    CMatrix matrix;GetMatrix(&matrix);
    // sqrt(dx^2+dy^2+dz^2), retaining unrounded differences and the extended ST0 return.
    __asm {
        lea eax,matrix
        fld dword ptr [eax+38h]
        fsub z
        fld dword ptr [eax+34h]
        fsub y
        fld dword ptr [eax+30h]
        fsub x
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fld st(3)
        fmul st(0),st(4)
        faddp st(1),st(0)
        fsqrt
        fstp st(3)
        fstp st(0)
        fstp st(0)
    }
}
