#ifndef SAMP_CLIENT_DEVICE_SETUP_H
#define SAMP_CLIENT_DEVICE_SETUP_H
#include <windows.h>
// Observed native buffers include the 261st byte cleared by startup.
// samp.dll RVA: 0x002ac7a8
extern char g_clientDataDirectory[261];
// samp.dll RVA: 0x002ac8b0
extern char g_clientCacheDirectory[261];
// samp.dll RVA: 0x0016be78
extern WNDPROC g_clientPreviousWindowProcedure;
// samp.dll RVA: 0x002aca50
extern DWORD g_clientPreviousRenderState;
// samp.dll RVA: 0x002aca54
extern DWORD g_clientRenderCaller;
// samp.dll RVA: 0x002aca58
extern DWORD g_clientRenderState;
// samp.dll RVA: 0x002aca5c
extern DWORD g_clientRenderValue;
// Native _stat success test accepts any existing file type.
// samp.dll RVA: 0x000b5900
BOOL ClientPathExists(const char *path);
// samp.dll RVA: 0x000c51c0
void ClientInitializeCacheDirectory();
// samp.dll RVA: 0x000c5340
void ClientInitializePaths();
// samp.dll RVA: 0x00061970
BOOL ClientInstallWindowProcedure();
// Address-only dependency; full input handler remains open.
// samp.dll RVA: 0x000610d0
LRESULT CALLBACK ClientWindowProcedure(HWND window,UINT message,WPARAM wparam,LPARAM lparam);
// Naked two-word GTA render-state callback; tail-jumps through original target.
// samp.dll RVA: 0x000c5450
void ClientRenderStateHook();
#endif
