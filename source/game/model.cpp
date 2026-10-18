#include "game/model.h"
#include "game/game.h"
// samp.dll RVA: 0x001c0444
void *g_gameCustomModelInfo[32768];
// samp.dll RVA: 0x001e0444
DWORD g_gameUseCustomModelInfo;
// samp.dll RVA: 0x00152ae0
void **g_gameModelInfoTable=g_gameCustomModelInfo;
// samp.dll RVA: 0x0011e664
const Scripting::OpcodeInfo g_gameRequestModelCommand={0x247,"i"};
// samp.dll RVA: 0x0011e678
const Scripting::OpcodeInfo g_gameLoadModelsCommand={0x38b,""};
// samp.dll RVA: 0x0011e6a0
const Scripting::OpcodeInfo g_gameModelAvailableCommand={0x248,"i"};
// samp.dll RVA: 0x0011e618
const char g_gameModelTimeoutWarning[41]="Warning: Model %u wouldn't load in time!";
// samp.dll RVA: 0x000a7630
void *__stdcall GameGetModelInfo(int model) {
    if(g_gameUseCustomModelInfo)return g_gameModelInfoTable[model];
    if(model>=0 && model<=20000)return ((void**)0xa9b0c8)[model];
    return 0;
}
// samp.dll RVA: 0x000b41a0
DWORD __stdcall GameGetModelData1c(int model) { return *(DWORD*)((BYTE*)GameGetModelInfo(model)+0x1c); }
// samp.dll RVA: 0x000a0780
void CGame::RequestModel(int model,int loadingStream) { (void)loadingStream;Scripting::ProcessCommand(&g_gameRequestModelCommand,model); }
// samp.dll RVA: 0x000a07a0
void CGame::LoadRequestedModels() { Scripting::ProcessCommand(&g_gameLoadModelsCommand); }
// Invalid IDs report TRUE. Preserve the inclusive upper bound and raw result.
// samp.dll RVA: 0x000a07b0
BOOL CGame::IsModelAvailable(int model) { if(model>=0&&model<=20000)return Scripting::ProcessCommand(&g_gameModelAvailableCommand,model);return TRUE; }
