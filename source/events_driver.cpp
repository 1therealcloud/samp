#include "game/patches.h"
#include "main.h"
#include "game/playerped.h"
#include "events_driver.h"
// samp.dll RVA: 0x00156980
CPed *g_driverPed;
// samp.dll RVA: 0x00156998
BOOL g_driverCompareFlag;
// samp.dll RVA: 0x0015699c
void *g_driverScriptThread;
// samp.dll RVA: 0x00001c60
__declspec(naked) void GamePlayerDriverOpcode() {
    __asm {
        pushad
        mov g_driverScriptThread,esi
        mov g_driverCompareFlag,1
        cmp g_pGame,0
        je compare
        mov ecx,g_pGame
        call CGame::GetPlayerPed
        mov g_driverPed,eax
        mov ecx,g_driverPed
        call CPed::IsInVehicle
        test eax,eax
        je compare
        mov ecx,g_driverPed
        call CPed::IsPassenger
        test eax,eax
        jne compare
        mov g_driverCompareFlag,0
    compare:
        push g_driverCompareFlag
        mov ecx,g_driverScriptThread
        mov ebx,0x4859d0
        call ebx
        popad
        mov ebx,0x47bf2b
        jmp ebx
    }
}
