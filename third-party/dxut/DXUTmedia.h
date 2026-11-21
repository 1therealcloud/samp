#ifndef DXUT_MEDIA_H
#define DXUT_MEDIA_H
#include <windows.h>
HRESULT DXUTFindDXSDKMediaFileCch(char* dest, int size, const char* filename);
bool DXUTFindMediaSearchTypicalDirs(char* dest, int size, const char* leaf, char* exePath, char* exeName);
bool DXUTFindMediaSearchParentDirs(char* dest, int size, char* start, char* leaf);
char* InputDxutMediaSearchPath();
extern char g_dxutMediaSearchPath[MAX_PATH];
#endif
