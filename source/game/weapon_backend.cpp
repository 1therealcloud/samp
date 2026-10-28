#include "game/weapon_backend.h"

// VC7 cannot express a free-function __thiscall pointer. These thin native
// adapters set ECX and keep the backend callee-pop and raw AL return intact.

// GTA SA virtual address: 0x0073FB10.
// 0x000b6020
__declspec(naked) bool __stdcall GameWeaponFireInstantHit(void* weapon, void* shooter, const CVector* origin, const CVector* muzzle, void* victim, const CVector* target,
                                                          const CVector* driveBy, DWORD crossHair, DWORD effects)
{
    // clang-format off
    __asm {
        push ebp
        mov ebp,esp
        push dword ptr [ebp + 0x28]
        push dword ptr [ebp + 0x24]
        push dword ptr [ebp + 0x20]
        push dword ptr [ebp + 0x1c]
        push dword ptr [ebp + 0x18]
        push dword ptr [ebp + 0x14]
        push dword ptr [ebp + 0x10]
        push dword ptr [ebp + 0xc]
        mov ecx,dword ptr [ebp + 0x8]
        mov edx,0x73fb10
        call edx
        pop ebp
        ret 0x24
    }
    // clang-format on
}

// GTA SA virtual address: 0x0073AAC0.
// 0x000b6050
__declspec(naked) bool __stdcall GameWeaponFireSniper(void* weapon, void* shooter)
{
    // clang-format off
    __asm {
        push ebp
        mov ebp,esp
        push 0x0
        push 0x0
        push dword ptr [ebp + 0xc]
        mov ecx,dword ptr [ebp + 0x8]
        mov edx,0x73aac0
        call edx
        pop ebp
        ret 0x8
    }
    // clang-format on
}
