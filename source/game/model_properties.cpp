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

// These helpers receive an existing backend model-info pointer, not a model ID.
// samp.dll RVA: 0x000b4150
void __stdcall GameSetModelDrawDistance(void *info,float distance) {
    *(float*)((BYTE*)info+0x18)=distance;
}
// samp.dll RVA: 0x000b4160
float __stdcall GameGetModelDrawDistance(const void *info) {
    return *(const float*)((const BYTE*)info+0x18);
}

// Native writes +1C before invoking the backend virtual slot. Its meaning
// remains unresolved; preserve the unchecked model lookup and callee-pop call.
// samp.dll RVA: 0x000b4200
void __stdcall GameSetModelRenderObjectCall3c(int model,void *object) {
    BYTE *info=(BYTE*)GameGetModelInfo(model);
    *(void**)(info+0x1c)=object;
    __asm {
        mov ecx,info
        push object
        mov edx,dword ptr [ecx]
        call dword ptr [edx+0x3c]
    }
}

// GTA SA virtual address: 0x004C4BC0; ECX=model info, initPairedModel=0.
// samp.dll RVA: 0x000b4230
void __stdcall GameSetModelCollision(int model,void *collision) {
    void *info=GameGetModelInfo(model);
    __asm {
        mov ecx,info
        push 0
        push collision
        mov edx,0x4c4bc0
        call edx
    }
}

// Native zero-extends the WORD, including FFFF -> 65535; lookup failure -> -1.
// samp.dll RVA: 0x000b4280
DWORD __stdcall GameGetModelTxdIndex(int model) {
    const BYTE *info=(const BYTE*)GameGetModelInfo(model);
    return info?*(const WORD*)(info+0xa):0xffffffffu;
}

// No null check is present in either of the following native entries.
// samp.dll RVA: 0x000b42d0
void *__stdcall GameGetModelCollision(int model) {
    BYTE *info=(BYTE*)GameGetModelInfo(model);
    return *(void**)(info+0x14);
}
// samp.dll RVA: 0x000b4300
WORD __stdcall GameGetModelReferenceCount(int model) {
    const BYTE *info=(const BYTE*)GameGetModelInfo(model);
    return *(const WORD*)(info+8);
}

#include "game/effects.h"
// Native preserves the first model-info pointer across the second lookup made
// by the TXD-index helper. Backend call is ECX, no added failure cleanup.
// samp.dll RVA: 0x000b1c40
BOOL GameCleanupModelAndTxd(int model) {
    void *info=GameGetModelInfo(model);
    if(!info)return FALSE;
    int index=(int)GameGetModelTxdIndex(model);
    __asm {
        mov ecx,info
        mov edx,0x4c4d50
        call edx
    }
    if(GameTxdGetReferenceCount(index)==0) {
        GameTxdRemove(index);
        GameTxdRemoveSlot(index);
    }
    return TRUE;
}

// Clear the observed collision slot only; no destructor or release is native.
// samp.dll RVA: 0x000b1070
void GameClearModelCollision(int model) {
    BYTE *info=(BYTE*)GameGetModelInfo(model);
    if(info)*(void**)(info+0x14)=0;
}
