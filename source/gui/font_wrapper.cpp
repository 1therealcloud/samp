#include "gui/font_wrapper.h"
#include "gui/font_color.h"
#include "gui/chat_dependencies.h"
#include <string.h>

// samp.dll RVA: 0x00073770
HRESULT STDMETHODCALLTYPE CFontWrapper::QueryInterface(REFIID iid, void **output) {
    return font->QueryInterface(iid, output);
}

// samp.dll RVA: 0x00073780
ULONG STDMETHODCALLTYPE CFontWrapper::AddRef() {
    return font->AddRef();
}

// samp.dll RVA: 0x00073790
ULONG STDMETHODCALLTYPE CFontWrapper::Release() {
    return font->Release();
}

// samp.dll RVA: 0x000737a0
HRESULT STDMETHODCALLTYPE CFontWrapper::GetDevice(IDirect3DDevice9 **device) {
    return font->GetDevice(device);
}

// samp.dll RVA: 0x000737b0
HRESULT STDMETHODCALLTYPE CFontWrapper::GetDescA(D3DXFONT_DESCA *desc) {
    return font->GetDescA(desc);
}

// samp.dll RVA: 0x000737c0
HRESULT STDMETHODCALLTYPE CFontWrapper::GetDescW(D3DXFONT_DESCW *desc) {
    return font->GetDescW(desc);
}

// samp.dll RVA: 0x000737d0
BOOL STDMETHODCALLTYPE CFontWrapper::GetTextMetricsA(TEXTMETRICA *metrics) {
    return font->GetTextMetricsA(metrics);
}

// samp.dll RVA: 0x000737e0
BOOL STDMETHODCALLTYPE CFontWrapper::GetTextMetricsW(TEXTMETRICW *metrics) {
    return font->GetTextMetricsW(metrics);
}

// samp.dll RVA: 0x000737f0
HDC STDMETHODCALLTYPE CFontWrapper::GetDC() {
    return font->GetDC();
}

// samp.dll RVA: 0x00073800
HRESULT STDMETHODCALLTYPE CFontWrapper::GetGlyphData(UINT glyph, IDirect3DTexture9 **texture, RECT *box, POINT *increment) {
    return font->GetGlyphData(glyph, texture, box, increment);
}

// samp.dll RVA: 0x00073810
HRESULT STDMETHODCALLTYPE CFontWrapper::PreloadCharacters(UINT first, UINT last) {
    return font->PreloadCharacters(first, last);
}

// samp.dll RVA: 0x00073820
HRESULT STDMETHODCALLTYPE CFontWrapper::PreloadGlyphs(UINT first, UINT last) {
    return font->PreloadGlyphs(first, last);
}

// samp.dll RVA: 0x00073830
HRESULT STDMETHODCALLTYPE CFontWrapper::PreloadTextA(const char *text, INT count) {
    return font->PreloadTextA(text, count);
}

// samp.dll RVA: 0x00073840
HRESULT STDMETHODCALLTYPE CFontWrapper::PreloadTextW(const wchar_t *text, INT count) {
    return font->PreloadTextW(text, count);
}

// samp.dll RVA: 0x00073680
INT STDMETHODCALLTYPE CFontWrapper::DrawTextA(ID3DXSprite *sprite, const char *text, INT count, RECT *rect, DWORD format, D3DCOLOR color) {
    if (!sprite) return font->DrawTextA(0, text, count, rect, format, color);
    g_fontRealSprite = sprite;
    g_fontGlyphColor = color;
    g_fontGlyphCursor = 0;
    if (strlen(text) >= 100001) return 0;
    strcpy(g_fontOriginalText, text);
    strcpy(g_fontStrippedText, text);
    FontAnsiToWide(g_fontOriginalText, g_fontWideText, 20000);
    ChatStripColorTags(g_fontStrippedText);
    return font->DrawTextA(FontColorSprite(), g_fontStrippedText,
        (INT)strlen(g_fontStrippedText), rect, format, color);
}

// samp.dll RVA: 0x00073850
INT STDMETHODCALLTYPE CFontWrapper::DrawTextW(ID3DXSprite *sprite, const wchar_t *text, INT count, RECT *rect, DWORD format, D3DCOLOR color) {
    return font->DrawTextW(sprite, text, count, rect, format, color);
}

// samp.dll RVA: 0x00073860
HRESULT STDMETHODCALLTYPE CFontWrapper::OnLostDevice() {
    return font->OnLostDevice();
}

// samp.dll RVA: 0x00073870
HRESULT STDMETHODCALLTYPE CFontWrapper::OnResetDevice() {
    return font->OnResetDevice();
}


