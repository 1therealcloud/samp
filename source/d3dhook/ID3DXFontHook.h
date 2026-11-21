// ID3DXFontHook
#ifndef ID3DXFONTHOOK_H
#define ID3DXFONTHOOK_H

#include <d3dx9.h>

#include <windows.h>

#include <static_assert.h>

// Native vtable RVA 0x0011c438. No added destructor or reference count.
class CFontWrapper : public ID3DXFont
{
public:
    // Initializes only the COM vptr; wrapped font pointer is untouched.
    // 0x0006b8d0
    CFontWrapper() {}
    ID3DXFont* font;
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** output);
    // R5 address to verify
    virtual ULONG STDMETHODCALLTYPE AddRef();
    // R5 address to verify
    virtual ULONG STDMETHODCALLTYPE Release();
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** device);
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE GetDescA(D3DXFONT_DESCA* desc);
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE GetDescW(D3DXFONT_DESCW* desc);
    // R5 address to verify
    virtual BOOL STDMETHODCALLTYPE GetTextMetricsA(TEXTMETRICA* metrics);
    // R5 address to verify
    virtual BOOL STDMETHODCALLTYPE GetTextMetricsW(TEXTMETRICW* metrics);
    // R5 address to verify
    virtual HDC STDMETHODCALLTYPE GetDC();
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE GetGlyphData(UINT glyph, IDirect3DTexture9** texture, RECT* box, POINT* increment);
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE PreloadCharacters(UINT first, UINT last);
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE PreloadGlyphs(UINT first, UINT last);
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE PreloadTextA(const char* text, INT count);
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE PreloadTextW(const wchar_t* text, INT count);
    // R5 address to verify
    virtual INT STDMETHODCALLTYPE DrawTextA(ID3DXSprite* sprite, const char* text, INT count, RECT* rect, DWORD format, D3DCOLOR color);
    // R5 address to verify
    virtual INT STDMETHODCALLTYPE DrawTextW(ID3DXSprite* sprite, const wchar_t* text, INT count, RECT* rect, DWORD format, D3DCOLOR color);
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE OnLostDevice();
    // R5 address to verify
    virtual HRESULT STDMETHODCALLTYPE OnResetDevice();
};

STATIC_ASSERT(sizeof(CFontWrapper) == 8);

// R5 address to verify
extern char* g_fontOriginalText;
// R5 address to verify
extern char* g_fontStrippedText;
// R5 address to verify
extern wchar_t g_fontWideText[20000];
// R5 address to verify
extern ID3DXSprite* g_fontRealSprite;
// R5 address to verify
extern D3DCOLOR g_fontGlyphColor;
// R5 address to verify
extern int g_fontGlyphCursor;
// R5 address to verify
void FontAnsiToWide(const char*, wchar_t*, int);
// 0x000b5e40
int FontIsWideHexDigit(unsigned short);
// 0x000b5f90
unsigned long FontParseWideColorTag(const wchar_t*);
// R5 address to verify
void FontNextAnsiGlyphColor();
// 0x00073ae0
void FontNextGlyphColor();
// inline
void InitializeFontDrawBuffers();

#endif // ID3DXFONTHOOK_H
