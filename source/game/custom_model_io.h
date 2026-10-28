#ifndef SAMP_CUSTOM_MODEL_IO_H
#define SAMP_CUSTOM_MODEL_IO_H
#include <windows.h>
// Descriptive interfaces for observed model-info prefixes; not complete classes.
// samp.dll RVA: 0x000b5900
BOOL GameFileExists(const char* path);
// samp.dll RVA: 0x000a77c0
BOOL GameRegisterCustomPed(int base, int model, const char* name, const char* dff, const char* txd);
// samp.dll RVA: 0x000a7820
BOOL GameRegisterCustomObject(int base, int model, const char* name, const char* dff, const char* txd);
// samp.dll RVA: 0x000a7670
void* GameClonePedModelInfo(int base, int model);
// samp.dll RVA: 0x000a76c0
void* GameCloneObjectModelInfo(int base, int model);
// samp.dll RVA: 0x000a7750
BOOL GameAttachModelTxd(int model, const char* name, const char* path);
// samp.dll RVA: 0x000b1b10
void* GameLoadPedDff(const char* path);
// samp.dll RVA: 0x000b1bc0
BOOL GameLoadObjectDff(const char* path, int model);
// samp.dll RVA: 0x000b41d0
void __stdcall GamePublishPedClump(int model, void* clump);
// samp.dll RVA: 0x000b34a0
int GameTxdAdd(const char* name);
// samp.dll RVA: 0x000b34c0
DWORD GameTxdLoad(int index, const char* path);
// samp.dll RVA: 0x000b4260
void __stdcall GameSetModelTxd(int model, int index);
// samp.dll RVA: 0x000b19c0
void* GameRwStreamOpen(int type, int access, const void* data);
// samp.dll RVA: 0x000b1a00
BOOL GameRwStreamClose(void* stream, void* data);
// samp.dll RVA: 0x000b1a80
BOOL GameRwFindChunk(void* stream, DWORD type, DWORD* length, DWORD* version);
// samp.dll RVA: 0x000b1ad0
void* GameRwReadClump(void* stream);
// samp.dll RVA: 0x000b1ba0
DWORD GameLoadAtomicStream(void* stream, int model);
// samp.dll RVA: 0x000b4330
void __stdcall GameSetModelField8(int model, WORD value);
// samp.dll RVA: 0x000b40e0
BOOL __stdcall GameIsObjectModelInfo(void* info);
// samp.dll RVA: 0x001e0448
extern DWORD g_gameLoadingCustomModel;
#endif
