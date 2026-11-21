// SA-MP font orchestration; COM/D3DX implementations remain external.
#include "render/fontrender.h"

#include "d3dhook/ID3DXFontHook.h"
#include "gui/chatwindow.h"
#include "gui/cmdwindow.h"

#include <DXUTgui.h>

#include <stdlib.h>
#include <string.h>
// 0x00118400
const char g_fontMeasureProbe[2] = "Y";
// 0x002aca2c
CFonts* g_deviceFonts;

// 0x0006b270
DWORD __stdcall FontLightenColor(DWORD color, BYTE amount)
{
    BYTE alpha = (BYTE)(color >> 24);
    BYTE red = (BYTE)(color >> 16), green = (BYTE)(color >> 8), blue = (BYTE)color;
    if (red + amount < 255)
        red += amount;
    else
        red = 255;
    if (green + amount < 255)
        green += amount;
    else
        green = 255;
    if (blue + amount < 255)
        blue += amount;
    else
        blue = 255;
    DWORD result = ((DWORD)alpha << 8) | red;
    result = (result << 8) | green;
    return (result << 8) | blue;
}

// 0x0006b210
DWORD __stdcall FontDarkenColor(DWORD color, BYTE amount)
{
    BYTE alpha = (BYTE)(color >> 24);
    BYTE red = (BYTE)(color >> 16), green = (BYTE)(color >> 8), blue = (BYTE)color;
    if (red > amount)
        red -= amount;
    else
        red = 0;
    if (green > amount)
        green -= amount;
    else
        red = 0;
    if (blue > amount)
        blue -= amount;
    else
        blue = 0;
    DWORD result = ((DWORD)alpha << 8) | red;
    result = (result << 8) | green;
    return (result << 8) | blue;
}

// 0x0006b530
CFonts::CFonts(IDirect3DDevice9* device)
{
    m_pDevice = device;
    m_pFont = 0;
    m_pLittleFont = 0;
    m_pShadow = 0;
    m_pLittleShadow = 0;
    m_pDefaultSprite = 0;
    m_pLicensePlateFont = 0;
    Reset();
}

#define RELEASE_FONT(p)                                                                                                                                                            \
    if (p)                                                                                                                                                                         \
    {                                                                                                                                                                              \
        p->Release();                                                                                                                                                              \
        p = 0;                                                                                                                                                                     \
    }
// 0x0006ab40
CFonts::~CFonts()
{
    RELEASE_FONT(m_pFont)
    RELEASE_FONT(m_pLittleFont)
    RELEASE_FONT(m_pShadow)
    RELEASE_FONT(m_pLittleShadow)
    RELEASE_FONT(m_pDefaultSprite)
    RELEASE_FONT(m_pLicensePlateFont)
}

// 0x0006abc0
void CFonts::OnLostDevice()
{
    m_pFont->OnLostDevice();
    m_pLittleFont->OnLostDevice();
    m_pShadow->OnLostDevice();
    m_pLittleShadow->OnLostDevice();
    m_pDefaultSprite->OnLostDevice();
    m_pLicensePlateFont->OnLostDevice();
}

// 0x0006ac00
void CFonts::OnResetDevice()
{
    m_pFont->OnResetDevice();
    m_pLittleFont->OnResetDevice();
    m_pShadow->OnResetDevice();
    m_pLittleShadow->OnResetDevice();
    m_pDefaultSprite->OnResetDevice();
    m_pLicensePlateFont->OnResetDevice();
}

// 0x0006ac40
FontSize CFonts::GetTextScreenSize(const char* text, int format)
{
    FontSize size = {0, 0};
    if (strlen(text) > 100000)
        return size;
    strcpy(m_szTempBuffer, text);
    ChatStripColorTags(m_szTempBuffer);
    RECT rect;
    m_pFont->DrawTextA(0, m_szTempBuffer, -1, &rect, format | DT_CALCRECT, 0xff000000);
    size.cx = rect.right - rect.left;
    size.cy = rect.bottom - rect.top;
    return size;
}

// 0x0006acf0
FontSize CFonts::GetLittleTextScreenSize(const char* text, int format)
{
    FontSize size = {0, 0};
    if (strlen(text) > 100000)
        return size;
    strcpy(m_szTempBuffer, text);
    ChatStripColorTags(m_szTempBuffer);
    RECT rect;
    m_pLittleFont->DrawTextA(0, m_szTempBuffer, -1, &rect, format | DT_CALCRECT, 0xff000000);
    size.cx = rect.right - rect.left;
    size.cy = rect.bottom - rect.top;
    return size;
}

// 0x0006ada0
void CFonts::DrawText(ID3DXSprite* sprite, const char* text, RECT rect, D3DCOLOR color, BOOL shadow)
{
    if (!text || !*text)
        return;
    BOOL ownSprite = FALSE;
    if (!sprite)
    {
        sprite = m_pDefaultSprite;
        ownSprite = TRUE;
        sprite->Begin(D3DXSPRITE_ALPHABLEND);
    }
    if (shadow)
    {
        if (strlen(text) > 100000)
            return;
        strcpy(m_szTempBuffer, text);
        ChatStripColorTags(m_szTempBuffer);
        int length = (int)strlen(m_szTempBuffer);
        --rect.top;
        m_pShadow->DrawTextA(sprite, m_szTempBuffer, length, &rect, DT_NOCLIP, 0xff000000);
        rect.top += 2;
        m_pShadow->DrawTextA(sprite, m_szTempBuffer, length, &rect, DT_NOCLIP, 0xff000000);
        --rect.top;
        --rect.left;
        m_pShadow->DrawTextA(sprite, m_szTempBuffer, length, &rect, DT_NOCLIP, 0xff000000);
        rect.left += 2;
        m_pShadow->DrawTextA(sprite, m_szTempBuffer, length, &rect, DT_NOCLIP, 0xff000000);
        --rect.left;
    }
    m_pFont->DrawTextA(sprite, text, -1, &rect, DT_NOCLIP, color);
    if (ownSprite)
        sprite->End();
}

