// ID3DXFontHook.cpp

#include "d3dhook/ID3DXFontHook.h"

#include "gui/chatwindow.h"
#include "gui/cmdwindow.h"

#include <DXUTgui.h>

#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include <static_assert.h>

// Native vtable RVA 0x0011c76c. State is shared, as in the DLL.
class CFontColorSprite : public ID3DXSprite
{
public:
    // R5 address to verify
    CFontColorSprite();
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** output);
    // R5 address to verify
    virtual ULONG STDMETHODCALLTYPE AddRef();
    // R5 address to verify
    virtual ULONG STDMETHODCALLTYPE Release();
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** device);
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE GetTransform(D3DXMATRIX* transform);
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE SetTransform(const D3DXMATRIX* transform);
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE SetWorldViewRH(const D3DXMATRIX* world, const D3DXMATRIX* view);
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE SetWorldViewLH(const D3DXMATRIX* world, const D3DXMATRIX* view);
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE Begin(DWORD flags);
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE Draw(IDirect3DTexture9* texture, const RECT* rect, const D3DXVECTOR3* center, const D3DXVECTOR3* position, D3DCOLOR color);
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE Flush();
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE End();
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE OnLostDevice();
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE OnResetDevice();
};
// R5 address to verify
static CFontColorSprite g_fontColorSprite;
// R5 address to verify
char* g_fontOriginalText;
// R5 address to verify
char* g_fontStrippedText;
// R5 address to verify
wchar_t g_fontWideText[20000];
// R5 address to verify
ID3DXSprite* g_fontRealSprite;
// R5 address to verify
D3DCOLOR g_fontGlyphColor;
// R5 address to verify
int g_fontGlyphCursor;

// R5 address to verify
CFontColorSprite::CFontColorSprite() {}

STATIC_ASSERT(sizeof(CFontColorSprite) == 4);

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontWrapper::QueryInterface(REFIID iid, void** output)
{
    return font->QueryInterface(iid, output);
}

// R5 address to verify
ULONG STDMETHODCALLTYPE CFontWrapper::AddRef()
{
    return font->AddRef();
}

