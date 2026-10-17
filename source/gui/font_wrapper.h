#ifndef SAMP_RECOVERED_FONT_WRAPPER_H
#define SAMP_RECOVERED_FONT_WRAPPER_H
#include <windows.h>
#include <d3dx9.h>
// Native vtable RVA 0x0011c438. No added destructor or reference count.
class CFontWrapper : public ID3DXFont {
public:
    ID3DXFont *font;
    // samp.dll RVA: 0x00073770
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **output);
    // samp.dll RVA: 0x00073780
    virtual ULONG STDMETHODCALLTYPE AddRef();
    // samp.dll RVA: 0x00073790
    virtual ULONG STDMETHODCALLTYPE Release();
    // samp.dll RVA: 0x000737a0
    virtual HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9 **device);
    // samp.dll RVA: 0x000737b0
    virtual HRESULT STDMETHODCALLTYPE GetDescA(D3DXFONT_DESCA *desc);
    // samp.dll RVA: 0x000737c0
    virtual HRESULT STDMETHODCALLTYPE GetDescW(D3DXFONT_DESCW *desc);
    // samp.dll RVA: 0x000737d0
    virtual BOOL STDMETHODCALLTYPE GetTextMetricsA(TEXTMETRICA *metrics);
    // samp.dll RVA: 0x000737e0
    virtual BOOL STDMETHODCALLTYPE GetTextMetricsW(TEXTMETRICW *metrics);
    // samp.dll RVA: 0x000737f0
    virtual HDC STDMETHODCALLTYPE GetDC();
    // samp.dll RVA: 0x00073800
    virtual HRESULT STDMETHODCALLTYPE GetGlyphData(UINT glyph, IDirect3DTexture9 **texture, RECT *box, POINT *increment);
    // samp.dll RVA: 0x00073810
    virtual HRESULT STDMETHODCALLTYPE PreloadCharacters(UINT first, UINT last);
    // samp.dll RVA: 0x00073820
    virtual HRESULT STDMETHODCALLTYPE PreloadGlyphs(UINT first, UINT last);
    // samp.dll RVA: 0x00073830
    virtual HRESULT STDMETHODCALLTYPE PreloadTextA(const char *text, INT count);
    // samp.dll RVA: 0x00073840
    virtual HRESULT STDMETHODCALLTYPE PreloadTextW(const wchar_t *text, INT count);
    // samp.dll RVA: 0x00073680
    virtual INT STDMETHODCALLTYPE DrawTextA(ID3DXSprite *sprite, const char *text, INT count, RECT *rect, DWORD format, D3DCOLOR color);
    // samp.dll RVA: 0x00073850
    virtual INT STDMETHODCALLTYPE DrawTextW(ID3DXSprite *sprite, const wchar_t *text, INT count, RECT *rect, DWORD format, D3DCOLOR color);
    // samp.dll RVA: 0x00073860
    virtual HRESULT STDMETHODCALLTYPE OnLostDevice();
    // samp.dll RVA: 0x00073870
    virtual HRESULT STDMETHODCALLTYPE OnResetDevice();
};
typedef char FontWrapperSize[(sizeof(CFontWrapper) == 8) ? 1 : -1];
#endif

