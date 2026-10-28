#ifndef SAMP_DXUT_MEDIA_NATIVE_H
#define SAMP_DXUT_MEDIA_NATIVE_H
#include <windows.h>
// samp.dll RVA: 0x000972b0
HRESULT DXUTFindDXSDKMediaFileCch(char* dest, int size, const char* filename);
// samp.dll RVA: 0x000961c0
bool DXUTFindMediaSearchTypicalDirs(char* dest, int size, const char* leaf, char* exePath, char* exeName);
// samp.dll RVA: 0x00096320
bool DXUTFindMediaSearchParentDirs(char* dest, int size, char* start, char* leaf);
// samp.dll RVA: 0x00093290
char* InputDxutMediaSearchPath();
// samp.dll RVA: 0x00181ac8
extern char g_dxutMediaSearchPath[MAX_PATH];
#endif
