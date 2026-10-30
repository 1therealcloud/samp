#include "d3dhook/ID3DXFontHook.h"
#include "cmdwindow.h"
#include "runutil.h"
#include <DXUTgui.h>
#include <string.h>

// 0x0006b310
CFontWrapper::CFontWrapper() {}

// 0x00073770
HRESULT STDMETHODCALLTYPE CFontWrapper::QueryInterface(REFIID iid, void** output)
{
    return font->QueryInterface(iid, output);
}

// 0x00073780
ULONG STDMETHODCALLTYPE CFontWrapper::AddRef()
{
    return font->AddRef();
}

// 0x00073790
ULONG STDMETHODCALLTYPE CFontWrapper::Release()
{
    return font->Release();
}

// 0x000737a0
HRESULT STDMETHODCALLTYPE CFontWrapper::GetDevice(IDirect3DDevice9** device)
{
    return font->GetDevice(device);
}

// 0x000737b0
HRESULT STDMETHODCALLTYPE CFontWrapper::GetDescA(D3DXFONT_DESCA* desc)
{
    return font->GetDescA(desc);
}

// 0x000737c0
HRESULT STDMETHODCALLTYPE CFontWrapper::GetDescW(D3DXFONT_DESCW* desc)
{
    return font->GetDescW(desc);
}

// 0x000737d0
BOOL STDMETHODCALLTYPE CFontWrapper::GetTextMetricsA(TEXTMETRICA* metrics)
{
    return font->GetTextMetricsA(metrics);
}

// 0x000737e0
BOOL STDMETHODCALLTYPE CFontWrapper::GetTextMetricsW(TEXTMETRICW* metrics)
{
    return font->GetTextMetricsW(metrics);
}

// 0x000737f0
HDC STDMETHODCALLTYPE CFontWrapper::GetDC()
{
    return font->GetDC();
}

// 0x00073800
HRESULT STDMETHODCALLTYPE CFontWrapper::GetGlyphData(UINT glyph, IDirect3DTexture9** texture, RECT* box, POINT* increment)
{
    return font->GetGlyphData(glyph, texture, box, increment);
}

// 0x00073810
HRESULT STDMETHODCALLTYPE CFontWrapper::PreloadCharacters(UINT first, UINT last)
{
    return font->PreloadCharacters(first, last);
}

// 0x00073820
HRESULT STDMETHODCALLTYPE CFontWrapper::PreloadGlyphs(UINT first, UINT last)
{
    return font->PreloadGlyphs(first, last);
}

// 0x00073830
HRESULT STDMETHODCALLTYPE CFontWrapper::PreloadTextA(const char* text, INT count)
{
    return font->PreloadTextA(text, count);
}

// 0x00073840
HRESULT STDMETHODCALLTYPE CFontWrapper::PreloadTextW(const wchar_t* text, INT count)
{
    return font->PreloadTextW(text, count);
}

// 0x00073680
INT STDMETHODCALLTYPE CFontWrapper::DrawTextA(ID3DXSprite* sprite, const char* text, INT count, RECT* rect, DWORD format, D3DCOLOR color)
{
    if (!sprite)
        return font->DrawTextA(0, text, count, rect, format, color);
    g_fontRealSprite = sprite;
    g_fontGlyphColor = color;
    g_fontGlyphCursor = 0;
    if (strlen(text) >= 100001)
        return 0;
    strcpy(g_fontOriginalText, text);
    strcpy(g_fontStrippedText, text);
    FontAnsiToWide(g_fontOriginalText, g_fontWideText, 20000);
    ChatStripColorTags(g_fontStrippedText);
    return font->DrawTextA(FontColorSprite(), g_fontStrippedText, (INT)strlen(g_fontStrippedText), rect, format, color);
}

// 0x00073850
INT STDMETHODCALLTYPE CFontWrapper::DrawTextW(ID3DXSprite* sprite, const wchar_t* text, INT count, RECT* rect, DWORD format, D3DCOLOR color)
{
    return font->DrawTextW(sprite, text, count, rect, format, color);
}

// 0x00073860
HRESULT STDMETHODCALLTYPE CFontWrapper::OnLostDevice()
{
    return font->OnLostDevice();
}

// 0x00073870
HRESULT STDMETHODCALLTYPE CFontWrapper::OnResetDevice()
{
    return font->OnResetDevice();
}

#include <stdlib.h>
#include <wchar.h>
// 0x0017e97c
char* g_fontOriginalText;
// 0x0017e980
char* g_fontStrippedText;
// 0x00174d38
wchar_t g_fontWideText[20000];
// 0x00174d30
ID3DXSprite* g_fontRealSprite;
// 0x00174d20
D3DCOLOR g_fontGlyphColor;
// 0x00174d34
int g_fontGlyphCursor;

// samp.dll RVA: unknown (integration helper extracted from startup RVA 0x000c5e70)
void InitializeFontDrawBuffers()
{
    g_fontOriginalText = (char*)calloc(1, 100001);
    g_fontStrippedText = (char*)calloc(1, 100001);
}

// 0x00073440
void FontAnsiToWide(const char* text, wchar_t* output, int capacity)
{
    memset(output, 0, capacity * sizeof(wchar_t));
    int length = MultiByteToWideChar(CP_ACP, 0, text, (int)strlen(text), 0, 0);
    if (length < capacity)
        MultiByteToWideChar(CP_ACP, 0, text, (int)strlen(text), output, length);
}

