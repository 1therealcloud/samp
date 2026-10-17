#ifndef SAMP_RECOVERED_FONT_COLOR_H
#define SAMP_RECOVERED_FONT_COLOR_H
#include <windows.h>
#include <d3dx9.h>
// samp.dll RVA: 0x0017e97c
extern char *g_fontOriginalText;
// samp.dll RVA: 0x0017e980
extern char *g_fontStrippedText;
// samp.dll RVA: 0x00174d38
extern wchar_t g_fontWideText[20000];
// samp.dll RVA: 0x00174d30
extern ID3DXSprite *g_fontRealSprite;
// samp.dll RVA: 0x00174d20
extern D3DCOLOR g_fontGlyphColor;
// samp.dll RVA: 0x00174d34
extern int g_fontGlyphCursor;
// samp.dll RVA: 0x00073440
void FontAnsiToWide(const char *, wchar_t *, int);
// samp.dll RVA: 0x000b5980
int FontIsWideHexDigit(unsigned short);
// samp.dll RVA: 0x000b5ad0
unsigned long FontParseWideColorTag(const wchar_t *);
// samp.dll RVA: 0x00073580
void FontNextGlyphColor();
// samp.dll RVA: unknown (integration helper extracted from startup RVA 0x000c5e70)
void InitializeFontDrawBuffers();
// samp.dll RVA: unknown (recovery accessor for static sprite object RVA 0x00140bd8)
ID3DXSprite *FontColorSprite();
#endif
