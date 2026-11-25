#ifndef SVRNETSTATS_H
#define SVRNETSTATS_H

#include <windows.h>
struct IDirect3DDevice9;
class CSrvNetStats
{
public:
    BYTE nativeState[0x18];

    CSrvNetStats(IDirect3DDevice9* device);

    void Draw();
};

extern DWORD g_srvNetStatsHistory[74];

#endif // SVRNETSTATS_H
