#ifndef FONTRENDER_H
#define FONTRENDER_H

#include <d3dx9.h>

#include <stddef.h>
#include <windows.h>

// Native green-underflow branch clears red and retains green; not generic saturation.

DWORD __stdcall FontDarkenColor(DWORD color, BYTE amount);
// Saturates each RGB channel at255, retaining alpha.

DWORD __stdcall FontLightenColor(DWORD color, BYTE amount);
struct FontSize
{
    long cx, cy;
};
class CFonts
{
public:
    ID3DXFont *m_pFont, *m_pLittleFont, *m_pShadow, *m_pLittleShadow;
    ID3DXFont* m_pLicensePlateFont;
    ID3DXSprite* m_pDefaultSprite;
    IDirect3DDevice9* m_pDevice;
    char* m_szTempBuffer;
    int m_nFontHeight, m_nLittleFontHeight;

    CFonts(IDirect3DDevice9*);

    ~CFonts();

    void OnLostDevice();

    void OnResetDevice();

    FontSize GetTextScreenSize(const char*, int);

    FontSize GetLittleTextScreenSize(const char*, int);

    void DrawText(ID3DXSprite*, const char*, RECT, D3DCOLOR, BOOL);

    void DrawLittleText(ID3DXSprite*, const char*, RECT, int, D3DCOLOR, BOOL);

    void DrawLicensePlateText(const char*, RECT, D3DCOLOR);

    void Reset();
};
static_assert(sizeof(CFonts) == 40);
static_assert(offsetof(CFonts, m_szTempBuffer) == 28);

extern const char g_fontMeasureProbe[2];

extern CFonts* g_deviceFonts;

int FontGetHeight();

int FontGetDeathWindowHeight();

int FontGetWeight();

const char* FontGetFace();

int FontGetDialogHeight();

#endif // FONTRENDER_H
