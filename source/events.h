#ifndef SAMP_RECOVERED_SCM_EVENTS_H
#define SAMP_RECOVERED_SCM_EVENTS_H
#include <windows.h>
// 0x00117690
extern int g_scmEventRpcId;
// 0x00156988
extern DWORD g_scmEventParameters[4];
// 0x00001aa0
void GameProcessIncomingEvent(WORD player, DWORD eventType, WORD vehicle, DWORD param2, DWORD param3);
// 0x00001d10
void GameProcessOutgoingEvent(DWORD vehicleHandle, DWORD param2, DWORD param3, DWORD eventType);
// Enters with ESI=CRunningScript; resumes GTA at 0x47bf2b instead of returning.
// 0x000020c0
void GameSetEventOpcode();
#endif

#ifndef SAMP_RECOVERED_DRIVER_OPCODE_H
#define SAMP_RECOVERED_DRIVER_OPCODE_H
#include "game/playerped.h"
// 0x00156980
extern CPed* g_driverPed;
// 0x00156998
extern BOOL g_driverCompareFlag;
// 0x0015699c
extern void* g_driverScriptThread;
#endif
