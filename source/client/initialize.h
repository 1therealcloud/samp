#ifndef SAMP_CLIENT_INITIALIZE_H
#define SAMP_CLIENT_INITIALIZE_H
#include <windows.h>
struct IDirect3DDevice9;
// Native start addresses; buffer backing windows are bounded by the next known native buffer.
// Overflow adjacency is not established by these declarations.
// samp.dll RVA: 0x002ac498
extern char g_clientConfigFilename[520];
// samp.dll RVA: 0x002ac6a0
extern char g_clientChatFilename[264];
// samp.dll RVA: 0x002abe70
extern IDirect3DDevice9 *g_clientSavedGameDevice;
// samp.dll RVA: 0x002aca40
extern DWORD g_clientDeviceInitialized;
// samp.dll RVA: 0x002aca44
extern DWORD g_clientNetworkInitialized;
// samp.dll RVA: 0x000c5e70
void ClientInitializeDeviceAndGui();
#include "client/commands.h"
#include "game/debugscript.h"
#endif
