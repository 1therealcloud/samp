#ifndef FONTRENDER_H
#define FONTRENDER_H

#include <d3dx9.h>

#include <stddef.h>
#include <windows.h>

#include <static_assert.h>

// Native green-underflow branch clears red and retains green; not generic saturation.
// R5 address to verify
DWORD __stdcall FontDarkenColor(DWORD color, BYTE amount);
// Saturates each RGB channel at255, retaining alpha.
// R5 address to verify
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
    // 0x0006baf0
    CFonts(IDirect3DDevice9*);
    // 0x0006b100
    ~CFonts();
    // 0x0006b180
    void OnLostDevice();
    // 0x0006b1c0
    void OnResetDevice();
    // 0x0006b200
    FontSize GetTextScreenSize(const char*, int);
    // 0x0006b2b0
    FontSize GetLittleTextScreenSize(const char*, int);
    // 0x0006b360
    void DrawText(ID3DXSprite*, const char*, RECT, D3DCOLOR, BOOL);
    // 0x0006b4e0
    void DrawLittleText(ID3DXSprite*, const char*, RECT, int, D3DCOLOR, BOOL);
    // 0x0006b650
    void DrawLicensePlateText(const char*, RECT, D3DCOLOR);
    // 0x0006b8e0
    void Reset();
};
STATIC_ASSERT(sizeof(CFonts) == 40);
STATIC_ASSERT(offsetof(CFonts, m_szTempBuffer) == 28);

// 0x000e637c
extern const char g_fontMeasureProbe[2];
// 0x0026eb9c
extern CFonts* g_deviceFonts;

// 0x000c5290
int FontGetHeight();
// 0x000c52e0
int FontGetDeathWindowHeight();
// 0x000c5340
int FontGetWeight();
// 0x000c5370
const char* FontGetFace();
// R5 address to verify
int FontGetDialogHeight();

#endif // FONTRENDER_H