// R5 address to verify
ULONG STDMETHODCALLTYPE CFontWrapper::Release()
{
    return font->Release();
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontWrapper::GetDevice(IDirect3DDevice9** device)
{
    return font->GetDevice(device);
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontWrapper::GetDescA(D3DXFONT_DESCA* desc)
{
    return font->GetDescA(desc);
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontWrapper::GetDescW(D3DXFONT_DESCW* desc)
{
    return font->GetDescW(desc);
}

// R5 address to verify
BOOL STDMETHODCALLTYPE CFontWrapper::GetTextMetricsA(TEXTMETRICA* metrics)
{
    return font->GetTextMetricsA(metrics);
}

// R5 address to verify
BOOL STDMETHODCALLTYPE CFontWrapper::GetTextMetricsW(TEXTMETRICW* metrics)
{
    return font->GetTextMetricsW(metrics);
}

// R5 address to verify
HDC STDMETHODCALLTYPE CFontWrapper::GetDC()
{
    return font->GetDC();
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontWrapper::GetGlyphData(UINT glyph, IDirect3DTexture9** texture, RECT* box, POINT* increment)
{
    return font->GetGlyphData(glyph, texture, box, increment);
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontWrapper::PreloadCharacters(UINT first, UINT last)
{
    return font->PreloadCharacters(first, last);
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontWrapper::PreloadGlyphs(UINT first, UINT last)
{
    return font->PreloadGlyphs(first, last);
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontWrapper::PreloadTextA(const char* text, INT count)
{
    return font->PreloadTextA(text, count);
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontWrapper::PreloadTextW(const wchar_t* text, INT count)
{
    return font->PreloadTextW(text, count);
}

// R5 address to verify
INT STDMETHODCALLTYPE CFontWrapper::DrawTextA(ID3DXSprite* sprite, const char* text, INT count, RECT* rect, DWORD format, D3DCOLOR color)
{
    if (sprite)
    {
        g_fontRealSprite = sprite;
        g_fontGlyphColor = color;
        g_fontGlyphCursor = 0;
        if (strlen(text) > 100000)
            return 0;
        strcpy(g_fontOriginalText, text);
        strcpy(g_fontStrippedText, text);
        FontAnsiToWide(g_fontOriginalText, g_fontWideText, 20000);
        ChatStripColorTags(g_fontStrippedText);
        return font->DrawTextA(&g_fontColorSprite, g_fontStrippedText, (INT)strlen(g_fontStrippedText), rect, format, color);
    }
    return font->DrawTextA(0, text, count, rect, format, color);
}

// R5 address to verify
INT STDMETHODCALLTYPE CFontWrapper::DrawTextW(ID3DXSprite* sprite, const wchar_t* text, INT count, RECT* rect, DWORD format, D3DCOLOR color)
{
    return font->DrawTextW(sprite, text, count, rect, format, color);
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontWrapper::OnLostDevice()
{
    return font->OnLostDevice();
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontWrapper::OnResetDevice()
{
    return font->OnResetDevice();
}

// inline
void InitializeFontDrawBuffers()
{
    g_fontOriginalText = (char*)calloc(1, 100001);
    g_fontStrippedText = (char*)calloc(1, 100001);
}

// R5 address to verify
void FontAnsiToWide(const char* text, wchar_t* output, int capacity)
{
    memset(output, 0, capacity * sizeof(wchar_t));
    int length = MultiByteToWideChar(CP_ACP, 0, text, (int)strlen(text), 0, 0);
    if (length < capacity)
        MultiByteToWideChar(CP_ACP, 0, text, (int)strlen(text), output, length);
}

// 0x000b5e40
int FontIsWideHexDigit(unsigned short value)
{
    return (value >= '0' && value <= '9') || (value >= 'A' && value <= 'F') || (value >= 'a' && value <= 'f');
}

// 0x000b5f90
unsigned long FontParseWideColorTag(const wchar_t* text)
{
    if (text[0] && text[0] == L'{' && text[1] && FontIsWideHexDigit(text[1]) && text[2] && FontIsWideHexDigit(text[2]) && text[3] && FontIsWideHexDigit(text[3]) && text[4] &&
        FontIsWideHexDigit(text[4]) && text[5] && FontIsWideHexDigit(text[5]) && text[6] && FontIsWideHexDigit(text[6]) && text[7] && text[7] == L'}')
    {
        wchar_t digits[17];
        memset(digits, 0, sizeof(digits));
        wcsncpy(digits, text + 1, 6);
        return wcstoul(digits, 0, 16);
    }
    return 0xffffffff;
}

// R5 address to verify
void FontNextAnsiGlyphColor()
{
    char ch = g_fontOriginalText[g_fontGlyphCursor];
    while (ch && (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r'))
        ch = g_fontOriginalText[++g_fontGlyphCursor];
    unsigned long color = ChatParseColorTag(g_fontOriginalText + g_fontGlyphCursor);
    while (color != 0xffffffff)
    {
        g_fontGlyphColor = color | 0xff000000;
        g_fontGlyphCursor += 8;
        ch = g_fontOriginalText[g_fontGlyphCursor];
        while (ch && (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r'))
            ch = g_fontOriginalText[++g_fontGlyphCursor];
        color = ChatParseColorTag(g_fontOriginalText + g_fontGlyphCursor);
    }
    if (g_fontOriginalText[g_fontGlyphCursor])
    {
        int length = mblen(g_fontOriginalText + g_fontGlyphCursor, MB_CUR_MAX);
        if (length < 0)
            ++g_fontGlyphCursor;
        else
            g_fontGlyphCursor += length;
    }
}

// 0x00073ae0
void FontNextGlyphColor()
{
    wchar_t ch = g_fontWideText[g_fontGlyphCursor];
    while (ch && (ch == L' ' || ch == L'\t' || ch == L'\n' || ch == L'\r'))
        ch = g_fontWideText[++g_fontGlyphCursor];
    unsigned long color = FontParseWideColorTag(g_fontWideText + g_fontGlyphCursor);
    while (color != 0xffffffff)
    {
        g_fontGlyphColor = color | 0xff000000;
        g_fontGlyphCursor += 8;
        ch = g_fontWideText[g_fontGlyphCursor];
        while (ch && (ch == L' ' || ch == L'\t' || ch == L'\n' || ch == L'\r'))
            ch = g_fontWideText[++g_fontGlyphCursor];
        color = FontParseWideColorTag(g_fontWideText + g_fontGlyphCursor);
    }
    // Native code checks the ANSI buffer with the same index used for UTF-16.
    if (g_fontOriginalText[g_fontGlyphCursor])
        ++g_fontGlyphCursor;
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontColorSprite::QueryInterface(REFIID iid, void** output)
{
    return g_fontRealSprite->QueryInterface(iid, output);
}

// R5 address to verify
ULONG STDMETHODCALLTYPE CFontColorSprite::AddRef()
{
    return g_fontRealSprite->AddRef();
}

// R5 address to verify
ULONG STDMETHODCALLTYPE CFontColorSprite::Release()
{
    return g_fontRealSprite->Release();
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontColorSprite::GetDevice(IDirect3DDevice9** device)
{
    return g_fontRealSprite->GetDevice(device);
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontColorSprite::GetTransform(D3DXMATRIX* transform)
{
    return g_fontRealSprite->GetTransform(transform);
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontColorSprite::SetTransform(const D3DXMATRIX* transform)
{
    return g_fontRealSprite->SetTransform(transform);
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontColorSprite::SetWorldViewRH(const D3DXMATRIX* world, const D3DXMATRIX* view)
{
    return g_fontRealSprite->SetWorldViewRH(world, view);
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontColorSprite::SetWorldViewLH(const D3DXMATRIX* world, const D3DXMATRIX* view)
{
    return g_fontRealSprite->SetWorldViewLH(world, view);
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontColorSprite::Begin(DWORD flags)
{
    return g_fontRealSprite->Begin(flags);
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontColorSprite::Draw(IDirect3DTexture9* texture, const RECT* rect, const D3DXVECTOR3* center, const D3DXVECTOR3* position, D3DCOLOR color)
{
    FontNextGlyphColor();
    return g_fontRealSprite->Draw(texture, rect, center, position, g_fontGlyphColor);
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontColorSprite::Flush()
{
    return g_fontRealSprite->Flush();
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontColorSprite::End()
{
    return g_fontRealSprite->End();
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontColorSprite::OnLostDevice()
{
    return g_fontRealSprite->OnLostDevice();
}

// R5 address to verify
HRESULT STDMETHODCALLTYPE CFontColorSprite::OnResetDevice()
{
    return g_fontRealSprite->OnResetDevice();
}
