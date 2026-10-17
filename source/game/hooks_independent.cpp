#include "game/hooks_independent.h"
#include "game/input.h"
#include "game/playerped.h"
#include "game/player_colors.h"
// samp.dll RVA: 0x0018f4c8
DWORD g_hookRadarIndex;
// samp.dll RVA: 0x0018f4cc
DWORD g_hookCheatFunction;
// samp.dll RVA: 0x0018f4b0
DWORD g_hookCameraPed;
// samp.dll RVA: 0x0018f540
DWORD g_hookRenderEsi;
// samp.dll RVA: 0x0018f544
DWORD g_hookRenderEbx;
// samp.dll RVA: 0x0018f568
DWORD g_hookFreedBuffer;
// samp.dll RVA: 0x00151ab8
DWORD g_hookKeepBuffer=1;
// samp.dll RVA: 0x0018f560
DWORD g_hookCachedBuffer;
// samp.dll RVA: 0x00151abc
DWORD g_hookAllocateFunction=0x7fb270;
// samp.dll RVA: 0x0018f570
DWORD g_hookSavedEsi;
// samp.dll RVA: 0x0018f5c4
DWORD g_hookRenderReturn;
// samp.dll RVA: 0x00151ac8
BYTE g_hookRenderRestore[3]={0xff,0x52,0x20};
// samp.dll RVA: 0x0018f5d4
DWORD g_hookSavedFontA;
// samp.dll RVA: 0x0018f5d8
DWORD g_hookSavedFontB;

// Yield before entering the original game update routine.
// samp.dll RVA: 0x000a2200
__declspec(naked) void GameHook_000a2200() {
    __asm { push 1
        call dword ptr [Sleep]
        mov edx,0x561a80
        jmp edx }
}
// The original argument remains on the caller's stack; Get consumes its copy.
// samp.dll RVA: 0x000a37a0
__declspec(naked) void GameHook_000a37a0() {
    __asm { mov eax,[esp+4]
        mov g_hookRadarIndex,eax
        mov eax,g_hookRadarIndex
        push eax
        call PlayerColors::Get
        ret }
}
// samp.dll RVA: 0x000a37c0
__declspec(naked) void GameHook_000a37c0() {
    __asm { mov g_hookCheatFunction,eax
        pushad
        mov edx,0x96918c
        mov byte ptr [edx],1
        popad
        mov edx,0x438581
        cmp eax,0
        jmp edx }
}
// Intentional native one-RET callback, not a replacement for an unknown body.
// samp.dll RVA: 0x000a3ac0
__declspec(naked) void GameHook_000a3ac0() { __asm ret }
// samp.dll RVA: 0x000a3ad0
__declspec(naked) void GameHook_000a3ad0() {
    __asm { mov ebx,[esp+8]
        mov g_hookCameraPed,ebx
        pushad
        call GameFindLocalPed
        cmp g_hookCameraPed,eax
        jne original
        mov byte ptr ds:[0xc8a7c0],1
        mov byte ptr ds:[0xc8a7c1],1
    original:
        popad
        mov ebx,0x73c260
        jmp ebx }
}
// Preserve the sign bit while replacing the remaining float bits by 1.0f.
// samp.dll RVA: 0x000a3b00
__declspec(naked) void GameHook_000a3b00() {
    __asm { mov eax,[esi+0x5a4]
        and eax,0x80000000
        or eax,0x3f800000
        mov [esi+0x5a4],eax
        ret }
}
// The non-null branch continues after GTA's original PUSH EDI prologue.
// samp.dll RVA: 0x000a3b20
__declspec(naked) void GameHook_000a3b20() {
    __asm { push edi
        mov edi,[esp+8]
        test edi,edi
        jz missing
        mov eax,0x4d41c5
        jmp eax
    missing:
        pop edi
        ret }
}
// samp.dll RVA: 0x000a3ce0
__declspec(naked) void GameHook_000a3ce0() {
    __asm { mov g_hookRenderEsi,esi
        mov g_hookRenderEbx,ebx
        pushad
        mov eax,g_hookRenderEbx
        mov ecx,[eax+4]
        test ecx,ecx
        jnz present
        push g_hookRenderEsi
        mov eax,0x739a40
        call eax
        add esp,4
        popad
        mov dl,0xff
        jmp resume
    present:
        popad
        mov ecx,[ebx+4]
        mov dl,[ecx+0x36]
    resume:
        push 0x738f40
        ret }
}
// EBP is an incoming GTA frame pointer, not a compiler-owned C++ frame.
// samp.dll RVA: 0x000a3d20
__declspec(naked) void GameHook_000a3d20() {
    __asm { cmp dword ptr [esi-0x20],0x27
        mov edi,[ebp]
        jne fallback
        mov eax,[esp]
        cmp [esi-0x1c],eax
        jne fallback
        mov eax,0x738880
        jmp eax
    fallback:
        mov eax,0x7388db
        jmp eax }
}
// samp.dll RVA: 0x000a3e50
__declspec(naked) void GameHook_000a3e50() {
    __asm { test ecx,ecx
        jnz present
        ret
    present:
        mov eax,[ecx+0x1c]
        shr eax,31
        cmp eax,1
        jne original
        ret
    original:
        mov edx,0x54dfb0
        jmp edx }
}
// samp.dll RVA: 0x000a4190
__declspec(naked) void GameHook_000a4190() {
    __asm { mov eax,[esp+4]
        mov g_hookFreedBuffer,eax
        pushad
        cmp g_hookKeepBuffer,0
        je original
        cmp g_hookCachedBuffer,0
        je original
        mov eax,g_hookFreedBuffer
        cmp eax,g_hookCachedBuffer
        jne original
        popad
        mov eax,1
        ret
    original:
        popad
        push esi
        mov esi,[esp+8]
        push esi
        mov edx,g_gameHookFarClipReturn
        jmp edx }
}
// Preserve the native sticky allocator selection and four POP EBX operations.
// samp.dll RVA: 0x000a4280
__declspec(naked) void GameHook_000a4280() {
    __asm { mov eax,g_hookCachedBuffer
        test eax,eax
        jnz cached
        cmp g_gameVersion,1
        jne allocate
        mov g_hookAllocateFunction,0x7fb230
    allocate:
        push 0x604
        push 0x20
        push 0x10
        push 0x40
        mov ebx,g_hookAllocateFunction
        call ebx
        mov g_hookCachedBuffer,eax
        pop ebx
        pop ebx
        pop ebx
        pop ebx
    cached:
        mov eax,g_hookCachedBuffer
        ret }
}
// samp.dll RVA: 0x000a42c0
__declspec(naked) void GameHook_000a42c0() { __asm { mov g_hookSavedEsi,esi
        ret } }
