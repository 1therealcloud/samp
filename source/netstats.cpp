#include "netstats.h"
#include "gui/render_dependencies.h"
#include "gui/device_objects.h"
#include "fontrender.h"
#include "main.h"
#include "game/debug.h"
#include "game/playerped.h"
#include "net/vehiclepool.h"
#include "net/objectpool.h"
#include "net/localplayer.h"
#include "RakClientInterface.h"
#include "RakNetStatistics.h"
#include <stdio.h>
#include <string.h>
#include <psapi.h>
// Match the selected RakNet fields actually consumed by the original UI.
typedef char NetStatsSentOffset[(offsetof(RakNetStatisticsStruct,totalBitsSent)==0xb8)?1:-1];
typedef char NetStatsReceivedOffset[(offsetof(RakNetStatisticsStruct,bitsReceived)==0xd8)?1:-1];
typedef char NetStatsResentOffset[(offsetof(RakNetStatisticsStruct,messagesTotalBitsResent)==0x94)?1:-1];
typedef char NetStatsProcessCountersExtent[(sizeof(PROCESS_MEMORY_COUNTERS)==40)?1:-1];
// samp.dll RVA: 0x00163a78
char g_netStatsLine[1024];
// samp.dll RVA: 0x00163e78
char g_netStatsScratch[16384];
// samp.dll RVA: 0x00167e78
char g_netStatsDisplay[16384];
// samp.dll RVA: 0x00156ab0
DWORD g_netStatsShotCompMode;
// samp.dll RVA: 0x0011b8e4
const char g_netStatsRatesFormat[49]="Download Rate: %.2f KB/s\nUpload Rate: %.2f KB/s\n";
// samp.dll RVA: 0x0011b858
const char g_netStatsModelsFormat[140]="Vehicles: %u\nVehicle Models: %u\nVehicle Models Loaded: %u\nRWObjects: %u\nPedSlotsUsed: %u\nObjectSlotsUsed: %u\nShotCompMode: %u\nFreeIDEs: %u\n";
// samp.dll RVA: 0x0011b83c
const char g_netStatsOnFootFormat[22]="OnFoot Send Rate: %u\n";
// samp.dll RVA: 0x0011b824
const char g_netStatsInCarFormat[21]="InCar Send Rate: %u\n";
// samp.dll RVA: 0x0011b7f4
const char g_netStatsMemoryFormat[46]="Streaming Mem: %uMB Total: %uMB System: %uMB\n";
// samp.dll RVA: 0x0011b7cc
const char g_netStatsProcessFormat[37]="Process Mem: %uKB Working Set: %uKB\n";
// samp.dll RVA: 0x0011b7b4
const char g_netStatsTitle[21]="Client Network Stats";

