#include "filehooks.h"

#include "archive/ArchiveFS.h"
#include "filechecks.h"
#include "md5.h"

#include <detours.h>

#include <ctype.h>
#include <string.h>
// 0x0012df48
ArchiveFileRecord g_archiveOpenRecords[50];
// 0x0012de80
BOOL g_archiveRecordUsed[50];
// 0x0012e360
BOOL g_archiveHooksInstalled;
// 0x0012e334
BYTE* g_archiveRealGetFileSize;
// 0x0012e338
BYTE* g_archiveRealSetFilePointer;
// 0x0012e33c
BYTE* g_archiveRealCreateFileA;
// 0x0012e344
BYTE* g_archiveRealReadFile;
// 0x0012e348
BYTE* g_archiveRealCloseHandle;
// 0x0012e34c
BYTE* g_archiveRealGetFileType;
// R5 address to verify
BYTE* g_archiveRealShowCursor;
// 0x00102350
DWORD g_archiveNextHandle = 0xff000001;
// R5 address to verify
BYTE* g_archiveRealCreateFileAValidation;

// 0x00062ca0
void ArchiveInstallFileHooks()
{
    if (g_archiveHooksInstalled)
        return;
    memset(g_archiveOpenRecords, 0, 1000);
    memset(g_archiveRecordUsed, 0, 200);
    g_archiveRealCreateFileA = DetourFunction(DetourFindFunction("kernel32.dll", "CreateFileA"), (BYTE*)Arch_CreateFileA);
    g_archiveRealReadFile = DetourFunction(DetourFindFunction("kernel32.dll", "ReadFile"), (BYTE*)Arch_ReadFile);
    g_archiveRealGetFileSize = DetourFunction(DetourFindFunction("kernel32.dll", "GetFileSize"), (BYTE*)Arch_GetFileSize);
    g_archiveRealSetFilePointer = DetourFunction(DetourFindFunction("kernel32.dll", "SetFilePointer"), (BYTE*)Arch_SetFilePointer);
    g_archiveRealCloseHandle = DetourFunction(DetourFindFunction("kernel32.dll", "CloseHandle"), (BYTE*)Arch_CloseHandle);
    g_archiveRealGetFileType = DetourFunction(DetourFindFunction("kernel32.dll", "GetFileType"), (BYTE*)Arch_GetFileType);
    g_archiveHooksInstalled = TRUE;
}

// R5 address to verify
void ArchiveInstallCursorHook()
{
    g_archiveRealShowCursor = DetourFunction(DetourFindFunction("user32.dll", "ShowCursor"), (BYTE*)Arch_ShowCursor);
}

// 0x00062d90
void ArchiveRemoveHooks()
{
    if (!g_archiveHooksInstalled)
        return;
    DetourRemove(g_archiveRealCreateFileA, (BYTE*)Arch_CreateFileA);
    DetourRemove(g_archiveRealReadFile, (BYTE*)Arch_ReadFile);
    DetourRemove(g_archiveRealGetFileSize, (BYTE*)Arch_GetFileSize);
    DetourRemove(g_archiveRealSetFilePointer, (BYTE*)Arch_SetFilePointer);
    DetourRemove(g_archiveRealCloseHandle, (BYTE*)Arch_CloseHandle);
    DetourRemove(g_archiveRealGetFileType, (BYTE*)Arch_GetFileType);
    DetourRemove(g_archiveRealShowCursor, (BYTE*)Arch_ShowCursor);
    g_archiveHooksInstalled = FALSE;
}

// R5 address to verify
int WINAPI Arch_ShowCursor(BOOL visible)
{
    return visible ? 0 : -1;
}

// 0x00062580
BOOL ArchiveIsCustomHandle(DWORD handle)
{
    return handle >= 0xff000001 && handle < 0xff000101;
}

// 0x000625a0
int ArchiveFindUnusedRecord()
{
    for (int i = 0; i != 50; i++)
        if (g_archiveRecordUsed[i] == FALSE)
            return i;
    return -1;
}

// 0x000625c0
int ArchiveFindRecord(HANDLE handle)
{
    for (int i = 0; i != 50; i++)
        if (g_archiveRecordUsed[i] == TRUE && g_archiveOpenRecords[i].handle == handle)
            return i;
    return -1;
}

// 0x000625f0
HANDLE ArchiveCreateRecord(DWORD index)
{
    int slot = ArchiveFindUnusedRecord();
    if (slot == -1)
        return 0;
    g_archiveRecordUsed[slot] = TRUE;
    ArchiveFileRecord& record = g_archiveOpenRecords[slot];
    record.handle = (HANDLE)g_archiveNextHandle;
    g_archiveNextHandle += 2;
    record.position = 0;
    record.size = g_pArchive->GetFileSize(index);
    record.start = g_pArchive->GetFileData(index);
    record.current = record.start;
    return record.handle;
}

