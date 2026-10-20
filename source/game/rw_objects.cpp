#include "game/rw_objects.h"
#include "game/input.h"

// GTA SA virtual address: 0x007498E0 (fixed).
// samp.dll RVA: 0x000b1010
int GameRwCountClumpAtomics(void *clump) {
    return ((int(__cdecl*)(void*))0x7498e0)(clump);
}

// GTA SA virtual address: 0x00749B70 (fixed).
// samp.dll RVA: 0x000b1090
void *GameRwForAllAtomics(void *clump,GameRwObjectCallback callback,void *data) {
    return ((void*(__cdecl*)(void*,GameRwObjectCallback,void*))0x749b70)(clump,callback,data);
}

// GTA SA virtual address: 0x007F1200; every non-USA version uses +0x40.
// samp.dll RVA: 0x000b10c0
void *GameRwForAllFrameObjects(void *frame,GameRwObjectCallback callback,void *data) {
    DWORD target=g_gameVersion==1?0x7f1200:0x7f1240;
    return ((void*(__cdecl*)(void*,GameRwObjectCallback,void*))target)(frame,callback,data);
}

// GTA SA virtual address: 0x007F3820; every non-USA version uses +0x40.
// samp.dll RVA: 0x000b1360
void GameRwDestroyTexture(void *texture) {
    DWORD target=g_gameVersion==1?0x7f3820:0x7f3860;
    ((BOOL(__cdecl*)(void*))target)(texture);
}

// GTA SA virtual address: 0x007F05A0; every non-USA version uses +0x40.
// samp.dll RVA: 0x000b13d0
void GameRwDestroyFrame(void *frame) {
    DWORD target=g_gameVersion==1?0x7f05a0:0x7f05e0;
    ((BOOL(__cdecl*)(void*))target)(frame);
}

// GTA SA virtual address: 0x00749DC0; every non-USA version uses +0x50.
// samp.dll RVA: 0x000b1400
void GameRwDestroyAtomic(void *atomic) {
    DWORD target=g_gameVersion==1?0x749dc0:0x749e10;
    ((BOOL(__cdecl*)(void*))target)(atomic);
}

// GTA SA virtual address: 0x007FB020; every non-USA version uses +0x40.
// samp.dll RVA: 0x000b1470
void GameRwDestroyRaster(void *raster) {
    DWORD target=g_gameVersion==1?0x7fb020:0x7fb060;
    ((BOOL(__cdecl*)(void*))target)(raster);
}

// GTA SA virtual address: 0x007EE340; every non-USA version uses +0x40.
// samp.dll RVA: 0x000b14f0
void GameRwClearCamera(void *camera,const void *color,DWORD flags) {
    DWORD target=g_gameVersion==1?0x7ee340:0x7ee380;
    ((void*(__cdecl*)(void*,const void*,DWORD))target)(camera,color,flags);
}

// GTA SA virtual address: 0x007EE190; every non-USA version uses +0x40.
// samp.dll RVA: 0x000b1520
void GameRwBeginCameraUpdate(void *camera) {
    DWORD target=g_gameVersion==1?0x7ee190:0x7ee1d0;
    ((void*(__cdecl*)(void*))target)(camera);
}

// GTA SA virtual address: 0x007EE180; every non-USA version uses +0x40.
// samp.dll RVA: 0x000b1550
void GameRwEndCameraUpdate(void *camera) {
    DWORD target=g_gameVersion==1?0x7ee180:0x7ee1c0;
    ((void*(__cdecl*)(void*))target)(camera);
}

// GTA SA virtual address: 0x00749B20; every non-USA version uses +0x50.
// samp.dll RVA: 0x000b15d0
void GameRwRenderClump(void *clump) {
    DWORD target=g_gameVersion==1?0x749b20:0x749b70;
    ((void*(__cdecl*)(void*))target)(clump);
}

// GTA SA virtual address: 0x0074A4F0; every non-USA version uses +0x50.
// samp.dll RVA: 0x000b1740
void GameRwAddClumpLight(void *clump,void *light) {
    DWORD target=g_gameVersion==1?0x74a4f0:0x74a540;
    ((void*(__cdecl*)(void*,void*))target)(clump,light);
}

// GTA SA virtual address: 0x00751A70 (fixed).
// samp.dll RVA: 0x000b17a0
void GameRwSetLightRadius(void *light,float radius) {
    ((void*(__cdecl*)(void*,float))0x751a70)(light,radius);
}

// GTA SA virtual address: 0x0074A310; every non-USA version uses +0x50.
// samp.dll RVA: 0x000b1880
BOOL GameRwDestroyClump(void *clump) {
    DWORD target=g_gameVersion==1?0x74a310:0x74a360;
    return ((BOOL(__cdecl*)(void*))target)(clump);
}

// Native object tag must be1 before the raw +48 field is read.
// Field/callback semantics remain deliberately opaque in this interface.
// samp.dll RVA: 0x000b1030
void *GameRwQueryAtomic48(const void *object) {
    const BYTE *bytes=(const BYTE*)object;
    return bytes && *bytes==1?*(void* const*)(bytes+0x48):0;
}

// Nullable raw +34 read; no full backend object layout is inferred.
// samp.dll RVA: 0x000b1130
void *__stdcall GameRwReadField34(const void *object) {
    return object?*(void* const*)((const BYTE*)object+0x34):0;
}
