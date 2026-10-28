#ifndef SAMP_GAME_FONT_H
#define SAMP_GAME_FONT_H
#include <windows.h>
// 0x0009fc80
void Font_PrintString(float x, float y, char* text);
// 0x0009fca0
void Font_SetColor(DWORD color);
// 0x0009fcb0
void Font_SetDropColor(DWORD color);
// 0x0009fcc0
void Font_SetOutline(int outline);
// 0x0009fcd0
void Font_Unk12(int value);
// 0x0009fce0
void Font_SetScale(float x, float y);
// 0x0009fcf0
void Font_SetJustify(int justify);
// 0x0009fd00
void Font_SetFontStyle(int style);
// 0x0009fd10
void Font_SetProportional(int proportional);
// 0x0009fd20
void Font_SetRightJustifyWrap(float wrap);
// 0x0009fd30
void Font_UseBox(int use, int unknown);
// 0x0009fd40
void Font_UseBoxColor(DWORD color);
// 0x0009fd50
void Font_SetLineWidth(float width);
// 0x0009fd60
void Font_SetLineHeight(float height);
// 0x0009fd70
void Font_SetShadow(int shadow);
// 0x0009fd90
void Font_UnkConv2(char* text);
// 0x0009fd80
DWORD Font_UnkConv(char* text, int a, int b, int c, int d, int e, int f, char* output);
#endif
