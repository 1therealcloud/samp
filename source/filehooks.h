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

extern BYTE* g_archiveRealShowCursor;

void ArchiveInstallFileHooks();
void ArchiveInstallCursorHook();

#endif // FILEHOOKS_H
