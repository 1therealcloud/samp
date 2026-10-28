#ifndef SAMP_RECOVERED_ANIMATION_H
#define SAMP_RECOVERED_ANIMATION_H
#include <windows.h>
// 0x001419d0
extern char g_animationNames[1812][36];
// 0x001871a8
extern DWORD g_animationKeys[1812];
// 0x0009c870
WORD GameFindAnimation(const char* library, const char* name);
// 0x0009c950
char* GameGetAnimationName(short index);
// One-based index; a miss returns 0xffff in AX.
// 0x0009ca50
WORD GameFindAnimationByKey(DWORD key);
// 0x000b4030
DWORD __stdcall GameGetAnimationGroupKey(int group, int animation);
// 0x000b4080
DWORD __stdcall GameAnimationKey(const char* name);
// 0x0009c980
void GameInitializeAnimationKeys();
#endif
