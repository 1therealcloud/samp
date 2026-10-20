#ifndef SAMP_RASTER_HELPERS_H
#define SAMP_RASTER_HELPERS_H
#include <windows.h>
// samp.dll RVA: 0x002a9d98
extern int g_rwImageDepth;
// samp.dll RVA: 0x002a9d9c
extern int g_rwImageWidth;
// samp.dll RVA: 0x002a9a40
extern int g_rwImageHeight;
// samp.dll RVA: 0x002aae10
extern void * g_rwImageScratch;
// samp.dll RVA: 0x00155400
extern void ** g_rwSceneCameraSlot;
// samp.dll RVA: 0x000b5690
void GameWriteRasterBmp(void *raster,const char *filename);
// samp.dll RVA: 0x000b57d0
void GameCreateSceneRasters();
// Raw GPR-preserving entry, not a RenderWare camera method.
// samp.dll RVA: 0x000b5860
void GameRenderWithSceneRasters();
#endif
