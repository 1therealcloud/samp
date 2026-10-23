#include "game/game.h"
#include <string.h>
// samp.dll RVA: 0x0018e1d8
BOOL g_gameUsedPlayerSlots[210];
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

#include "game/scripting.h"

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

// Observed checkpoint region at CGame+0x0c, not a separate native allocation.
#pragma pack(push,1)
struct GameCheckpointState {
    CVector racePosition,raceNext;
    float raceSize;
    BYTE raceType;
    DWORD raceEnabled,raceMarker,raceCheckpoint;
    CVector position,size;
    DWORD enabled,marker;
};
#pragma pack(pop)
typedef char CheckpointRegionSize[(sizeof(GameCheckpointState)==0x49)?1:-1];
typedef char CheckpointMarkerOffset[(offsetof(GameCheckpointState,marker)==0x45)?1:-1];
typedef char RacingMarkerOffset[(offsetof(GameCheckpointState,raceMarker)==0x21)?1:-1];

// samp.dll RVA: 0x000a0bb0
DWORD CGame::CreateMarker(int icon,float x,float y,float z,DWORD color,int type) {
    DWORD marker=0;
    const Scripting::OpcodeInfo *command=&g_gameCreateMarkerCommand;
    if(type==1) command=&g_gameCreateMarkerType1Command;
    else if(type==2) command=&g_gameCreateMarkerType2Command;
    else if(type==3) command=&g_gameCreateMarkerType3Command;
    Scripting::ProcessCommand(command,(double)x,(double)y,(double)z,icon,&marker);
    if(icon==0) {
        Scripting::ProcessCommand(&g_gameMarkerColorCommand,marker,color);
        Scripting::ProcessCommand(&g_gameMarkerModeCommand,marker,color<1004u?2:3);
    }
    return marker;
}

// samp.dll RVA: 0x000a1c00
void CGame::SetCheckpoint(const CVector *position,const CVector *size) {
    GameCheckpointState *s=(GameCheckpointState*)((BYTE*)this+0x0c);
    s->position=*position;
    s->size=*size;
    if(s->marker) {
        Scripting::ProcessCommand(&g_gameDeleteMarkerCommand,s->marker);
        s->marker=0;
        DWORD marker=0;
        Scripting::ProcessCommand(&g_gameCreateMarkerCommand,(double)s->position.x,(double)s->position.y,(double)s->position.z,0,&marker);
        Scripting::ProcessCommand(&g_gameMarkerColorCommand,marker,1005);
        Scripting::ProcessCommand(&g_gameMarkerModeCommand,marker,3);
        s->marker=marker;
    }
}

// samp.dll RVA: 0x000a1cc0
void CGame::CreateRacingCheckpoint() {
    GameCheckpointState *s=(GameCheckpointState*)((BYTE*)this+0x0c);
    if(s->raceCheckpoint) {
        Scripting::ProcessCommand(&g_gameDeleteRacingCheckpointCommand,s->raceCheckpoint);
        s->raceCheckpoint=0;
    }
    s->raceEnabled=0;
    Scripting::ProcessCommand(&g_gameCreateRacingCheckpointCommand,(DWORD)s->raceType,
        (double)s->racePosition.x,(double)s->racePosition.y,(double)s->racePosition.z,
        (double)s->raceNext.x,(double)s->raceNext.y,(double)s->raceNext.z,
        (double)s->raceSize,&s->raceCheckpoint);
    s->raceEnabled=1;
}

// samp.dll RVA: 0x000a1f20
void CGame::SetRacingCheckpoint(BYTE type,const CVector *position,const CVector *next,float size) {
    GameCheckpointState *s=(GameCheckpointState*)((BYTE*)this+0x0c);
    s->racePosition=*position;
    s->raceNext=*next;
    s->raceSize=size;
    s->raceType=type;
    if(s->raceMarker) {
        Scripting::ProcessCommand(&g_gameDeleteMarkerCommand,s->raceMarker);
        s->raceMarker=0;
        DWORD marker=0;
        Scripting::ProcessCommand(&g_gameCreateMarkerCommand,(double)s->racePosition.x,(double)s->racePosition.y,(double)s->racePosition.z,0,&marker);
        Scripting::ProcessCommand(&g_gameMarkerColorCommand,marker,1005);
        Scripting::ProcessCommand(&g_gameMarkerModeCommand,marker,3);
        s->raceMarker=marker;
    }
    CreateRacingCheckpoint();
}

// samp.dll RVA: 0x000a1d40
void CGame::ProcessCheckpoints() {
    GameCheckpointState *s=(GameCheckpointState*)((BYTE*)this+0x0c);
    if(s->enabled) {
        CPed *ped=GetPlayerPed();
        if(ped) {
            Scripting::ProcessCommand(&g_gameLocateCheckpointCommand,ped->m_nHandle,
                (double)s->position.x,(double)s->position.y,(double)s->position.z,
                (double)s->size.x,(double)s->size.y,(double)s->size.z,1);
            if(!s->marker) {
                DWORD marker=0;
                Scripting::ProcessCommand(&g_gameCreateMarkerCommand,(double)s->position.x,(double)s->position.y,(double)s->position.z,0,&marker);
                Scripting::ProcessCommand(&g_gameMarkerColorCommand,marker,1005);
                Scripting::ProcessCommand(&g_gameMarkerModeCommand,marker,3);
                s->marker=marker;
            }
        }
    } else if(s->marker) {
        Scripting::ProcessCommand(&g_gameDeleteMarkerCommand,s->marker);
        s->marker=0;
    }
    if(s->raceEnabled) {
        if(GetPlayerPed()&&!s->raceMarker) {
            DWORD marker=0;
            Scripting::ProcessCommand(&g_gameCreateMarkerCommand,(double)s->racePosition.x,(double)s->racePosition.y,(double)s->racePosition.z,0,&marker);
            Scripting::ProcessCommand(&g_gameMarkerColorCommand,marker,1005);
            Scripting::ProcessCommand(&g_gameMarkerModeCommand,marker,3);
            s->raceMarker=marker;
        }
    } else if(s->raceMarker) {
        Scripting::ProcessCommand(&g_gameDeleteMarkerCommand,s->raceMarker);
        if(s->raceCheckpoint) {
            Scripting::ProcessCommand(&g_gameDeleteRacingCheckpointCommand,s->raceCheckpoint);
            s->raceCheckpoint=0;
        }
        s->raceEnabled=0;
        s->raceMarker=0;
    }
}


// samp.dll RVA: 0x000a0b20
void CGame::DisplayGameText(const char *text,int time,int style) {
    if(style<=200) {
        Scripting::ProcessCommand(&g_gameClearTextCommand);
        *(DWORD*)g_gameTextBuffer=0;
        strncpy(g_gameTextBuffer,text,512);
        // GTA SA virtual address: 0x0069f2b0, cdecl.
        ((void (__cdecl *)(const char*,int,int))0x69f2b0)(g_gameTextBuffer,time,style);
    }
}

// samp.dll RVA: 0x000a1010
DWORD CGame::CreatePickup(int model,int type,float x,float y,float z,DWORD *index) {
    int selected=model;
    if(!GameModelQueryB4170(selected)) selected=18631;
    if(selected>=0&&selected<=20000) {
        if(!Scripting::ProcessCommand(&g_gameModelAvailableCommand,selected)) {
            Scripting::ProcessCommand(&g_gameRequestModelCommand,selected);
            Scripting::ProcessCommand(&g_gameLoadModelsCommand);
            void (WINAPI *volatile sleep)(DWORD)=::Sleep;
            while(selected<=20000&&!Scripting::ProcessCommand(&g_gameModelAvailableCommand,selected)) sleep(1);
        }
    }
    // The native output aliases the original model argument, not the selected fallback.
    Scripting::ProcessCommand(&g_gameCreatePickupCommand,selected,type,(double)x,(double)y,(double)z,&model);
    if(index) *index=(DWORD)model&0xffff;
    return (DWORD)model;
}

// samp.dll RVA: 0x000a1260
void CGame::DrawGangZone(const float *position,DWORD color) {
    // GTA SA virtual addresses: radar flag0x00ba67a4 and cdecl draw0x005853d0.
    ((void (__cdecl *)(const float*,DWORD*,DWORD))0x5853d0)(position,&color,(DWORD)*(BYTE*)0xba67a4);
}

#include "game/custommodels.h"
#include "game/util.h"
// samp.dll RVA: 0x000a07e0
void CGame::ReleaseModel(int model,BYTE force) {
    if(model>=0) {
        if(model>=20000) return;
        if(force) {
            if(Scripting::ProcessCommand(&g_gameModelAvailableCommand,model)) {
                // GTA SA virtual address: 0x004089a0, cdecl model argument.
                ((void (__cdecl *)(int))0x4089a0)(model);
            }
        } else if(Scripting::ProcessCommand(&g_gameModelAvailableCommand,model)) {
            Scripting::ProcessCommand(&g_gameReleaseModelCommand,model);
        }
    } else if(g_customModels&&g_customModels->IsObjectEntry(model)&&!GamePoolHasObjectModel(model)) {
        g_customModels->ReleaseObjectEntry(model);
    }
}

