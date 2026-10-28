#ifndef SAMP_RW_PREVIEW_H
#define SAMP_RW_PREVIEW_H
#include "game/entity.h"
// 0x0015535c
extern const CVector g_rwPreviewAxes[3];
// 0x000b1700
void* GameRwCreateLight(int type);
// 0x000b1770
void GameRwSetLightColor(void* light, const float* rgba);
// 0x000b12d0
void* GameRwCreateRaster(int width, int height, int depth, int flags);
// 0x000b1430
void* GameRwCreateCamera();
// 0x000b1390
void* GameRwCreateFrame();
// 0x000b18f0
void GameRwTranslateFrame(void* frame, const CVector* position, int combine);
// 0x000b1920
void GameRwRotateFrame(void* frame, int axis, float angle);
// 0x000b15b0
void GameRwSetCameraRasters(void* camera, void* color, void* depth);
// 0x000b1580
void GameRwSetObjectFrame(void* object, void* frame);
// 0x000b1680
void GameRwSetClipDistances(void* camera, float farClip, float nearClip);
// 0x000b16d0
void GameRwSetViewWindow(void* camera, const float* window);
// 0x000b18c0
void GameRwSetProjection(void* camera, int projection);
// 0x000b14b0
void GameRwAddCameraToWorld(void* camera);
// 0x000b1320
void* GameRwCreateTexture(void* raster);
#endif
