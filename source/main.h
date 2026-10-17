#ifndef SAMP_RECOVERED_RUNTIME_GLOBALS_H
#define SAMP_RECOVERED_RUNTIME_GLOBALS_H
#include <windows.h>
class CInput;
class CChat;
#include "game/game.h"
// samp.dll RVA: 0x002aca3c
extern CGame *g_pGame;
// samp.dll RVA: 0x002aca14
extern CInput *g_pInput;
// samp.dll RVA: 0x002aca10
extern CChat *g_pChat;
// samp.dll RVA: 0x002aca24
extern void *g_pNetGame;
// GTA SA VA: 0x00c17044 (outside samp.dll)
extern int g_chatScreenWidth; // GTA binding remains unresolved.
// GTA SA VA: 0x00c97c1c (outside samp.dll)
extern HWND g_inputGameWindow; // GTA binding remains unresolved.
#endif
