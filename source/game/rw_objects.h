#ifndef SAMP_RW_OBJECTS_H
#define SAMP_RW_OBJECTS_H
#include <windows.h>
// Opaque GTA/RenderWare objects, not SA-MP CEntity/CCamera wrapper pointers.
typedef void *(__cdecl *GameRwObjectCallback)(void *object,void *data);
// samp.dll RVA: 0x000b1010
int GameRwCountClumpAtomics(void *clump);
// samp.dll RVA: 0x000b1090
void *GameRwForAllAtomics(void *clump,GameRwObjectCallback callback,void *data);
// samp.dll RVA: 0x000b10c0
void *GameRwForAllFrameObjects(void *frame,GameRwObjectCallback callback,void *data);
// samp.dll RVA: 0x000b1360
void GameRwDestroyTexture(void *texture);
// samp.dll RVA: 0x000b13d0
void GameRwDestroyFrame(void *frame);
// samp.dll RVA: 0x000b1400
void GameRwDestroyAtomic(void *atomic);
// samp.dll RVA: 0x000b1470
void GameRwDestroyRaster(void *raster);
// samp.dll RVA: 0x000b14f0
void GameRwClearCamera(void *camera,const void *color,DWORD flags);
// samp.dll RVA: 0x000b1520
void GameRwBeginCameraUpdate(void *camera);
// samp.dll RVA: 0x000b1550
void GameRwEndCameraUpdate(void *camera);
// samp.dll RVA: 0x000b15d0
void GameRwRenderClump(void *clump);
// samp.dll RVA: 0x000b1740
void GameRwAddClumpLight(void *clump,void *light);
// samp.dll RVA: 0x000b17a0
void GameRwSetLightRadius(void *light,float radius);
// samp.dll RVA: 0x000b1880
BOOL GameRwDestroyClump(void *clump);
// samp.dll RVA: 0x000b1030
void *GameRwQueryAtomic48(const void *object);
// samp.dll RVA: 0x000b1130
void *__stdcall GameRwReadField34(const void *object);
#endif
