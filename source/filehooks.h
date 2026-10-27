#ifndef SAMP_ARCHIVE_FILEHOOKS_H
#define SAMP_ARCHIVE_FILEHOOKS_H
#include <windows.h>
#pragma pack(push,1)
struct ArchiveFileRecord {
    HANDLE handle;
    DWORD position,size;
    BYTE *start,*current;
};
#pragma pack(pop)
typedef char ArchiveFileRecordExtent[(sizeof(ArchiveFileRecord)==20)?1:-1];
// samp.dll RVA: 0x0016bf60
extern ArchiveFileRecord g_archiveOpenRecords[50];
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
// samp.dll RVA: 0x00140338
extern DWORD g_archiveNextHandle;
// samp.dll RVA: 0x00062000
BOOL ArchiveIsCustomHandle(DWORD handle);
// samp.dll RVA: 0x00062020
int ArchiveFindUnusedRecord();
// samp.dll RVA: 0x00062040
int ArchiveFindRecord(HANDLE handle);
// samp.dll RVA: 0x00062070
HANDLE ArchiveCreateRecord(DWORD index);
// This native helper removes ONE trailing space in place, despite the const
// filename type in CreateFileA. Leading separator at byte zero is not skipped.
// samp.dll RVA: 0x00062410
char *ArchiveFileNameOnly(char *path);
// samp.dll RVA: 0x000624d0
char *ArchiveLowercaseName(char *name);
// Additional observed callback; native installer62720 does not install this entry.
// samp.dll RVA: 0x0016c358
extern BYTE *g_archiveRealCreateFileAValidation;
// Three-byte prefix check only: no terminator requirement.
// samp.dll RVA: 0x00061df0
BOOL ArchiveIsIdeOrIplExtension(const char *extension);
// Signed byte accumulation and arithmetic shifts, not unsigned Jenkins hashing.
// samp.dll RVA: 0x00061f90
DWORD ArchiveHashLowercaseName(const char *name);
// Removes one trailing space, then finds the last non-leading dot.
// samp.dll RVA: 0x00062470
char *ArchiveFileExtension(char *path);
// Observed digest comparison has no enforcement or published result.
// samp.dll RVA: 0x00061e50
void ArchiveCheckFileDigest(DWORD hash,HANDLE file);
// samp.dll RVA: 0x000626b0
HANDLE WINAPI Arch_CreateFileAWithValidation(LPCSTR name,DWORD access,DWORD share,LPSECURITY_ATTRIBUTES security,DWORD disposition,DWORD flags,HANDLE templateFile);
// samp.dll RVA: 0x000625d0
HANDLE WINAPI Arch_CreateFileA(LPCSTR name,DWORD access,DWORD share,LPSECURITY_ATTRIBUTES security,DWORD disposition,DWORD flags,HANDLE templateFile);
// samp.dll RVA: 0x000620f0
BOOL WINAPI Arch_ReadFile(HANDLE file,LPVOID buffer,DWORD requested,LPDWORD read,LPOVERLAPPED overlapped);
// samp.dll RVA: 0x00062270
DWORD WINAPI Arch_GetFileSize(HANDLE file,PDWORD high);
// samp.dll RVA: 0x000622c0
DWORD WINAPI Arch_SetFilePointer(HANDLE file,LONG distance,PLONG high,DWORD method);
// samp.dll RVA: 0x00062360
BOOL WINAPI Arch_CloseHandle(HANDLE file);
// samp.dll RVA: 0x000623a0
DWORD WINAPI Arch_GetFileType(HANDLE file);
#endif
