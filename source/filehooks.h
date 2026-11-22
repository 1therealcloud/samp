#ifndef FILEHOOKS_H
#define FILEHOOKS_H

#include <windows.h>

#include <static_assert.h>

#pragma pack(push, 1)
struct ArchiveFileRecord
{
    HANDLE handle;
    DWORD position, size;
    BYTE *start, *current;
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(ArchiveFileRecord) == 20);
// 0x0012df48
extern ArchiveFileRecord g_archiveOpenRecords[50];
// 0x0012de80
extern BOOL g_archiveRecordUsed[50];
// 0x0012e360
extern BOOL g_archiveHooksInstalled;
// 0x0012e334
extern BYTE* g_archiveRealGetFileSize;
// 0x0012e338
extern BYTE* g_archiveRealSetFilePointer;
// 0x0012e33c
extern BYTE* g_archiveRealCreateFileA;
// 0x0012e344
extern BYTE* g_archiveRealReadFile;
// 0x0012e348
extern BYTE* g_archiveRealCloseHandle;
// 0x0012e34c
extern BYTE* g_archiveRealGetFileType;
// R5 address to verify
extern BYTE* g_archiveRealShowCursor;
// 0x00062ca0
void ArchiveInstallFileHooks();
// R5 address to verify
void ArchiveInstallCursorHook();
// 0x00062d90
void ArchiveRemoveHooks();
// R5 address to verify
int WINAPI Arch_ShowCursor(BOOL visible);
// 0x00102350
extern DWORD g_archiveNextHandle;
// 0x00062580
BOOL ArchiveIsCustomHandle(DWORD handle);
// 0x000625a0
int ArchiveFindUnusedRecord();
// 0x000625c0
int ArchiveFindRecord(HANDLE handle);
// 0x000625f0
HANDLE ArchiveCreateRecord(DWORD index);
// This native helper removes ONE trailing space in place, despite the const
// filename type in CreateFileA. Leading separator at byte zero is not skipped.
// 0x00062990
char* ArchiveFileNameOnly(char* path);
// 0x00062a50
char* ArchiveLowercaseName(char* name);
// Additional observed callback; native installer62720 does not install this entry.
// R5 address to verify
extern BYTE* g_archiveRealCreateFileAValidation;
// Three-byte prefix check only: no terminator requirement.
// R5 address to verify
BOOL ArchiveIsIdeOrIplExtension(const char* extension);
// Signed byte accumulation and arithmetic shifts, not unsigned Jenkins hashing.
// R5 address to verify
DWORD ArchiveHashLowercaseName(const char* name);
// Removes one trailing space, then finds the last non-leading dot.
// R5 address to verify
char* ArchiveFileExtension(char* path);
// Observed digest comparison has no enforcement or published result.
// R5 address to verify
void ArchiveCheckFileDigest(DWORD hash, HANDLE file);
// R5 address to verify
HANDLE WINAPI Arch_CreateFileAWithValidation(LPCSTR name, DWORD access, DWORD share, LPSECURITY_ATTRIBUTES security, DWORD disposition, DWORD flags, HANDLE templateFile);
// 0x00062b50
HANDLE WINAPI Arch_CreateFileA(LPCSTR name, DWORD access, DWORD share, LPSECURITY_ATTRIBUTES security, DWORD disposition, DWORD flags, HANDLE templateFile);
// 0x00062670
BOOL WINAPI Arch_ReadFile(HANDLE file, LPVOID buffer, DWORD requested, LPDWORD read, LPOVERLAPPED overlapped);
// 0x000627f0
DWORD WINAPI Arch_GetFileSize(HANDLE file, PDWORD high);
// 0x00062840
DWORD WINAPI Arch_SetFilePointer(HANDLE file, LONG distance, PLONG high, DWORD method);
// 0x000628e0
BOOL WINAPI Arch_CloseHandle(HANDLE file);
// 0x00062920
DWORD WINAPI Arch_GetFileType(HANDLE file);

#endif // FILEHOOKS_H
