#include "gui/svrnetstats.h"

#include <string.h>

DWORD g_srvNetStatsHistory[74];

CSrvNetStats::CSrvNetStats(IDirect3DDevice9* device)
{
    DWORD* state = (DWORD*)nativeState;
    state[2] = GetTickCount();
    state[1] = 0;
    state[0] = 0;
    state[4] = 0;
    state[3] = 0;
    state[5] = (DWORD)device;
    memset(g_srvNetStatsHistory, 0, sizeof(g_srvNetStatsHistory));
}

// The original returns without drawing.

void CSrvNetStats::Draw() {}
