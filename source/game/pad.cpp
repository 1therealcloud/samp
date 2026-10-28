#include "game/pad.h"
// GTA SA virtual addresses: GetPad 0x0053FB70; Clear 0x00541A70.
// VC7 ABI bridge: GetPad(0), then ECX=pad and two unmodified argument words.
// samp.dll RVA: 0x000b58e0
__declspec(naked) void GameClearLocalPad(DWORD first, DWORD second)
{
    // clang-format off
    __asm {
        push ebp
        mov ebp,esp
        push 0x0
        mov edx,0x53fb70
        call edx
        mov ecx,eax
        pop edx
        push dword ptr [ebp + 0xc]
        push dword ptr [ebp + 0x8]
        mov edx,0x541a70
        call edx
        pop ebp
        ret
    }
    // clang-format on
}
