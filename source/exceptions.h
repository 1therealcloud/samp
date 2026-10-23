#ifndef SAMP_CLIENT_EXCEPTIONS_H
#define SAMP_CLIENT_EXCEPTIONS_H
#include <windows.h>
// samp.dll RVA: 0x0015fa68
extern CONTEXT *g_clientExceptionContext;
// samp.dll RVA: 0x0015fa70
extern char g_clientCrashText[16384];
// samp.dll RVA: 0x00151960
extern BYTE *g_clientCrashPedContext;
// samp.dll RVA: 0x0018f584
extern WORD g_clientCrashLastRenderedObject;
// samp.dll RVA: 0x00060430
LONG WINAPI ClientExceptionFilter(EXCEPTION_POINTERS *exception);
// samp.dll RVA: 0x00060330
INT_PTR CALLBACK ClientExceptionDialog(HWND dialog,UINT message,WPARAM wParam,LPARAM lParam);
// samp.dll RVA: 0x00060070
void ClientConstructCrashText(BOOL includeModules);
// samp.dll RVA: 0x0005fd50
void ClientDumpMemory(const BYTE *data,int count,char *text,BOOL asDwords);
// Observed pool prefixes only; no complete pool/class layout claim.
// samp.dll RVA: 0x0005fc20
void ClientDumpNetworkState(char *text);
// samp.dll RVA: 0x0005fee0
void ClientDumpLoadedModules(char *text);
// samp.dll RVA: 0x00060260
void ClientSendCrashReport();
#endif
