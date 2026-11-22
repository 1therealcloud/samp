#ifndef FONT_H
#define FONT_H

#include <windows.h>
// 0x0009fe40
void Font_PrintString(float x, float y, char* text);
// R5 address to verify
void Font_SetColor(DWORD color);
// R5 address to verify
void Font_SetDropColor(DWORD color);
// R5 address to verify
void Font_SetOutline(int outline);
// R5 address to verify
void Font_SetJustify(int value);
// R5 address to verify
void Font_SetScale(float x, float y);
// R5 address to verify
void Font_SetOrientation(int justify);
// R5 address to verify
void Font_SetFontStyle(int style);
// R5 address to verify
void Font_SetProportional(int proportional);
// R5 address to verify
void Font_SetRightJustifyWrap(float wrap);
// R5 address to verify
void Font_UseBox(int use, int unknown);
// R5 address to verify
void Font_UseBoxColor(DWORD color);
// R5 address to verify
void Font_SetLineWidth(float width);
// R5 address to verify
void Font_SetLineHeight(float height);
// R5 address to verify
void Font_SetShadow(int shadow);
// R5 address to verify
void Font_ExpandKeyTokens(char* text);
// R5 address to verify
DWORD Font_GetTextSubstitutionAddress(char* text, int a, int b, int c, int d, int e, int f, char* output);

#endif // FONT_H
