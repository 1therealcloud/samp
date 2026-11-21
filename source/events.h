#ifndef EVENTS_H
#define EVENTS_H

#include <windows.h>
// 0x000e5620
extern int g_scmEventRpcId;
// R5 address to verify
extern DWORD g_scmEventParameters[4];
// 0x00001a60
void GameProcessIncomingEvent(WORD player, DWORD eventType, WORD vehicle, DWORD param2, DWORD param3);
// 0x00001ce0
void GameProcessOutgoingEvent(DWORD vehicleHandle, DWORD param2, DWORD param3, DWORD eventType);
// Enters with ESI=CRunningScript; resumes GTA at 0x47bf2b instead of returning.
// R5 address to verify
void GameSetEventOpcode();
#endif

#ifndef SAMP_RECOVERED_DRIVER_OPCODE_H
#define SAMP_RECOVERED_DRIVER_OPCODE_H
#include "game/playerped.h"
// R5 address to verify
extern CPed* g_driverPed;
// R5 address to verify
extern BOOL g_driverCompareFlag;
// R5 address to verify
extern void* g_driverScriptThread;

#endif // EVENTS_H
