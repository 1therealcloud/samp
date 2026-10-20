#ifndef SAMP_LOCAL_RPC_H
#define SAMP_LOCAL_RPC_H
#include "game/scripting.h"
// samp.dll RVA: 0x00117960
extern int g_localEnterVehicleRpc;
// samp.dll RVA: 0x00117964
extern int g_localExitVehicleRpc;
// samp.dll RVA: 0x0011796c
extern int g_localPlayerDamageRpc;
// samp.dll RVA: 0x00117974
extern int g_localActorDamageRpc;
// samp.dll RVA: 0x00117970
extern const float g_localDamageScale;
// samp.dll RVA: 0x00117840
extern const Scripting::OpcodeInfo g_localTrainCameraCommand;
// samp.dll RVA: 0x0013c0a4
extern DWORD g_localTrainCameraTick;
#endif
