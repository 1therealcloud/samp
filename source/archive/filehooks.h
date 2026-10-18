#ifndef SAMP_ARCHIVE_FILEHOOKS_H
#define SAMP_ARCHIVE_FILEHOOKS_H
#include <windows.h>
// Record layout is not yet recovered; installer clears exactly 1000 bytes.
// samp.dll RVA: 0x0016bf60
extern BYTE g_archiveOpenRecords[1000];
// samp.dll RVA: 0x0016be98
extern BOOL g_archiveRecordUsed[50];
// samp.dll RVA: 0x0016c378
extern BOOL g_archiveHooksInstalled;
// samp.dll RVA: 0x0016c34c
extern BYTE *g_archiveRealGetFileSize;
// samp.dll RVA: 0x0016c350
extern BYTE *g_archiveRealSetFilePointer;
// samp.dll RVA: 0x0016c354
extern BYTE *g_archiveRealCreateFileA;
// samp.dll RVA: 0x0016c35c
extern BYTE *g_archiveRealReadFile;
// samp.dll RVA: 0x0016c360
extern BYTE *g_archiveRealCloseHandle;
// samp.dll RVA: 0x0016c364
extern BYTE *g_archiveRealGetFileType;
// samp.dll RVA: 0x0016c368
extern BYTE *g_archiveRealShowCursor;
// samp.dll RVA: 0x00062720
void ArchiveInstallFileHooks();
// samp.dll RVA: 0x000623f0
void ArchiveInstallCursorHook();
// samp.dll RVA: 0x00062810
void ArchiveRemoveHooks();
// samp.dll RVA: 0x000623e0
int WINAPI Arch_ShowCursor(BOOL visible);
// Address-only callback declarations used by installers. Their full argument
// ABIs must be reviewed before implementing or calling them through these types.
// samp.dll RVA: 0x000625d0
void Arch_CreateFileA();
// samp.dll RVA: 0x000620f0
void Arch_ReadFile();
// samp.dll RVA: 0x00062270
void Arch_GetFileSize();
// samp.dll RVA: 0x000622c0
void Arch_SetFilePointer();
// samp.dll RVA: 0x00062360
void Arch_CloseHandle();
// samp.dll RVA: 0x000623a0
void Arch_GetFileType();
#endif
