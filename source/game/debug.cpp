#include "game/debug.h"

#include "game/game.h"
#include "game/hooks.h"
#include "game/keystuff.h"
#include "game/playerped.h"
#include "game/util.h"
#include "game/vehicle.h"
#include "gui/chatwindow.h"
#include "gui/gui.h"
#include "main.h"

#include <string.h>
// 0x0011f56c
const char g_taskNoneName[5] = "None";
// 0x001518d4
DWORD g_debugSelectedVehicleModel = 400;
// 0x0018dd48
KeyPad* g_debugVehicleKeys;
// 0x0018dd4c
CCamera* g_debugVehicleCamera;
// 0x0018de30
DWORD g_debugVehicleSelectionActive;
// 0x0018de34
CVehicle* g_debugSelectedVehicle;
// 0x0018ddec
DWORD g_debugSelectionFlagA;
// 0x0018ddf0
DWORD g_debugSelectionFlagB;
// 0x0011e530
const char g_debugVehicleSelectText[15] = "Vehicle Select";
// 0x0018dc20
BYTE g_nativeDebugFlag18DC20 = 0;
// 0x0011f594
const char g_debugRasterMissingText[] = "No texture!";
// 0x000b3cf0
const char* GameGetTaskName(void* task)
{
    (void)task;
    return g_taskNoneName;
}

// 0x000b3ca0
DWORD GameGetTaskType(void* task)
{
    DWORD result = 0;
    if (task)
    {
        DWORD table = *(DWORD*)task;
        if (table >= 0x800000 && table <= 0x900000)
        {
            // clang-format off
            __asm {
                pushad
                mov edx,task
                mov ebx,[edx]
                mov edx,[ebx+0x10]
                mov ecx,task
                call edx
                mov result,eax
                popad
            }
            // clang-format on
            return result;
        }
    }
    return 0;
}

// 0x000b3c50
void* GameGetTaskChild(void* task)
{
    void* result = 0;
    if (task)
    {
        DWORD table = *(DWORD*)task;
        if (table >= 0x800000 && table <= 0x900000)
        {
            // clang-format off
            // GTA SA virtual interface: preserve caller registers across the call.
            __asm {
                pushad
                mov edx,task
                mov ebx,[edx]
                mov edx,[ebx+8]
                mov ecx,task
                call edx
                mov result,eax
                popad
            }
            // clang-format on
            return result;
        }
    }
    return 0;
}

// 0x0009dee0
void GameSetDebugState(DWORD first, DWORD second, DWORD stage)
{
    g_gameDebugStage = stage;
    g_debugSelectionFlagA = first;
    g_debugSelectionFlagB = second;
}

// 0x0009df00
void GameClearDebugState()
{
    g_gameDebugStage = 0;
}

// 0x0009df10
void GameRebuildDebugVehicle()
{
    if (g_debugSelectedVehicle)
        delete g_debugSelectedVehicle;
    g_debugSelectedVehicle = g_pGame->CreateVehicle(g_debugSelectedVehicleModel, 5, 5, 500, 0, FALSE);
}

