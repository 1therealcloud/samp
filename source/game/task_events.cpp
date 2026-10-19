#include "game/task_events.h"
#include "main.h"
#include "game/playerped.h"
#include "gui/render_dependencies.h"
#include "net/playerpool.h"
#include "client/commands.h"
// samp.dll RVA: 0x0018f4a8
DWORD g_hookIgnoreNextExit;
// samp.dll RVA: 0x0018e550
DWORD g_hookExitTask;
// samp.dll RVA: 0x0018e5d4
CVehiclePool *g_hookEventVehiclePool;
// samp.dll RVA: 0x0018e568
WORD g_hookEventVehicleId;
// samp.dll RVA: 0x0018e868
CVehicle *g_hookExitVehicle;
// samp.dll RVA: 0x0018eb18
CLocalPlayer *g_hookEventLocalPlayer;
// samp.dll RVA: 0x0018eb30
CMatrix g_hookExitMatrix;
// samp.dll RVA: 0x00151aa0
DWORD g_hookExitTaskArgument=0xffffffff;
// samp.dll RVA: 0x0011c65c
const float g_hookTrainExitOffset=2.5f;
// samp.dll RVA: 0x0018e5bc
DWORD g_hookQuitDeadline;
// samp.dll RVA: 0x0018f538
DWORD g_hookDamageResponse;
// samp.dll RVA: 0x0018f53c
DWORD g_hookDamagePed;
// samp.dll RVA: 0x00001180
CVehiclePool *CNetGame::GetVehiclePool() { return *(CVehiclePool**)(*(BYTE**)(nativeState+0x3de)+0xc); }
// samp.dll RVA: 0x00001170
CPlayerPool *CNetGame::GetPlayerPool() { return *(CPlayerPool**)(*(BYTE**)(nativeState+0x3de)+8); }
// samp.dll RVA: 0x00001a80
CLocalPlayer *CPlayerPool::GetLocalPlayer() { return *(CLocalPlayer**)((BYTE*)this+0x1e); }
// samp.dll RVA: 0x00002da0
CPed *CLocalPlayer::GetPed() { return *(CPed**)this; }
// samp.dll RVA: 0x000b6fa0
BOOL CVehicle::IsTrainPart() {
    if(!m_pGameVehicle)return FALSE;
    short model=*(short*)((BYTE*)m_pGameVehicle+0x22);
    return model==0x21a || model==0x23a || model==0x219 || model==0x239 || model==0x1c1;
}
// Raw GTA task ABI: continuation jumps and original constructor stack words.
// samp.dll RVA: 0x000a3580
__declspec(naked) void GameHook_000a3580() {
    __asm {
        mov eax,[esp]
        mov g_hookAnimationReturn,eax
        mov eax,[esp+4]
        mov g_hookControlVehicle,eax
        pushad
        cmp g_hookAnimationReturn,0x570a20
        je notify
        cmp g_hookAnimationReturn,0x570a99
        jne original
    notify:
        call GameNotifyEnterVehicle
        test eax,eax
        jne original
        popad
        mov ebx,[ecx]
        test ebx,ebx
        jz cancelled
        push 1
        call dword ptr [ebx]
    cancelled:
        pop eax
        pop eax
        mov eax,0x570a9e
        jmp eax
    original:
        popad
        mov eax,[esp+4]
        push esi
        push 0
        mov ebx,0x6402f7
        jmp ebx
    }
}
// samp.dll RVA: 0x000a35e0
__declspec(naked) void GameHook_000a35e0() {
    __asm {
        mov ebp,esp
        mov g_hookExitTask,ecx
        mov eax,[ebp]
        mov g_hookAnimationReturn,eax
        mov eax,[ebp+4]
        mov g_hookControlVehicle,eax
        mov g_hookExitTaskArgument,0xffffffff
        cmp g_hookIgnoreNextExit,0
        jne clear_ignore
        cmp g_hookAnimationReturn,0x5704a1
        je notify
        cmp g_hookAnimationReturn,0x5703fc
        jne clear_ignore
    notify:
        cmp g_pNetGame,0
        je done
        call GameFindLocalPed
        mov eax,[eax+0x58c]
        cmp eax,g_hookControlVehicle
        jne done
        mov ecx,g_pNetGame
        call CNetGame::GetVehiclePool
        mov g_hookEventVehiclePool,eax
        call GameFindLocalPed
        mov ecx,[eax+0x58c]
        push ecx
        mov ecx,g_hookEventVehiclePool
        call CVehiclePool::Find
        mov g_hookEventVehicleId,ax
        movzx edx,g_hookEventVehicleId
        cmp edx,0xffff
        je done
        mov ax,g_hookEventVehicleId
        push eax
        mov ecx,g_hookEventVehiclePool
        call CVehiclePool::Get
        mov g_hookExitVehicle,eax
        mov ecx,g_pNetGame
        call CNetGame::GetPlayerPool
        mov ecx,eax
        call CPlayerPool::GetLocalPlayer
        mov g_hookEventLocalPlayer,eax
        cmp g_hookExitVehicle,0
        je done
        cmp g_hookEventLocalPlayer,0
        je done
        mov ecx,g_hookExitVehicle
        call CVehicle::IsTrainPart
        test eax,eax
        je send_exit
        mov g_hookExitTaskArgument,1
        push offset g_hookExitMatrix
        mov ecx,g_hookExitVehicle
        call CEntity::GetMatrix
        mov ecx,dword ptr [g_hookExitMatrix+0x38]
        push ecx
        fld dword ptr [g_hookExitMatrix+0x34]
        fadd g_hookTrainExitOffset
        push ecx
        fstp dword ptr [esp]
        fld dword ptr [g_hookExitMatrix+0x30]
        fadd g_hookTrainExitOffset
        push ecx
        fstp dword ptr [esp]
        mov ecx,g_hookEventLocalPlayer
        call CLocalPlayer::GetPed
        mov ecx,eax
        call CPed::WarpFromVehicle
        jmp done
    send_exit:
        mov dx,g_hookEventVehicleId
        push edx
        mov ecx,g_hookEventLocalPlayer
        call CLocalPlayer::ExitVehicle
        jmp done
    clear_ignore:
        mov g_hookIgnoreNextExit,0
    done:
        push g_hookExitTaskArgument
        push 0x841618
        mov ecx,g_hookExitTask
        mov edx,0x63b8c0
        add edx,7
        jmp edx
    }
}
// samp.dll RVA: 0x000a3a70
__declspec(naked) void GameHook_000a3a70() {
    __asm {
        mov eax,fs:[0]
        test ecx,ecx
        jz original
        mov g_hookDamageResponse,ecx
        mov ebx,[esp+4]
        test ebx,ebx
        jz original
        mov g_hookDamagePed,ebx
        pushad
        mov eax,g_hookDamagePed
        push eax
        mov ecx,g_hookDamageResponse
        push ecx
        call GameNotifyPedDamage
        test eax,eax
        jz continue_original
        popad
        ret 12
    continue_original:
        popad
    original:
        mov edx,0x4b5ac6
        jmp edx
    }
}
// Native does not return. Preserve its unsigned deadline comparison/wrap behavior.
// samp.dll RVA: 0x000a3800
__declspec(naked) void GameHook_000a3800() {
    __asm {
        mov esi,dword ptr [GetTickCount]
        call esi
        add eax,2000
        mov g_hookQuitDeadline,eax
        call ClientRequestQuit
        call esi
        cmp eax,g_hookQuitDeadline
        jae exit_game
        mov edi,dword ptr [Sleep]
    wait_loop:
        push 100
        call edi
        call esi
        cmp eax,g_hookQuitDeadline
        jb wait_loop
    exit_game:
        push 0
        call dword ptr [ExitProcess]
    }
}
