#ifndef SAMP_PED_CONTROL_H
#define SAMP_PED_CONTROL_H
#include "game/hooks_control.h"
#include "game/scripting.h"
#include "net/playerpool.h"
#include "net/remoteplayer.h"
// samp.dll RVA: 0x0018e8c0
extern DWORD g_hookPedControlField540;
// samp.dll RVA: 0x0018f378
extern CPlayerPool* g_hookFaultPlayerPool;
// samp.dll RVA: 0x00151970
extern WORD g_hookFaultPlayerId;
// samp.dll RVA: 0x0018e824
extern CRemotePlayer* g_hookFaultRemotePlayer;
// samp.dll RVA: 0x00151a70
extern const BYTE g_hookPedRestoreA[6];
// samp.dll RVA: 0x00151a88
extern const BYTE g_hookPedRestoreB[6];
// samp.dll RVA: 0x00151a90
extern const BYTE g_hookPedRemoteRestore[5];
// samp.dll RVA: 0x0011e7b8
extern const Scripting::OpcodeInfo g_gameCurrentInteriorCommand;
// samp.dll RVA: 0x0011ebcc
extern const char g_hookPedExceptionLabel[10];
// samp.dll RVA: 0x0011ebac
extern const char g_hookPedRemovedMessage[32];
// samp.dll RVA: 0x000a2530
void GameRunPedControlBackend();
// samp.dll RVA: unknown (reconstruction-only body helper extracted from A26D0; no standalone native entry)
void GameProcessPedControlBody();
#endif
