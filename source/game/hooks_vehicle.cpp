#include "game/hooks_vehicle.h"
#include "game/playerped.h"
#include "client/exceptions.h"
#include "net/vehiclepool.h"
#include "main.h"
// samp.dll RVA: 0x0018f690
DWORD g_hookContactThis;
// samp.dll RVA: 0x0018f694
DWORD g_hookContactOther;
// samp.dll RVA: 0x0018f698
DWORD g_hookContactReturn;
// samp.dll RVA: 0x0018f69c
DWORD g_hookSirenVehicle;
// samp.dll RVA: 0x0018f6a0
BYTE g_hookSirenResult;
// samp.dll RVA: 0x0018f6a4
DWORD g_hookSirenOnce;
// samp.dll RVA: 0x00151b0c
WORD g_hookSirenVehicleId=0xffff;
// samp.dll RVA: 0x0018f63c
DWORD g_hookPedQueryThis;
// samp.dll RVA: 0x0018f640
DWORD g_hookPedQueryPed;

// Raw GTA collision method ABI; suppression returns with RET8.
// samp.dll RVA: 0x000a5870
__declspec(naked) void GameHook_000a5870() {
    __asm {
        mov g_hookContactThis,ecx
        mov edx,[esp]
        mov g_hookContactReturn,edx
        mov edx,[esp+4]
        mov g_hookContactOther,edx
        pushad
        cmp g_pNetGame,0
        je original
        mov eax,g_pNetGame
        movzx ecx,byte ptr [eax+0x232]
        test ecx,ecx
        je original
        cmp g_hookContactThis,0
        je original
        cmp g_hookContactOther,0
        je original
        mov edx,g_hookContactOther
        movsx eax,word ptr [edx+0x22]
        cmp eax,400
        jl original
        mov ecx,g_hookContactOther
        movsx edx,word ptr [ecx+0x22]
        cmp edx,611
        jg original
        mov eax,g_hookContactThis
        cmp dword ptr [eax+0x460],0
        je original
        mov ecx,g_hookContactOther
        cmp dword ptr [ecx+0x460],0
        je original
        popad
        mov eax,0
        ret 8
    original:
        popad
        mov edx,0x6ace70
        jmp edx
    }
}

// Raw GTA collision method ABI; suppression returns with RET8.
// samp.dll RVA: 0x000a5910
__declspec(naked) void GameHook_000a5910() {
    __asm {
        mov g_hookContactThis,ecx
        mov edx,[esp]
        mov g_hookContactReturn,edx
        mov edx,[esp+4]
        mov g_hookContactOther,edx
        pushad
        cmp g_pNetGame,0
        je original
        mov eax,g_pNetGame
        movzx ecx,byte ptr [eax+0x232]
        test ecx,ecx
        je original
        cmp g_hookContactThis,0
        je original
        cmp g_hookContactOther,0
        je original
        mov edx,g_hookContactOther
        movsx eax,word ptr [edx+0x22]
        cmp eax,400
        jl original
        mov ecx,g_hookContactOther
        movsx edx,word ptr [ecx+0x22]
        cmp edx,611
        jg original
        mov eax,g_hookContactThis
        cmp dword ptr [eax+0x460],0
        je original
        mov ecx,g_hookContactOther
        cmp dword ptr [ecx+0x460],0
        je original
        popad
        mov eax,0
        ret 8
    original:
        popad
        mov edx,0x6bdea0
        jmp edx
    }
}

// Raw GTA collision method ABI; suppression returns with RET8.
// samp.dll RVA: 0x000a59b0
__declspec(naked) void GameHook_000a59b0() {
    __asm {
        mov g_hookContactThis,ecx
        mov edx,[esp]
        mov g_hookContactReturn,edx
        mov edx,[esp+4]
        mov g_hookContactOther,edx
        pushad
        cmp g_pNetGame,0
        je original
        mov eax,g_pNetGame
        movzx ecx,byte ptr [eax+0x232]
        test ecx,ecx
        je original
        cmp g_hookContactThis,0
        je original
        cmp g_hookContactOther,0
        je original
        mov edx,g_hookContactOther
        movsx eax,word ptr [edx+0x22]
        cmp eax,400
        jl original
        mov ecx,g_hookContactOther
        movsx edx,word ptr [ecx+0x22]
        cmp edx,611
        jg original
        mov eax,g_hookContactThis
        cmp dword ptr [eax+0x460],0
        je original
        mov ecx,g_hookContactOther
        cmp dword ptr [ecx+0x460],0
        je original
        popad
        mov eax,0
        ret 8
    original:
        popad
        mov edx,0x6c8ae0
        jmp edx
    }
}

