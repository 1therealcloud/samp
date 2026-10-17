#ifndef SAMP_GAME_HOOKS_RENDER_H
#define SAMP_GAME_HOOKS_RENDER_H
#include "game/hooks_independent.h"
// samp.dll RVA: 0x0018f504
extern DWORD g_hookHudScaleX;
// samp.dll RVA: 0x0018f508
extern DWORD g_hookHudScaleY;
// samp.dll RVA: 0x0018f50c
extern float g_hookHudWidth;
// samp.dll RVA: 0x0018f510
extern float g_hookHudHeight;
// samp.dll RVA: 0x00151a6e
extern BYTE g_hookHudAspectFix;
// samp.dll RVA: 0x0011c728
extern const float g_hookHudAspectThreshold;
// samp.dll RVA: 0x0011eb68
extern const float g_hookHudScaleNumerator;
// samp.dll RVA: 0x002aca6c
extern DWORD g_hookFogEnabled;
// samp.dll RVA: 0x002aca70
extern DWORD g_hookDisableFog;
// samp.dll RVA: 0x0018e560
extern DWORD g_hookCollisionPed;
// samp.dll RVA: 0x0018e8e0
extern DWORD g_hookSavedMatrix[16];
// samp.dll RVA: 0x0018eab4
extern DWORD g_hookSavedSpeed[3];
// samp.dll RVA: 0x0018f52c
extern DWORD g_hookHornArgument;
// samp.dll RVA: 0x0018f530
extern DWORD g_hookHornThis;
// samp.dll RVA: 0x0018eb24
extern DWORD g_hookHornVehicle;
// samp.dll RVA: 0x0018f534
extern BYTE g_hookHornStatus;
// samp.dll RVA: 0x0018f4c4
extern DWORD g_hookAnimationReturn;
// samp.dll RVA: 0x0018e584
extern DWORD g_hookAnimationClump;
// samp.dll RVA: 0x0018f548
extern DWORD g_hookAnimationGroup;
// samp.dll RVA: 0x0018f54c
extern DWORD g_hookAnimationId;
// samp.dll RVA: 0x0018f550
extern DWORD g_hookAnimationBlend;
// samp.dll RVA: 0x0018eb90
extern DWORD g_hookAnimationPed;
// samp.dll RVA: 0x0018eb88
extern DWORD g_hookAnimationLocalClump;
// samp.dll RVA: 0x0018f4ec
extern DWORD g_hookAnimationType;
// samp.dll RVA: 0x0018f4f0
extern DWORD g_hookQueuedAnimationGroup;
// samp.dll RVA: 0x0018f4f4
extern DWORD g_hookQueuedAnimationId;
// samp.dll RVA: 0x0018f4f8
extern DWORD g_hookQueuedAnimationBlend;
// samp.dll RVA: 0x00151aa4
extern DWORD g_hookAnimationContinueA;
// samp.dll RVA: 0x00151aa8
extern DWORD g_hookAnimationContinueB;
// samp.dll RVA: 0x0018f5cc
extern DWORD g_hookRenderObject;
// samp.dll RVA: 0x00151acc
extern BYTE g_hookObjectRestore[5];
// samp.dll RVA: 0x0018f5b4
extern DWORD g_hookMarker;
// samp.dll RVA: 0x0018f5b8
extern DWORD g_hookMarkerArgument;
// samp.dll RVA: 0x0018f5bc
extern DWORD g_hookSuppressAllMarkers;
// samp.dll RVA: 0x000a2220
void GamePrepareHudDraw();
// samp.dll RVA: 0x000c5400
void GameSetupD3DFog(DWORD enabled);
#endif
