#ifndef NETSTATS_H
#define NETSTATS_H

#include <stddef.h>
#include <windows.h>

struct IDirect3DDevice9;
class CNetStats
{
public:
    DWORD lastTotalSent, lastTotalReceived, lastUpdateTick, uploadRate, downloadRate;
    IDirect3DDevice9* device;

    CNetStats(IDirect3DDevice9* device);

    void Draw();
};
static_assert(sizeof(CNetStats) == 24);
static_assert(offsetof(CNetStats, lastUpdateTick) == 8);
// Native buffers have unsafe writes/concatenations; overflow adjacency is unproven.

extern char g_netStatsLine[1024];

extern char g_netStatsScratch[16384];

extern char g_netStatsDisplay[16384];

extern DWORD g_netStatsShotCompMode;

extern const char g_netStatsRatesFormat[49];

extern const char g_netStatsModelsFormat[140];

extern const char g_netStatsOnFootFormat[22];

extern const char g_netStatsInCarFormat[21];

extern const char g_netStatsMemoryFormat[46];

extern const char g_netStatsProcessFormat[37];

extern const char g_netStatsTitle[21];

#endif // NETSTATS_H
