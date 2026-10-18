#ifndef SAMP_CLIENT_BOOTSTRAP_H
#define SAMP_CLIENT_BOOTSTRAP_H
#include <windows.h>
// samp.dll RVA: 0x002ac9c8
extern HMODULE g_clientModule;
// samp.dll RVA: 0x000c6780
BOOL WINAPI ClientDllMain(HINSTANCE module,DWORD reason,LPVOID reserved);
// samp.dll RVA: 0x000c6930
BOOL WINAPI ClientDllMainForward(HINSTANCE module,DWORD reason,LPVOID reserved);
// Dependency: exception filter body remains unrecovered.
// samp.dll RVA: 0x00060430
LONG WINAPI ClientExceptionFilter(EXCEPTION_POINTERS *exception);
// Stored callback only; frame/device implementation remains unrecovered.
// samp.dll RVA: 0x000c66b0
void ClientGameLoop();
#endif
