#ifndef SAMP_RECOVERED_ANIMATION_H
#define SAMP_RECOVERED_ANIMATION_H
#include <windows.h>
// samp.dll RVA: 0x001419d0
extern char g_animationNames[1812][36];
// samp.dll RVA: 0x001871a8
extern DWORD g_animationKeys[1812];
// samp.dll RVA: 0x00017440
void GameUppercaseAscii(char *text);
// samp.dll RVA: 0x0009c870
WORD GameFindAnimation(const char *library,const char *name);
// samp.dll RVA: 0x0009c950
char *GameGetAnimationName(short index);
// One-based index; a miss returns 0xffff in AX.
// samp.dll RVA: 0x0009ca50
WORD GameFindAnimationByKey(DWORD key);
// samp.dll RVA: 0x000b4030
DWORD __stdcall GameGetAnimationGroupKey(int group,int animation);
// samp.dll RVA: 0x000b4080
DWORD __stdcall GameAnimationKey(const char *name);
// samp.dll RVA: 0x0009c980
void GameInitializeAnimationKeys();
#endif
