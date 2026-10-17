#include "gui/font_color.h"
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
// samp.dll RVA: 0x0017e97c
char *g_fontOriginalText;
// samp.dll RVA: 0x0017e980
char *g_fontStrippedText;
// samp.dll RVA: 0x00174d38
wchar_t g_fontWideText[20000];
// samp.dll RVA: 0x00174d30
ID3DXSprite *g_fontRealSprite;
// samp.dll RVA: 0x00174d20
D3DCOLOR g_fontGlyphColor;
// samp.dll RVA: 0x00174d34
int g_fontGlyphCursor;

// samp.dll RVA: unknown (integration helper extracted from startup RVA 0x000c5e70)
void InitializeFontDrawBuffers() {
    g_fontOriginalText = (char *)calloc(1, 100001);
    g_fontStrippedText = (char *)calloc(1, 100001);
}

// samp.dll RVA: 0x00073440
void FontAnsiToWide(const char *text, wchar_t *output, int capacity) {
    memset(output, 0, capacity * sizeof(wchar_t));
    int length = MultiByteToWideChar(CP_ACP, 0, text, (int)strlen(text), 0, 0);
    if (length < capacity)
        MultiByteToWideChar(CP_ACP, 0, text, (int)strlen(text), output, length);
}

// samp.dll RVA: 0x000b5980
int FontIsWideHexDigit(unsigned short value) {
    return (value >= '0' && value <= '9') ||
        (value >= 'A' && value <= 'F') || (value >= 'a' && value <= 'f');
}

// samp.dll RVA: 0x000b5ad0
unsigned long FontParseWideColorTag(const wchar_t *text) {
    if (text[0] && text[0] == L'{' &&
        text[1] && FontIsWideHexDigit(text[1]) &&
        text[2] && FontIsWideHexDigit(text[2]) &&
        text[3] && FontIsWideHexDigit(text[3]) &&
        text[4] && FontIsWideHexDigit(text[4]) &&
        text[5] && FontIsWideHexDigit(text[5]) &&
        text[6] && FontIsWideHexDigit(text[6]) && text[7] && text[7] == L'}') {
        wchar_t digits[17];
        memset(digits, 0, sizeof(digits));
        wcsncpy(digits, text + 1, 6);
        return wcstoul(digits, 0, 16);
    }
    return 0xffffffff;
}

// samp.dll RVA: 0x00073580
void FontNextGlyphColor() {
    wchar_t ch = g_fontWideText[g_fontGlyphCursor];
    while (ch && (ch == L' ' || ch == L'\t' || ch == L'\n' || ch == L'\r'))
        ch = g_fontWideText[++g_fontGlyphCursor];
    unsigned long color = FontParseWideColorTag(g_fontWideText + g_fontGlyphCursor);
    while (color != 0xffffffff) {
        g_fontGlyphColor = color | 0xff000000;
        g_fontGlyphCursor += 8;
        ch = g_fontWideText[g_fontGlyphCursor];
        while (ch && (ch == L' ' || ch == L'\t' || ch == L'\n' || ch == L'\r'))
            ch = g_fontWideText[++g_fontGlyphCursor];
        color = FontParseWideColorTag(g_fontWideText + g_fontGlyphCursor);
    }
    // Native code checks the ANSI buffer with the same index used for UTF-16.
    if (g_fontOriginalText[g_fontGlyphCursor]) ++g_fontGlyphCursor;
}
