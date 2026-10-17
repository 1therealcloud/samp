#ifndef SAMP_GAME_HOOKS_INDEPENDENT_H
#define SAMP_GAME_HOOKS_INDEPENDENT_H
#include "game/hooks.h"
// Raw ABI entries are declared in hooks.h for address installation only.
// Recovered bodies are naked because callers supply registers or an in-flight
// GTA stack frame, and some paths tail-jump into the original game function.
// samp.dll RVA: 0x0018f4c8
extern DWORD g_hookRadarIndex;
// samp.dll RVA: 0x0018f4cc
extern DWORD g_hookCheatFunction;
// samp.dll RVA: 0x0018f4b0
extern DWORD g_hookCameraPed;
// samp.dll RVA: 0x0018f540
extern DWORD g_hookRenderEsi;
// samp.dll RVA: 0x0018f544
extern DWORD g_hookRenderEbx;
// samp.dll RVA: 0x0018f568
extern DWORD g_hookFreedBuffer;
// samp.dll RVA: 0x00151ab8
extern DWORD g_hookKeepBuffer;
// samp.dll RVA: 0x0018f560
extern DWORD g_hookCachedBuffer;
// samp.dll RVA: 0x00151abc
extern DWORD g_hookAllocateFunction;
// samp.dll RVA: 0x0018f570
extern DWORD g_hookSavedEsi;
// samp.dll RVA: 0x0018f5c4
extern DWORD g_hookRenderReturn;
// samp.dll RVA: 0x00151ac8
extern BYTE g_hookRenderRestore[3];
// samp.dll RVA: 0x0018f5d4
extern DWORD g_hookSavedFontA;
// samp.dll RVA: 0x0018f5d8
extern DWORD g_hookSavedFontB;
#endif