// samp.dll RVA: 0x000a1430
void CGame::SetRequiredVehicleModels(BYTE *priorities) {
    memset(m_aKeepLoadedVehicleModels,0,sizeof(m_aKeepLoadedVehicleModels));
    for(int model=400;model<612;++model) ReleaseModel(model,0);
    int models[212];
    for(int i=0;i<212;++i) models[i]=400+i;
    for(int count=211;count>0;--count) {
        for(int i=0;i<count;++i) {
            if(priorities[i+1]>priorities[i]) {
                BYTE priority=priorities[i];
                priorities[i]=priorities[i+1];
                int model=models[i];
                models[i]=models[i+1];
                priorities[i+1]=priority;
                models[i+1]=model;
            }
        }
    }
    for(int i=0;i<80;++i) {
        if(priorities[i]) {
            int model=models[i];
            Scripting::ProcessCommand(&g_gameRequestModelCommand,model);
            Scripting::ProcessCommand(&g_gameLoadModelsCommand);
            m_aKeepLoadedVehicleModels[model-400]=true;
        }
    }
}

// samp.dll RVA: 0x000a16c0
void CGame::LoadAnimationsAndModels() {
    if(!IsAnimationLoaded(g_gameParachuteLibrary)) RequestAnimation(g_gameParachuteLibrary);
    if(!IsAnimationLoaded(g_pedDrinkLibrary)) RequestAnimation(g_pedDrinkLibrary);
    if(!IsAnimationLoaded(g_pedSmokeLibrary)) RequestAnimation(g_pedSmokeLibrary);
    if(!IsAnimationLoaded(g_pedUrinationLibrary)) RequestAnimation(g_pedUrinationLibrary);
    if(!IsAnimationLoaded(g_gameVendingLibrary)) RequestAnimation(g_gameVendingLibrary);
    if(!IsAnimationLoaded(g_gameFightBLibrary)) RequestAnimation(g_gameFightBLibrary);
    if(!IsAnimationLoaded(g_gameFightCLibrary)) RequestAnimation(g_gameFightCLibrary);
    if(!IsAnimationLoaded(g_gameFightDLibrary)) RequestAnimation(g_gameFightDLibrary);
    if(!IsAnimationLoaded(g_gameFightELibrary)) RequestAnimation(g_gameFightELibrary);
    if(!Scripting::ProcessCommand(&g_gameModelAvailableCommand,3131))
        Scripting::ProcessCommand(&g_gameRequestModelCommand,3131);
    if(!Scripting::ProcessCommand(&g_gameModelAvailableCommand,1485))
        Scripting::ProcessCommand(&g_gameRequestModelCommand,1485);
    if(!Scripting::ProcessCommand(&g_gameModelAvailableCommand,1486))
        Scripting::ProcessCommand(&g_gameRequestModelCommand,1486);
    if(!Scripting::ProcessCommand(&g_gameModelAvailableCommand,1543))
        Scripting::ProcessCommand(&g_gameRequestModelCommand,1543);
    if(!Scripting::ProcessCommand(&g_gameModelAvailableCommand,1546))
        Scripting::ProcessCommand(&g_gameRequestModelCommand,1546);
    if(!Scripting::ProcessCommand(&g_gameModelAvailableCommand,343))
        Scripting::ProcessCommand(&g_gameRequestModelCommand,343);
    if(!Scripting::ProcessCommand(&g_gameModelAvailableCommand,342))
        Scripting::ProcessCommand(&g_gameRequestModelCommand,342);
    if(!Scripting::ProcessCommand(&g_gameModelAvailableCommand,344))
        Scripting::ProcessCommand(&g_gameRequestModelCommand,344);
    if(!Scripting::ProcessCommand(&g_gameModelAvailableCommand,330))
        Scripting::ProcessCommand(&g_gameRequestModelCommand,330);
}

#include <stddef.h>
// samp.dll RVA: 0x002ac9d0
IDirect3DDevice9 *g_gameD3DDevice;
// samp.dll RVA: 0x002ac9b8
int g_gameVersion;
// samp.dll RVA: 0x0018e1c8
DWORD g_gameMouseDummy;
// samp.dll RVA: 0x00151918
BYTE g_gameKeyCall[5]={0xe8,0x46,0xf3,0xfe,0xff};
// samp.dll RVA: 0x00151920
BYTE g_gameMouseCallEU[5]={0xe8,0x04,0x7b,0x20,0x00};
// samp.dll RVA: 0x00151928
BYTE g_gameMouseCallUS[5]={0xe8,0xb4,0x7a,0x20,0x00};

// samp.dll RVA: 0x000aa0b0
void GameUnprotect(void *address,SIZE_T size) {
    DWORD oldProtect;
    VirtualProtect(address,size,PAGE_EXECUTE_READWRITE,&oldProtect);
}
// samp.dll RVA: 0x000a02f0
void GameClearMouseDelta() {
    *(DWORD*)0xb73424=0; *(DWORD*)0xb73428=0;
    ((void (__cdecl*)())0x541bd0)();
}
// samp.dll RVA: 0x000a0310
void GameUpdateMouse() { ((void (__cdecl*)())0x541dd0)(); }
// samp.dll RVA: 0x000a0320
void GameDisableMousePointers() {
    *(DWORD*)0xb7340c=0; *(DWORD*)0xb73410=0; *(DWORD*)0xb73414=0;
    GameUnprotect((void*)0x53f47a,4);GameUnprotect((void*)0x53f49a,4);GameUnprotect((void*)0x53f4b3,4);
    *(DWORD*)0x53f47a=(DWORD)&g_gameMouseDummy;
    *(DWORD*)0x53f49a=(DWORD)&g_gameMouseDummy;
    *(DWORD*)0x53f4b3=(DWORD)&g_gameMouseDummy;
}
// samp.dll RVA: 0x000a0370
void GameRestoreMousePointers() {
    GameUnprotect((void*)0x53f47a,4);GameUnprotect((void*)0x53f49a,4);GameUnprotect((void*)0x53f4b3,4);
    *(DWORD*)0x53f47a=0xb73410;*(DWORD*)0x53f49a=0xb73414;*(DWORD*)0x53f4b3=0xb7340c;
}
// samp.dll RVA: 0x000a03c0
void GameDisableMouseCalls() {
    GameUnprotect((void*)0x53f417,5);memset((void*)0x53f417,0x90,5);
    GameUnprotect((void*)0x53f41f,4);
    *(BYTE*)0x53f41f=0x33;*(BYTE*)0x53f420=0xc0;*(BYTE*)0x53f421=0x0f;*(BYTE*)0x53f422=0x84;
}
// samp.dll RVA: 0x000a0760
BOOL CGame::IsMenuVisible() { return *(DWORD*)0xba67a4!=0; }
// samp.dll RVA: 0x000a0530
void CGame::SetCursorMode(int mode,BOOL immediately) {
    CGame *state=this;
    if(mode==2) {
        GameUnprotect((void*)0x541df5,5);memset((void*)0x541df5,0x90,5);
        GameDisableMouseCalls();GameClearMouseDelta();GameUpdateMouse();
        GameUnprotect((void*)0x6194a0,1);*(BYTE*)0x6194a0=0xc3;
        g_gameD3DDevice->ShowCursor(TRUE);state->m_nCursorMode=2;
    } else if(mode==1) {
        if(state->m_nCursorMode!=1) {
            GameUnprotect((void*)0x541df5,5);memset((void*)0x541df5,0x90,5);state->m_nCursorMode=1;
        }
    } else if(mode==3) {
        if(state->m_nCursorMode!=3) {
            GameDisableMouseCalls();GameClearMouseDelta();GameUpdateMouse();
            GameUnprotect((void*)0x6194a0,1);*(BYTE*)0x6194a0=0xc3;
            g_gameD3DDevice->ShowCursor(TRUE);state->m_nCursorMode=3;
        }
    } else if(mode==4) {
        if(state->m_nCursorMode!=4) {
            GameDisableMousePointers();GameClearMouseDelta();GameUpdateMouse();
            GameUnprotect((void*)0x6194a0,1);*(BYTE*)0x6194a0=0xc3;state->m_nCursorMode=4;
        }
    } else if(mode==0 && state->m_nCursorMode!=0) {
        state->m_nInputEnableWaitFrames=immediately?0:10;
        g_gameD3DDevice->ShowCursor(FALSE);state->m_nCursorMode=0;
    }
}
// samp.dll RVA: 0x000a0410
void CGame::ProcessInputEnabling() {
    CGame *state=this;
    if(state->m_nCursorMode==0) {
        if(state->m_nInputEnableWaitFrames==0) {
            GameUnprotect((void*)0x541df5,5);memcpy((void*)0x541df5,g_gameKeyCall,5);
            GameUnprotect((void*)0x53f417,5);
            memcpy((void*)0x53f417,g_gameVersion==1?g_gameMouseCallUS:g_gameMouseCallEU,5);
            GameRestoreMousePointers();
            // Native changes protection at +2, then writes the four-byte instruction.
            GameUnprotect((void*)0x53f421,4);
            *(BYTE*)0x53f41f=0x85;*(BYTE*)0x53f420=0xc0;*(BYTE*)0x53f421=0x0f;*(BYTE*)0x53f422=0x8c;
            GameClearMouseDelta();GameUpdateMouse();GameClearMouseDelta();
            GameUnprotect((void*)0x6194a0,1);*(BYTE*)0x6194a0=0xe9;
            g_gameD3DDevice->ShowCursor(FALSE);--state->m_nInputEnableWaitFrames;
        } else if(state->m_nInputEnableWaitFrames>0) --state->m_nInputEnableWaitFrames;
    }
}

