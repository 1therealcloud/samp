#ifndef FONT_H
#define FONT_H

#include <windows.h>

void Font_PrintString(float x, float y, char* text);

void Font_SetColor(DWORD color);

void Font_SetDropColor(DWORD color);

void Font_SetOutline(int outline);

void Font_SetJustify(int value);

void Font_SetScale(float x, float y);

void Font_SetOrientation(int justify);

void Font_SetFontStyle(int style);

void Font_SetProportional(int proportional);

void Font_SetRightJustifyWrap(float wrap);

void Font_UseBox(int use, int unknown);

void Font_UseBoxColor(DWORD color);

void Font_SetLineWidth(float width);

void Font_SetLineHeight(float height);

void Font_SetShadow(int shadow);

void Font_ExpandKeyTokens(char* text);

DWORD Font_GetTextSubstitutionAddress(char* text, int a, int b, int c, int d, int e, int f, char* output);

#endif // FONT_H