// Raw GTA collision method ABI; suppression returns with RET8.
// samp.dll RVA: 0x000a5a50
__declspec(naked) void GameHook_000a5a50() {
    __asm {
        mov g_hookContactThis,ecx
        mov edx,[esp]
        mov g_hookContactReturn,edx
        mov edx,[esp+4]
        mov g_hookContactOther,edx
        pushad
        cmp g_pNetGame,0
        je original
        mov eax,g_pNetGame
        movzx ecx,byte ptr [eax+0x232]
        test ecx,ecx
        je original
        cmp g_hookContactThis,0
        je original
        cmp g_hookContactOther,0
        je original
        mov edx,g_hookContactOther
        movsx eax,word ptr [edx+0x22]
        cmp eax,400
        jl original
        mov ecx,g_hookContactOther
        movsx edx,word ptr [ecx+0x22]
        cmp edx,611
        jg original
        mov eax,g_hookContactThis
        cmp dword ptr [eax+0x460],0
        je original
        mov ecx,g_hookContactOther
        cmp dword ptr [ecx+0x460],0
        je original
        popad
        mov eax,0
        ret 8
    original:
        popad
        mov edx,0x546d00
        jmp edx
    }
}
// samp.dll RVA: 0x000a5820
__declspec(naked) void GameHook_000a5820() {
    __asm {
        pushad
        cmp g_pNetGame,0
        je original
        mov eax,g_pNetGame
        movzx ecx,byte ptr [eax+0x232]
        test ecx,ecx
        je original
        popad
        xor eax,eax
        ret
    original:
        popad
        mov eax,0x41a990
        jmp eax
    }
}
// samp.dll RVA: 0x000a5100
__declspec(naked) void GameHook_000a5100() {
    __asm {
        mov g_hookPedQueryThis,ecx
        mov eax,[esp+4]
        mov g_hookPedQueryPed,eax
        pushad
        cmp g_pNetGame,0
        je original
        call GameFindLocalPed
        cmp g_hookPedQueryPed,eax
        jne original
        mov eax,g_clientCrashPedContext
        movzx ecx,byte ptr [eax]
        test ecx,ecx
        je original
        popad
        xor eax,eax
        mov al,1
        ret 4
    original:
        popad
        sub esp,12
        push esi
        mov esi,ecx
        mov eax,0x4b35a6
        jmp eax
    }
}
// Raw siren query returns AL. The one-shot path deliberately leaves upper EAX.
// samp.dll RVA: 0x000a5af0
__declspec(naked) void GameHook_000a5af0() {
    __asm {
        mov eax,g_hookSirenOnce
        test eax,eax
        je backend
        mov g_hookSirenOnce,0
        mov al,1
        ret
    backend:
        mov g_hookSirenVehicle,ecx
        mov edx,0x6d8470
        call edx
        mov g_hookSirenResult,al
        mov al,g_hookSirenResult
        test al,al
        jne finish
        pushad
        mov eax,g_pNetGame
        test eax,eax
        je restore
        mov eax,[eax+0x3de]
        mov ecx,[eax+0xc]
        test ecx,ecx
        je restore
        mov edx,g_hookSirenVehicle
        push edx
        call CVehiclePool::Find
        cmp ax,0xffff
        mov g_hookSirenVehicleId,ax
        je restore
        mov ecx,g_pNetGame
        mov edx,[ecx+0x3de]
        mov ecx,[edx+0xc]
        push eax
        call CVehiclePool::Get
        mov ecx,eax
        call CVehicle::HasSiren
        test eax,eax
        je restore
        mov g_hookSirenResult,1
    restore:
        popad
    finish:
        mov al,g_hookSirenResult
        ret
    }
}
