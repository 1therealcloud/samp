#ifndef SAMP_RECOVERED_SCM_EVENTS_H
#define SAMP_RECOVERED_SCM_EVENTS_H
#include <windows.h>
// samp.dll RVA: 0x00117690
extern int g_scmEventRpcId;
// samp.dll RVA: 0x00156988
extern DWORD g_scmEventParameters[4];
// samp.dll RVA: 0x00001d10
void GameProcessOutgoingEvent(DWORD vehicleHandle,DWORD param2,DWORD param3,DWORD eventType);
// Enters with ESI=CRunningScript; resumes GTA at 0x47bf2b instead of returning.
// samp.dll RVA: 0x000020c0
void GameSetEventOpcode();
#endif
