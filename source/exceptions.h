#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <windows.h>

extern BYTE* g_clientCrashPedContext;

extern WORD g_clientCrashLastRenderedObject;

LONG WINAPI ClientExceptionFilter(EXCEPTION_POINTERS* exception);

#endif // EXCEPTIONS_H
