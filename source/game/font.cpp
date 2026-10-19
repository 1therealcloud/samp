#include "game/font.h"
// samp.dll RVA: 0x0009fc80
void Font_PrintString(float x,float y,char *text) { ((void (__cdecl*)(float,float,char*))0x71a700)(x,y,text); }
// samp.dll RVA: 0x0009fca0
__declspec(naked) void Font_SetColor(DWORD color) {
    __asm mov eax,0x719430 // GTA SA virtual address.
    __asm jmp eax
}
// samp.dll RVA: 0x0009fcb0
__declspec(naked) void Font_SetDropColor(DWORD color) {
    __asm mov eax,0x719510 // GTA SA virtual address.
    __asm jmp eax
}
// samp.dll RVA: 0x0009fcc0
__declspec(naked) void Font_SetOutline(int outline) {
    __asm mov eax,0x719590 // GTA SA virtual address.
    __asm jmp eax
}
// samp.dll RVA: 0x0009fcd0
__declspec(naked) void Font_Unk12(int value) {
    __asm mov eax,0x719600 // GTA SA virtual address.
    __asm jmp eax
}
// samp.dll RVA: 0x0009fce0
__declspec(naked) void Font_SetScale(float x,float y) {
    __asm mov eax,0x719380 // GTA SA virtual address.
    __asm jmp eax
}
// samp.dll RVA: 0x0009fcf0
__declspec(naked) void Font_SetJustify(int justify) {
    __asm mov eax,0x719610 // GTA SA virtual address.
    __asm jmp eax
}
// samp.dll RVA: 0x0009fd00
__declspec(naked) void Font_SetFontStyle(int style) {
    __asm mov eax,0x719490 // GTA SA virtual address.
    __asm jmp eax
}
// samp.dll RVA: 0x0009fd10
__declspec(naked) void Font_SetProportional(int proportional) {
    __asm mov eax,0x7195b0 // GTA SA virtual address.
    __asm jmp eax
}
// samp.dll RVA: 0x0009fd20
__declspec(naked) void Font_SetRightJustifyWrap(float wrap) {
    __asm mov eax,0x7194f0 // GTA SA virtual address.
    __asm jmp eax
}
// samp.dll RVA: 0x0009fd30
__declspec(naked) void Font_UseBox(int use,int unknown) {
    __asm mov eax,0x7195c0 // GTA SA virtual address.
    __asm jmp eax
}
// samp.dll RVA: 0x0009fd40
__declspec(naked) void Font_UseBoxColor(DWORD color) {
    __asm mov eax,0x7195e0 // GTA SA virtual address.
    __asm jmp eax
}
// samp.dll RVA: 0x0009fd50
__declspec(naked) void Font_SetLineWidth(float width) {
    __asm mov eax,0x7194d0 // GTA SA virtual address.
    __asm jmp eax
}
// samp.dll RVA: 0x0009fd60
__declspec(naked) void Font_SetLineHeight(float height) {
    __asm mov eax,0x7194e0 // GTA SA virtual address.
    __asm jmp eax
}
// samp.dll RVA: 0x0009fd70
__declspec(naked) void Font_SetShadow(int shadow) {
    __asm mov eax,0x719570 // GTA SA virtual address.
    __asm jmp eax
}
// samp.dll RVA: 0x0009fd90
__declspec(naked) void Font_UnkConv2(char *text) {
    __asm mov eax,0x69e160 // GTA SA virtual address.
    __asm jmp eax
}
// Native returns this GTA address without calling it.
// samp.dll RVA: 0x0009fd80
DWORD Font_UnkConv(char *text,int a,int b,int c,int d,int e,int f,char *output) { return 0x69de90; }
