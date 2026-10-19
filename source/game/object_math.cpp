#include "game/object_math.h"
// samp.dll RVA: 0x001181ac
extern const float g_objectMutationStep=0.1f;
// samp.dll RVA: 0x001181c4
extern const float g_objectRotationFactor=10.0f;
// samp.dll RVA: 0x0011f578
extern const float g_angleUpperBound=180.0f;
// samp.dll RVA: 0x0011f574
extern const float g_angleLowerBound=-180.0f;
// samp.dll RVA: 0x001181b4
extern const float g_anglePeriod=360.0f;
// samp.dll RVA: 0x001179b0
extern const float g_objectScaleLowerBound=0.0f;
// samp.dll RVA: 0x000b44e0
__declspec(naked) float __cdecl GameNormalizeAngle(float angle) {
    // Explicit x87 preserves the native extended ST0 result and unordered comparisons.
    __asm {
        fld dword ptr [esp+4]
        fcom g_angleUpperBound
        fnstsw ax
        test ah,41h
        jnz lower
        fsub g_anglePeriod
    lower:
        fcom g_angleLowerBound
        fnstsw ax
        test ah,5
        jp done
        fadd g_anglePeriod
    done:
        ret
    }
}
// samp.dll RVA: 0x000b4850
void GameTransformPoint(CVector *out,const CMatrix *matrix,const CVector *delta) {
    out->x=((matrix->at.x*delta->z+matrix->up.x*delta->y)+matrix->right.x*delta->x)+matrix->position.x;
    out->y=((matrix->at.y*delta->z+matrix->up.y*delta->y)+matrix->right.y*delta->x)+matrix->position.y;
    out->z=((matrix->at.z*delta->z+matrix->up.z*delta->y)+matrix->right.z*delta->x)+matrix->position.z;
}