#include "game/keystuff.h"
#include "game/aimstuff.h"
#include "game/player_colors.h"
#include <stdlib.h>
// samp.dll RVA: 0x0018e1cc
char *g_gameTextBuffer;
// samp.dll RVA: 0x000a06d0
void CGame::InitGame() {
    g_gameTextBuffer=(char*)calloc(1,513);
    KeyStuff::Initialize();AimStuff::Initialize();PlayerColors::Initialize();
    if(!GameInitializeVersionPatches()) {
        MessageBoxA(0,"I can't determine your GTA version.\r\nSA-MP only supports GTA:SA v1.0 USA/EU","Version Error",MB_ICONWARNING);
        ExitProcess(1);
    }
}

#include <stdio.h>
// samp.dll RVA: 0x002a9270
DWORD g_gamePhysicalMegabytes;
// samp.dll RVA: 0x002a9274
DWORD g_gameStreamingBytes;
// samp.dll RVA: 0x0011ee84
const char g_gameStartupTitle[6]="title";
// samp.dll RVA: 0x0018f388
char g_gameFrameNodeError[256];
// samp.dll RVA: 0x0018f6b4
bool g_gameCustomStreamRead;
// samp.dll RVA: 0x0018f6b8
void *g_gameStreamBuffer;
// samp.dll RVA: 0x000aa060
void GameWaitUnprotect(BYTE *address,SIZE_T size,BYTE expected) {
    DWORD oldProtect;
    VirtualProtect(address,size,PAGE_EXECUTE_READWRITE,&oldProtect);
    if(*address!=expected) {
        do { Sleep(1); } while(*address!=expected);
        VirtualProtect(address,size,PAGE_EXECUTE_READWRITE,&oldProtect);
    }
}
// samp.dll RVA: 0x000aa300
BOOL GameInitializeVersionPatches() {
    BYTE *version=(BYTE*)0x747483;
    int count=0;
    while(*version!=0x89 && *version!=0xc8) {
        if(*(BYTE*)0xba6831==1)return FALSE;
        Sleep(10);if(++count>6000)return FALSE;
    }
    if(*version==0x89)g_gameVersion=1;
    else if(*version==0xc8)g_gameVersion=2;
    DWORD oldProtect;
    BYTE *skip=0;
    if(g_gameVersion==1)skip=(BYTE*)0x747483;
    else if(g_gameVersion==2)skip=(BYTE*)0x7474d3;
    if(skip) {
        VirtualProtect(skip,6,PAGE_EXECUTE_READWRITE,&oldProtect);
        *(BYTE*)0xc8d4c0=5;memset(skip,0x90,6);
    }
    VirtualProtect((void*)0x866cd8,10,PAGE_EXECUTE_READWRITE,&oldProtect);
    VirtualProtect((void*)0x866ccc,10,PAGE_EXECUTE_READWRITE,&oldProtect);
    memcpy((void*)0x866cd8,g_gameStartupTitle,6);memcpy((void*)0x866ccc,g_gameStartupTitle,6);
    VirtualProtect((void*)0x745b87,0x44,PAGE_EXECUTE_READWRITE,&oldProtect);memset((void*)0x745b87,0x90,0x44);
    VirtualProtect((void*)0x7459e1,2,PAGE_EXECUTE_READWRITE,&oldProtect);*(WORD*)0x7459e1=0x9090;
    GameWaitUnprotect((BYTE*)0x561872,0x1e,0x85);
    *(BYTE*)0x561872=0x33;*(BYTE*)0x561873=0xc0;memset((void*)0x561874,0x90,27);
    MEMORYSTATUSEX memory;memory.dwLength=sizeof(memory);GlobalMemoryStatusEx(&memory);
    g_gamePhysicalMegabytes=(DWORD)(memory.ullTotalPhys>>20);
    if(g_gamePhysicalMegabytes>4000)g_gameStreamingBytes=0x40000000;
    else if(g_gamePhysicalMegabytes>2000)g_gameStreamingBytes=0x20000000;
    else if(g_gamePhysicalMegabytes>1000)g_gameStreamingBytes=0x10000000;
    else if(g_gamePhysicalMegabytes>500)g_gameStreamingBytes=0x8000000;
    else g_gameStreamingBytes=0x6000000;
    VirtualProtect((void*)0x5b8e6a,4,PAGE_EXECUTE_READWRITE,&oldProtect);*(DWORD*)0x5b8e6a=g_gameStreamingBytes;
    GameWaitUnprotect((BYTE*)0x4083c0,1,0xb8);*(BYTE*)0x4083c0=0xc3;
    VirtualProtect((void*)0x590099,5,PAGE_EXECUTE_READWRITE,&oldProtect);memset((void*)0x590099,0x90,5);
    VirtualProtect((void*)0x53e94c,1,PAGE_EXECUTE_READWRITE,&oldProtect);*(BYTE*)0x53e94c=2;
    VirtualProtect((void*)0x731f60,4,PAGE_EXECUTE_READWRITE,&oldProtect);*(DWORD*)0x731f60=20000;
    VirtualProtect((void*)0x72fad6,4,PAGE_EXECUTE_READWRITE,&oldProtect);*(DWORD*)0x72fad6=(DWORD)GameFrameNodeNameHook;
    VirtualProtect((void*)0x6fa996,4,PAGE_EXECUTE_READWRITE,&oldProtect);*(DWORD*)0x6fa996=(DWORD)GameDataReadHook;
    return TRUE;
}
// samp.dll RVA: 0x000a5c10
DWORD __cdecl GameFrameNodeNameHook(void *arg0,DWORD length,void *arg2) {
    if(length>23) {
        if(g_pGame) {
            _snprintf(g_gameFrameNodeError,256,"SA-MP: FrameNode name was too long (%u). SA-MP must exit.",length);
            ShowWindow(g_inputGameWindow,SW_MINIMIZE);
            MessageBoxA(0,g_gameFrameNodeError,"SA-MP",MB_ICONWARNING);
        }
        __asm mov esp,0
        __asm int 3
    }
    return ((DWORD(__cdecl*)(void*,DWORD,void*))0x72fa50)(arg0,length,arg2);
}
// samp.dll RVA: 0x000a5c90
DWORD __cdecl GameDataReadHook(void *arg0,DWORD length,void *arg2) {
    if(g_gameCustomStreamRead) {
        g_gameStreamBuffer=calloc(1,length);
        GameReadStream(arg0,g_gameStreamBuffer,length);
        GameParseStreamData(g_gameStreamBuffer,length,arg2);
        if(g_gameStreamBuffer) { free(g_gameStreamBuffer);g_gameStreamBuffer=0; }
        return (DWORD)arg0;
    }
    return ((DWORD(__cdecl*)(void*,DWORD,void*))0x6f9fd0)(arg0,length,arg2);
}
// samp.dll RVA: 0x000b1a40
DWORD GameReadStream(void *stream,void *buffer,DWORD length) {
    DWORD address=g_gameVersion==1?0x7ec9d0:0x7eca10;
    return ((DWORD(__cdecl*)(void*,void*,DWORD))address)(stream,buffer,length);
}

