#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <windows.h>

extern CONTEXT* g_clientExceptionContext;

extern char g_clientCrashText[16384];

extern BYTE* g_clientCrashPedContext;

extern WORD g_clientCrashLastRenderedObject;

LONG WINAPI ClientExceptionFilter(EXCEPTION_POINTERS* exception);

INT_PTR CALLBACK ClientExceptionDialog(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);

void ClientConstructCrashText(BOOL includeModules);

void ClientDumpMemory(const BYTE* data, int count, char (&text)[16384], BOOL asDwords);
// Observed pool prefixes only; no complete pool/class layout claim.

void ClientDumpNetworkState(char (&text)[16384]);

void ClientDumpLoadedModules(char (&text)[16384]);

void ClientSendCrashReport();

#endif // EXCEPTIONS_H
