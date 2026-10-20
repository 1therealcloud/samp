#include "game/entity.h"
#include "game/game.h"
#include "game/camera.h"
#include "main.h"

// samp.dll RVA: 0x0011e644
const float g_entityUnavailableDistance=100000.0f;

// Native thiscall; preserve uninitialized GetMatrix outputs and extended ST0.
// samp.dll RVA: 0x0009ef00
__declspec(naked) float CEntity::GetDistanceToLocalPlayer() {
    __asm {
        sub esp,0x80
        push esi
        mov esi,ecx
        mov ecx,dword ptr g_pGame
        push edi
        call CGame::GetPlayerPed
        mov edi,eax
        test edi,edi
        jnz branch_1009ef2a
        fld float ptr g_entityUnavailableDistance
        pop edi
        pop esi
        add esp,0x80
        ret
    branch_1009ef2a:
        lea eax,[esp + 0x48]
        push eax
        mov ecx,esi
        call CEntity::GetMatrix
        mov eax,g_pNetGame
        test eax,eax
        jz branch_1009ef7f
        mov ecx,dword ptr [eax + 0x3de]
        mov edx,dword ptr [ecx + 0x8]
        mov eax,dword ptr [edx + 0x1e]
        test eax,eax
        jz branch_1009ef78
        mov ecx,dword ptr [eax + 0x12c]
        test ecx,ecx
        jnz branch_1009ef63
        mov ecx,dword ptr [eax + 0x1d4]
        test ecx,ecx
        jz branch_1009ef78
    branch_1009ef63:
        mov ecx,dword ptr g_pGame
        mov ecx,dword ptr [ecx + 0x4]
        lea eax,[esp + 0x8]
        push eax
        call CCamera::GetMatrix
        jmp branch_1009ef8b
    branch_1009ef78:
        lea edx,[esp + 0x8]
        push edx
        jmp branch_1009ef84
    branch_1009ef7f:
        lea eax,[esp + 0x8]
        push eax
    branch_1009ef84:
        mov ecx,edi
        call CEntity::GetMatrix
    branch_1009ef8b:
        fld float ptr [esp + 0x80]
        pop edi
        fsub float ptr [esp + 0x3c]
        pop esi
        fld float ptr [esp + 0x74]
        fsub float ptr [esp + 0x34]
        fld float ptr [esp + 0x70]
        fsub float ptr [esp + 0x30]
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fld st(3)
        fmul st(0),st(4)
        faddp st(1),st(0)
        fsqrt
        fstp st(3)
        fstp st(0)
        fstp st(0)
        add esp,0x80
        ret
    }
}

// Native thiscall; preserve uninitialized GetMatrix outputs and extended ST0.
// samp.dll RVA: 0x0009efd0
__declspec(naked) float CEntity::GetDistanceToCamera() {
    __asm {
        mov eax,dword ptr [ecx + 0x40]
        sub esp,0x40
        test eax,eax
        jz branch_1009f025
        cmp dword ptr [eax],0x863c40
        jz branch_1009f025
        lea eax,[esp]
        push eax
        call CEntity::GetMatrix
        fld float ptr [esp + 0x38]
        fsub dword ptr ds:[0x00b6f9d4]
        fld float ptr [esp + 0x34]
        fsub dword ptr ds:[0x00b6f9d0]
        fld float ptr [esp + 0x30]
        fsub dword ptr ds:[0x00b6f9cc]
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fld st(3)
        fmul st(0),st(4)
        faddp st(1),st(0)
        fsqrt
        fstp st(3)
        fstp st(0)
        fstp st(0)
        add esp,0x40
        ret
    branch_1009f025:
        fld float ptr g_entityUnavailableDistance
        add esp,0x40
        ret
    }
}

