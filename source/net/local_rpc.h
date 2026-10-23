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

struct CVector;
class CMatrix;
// samp.dll RVA: 0x001569a8
extern CVector g_localSurfPedTurnSpeed;
// samp.dll RVA: 0x001569b8
extern CMatrix g_localSurfPedMatrix;
// samp.dll RVA: 0x001569f8
extern CVector g_localSurfEntitySpeed;
// samp.dll RVA: 0x00156a08
extern CMatrix g_localSurfInverseMatrix;
// samp.dll RVA: 0x00156a48
extern CVector g_localSurfPedSpeed;
// samp.dll RVA: 0x00156a58
extern CMatrix g_localSurfEntityMatrix;
// samp.dll RVA: 0x00156aa4
extern CVector g_localSurfEntityTurnSpeed;
