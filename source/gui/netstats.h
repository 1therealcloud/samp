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
    // 0x00060d40
    CNetStats(IDirect3DDevice9* device);
    // 0x00060d70
    void Draw();
};
STATIC_ASSERT(sizeof(CNetStats) == 24);
STATIC_ASSERT(offsetof(CNetStats, lastUpdateTick) == 8);
// Native buffers have unsafe writes/concatenations; overflow adjacency is unproven.
// R5 address to verify
extern char g_netStatsLine[1024];
// R5 address to verify
extern char g_netStatsScratch[16384];
// R5 address to verify
extern char g_netStatsDisplay[16384];
// R5 address to verify
extern DWORD g_netStatsShotCompMode;
// 0x000e98ac
extern const char g_netStatsRatesFormat[49];
// 0x000e9820
extern const char g_netStatsModelsFormat[140];
// 0x000e9804
extern const char g_netStatsOnFootFormat[22];
// 0x000e97ec
extern const char g_netStatsInCarFormat[21];
// 0x000e97bc
extern const char g_netStatsMemoryFormat[46];
// 0x000e9794
extern const char g_netStatsProcessFormat[37];
// 0x000e977c
extern const char g_netStatsTitle[21];

#endif // NETSTATS_H
