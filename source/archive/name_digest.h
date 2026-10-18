#ifndef SAMP_ARCHIVE_NAME_DIGEST_H
#define SAMP_ARCHIVE_NAME_DIGEST_H
#include <windows.h>
#pragma pack(push,1)
// Distinct from RakNet's CSHA1. Native output consists of five host-order
// DWORDs; this is not a 20-byte big-endian SHA-1 serialization.
class ArchiveNameSha1 {
public:
    DWORD state[5],lengthLow,lengthHigh;
    BYTE block[64];
    DWORD blockIndex;
    bool computed,corrupted;
    // samp.dll RVA: 0x000635c0
    ArchiveNameSha1();
    // samp.dll RVA: 0x00063550
    virtual ~ArchiveNameSha1();
    // samp.dll RVA: 0x00063560
    void Reset();
    // Native thunk RVA 0x00063da0 forwards to this body without other work.
    // samp.dll RVA: 0x00063d30
    void Input(const BYTE *data,DWORD length);
    // samp.dll RVA: 0x00063ce0
    bool Result(DWORD *digest);
    // samp.dll RVA: 0x00063c20
    void Pad();
    // samp.dll RVA: 0x000635f0
    void ProcessBlock();
};
#pragma pack(pop)
typedef char ArchiveNameSha1Extent[(sizeof(ArchiveNameSha1)==102)?1:-1];
// Three independent native digest comparisons: handling.cfg, weapon.dat,
// melee.dat. Publication increments CGame's observed DWORD at offset 0x55.
// samp.dll RVA: 0x0014033c
extern DWORD g_archiveTrackedNameDigests[3][5];
// samp.dll RVA: 0x00062500
void ArchivePublishNameDigest(const DWORD *digest);
// samp.dll RVA: 0x00062a60
void ArchiveComputeNameDigest(const void *data,DWORD length,DWORD *digest);
#endif
