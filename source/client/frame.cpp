#include "client/frame.h"
#include "game/device_integrity.h"
#include "game/input.h"
#include "game/hooks_render.h"
#include "gui/device_objects.h"
#include "gui/render_dependencies.h"
#include <stdlib.h>
// samp.dll RVA: 0x002aca60
DWORD g_clientIntegrityFailures;
// samp.dll RVA: 0x002aca64
DWORD g_clientIntegrityFailed;
// samp.dll RVA: 0x002aca68
DWORD g_clientSkipGameFrame;
// samp.dll RVA: 0x002ac9c0
DWORD g_clientQuitRequested;
// samp.dll RVA: 0x002ac9c4
DWORD g_clientQuitStarted;
// samp.dll RVA: 0x000c5c10
DWORD __stdcall GameIntegrityMismatch(DWORD code,DWORD a,DWORD b,DWORD c,DWORD d,DWORD e,DWORD f) {
    if(g_pNetGame&&g_pGame) {
        GameUnprotect((void*)0x541df5,5);*(DWORD*)0x541df5=0x90909090;*(BYTE*)0x541df9=0x90;
        GameDisableMouseCalls();GameClearMouseDelta();GameUpdateMouse();
        GameUnprotect((void*)0x6194a0,1);*(BYTE*)0x6194a0=0xc3;
        ++g_clientIntegrityFailures;
    }
    return 69;
}
// samp.dll RVA: 0x000c6630
void ClientProcessFrame() {
    ClientInitializeDeviceAndGui();
    if(g_gameD3DDevice) {
        g_gameD3DDevice->SetRenderState(D3DRS_FOGENABLE,g_hookDisableFog==0);
        g_gameD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE,D3DFOG_NONE);
        g_gameD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);
    }
    if(g_pNetGame)((CNetGame*)g_pNetGame)->Process();
    if(g_renderAudioStream)g_renderAudioStream->ControlGameRadio();
    if(g_pGame)g_pGame->m_pAudio->Process();
}
// samp.dll RVA: 0x000c66b0
__declspec(naked) void ClientGameLoop() {
    __asm {
        push ebx
        push esi
        push edi
        pushad
        call ClientProcessFrame
        popad
        mov eax,g_clientSkipGameFrame
        xor edi,edi
        cmp eax,edi
        jne clear_skip
        mov edx,0x53e230
        call edx
        cmp g_clientSkipGameFrame,edi
        je after_game
    clear_skip:
        mov g_clientSkipGameFrame,edi
    after_game:
        pushad
        mov ecx,g_pGame
        cmp ecx,edi
        je after_text
        call CGame::IsMenuVisible
        test eax,eax
        jne reload_game
        mov eax,g_pNetGame
        cmp eax,edi
        je reload_game
        mov eax,[eax+0x3de]
        mov ecx,[eax+0x20]
        cmp ecx,edi
        je reload_game
        call CTextDrawPool::Draw
    reload_game:
        mov ecx,g_pGame
    after_text:
        cmp g_clientQuitRequested,edi
        je continue_game
        call dword ptr [GetTickCount]
        sub eax,g_clientQuitStarted
        cmp eax,1000
        jbe finish
        mov ecx,g_pNetGame
        cmp ecx,edi
        je exit_game
        mov esi,ecx
        call CNetGame::Destroy
        push esi
        call free
        add esp,4
        mov g_pNetGame,edi
    exit_game:
        push edi
        call dword ptr [ExitProcess]
    finish:
        popad
        pop edi
        pop esi
        pop ebx
        ret
    continue_game:
        cmp ecx,edi
        je check_failures
        call CGame::ProcessInputEnabling
    check_failures:
        cmp g_clientIntegrityFailures,10
        jbe finish
        mov g_clientIntegrityFailed,1
        jmp finish
    }
}