// 0x000b5980
int FontIsWideHexDigit(unsigned short value)
{
    return (value >= '0' && value <= '9') || (value >= 'A' && value <= 'F') || (value >= 'a' && value <= 'f');
}

// 0x000b5ad0
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

// 0x00073580
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

// Native vtable RVA 0x0011c76c. State is shared, as in the DLL.
class CFontColorSprite : public ID3DXSprite
{
public:
    // 0x00073960
    CFontColorSprite();
    // 0x00073880
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** output);
    // 0x00073890
    virtual ULONG STDMETHODCALLTYPE AddRef();
    // 0x000738a0
    virtual ULONG STDMETHODCALLTYPE Release();
    // 0x000738b0
    virtual HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** device);
    // 0x000738c0
    virtual HRESULT STDMETHODCALLTYPE GetTransform(D3DXMATRIX* transform);
    // 0x000738d0
    virtual HRESULT STDMETHODCALLTYPE SetTransform(const D3DXMATRIX* transform);
    // 0x000738e0
    virtual HRESULT STDMETHODCALLTYPE SetWorldViewRH(const D3DXMATRIX* world, const D3DXMATRIX* view);
    // 0x000738f0
    virtual HRESULT STDMETHODCALLTYPE SetWorldViewLH(const D3DXMATRIX* world, const D3DXMATRIX* view);
    // 0x00073900
    virtual HRESULT STDMETHODCALLTYPE Begin(DWORD flags);
    // 0x00073660
    virtual HRESULT STDMETHODCALLTYPE Draw(IDirect3DTexture9* texture, const RECT* rect, const D3DXVECTOR3* center, const D3DXVECTOR3* position, D3DCOLOR color);
    // 0x00073910
    virtual HRESULT STDMETHODCALLTYPE Flush();
    // 0x00073920
    virtual HRESULT STDMETHODCALLTYPE End();
    // 0x00073930
    virtual HRESULT STDMETHODCALLTYPE OnLostDevice();
    // 0x00073940
    virtual HRESULT STDMETHODCALLTYPE OnResetDevice();
};
// 0x00073960
CFontColorSprite::CFontColorSprite() {}

typedef char FontColorSpriteSize[(sizeof(CFontColorSprite) == 4) ? 1 : -1];
// 0x00140bd8
static CFontColorSprite g_fontColorSprite;

// samp.dll RVA: unknown (recovery accessor for static sprite object RVA 0x00140bd8)
ID3DXSprite* FontColorSprite()
{
    return &g_fontColorSprite;
}

// 0x00073880
HRESULT STDMETHODCALLTYPE CFontColorSprite::QueryInterface(REFIID iid, void** output)
{
    return g_fontRealSprite->QueryInterface(iid, output);
}

// 0x00073890
ULONG STDMETHODCALLTYPE CFontColorSprite::AddRef()
{
    return g_fontRealSprite->AddRef();
}

// 0x000738a0
ULONG STDMETHODCALLTYPE CFontColorSprite::Release()
{
    return g_fontRealSprite->Release();
}

// 0x000738b0
HRESULT STDMETHODCALLTYPE CFontColorSprite::GetDevice(IDirect3DDevice9** device)
{
    return g_fontRealSprite->GetDevice(device);
}

// 0x000738c0
HRESULT STDMETHODCALLTYPE CFontColorSprite::GetTransform(D3DXMATRIX* transform)
{
    return g_fontRealSprite->GetTransform(transform);
}

// 0x000738d0
HRESULT STDMETHODCALLTYPE CFontColorSprite::SetTransform(const D3DXMATRIX* transform)
{
    return g_fontRealSprite->SetTransform(transform);
}

// 0x000738e0
HRESULT STDMETHODCALLTYPE CFontColorSprite::SetWorldViewRH(const D3DXMATRIX* world, const D3DXMATRIX* view)
{
    return g_fontRealSprite->SetWorldViewRH(world, view);
}

// 0x000738f0
HRESULT STDMETHODCALLTYPE CFontColorSprite::SetWorldViewLH(const D3DXMATRIX* world, const D3DXMATRIX* view)
{
    return g_fontRealSprite->SetWorldViewLH(world, view);
}

// 0x00073900
HRESULT STDMETHODCALLTYPE CFontColorSprite::Begin(DWORD flags)
{
    return g_fontRealSprite->Begin(flags);
}

// 0x00073660
HRESULT STDMETHODCALLTYPE CFontColorSprite::Draw(IDirect3DTexture9* texture, const RECT* rect, const D3DXVECTOR3* center, const D3DXVECTOR3* position, D3DCOLOR color)
{
    FontNextGlyphColor();
    return g_fontRealSprite->Draw(texture, rect, center, position, g_fontGlyphColor);
}

// 0x00073910
HRESULT STDMETHODCALLTYPE CFontColorSprite::Flush()
{
    return g_fontRealSprite->Flush();
}

// 0x00073920
HRESULT STDMETHODCALLTYPE CFontColorSprite::End()
{
    return g_fontRealSprite->End();
}

// 0x00073930
HRESULT STDMETHODCALLTYPE CFontColorSprite::OnLostDevice()
{
    return g_fontRealSprite->OnLostDevice();
}

// 0x00073940
HRESULT STDMETHODCALLTYPE CFontColorSprite::OnResetDevice()
{
    return g_fontRealSprite->OnResetDevice();
}
