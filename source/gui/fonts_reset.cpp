#include "fontrender.h"
#include "gui/font_wrapper.h"
#include "gui/font_settings.h"
#include <stdlib.h>

// samp.dll RVA: 0x0006b320
void CFonts::Reset() {
    if (!m_pDevice) return;
    if (m_pFont) { m_pFont->Release(); m_pFont = 0; }
    if (m_pLittleFont) { m_pLittleFont->Release(); m_pLittleFont = 0; }
    if (m_pShadow) { m_pShadow->Release(); m_pShadow = 0; }
    if (m_pLittleShadow) { m_pLittleShadow->Release(); m_pLittleShadow = 0; }
    if (m_pDefaultSprite) { m_pDefaultSprite->Release(); m_pDefaultSprite = 0; }
    if (m_pLicensePlateFont) { m_pLicensePlateFont->Release(); m_pLicensePlateFont = 0; }

    int height = FontGetHeight();
    int smallHeight = height - 2;
    int weight = FontGetWeight();
    const char *face = FontGetFace();
    ID3DXFont *font;
    D3DXCreateFontA(m_pDevice, height, 0, weight, 1, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, face, &font);
    CFontWrapper *wrapper = new CFontWrapper;
    wrapper->font = font;
    m_pFont = wrapper;
    D3DXCreateFontA(m_pDevice, height, 0, weight, 1, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, face, &m_pShadow);
    D3DXCreateFontA(m_pDevice, smallHeight, 0, weight, 1, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, face, &font);
    wrapper = new CFontWrapper;
    wrapper->font = font;
    m_pLittleFont = wrapper;
    D3DXCreateFontA(m_pDevice, smallHeight, 0, weight, 1, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, face, &m_pLittleShadow);
    D3DXCreateSprite(m_pDevice, &m_pDefaultSprite);
    // The native Reset overwrites this pointer without freeing its old buffer.
    m_szTempBuffer = (char *)calloc(1, 100001);
    D3DXCreateFontA(m_pDevice, 38, 10, 700, 1, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Arial", &m_pLicensePlateFont);
    wrapper = new CFontWrapper;
    wrapper->font = m_pLicensePlateFont;
    m_pLicensePlateFont = wrapper;
    FontSize size;
    GetTextScreenSize(&size, "Y", 0);
    m_nFontHeight = size.cy;
    GetLittleTextScreenSize(&size, "Y", 0);
    m_nLittleFontHeight = size.cy;
}
