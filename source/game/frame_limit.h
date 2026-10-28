#ifndef SAMP_FRAME_LIMIT_H
#define SAMP_FRAME_LIMIT_H
#include <windows.h>
// 0x0018e520
extern BYTE g_frameKeyCycle;
// 0x0018e524
extern DWORD g_frameKeyCycleTick;
// 0x0018e528
extern DWORD g_frameSampleCount;
// 0x0018e52c
extern DWORD g_frameSampleTick;
// 0x0018e530
extern DWORD g_frameSleepTick;
// 0x0018e534
extern float g_frameScale;
// 0x001181d8
extern const float g_frameScaleMultiplier;
// 0x0011c664
extern const float g_frameScaleNumerator;
// 0x0009fe50
void GameUpdateKeyCycle();
#endif
