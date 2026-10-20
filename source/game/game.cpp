#include "game/input.h"
#include <string.h>
// samp.dll RVA: 0x0018e1d8
BOOL g_gameUsedPlayerSlots[210];
// samp.dll RVA: 0x0009fda0
CCamera::CCamera() { m_pMatrix=(CMatrix*)0xb6f99c;m_pAttachedTo=0; }
// samp.dll RVA: 0x0009fdc0
CGame::CGame() {
    m_pAudio=new CAudio;
    m_pCamera=new CCamera;
    m_pPlayerPed=0;
    unknown4d=0;unknown29=0;unknown31=0;
    m_nCursorMode=0;m_nInputEnableWaitFrames=0;m_bClockEnabled=FALSE;unknown6d=0;
    memset(g_gameUsedPlayerSlots,0,sizeof(g_gameUsedPlayerSlots));
    memset(m_aKeepLoadedVehicleModels,0,sizeof(m_aKeepLoadedVehicleModels));
    unknown55=0;m_bHeadMove=TRUE;m_nFrameLimiter=90;
}
// samp.dll RVA: 0x000a1190
IDirect3DDevice9 *CGame::GetDevice() {
    // Legacy 0.2x source matches this native body; both RenderWare calls take no stack arguments.
    IDirect3DDevice9 *device=0;
    if(g_gameVersion==1) {
        __asm mov edx,0x7f9d50
        __asm call edx
        __asm mov device,eax
    } else if(g_gameVersion==2) {
        __asm mov edx,0x7f9d90
        __asm call edx
        __asm mov device,eax
    }
    return device;
}
// samp.dll RVA: 0x000a0770
BOOL CGame::IsStarted() { return *(BYTE*)0xba6831==0; }
// samp.dll RVA: 0x000a09f0
void CGame::SetFrameLimiter(int value) {
    m_nFrameLimiter=value;
    GameUnprotect((void*)0xc1704c,4);*(DWORD*)0xc1704c=200;
}
// samp.dll RVA: 0x000a1280
void CGame::EnableClock(bool enable) {
    GameUnprotect((void*)0x859a6c,10);
    GameUnprotect((void*)0x52cf10,1);
    if(enable) {
        *(BYTE*)0x52cf10=0x56;m_bClockEnabled=TRUE;
        memcpy((void*)0x859a6c,"%02d:%02d",10);
    } else {
        *(BYTE*)0x52cf10=0xc3;m_bClockEnabled=FALSE;
        memset((void*)0x859a6c,0,10);
    }
}
// samp.dll RVA: 0x00002d40
void *CGame::GetWindowHandle() { return *(void**)0xc97c1c; }
// samp.dll RVA: 0x00002d50
CAudio *CGame::GetAudio() { return m_pAudio; }
// samp.dll RVA: 0x00002d60
CCamera *CGame::GetCamera() { return m_pCamera; }
// samp.dll RVA: 0x00002d70
BOOL CGame::DoesHeadMoves() { return m_bHeadMove; }

// GTA SA virtual addresses: fixed range8E6C10..8E7C8C, stride20 (211 records).
// Tests the entire DWORD, never substitute a live vehicle-pool count.
// samp.dll RVA: 0x000a1410
int CGame::CountBackendVehicleModelRecords() {
    int count=0;
    for(DWORD address=0x8e6c10;address!=0x8e7c8c;address+=20) {
        if(*(DWORD*)address)++count;
    }
    return count;
}
