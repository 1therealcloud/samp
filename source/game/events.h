#ifndef EVENTS_H
#define EVENTS_H

#include <windows.h>

extern int g_scmEventRpcId;

extern DWORD g_scmEventParameters[4];

void GameProcessIncomingEvent(WORD player, DWORD eventType, WORD vehicle, DWORD param2, DWORD param3);

void GameProcessOutgoingEvent(DWORD vehicleHandle, DWORD param2, DWORD param3, DWORD eventType);
// Enters with ESI=CRunningScript; resumes GTA at 0x47bf2b instead of returning.

void GameSetEventOpcode();
#endif

#ifndef SAMP_RECOVERED_DRIVER_OPCODE_H
#define SAMP_RECOVERED_DRIVER_OPCODE_H
#include "game/playerped.h"

extern CPed* g_driverPed;

extern BOOL g_driverCompareFlag;

extern void* g_driverScriptThread;

#endif // EVENTS_H
