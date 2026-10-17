#ifndef SAMP_RECOVERED_GAME_INPUT_H
#define SAMP_RECOVERED_GAME_INPUT_H
#include "main.h"
#include <d3d9.h>
// samp.dll RVA: 0x000aa0b0
void GameUnprotect(void *address,SIZE_T size);
// samp.dll RVA: 0x000a02f0
void GameClearMouseDelta();
// samp.dll RVA: 0x000a0310
void GameUpdateMouse();
// samp.dll RVA: 0x000a0320
void GameDisableMousePointers();
// samp.dll RVA: 0x000a0370
void GameRestoreMousePointers();
// samp.dll RVA: 0x000a03c0
void GameDisableMouseCalls();
// samp.dll RVA: 0x002ac9d0
extern IDirect3DDevice9 *g_gameD3DDevice;
// samp.dll RVA: 0x002ac9b8
extern int g_gameVersion;
// samp.dll RVA: 0x0018e1c8
extern DWORD g_gameMouseDummy;
// samp.dll RVA: 0x00151918
extern BYTE g_gameKeyCall[5];
// samp.dll RVA: 0x00151920
extern BYTE g_gameMouseCallEU[5];
// samp.dll RVA: 0x00151928
extern BYTE g_gameMouseCallUS[5];
#endif
