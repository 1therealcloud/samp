#include "filechecks.h"
#include "main.h"
#include <string.h>
// 0x0014033c
DWORD g_archiveTrackedNameDigests[3][5] = {{0x11c714fd, 0x0a46694e, 0x7aec2920, 0x2100e2e4, 0x94f7372e},
                                           {0x0b85a2ad, 0x344bdae1, 0xa19471d5, 0x7d49d3e9, 0x7dde6cbe},
                                           {0x7c425b89, 0xf67763c0, 0xa2e49f0e, 0x93ebf8d2, 0x0e842901}};
// 0x000635c0
ArchiveNameSha1::ArchiveNameSha1()
{
    Reset();
}
// 0x00063550
ArchiveNameSha1::~ArchiveNameSha1() {}
// 0x00063560
void ArchiveNameSha1::Reset()
{
    lengthLow = lengthHigh = blockIndex = 0;
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;
    state[4] = 0xc3d2e1f0;
    computed = corrupted = false;
}
// 0x000635f0
void ArchiveNameSha1::ProcessBlock()
{
    DWORD words[80];
    for (int i = 0; i < 16; i++)
        words[i] = ((DWORD)block[i * 4] << 24) | ((DWORD)block[i * 4 + 1] << 16) | ((DWORD)block[i * 4 + 2] << 8) | block[i * 4 + 3];
    for (int j = 16; j < 80; j++)
    {
        DWORD value = words[j - 3] ^ words[j - 8] ^ words[j - 14] ^ words[j - 16];
        words[j] = (value << 1) | (value >> 31);
    }
    DWORD a = state[0], b = state[1], c = state[2], d = state[3], e = state[4];
    for (int round = 0; round < 80; round++)
    {
        DWORD f, k;
        if (round < 20)
        {
            f = (b & c) | (~b & d);
            k = 0x5a827999;
        }
        else if (round < 40)
        {
            f = b ^ c ^ d;
            k = 0x6ed9eba1;
        }
        else if (round < 60)
        {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8f1bbcdc;
        }
        else
        {
            f = b ^ c ^ d;
            k = 0xca62c1d6;
        }
        DWORD next = ((a << 5) | (a >> 27)) + f + e + words[round] + k;
        e = d;
        d = c;
        c = (b << 30) | (b >> 2);
        b = a;
        a = next;
    }
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    blockIndex = 0;
}
// 0x00063d30
void ArchiveNameSha1::Input(const BYTE* data, DWORD length)
{
    if (!length)
        return;
    if (computed || corrupted)
    {
        corrupted = true;
        return;
    }
    do
    {
        --length;
        if (corrupted)
            return;
        block[blockIndex++] = *data++;
        lengthLow += 8;
        if (!lengthLow)
        {
            ++lengthHigh;
            if (!lengthHigh)
                corrupted = true;
        }
        if (blockIndex == 64)
            ProcessBlock();
    }
    while (length);
}
// 0x00063c20
void ArchiveNameSha1::Pad()
{
    DWORD previous = blockIndex;
    block[blockIndex++] = 0x80;
    if (previous >= 56)
    {
        while (blockIndex < 64)
            block[blockIndex++] = 0;
        ProcessBlock();
    }
    while (blockIndex < 56)
        block[blockIndex++] = 0;
    block[56] = (BYTE)(lengthHigh >> 24);
    block[57] = (BYTE)(lengthHigh >> 16);
    block[58] = (BYTE)(lengthHigh >> 8);
    block[59] = (BYTE)lengthHigh;
    block[60] = (BYTE)(lengthLow >> 24);
    block[61] = (BYTE)(lengthLow >> 16);
    block[62] = (BYTE)(lengthLow >> 8);
    block[63] = (BYTE)lengthLow;
    ProcessBlock();
}
// 0x00063ce0
bool ArchiveNameSha1::Result(DWORD* digest)
{
    if (corrupted)
        return false;
    if (!computed)
    {
        Pad();
        computed = true;
    }
    for (int i = 0; i < 5; i++)
        digest[i] = state[i];
    return true;
}
// 0x00062a60
void ArchiveComputeNameDigest(const void* data, DWORD length, DWORD* digest)
{
    ArchiveNameSha1 sha;
    memset(digest, 0, 20);
    if (!IsBadReadPtr(data, length))
    {
        sha.Input((const BYTE*)data, length);
        sha.Result(digest);
    }
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

#include <stdio.h>
#include <stdlib.h>
// 0x00062950
void ArchiveComputeFileDigest(const char* filename, DWORD* digest)
{
    ArchiveNameSha1 sha;
    BYTE* buffer = (BYTE*)calloc(1, 0x8000);
    memset(digest, 0, 20);
    if (!buffer)
        return;
    FILE* file = fopen(filename, "rb");
    if (file)
    {
        while (!feof(file))
        {
            int count = (int)fread(buffer, 1, 0x8000, file);
            if (count > 0)
                sha.Input(buffer, count);
        }
        fclose(file);
        free(buffer);
        sha.Result(digest);
    }
}

// 0x00063da0
void ArchiveNameSha1::Input(const char* data, DWORD length)
{
    Input((const BYTE*)data, length);
}
// 0x00063db0
void ArchiveNameSha1::Input(BYTE value)
{
    Input(&value, 1);
}
// 0x00063dc0
void ArchiveNameSha1::Input(char value)
{
    Input((const BYTE*)&value, 1);
}
// 0x00063dd0
ArchiveNameSha1& ArchiveNameSha1::operator<<(const char* text)
{
    while (*text)
    {
        Input(*text);
        ++text;
    }
    return *this;
}
// 0x00063e00
ArchiveNameSha1& ArchiveNameSha1::operator<<(const BYTE* text)
{
    while (*text)
    {
        Input(*text);
        ++text;
    }
    return *this;
}
// 0x00063e30
ArchiveNameSha1& ArchiveNameSha1::operator<<(char value)
{
    Input((const BYTE*)&value, 1);
    return *this;
}
// 0x00063e50
ArchiveNameSha1& ArchiveNameSha1::operator<<(BYTE value)
{
    Input(&value, 1);
    return *this;
}
// 0x000635a0
DWORD ArchiveNameSha1::CircularShift(int bits, DWORD word)
{
    DWORD amount = (DWORD)bits & 31;
    return (word << amount) | (word >> ((0 - amount) & 31));
}