#include "game/effects.h"
// samp.dll RVA: 0x000b36a0
BYTE GameBufferReader::ReadByte() {
    BYTE result=0;
    Read(&result,1);
    return result;
}
// samp.dll RVA: 0x000bc1d0
DWORD GameBufferReader::Read(void *out,DWORD count) {
    memset(out,0,count);
    if(position+count>length)count=length-position;
    if(!count)return 0;
    memcpy(out,data+position,count);
    if(position+count<length)position+=count;else { position=length;eof=true; }
    return count;
}
// samp.dll RVA: 0x000b3480
int GameTxdFind(const char *name) { return ((int(__cdecl*)(const char*))0x731850)(name); }
// samp.dll RVA: 0x000b3510
void GameTxdPush() { ((void(__cdecl*)())0x7316a0)(); }
// samp.dll RVA: 0x000b3520
void GameTxdPop() { ((void(__cdecl*)())0x7316b0)(); }
// samp.dll RVA: 0x000b3530
void GameTxdSetCurrent(int index) { ((void(__cdecl*)(int))0x7319c0)(index); }
// samp.dll RVA: 0x000b3540
void *GameTxdReadTexture(const char *name) {
    // Native constructs a one-pointer CSprite2d on the stack; this is an out-object, not the EAX result.
    void *texture=0;
    __asm push name
    __asm lea ecx,texture
    __asm mov edx,0x727270
    __asm call edx
    return texture;
}
// samp.dll RVA: 0x000b2860
void *GameLoadDictionaryTexture(const char *dictionary,const char *name) {
    int index=GameTxdFind(dictionary);if(index==-1)return 0;
    GameTxdPush();GameTxdSetCurrent(index);
    void *texture=GameTxdReadTexture(name);GameTxdPop();return texture;
}
// samp.dll RVA: 0x000b6610
void GameParseStreamData(const void *data,DWORD length,void *destination) {
    GameBufferReader *reader=new GameBufferReader(data,length);
    DWORD count=0;reader->Read(&count,4);
    if(count==0 || count>128) { delete reader;return; }
    DWORD selected=0;
    for(DWORD i=0;i<count;i++) {
        if(reader->eof)break;
        BYTE position[12];DWORD type=0,size=0;
        reader->Read(position,12);reader->Read(&type,4);reader->Read(&size,4);
        if((type==0 && (size==76 || size==80)) || (type==10 && size==40))++selected;
        reader->Skip(size);
    }
    if(!selected)return; // Native leaks the reader on this path; preserve it.
    BYTE *result=(BYTE*)calloc(1,selected*64+4);*(DWORD*)result=selected;
    reader->position=0;reader->eof=false;
    DWORD ignoredCount=0;reader->Read(&ignoredCount,4);
    DWORD written=0;BYTE *record=result+4;
    do {
        if(reader->eof)break;
        BYTE position[12];DWORD type=0,size=0;
        reader->Read(position,12);reader->Read(&type,4);reader->Read(&size,4);
        if(type==0 && (size==76 || size==80)) {
            BYTE effect[56]={0};memcpy(effect,position,12);
            reader->Read(effect+16,4); // Color.
            reader->Read(effect+20,4);reader->Read(effect+24,4);reader->Read(effect+28,4);reader->Read(effect+32,4);
            reader->Read(effect+38,1);reader->Read(effect+39,1);reader->Read(effect+40,1);reader->Read(effect+41,1);
            BYTE low=0,high=0,discard=0;reader->Read(&low,1);
            char first[65]={0},second[65]={0};reader->Read(first,24);reader->Read(second,24);
            reader->Read(effect+42,1);reader->Read(&high,1);*(WORD*)(effect+36)=low|((WORD)high<<8);
            if(size==80) { reader->Read(effect+43,1);reader->Read(effect+44,1);reader->Read(effect+45,1);reader->Read(&discard,1); }
            else { effect[43]=0;effect[44]=0;effect[45]=100; }
            reader->Read(&discard,1);
            if(strlen(first))*(void**)(effect+48)=GameLoadDictionaryTexture("particle",first);
            if(strlen(second))*(void**)(effect+52)=GameLoadDictionaryTexture("particle",second);
            memcpy(record,effect,56);record+=64;++written;
        } else if(type==10 && size==40) {
            BYTE effect[53]={0};memcpy(effect,position,12);*(DWORD*)(effect+12)=10;
            reader->Read(effect+16,12);reader->Read(effect+28,12);reader->Read(effect+40,12);
            DWORD value=0;reader->Read(&value,4);effect[52]=(BYTE)value;
            memcpy(record,effect,53);record+=64;++written;
        } else reader->Skip(size);
    } while(written!=selected);
    if(!written)free(result);
    else { *(DWORD*)result=written;*(void**)((BYTE*)destination+0x78)=result; }
    delete reader;
}

// Texture dictionary reference/slot adapters. Only the native -1 gates are
// applied; other indices pass through unchanged to their GTA backends.
// samp.dll RVA: 0x000b34f0
void *GameTxdAddRef(int index) { return ((void*(__cdecl*)(int))0x731a00)(index); }
// samp.dll RVA: 0x000b3500
void GameTxdRemoveRef(int index) { ((void(__cdecl*)(int))0x731a30)(index); }
// samp.dll RVA: 0x000b35b0
void GameTxdRemove(int index) { if(index!=-1)((void(__cdecl*)(int))0x731e90)(index); }
// samp.dll RVA: 0x000b35d0
void GameTxdRemoveSlot(int index) { if(index!=-1)((void(__cdecl*)(int))0x731cd0)(index); }
// samp.dll RVA: 0x000b35f0
DWORD GameTxdGetReferenceCount(int index) { return index==-1?0xffffffffu:((DWORD(__cdecl*)(int))0x731aa0)(index); }

// Native performs name lookup, then removes the slot only when it is not -1.
// samp.dll RVA: 0x000b3580
void GameTxdRemoveSlotByName(const char *name) {
    int index=GameTxdFind(name);
    if(index!=-1)((void(__cdecl*)(int))0x731cd0)(index);
}

// GTA SA virtual addresses: USA7C7550, fallback7C7590. Exactly one argument.
// samp.dll RVA: 0x000b1ce0
void *GameRwGetGeometrySkin(void *geometry) {
    DWORD target=g_gameVersion==1?0x7c7550:0x7c7590;
    return ((void*(__cdecl*)(void*))target)(geometry);
}
// GTA SA virtual addresses: backend74BDD0/+50; initialization flag C97B20.
// Callback addresses are opaque GTA code pointers; no invented callable types.
// samp.dll RVA: 0x000b1da0
int GameRwRegisterClumpPlugin(DWORD pluginId) {
    DWORD target=g_gameVersion==1?0x74bdd0:0x74be20;
    *(DWORD*)0xc97b20=0;
    int result=((int(__cdecl*)(DWORD,DWORD,DWORD,DWORD,DWORD))target)(0,pluginId,0x41b1a0,0x41b1c0,0x41b1b0);
    *(DWORD*)0xc97b20=1;
    return result;
}
// GTA SA virtual addresses: backend74BE70/+50, callbacks41B2F0/41B300.
// Native forces the flag back to1, not to its previous value, even on failure.
// samp.dll RVA: 0x000b1e00
int GameRwRegisterClumpPluginStream(DWORD pluginId,DWORD readCallback) {
    DWORD target=g_gameVersion==1?0x74be70:0x74bec0;
    *(DWORD*)0xc97b20=0;
    int result=((int(__cdecl*)(DWORD,DWORD,DWORD,DWORD))target)(pluginId,readCallback,0x41b2f0,0x41b300);
    *(DWORD*)0xc97b20=1;
    return result;
}
// GTA SA virtual addresses: USA7F3560, fallback7F35A0; fixed enable argument1.
// samp.dll RVA: 0x000b1e60
void GameRwEnableAutoMipmapping() {
    DWORD target=g_gameVersion==1?0x7f3560:0x7f35a0;
    ((void(__cdecl*)(DWORD))target)(1);
}

// GTA SA virtual addresses: USA734820, all other version values734880 (+60).
// samp.dll RVA: 0x000b1ca0
void *GameRwGetFirstAtomic(void *clump) {
    DWORD target=g_gameVersion==1?0x734820:0x734880;
    return ((void*(__cdecl*)(void*))target)(clump);
}
// GTA SA virtual address: 0x00734B10, exactly one clump argument.
// samp.dll RVA: 0x000b1d40
void *GameRwGetClumpAnimHierarchy(void *clump) {
    return ((void*(__cdecl*)(void*))0x734b10)(clump);
}
// GTA SA virtual address: 0x004D60E0, exactly one hierarchy argument.
// samp.dll RVA: 0x000b1d70
void *GameRwCreateHierarchyAnimation(void *hierarchy) {
    return ((void*(__cdecl*)(void*))0x4d60e0)(hierarchy);
}

#include "game/patches.h"
#include "game/hooks.h"
#include "game/animation.h"
// samp.dll RVA: 0x000a0720
void CGame::StartGame() {
    GameApplyInGamePatches();GameInstallHooks();Scripting::Initialize();GameInitializeAnimationKeys();
    *(DWORD*)0xc8d4c0=8;*(BYTE*)0xba6831=1;*(BYTE*)0xba67a4=0;*(BYTE*)0xba677b=0;
}

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


// samp.dll RVA: 0x000b39b0
BOOL __stdcall GameHasModelInfo(int model) { return GameGetModelInfo(model)!=0; }
// samp.dll RVA: 0x000b39d0
BOOL __stdcall GameIsPedModel(int model) {
    if(model>=0 && model<=30000) {
        void *info=GameGetModelInfo(model);
        // GTA SA virtual vtable address, not a samp.dll RVA.
        if(info && *(DWORD*)info==0x85bdc0)return TRUE;
    }
    return FALSE;
}
// samp.dll RVA: 0x000b4170
BOOL __stdcall GameModelQueryB4170(int model) {
    void *info=GameGetModelInfo(model);
    return info && (*(DWORD*)info+0x14u)!=0;
}

