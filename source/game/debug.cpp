#include "game/debug.h"
#include "game/game.h"
#include "game/vehicle.h"
#include "game/playerped.h"
#include "game/keystuff.h"
#include "gui/render_dependencies.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
// samp.dll RVA: 0x001518d4
DWORD g_debugSelectedVehicleModel=400;
// samp.dll RVA: 0x0018dd48
KeyPad *g_debugVehicleKeys;
// samp.dll RVA: 0x0018dd4c
CCamera *g_debugVehicleCamera;
// samp.dll RVA: 0x0018de30
DWORD g_debugVehicleSelectionActive;
// samp.dll RVA: 0x0018de34
CVehicle *g_debugSelectedVehicle;
// samp.dll RVA: 0x0018ddec
DWORD g_debugSelectionFlagA;
// samp.dll RVA: 0x0018ddf0
DWORD g_debugSelectionFlagB;
// samp.dll RVA: 0x0011e530
const char g_debugVehicleSelectText[15]="Vehicle Select";
// samp.dll RVA: 0x0011c198
const char g_debugVehicleTypeFormat[8]="TYPE_%d";
// samp.dll RVA: 0x0009dee0
void GameSetDebugState(DWORD first,DWORD second,DWORD stage) {
    g_gameDebugStage=stage;g_debugSelectionFlagA=first;g_debugSelectionFlagB=second;
}
// samp.dll RVA: 0x0009df00
void GameClearDebugState() {g_gameDebugStage=0;}

// samp.dll RVA: 0x0009df10
void GameRebuildDebugVehicle() {
    if(g_debugSelectedVehicle)delete g_debugSelectedVehicle;
    g_debugSelectedVehicle=g_pGame->CreateVehicle(g_debugSelectedVehicleModel,5,5,500,0,FALSE);
}
// samp.dll RVA: 0x0009df50
void GameDrawDebugEntities() {
    if(!g_debugVehicleSelectionActive) {
        g_debugVehicleKeys=KeyStuff::GetInternalKeys();
        g_debugVehicleCamera=g_pGame->m_pCamera;
        CVector position={-4,-4,502},rotation={0,0,0};
        g_debugVehicleCamera->Set(position,rotation);
        CVector point={5,5,500};g_debugVehicleCamera->PointAt(point,1);
        g_pGame->GetPlayerPed()->SetControllable(FALSE);
        g_pGame->DisplayGameText(g_debugVehicleSelectText,4000,6);
        GameRebuildDebugVehicle();
        g_debugVehicleSelectionActive=1;
    }
    g_pGame->EnableHUD(FALSE);
    CVehicle *preview=g_debugSelectedVehicle;
    if(preview&&preview->m_pGameEntity) {
        CVector turn={0,0,0.03f},speed={0,0,0};
        preview->SetTurnSpeed(turn);
        g_debugSelectedVehicle->SetSpeed(speed);
        CMatrix matrix;g_debugSelectedVehicle->GetMatrix(&matrix);
        CVector position={5,5,500};memcpy(&matrix.position,&position,12);
        g_debugSelectedVehicle->SetMatrix(matrix);
        preview=g_debugSelectedVehicle;
    }
    WORD *keys=(WORD*)g_debugVehicleKeys;
    if(keys[0x1c/2]&&!keys[0x4c/2]) {
        --g_debugSelectedVehicleModel;
        if(g_debugSelectedVehicleModel==538)g_debugSelectedVehicleModel=536;
        else if(g_debugSelectedVehicleModel==399)g_debugSelectedVehicleModel=611;
        GameRebuildDebugVehicle();return;
    }
    if(keys[0x20/2]&&!keys[0x50/2]) {
        ++g_debugSelectedVehicleModel;
        if(g_debugSelectedVehicleModel==537)g_debugSelectedVehicleModel=539;
        else if(g_debugSelectedVehicleModel==612)g_debugSelectedVehicleModel=400;
        GameRebuildDebugVehicle();return;
    }
    if(keys[0x1e/2]&&!keys[0x4e/2]) {
        if(preview)delete preview;
        g_debugSelectedVehicle=0;
        g_debugVehicleCamera->Restore();
        g_gameDebugStage=0;g_debugSelectionFlagA=0;g_debugSelectionFlagB=0;
        CPed *ped=g_pGame->GetPlayerPed();
        if(ped) {
            CMatrix matrix;ped->GetMatrix(&matrix);
            // Native still formats this unused temporary model label.
            char type[9]={0};sprintf(type,g_debugVehicleTypeFormat,g_debugSelectedVehicleModel);
            CVehicle *created=g_pGame->CreateVehicle(g_debugSelectedVehicleModel,
                matrix.position.x-5.0f,matrix.position.y-5.0f,matrix.position.z+1.0f,0,FALSE);
            if(created)ped->PutIntoVehicle(created->m_nHandle,0);
        }
        g_debugVehicleCamera->Detach();g_debugVehicleCamera->Restore();
        g_pGame->GetPlayerPed()->SetControllable(TRUE);
        g_pGame->EnableHUD(TRUE);g_debugVehicleSelectionActive=0;
    }
}
// samp.dll RVA: 0x000b3a00
DWORD __stdcall GameCountUnusedModelSlots() {
    DWORD count=0;
    // GTA SA virtual addresses: native model slots1000..19999.
    for(DWORD address=0xa9c068;address<0xaae948;address+=4)
        if(!*(DWORD*)address)++count;
    return count;
}
