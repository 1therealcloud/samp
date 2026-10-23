#ifndef SAMP_CLIENT_NETSTATS_H
#define SAMP_CLIENT_NETSTATS_H
#include <windows.h>
#include <stddef.h>
struct IDirect3DDevice9;
class CNetStats {
public:
    DWORD lastTotalSent,lastTotalReceived,lastUpdateTick,uploadRate,downloadRate;
    IDirect3DDevice9 *device;
    // samp.dll RVA: 0x000607c0
    CNetStats(IDirect3DDevice9 *device);
    // samp.dll RVA: 0x000607f0
    void Draw();
};
typedef char NetStatsExtent[(sizeof(CNetStats)==24)?1:-1];
typedef char NetStatsTickOffset[(offsetof(CNetStats,lastUpdateTick)==8)?1:-1];
// Native buffers have unsafe writes/concatenations; overflow adjacency is unproven.
// samp.dll RVA: 0x00163a78
extern char g_netStatsLine[1024];
// samp.dll RVA: 0x00163e78
extern char g_netStatsScratch[16384];
// samp.dll RVA: 0x00167e78
extern char g_netStatsDisplay[16384];
// samp.dll RVA: 0x00156ab0
extern DWORD g_netStatsShotCompMode;
// samp.dll RVA: 0x0011b8e4
extern const char g_netStatsRatesFormat[49];
// samp.dll RVA: 0x0011b858
extern const char g_netStatsModelsFormat[140];
// samp.dll RVA: 0x0011b83c
extern const char g_netStatsOnFootFormat[22];
// samp.dll RVA: 0x0011b824
extern const char g_netStatsInCarFormat[21];
// samp.dll RVA: 0x0011b7f4
extern const char g_netStatsMemoryFormat[46];
// samp.dll RVA: 0x0011b7cc
extern const char g_netStatsProcessFormat[37];
// samp.dll RVA: 0x0011b7b4
extern const char g_netStatsTitle[21];
#endif
