#ifndef SAMP_GAME_LIMIT_DATA_H
#define SAMP_GAME_LIMIT_DATA_H
#include <windows.h>
// 0x001546b0
extern BYTE g_gamePoolPatchOne[7];
// 0x001546b8
extern BYTE g_gamePoolPatchTwo[5];
// 0x001546f8
extern DWORD g_gameModelReferenceSites[14];
// 0x00154730
extern DWORD g_gameScanDirectUSA[14];
// 0x00154768
extern DWORD g_gameScanDirectEU[14];
// 0x001547a0
extern DWORD g_gameScanOperandUSA[56];
// 0x00154880
extern DWORD g_gameScanOperandEU[56];
// 0x00154960
extern DWORD g_gameScanSecondSites[11];
// 0x0015498c
extern DWORD g_gamePedReferenceSites[4];
// 0x001549a0
extern BYTE g_gameDefaultModelFlags[1024];
#endif
