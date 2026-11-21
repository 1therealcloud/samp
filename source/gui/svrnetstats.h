#ifndef SVRNETSTATS_H
#define SVRNETSTATS_H

#include <windows.h>
struct IDirect3DDevice9;
class CSrvNetStats
{
public:
    BYTE nativeState[0x18];
    // 0x00071220
    CSrvNetStats(IDirect3DDevice9* device);
    // 0x00071260
    void Draw();
};
// 0x0012e968
extern DWORD g_srvNetStatsHistory[74];

#endif // SVRNETSTATS_H
