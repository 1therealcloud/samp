#ifndef SAMP_SERVER_NETSTATS_H
#define SAMP_SERVER_NETSTATS_H
#include <windows.h>
struct IDirect3DDevice9;
class CSrvNetStats
{
public:
    // Observed allocation extent, not a complete recovered interface.
    BYTE nativeState[0x18];
    // samp.dll RVA: 0x00070cc0
    CSrvNetStats(IDirect3DDevice9* device);
    // samp.dll RVA: 0x00070d00
    void Draw();
};
// samp.dll RVA: 0x0016c7f8
extern DWORD g_srvNetStatsHistory[74];
#endif
