#ifndef FILEHOOKS_H
#define FILEHOOKS_H

#include <windows.h>

struct ArchiveFileRecord
{
    HANDLE handle;
    DWORD position, size;
    BYTE *start, *current;
};
static_assert(sizeof(ArchiveFileRecord) == 20);

extern ArchiveFileRecord g_archiveOpenRecords[50];

extern BOOL g_archiveRecordUsed[50];

extern BOOL g_archiveHooksInstalled;

extern BYTE* g_archiveRealGetFileSize;

extern BYTE* g_archiveRealSetFilePointer;

extern BYTE* g_archiveRealCreateFileA;

extern BYTE* g_archiveRealReadFile;

extern BYTE* g_archiveRealCloseHandle;

extern BYTE* g_archiveRealGetFileType;

extern BYTE* g_archiveRealShowCursor;

void ArchiveInstallFileHooks();

void ArchiveInstallCursorHook();

//void ArchiveRemoveHooks();

int WINAPI Arch_ShowCursor(BOOL visible);

extern DWORD g_archiveNextHandle;

BOOL ArchiveIsCustomHandle(DWORD handle);

int ArchiveFindUnusedRecord();

int ArchiveFindRecord(HANDLE handle);

HANDLE ArchiveCreateRecord(DWORD index);
// This native helper removes ONE trailing space in place, despite the const
// filename type in CreateFileA. Leading separator at byte zero is not skipped.

char* ArchiveFileNameOnly(char* path);

char* ArchiveLowercaseName(char* name);
// Additional observed callback; native installer62720 does not install this entry.

extern BYTE* g_archiveRealCreateFileAValidation;
// Three-byte prefix check only: no terminator requirement.

BOOL ArchiveIsIdeOrIplExtension(const char* extension);
// Signed byte accumulation and arithmetic shifts, not unsigned Jenkins hashing.

DWORD ArchiveHashLowercaseName(const char* name);
// Removes one trailing space, then finds the last non-leading dot.

char* ArchiveFileExtension(char* path);
// Observed digest comparison has no enforcement or published result.

void ArchiveCheckFileDigest(DWORD hash, HANDLE file);

HANDLE WINAPI Arch_CreateFileAWithValidation(LPCSTR name, DWORD access, DWORD share, LPSECURITY_ATTRIBUTES security, DWORD disposition, DWORD flags, HANDLE templateFile);

HANDLE WINAPI Arch_CreateFileA(LPCSTR name, DWORD access, DWORD share, LPSECURITY_ATTRIBUTES security, DWORD disposition, DWORD flags, HANDLE templateFile);

BOOL WINAPI Arch_ReadFile(HANDLE file, LPVOID buffer, DWORD requested, LPDWORD read, LPOVERLAPPED overlapped);

DWORD WINAPI Arch_GetFileSize(HANDLE file, PDWORD high);

DWORD WINAPI Arch_SetFilePointer(HANDLE file, LONG distance, PLONG high, DWORD method);

BOOL WINAPI Arch_CloseHandle(HANDLE file);

DWORD WINAPI Arch_GetFileType(HANDLE file);

#endif // FILEHOOKS_H
