#ifndef SAMP_RECOVERED_GAME_LIMITS_H
#define SAMP_RECOVERED_GAME_LIMITS_H
#include "game/limit_data.h"
#pragma pack(push,1)
struct GamePedModelRecord { DWORD vtable;BYTE data[64]; };
struct GameModelRecord32 { DWORD vtable;BYTE data[28]; };
#pragma pack(pop)
// samp.dll RVA: 0x001e04b0
extern DWORD g_gameScanList[40000];
// samp.dll RVA: 0x002075b0
extern GamePedModelRecord g_gamePedModels[319];
// samp.dll RVA: 0x0020ca70
extern GameModelRecord32 g_gameModelRecords[20000];
// samp.dll RVA: 0x002a8e70
extern BYTE g_gameModelFlags[1024];
// samp.dll RVA: 0x000aa6a0
void GameRelocateScanList();
// samp.dll RVA: 0x000aa5e0
void GameRelocatePedModels();
// samp.dll RVA: 0x000aa630
void GameRelocateModelRecords();
// samp.dll RVA: 0x000aa980
void GameRelocateModelFlags();
// samp.dll RVA: 0x000aa180
void GameIncreasePoolLimits();
#endif
