#include "game/ped_control.h"
#include "game/game.h"
#include "game/input.h"
#include "game/scripting.h"
#include "main.h"
#include "chatwindow.h"
#include <string.h>
// samp.dll RVA: 0x0018e8c0
DWORD g_hookPedControlField540;
// samp.dll RVA: 0x0018f378
CPlayerPool *g_hookFaultPlayerPool;
// samp.dll RVA: 0x00151970
WORD g_hookFaultPlayerId=0xffff;
// samp.dll RVA: 0x0018e824
CRemotePlayer *g_hookFaultRemotePlayer;
// samp.dll RVA: 0x00151a70
const BYTE g_hookPedRestoreA[6]={0xd9,0x96,0x5c,0x05,0,0};
// samp.dll RVA: 0x00151a88
const BYTE g_hookPedRestoreB[6]={0xd9,0x96,0x5c,0x05,0,0};
// samp.dll RVA: 0x00151a90
const BYTE g_hookPedRemoteRestore[5]={0xe8,0x17,0x88,0xff,0xff};
// samp.dll RVA: 0x0011e7b8
const Scripting::OpcodeInfo g_gameCurrentInteriorCommand={0x77e,"v"};
// samp.dll RVA: 0x0011ebcc
const char g_hookPedExceptionLabel[10]="playerped";
// samp.dll RVA: 0x0011ebac
const char g_hookPedRemovedMessage[32]="Removed player %u due to error.";
// samp.dll RVA: 0x000a0d00
BYTE CGame::GetCurrentInterior() {
    DWORD interior;Scripting::ProcessCommand(&g_gameCurrentInteriorCommand,&interior);
    return (BYTE)interior;
}
// samp.dll RVA: 0x000a2530
void GameRunPedControlBackend() {
    __try {
        // GTA SA thiscall backend, ECX=the current saved ped.
        ((void(__fastcall*)(void*,void*))0x60ea90)((void*)g_hookCameraPed,0);
    } __except(GameExceptionFilter(GetExceptionCode(),GetExceptionInformation(),g_hookPedExceptionLabel)) {
        if(*g_clientCrashPedContext && g_pNetGame) {
            g_hookFaultPlayerPool=*(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+8);
            if(g_hookFaultPlayerPool) {
                WORD id=g_hookFaultPlayerPool->Find((void*)g_hookCameraPed);
                g_hookFaultPlayerId=id;
                if(id!=0xffff) {
                    g_hookFaultRemotePlayer=g_hookFaultPlayerPool->GetPlayer(id);
                    if(g_hookFaultRemotePlayer) {
                        g_hookFaultRemotePlayer->Remove();id=g_hookFaultPlayerId;
                    }
                    if(g_pChat)g_pChat->AddDebugMessage(g_hookPedRemovedMessage,(unsigned)id);
                }
            }
        }
    }
}
// Extracted readable body; outer native entry preserves PUSHAD state.
// samp.dll RVA: unknown (reconstruction-only body helper extracted from A26D0; no standalone native entry)
void GameProcessPedControlBody() {
    g_hookCollisionPed=g_hookCameraPed;
    g_hookSavedPlayer=*g_clientCrashPedContext;
    g_hookPlayerIndex=GameFindPlayerPedIndex((void*)g_hookCameraPed);
    if(g_hookCameraPed && g_hookPlayerIndex && !g_hookSavedPlayer) {
        KeyStuff::UpdateKeys();KeyStuff::ApplyPlayerKeys(g_hookPlayerIndex);
        g_hookSavedCameraMode=*g_aimInternalCameraMode;
        *g_aimInternalCameraMode=AimStuff::GetPlayerCameraMode(g_hookPlayerIndex);
        g_hookSavedCameraModeWord=*g_hookCameraModeWord;
        *g_hookCameraModeWord=AimStuff::GetPlayerCameraMode(g_hookPlayerIndex);
        if(*g_hookCameraModeWord==4)*g_hookCameraModeWord=0;
        AimStuff::UpdateCamera();AimStuff::ApplyPlayerCamera(g_hookPlayerIndex);
        AimStuff::UpdateAim();AimStuff::ApplyPlayerAim(g_hookPlayerIndex);
        AimStuff::UpdateLocalSkills();AimStuff::ApplyPlayerSkills(g_hookPlayerIndex);
        *g_clientCrashPedContext=g_hookPlayerIndex;
        g_hookPedControlField540=*(DWORD*)(g_hookCollisionPed+0x540);
        GameUnprotect((void*)0x5e92f4,5);
        *(DWORD*)0x5e92f4=0x90909090;*(BYTE*)0x5e92f8=0x90;
        GameRunPedControlBackend();
        memcpy((void*)0x5e92f4,g_hookPedRemoteRestore,5);
        AimStuff::ApplyLocalSkills();
        *g_aimInternalCameraMode=g_hookSavedCameraMode;
        *g_hookCameraModeWord=g_hookSavedCameraModeWord;
        AimStuff::ApplyLocalCamera();*g_clientCrashPedContext=0;
        KeyStuff::ApplyLocalKeys();AimStuff::ApplyLocalAim();
    } else {
        if(g_pNetGame && *(DWORD*)(*(BYTE**)(*(BYTE**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+8)+0x1e)+0xf4)) {
            BYTE *settings=*(BYTE**)((BYTE*)g_pNetGame+0x3d5);
            float positiveX=*(float*)(settings+5),negativeX=*(float*)(settings+9);
            float positiveY=*(float*)(settings+0xd),negativeY=*(float*)(settings+0x11);
            if(g_pGame->GetPlayerPed()->HasExceededWorldBoundries(positiveX,negativeX,positiveY,negativeY) &&
                !g_pGame->GetCurrentInterior() && !(*(DWORD*)(g_hookCollisionPed+0x46c)&0x100) &&
                (*(DWORD*)(g_hookCollisionPed+0x46c)&3)) {
                g_hookVehicleKeys=(DWORD)KeyStuff::GetInternalKeys();
                *(WORD*)(g_hookVehicleKeys+0x1c)=0xff;*(WORD*)(g_hookVehicleKeys+0x4c)=0;
            }
        }
        if(g_pGame->GetPlayerPed()) {
            if(g_pGame->GetPlayerPed()->unknown2f7) {
                g_hookVehicleKeys=(DWORD)KeyStuff::GetInternalKeys();
                *(WORD*)(g_hookVehicleKeys+0x24)=0;*(WORD*)(g_hookVehicleKeys+0x54)=0;
            }
            if(g_pGame->GetPlayerPed()->unknown2f6) {
                g_hookVehicleKeys=(DWORD)KeyStuff::GetInternalKeys();
                *(WORD*)(g_hookVehicleKeys+0x1c)=0;*(WORD*)(g_hookVehicleKeys+0x4c)=0;
            }
            if(g_pGame->GetPlayerPed()->unknown2f8) {
                g_hookVehicleKeys=(DWORD)KeyStuff::GetInternalKeys();
                *(WORD*)(g_hookVehicleKeys+0x20)=0;*(WORD*)(g_hookVehicleKeys+0x50)=0;
            }
        }
        memcpy((void*)0x6884c4,g_hookPedRestoreA,6);memcpy((void*)0x688200,g_hookPedRestoreB,6);
        ((void(__fastcall*)(void*,void*))0x60ea90)((void*)g_hookCameraPed,0);
        memset((void*)0x6884c4,0x90,6);memset((void*)0x688200,0x90,6);
    }
}
// Raw entry: ECX=GTA ped; original preserves all PUSHAD registers and returns.
// samp.dll RVA: 0x000a26d0
__declspec(naked) void GameHook_000a26d0() {
    __asm {
        mov g_hookCameraPed,ecx
        pushad
        call GameProcessPedControlBody
        popad
        ret
    }
}
