#include "svrnetstats.h"
#include <string.h>
// samp.dll RVA: 0x0016c7f8
DWORD g_srvNetStatsHistory[74];
// samp.dll RVA: 0x00070cc0
CSrvNetStats::CSrvNetStats(IDirect3DDevice9 *device){
    DWORD *state=(DWORD*)nativeState;state[2]=GetTickCount();state[1]=0;state[0]=0;state[4]=0;state[3]=0;state[5]=(DWORD)device;
    memset(g_srvNetStatsHistory,0,sizeof(g_srvNetStatsHistory));
}

// The original DL function is RET. This is native disabled behavior,
// not a stand-in for an unrecovered renderer.
// samp.dll RVA: 0x00070d00
void CSrvNetStats::Draw() {}
