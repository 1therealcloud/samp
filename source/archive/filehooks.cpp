#include "archive/filehooks.h"
#include <detours.h>
#include <string.h>
// samp.dll RVA: 0x0016bf60
ArchiveFileRecord g_archiveOpenRecords[50];
// samp.dll RVA: 0x0016be98
BOOL g_archiveRecordUsed[50];
// samp.dll RVA: 0x0016c378
BOOL g_archiveHooksInstalled;
// samp.dll RVA: 0x0016c34c
BYTE *g_archiveRealGetFileSize;
// samp.dll RVA: 0x0016c350
BYTE *g_archiveRealSetFilePointer;
// samp.dll RVA: 0x0016c354
BYTE *g_archiveRealCreateFileA;
// samp.dll RVA: 0x0016c35c
BYTE *g_archiveRealReadFile;
// samp.dll RVA: 0x0016c360
BYTE *g_archiveRealCloseHandle;
// samp.dll RVA: 0x0016c364
BYTE *g_archiveRealGetFileType;
// samp.dll RVA: 0x0016c368
BYTE *g_archiveRealShowCursor;
// samp.dll RVA: 0x00062720
void ArchiveInstallFileHooks() {
    if(g_archiveHooksInstalled)return;
    memset(g_archiveOpenRecords,0,1000);memset(g_archiveRecordUsed,0,200);
    g_archiveRealCreateFileA=DetourFunction(DetourFindFunction("kernel32.dll","CreateFileA"),(BYTE*)Arch_CreateFileA);
    g_archiveRealReadFile=DetourFunction(DetourFindFunction("kernel32.dll","ReadFile"),(BYTE*)Arch_ReadFile);
    g_archiveRealGetFileSize=DetourFunction(DetourFindFunction("kernel32.dll","GetFileSize"),(BYTE*)Arch_GetFileSize);
    g_archiveRealSetFilePointer=DetourFunction(DetourFindFunction("kernel32.dll","SetFilePointer"),(BYTE*)Arch_SetFilePointer);
    g_archiveRealCloseHandle=DetourFunction(DetourFindFunction("kernel32.dll","CloseHandle"),(BYTE*)Arch_CloseHandle);
    g_archiveRealGetFileType=DetourFunction(DetourFindFunction("kernel32.dll","GetFileType"),(BYTE*)Arch_GetFileType);
    g_archiveHooksInstalled=TRUE;
}
// samp.dll RVA: 0x000623f0
void ArchiveInstallCursorHook() {
    g_archiveRealShowCursor=DetourFunction(DetourFindFunction("user32.dll","ShowCursor"),(BYTE*)Arch_ShowCursor);
}
// samp.dll RVA: 0x00062810
void ArchiveRemoveHooks() {
    if(!g_archiveHooksInstalled)return;
    DetourRemove(g_archiveRealCreateFileA,(BYTE*)Arch_CreateFileA);
    DetourRemove(g_archiveRealReadFile,(BYTE*)Arch_ReadFile);
    DetourRemove(g_archiveRealGetFileSize,(BYTE*)Arch_GetFileSize);
    DetourRemove(g_archiveRealSetFilePointer,(BYTE*)Arch_SetFilePointer);
    DetourRemove(g_archiveRealCloseHandle,(BYTE*)Arch_CloseHandle);
    DetourRemove(g_archiveRealGetFileType,(BYTE*)Arch_GetFileType);
    DetourRemove(g_archiveRealShowCursor,(BYTE*)Arch_ShowCursor);
    g_archiveHooksInstalled=FALSE;
}
// samp.dll RVA: 0x000623e0
int WINAPI Arch_ShowCursor(BOOL visible) { return visible?0:-1; }
