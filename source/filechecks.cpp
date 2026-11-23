#include "filechecks.h"

#include "SHA1.h"
#include "main.h"

#include <stdio.h>
#include <string.h>

// 0x0014033c
DWORD g_archiveTrackedNameDigests[3][5] = {{0x11c714fd, 0x0a46694e, 0x7aec2920, 0x2100e2e4, 0x94f7372e},
                                           {0x0b85a2ad, 0x344bdae1, 0xa19471d5, 0x7d49d3e9, 0x7dde6cbe},
                                           {0x7c425b89, 0xf67763c0, 0xa2e49f0e, 0x93ebf8d2, 0x0e842901}};

// 0x00062b00
char* GetSHA1Result(unsigned int* digest, char* buffer)
{
    snprintf(buffer, 41, "%08X%08X%08X%08X%08X", digest[4], digest[2], digest[3], digest[1], digest[0]);
    return buffer;
}

static void ArchiveFinishSha1(CSHA1& sha, DWORD* digest)
{
    BYTE hash[SHA1_LENGTH];
    sha.Final();
    sha.GetHash(hash);

    for (int i = 0; i < 5; ++i)
    {
        const int offset = i * 4;
        digest[i] = ((DWORD)hash[offset] << 24) | ((DWORD)hash[offset + 1] << 16) | ((DWORD)hash[offset + 2] << 8) | (DWORD)hash[offset + 3];
    }
}

// 0x00062a60
void ArchiveComputeNameDigest(const void* data, DWORD length, DWORD* digest)
{
    memset(digest, 0, 20);
    if (IsBadReadPtr(data, length))
        return;

    CSHA1 sha;
    sha.Update((BYTE*)data, length);
    ArchiveFinishSha1(sha, digest);
}

// 0x00062500
void ArchivePublishNameDigest(const DWORD* digest)
{
    for (int entry = 0; entry < 3; entry++)
    {
        int word = 0;
        while (word < 5 && digest[word] == g_archiveTrackedNameDigests[entry][word])
            ++word;
        if (word == 5 && g_pGame)
            ++g_pGame->m_nTrackedNameDigestMatches;
    }
}

// 0x00062950
void ArchiveComputeFileDigest(const char* filename, DWORD* digest)
{
    memset(digest, 0, 20);

    FILE* file = nullptr;
    fopen_s(&file, filename, "rb");
    if (!file)
        return;

    CSHA1 sha;
    BYTE buffer[0x8000];
    size_t count;

    while ((count = fread(buffer, 1, sizeof(buffer), file)) > 0)
        sha.Update(buffer, (unsigned int)count);

    const bool failed = ferror(file) != 0;
    fclose(file);

    if (!failed)
        ArchiveFinishSha1(sha, digest);
}
