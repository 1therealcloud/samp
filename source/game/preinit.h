#ifndef SAMP_RECOVERED_GAME_PREINIT_H
#define SAMP_RECOVERED_GAME_PREINIT_H
#include "game/input.h"
// samp.dll RVA: 0x000aa060
void GameWaitUnprotect(BYTE *address,SIZE_T size,BYTE expected);
// samp.dll RVA: 0x000aa300
BOOL GameInitializeVersionPatches();
// samp.dll RVA: 0x000a5c10
DWORD __cdecl GameFrameNodeNameHook(void *arg0,DWORD length,void *arg2);
// samp.dll RVA: 0x000a5c90
DWORD __cdecl GameDataReadHook(void *arg0,DWORD length,void *arg2);
// samp.dll RVA: 0x000b1a40
DWORD GameReadStream(void *stream,void *buffer,DWORD length);
// samp.dll RVA: 0x000b6610
void GameParseStreamData(const void *data,DWORD length,void *destination);
// samp.dll RVA: 0x002a9270
extern DWORD g_gamePhysicalMegabytes;
// samp.dll RVA: 0x002a9274
extern DWORD g_gameStreamingBytes;
// samp.dll RVA: 0x0011ee84
extern const char g_gameStartupTitle[6];
// samp.dll RVA: 0x0018f388
extern char g_gameFrameNodeError[256];
// samp.dll RVA: 0x0018f6b4
extern bool g_gameCustomStreamRead;
// samp.dll RVA: 0x0018f6b8
extern void *g_gameStreamBuffer;
#endif