// 0x00062990
char* ArchiveFileNameOnly(char* path)
{
    if (path[strlen(path) - 1] == ' ')
        path[strlen(path) - 1] = 0;
    char* search = path + strlen(path);
    while (search != path)
    {
        if (*search == '/' || *search == '\\')
            return search + 1;
        --search;
    }
    return path;
}

// 0x00062a50
char* ArchiveLowercaseName(char* name)
{
    char* cursor = name;
    while (*cursor)
    {
        *cursor = (char)tolower(*cursor);
        ++cursor;
    }
    return name;
}

// 0x00062b50
HANDLE WINAPI Arch_CreateFileA(LPCSTR name, DWORD access, DWORD share, LPSECURITY_ATTRIBUTES security, DWORD disposition, DWORD flags, HANDLE templateFile)
{
    DWORD index = g_pArchive->GetFileIndex(ArchiveFileNameOnly((char*)name));
    if (index != 0xffffffff)
    {
        HANDLE handle = ArchiveCreateRecord(index);
        char fileName[MAX_PATH];
        DWORD digest[5];
        strcpy(fileName, ArchiveFileNameOnly((char*)name));
        ArchiveLowercaseName(fileName);
        ArchiveComputeNameDigest(fileName, strlen(fileName), digest);
        ArchivePublishNameDigest(digest);
        return handle;
    }
    typedef HANDLE(WINAPI * Original)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
    return ((Original)g_archiveRealCreateFileA)(name, access, share, security, disposition, flags, templateFile);
}

// 0x00062670
BOOL WINAPI Arch_ReadFile(HANDLE file, LPVOID buffer, DWORD requested, LPDWORD read, LPOVERLAPPED overlapped)
{
    int slot;
    if (ArchiveIsCustomHandle((DWORD)file) && (slot = ArchiveFindRecord(file)) != -1)
    {
        ArchiveFileRecord& record = g_archiveOpenRecords[slot];
        DWORD size = record.size;
        HANDLE event = 0;
        if (overlapped)
        {
            record.position = overlapped->Offset;
            record.current = (BYTE*)((DWORD)record.start + overlapped->Offset);
            event = overlapped->hEvent;
        }
        if (record.position >= size)
        {
            if (read)
                *read = 0;
            if (event)
                SetEvent(event);
            SetLastError(ERROR_HANDLE_EOF);
            return TRUE;
        }
        DWORD end = record.position + requested, count = requested;
        bool eof = end >= size;
        if (eof)
            count = requested - (end - size);
        memcpy(buffer, record.current, count);
        record.current += count;
        record.position += count;
        if (read)
            *read = count;
        if (event)
            SetEvent(event);
        if (eof)
            SetLastError(ERROR_HANDLE_EOF);
        return TRUE;
    }
    typedef BOOL(WINAPI * Original)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
    return ((Original)g_archiveRealReadFile)(file, buffer, requested, read, overlapped);
}

// 0x000627f0
DWORD WINAPI Arch_GetFileSize(HANDLE file, PDWORD high)
{
    int slot;
    if (ArchiveIsCustomHandle((DWORD)file) && (slot = ArchiveFindRecord(file)) != -1)
    {
        if (high)
            *high = 0;
        return g_archiveOpenRecords[slot].size;
    }
    typedef DWORD(WINAPI * Original)(HANDLE, PDWORD);
    return ((Original)g_archiveRealGetFileSize)(file, high);
}

// 0x00062840
DWORD WINAPI Arch_SetFilePointer(HANDLE file, LONG distance, PLONG high, DWORD method)
{
    int slot;
    if (ArchiveIsCustomHandle((DWORD)file) && (slot = ArchiveFindRecord(file)) != -1)
    {
        ArchiveFileRecord& record = g_archiveOpenRecords[slot];
        if (method == FILE_BEGIN)
        {
            record.position = distance;
            record.current = (BYTE*)((DWORD)record.start + (DWORD)distance);
        }
        else if (method == FILE_CURRENT)
        {
            record.position += distance;
            record.current = (BYTE*)((DWORD)record.current + (DWORD)distance);
        }
        else if (method == FILE_END)
        {
            record.position = record.size;
            record.current = (BYTE*)((DWORD)record.start + record.size);
        }
        return record.position;
    }
    typedef DWORD(WINAPI * Original)(HANDLE, LONG, PLONG, DWORD);
    return ((Original)g_archiveRealSetFilePointer)(file, distance, high, method);
}