// samp.dll RVA: 0x000607c0
CNetStats::CNetStats(IDirect3DDevice9 *value) {
    lastUpdateTick=GetTickCount();lastTotalReceived=0;lastTotalSent=0;
    downloadRate=0;uploadRate=0;device=value;
}
// samp.dll RVA: 0x000607f0
void CNetStats::Draw() {
    if(!g_pNetGame)return;
    RakClientInterface *client=*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c);
    if(!client||!client->IsConnected())return;
    RakNetStatisticsStruct *stats=(*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->GetStatistics();
    if(GetTickCount()-lastUpdateTick>1000) {
        lastUpdateTick=GetTickCount();
        downloadRate=(DWORD)(stats->bitsReceived>>3)-lastTotalReceived;
        lastTotalReceived=(DWORD)(stats->bitsReceived>>3);
        uploadRate=(DWORD)(stats->totalBitsSent>>3)-lastTotalSent;
        lastTotalSent=(DWORD)(stats->totalBitsSent>>3);
    }
    double down=(double)downloadRate*0.0009765625;
    double up=(double)uploadRate*0.0009765625;
    BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);
    CLocalPlayer *local=*(CLocalPlayer**)(*(BYTE**)(pools+8)+0x1e);
    CPed *ped=*(CPed**)local;
    sprintf(g_netStatsDisplay,g_netStatsRatesFormat,down,up);
    BYTE modelCounts[212]={0};
    pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);
    CVehiclePool *vehicles=*(CVehiclePool**)(pools+0xc);
    CObjectPool *objects=*(CObjectPool**)(pools+0x14);
    DWORD vehicleCount=0,uniqueModels=0,objectCount=objects?objects->GetCount():0;
    if(vehicles) {
        int loadedModels=g_pGame->CountBackendVehicleModelRecords();
        for(int id=0;id<2000;id++)if(vehicles->m_bNotEmpty[id]) {
            ++vehicleCount;
            // Preserve native unchecked wrapper and model index, BYTE wrap,
            // and the second live wrapper load before the backend model read.
            CVehicle *vehicle=vehicles->m_pObjects[id];
            if(vehicle->m_pGameVehicle) {
                vehicle=vehicles->m_pObjects[id];
                int model=*(short*)((BYTE*)vehicle->m_pGameVehicle+0x22);
                ++modelCounts[model-400];
            }
        }
        for(int model=0;model<212;model++)if(modelCounts[model])++uniqueModels;
        DWORD freeModels=GameCountUnusedModelSlots();
        DWORD shotComp=g_netStatsShotCompMode;
        BYTE pedSlots=g_pGame->CountUsedPlayerSlots();
        DWORD rwObjects=g_pGame->GetBackendWordB71804();
        char text[256];
        sprintf(text,g_netStatsModelsFormat,vehicleCount,uniqueModels,loadedModels,
            rwObjects,(DWORD)pedSlots,objectCount,shotComp,freeModels);
        strcat(g_netStatsDisplay,text);
    }
    if(ped) {
        if(!ped->IsInVehicle())sprintf(g_netStatsScratch,g_netStatsOnFootFormat,local->GetOnFootSendRate());
        else sprintf(g_netStatsScratch,g_netStatsInCarFormat,local->GetInCarSendRate());
        strcat(g_netStatsDisplay,g_netStatsScratch);
    }
    if(g_pGame) {
        DWORD used=g_pGame->GetUsedMemory();
        DWORD total=g_pGame->GetStreamingMemory();
        if(used&&total)sprintf(g_netStatsScratch,g_netStatsMemoryFormat,used>>20,total>>20,g_gamePhysicalMegabytes);
        // Native appends the previous scratch contents when either value is0.
        strcat(g_netStatsDisplay,g_netStatsScratch);
    }
    PROCESS_MEMORY_COUNTERS memory;memset(&memory,0,sizeof(memory));
    if(GetProcessMemoryInfo(GetCurrentProcess(),&memory,sizeof(memory))) {
        sprintf(g_netStatsScratch,g_netStatsProcessFormat,
            memory.PagefileUsage>>10,memory.WorkingSetSize>>10);
        strcat(g_netStatsDisplay,g_netStatsScratch);
    }
    StatisticsToString(stats,g_netStatsScratch,4);
    strcat(g_netStatsDisplay,g_netStatsScratch);
    FontSize fullSize,probe;
    g_deviceFonts->GetTextScreenSize(&fullSize,g_netStatsDisplay,0);
    g_deviceFonts->GetTextScreenSize(&probe,g_fontMeasureProbe,0);
    // GTA SA virtual address: width captured once for title and all body lines.
    int right=*(DWORD*)0xc17044-150;
    RECT title={10,10,right,10+probe.cy};
    g_deviceFonts->DrawText(0,g_netStatsTitle,title,0xff8888ee,TRUE);
    int top=10+probe.cy,bottom=10+probe.cy*2;
    int used=0;
    for(char *text=g_netStatsDisplay;*text;++text) {
        g_netStatsLine[used]=*text;
        if(*text=='\n') {
            g_netStatsLine[used]=0;
            RECT line={10,top,right,bottom};
            g_deviceFonts->DrawText(0,g_netStatsLine,line,0xffffffff,TRUE);
            top+=probe.cy;bottom+=probe.cy;used=0;
        } else ++used;
    }
    // Original draws only newline-terminated lines; no final-tail flush.
}