// Native thiscall; preserve uninitialized GetMatrix outputs and extended ST0.
// samp.dll RVA: 0x0009f030
__declspec(naked) float CEntity::GetDistanceToLocalPlayerNoHeight() {
    __asm {
        sub esp,0x80
        push esi
        mov esi,ecx
        mov ecx,dword ptr g_pGame
        push edi
        call CGame::GetPlayerPed
        mov edi,eax
        test edi,edi
        jnz branch_1009f05a
        fld float ptr g_entityUnavailableDistance
        pop edi
        pop esi
        add esp,0x80
        ret
    branch_1009f05a:
        lea eax,[esp + 0x48]
        push eax
        mov ecx,esi
        call CEntity::GetMatrix
        mov eax,g_pNetGame
        test eax,eax
        jz branch_1009f0af
        mov ecx,dword ptr [eax + 0x3de]
        mov edx,dword ptr [ecx + 0x8]
        mov eax,dword ptr [edx + 0x1e]
        test eax,eax
        jz branch_1009f0a8
        mov ecx,dword ptr [eax + 0x12c]
        test ecx,ecx
        jnz branch_1009f093
        mov ecx,dword ptr [eax + 0x1d4]
        test ecx,ecx
        jz branch_1009f0a8
    branch_1009f093:
        mov ecx,dword ptr g_pGame
        mov ecx,dword ptr [ecx + 0x4]
        lea eax,[esp + 0x8]
        push eax
        call CCamera::GetMatrix
        jmp branch_1009f0bb
    branch_1009f0a8:
        lea edx,[esp + 0x8]
        push edx
        jmp branch_1009f0b4
    branch_1009f0af:
        lea eax,[esp + 0x8]
        push eax
    branch_1009f0b4:
        mov ecx,edi
        call CEntity::GetMatrix
    branch_1009f0bb:
        fld float ptr [esp + 0x7c]
        pop edi
        fsub float ptr [esp + 0x38]
        pop esi
        fld float ptr [esp + 0x70]
        fsub float ptr [esp + 0x30]
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fsqrt
        fstp st(2)
        fstp st(0)
        add esp,0x80
        ret
    }
}

// Descriptive name: native only establishes setting backend +1C bit 31.
// samp.dll RVA: 0x0009f560
void CEntity::SetBackendHighFlag() {
    DWORD *backend=(DWORD*)m_pGameEntity;
    if(backend && *backend!=0x863c40)backend[7]|=0x80000000;
}
// GTA vtable slot 10 (+28), ProcessControl. Native does not check backend.
// samp.dll RVA: 0x0009fa20
void CEntity::ProcessControl() {
    void *backend=m_pGameEntity;
    __asm {
        mov ecx,backend
        mov eax,[ecx]
        call dword ptr [eax+0x28]
    }
}

// Observed CEntity backend wrapper; semantic name/raw argument types remain provisional.
// GTA callees use their original ECX/stack ABI.
// samp.dll RVA: 0x0009ed60
__declspec(naked) void CEntity::Native009ed60(DWORD argument) {
    __asm {
        push ebp
        mov ebp,esp
        push ecx
        mov ecx,dword ptr [ecx + 0x40]
        test ecx,ecx
        mov dword ptr [ebp + -0x4],ecx
        jz branch_1009ed7b
        push dword ptr [ebp + 0x8]
        mov ecx,dword ptr [ebp + -0x4]
        mov edx,0x534120
        call edx
    branch_1009ed7b:
        mov esp,ebp
        pop ebp
        ret 0x4
    }
}

// Observed CEntity backend wrapper; semantic name/raw argument types remain provisional.
// GTA callees use their original ECX/stack ABI.
// samp.dll RVA: 0x0009f5d0
__declspec(naked) void CEntity::Native009f5d0(DWORD argument) {
    __asm {
        push ebp
        mov ebp,esp
        push ecx
        mov eax,dword ptr [ecx + 0x40]
        test eax,eax
        jz branch_1009f600
        mov ecx,dword ptr [eax + 0x18]
        test ecx,ecx
        jz branch_1009f600
        cmp dword ptr [eax],0x863c40
        jz branch_1009f600
        mov dword ptr [ebp + -0x4],eax
        mov eax,dword ptr [ebp + -0x4]
        mov edx,dword ptr [eax + 0x18]
        push dword ptr [ebp + 0x8]
        push edx
        mov eax,0x732b00
        call eax
        pop edx
        pop edx
    branch_1009f600:
        mov esp,ebp
        pop ebp
        ret 0x4
    }
}

// Observed CEntity backend wrapper; semantic name/raw argument types remain provisional.
// GTA callees use their original ECX/stack ABI.
// samp.dll RVA: 0x0009f810
__declspec(naked) void CEntity::Native009f810(DWORD a1,DWORD a2,DWORD a3,DWORD a4,DWORD a5,DWORD a6) {
    __asm {
        push ebp
        mov ebp,esp
        push ecx
        mov ecx,dword ptr [ecx + 0x40]
        test ecx,ecx
        mov dword ptr [ebp + -0x4],ecx
        jz branch_1009f83a
        push dword ptr [ebp + 0x1c]
        push dword ptr [ebp + 0x18]
        push dword ptr [ebp + 0x14]
        push dword ptr [ebp + 0x10]
        push dword ptr [ebp + 0xc]
        push dword ptr [ebp + 0x8]
        mov eax,0x542a50
        mov ecx,dword ptr [ebp + -0x4]
        call eax
    branch_1009f83a:
        mov esp,ebp
        pop ebp
        ret 0x18
    }
}

