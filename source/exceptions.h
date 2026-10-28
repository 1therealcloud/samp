#ifndef SAMP_CLIENT_EXCEPTIONS_H
#define SAMP_CLIENT_EXCEPTIONS_H
#include <windows.h>
// 0x0015fa68
extern CONTEXT* g_clientExceptionContext;
// 0x0015fa70
extern char g_clientCrashText[16384];
// 0x00151960
extern BYTE* g_clientCrashPedContext;
// 0x0018f584
extern WORD g_clientCrashLastRenderedObject;
// 0x00060430
LONG WINAPI ClientExceptionFilter(EXCEPTION_POINTERS* exception);
// 0x00060330
INT_PTR CALLBACK ClientExceptionDialog(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);
// 0x00060070
void ClientConstructCrashText(BOOL includeModules);
// 0x0005fd50
void ClientDumpMemory(const BYTE* data, int count, char* text, BOOL asDwords);
// Observed pool prefixes only; no complete pool/class layout claim.
// 0x0005fc20
void ClientDumpNetworkState(char* text);
// 0x0005fee0
void ClientDumpLoadedModules(char* text);
// 0x00060260
void ClientSendCrashReport();
#endif
