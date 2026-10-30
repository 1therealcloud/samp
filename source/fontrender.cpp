// SA-MP font orchestration; COM/D3DX implementations remain external.
#include "fontrender.h"
// 0x0006b270
DWORD __stdcall FontLightenColor(DWORD color, BYTE amount)
{
    unsigned red = ((color >> 16) & 255) + amount;
    unsigned green = ((color >> 8) & 255) + amount;
    unsigned blue = (color & 255) + amount;
    if (red >= 255)
        red = 255;
    if (green >= 255)
        green = 255;
    if (blue >= 255)
        blue = 255;
    return (color & 0xff000000) | (red << 16) | (green << 8) | blue;
}
// 0x0006b210
DWORD __stdcall FontDarkenColor(DWORD color, BYTE amount)
{
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
    return (color & 0xff000000) | ((DWORD)red << 16) | ((DWORD)green << 8) | blue;
}
#include "cmdwindow.h"
#include "runutil.h"
#include <DXUTgui.h>
#include <string.h>

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
FontSize* CFonts::GetTextScreenSize(FontSize* output, const char* text, int format)
{
    FontSize size;
    if (strlen(text) > 100000)
    {
        *output = size;
        return output;
    }
    strcpy(m_szTempBuffer, text);
    ChatStripColorTags(m_szTempBuffer);
    RECT rect;
    m_pFont->DrawTextA(0, m_szTempBuffer, -1, &rect, format | DT_CALCRECT, 0xff000000);
    size.cx = rect.right - rect.left;
    size.cy = rect.bottom - rect.top;
    *output = size;
    return output;
}
// 0x0006acf0
FontSize* CFonts::GetLittleTextScreenSize(FontSize* output, const char* text, int format)
{
    FontSize size;
    if (strlen(text) > 100000)
    {
        *output = size;
        return output;
    }
    strcpy(m_szTempBuffer, text);
    ChatStripColorTags(m_szTempBuffer);
    RECT rect;
    m_pLittleFont->DrawTextA(0, m_szTempBuffer, -1, &rect, format | DT_CALCRECT, 0xff000000);
    size.cx = rect.right - rect.left;
    size.cy = rect.bottom - rect.top;
    *output = size;
    return output;
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

#include "d3dhook/ID3DXFontHook.h"
#include <stdlib.h>

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
    FontSize size;
    GetTextScreenSize(&size, g_fontMeasureProbe, 0);
    m_nFontHeight = size.cy;
    GetLittleTextScreenSize(&size, g_fontMeasureProbe, 0);
    m_nLittleFontHeight = size.cy;
}

#include "config.h"
#include "game/gta_globals.h"

// 0x000c6950
int FontGetHeight()
{
    int height;
    if (g_chatScreenWidth < 1024)
        height = 14;
    else if (g_chatScreenWidth < 1400)
        height = 16;
    else if (g_chatScreenWidth < 1600)
        height = 18;
    else
        height = 20;
    return height + g_pConfig->GetIntValue("fontsize") * 2;
}

// 0x000c69a0
int FontGetDeathWindowHeight()
{
    int base = g_chatScreenWidth < 1024 ? 12 : 14;
    int adjusted = (int)((DWORD)base + (DWORD)g_pConfig->GetIntValue("fontsize") * 2);
    return adjusted < base ? base : adjusted;
}

// 0x000c6a00
int FontGetWeight()
{
    int weight = g_pConfig->GetIntValue("fontweight");
    if (weight == 0)
        return 700;
    return weight == 1 ? 400 : 700;
}

// 0x000c6a30
const char* FontGetFace()
{
    if (g_pConfig && g_pConfig->GetStringValue("fontface"))
        return g_pConfig->GetStringValue("fontface");
    return "Arial";
}

// 0x00118400
const char g_fontMeasureProbe[2] = "Y";

// 0x002aca2c
CFonts* g_deviceFonts;
