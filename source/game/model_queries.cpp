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