// 0x000628e0
BOOL WINAPI Arch_CloseHandle(HANDLE file)
{
    int slot;
    if (ArchiveIsCustomHandle((DWORD)file) && (slot = ArchiveFindRecord(file)) != -1)
    {
        g_archiveRecordUsed[slot] = FALSE;
        return TRUE;
    }
    typedef BOOL(WINAPI * Original)(HANDLE);
    return ((Original)g_archiveRealCloseHandle)(file);
}

// 0x00062920
DWORD WINAPI Arch_GetFileType(HANDLE file)
{
    int slot;
    if (ArchiveIsCustomHandle((DWORD)file) && (slot = ArchiveFindRecord(file)) != -1)
        return FILE_TYPE_DISK;
    typedef DWORD(WINAPI * Original)(HANDLE);
    return ((Original)g_archiveRealGetFileType)(file);
}

// R5 address to verify
BOOL ArchiveIsIdeOrIplExtension(const char* extension)
{
    if (((BYTE)extension[0] & 0xdf) != 'I')
        return FALSE;
    if (((BYTE)extension[1] & 0xdf) == 'D')
        return ((BYTE)extension[2] & 0xdf) == 'E';
    if (((BYTE)extension[1] & 0xdf) == 'P')
        return ((BYTE)extension[2] & 0xdf) == 'L';
    return FALSE;
}

// R5 address to verify
DWORD ArchiveHashLowercaseName(const char* name)
{
    DWORD hash = 0;
    const char* end = name + strlen(name);
    while (name < end)
    {
        int value = *name;
        if (value >= 'A' && value <= 'Z')
            value |= 0x20;
        hash += (DWORD)value;
        hash *= 0x401;
        hash ^= (DWORD)((LONG)hash >> 6);
        ++name;
    }
    hash *= 9;
    hash ^= (DWORD)((LONG)hash >> 11);
    return hash * 0x8001;
}

// R5 address to verify
char* ArchiveFileExtension(char* path)
{
    if (path[strlen(path) - 1] == ' ')
        path[strlen(path) - 1] = 0;
    char* search = path + strlen(path);
    while (search != path)
    {
        if (*search == '.')
            return search + 1;
        --search;
    }
    return path;
}

// R5 address to verify
void ArchiveCheckFileDigest(DWORD hash, HANDLE file)
{
    DWORD index = g_pArchive->GetFileIndex((DWORD)0xa848b69a);
    if (index == 0xffffffff)
        return;
    BYTE* data = g_pArchive->GetFileData(index);
    BYTE count = *data++;
    BYTE node = 0;
    while (node < count)
    {
        BYTE* record = data + node * 8;
        LONG key = *(LONG*)record;
        if (key == (LONG)hash)
        {
            BYTE digestIndex = record[6];
            if (digestIndex >= count)
                return;
            // Retain the native comparison reads even though no action follows.
            volatile BYTE* expected = data + 8 * ((DWORD)count + (DWORD)digestIndex * 2);
            MD5Context context;
            MD5Init(&context);
            BYTE buffer[1024], digest[16];
            DWORD read;
            while (Arch_ReadFile(file, buffer, 1024, &read, 0) && read)
                MD5Update(&context, buffer, read);
            MD5Final(digest, &context);
            LONG high = 0;
            Arch_SetFilePointer(file, 0, &high, FILE_BEGIN);
            for (BYTE i = 0; i < 16; ++i)
                if (digest[i] != expected[i])
                    break;
            return;
        }
        node = record[key < (LONG)hash ? 5 : 4];
    }
}

// R5 address to verify
HANDLE WINAPI Arch_CreateFileAWithValidation(LPCSTR name, DWORD access, DWORD share, LPSECURITY_ATTRIBUTES security, DWORD disposition, DWORD flags, HANDLE templateFile)
{
    typedef HANDLE(WINAPI * Original)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
    HANDLE file = ((Original)g_archiveRealCreateFileAValidation)(name, access, share, security, disposition, flags, templateFile);
    if (ArchiveIsIdeOrIplExtension(ArchiveFileExtension((char*)name)))
        ArchiveCheckFileDigest(ArchiveHashLowercaseName(ArchiveLowercaseName(ArchiveFileNameOnly((char*)name))), file);
    return file;
}

// R5 address to verify
void __fastcall NativeIncrementDword0055(void* object)
{
    ++*(DWORD*)((BYTE*)object + 0x55);
}
