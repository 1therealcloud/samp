#ifndef FILECHECKS_H
#define FILECHECKS_H

#include <windows.h>

char* GetSHA1Result(unsigned int* digest, char* buffer);

extern DWORD g_archiveTrackedNameDigests[3][5];

void ArchivePublishNameDigest(const DWORD* digest);

void ArchiveComputeFileDigest(const char* filename, DWORD* digest);

void ArchiveComputeNameDigest(const void* data, DWORD length, DWORD* digest);

#endif // FILECHECKS_H
