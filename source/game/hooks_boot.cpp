#include "game/hooks_boot.h"
#include "game/hooks_render.h"
#include "game/game.h"
#include "game/playerped.h"
#include "game/aimstuff.h"
#include "net/playerpool.h"
#include "main.h"

// samp.dll RVA: 0x0018f514
BYTE g_hookCameraHudPrepared;
// samp.dll RVA: 0x0018f6a8
DWORD g_hookAudioSavedState;
// samp.dll RVA: 0x0018f6ac
DWORD g_hookAudioArgument0;
// samp.dll RVA: 0x0018f6b0
DWORD g_hookAudioArgument1;

// Raw GTA entry ABIs: preserve live registers and native CALL/JMP/RET behavior.
// samp.dll RVA: 0x000a22f0
__declspec(naked) void GameHook_000a22f0() {
    __asm {
        pushad
        mov ecx,g_pGame
        test ecx,ecx
        mov g_hookCameraHudPrepared,0
        jz draw
        call CGame::GetPlayerPed
        test eax,eax
        jz draw
        mov ecx,g_pGame
        call CGame::GetPlayerPed
        mov al,[eax+0x2b0]
        test al,al
        jnz player_mode
        call AimStuff::GetCameraMode
        jmp check_mode
    player_mode:
        push eax
        call AimStuff::GetPlayerCameraMode
    check_mode:
        cmp al,0x35
        jne draw
        call GamePrepareHudDraw
        mov g_hookCameraHudPrepared,1
    draw:
        popad
        mov edx,0x58e020
        call edx
        pushad
        mov al,g_hookCameraHudPrepared
        test al,al
        jz finish
        mov eax,g_hookHudScaleX
        mov dword ptr ds:[0x859520],eax
        mov ecx,g_hookHudScaleY
        mov dword ptr ds:[0x859524],ecx
        mov g_hookCameraHudPrepared,0
    finish:
        popad
        ret
    }
}
// samp.dll RVA: 0x000a2370
__declspec(naked) void GameHook_000a2370() {
    __asm {
        mov edx,0x563430
        call edx
        pushad
        mov eax,g_pNetGame
        test eax,eax
        jz finish
        mov eax,[eax+0x3de]
        mov ecx,[eax+8]
        test ecx,ecx
        jz finish
        call CPlayerPool::Native013840
    finish:
        popad
        ret
    }
}
// samp.dll RVA: 0x000a23a0
__declspec(naked) void GameHook_000a23a0() {
    __asm {
        pushad
        cmp g_pGame,0
        je continue_game
        mov ecx,g_pGame
        call CGame::GetCamera
        mov ecx,eax
        call CCamera::SetToOwner
    continue_game:
        popad
        mov eax,0x52b730
        jmp eax
    }
}
// samp.dll RVA: 0x000a5b90
__declspec(naked) void GameHook_000a5b90() {
    __asm {
        mov edx,[esp+4]
        mov g_hookAudioArgument0,edx
        mov edx,[esp+8]
        mov g_hookAudioArgument1,edx
        mov eax,dword ptr ds:[0xb72914]
        mov g_hookAudioSavedState,eax
        mov eax,g_pGame
        test eax,eax
        jz play
        mov ecx,[eax]
        mov al,[ecx+4]
        test al,al
        jz play
        mov dword ptr ds:[0xb72914],1
    play:
        push g_hookAudioArgument1
        push g_hookAudioArgument0
        mov edx,0x505a00
        call edx
        mov edx,g_hookAudioSavedState
        mov dword ptr ds:[0xb72914],edx
        ret 8
    }
}
