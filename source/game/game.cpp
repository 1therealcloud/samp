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

// GTA SA virtual addresses: direct original game storage; no SA-MP-owned alias.
// samp.dll RVA: 0x000a0db0
int CGame::GetPlayerMoney() { return *(int*)0xb7ce50; }

// GTA SA virtual addresses: direct original game storage; no SA-MP-owned alias.
// samp.dll RVA: 0x000a13f0
DWORD CGame::GetUsedMemory() { return *(DWORD*)0x8e4cb4; }

// GTA SA virtual addresses: direct original game storage; no SA-MP-owned alias.
// samp.dll RVA: 0x000a1400
DWORD CGame::GetStreamingMemory() { return *(DWORD*)0x8a5a80; }

// GTA SA virtual addresses: direct original game storage; no SA-MP-owned alias.
// samp.dll RVA: 0x000a1240
void CGame::SetWantedLevel(BYTE level) { *(BYTE*)0x58db60=level; }

// GTA SA virtual addresses: direct original game storage; no SA-MP-owned alias.
// samp.dll RVA: 0x000a0920
void CGame::GetWorldTime(BYTE *hour,BYTE *minute) { *minute=*(BYTE*)0xb70152;
    *hour=*(BYTE*)0xb70153; }

#include "game/wrapper_commands.h"

// samp.dll RVA: 0x000a0a50
void CGame::DisableTrainTraffic() { Scripting::ProcessCommand(&g_gameDisableTrainTrafficCommand,0); }

// samp.dll RVA: 0x000a0a90
void CGame::RequestAnimation(const char *name) { Scripting::ProcessCommand(&g_gameRequestAnimationCommand,name); }

// samp.dll RVA: 0x000a0ab0
DWORD CGame::IsAnimationLoaded(const char *name) { return Scripting::ProcessCommand(&g_gameIsAnimationLoadedCommand,name); }

// samp.dll RVA: 0x000a0ce0
void CGame::DeleteMarker(DWORD marker) { Scripting::ProcessCommand(&g_gameDeleteMarkerCommand,marker); }

// samp.dll RVA: 0x000a0d90
void CGame::IncreasePlayerMoney(int amount) { Scripting::ProcessCommand(&g_gameIncreaseMoneyCommand,0,amount); }

// samp.dll RVA: 0x000a1340
void CGame::EnableZoneDisplaying(BYTE enabled) { Scripting::ProcessCommand(&g_gameZoneDisplayCommand,(DWORD)enabled); }

// samp.dll RVA: 0x000a1360
void CGame::EnableStuntBonus(BYTE enabled) {
    // GTA SA virtual address: 0x00a4a474.
    GameUnprotect((void*)0xa4a474,4);
    *(DWORD*)0xa4a474=enabled; }

// samp.dll RVA: 0x000a1220
void CGame::SetWorldGravity(float gravity) {
    // GTA SA virtual address: 0x00863984; store raw float bits without x87 conversion.
    GameUnprotect((void*)0x863984,4);
    *(DWORD*)0x863984=*(DWORD*)&gravity; }

// samp.dll RVA: 0x000a13d0
void CGame::LoadScene(const char *name) {
    // GTA SA virtual address: 0x005b8700, cdecl.
    ((void (__cdecl *)(const char*))0x5b8700)(name);
}

// samp.dll RVA: 0x000a19a0
void CGame::LoadCollisionFile(const char *name) {
    // GTA SA virtual address: 0x005b4e60, cdecl; native second argument is zero.
    ((void (__cdecl *)(const char*,int))0x5b4e60)(name,0);
}

// samp.dll RVA: 0x000a19c0
void CGame::LoadCullZone(const char *name) {
    // GTA SA virtual address: 0x005b4b40, cdecl.
    ((void (__cdecl *)(const char*))0x5b4b40)(name);
}

// samp.dll RVA: 0x000a1200
void * CGame::GetWeaponInfo(int weapon,int skill) {
    // GTA SA virtual address: 0x00743c60, cdecl; raw backend pointer.
    return ((void *(__cdecl *)(int,int))0x743c60)(weapon,skill);
}

// samp.dll RVA: 0x000a19f0
void CGame::DisableAutoAiming() {
    // GTA SA virtual addresses: 0x0060dc50 and 0x0060e530..0x0060e532.
    GameUnprotect((void*)0x60dc50,1);
    *(BYTE*)0x60dc50=0xc3;
    GameUnprotect((void*)0x60e530,3);
    *(BYTE*)0x60e530=0xc2;
    *(BYTE*)0x60e531=8;
    *(BYTE*)0x60e532=0;
}

// samp.dll RVA: 0x000a1380
void CGame::DisableEnterExits() {
    // GTA SA virtual address: 0x0096a7d8; unchecked native array/count prefix.
    BYTE *pool=*(BYTE *volatile*)0x96a7d8;
    BYTE *record=*(BYTE *volatile*)pool;
    DWORD count=*(volatile DWORD*)(pool+8);
    while(count) {
        *(volatile WORD*)(record+0x30)&=0;
        record+=0x3c;
        --count;
    }
}

// samp.dll RVA: 0x000a0240
float CGame::FindGroundZ(float x,float y,float z) {
    Scripting::ProcessCommand(&g_gameFindGroundZCommand,(double)x,(double)y,(double)z,&z);
    return z;
}

// samp.dll RVA: 0x000a0a60
void CGame::RefreshRenderer(float x,float y) {
    Scripting::ProcessCommand(&g_gameRefreshRendererCommand,(double)x,(double)y);
}

#include "game/playerped.h"
#include "game/vehicle.h"

// samp.dll RVA: 0x000a0090
CVehicle * CGame::CreateVehicle(int model,float x,float y,float z,float rotation,BOOL siren) {
    // Native indexes this+model-0x122 without a model-range check.
    DWORD keepLoaded=*((BYTE*)this+model-0x122)!=0;
    CVehicle *vehicle=new CVehicle(model,x,y,z,rotation,keepLoaded,siren);
    // Native dereferences even a null allocation; failed backend creation leaks this wrapper.
    return vehicle->m_pGameVehicle?vehicle:0;
}

// samp.dll RVA: 0x000a0050
BOOL CGame::RemovePed(CPed *ped) {
    if(!ped) return FALSE;
    BYTE player=ped->m_nPlayerNumber;
    delete ped;
    g_gameUsedPlayerSlots[player]=FALSE;
    return TRUE;
}

// samp.dll RVA: 0x000a1ee0
void CGame::ResetMoney() {
    // GTA SA virtual address: 0x00b7ce50. Modular negation preserves INT_MIN.
    DWORD money=*(DWORD*)0xb7ce50;
    if(money) Scripting::ProcessCommand(&g_gameIncreaseMoneyCommand,0,0u-money);
}

// samp.dll RVA: 0x000a0b80
void CGame::DeleteRacingCheckpoint() {
    if(unknown31) {
        Scripting::ProcessCommand(&g_gameDeleteRacingCheckpointCommand,unknown31);
        unknown31=0;
    }
    unknown29=0;
}
