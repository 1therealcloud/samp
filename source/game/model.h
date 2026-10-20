#ifndef SAMP_GAME_MODEL_H
#define SAMP_GAME_MODEL_H
#include "game/scripting.h"
// Physical native bank ends at the following enable flag. The enabled lookup
// does not check indices; adjacency outside this bank is not proven equivalent.
// samp.dll RVA: 0x001c0444
extern void *g_gameCustomModelInfo[32768];
// samp.dll RVA: 0x001e0444
extern DWORD g_gameUseCustomModelInfo;
// samp.dll RVA: 0x00152ae0
extern void **g_gameModelInfoTable;
// samp.dll RVA: 0x0011e664
extern const Scripting::OpcodeInfo g_gameRequestModelCommand;
// samp.dll RVA: 0x0011e678
extern const Scripting::OpcodeInfo g_gameLoadModelsCommand;
// samp.dll RVA: 0x0011e6a0
extern const Scripting::OpcodeInfo g_gameModelAvailableCommand;
// samp.dll RVA: 0x0011e618
extern const char g_gameModelTimeoutWarning[41];
// samp.dll RVA: 0x000a7630
void *__stdcall GameGetModelInfo(int model);
// Observed raw DWORD at model-info +0x1c. No additional null guard is native.
// samp.dll RVA: 0x000b41a0
DWORD __stdcall GameGetModelData1c(int model);
// samp.dll RVA: 0x000b39b0
BOOL __stdcall GameHasModelInfo(int model);
// samp.dll RVA: 0x000b39d0
BOOL __stdcall GameIsPedModel(int model);
// Observed wrapped vtable-address-plus-14 test, not a slot-content query.
// samp.dll RVA: 0x000b4170
BOOL __stdcall GameModelQueryB4170(int model);
struct CVector;
// samp.dll RVA: 0x000b5ef0
float __cdecl GameSetModelCollisionRadius(int model,float radius);
// samp.dll RVA: 0x000b5f50
BOOL __cdecl GameGetModelCollisionCentre(int model,CVector *centre);
// samp.dll RVA: 0x000b5fe0
BYTE __cdecl GameGetModelFlagsByte(int model);
// samp.dll RVA: 0x000b6000
WORD __cdecl GameGetModelFlagsWord(int model);
// Raw EAX from model-info vtable +2C, gated by the existing game pointer.
// samp.dll RVA: 0x000b5d90
DWORD __cdecl GameQueryModelVirtual2c(int model);
// samp.dll RVA: 0x000b4150
void __stdcall GameSetModelDrawDistance(void *info,float distance);
// samp.dll RVA: 0x000b4160
float __stdcall GameGetModelDrawDistance(const void *info);
// samp.dll RVA: 0x000b4200
void __stdcall GameSetModelRenderObjectCall3c(int model,void *object);
// samp.dll RVA: 0x000b4230
void __stdcall GameSetModelCollision(int model,void *collision);
// samp.dll RVA: 0x000b4280
DWORD __stdcall GameGetModelTxdIndex(int model);
// samp.dll RVA: 0x000b42d0
void *__stdcall GameGetModelCollision(int model);
// samp.dll RVA: 0x000b4300
WORD __stdcall GameGetModelReferenceCount(int model);
// samp.dll RVA: 0x000b1c40
BOOL GameCleanupModelAndTxd(int model);
// samp.dll RVA: 0x000b1070
void GameClearModelCollision(int model);
#endif