// Observed CEntity backend wrapper; semantic name/raw argument types remain provisional.
// GTA callees use their original ECX/stack ABI.
// samp.dll RVA: 0x0009f840
__declspec(naked) void CEntity::Native009f840(DWORD argument) {
    __asm {
        push ebp
        mov ebp,esp
        push ecx
        mov ecx,dword ptr [ecx + 0x40]
        test ecx,ecx
        mov dword ptr [ebp + -0x4],ecx
        jz branch_1009f863
        push 0x0
        push dword ptr [ebp + 0x8]
        mov edx,dword ptr [ebp + -0x4]
        lea ecx,[edx + 0x138]
        mov edx,0x4f6420
        call edx
    branch_1009f863:
        mov esp,ebp
        pop ebp
        ret 0x4
    }
}

// Observed CEntity backend wrapper; semantic name/raw argument types remain provisional.
// GTA callees use their original ECX/stack ABI.
// samp.dll RVA: 0x0009f960
__declspec(naked) void CEntity::Native009f960() {
    __asm {
        push ebp
        mov ebp,esp
        sub esp,0xc
        push esi
        mov esi,ecx
        mov eax,dword ptr [esi + 0x40]
        mov ecx,dword ptr [eax]
        mov dword ptr [ebp + -0xc],ecx
        mov ecx,eax
        test ecx,ecx
        push edi
        mov dword ptr [ebp + -0x4],ecx
        jnz branch_1009f97f
        xor edi,edi
        jmp branch_1009f982
    branch_1009f97f:
        mov edi,dword ptr [eax + 0x18]
    branch_1009f982:
        mov ax,word ptr [eax + 0x22]
        cmp ax,0x190
        mov dword ptr [ebp + -0x8],edi
        jl branch_1009f9a4
        cmp ax,0x263
        jg branch_1009f9a4
        test edi,edi
        jz branch_1009f9a4
        push dword ptr [ebp + -0x8]
        mov edx,0x733160
        call edx
        pop edx
    branch_1009f9a4:
        mov eax,dword ptr [ebp + -0xc]
        mov ecx,dword ptr [ebp + -0x4]
        call dword ptr [eax + 0x44]
        mov edx,dword ptr [esi + 0x40]
        mov ax,word ptr [edx + 0x22]
        cmp ax,0x190
        jl branch_1009f9d5
        cmp ax,0x263
        jg branch_1009f9d5
        test edi,edi
        jz branch_1009f9e4
        push dword ptr [ebp + -0x4]
        mov edx,0x553260
        call edx
        pop edx
        pop edi
        pop esi
        mov esp,ebp
        pop ebp
        ret
    branch_1009f9d5:
        test edi,edi
        jz branch_1009f9e4
        push dword ptr [ebp + -0x4]
        mov edx,0x553260
        call edx
        pop edx
    branch_1009f9e4:
        pop edi
        pop esi
        mov esp,ebp
        pop ebp
        ret
    }
}

// Observed CEntity backend wrapper; semantic name/raw argument types remain provisional.
// GTA callees use their original ECX/stack ABI.
// samp.dll RVA: 0x0009fba0
__declspec(naked) void CEntity::ApplyLinearSpeed() {
    __asm {
        sub esp,0x40
        push ebx
        mov ebx,ecx
        mov eax,dword ptr [ebx + 0x40]
        test eax,eax
        push esi
        push edi
        jz branch_1009fc0a
        fld dword ptr ds:[0x00b7cb5c]
        lea eax,[esp + 0xc]
        push eax
        call CEntity::GetMatrix
        fld st(0)
        mov eax,dword ptr [ebx + 0x40]
        fmul float ptr [eax + 0x44]
        sub esp,0x40
        mov edi,esp
        mov ecx,0x10
        fadd float ptr [esp + 0x7c]
        lea esi,[esp + 0x4c]
        fstp float ptr [esp + 0x7c]
        fld st(0)
        fmul float ptr [eax + 0x48]
        fadd float ptr [esp + 0x80]
        fstp float ptr [esp + 0x80]
        fmul float ptr [eax + 0x4c]
        fadd float ptr [esp + 0x84]
        fstp float ptr [esp + 0x84]
        rep movsd
        mov ecx,ebx
        call CEntity::ApplyMatrix
    branch_1009fc0a:
        pop edi
        pop esi
        pop ebx
        add esp,0x40
        ret
    }
}
