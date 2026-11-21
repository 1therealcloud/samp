#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <windows.h>
// 0x00121a50
extern CONTEXT* g_clientExceptionContext;
// 0x00121a58
extern char g_clientCrashText[16384];
// 0x00113978
extern BYTE* g_clientCrashPedContext;
// R5 address to verify
extern WORD g_clientCrashLastRenderedObject;
// 0x00060970
LONG WINAPI ClientExceptionFilter(EXCEPTION_POINTERS* exception);
// 0x00060870
INT_PTR CALLBACK ClientExceptionDialog(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);
// 0x000605b0
void ClientConstructCrashText(BOOL includeModules);
// 0x00060290
void ClientDumpMemory(const BYTE* data, int count, char* text, BOOL asDwords);
// Observed pool prefixes only; no complete pool/class layout claim.
// R5 address to verify
void ClientDumpNetworkState(char* text);
// 0x00060420
void ClientDumpLoadedModules(char* text);
// 0x000607a0
void ClientSendCrashReport();

#endif // EXCEPTIONS_H
