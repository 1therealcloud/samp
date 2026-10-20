#include "game/model.h"
#include "game/entity.h"
#include "game/object_math.h"

// Preserve the second lookup: its result is deliberately not null-checked again.
// samp.dll RVA: 0x000b5ef0
float __cdecl GameSetModelCollisionRadius(int model,float radius) {
    if(GameGetModelInfo(model)) {
        BYTE *info=(BYTE*)GameGetModelInfo(model);
        BYTE *collision=*(BYTE**)(info+0x14);
        if(collision) {
            *(float*)(collision+0x24)=radius;
            return radius;
        }
    }
    return g_objectScaleLowerBound;
}
// Publish one raw DWORD at a time, preserving alias order and NaN payload bits.
// samp.dll RVA: 0x000b5f50
BOOL __cdecl GameGetModelCollisionCentre(int model,CVector *centre) {
    if(GameGetModelInfo(model)) {
        BYTE *info=(BYTE*)GameGetModelInfo(model);
        BYTE *collision=*(BYTE**)(info+0x14);
        if(collision) {
            *(DWORD*)&centre->x=*(DWORD*)(collision+0x18);
            *(DWORD*)&centre->y=*(DWORD*)(collision+0x1c);
            *(DWORD*)&centre->z=*(DWORD*)(collision+0x20);
            return TRUE;
        }
    }
    return FALSE;
}
// Native exposes only AL; no meaning is assigned to individual flag bits.
// samp.dll RVA: 0x000b5fe0
BYTE __cdecl GameGetModelFlagsByte(int model) {
    BYTE *info=(BYTE*)GameGetModelInfo(model);
    return info?*(BYTE*)(info+0x12):0;
}
// Native exposes AX; this reads the full overlapping two-byte field.
// samp.dll RVA: 0x000b6000
WORD __cdecl GameGetModelFlagsWord(int model) {
    BYTE *info=(BYTE*)GameGetModelInfo(model);
    return info?*(WORD*)(info+0x12):0;
}
