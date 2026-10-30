#ifndef DXUT_MEDIA_H
#define DXUT_MEDIA_H
#include <windows.h>
// 0x000972b0
HRESULT DXUTFindDXSDKMediaFileCch(char* dest, int size, const char* filename);
// 0x000961c0
bool DXUTFindMediaSearchTypicalDirs(char* dest, int size, const char* leaf, char* exePath, char* exeName);
// 0x00096320
bool DXUTFindMediaSearchParentDirs(char* dest, int size, char* start, char* leaf);
// 0x00093290
char* InputDxutMediaSearchPath();
// 0x00181ac8
extern char g_dxutMediaSearchPath[MAX_PATH];
#endif
