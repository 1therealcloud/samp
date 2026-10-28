#ifndef SAMP_LOCAL_RPC_H
#define SAMP_LOCAL_RPC_H
#include "game/scripting.h"
// 0x0011787c
extern int g_localSpawnRequestRpc;
// 0x00117880
extern int g_localSpawnRpc;
// Skip one SetInitialState call when nonzero; broader startup meaning unresolved.
// 0x0013c0a0
extern DWORD g_localSkipInitialState;
// 0x00117954
extern int g_localRequestClassRpc;
// 0x00117958
extern int g_localInteriorChangeRpc;
// 0x0011795c
extern int g_localChatRpc;
// 0x00117960
extern int g_localEnterVehicleRpc;
// 0x00117964
extern int g_localExitVehicleRpc;
// 0x0011796c
extern int g_localPlayerDamageRpc;
// 0x00117974
extern int g_localActorDamageRpc;
// 0x00117970
extern const float g_localDamageScale;
// 0x00117840
extern const Scripting::OpcodeInfo g_localTrainCameraCommand;
// 0x0013c0a4
extern DWORD g_localTrainCameraTick;
#endif

struct CVector;
class CMatrix;
// 0x001569a8
extern CVector g_localSurfPedTurnSpeed;
// 0x001569b8
extern CMatrix g_localSurfPedMatrix;
// 0x001569f8
extern CVector g_localSurfEntitySpeed;
// 0x00156a08
extern CMatrix g_localSurfInverseMatrix;
// 0x00156a48
extern CVector g_localSurfPedSpeed;
// 0x00156a58
extern CMatrix g_localSurfEntityMatrix;
// 0x00156aa4
extern CVector g_localSurfEntityTurnSpeed;

// 0x0013c0a8
extern DWORD g_localOnFootSendRate;
// 0x0013c0ac
extern DWORD g_localInCarSendRate;