#include "main.h"
// Vtable slot semantics remain unknown; do not relabel this as model type.
// Both successful lookup checks are native, including the second null guard.
// samp.dll RVA: 0x000b5d90
DWORD __cdecl GameQueryModelVirtual2c(int model) {
    if(!g_pGame || !GameGetModelInfo(model))return 0;
    void *info=GameGetModelInfo(model);
    if(!info)return 0;
    DWORD result;
    __asm {
        mov ecx,info
        mov edx,dword ptr [ecx]
        call dword ptr [edx + 0x2c]
        mov result,eax
    }
    return result;
}

// samp.dll RVA: 0x000b3d00
DWORD __cdecl GameReadPedModelTable28(int model) {
    if(model<0||model>30000) return 0;
    void *info=GameGetModelInfo(model);
    // GTA SA virtual address: observed model-info first-word identity0x0085bdc0.
    if(!info||*(DWORD*)info!=0x85bdc0) return 0;
    // Keep the second lookup and native extra pointer indirection unchecked.
    BYTE *table=*(BYTE**)GameGetModelInfo(model);
    return *(DWORD*)(table+0x28);
}

// samp.dll RVA: 0x000b3d40
DWORD __cdecl GameReadPedModelTable2c(int model) {
    if(model<0||model>30000) return 0;
    void *info=GameGetModelInfo(model);
    // GTA SA virtual address: observed model-info first-word identity0x0085bdc0.
    if(!info||*(DWORD*)info!=0x85bdc0) return 0;
    // Keep the second lookup and native extra pointer indirection unchecked.
    BYTE *table=*(BYTE**)GameGetModelInfo(model);
    return *(DWORD*)(table+0x2c);
}

// samp.dll RVA: 0x000b3d80
DWORD __cdecl GameReadPedModelTable24(int model) {
    if(model<0||model>30000) return 0;
    void *info=GameGetModelInfo(model);
    // GTA SA virtual address: observed model-info first-word identity0x0085bdc0.
    if(!info||*(DWORD*)info!=0x85bdc0) return 0;
    // Keep the second lookup and native extra pointer indirection unchecked.
    BYTE *table=*(BYTE**)GameGetModelInfo(model);
    return *(DWORD*)(table+0x24);
}

#include "game/entity.h"

// Preserve the second lookup: its result is deliberately not null-checked again.
// samp.dll RVA: 0x000b5ef0
float __cdecl GameSetModelCollisionRadius(int model,float radius) {
    if(GameGetModelInfo(model)) {
        BYTE *info=(BYTE*)GameGetModelInfo(model);
        BYTE *collision=*(BYTE**)(info+0x14);
        if(collision) {
            *(float*)(collision+0x24)=radius;
            return radius;
        }
    }
    return g_objectScaleLowerBound;
}
// Publish one raw DWORD at a time, preserving alias order and NaN payload bits.
// samp.dll RVA: 0x000b5f50
BOOL __cdecl GameGetModelCollisionCentre(int model,CVector *centre) {
    if(GameGetModelInfo(model)) {
        BYTE *info=(BYTE*)GameGetModelInfo(model);
        BYTE *collision=*(BYTE**)(info+0x14);
        if(collision) {
            *(DWORD*)&centre->x=*(DWORD*)(collision+0x18);
            *(DWORD*)&centre->y=*(DWORD*)(collision+0x1c);
            *(DWORD*)&centre->z=*(DWORD*)(collision+0x20);
            return TRUE;
        }
    }
    return FALSE;
}
// Native exposes only AL; no meaning is assigned to individual flag bits.
// samp.dll RVA: 0x000b5fe0
BYTE __stdcall GameGetModelFlagsByte(int model) {
    BYTE *info=(BYTE*)GameGetModelInfo(model);
    return info?*(BYTE*)(info+0x12):0;
}
// Native exposes AX; this reads the full overlapping two-byte field.
// samp.dll RVA: 0x000b6000
WORD __stdcall GameGetModelFlagsWord(int model) {
    BYTE *info=(BYTE*)GameGetModelInfo(model);
    return info?*(WORD*)(info+0x12):0;
}

// These helpers receive an existing backend model-info pointer, not a model ID.
// samp.dll RVA: 0x000b4150
void __stdcall GameSetModelDrawDistance(void *info,float distance) {
    *(float*)((BYTE*)info+0x18)=distance;
}
// samp.dll RVA: 0x000b4160
float __stdcall GameGetModelDrawDistance(const void *info) {
    return *(const float*)((const BYTE*)info+0x18);
}

// Native writes +1C before invoking the backend virtual slot. Its meaning
// remains unresolved; preserve the unchecked model lookup and callee-pop call.
// samp.dll RVA: 0x000b4200
void __stdcall GameSetModelRenderObjectCall3c(int model,void *object) {
    BYTE *info=(BYTE*)GameGetModelInfo(model);
    *(void**)(info+0x1c)=object;
    __asm {
        mov ecx,info
        push object
        mov edx,dword ptr [ecx]
        call dword ptr [edx+0x3c]
    }
}

// GTA SA virtual address: 0x004C4BC0; ECX=model info, initPairedModel=0.
// samp.dll RVA: 0x000b4230
void __stdcall GameSetModelCollision(int model,void *collision) {
    void *info=GameGetModelInfo(model);
    __asm {
        mov ecx,info
        push 0
        push collision
        mov edx,0x4c4bc0
        call edx
    }
}

// Native zero-extends the WORD, including FFFF -> 65535; lookup failure -> -1.
// samp.dll RVA: 0x000b4280
DWORD __stdcall GameGetModelTxdIndex(int model) {
    const BYTE *info=(const BYTE*)GameGetModelInfo(model);
    return info?*(const WORD*)(info+0xa):0xffffffffu;
}

// No null check is present in either of the following native entries.
// samp.dll RVA: 0x000b42d0
void *__stdcall GameGetModelCollision(int model) {
    BYTE *info=(BYTE*)GameGetModelInfo(model);
    return *(void**)(info+0x14);
}
// samp.dll RVA: 0x000b4300
WORD __stdcall GameGetModelReferenceCount(int model) {
    const BYTE *info=(const BYTE*)GameGetModelInfo(model);
    return *(const WORD*)(info+8);
}

// Native preserves the first model-info pointer across the second lookup made
// by the TXD-index helper. Backend call is ECX, no added failure cleanup.
// samp.dll RVA: 0x000b1c40
BOOL GameCleanupModelAndTxd(int model) {
    void *info=GameGetModelInfo(model);
    if(!info)return FALSE;
    int index=(int)GameGetModelTxdIndex(model);
    __asm {
        mov ecx,info
        mov edx,0x4c4d50
        call edx
    }
    if(GameTxdGetReferenceCount(index)==0) {
        GameTxdRemove(index);
        GameTxdRemoveSlot(index);
    }
    return TRUE;
}

// Clear the observed collision slot only; no destructor or release is native.
// samp.dll RVA: 0x000b1070
void GameClearModelCollision(int model) {
    BYTE *info=(BYTE*)GameGetModelInfo(model);
    if(info)*(void**)(info+0x14)=0;
}

// samp.dll RVA: 0x0011e68c
const Scripting::OpcodeInfo g_gameSetWorldTime={0xc0,"ii"};
// samp.dll RVA: 0x000a0980
void CGame::SetWorldWeather(int weather){
    BOOL clockEnabled=m_bClockEnabled;
    // GTA SA virtual addresses: 0x00c81318, 0x00c8131c, 0x00c81320.
    *(DWORD*)0xc81318=weather;
    if(!clockEnabled){*(DWORD*)0xc8131c=weather;*(DWORD*)0xc81320=weather;}
}
// samp.dll RVA: 0x000a08f0
void CGame::SetWorldTime(int hour,int minute){
    // GTA SA virtual addresses: minute 0x00b70152, hour 0x00b70153.
    *(BYTE*)0xb70152=(BYTE)minute;*(BYTE*)0xb70153=(BYTE)hour;
    Scripting::ProcessCommand(&g_gameSetWorldTime,hour,minute);
}

// samp.dll RVA: 0x000a0a20
void CGame::SetMaxStats(){
    ((void(__cdecl*)())0x4399d0)();((void(__cdecl*)())0x439940)();
    GameUnprotect((void*)0x55a070,1);*(BYTE*)0x55a070=0xc3;
}
// samp.dll RVA: 0x000a0940
void CGame::LetTimeGo(char enabled){
    GameUnprotect((void*)0x52cf10,1);
    if(enabled){*(BYTE*)0x52cf10=0x56;m_bClockEnabled=TRUE;}
    else{*(BYTE*)0x52cf10=0xc3;m_bClockEnabled=FALSE;}
}