// 0x0009df50
void GameDrawDebugEntities()
{
    if (!g_debugVehicleSelectionActive)
    {
        g_debugVehicleKeys = KeyStuff::GetInternalKeys();
        g_debugVehicleCamera = g_pGame->m_pCamera;
        CVector position = {-4, -4, 502}, rotation = {0, 0, 0};
        g_debugVehicleCamera->Set(position, rotation);
        CVector point = {5, 5, 500};
        g_debugVehicleCamera->PointAt(point, 1);
        g_pGame->GetPlayerPed()->SetControllable(FALSE);
        g_pGame->DisplayGameText(g_debugVehicleSelectText, 4000, 6);
        GameRebuildDebugVehicle();
        g_debugVehicleSelectionActive = 1;
    }
    g_pGame->EnableHUD(FALSE);
    CVehicle* preview = g_debugSelectedVehicle;
    if (preview && preview->m_pGameEntity)
    {
        CVector turn = {0, 0, 0.03f}, speed = {0, 0, 0};
        preview->SetTurnSpeed(turn);
        g_debugSelectedVehicle->SetSpeed(speed);
        CMatrix matrix;
        g_debugSelectedVehicle->GetMatrix(&matrix);
        CVector position = {5, 5, 500};
        memcpy(&matrix.position, &position, 12);
        g_debugSelectedVehicle->SetMatrix(matrix);
        preview = g_debugSelectedVehicle;
    }
    WORD* keys = (WORD*)g_debugVehicleKeys;
    if (keys[0x1c / 2] && !keys[0x4c / 2])
    {
        --g_debugSelectedVehicleModel;
        if (g_debugSelectedVehicleModel == 538)
            g_debugSelectedVehicleModel = 536;
        else if (g_debugSelectedVehicleModel == 399)
            g_debugSelectedVehicleModel = 611;
        GameRebuildDebugVehicle();
        return;
    }
    if (keys[0x20 / 2] && !keys[0x50 / 2])
    {
        ++g_debugSelectedVehicleModel;
        if (g_debugSelectedVehicleModel == 537)
            g_debugSelectedVehicleModel = 539;
        else if (g_debugSelectedVehicleModel == 612)
            g_debugSelectedVehicleModel = 400;
        GameRebuildDebugVehicle();
        return;
    }
    if (keys[0x1e / 2] && !keys[0x4e / 2])
    {
        if (preview)
            delete preview;
        g_debugSelectedVehicle = 0;
        g_debugVehicleCamera->Restore();
        g_gameDebugStage = 0;
        g_debugSelectionFlagA = 0;
        g_debugSelectionFlagB = 0;
        CPed* ped = g_pGame->GetPlayerPed();
        if (ped)
        {
            CMatrix matrix;
            ped->GetMatrix(&matrix);
            CVehicle* created = g_pGame->CreateVehicle(g_debugSelectedVehicleModel, matrix.position.x - 5.0f, matrix.position.y - 5.0f, matrix.position.z + 1.0f, 0, FALSE);
            if (created)
                ped->PutIntoVehicle(created->m_nHandle, 0);
        }
        g_debugVehicleCamera->Detach();
        g_debugVehicleCamera->Restore();
        g_pGame->GetPlayerPed()->SetControllable(TRUE);
        g_pGame->EnableHUD(TRUE);
        g_debugVehicleSelectionActive = 0;
    }
}

// 0x000b3a00
DWORD __stdcall GameCountUnusedModelSlots()
{
    DWORD count = 0;
    // GTA SA virtual addresses: native model slots1000..19999.
    for (DWORD address = 0xa9c068; address != 0xaae948; address += 4)
        if (!*(DWORD*)address)
            ++count;
    return count;
}

// 0x0009d3a0
void __stdcall GameSetNativeDebugFlag18DC20(BYTE value)
{
    g_nativeDebugFlag18DC20 = value;
}

// Ignores GTA result and returns raw collision-position Z, even on failure.
// 0x0009dc80
float GameDebugGroundHeight(const CVector* position)
{
    CVector end;
    memcpy(&end, position, 8);
    end.z = -100.0f;
    BYTE collision[52];
    GameProcessLineOfSightRaw(position, &end, collision, (void**)&position, 1, 0, 0, 1, 1, 1, 0, 0);
    return *(float*)(collision + 8);
}

// 0x002abe40
#define g_debugRasterSource (*(void**)(g_entityCollisionScratch + 0x1028))
// 0x002abe44
#define g_debugRasterResult (*(void**)(g_entityCollisionScratch + 0x102c))
// 0x000b5610
void GameDebugBindRasterResult()
{
    void* volatile& source = g_debugRasterSource;
    void* volatile& result = g_debugRasterResult;
    source = *(void**)0xc402d8;
    result = ((void*(__cdecl*)(void*))0x7f37c0)(source);
    if (!result)
        g_pChat->AddDebugMessage(g_debugRasterMissingText);
    // Native live result reloads: raw GTA globals, including publication of NULL.
    *(void**)0xb4e690 = result;
    *(void**)0xb4e68c = result;
    *(void**)0xb4e47c = result;
    *(void**)0xb4e3ec = result;
    *(DWORD*)0x8a7780 = 0x40000000;
}
