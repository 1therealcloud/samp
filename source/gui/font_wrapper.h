#ifndef SAMP_RECOVERED_FONT_WRAPPER_H
#define SAMP_RECOVERED_FONT_WRAPPER_H
#include <d3dx9.h>
#include <windows.h>
// Native vtable RVA 0x0011c438. No added destructor or reference count.
class CFontWrapper : public ID3DXFont
{
public:
    // Initializes only the COM vptr; wrapped font pointer is untouched.
    // samp.dll RVA: 0x0006b310
    CFontWrapper();
    ID3DXFont* font;
    // samp.dll RVA: 0x00073770
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** output);
    // samp.dll RVA: 0x00073780
    virtual ULONG STDMETHODCALLTYPE AddRef();
    // samp.dll RVA: 0x00073790
    virtual ULONG STDMETHODCALLTYPE Release();
    // samp.dll RVA: 0x000737a0
    virtual HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9** device);
    // samp.dll RVA: 0x000737b0
    virtual HRESULT STDMETHODCALLTYPE GetDescA(D3DXFONT_DESCA* desc);
    // samp.dll RVA: 0x000737c0
    virtual HRESULT STDMETHODCALLTYPE GetDescW(D3DXFONT_DESCW* desc);
    // samp.dll RVA: 0x000737d0
    virtual BOOL STDMETHODCALLTYPE GetTextMetricsA(TEXTMETRICA* metrics);
    // samp.dll RVA: 0x000737e0
    virtual BOOL STDMETHODCALLTYPE GetTextMetricsW(TEXTMETRICW* metrics);
    // samp.dll RVA: 0x000737f0
    virtual HDC STDMETHODCALLTYPE GetDC();
    // samp.dll RVA: 0x00073800
    virtual HRESULT STDMETHODCALLTYPE GetGlyphData(UINT glyph, IDirect3DTexture9** texture, RECT* box, POINT* increment);
    // samp.dll RVA: 0x00073810
    virtual HRESULT STDMETHODCALLTYPE PreloadCharacters(UINT first, UINT last);
    // samp.dll RVA: 0x00073820
    virtual HRESULT STDMETHODCALLTYPE PreloadGlyphs(UINT first, UINT last);
    // samp.dll RVA: 0x00073830
    virtual HRESULT STDMETHODCALLTYPE PreloadTextA(const char* text, INT count);
    // samp.dll RVA: 0x00073840
    virtual HRESULT STDMETHODCALLTYPE PreloadTextW(const wchar_t* text, INT count);
    // samp.dll RVA: 0x00073680
    virtual INT STDMETHODCALLTYPE DrawTextA(ID3DXSprite* sprite, const char* text, INT count, RECT* rect, DWORD format, D3DCOLOR color);
    // samp.dll RVA: 0x00073850
    virtual INT STDMETHODCALLTYPE DrawTextW(ID3DXSprite* sprite, const wchar_t* text, INT count, RECT* rect, DWORD format, D3DCOLOR color);
    // samp.dll RVA: 0x00073860
    virtual HRESULT STDMETHODCALLTYPE OnLostDevice();
    // samp.dll RVA: 0x00073870
    virtual HRESULT STDMETHODCALLTYPE OnResetDevice();
};
typedef char FontWrapperSize[(sizeof(CFontWrapper) == 8) ? 1 : -1];
#endif

#ifndef SAMP_RECOVERED_FONT_COLOR_H
#define SAMP_RECOVERED_FONT_COLOR_H
// samp.dll RVA: 0x0017e97c
extern char* g_fontOriginalText;
// samp.dll RVA: 0x0017e980
extern char* g_fontStrippedText;
// samp.dll RVA: 0x00174d38
extern wchar_t g_fontWideText[20000];
// samp.dll RVA: 0x00174d30
extern ID3DXSprite* g_fontRealSprite;
// samp.dll RVA: 0x00174d20
extern D3DCOLOR g_fontGlyphColor;
// samp.dll RVA: 0x00174d34
extern int g_fontGlyphCursor;
// samp.dll RVA: 0x00073440
void FontAnsiToWide(const char*, wchar_t*, int);
// samp.dll RVA: 0x000b5980
int FontIsWideHexDigit(unsigned short);
// samp.dll RVA: 0x000b5ad0
unsigned long FontParseWideColorTag(const wchar_t*);
// samp.dll RVA: 0x00073580
void FontNextGlyphColor();
// samp.dll RVA: unknown (integration helper extracted from startup RVA 0x000c5e70)
void InitializeFontDrawBuffers();
// samp.dll RVA: unknown (recovery accessor for static sprite object RVA 0x00140bd8)
ID3DXSprite* FontColorSprite();
#endif
