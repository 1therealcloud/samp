#include "game/building_removal.h"
// samp.dll RVA: 0x00188df8
BuildingRemovalRecord g_buildingRemovalRecords[1000];
// samp.dll RVA: 0x0018dc18
int g_buildingRemovalCount;
// Native ST0 returns an unspilled x87 result. Order is dz^2 + dy^2 + dx^2;
// reassociation or a float temporary changes radius-boundary comparisons.
// samp.dll RVA: 0x000b4570
__declspec(naked) float __stdcall GameDistanceBetweenPoints(float x,float y,float z,float otherX,float otherY,float otherZ) {
    __asm {
        fld dword ptr [esp+4]
        fsub dword ptr [esp+16]
        fld dword ptr [esp+8]
        fsub dword ptr [esp+20]
        fld dword ptr [esp+12]
        fsub dword ptr [esp+24]
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
        ret 24
    }
}
// samp.dll RVA: 0x0009cae0
BOOL GameShouldSuppressMarker(int model,float x,float y,float z) {
    if(g_buildingRemovalCount) {
        int index=0;
        do {
            const BuildingRemovalRecord &entry=g_buildingRemovalRecords[index];
            if(entry.model==-1 || entry.model==model) {
                if(GameDistanceBetweenPoints(x,y,z,entry.x,entry.y,entry.z)<=entry.radius)return TRUE;
            }
            ++index;
        } while(index!=g_buildingRemovalCount);
    }
    return FALSE;
}
