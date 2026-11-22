#include "game/font.h"
// 0x0009fe40
void Font_PrintString(float x, float y, char* text)
{
    // GTA SA cdecl backend. Forward the three argument words.
    // clang-format off
    __asm {
        push text
        push y
        push x
        mov eax,0x71a700
        call eax
        pop eax
        pop eax
        pop eax
    }
    // clang-format on
}

// R5 address to verify
__declspec(naked) void Font_SetColor(DWORD color)
{
    // clang-format off
    __asm mov eax,0x719430 // GTA SA virtual address.
    __asm jmp eax
    // clang-format on
}

// R5 address to verify
__declspec(naked) void Font_SetDropColor(DWORD color)
{
    // clang-format off
    __asm mov eax,0x719510 // GTA SA virtual address.
    __asm jmp eax
    // clang-format on
}

// R5 address to verify
__declspec(naked) void Font_SetOutline(int outline)
{
    // clang-format off
    __asm mov eax,0x719590 // GTA SA virtual address.
    __asm jmp eax
    // clang-format on
}

// R5 address to verify
__declspec(naked) void Font_Unk12(int value)
{
    // clang-format off
    __asm mov eax,0x719600 // GTA SA virtual address.
    __asm jmp eax
    // clang-format on
}

// R5 address to verify
__declspec(naked) void Font_SetScale(float x, float y)
{
    // clang-format off
    __asm mov eax,0x719380 // GTA SA virtual address.
    __asm jmp eax
    // clang-format on
}

// R5 address to verify
__declspec(naked) void Font_SetJustify(int justify)
{
    // clang-format off
    __asm mov eax,0x719610 // GTA SA virtual address.
    __asm jmp eax
    // clang-format on
}

// R5 address to verify
__declspec(naked) void Font_SetFontStyle(int style)
{
    // clang-format off
    __asm mov eax,0x719490 // GTA SA virtual address.
    __asm jmp eax
    // clang-format on
}

// R5 address to verify
__declspec(naked) void Font_SetProportional(int proportional)
{
    // clang-format off
    __asm mov eax,0x7195b0 // GTA SA virtual address.
    __asm jmp eax
    // clang-format on
}

// R5 address to verify
__declspec(naked) void Font_SetRightJustifyWrap(float wrap)
{
    // clang-format off
    __asm mov eax,0x7194f0 // GTA SA virtual address.
    __asm jmp eax
    // clang-format on
}

// R5 address to verify
__declspec(naked) void Font_UseBox(int use, int unknown)
{
    // clang-format off
    __asm mov eax,0x7195c0 // GTA SA virtual address.
    __asm jmp eax
    // clang-format on
}

// R5 address to verify
__declspec(naked) void Font_UseBoxColor(DWORD color)
{
    // clang-format off
    __asm mov eax,0x7195e0 // GTA SA virtual address.
    __asm jmp eax
    // clang-format on
}

// R5 address to verify
__declspec(naked) void Font_SetLineWidth(float width)
{
    // clang-format off
    __asm mov eax,0x7194d0 // GTA SA virtual address.
    __asm jmp eax
    // clang-format on
}

// R5 address to verify
__declspec(naked) void Font_SetLineHeight(float height)
{
    // clang-format off
    __asm mov eax,0x7194e0 // GTA SA virtual address.
    __asm jmp eax
    // clang-format on
}

// R5 address to verify
__declspec(naked) void Font_SetShadow(int shadow)
{
    // clang-format off
    __asm mov eax,0x719570 // GTA SA virtual address.
    __asm jmp eax
    // clang-format on
}

// R5 address to verify
__declspec(naked) void Font_ExpandKeyTokens(char* text){
    // clang-format off
    __asm mov eax,0x69e160 // GTA SA virtual address.
    __asm jmp eax
    // clang-format on
} // Native returns this GTA address without calling it.
// R5 address to verify
DWORD Font_GetTextSubstitutionAddress(char* text, int a, int b, int c, int d, int e, int f, char* output)
{
    return 0x69de90;
}
