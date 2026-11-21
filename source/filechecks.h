#ifndef FILECHECKS_H
#define FILECHECKS_H

#include <windows.h>

#include <static_assert.h>

// R5 address to verify
char* GetSHA1Result(unsigned int* digest, char* buffer);

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
    // 0x00063b40
    ArchiveNameSha1();
    // 0x00063ad0
    virtual ~ArchiveNameSha1();
    // 0x00063ae0
    void Reset();
    // 0x000642b0
    void Input(const BYTE* data, DWORD length);
    // 0x00064320
    void Input(const char* data, DWORD length);
    // R5 address to verify
    void Input(BYTE value);
    // R5 address to verify
    void Input(char value);
    // String forms stop before NUL; single-byte forms also consume zero.
    // R5 address to verify
    ArchiveNameSha1& operator<<(const char* text);
    // R5 address to verify
    ArchiveNameSha1& operator<<(const BYTE* text);
    // R5 address to verify
    ArchiveNameSha1& operator<<(char value);
    // R5 address to verify
    ArchiveNameSha1& operator<<(BYTE value);
    // Native SHL/SHR mask counts modulo32, including zero/negative inputs.
    // R5 address to verify
    DWORD CircularShift(int bits, DWORD word);
    // 0x00064260
    bool Result(DWORD* digest);
    // 0x000641a0
    void Pad();
    // 0x00063b70
    void ProcessBlock();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(ArchiveNameSha1) == 102);
// Three independent native digest comparisons: handling.cfg, weapon.dat,
// melee.dat. Publication increments CGame's observed DWORD at offset 0x55.
// 0x00102354
extern DWORD g_archiveTrackedNameDigests[3][5];
// 0x00062a80
void ArchivePublishNameDigest(const DWORD* digest);
// Leaves output zero on allocation/open failure; failed open leaks the32K buffer.
// 0x00062ed0
void ArchiveComputeFileDigest(const char* filename, DWORD* digest);
// 0x00062fe0
void ArchiveComputeNameDigest(const void* data, DWORD length, DWORD* digest);

#endif // FILECHECKS_H
