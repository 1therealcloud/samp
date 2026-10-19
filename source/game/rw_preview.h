#ifndef SAMP_RW_PREVIEW_H
#define SAMP_RW_PREVIEW_H
#include "game/entity.h"
// samp.dll RVA: 0x0015535c
extern const CVector g_rwPreviewAxes[3];
// samp.dll RVA: 0x000b1700
void * GameRwCreateLight(int type);
// samp.dll RVA: 0x000b1770
void GameRwSetLightColor(void *light,const float *rgba);
// samp.dll RVA: 0x000b12d0
void * GameRwCreateRaster(int width,int height,int depth,int flags);
// samp.dll RVA: 0x000b1430
void * GameRwCreateCamera();
// samp.dll RVA: 0x000b1390
void * GameRwCreateFrame();
// samp.dll RVA: 0x000b18f0
void GameRwTranslateFrame(void *frame,const CVector *position,int combine);
// samp.dll RVA: 0x000b1920
void GameRwRotateFrame(void *frame,int axis,float angle);
// samp.dll RVA: 0x000b15b0
void GameRwSetCameraRasters(void *camera,void *color,void *depth);
// samp.dll RVA: 0x000b1580
void GameRwSetObjectFrame(void *object,void *frame);
// samp.dll RVA: 0x000b1680
void GameRwSetClipDistances(void *camera,float farClip,float nearClip);
// samp.dll RVA: 0x000b16d0
void GameRwSetViewWindow(void *camera,const float *window);
// samp.dll RVA: 0x000b18c0
void GameRwSetProjection(void *camera,int projection);
// samp.dll RVA: 0x000b14b0
void GameRwAddCameraToWorld(void *camera);
// samp.dll RVA: 0x000b1320
void * GameRwCreateTexture(void *raster);
#endif
