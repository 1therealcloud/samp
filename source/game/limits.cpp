#include "game/limits.h"
#include "game/input.h"
#include "game/preinit.h"
#include <string.h>
#include "game/limit_values.inc"
// samp.dll RVA: 0x001e04b0
DWORD g_gameScanList[40000];
// samp.dll RVA: 0x002075b0
GamePedModelRecord g_gamePedModels[319];
// samp.dll RVA: 0x0020ca70
GameModelRecord32 g_gameModelRecords[20000];
// samp.dll RVA: 0x002a8e70
BYTE g_gameModelFlags[1024];
// samp.dll RVA: 0x000aa6a0
void GameRelocateScanList() {
    memset(g_gameScanList,0,sizeof(g_gameScanList));
    for(int i=0;i<14;i++) {
        if(g_gameVersion==1) { GameUnprotect((void*)g_gameScanDirectUSA[i],4);*(DWORD*)g_gameScanDirectUSA[i]=(DWORD)g_gameScanList; }
        else if(g_gameVersion==2) { GameUnprotect((void*)g_gameScanDirectEU[i],4);*(DWORD*)g_gameScanDirectEU[i]=(DWORD)g_gameScanList; }
    }
    for(int j=0;j<56;j++) {
        if(g_gameVersion==1) { GameUnprotect((void*)g_gameScanOperandUSA[j],8);*(DWORD*)(g_gameScanOperandUSA[j]+3)=(DWORD)g_gameScanList; }
        else if(g_gameVersion==2) { GameUnprotect((void*)g_gameScanOperandEU[j],8);*(DWORD*)(g_gameScanOperandEU[j]+3)=(DWORD)g_gameScanList; }
    }
    for(int k=0;k<11;k++) { GameUnprotect((void*)g_gameScanSecondSites[k],8);*(DWORD*)(g_gameScanSecondSites[k]+3)=(DWORD)(g_gameScanList+1); }
    // In the native DLL the ped store begins exactly one-past the 40000-DWORD scan list.
    // These four operands terminate scan traversal; they do not reference ped records.
    for(int p=0;p<4;p++) { GameUnprotect((void*)g_gamePedReferenceSites[p],4);*(DWORD*)g_gamePedReferenceSites[p]=(DWORD)(g_gameScanList+40000); }
    GameUnprotect((void*)0x564dc7,4);*(DWORD*)0x564dc7=(DWORD)((BYTE*)g_gameScanList+0x1c200);
    GameUnprotect((void*)0x40936a,4);*(DWORD*)0x40936a=(DWORD)(g_gameScanList+1);
    memset((void*)0xb7d0b8,0,0x1c200);
}
// samp.dll RVA: 0x000aa5e0
void GameRelocatePedModels() {
    for(int i=0;i<319;i++) { g_gamePedModels[i].vtable=0x85bdc0;memset(g_gamePedModels[i].data,0,64); }
    GameUnprotect((void*)0x4c67ad,4);*(DWORD*)0x4c67ad=(DWORD)g_gamePedModels;
}
// samp.dll RVA: 0x000aa630
void GameRelocateModelRecords() {
    for(int i=0;i<20000;i++) { g_gameModelRecords[i].vtable=0x85bbf0;memset(g_gameModelRecords[i].data,0,28); }
    for(int j=0;j<14;j++) { GameUnprotect((void*)g_gameModelReferenceSites[j],4);*(DWORD*)g_gameModelReferenceSites[j]=(DWORD)g_gameModelRecords; }
}
// samp.dll RVA: 0x000aa980
void GameRelocateModelFlags() {
    memcpy(g_gameModelFlags,g_gameDefaultModelFlags,1024);
    GameUnprotect((void*)0x44b1c1,4);*(DWORD*)0x44b1c1=(DWORD)g_gameModelFlags;
    GameUnprotect((void*)0x4c8390,4);*(DWORD*)0x4c8390=(DWORD)g_gameModelFlags;
    GameUnprotect((void*)0x4c8399,4);*(DWORD*)0x4c8399=(DWORD)(g_gameModelFlags+1);
    GameUnprotect((void*)0x4c83a3,4);*(DWORD*)0x4c83a3=(DWORD)(g_gameModelFlags+2);
    GameUnprotect((void*)0x5817cc,4);*(DWORD*)0x5817cc=(DWORD)g_gameModelFlags;
    GameUnprotect((void*)0x582176,4);*(DWORD*)0x582176=(DWORD)g_gameModelFlags;
    GameUnprotect((void*)0x6a6ffa,4);*(DWORD*)0x6a6ffa=(DWORD)g_gameModelFlags;
}
// samp.dll RVA: 0x000aa180
void GameIncreasePoolLimits() {
    GameWaitUnprotect((BYTE*)0x551024,7,0x68);memcpy((void*)0x551024,g_gamePoolPatchOne,7);
    GameUnprotect((void*)0x55105f,4);*(DWORD*)0x55105f=20000;
    GameUnprotect((void*)0x5510cf,4);*(DWORD*)0x5510cf=16000;
    GameUnprotect((void*)0x550f46,4);*(DWORD*)0x550f46=300000;
    GameUnprotect((void*)0x550f82,4);*(DWORD*)0x550f82=210000;
    GameUnprotect((void*)0x550fba,4);*(DWORD*)0x550fba=50000;
    GameUnprotect((void*)0x551097,4);*(DWORD*)0x551097=0xc1c;
    GameUnprotect((void*)0x550ff2,1);*(BYTE*)0x550ff2=0xf0;
    GameUnprotect((void*)0x551283,1);*(BYTE*)0x551283=0xf0;
    GameUnprotect((void*)0x551140,1);*(BYTE*)0x551140=5;
    GameUnprotect((void*)0x551178,1);*(BYTE*)0x551178=1;
    GameUnprotect((void*)0x54f3a1,4);*(DWORD*)0x54f3a1=20000;
    GameUnprotect((void*)0x551106,5);memcpy((void*)0x551106,g_gamePoolPatchTwo,5);
}
