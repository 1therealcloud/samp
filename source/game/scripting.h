#ifndef SAMP_RECOVERED_SCRIPTING_H
#define SAMP_RECOVERED_SCRIPTING_H
#include <windows.h>
// Layout fields are not established yet; native Initialize allocates and clears 224 bytes.
struct CRunningScript { BYTE opaque[224]; };
// samp.dll RVA: 0x002a93f0
extern CRunningScript *g_scriptThread;
namespace Scripting {
    #pragma pack(push,1)
    struct OpcodeInfo { WORD opcode;char parameters[16]; };
    #pragma pack(pop)
    // samp.dll RVA: 0x000b2150
    void Initialize();
    // samp.dll RVA: 0x000b1ed0
    DWORD ExecBuffer();
    // samp.dll RVA: 0x000b1f10
    DWORD __cdecl ProcessCommand(const OpcodeInfo *command,...);
}
// samp.dll RVA: 0x002a92f0
extern BYTE g_scriptBuffer[256];
// samp.dll RVA: 0x002a92a8
extern DWORD *g_scriptVariables[18];
// samp.dll RVA: 0x002a93f4
extern DWORD g_scriptLastOpcode;
// samp.dll RVA: 0x002a93f8
extern BOOL g_scriptLocalDebug;
// samp.dll RVA: 0x00155390
extern FARPROC g_scriptProcessOneCommand;
// samp.dll RVA: 0x00163a70
extern int g_gameExceptionCount;
// samp.dll RVA: 0x00060470
int __stdcall GameExceptionFilter(DWORD code,EXCEPTION_POINTERS *pointers,const char *warning);
#endif