// samp.dll RVA: 0x000a4850
__declspec(naked) void GameHook_000a4850() {
    __asm { mov eax,[esp]
        mov g_hookRenderReturn,eax
        pushad
        push 3
        push 0x4096aa
        call GameUnprotect
        mov eax,0x90909090
        mov ecx,0x4096aa
        mov word ptr [ecx],ax
        add esp,8
        mov byte ptr [ecx+2],al
        popad
        mov eax,0x533d30
        call eax
        pushad
        mov ax,word ptr g_hookRenderRestore
        mov edx,0x4096aa
        mov word ptr [edx],ax
        mov cl,byte ptr g_hookRenderRestore[2]
        mov byte ptr [edx+2],cl
        popad
        ret }
}
// samp.dll RVA: 0x000a4960
__declspec(naked) void GameHook_000a4960() {
    __asm { pushad
        push 4
        push 0x858ba4
        call GameUnprotect
        push 4
        push 0x858f84
        call GameUnprotect
        mov eax,dword ptr ds:[0x858ba4]
        mov g_hookSavedFontA,eax
        mov ecx,dword ptr ds:[0x858f84]
        mov g_hookSavedFontB,ecx
        mov dword ptr ds:[0x858ba4],0x42200000
        add esp,16
        mov dword ptr ds:[0x858f84],0x42480000
        popad
        mov eax,0x717d30
        call eax
        pushad
        mov edx,g_hookSavedFontA
        mov dword ptr ds:[0x858ba4],edx
        mov eax,g_hookSavedFontB
        mov dword ptr ds:[0x858f84],eax
        popad
        ret }
}
// samp.dll RVA: 0x000a5150
__declspec(naked) void GameHook_000a5150() {
    __asm { mov eax,[esp+4]
        test eax,eax
        jz missing
        mov eax,0x6ff350
        jmp eax
    missing:
        ret }
}
// samp.dll RVA: 0x000a5850
__declspec(naked) void GameHook_000a5850() {
    __asm { mov eax,[esp+4]
        test eax,eax
        jz skip
        mov ecx,[eax+0xfc]
        cmp ecx,0
        jne skip
        mov edx,0x563f40
        jmp edx
    skip:
        mov eax,1
        ret }
}
// Continue into GTA after its 44-byte local-frame allocation, forwarding the
// observed argument-byte override rather than translating this into a C++ call.
// samp.dll RVA: 0x000a5d20
__declspec(naked) void GameHook_000a5d20() {
    __asm { mov eax,[esp+0x14]
        test eax,eax
        jz original
        mov byte ptr [esp+0x20],1
    original:
        sub esp,0x2c
        mov eax,0x407c37
        jmp eax }
}