// samp.dll RVA: 0x0011e410
const Scripting::OpcodeInfo g_cameraFade={0x16a,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011e424
const Scripting::OpcodeInfo g_cameraRestoreJumpcut={0x373,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011e438
const Scripting::OpcodeInfo g_cameraPointAt={0x160,{0x66,0x66,0x66,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011e44c
const Scripting::OpcodeInfo g_cameraRestore={0x2eb,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011e460
const Scripting::OpcodeInfo g_cameraSet={0x15f,{0x66,0x66,0x66,0x66,0x66,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011e474
const Scripting::OpcodeInfo g_cameraStop={0x925,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011e488
const Scripting::OpcodeInfo g_cameraMove={0x936,{0x66,0x66,0x66,0x66,0x66,0x66,0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011e49c
const Scripting::OpcodeInfo g_cameraTrack={0x920,{0x66,0x66,0x66,0x66,0x66,0x66,0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011e4b0
const Scripting::OpcodeInfo g_cameraPersistTrack={0x92f,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011e4c4
const Scripting::OpcodeInfo g_cameraPersistPosition={0x930,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011eaf0
const Scripting::OpcodeInfo g_audioPlay={0x18c,{0x66,0x66,0x66,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011eb04
const Scripting::OpcodeInfo g_audioLoad={0x3cf,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011eb40
const Scripting::OpcodeInfo g_audioUnload={0x40d,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011eb54
const Scripting::OpcodeInfo g_audioSetPosition={0x3d7,{0x69,0x66,0x66,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011eb18
const Scripting::OpcodeInfo g_audioIsLoaded={0x3d0,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011eb2c
const Scripting::OpcodeInfo g_audioStart={0x3d1,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f7a4
const Scripting::OpcodeInfo g_vehicleChangeInteriorCommand={0x840,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f678
const Scripting::OpcodeInfo g_vehicleIsSunkCommand={0x2bf,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f7b8
const Scripting::OpcodeInfo g_vehicleAttachTrailerCommand={0x893,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f7cc
const Scripting::OpcodeInfo g_vehicleDetachTrailerCommand={0x7ac,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f77c
const Scripting::OpcodeInfo g_vehicleRemoveComponentCommand={0x6e8,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f72c
const Scripting::OpcodeInfo g_vehiclePaintjobCommand={0x6ed,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f614
const Scripting::OpcodeInfo g_vehicleRotationCommand={0x175,{0x69,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f6a0
const Scripting::OpcodeInfo g_vehicleProofsCommand={0x2ac,{0x69,0x69,0x69,0x69,0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f790
const Scripting::OpcodeInfo g_vehicleDamageableCommand={0x53f,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f6b4
const Scripting::OpcodeInfo g_vehicleWarpPedCommand={0x362,{0x69,0x66,0x66,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f740
const Scripting::OpcodeInfo g_vehicleAddComponentCommand={0x6e7,{0x69,0x69,0x76,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f754
const Scripting::OpcodeInfo g_vehicleComponentLoadedCommand={0x6ea,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f768
const Scripting::OpcodeInfo g_vehicleRequestComponentCommand={0x6e9,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f628
const Scripting::OpcodeInfo g_vehicleCreateMarkerCommand={0x161,{0x69,0x69,0x69,0x76,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f63c
const Scripting::OpcodeInfo g_vehicleDestroyMarkerCommand={0x164,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f650
const Scripting::OpcodeInfo g_vehicleMarkerColorCommand={0x165,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f664
const Scripting::OpcodeInfo g_vehicleMarkerModeCommand={0x168,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f600
const Scripting::OpcodeInfo g_vehicleDestroyCommand={0xa6,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f6dc
const Scripting::OpcodeInfo g_vehicleDestroyTrainCommand={0x7bd,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f5ec
const Scripting::OpcodeInfo g_vehicleCreateCommand={0xa5,{0x69,0x66,0x66,0x66,0x76,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f6f0
const Scripting::OpcodeInfo g_vehicleGasTankExplosionCommand={0x9c4,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f718
const Scripting::OpcodeInfo g_vehicleHydraulicsCommand={0x7ff,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011f6c8
const Scripting::OpcodeInfo g_vehicleCreateTrainCommand={0x6d8,{0x69,0x66,0x66,0x66,0x69,0x76,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e790
const Scripting::OpcodeInfo g_gameDisableTrainTrafficCommand={0x6d7,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e7e0
const Scripting::OpcodeInfo g_gameRequestAnimationCommand={0x4ed,{0x73,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e7f4
const Scripting::OpcodeInfo g_gameIsAnimationLoadedCommand={0x4ee,{0x73,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e6b4
const Scripting::OpcodeInfo g_gameDeleteMarkerCommand={0x164,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e740
const Scripting::OpcodeInfo g_gameIncreaseMoneyCommand={0x109,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e86c
const Scripting::OpcodeInfo g_gameZoneDisplayCommand={0x9ba,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e754
const Scripting::OpcodeInfo g_gameFindGroundZCommand={0x2ce,{0x66,0x66,0x66,0x76,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e7cc
const Scripting::OpcodeInfo g_gameRefreshRendererCommand={0x4e4,{0x66,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e77c
const Scripting::OpcodeInfo g_gameDeleteRacingCheckpointCommand={0x6d6,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e6dc
const Scripting::OpcodeInfo g_gameCreateMarkerCommand={0x4ce,{0x66,0x66,0x66,0x69,0x76,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e718
const Scripting::OpcodeInfo g_gameCreateMarkerType1Command={0x2a8,{0x66,0x66,0x66,0x69,0x76,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e858
const Scripting::OpcodeInfo g_gameCreateMarkerType2Command={0x570,{0x66,0x66,0x66,0x69,0x76,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e704
const Scripting::OpcodeInfo g_gameCreateMarkerType3Command={0x2a7,{0x66,0x66,0x66,0x69,0x76,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e6c8
const Scripting::OpcodeInfo g_gameMarkerColorCommand={0x165,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e6f0
const Scripting::OpcodeInfo g_gameMarkerModeCommand={0x168,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e768
const Scripting::OpcodeInfo g_gameCreateRacingCheckpointCommand={0x6d5,{0x69,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x76,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e72c
const Scripting::OpcodeInfo g_gameLocateCheckpointCommand={0xfe,{0x69,0x66,0x66,0x66,0x66,0x66,0x66,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e81c
const Scripting::OpcodeInfo g_gameClearTextCommand={0xbe,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e830
const Scripting::OpcodeInfo g_gameCreatePickupCommand={0x213,{0x69,0x69,0x66,0x66,0x66,0x76,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e7a4
const Scripting::OpcodeInfo g_gameReleaseModelCommand={0x249,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011eae4
const char g_gameParachuteLibrary[10]={0x50,0x41,0x52,0x41,0x43,0x48,0x55,0x54,0x45,0x0};

// samp.dll RVA: 0x00118224
const char g_gameVendingLibrary[8]={0x56,0x45,0x4e,0x44,0x49,0x4e,0x47,0x0};

// samp.dll RVA: 0x0011eac4
const char g_gameFightBLibrary[8]={0x46,0x49,0x47,0x48,0x54,0x5f,0x42,0x0};

// samp.dll RVA: 0x0011eabc
const char g_gameFightCLibrary[8]={0x46,0x49,0x47,0x48,0x54,0x5f,0x43,0x0};

// samp.dll RVA: 0x0011eab4
const char g_gameFightDLibrary[8]={0x46,0x49,0x47,0x48,0x54,0x5f,0x44,0x0};

// samp.dll RVA: 0x0011eaac
const char g_gameFightELibrary[8]={0x46,0x49,0x47,0x48,0x54,0x5f,0x45,0x0};

#include "game/video_mode.h"
// GTA SA virtual address: 0x00745C70; no additional validation is native.
// samp.dll RVA: 0x000b5d10
void GameSetVideoMode(DWORD mode) {
    ((void(__cdecl*)(DWORD))0x745c70)(mode);
}

#include "game/frame_limit.h"
#include "game/game.h"
#include "game/playerped.h"
#include "game/patches.h"
#include <GetTime.h>

// samp.dll RVA: 0x0018e520
BYTE g_frameKeyCycle;
// samp.dll RVA: 0x0018e524
DWORD g_frameKeyCycleTick;
// samp.dll RVA: 0x0018e528
DWORD g_frameSampleCount;
// samp.dll RVA: 0x0018e52c
DWORD g_frameSampleTick;
// samp.dll RVA: 0x0018e530
DWORD g_frameSleepTick;
// samp.dll RVA: 0x0018e534
float g_frameScale;
// samp.dll RVA: 0x001181d8
const float g_frameScaleMultiplier=3.0f;
// samp.dll RVA: 0x0011c664
const float g_frameScaleNumerator=50.0f;

// samp.dll RVA: 0x0009fe50
void GameUpdateKeyCycle() {
    DWORD now=RakNet::GetTime();
    if(!g_frameKeyCycleTick){g_frameKeyCycleTick=now;return;}
    if((LONG)(now-g_frameKeyCycleTick)>30) {
        ++g_frameKeyCycle;
        g_frameKeyCycleTick=now;
        if(g_frameKeyCycle==5)g_frameKeyCycle=0;
    }
    *(BYTE*)0xb7356e=g_frameKeyCycle;
}
// samp.dll RVA: 0x0009fed0
void CGame::Sleep(int elapsed,int limit) {
    if(elapsed && limit) {
        int period=1000/limit;
        if(elapsed<period)::Sleep((DWORD)period-(DWORD)elapsed-1u);
    }
}
// samp.dll RVA: 0x000b3ca0
DWORD GameGetTaskType(void *task) {
    DWORD result=0;
    if(task) {
        DWORD table=*(DWORD*)task;
        if(table>=0x800000 && table<=0x900000) {
            __asm {
                pushad
                mov ecx,task
                mov edx,[ecx]
                call dword ptr [edx+0x10]
                mov result,eax
                popad
            }
        }
    }
    return result;
}
// samp.dll RVA: 0x000ac8f0
BOOL CPed::HasTaskType268() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(ped && !(*(DWORD*)(ped+0x46c)&0x100)) {
        void *task=*(void**)(*(BYTE**)(ped+0x47c)+0xc);
        if(task)return GameGetTaskType(task)==0x10c;
    }
    return FALSE;
}
// samp.dll RVA: 0x000a1a30
void CGame::ProcessFrameLimiter() {
    DWORD now=RakNet::GetTime();
    if(!g_frameSampleTick) {
        *(DWORD*)0xb7cb5c=0x3f800000;*(DWORD*)0xb7cb58=0x3f800000;
        g_gamePatchValue1546f0=0x40400000;
        g_frameSampleTick=RakNet::GetTime();g_frameSleepTick=g_frameSampleTick;return;
    }
    GameUpdateKeyCycle();++g_frameSampleCount;
    if((LONG)(now-g_frameSampleTick)>250) {
        LONG frames=(LONG)(g_frameSampleCount*4u-4u);
        // Preserve the unrounded quotient through publication and multiplication.
        __asm {
            cmp frames,16
            jle minimum_scale
            fild frames
            fdivr g_frameScaleNumerator
            jmp publish_scale
        minimum_scale:
            fld g_frameScaleMultiplier
        publish_scale:
            fst g_frameScale
            fld st(0)
            fxch st(1)
            fstp dword ptr ds:[0xb7cb5c]
            fst dword ptr ds:[0xb7cb58]
            fmul g_frameScaleMultiplier
            fstp g_gamePatchValue1546f0
        }
        g_frameSampleTick=RakNet::GetTime();g_frameSampleCount=0;
    }
    CPed *ped=GetPlayerPed();
    DWORD elapsed;
    if(ped && GetPlayerPed()->HasTaskType268()) {
        if(*(BYTE*)0xba6794 && (DWORD)m_nFrameLimiter<=45) {
            Sleep((int)(now-g_frameSleepTick),m_nFrameLimiter);
        } else {
            elapsed=now-g_frameSleepTick;
            if(elapsed && (LONG)elapsed<22)::Sleep(21u-elapsed);
        }
    } else {
        elapsed=now-g_frameSleepTick;
        if(!*(BYTE*)0xba6794) {
            if(elapsed && (LONG)elapsed<11)::Sleep(10u-elapsed);
        } else if(elapsed && m_nFrameLimiter) {
            int period=1000/m_nFrameLimiter;
            if((LONG)elapsed<period)::Sleep((DWORD)period-elapsed-1u);
        }
    }
    g_frameSleepTick=RakNet::GetTime();
}

// samp.dll RVA: 0x000a1bd0
void CGame::EnableHUD(BOOL enabled) {
    if(enabled){*(BYTE*)0xba6769=1;*(BYTE*)0xbaa3fb=0;}
    else {*(BYTE*)0xba6769=0;*(BYTE*)0xbaa3fb=1;}
}

#include "game/custom_model_io.h"
#include <stdio.h>
// samp.dll RVA: 0x000b2790
BOOL GameLoadTextureDictionary(const char *name,const char *filename,BYTE fromSamp) {
    char directory[64],path[136];
    strcpy(directory,fromSamp?"SAMP\\":"models\\txd");
    int index=GameTxdFind(name);if(index!=-1)return TRUE;
    index=GameTxdAdd(name);sprintf(path,"%s\\%s.txd",directory,filename);
    if(!GameFileExists(path))return FALSE;
    GameTxdLoad(index,path);GameTxdAddRef(index);return TRUE;
}

// samp.dll RVA: 0x0011e844
const Scripting::OpcodeInfo g_gameCreateWeaponPickupCommand={0x32b,"iiifffv"};
// samp.dll RVA: 0x000a10f0
DWORD CGame::CreateWeaponPickup(int model,int ammo,float x,float y,float z){
    int selected=model;
    if(selected>=0 && selected<=20000 && !Scripting::ProcessCommand(&g_gameModelAvailableCommand,selected)){
        Scripting::ProcessCommand(&g_gameRequestModelCommand,selected);
        Scripting::ProcessCommand(&g_gameLoadModelsCommand);
        void(WINAPI *volatile sleep)(DWORD)=::Sleep;
        while(!Scripting::ProcessCommand(&g_gameModelAvailableCommand,selected))sleep(5);
    }
    Scripting::ProcessCommand(&g_gameCreateWeaponPickupCommand,selected,4,ammo,(double)x,(double)y,(double)z,&model);
    return (DWORD)model;
}

// samp.dll RVA: 0x002abe58
BYTE g_gameLineSphereResult;
// samp.dll RVA: 0x000b3df0
int __cdecl GameCameraSphereQuery(const void *sphere) {
    void *camera=*(void**)0xc1703c;
    if(!camera)return 0;
    typedef int (__cdecl *Query)(void *,const void *);
    Query query=(Query)(g_gameVersion==1?0x7ee2d0:0x7ee310);
    return query(camera,sphere);
}
// samp.dll RVA: 0x000b6110
BYTE __stdcall GameTestEntityLineSphere(const CVector *origin,const CVector *end,void *backend,const CVector *centre,float scale,BYTE cameraCheck) {
    if(!backend || !*(void**)((BYTE*)backend+0x14))return false;
    float line[8]={0,0,0,0,0,0,0,0};
    BYTE *model=(BYTE*)GameGetModelInfo(*(short*)((BYTE*)backend+0x22));
    BYTE *collision=*(BYTE**)(model+0x14);
    if(!collision)return false;
    struct Sphere {CVector centre;float radius;};
    if(cameraCheck) {
        Sphere cameraSphere;cameraSphere.centre=*centre;
        cameraSphere.radius=*(float*)(collision+0x24);
        if(!GameCameraSphereQuery(&cameraSphere))return false;
    }
    line[0]=origin->x-centre->x;line[1]=origin->y-centre->y;line[2]=origin->z-centre->z;
    line[4]=end->x-centre->x;line[5]=end->y-centre->y;line[6]=end->z-centre->z;
    Sphere sphere;sphere.centre.x=0;sphere.centre.y=0;sphere.centre.z=0;
    // Keep the native unrounded radius comparison/multiplication sequence.
    const float half=0.5f,one=1.0f,enlarge=1.5f;
    float radius;
    __asm {
        mov eax,collision
        fld dword ptr [eax+0x24]
        fcom half
        fnstsw ax
        test ah,5
        jp radius_above_half
        fstp st(0)
        fld half
        jmp radius_enlarge
radius_above_half:
        fcom one
        fnstsw ax
        test ah,5
        jp radius_scale
radius_enlarge:
        fmul enlarge
radius_scale:
        fmul scale
        fstp radius
    }
    sphere.radius=radius;
    typedef BYTE (__cdecl *Intersect)(const void *,const void *);
    g_gameLineSphereResult=((Intersect)0x417470)(line,&sphere);
    return g_gameLineSphereResult;
}

#include "game/object.h"
// samp.dll RVA: 0x000a0170
CObject *CGame::NewObject(int model,float x,float y,float z,CVector rotation,float drawDistance,BYTE firstHour,BYTE lastHour) {
    CVector position;memcpy(&position.x,&x,4);memcpy(&position.y,&y,4);memcpy(&position.z,&z,4);
    return new CObject(model,position,rotation,drawDistance,0,firstHour,lastHour);
}

#include "game/ped_control.h"
// samp.dll RVA: 0x0011e808
const Scripting::OpcodeInfo g_gameReleaseAnimationCommand={0x4ef,"s"};
// samp.dll RVA: 0x0011e8e0
const float g_gameFarClipBase=1250.0f;
// samp.dll RVA: 0x0011e8dc
const float g_gameFarClipMinimum=700.0f;
// samp.dll RVA: 0x0015196c
float g_gameFarClipDistance=1400.0f;
// samp.dll RVA: 0x0011ea9c
const char g_gameWeaponName01[15]="Brass Knuckles";
// samp.dll RVA: 0x0011ea90
const char g_gameWeaponName02[10]="Golf Club";
// samp.dll RVA: 0x0011ea84
const char g_gameWeaponName03[11]="Nite Stick";
// samp.dll RVA: 0x0011ea7c
const char g_gameWeaponName04[6]="Knife";
// samp.dll RVA: 0x0011ea6c
const char g_gameWeaponName05[13]="Baseball Bat";
// samp.dll RVA: 0x0011ea64
const char g_gameWeaponName06[7]="Shovel";
// samp.dll RVA: 0x0011ea58
const char g_gameWeaponName07[9]="Pool Cue";
// samp.dll RVA: 0x0011ea50
const char g_gameWeaponName08[7]="Katana";
// samp.dll RVA: 0x0011ea44
const char g_gameWeaponName09[9]="Chainsaw";
// samp.dll RVA: 0x0011ea3c
const char g_gameWeaponName10[6]="Dildo";
// samp.dll RVA: 0x0011ea30
const char g_gameWeaponName12[9]="Vibrator";
// samp.dll RVA: 0x0011ea28
const char g_gameWeaponName14[8]="Flowers";
// samp.dll RVA: 0x0011ea20
const char g_gameWeaponName15[5]="Cane";
// samp.dll RVA: 0x0011ea18
const char g_gameWeaponName16[8]="Grenade";
// samp.dll RVA: 0x0011ea10
const char g_gameWeaponName17[8]="Teargas";
// samp.dll RVA: 0x0011ea08
const char g_gameWeaponName18[8]="Molotov";
// samp.dll RVA: 0x0011ea00
const char g_gameWeaponName22[8]="Colt 45";
// samp.dll RVA: 0x0011e9f0
const char g_gameWeaponName23[16]="Silenced Pistol";
// samp.dll RVA: 0x0011e9e0
const char g_gameWeaponName24[13]="Desert Eagle";
// samp.dll RVA: 0x0011e9d8
const char g_gameWeaponName25[8]="Shotgun";
// samp.dll RVA: 0x0011e9c4
const char g_gameWeaponName26[17]="Sawn-off Shotgun";
// samp.dll RVA: 0x0011e9b4
const char g_gameWeaponName27[15]="Combat Shotgun";
// samp.dll RVA: 0x0011e9b0
const char g_gameWeaponName28[4]="UZI";
// samp.dll RVA: 0x0011e9ac
const char g_gameWeaponName29[4]="MP5";
// samp.dll RVA: 0x0011e9a4
const char g_gameWeaponName30[5]="AK47";
// samp.dll RVA: 0x0011e9a0
const char g_gameWeaponName31[3]="M4";
// samp.dll RVA: 0x0011e998
const char g_gameWeaponName32[5]="TEC9";
// samp.dll RVA: 0x0011e990
const char g_gameWeaponName33[6]="Rifle";
// samp.dll RVA: 0x0011e980
const char g_gameWeaponName34[13]="Sniper Rifle";
// samp.dll RVA: 0x0011e970
const char g_gameWeaponName35[16]="Rocket Launcher";
// samp.dll RVA: 0x0011e964
const char g_gameWeaponName36[12]="Heat Seaker";
// samp.dll RVA: 0x0011e954
const char g_gameWeaponName37[13]="Flamethrower";
// samp.dll RVA: 0x0011e94c
const char g_gameWeaponName38[8]="Minigun";
// samp.dll RVA: 0x0011e938
const char g_gameWeaponName39[19]="Satchel Explosives";
// samp.dll RVA: 0x0011e930
const char g_gameWeaponName40[5]="Bomb";
// samp.dll RVA: 0x0011e924
const char g_gameWeaponName41[10]="Spray Can";
// samp.dll RVA: 0x0011e910
const char g_gameWeaponName42[18]="Fire Extinguisher";
// samp.dll RVA: 0x0011e908
const char g_gameWeaponName43[7]="Camera";
// samp.dll RVA: 0x0011e8fc
const char g_gameWeaponName46[10]="Parachute";
// samp.dll RVA: 0x0011e8f4
const char g_gameWeaponName49[8]="Vehicle";
// samp.dll RVA: 0x0011e8ec
const char g_gameWeaponName53[8]="Drowned";
// samp.dll RVA: 0x0011e8e4
const char g_gameWeaponName54[6]="Splat";

// samp.dll RVA: 0x000a0ad0
void CGame::ReleaseAnimation(const char *name) {
    if(Scripting::ProcessCommand(&g_gameIsAnimationLoadedCommand,name))
        Scripting::ProcessCommand(&g_gameReleaseAnimationCommand,name);
}
// samp.dll RVA: 0x000a0b00
void CGame::EnableRadar(BOOL enabled) {
    // GTA SA virtual address: radar-disable byte.
    *(BYTE*)0xbaa3fb=enabled==0;
}
// samp.dll RVA: 0x000a0d20
void CGame::UpdateFarClippingPlane() {
    BYTE *ped=(BYTE*)GameFindLocalPed();
    if(!ped){g_gameFarClipDistance=1250.0f;return;}
    DWORD interior;Scripting::ProcessCommand(&g_gameCurrentInteriorCommand,&interior);
    if((BYTE)interior){g_gameFarClipDistance=400.0f;return;}
    const float *height=(const float*)(*(BYTE**)(ped+0x14)+0x38);
    __asm {
        mov eax,height
        fld dword ptr [eax]
        fadd st(0),st(0)
        fsubr g_gameFarClipBase
        fst g_gameFarClipDistance
        fcomp g_gameFarClipMinimum
        fnstsw ax
        test ah,5
        jp far_clip_done
        mov g_gameFarClipDistance,442f0000h
far_clip_done:
    }
}
// samp.dll RVA: 0x000a0dc0
const char *CGame::GetWeaponName(int weapon) {
    switch(weapon) {
    case 1:return g_gameWeaponName01;
    case 2:return g_gameWeaponName02;
    case 3:return g_gameWeaponName03;
    case 4:return g_gameWeaponName04;
    case 5:return g_gameWeaponName05;
    case 6:return g_gameWeaponName06;
    case 7:return g_gameWeaponName07;
    case 8:return g_gameWeaponName08;
    case 9:return g_gameWeaponName09;
    case 10:case 11:return g_gameWeaponName10;
    case 12:case 13:return g_gameWeaponName12;
    case 14:return g_gameWeaponName14;
    case 15:return g_gameWeaponName15;
    case 16:return g_gameWeaponName16;
    case 17:return g_gameWeaponName17;
    case 18:return g_gameWeaponName18;
    case 19:case 20:case 21:case 44:case 45:case 47:case 48:case 50:case 51:case 52:return g_pedEmptyDanceAnimation;
    case 22:return g_gameWeaponName22;
    case 23:return g_gameWeaponName23;
    case 24:return g_gameWeaponName24;
    case 25:return g_gameWeaponName25;
    case 26:return g_gameWeaponName26;
    case 27:return g_gameWeaponName27;
    case 28:return g_gameWeaponName28;
    case 29:return g_gameWeaponName29;
    case 30:return g_gameWeaponName30;
    case 31:return g_gameWeaponName31;
    case 32:return g_gameWeaponName32;
    case 33:return g_gameWeaponName33;
    case 34:return g_gameWeaponName34;
    case 35:return g_gameWeaponName35;
    case 36:return g_gameWeaponName36;
    case 37:return g_gameWeaponName37;
    case 38:return g_gameWeaponName38;
    case 39:return g_gameWeaponName39;
    case 40:return g_gameWeaponName40;
    case 41:return g_gameWeaponName41;
    case 42:return g_gameWeaponName42;
    case 43:return g_gameWeaponName43;
    case 46:return g_gameWeaponName46;
    case 49:return g_gameWeaponName49;
    case 53:return g_gameWeaponName53;
    case 54:return g_gameWeaponName54;
    default:return g_pedEmptyDanceAnimation;
    }
}
// samp.dll RVA: 0x000a11d0
void CGame::Restart() {
    // GTA SA virtual addresses: restart flags and three no-argument backend calls.
    *(BYTE*)0xba67a5=1;*(BYTE*)0xba67a6=1;
    ((void(__cdecl*)())0x53c550)();
    ((void(__cdecl*)())0x561aa0)();
    ((void(__cdecl*)())0x53c680)();
    *(BYTE*)0xba6831=1;
}
// samp.dll RVA: 0x000a1250
void CGame::SetNumberOfIntroTextLinesThisFrame(WORD value) {*(WORD*)0xa44b68=value;}
// samp.dll RVA: 0x000a1590
int CGame::GetTimer() {return *(int*)0xb7cb84;}
// samp.dll RVA: 0x000a19e0
BOOL CGame::UsingGamepad() {return *(BYTE*)0xba6818!=0;}
