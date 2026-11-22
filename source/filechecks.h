#ifndef FILECHECKS_H
#define FILECHECKS_H

#include <windows.h>

// 0x00062b00
char* GetSHA1Result(unsigned int* digest, char* buffer);

// 0x0014033c
extern DWORD g_archiveTrackedNameDigests[3][5];

// 0x00062500
void ArchivePublishNameDigest(const DWORD* digest);

// 0x00062950
void ArchiveComputeFileDigest(const char* filename, DWORD* digest);

// 0x00062a60
void ArchiveComputeNameDigest(const void* data, DWORD length, DWORD* digest);

#endif // FILECHECKS_H
