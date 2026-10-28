#ifndef SAMP_GTA_GLOBALS_H
#define SAMP_GTA_GLOBALS_H
#include <windows.h>

#ifdef SAMP_DIRECT_GTA_GLOBALS
// GTA SA VA: 0x00c17044 (outside samp.dll)
#define g_chatScreenWidth (*(int*)0x00c17044)
// GTA SA VA: 0x00c97c1c (outside samp.dll)
#define g_inputGameWindow (*(HWND*)0x00c97c1c)
#else
// External bindings retained for the existing isolated diagnostic profiles.
// GTA SA VA: 0x00c17044 (outside samp.dll)
extern int g_chatScreenWidth;
// GTA SA VA: 0x00c97c1c (outside samp.dll)
extern HWND g_inputGameWindow;
#endif

#endif
