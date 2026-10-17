#include "gui/font_color.h"

// Native vtable RVA 0x0011c76c. State is shared, as in the DLL.
class CFontColorSprite : public ID3DXSprite {
public:
    // samp.dll RVA: 0x00073880
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **output);
    // samp.dll RVA: 0x00073890
    virtual ULONG STDMETHODCALLTYPE AddRef();
    // samp.dll RVA: 0x000738a0
    virtual ULONG STDMETHODCALLTYPE Release();
    // samp.dll RVA: 0x000738b0
    virtual HRESULT STDMETHODCALLTYPE GetDevice(IDirect3DDevice9 **device);
    // samp.dll RVA: 0x000738c0
    virtual HRESULT STDMETHODCALLTYPE GetTransform(D3DXMATRIX *transform);
    // samp.dll RVA: 0x000738d0
    virtual HRESULT STDMETHODCALLTYPE SetTransform(const D3DXMATRIX *transform);
    // samp.dll RVA: 0x000738e0
    virtual HRESULT STDMETHODCALLTYPE SetWorldViewRH(const D3DXMATRIX *world, const D3DXMATRIX *view);
    // samp.dll RVA: 0x000738f0
    virtual HRESULT STDMETHODCALLTYPE SetWorldViewLH(const D3DXMATRIX *world, const D3DXMATRIX *view);
    // samp.dll RVA: 0x00073900
    virtual HRESULT STDMETHODCALLTYPE Begin(DWORD flags);
    // samp.dll RVA: 0x00073660
    virtual HRESULT STDMETHODCALLTYPE Draw(IDirect3DTexture9 *texture, const RECT *rect, const D3DXVECTOR3 *center, const D3DXVECTOR3 *position, D3DCOLOR color);
    // samp.dll RVA: 0x00073910
    virtual HRESULT STDMETHODCALLTYPE Flush();
    // samp.dll RVA: 0x00073920
    virtual HRESULT STDMETHODCALLTYPE End();
    // samp.dll RVA: 0x00073930
    virtual HRESULT STDMETHODCALLTYPE OnLostDevice();
    // samp.dll RVA: 0x00073940
    virtual HRESULT STDMETHODCALLTYPE OnResetDevice();
};
typedef char FontColorSpriteSize[(sizeof(CFontColorSprite) == 4) ? 1 : -1];
// samp.dll RVA: 0x00140bd8
static CFontColorSprite g_fontColorSprite;

// samp.dll RVA: unknown (recovery accessor for static sprite object RVA 0x00140bd8)
ID3DXSprite *FontColorSprite() { return &g_fontColorSprite; }

// samp.dll RVA: 0x00073880
HRESULT STDMETHODCALLTYPE CFontColorSprite::QueryInterface(REFIID iid, void **output) {
    return g_fontRealSprite->QueryInterface(iid, output);
}

// samp.dll RVA: 0x00073890
ULONG STDMETHODCALLTYPE CFontColorSprite::AddRef() {
    return g_fontRealSprite->AddRef();
}

// samp.dll RVA: 0x000738a0
ULONG STDMETHODCALLTYPE CFontColorSprite::Release() {
    return g_fontRealSprite->Release();
}

// samp.dll RVA: 0x000738b0
HRESULT STDMETHODCALLTYPE CFontColorSprite::GetDevice(IDirect3DDevice9 **device) {
    return g_fontRealSprite->GetDevice(device);
}

// samp.dll RVA: 0x000738c0
HRESULT STDMETHODCALLTYPE CFontColorSprite::GetTransform(D3DXMATRIX *transform) {
    return g_fontRealSprite->GetTransform(transform);
}

// samp.dll RVA: 0x000738d0
HRESULT STDMETHODCALLTYPE CFontColorSprite::SetTransform(const D3DXMATRIX *transform) {
    return g_fontRealSprite->SetTransform(transform);
}

// samp.dll RVA: 0x000738e0
HRESULT STDMETHODCALLTYPE CFontColorSprite::SetWorldViewRH(const D3DXMATRIX *world, const D3DXMATRIX *view) {
    return g_fontRealSprite->SetWorldViewRH(world, view);
}

// samp.dll RVA: 0x000738f0
HRESULT STDMETHODCALLTYPE CFontColorSprite::SetWorldViewLH(const D3DXMATRIX *world, const D3DXMATRIX *view) {
    return g_fontRealSprite->SetWorldViewLH(world, view);
}

// samp.dll RVA: 0x00073900
HRESULT STDMETHODCALLTYPE CFontColorSprite::Begin(DWORD flags) {
    return g_fontRealSprite->Begin(flags);
}

// samp.dll RVA: 0x00073660
HRESULT STDMETHODCALLTYPE CFontColorSprite::Draw(IDirect3DTexture9 *texture, const RECT *rect, const D3DXVECTOR3 *center, const D3DXVECTOR3 *position, D3DCOLOR color) {
    FontNextGlyphColor();
    return g_fontRealSprite->Draw(texture, rect, center, position, g_fontGlyphColor);
}

// samp.dll RVA: 0x00073910
HRESULT STDMETHODCALLTYPE CFontColorSprite::Flush() {
    return g_fontRealSprite->Flush();
}

// samp.dll RVA: 0x00073920
HRESULT STDMETHODCALLTYPE CFontColorSprite::End() {
    return g_fontRealSprite->End();
}

// samp.dll RVA: 0x00073930
HRESULT STDMETHODCALLTYPE CFontColorSprite::OnLostDevice() {
    return g_fontRealSprite->OnLostDevice();
}

// samp.dll RVA: 0x00073940
HRESULT STDMETHODCALLTYPE CFontColorSprite::OnResetDevice() {
    return g_fontRealSprite->OnResetDevice();
}


