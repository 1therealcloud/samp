#ifndef NETSTATS_H
#define NETSTATS_H

#include <stddef.h>
#include <windows.h>

#include <static_assert.h>
struct IDirect3DDevice9;
class CNetStats
{
public:
    DWORD lastTotalSent, lastTotalReceived, lastUpdateTick, uploadRate, downloadRate;
    IDirect3DDevice9* device;
    // 0x000607c0
    CNetStats(IDirect3DDevice9* device);
    // 0x000607f0
    void Draw();
};
STATIC_ASSERT(sizeof(CNetStats) == 24);
STATIC_ASSERT(offsetof(CNetStats, lastUpdateTick) == 8);
// Native buffers have unsafe writes/concatenations; overflow adjacency is unproven.
// 0x00163a78
extern char g_netStatsLine[1024];
// 0x00163e78
extern char g_netStatsScratch[16384];
// 0x00167e78
extern char g_netStatsDisplay[16384];
// 0x00156ab0
extern DWORD g_netStatsShotCompMode;
// 0x0011b8e4
extern const char g_netStatsRatesFormat[49];
// 0x0011b858
extern const char g_netStatsModelsFormat[140];
// 0x0011b83c
extern const char g_netStatsOnFootFormat[22];
// 0x0011b824
extern const char g_netStatsInCarFormat[21];
// 0x0011b7f4
extern const char g_netStatsMemoryFormat[46];
// 0x0011b7cc
extern const char g_netStatsProcessFormat[37];
// 0x0011b7b4
extern const char g_netStatsTitle[21];

#endif // NETSTATS_H
