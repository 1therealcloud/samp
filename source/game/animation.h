#ifndef ANIMATION_H
#define ANIMATION_H

#include <windows.h>
// 0x001039e8
extern char g_animationNames[1812][36];
// 0x00149318
extern DWORD g_animationKeys[1812];
// 0x0009cdf0
WORD GameFindAnimation(const char* library, const char* name);
// 0x0009ced0
char* GameGetAnimationName(short index);
// One-based index; a miss returns 0xffff in AX.
// 0x0009cfd0
WORD GameFindAnimationByKey(DWORD key);
// 0x000b4430
DWORD __stdcall GameGetAnimationGroupKey(int group, int animation);
// 0x000b4480
DWORD __stdcall GameAnimationKey(const char* name);
// 0x0009cf00
void GameInitializeAnimationKeys();

#endif // ANIMATION_H
