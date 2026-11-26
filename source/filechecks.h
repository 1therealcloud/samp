#ifndef FILECHECKS_H
#define FILECHECKS_H

void ArchivePublishNameDigest(const DWORD* digest);
void ArchiveComputeNameDigest(const void* data, DWORD length, DWORD* digest);

#endif // FILECHECKS_H
