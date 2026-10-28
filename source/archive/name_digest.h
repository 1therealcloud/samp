#ifndef SAMP_ARCHIVE_NAME_DIGEST_H
#define SAMP_ARCHIVE_NAME_DIGEST_H
#include <windows.h>
#pragma pack(push, 1)
// Compiled-reference match: Paul E. Jones SHA-1 family; exact release unknown.
// Distinct from RakNet's CSHA1. Native output consists of five host-order
// DWORDs; this is not a 20-byte big-endian SHA-1 serialization.
class ArchiveNameSha1
{
public:
    DWORD state[5], lengthLow, lengthHigh;
    BYTE block[64];
    DWORD blockIndex;
    bool computed, corrupted;
    // samp.dll RVA: 0x000635c0
    ArchiveNameSha1();
    // samp.dll RVA: 0x00063550
    virtual ~ArchiveNameSha1();
    // samp.dll RVA: 0x00063560
    void Reset();
    // samp.dll RVA: 0x00063d30
    void Input(const BYTE* data, DWORD length);
    // samp.dll RVA: 0x00063da0
    void Input(const char* data, DWORD length);
    // samp.dll RVA: 0x00063db0
    void Input(BYTE value);
    // samp.dll RVA: 0x00063dc0
    void Input(char value);
    // String forms stop before NUL; single-byte forms also consume zero.
    // samp.dll RVA: 0x00063dd0
    ArchiveNameSha1& operator<<(const char* text);
    // samp.dll RVA: 0x00063e00
    ArchiveNameSha1& operator<<(const BYTE* text);
    // samp.dll RVA: 0x00063e30
    ArchiveNameSha1& operator<<(char value);
    // samp.dll RVA: 0x00063e50
    ArchiveNameSha1& operator<<(BYTE value);
    // Native SHL/SHR mask counts modulo32, including zero/negative inputs.
    // samp.dll RVA: 0x000635a0
    DWORD CircularShift(int bits, DWORD word);
    // samp.dll RVA: 0x00063ce0
    bool Result(DWORD* digest);
    // samp.dll RVA: 0x00063c20
    void Pad();
    // samp.dll RVA: 0x000635f0
    void ProcessBlock();
};
#pragma pack(pop)
typedef char ArchiveNameSha1Extent[(sizeof(ArchiveNameSha1) == 102) ? 1 : -1];
// Three independent native digest comparisons: handling.cfg, weapon.dat,
// melee.dat. Publication increments CGame's observed DWORD at offset 0x55.
// samp.dll RVA: 0x0014033c
extern DWORD g_archiveTrackedNameDigests[3][5];
// samp.dll RVA: 0x00062500
void ArchivePublishNameDigest(const DWORD* digest);
// Leaves output zero on allocation/open failure; failed open leaks the32K buffer.
// samp.dll RVA: 0x00062950
void ArchiveComputeFileDigest(const char* filename, DWORD* digest);
// samp.dll RVA: 0x00062a60
void ArchiveComputeNameDigest(const void* data, DWORD length, DWORD* digest);
#endif
