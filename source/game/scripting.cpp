#include "game/scripting.h"
#include <string.h>
// samp.dll RVA: 0x002a93f0
CRunningScript* g_scriptThread;
// samp.dll RVA: 0x000b2150
void Scripting::Initialize()
{
    g_scriptThread = (CRunningScript*)operator new(224);
    memset(g_scriptThread, 0, 224);
}

#include "chatwindow.h"
#include "main.h"
#include <stdarg.h>
// samp.dll RVA: 0x002a92f0
BYTE g_scriptBuffer[256];
// samp.dll RVA: 0x002a92a8
DWORD* g_scriptVariables[18];
// samp.dll RVA: 0x002a93f4
DWORD g_scriptLastOpcode;
// samp.dll RVA: 0x002a93f8
BOOL g_scriptLocalDebug;
// samp.dll RVA: 0x00155390
FARPROC g_scriptProcessOneCommand = (FARPROC)0x469eb0;
// samp.dll RVA: 0x00163a70
int g_gameExceptionCount;
// samp.dll RVA: 0x00060470
int __stdcall GameExceptionFilter(DWORD code, EXCEPTION_POINTERS* pointers, const char* warning)
{
    if (g_pChat)
    {
        if (!strcmp(warning, "opcode"))
            g_pChat->AddDebugMessage("Warning(opcode 0x%X): Exception 0x%X at 0x%X", g_scriptLastOpcode, code, pointers->ContextRecord->Eip);
        else
            g_pChat->AddDebugMessage("Warning(%s): Exception 0x%X at 0x%X", warning, code, pointers->ContextRecord->Eip);
    }
    if (g_gameExceptionCount < 10)
    {
        ++g_gameExceptionCount;
        return EXCEPTION_EXECUTE_HANDLER;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}
// samp.dll RVA: 0x000b1ed0
DWORD Scripting::ExecBuffer()
{
    g_scriptLocalDebug = TRUE;
    BYTE* buffer = g_scriptBuffer;
    memcpy(g_scriptThread->opaque + 0x14, &buffer, 4);
    // clang-format off
    __asm {
        mov ecx,g_scriptThread
        call g_scriptProcessOneCommand
    }
    // clang-format on
    DWORD result;
    memcpy(&result, g_scriptThread->opaque + 0xc5, 4);
    g_scriptLocalDebug = FALSE;
    return result;
}
// samp.dll RVA: 0x000b1f10
DWORD __cdecl Scripting::ProcessCommand(const OpcodeInfo* command, ...)
{
    va_list args;
    va_start(args, command);
    memcpy(g_scriptBuffer, &command->opcode, 2);
    int position = 2;
    WORD variables = 0;
    memset(g_scriptThread->opaque + 0x3c, 0, 18 * 4);
    g_scriptLastOpcode = command->opcode;
    for (const char* format = command->parameters; *format; format++)
    {
        switch (*format)
        {
            case 'i':
            {
                DWORD value = va_arg(args, DWORD);
                g_scriptBuffer[position++] = 1;
                memcpy(g_scriptBuffer + position, &value, 4);
                position += 4;
                break;
            }
            case 'f':
            {
                float value = (float)va_arg(args, double);
                g_scriptBuffer[position++] = 6;
                memcpy(g_scriptBuffer + position, &value, 4);
                position += 4;
                break;
            }
            case 'v':
            {
                DWORD* value = va_arg(args, DWORD*);
                g_scriptBuffer[position++] = 3;
                g_scriptVariables[variables] = value;
                memcpy(g_scriptThread->opaque + 0x3c + variables * 4, value, 4);
                memcpy(g_scriptBuffer + position, &variables, 2);
                position += 2;
                ++variables;
                break;
            }
            case 's':
            {
                const char* text = va_arg(args, const char*);
                BYTE length = (BYTE)strlen(text);
                g_scriptBuffer[position++] = 14;
                g_scriptBuffer[position++] = length;
                memcpy(g_scriptBuffer + position, text, length);
                position += length;
                break;
            }
            case 'z':
                g_scriptBuffer[position++] = 0;
                break;
            default:
                va_end(args);
                return 0;
        }
    }
    va_end(args);
    DWORD result = 0;
    // clang-format off
    __asm fwait
        // clang-format on
        __try
    {
        result = ExecBuffer();
        for (WORD i = 0; i < variables; i++)
            memcpy(g_scriptVariables[i], g_scriptThread->opaque + 0x3c + i * 4, 4);
        // clang-format off
        __asm fwait
        // clang-format on
    }
    __except (GameExceptionFilter(GetExceptionCode(), GetExceptionInformation(), "opcode")) {}
    return result;
}
