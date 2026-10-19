#ifndef SAMP_CLIENT_FRAME_H
#define SAMP_CLIENT_FRAME_H
#include <windows.h>
// samp.dll RVA: 0x002aca60
extern DWORD g_clientIntegrityFailures;
// samp.dll RVA: 0x002aca64
extern DWORD g_clientIntegrityFailed;
// samp.dll RVA: 0x002aca68
extern DWORD g_clientSkipGameFrame;
// samp.dll RVA: 0x002ac9c0
extern DWORD g_clientQuitRequested;
// samp.dll RVA: 0x002ac9c4
extern DWORD g_clientQuitStarted;
// samp.dll RVA: 0x000c6630
void ClientProcessFrame();
// Naked callback preserves the two native PUSHAD regions and GTA return registers.
// samp.dll RVA: 0x000c66b0
void ClientGameLoop();
// Stage 2/remaining startup dependency; complete device/UI initialization is open.
// samp.dll RVA: 0x000c5e70
void ClientInitializeDeviceAndGui();
#endif
