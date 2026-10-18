#ifndef SAMP_ARCHIVE_PROVIDER_H
#define SAMP_ARCHIVE_PROVIDER_H
#include <windows.h>
// Observed allocation extent and vtable pointer only. Archive methods, remaining
// layout and construction are still dependencies, not a recovered filesystem.
#pragma pack(push,1)
class CArchiveFS {
public:
    void **nativeVtable;
    BYTE opaque[0x931];
    // samp.dll RVA: 0x00064dd0
    CArchiveFS();
};
#pragma pack(pop)
typedef char ArchiveProviderExtent[(sizeof(CArchiveFS)==0x935)?1:-1];
// samp.dll RVA: 0x002ac9d4
extern CArchiveFS *g_pArchive;
#endif
