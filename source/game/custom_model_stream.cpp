#include "game/custom_model_io.h"
#include "game/input.h"
#include "game/preinit.h"

// samp.dll RVA: 0x000b19c0
void *GameRwStreamOpen(int type,int access,const void *data) {
    // GTA SA virtual addresses; every version other than 1 uses the EU path.
    DWORD target=g_gameVersion==1?0x7ecef0:0x7ecf30;
    return ((void*(__cdecl*)(int,int,const void*))target)(type,access,data);
}
// samp.dll RVA: 0x000b1a00
BOOL GameRwStreamClose(void *stream,void *data) {
    DWORD target=g_gameVersion==1?0x7ece20:0x7ece60;
    return ((BOOL(__cdecl*)(void*,void*))target)(stream,data);
}
// samp.dll RVA: 0x000b1a80
BOOL GameRwFindChunk(void *stream,DWORD type,DWORD *length,DWORD *version) {
    DWORD target=g_gameVersion==1?0x7ed2d0:0x7ed310;
    return ((BOOL(__cdecl*)(void*,DWORD,DWORD*,DWORD*))target)(stream,type,length,version);
}
// samp.dll RVA: 0x000b1ad0
void *GameRwReadClump(void *stream) {
    DWORD target=g_gameVersion==1?0x74b420:0x74b470;
    return ((void*(__cdecl*)(void*))target)(stream);
}
// samp.dll RVA: 0x000b1ba0
DWORD GameLoadAtomicStream(void *stream,int model) {
    // GTA SA virtual address; preserve zero-extension of AL.
    return ((BYTE(__cdecl*)(void*,int))0x5371f0)(stream,model);
}
// samp.dll RVA: 0x000b1b10
void *GameLoadPedDff(const char *path) {
    void *stream=GameRwStreamOpen(2,1,path);
    if(!stream)return 0;
    // GTA SA virtual address, restored to the native fixed byte, not a snapshot.
    BYTE *patch=(BYTE*)0x41b1d0;
    GameUnprotect(patch,1);
    *patch=0xc3;
    g_gameCustomStreamRead=true;
    void *clump=0;
    if(GameRwFindChunk(stream,0x10,0,0))clump=GameRwReadClump(stream);
    GameRwStreamClose(stream,0);
    *patch=0x64;
    g_gameCustomStreamRead=false;
    return clump;
}
// samp.dll RVA: 0x000b1bc0
BOOL GameLoadObjectDff(const char *path,int model) {
    void *stream=GameRwStreamOpen(2,1,path);
    if(!stream)return FALSE;
    BYTE *patch=(BYTE*)0x41b1d0;
    GameUnprotect(patch,1);
    *patch=0xc3;
    g_gameCustomStreamRead=true;
    if(!GameLoadAtomicStream(stream,model)) {
        // Native failure leaves the stream open.
        *patch=0x64;
        g_gameCustomStreamRead=false;
        return FALSE;
    }
    GameRwStreamClose(stream,0);
    GameSetModelField8(model,0);
    *patch=0x64;
    g_gameCustomStreamRead=false;
    return TRUE;
}
