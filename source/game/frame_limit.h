#ifndef SAMP_FRAME_LIMIT_H
#define SAMP_FRAME_LIMIT_H
#include <windows.h>
// samp.dll RVA: 0x0018e520
extern BYTE g_frameKeyCycle;
// samp.dll RVA: 0x0018e524
extern DWORD g_frameKeyCycleTick;
// samp.dll RVA: 0x0018e528
extern DWORD g_frameSampleCount;
// samp.dll RVA: 0x0018e52c
extern DWORD g_frameSampleTick;
// samp.dll RVA: 0x0018e530
extern DWORD g_frameSleepTick;
// samp.dll RVA: 0x0018e534
extern float g_frameScale;
// samp.dll RVA: 0x001181d8
extern const float g_frameScaleMultiplier;
// samp.dll RVA: 0x0011c664
extern const float g_frameScaleNumerator;
// samp.dll RVA: 0x0009fe50
void GameUpdateKeyCycle();
// samp.dll RVA: 0x000b3ca0
DWORD GameGetTaskType(void *task);
#endif
