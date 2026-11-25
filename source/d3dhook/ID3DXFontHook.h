// ID3DXFontHook
#ifndef ID3DXFONTHOOK_H
#define ID3DXFONTHOOK_H

#include <d3dx9.h>

#include <windows.h>

// Native vtable RVA 0x0011c438. No added destructor or reference count.
class CFontWrapper : public ID3DXFont
{
public:
    // Initializes only the COM vptr; wrapped font pointer is untouched.

    CFontWrapper() {}
    ID3DXFont* font;

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** output);

    virtual ULONG STDMETHODCALLTYPE AddRef();

    virtual ULONG STDMETHODCALLTYPE Release();

    virtual HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** device);

    virtual HRESULT STDMETHODCALLTYPE GetDescA(D3DXFONT_DESCA* desc);

    virtual HRESULT STDMETHODCALLTYPE GetDescW(D3DXFONT_DESCW* desc);

    virtual BOOL STDMETHODCALLTYPE GetTextMetricsA(TEXTMETRICA* metrics);

    virtual BOOL STDMETHODCALLTYPE GetTextMetricsW(TEXTMETRICW* metrics);

    virtual HDC STDMETHODCALLTYPE GetDC();

    virtual HRESULT STDMETHODCALLTYPE GetGlyphData(UINT glyph, IDirect3DTexture9** texture, RECT* box, POINT* increment);

    virtual HRESULT STDMETHODCALLTYPE PreloadCharacters(UINT first, UINT last);

    virtual HRESULT STDMETHODCALLTYPE PreloadGlyphs(UINT first, UINT last);

    virtual HRESULT STDMETHODCALLTYPE PreloadTextA(const char* text, INT count);

    virtual HRESULT STDMETHODCALLTYPE PreloadTextW(const wchar_t* text, INT count);

    virtual INT STDMETHODCALLTYPE DrawTextA(ID3DXSprite* sprite, const char* text, INT count, RECT* rect, DWORD format, D3DCOLOR color);

    virtual INT STDMETHODCALLTYPE DrawTextW(ID3DXSprite* sprite, const wchar_t* text, INT count, RECT* rect, DWORD format, D3DCOLOR color);

    virtual HRESULT STDMETHODCALLTYPE OnLostDevice();

    virtual HRESULT STDMETHODCALLTYPE OnResetDevice();
};

static_assert(sizeof(CFontWrapper) == 8);

extern char* g_fontOriginalText;

extern char* g_fontStrippedText;

extern wchar_t g_fontWideText[20000];

extern ID3DXSprite* g_fontRealSprite;

extern D3DCOLOR g_fontGlyphColor;

extern int g_fontGlyphCursor;

void FontAnsiToWide(const char*, wchar_t*, int);

int FontIsWideHexDigit(unsigned short);

unsigned long FontParseWideColorTag(const wchar_t*);

void FontNextAnsiGlyphColor();

void FontNextGlyphColor();

void InitializeFontDrawBuffers();

#endif // ID3DXFONTHOOK_H