// 0x0006af20
void CFonts::DrawLittleText(ID3DXSprite* sprite, const char* text, RECT rect, int format, D3DCOLOR color, BOOL shadow)
{
    if (!text || !*text)
        return;
    BOOL ownSprite = FALSE;
    if (!sprite)
    {
        sprite = m_pDefaultSprite;
        ownSprite = TRUE;
        sprite->Begin(D3DXSPRITE_ALPHABLEND);
    }
    if (shadow)
    {
        if (strlen(text) > 100000)
            return;
        strcpy(m_szTempBuffer, text);
        ChatStripColorTags(m_szTempBuffer);
        int length = (int)strlen(m_szTempBuffer);
        --rect.top;
        m_pLittleShadow->DrawTextA(sprite, m_szTempBuffer, length, &rect, format, 0xff000000);
        rect.top += 2;
        m_pLittleShadow->DrawTextA(sprite, m_szTempBuffer, length, &rect, format, 0xff000000);
        --rect.top;
        --rect.left;
        m_pLittleShadow->DrawTextA(sprite, m_szTempBuffer, length, &rect, format, 0xff000000);
        rect.left += 2;
        m_pLittleShadow->DrawTextA(sprite, m_szTempBuffer, length, &rect, format, 0xff000000);
        --rect.left;
    }
    m_pLittleFont->DrawTextA(sprite, text, -1, &rect, format, color);
    if (ownSprite)
        sprite->End();
}

// 0x0006b090
void CFonts::DrawLicensePlateText(const char* text, RECT rect, D3DCOLOR color)
{
    if (!text || !*text)
        return;
    ID3DXSprite* sprite = m_pDefaultSprite;
    sprite->Begin(D3DXSPRITE_ALPHABLEND);
    if (strlen(text) > 100000)
        return;
    strcpy(m_szTempBuffer, text);
    ChatStripColorTags(m_szTempBuffer);
    int length = (int)strlen(m_szTempBuffer);
    rect.top -= 2;
    m_pLicensePlateFont->DrawTextA(sprite, m_szTempBuffer, length, &rect, DT_NOCLIP | DT_CENTER | DT_VCENTER, 0xb4f0eee4);
    rect.top += 2;
    rect.left += 2;
    m_pLicensePlateFont->DrawTextA(sprite, m_szTempBuffer, length, &rect, DT_NOCLIP | DT_CENTER | DT_VCENTER, 0xb4f0eee4);
    rect.left -= 2;
    rect.top += 2;
    m_pLicensePlateFont->DrawTextA(sprite, m_szTempBuffer, length, &rect, DT_NOCLIP | DT_CENTER | DT_VCENTER, 0xb4f0eee4);
    rect.top -= 2;
    rect.left -= 2;
    m_pLicensePlateFont->DrawTextA(sprite, m_szTempBuffer, length, &rect, DT_NOCLIP | DT_CENTER | DT_VCENTER, 0xb4f0eee4);
    rect.left += 2;
    m_pLicensePlateFont->DrawTextA(sprite, text, -1, &rect, DT_NOCLIP | DT_CENTER | DT_VCENTER, color);
    sprite->End();
}

// 0x0006b320
void CFonts::Reset()
{
    if (!m_pDevice)
        return;
    if (m_pFont)
    {
        m_pFont->Release();
        m_pFont = 0;
    }
    if (m_pLittleFont)
    {
        m_pLittleFont->Release();
        m_pLittleFont = 0;
    }
    if (m_pShadow)
    {
        m_pShadow->Release();
        m_pShadow = 0;
    }
    if (m_pLittleShadow)
    {
        m_pLittleShadow->Release();
        m_pLittleShadow = 0;
    }
    if (m_pDefaultSprite)
    {
        m_pDefaultSprite->Release();
        m_pDefaultSprite = 0;
    }
    if (m_pLicensePlateFont)
    {
        m_pLicensePlateFont->Release();
        m_pLicensePlateFont = 0;
    }

    int height = FontGetHeight();
    int smallHeight = height - 2;
    int weight = FontGetWeight();
    const char* face = FontGetFace();
    ID3DXFont* font;
    D3DXCreateFontA(m_pDevice, height, 0, weight, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, face, &font);
    CFontWrapper* wrapper = new CFontWrapper;
    wrapper->font = font;
    m_pFont = wrapper;
    D3DXCreateFontA(m_pDevice, height, 0, weight, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, face, &m_pShadow);
    D3DXCreateFontA(m_pDevice, smallHeight, 0, weight, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, face, &font);
    wrapper = new CFontWrapper;
    wrapper->font = font;
    m_pLittleFont = wrapper;
    D3DXCreateFontA(m_pDevice, smallHeight, 0, weight, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, face, &m_pLittleShadow);
    D3DXCreateSprite(m_pDevice, &m_pDefaultSprite);
    // The native Reset overwrites this pointer without freeing its old buffer.
    m_szTempBuffer = (char*)calloc(1, 100001);
    D3DXCreateFontA(m_pDevice, 38, 10, 700, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Arial", &m_pLicensePlateFont);
    wrapper = new CFontWrapper;
    wrapper->font = m_pLicensePlateFont;
    m_pLicensePlateFont = wrapper;
    m_nFontHeight = GetTextScreenSize(g_fontMeasureProbe, 0).cy;
    m_nLittleFontHeight = GetLittleTextScreenSize(g_fontMeasureProbe, 0).cy;
}
