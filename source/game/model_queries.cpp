#include "game/model.h"

// samp.dll RVA: 0x000b39b0
BOOL __stdcall GameHasModelInfo(int model) { return GameGetModelInfo(model)!=0; }
// samp.dll RVA: 0x000b39d0
BOOL __stdcall GameIsPedModel(int model) {
    if(model>=0 && model<=30000) {
        void *info=GameGetModelInfo(model);
        // GTA SA virtual vtable address, not a samp.dll RVA.
        if(info && *(DWORD*)info==0x85bdc0)return TRUE;
    }
    return FALSE;
}
// samp.dll RVA: 0x000b4170
BOOL __stdcall GameModelQueryB4170(int model) {
    void *info=GameGetModelInfo(model);
    return info && (*(DWORD*)info+0x14u)!=0;
}

#include "main.h"
// Vtable slot semantics remain unknown; do not relabel this as model type.
// Both successful lookup checks are native, including the second null guard.
// samp.dll RVA: 0x000b5d90
DWORD __cdecl GameQueryModelVirtual2c(int model) {
    if(!g_pGame || !GameGetModelInfo(model))return 0;
    void *info=GameGetModelInfo(model);
    if(!info)return 0;
    DWORD result;
    __asm {
        mov ecx,info
        mov edx,dword ptr [ecx]
        call dword ptr [edx + 0x2c]
        mov result,eax
    }
    return result;
}
