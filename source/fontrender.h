#ifndef SAMP_RECOVERED_FONTS_H
#define SAMP_RECOVERED_FONTS_H
#include <d3dx9.h>
#include <stddef.h>
#include <windows.h>
// Native green-underflow branch clears red and retains green; not generic saturation.
// 0x0006b210
DWORD __stdcall FontDarkenColor(DWORD color, BYTE amount);
// Saturates each RGB channel at255, retaining alpha.
// 0x0006b270
DWORD __stdcall FontLightenColor(DWORD color, BYTE amount);
// samp.dll RVA: unknown (inline layout helper; no standalone entry established)
struct FontSize
{
    long cx, cy;
    FontSize()
    {
        cx = 0;
        cy = 0;
    }
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
    // 0x0006b530
    CFonts(IDirect3DDevice9*);
    // 0x0006ab40
    ~CFonts();
    // 0x0006abc0
    void OnLostDevice();
    // 0x0006ac00
    void OnResetDevice();
    // 0x0006ac40
    FontSize* GetTextScreenSize(FontSize*, const char*, int);
    // 0x0006acf0
    FontSize* GetLittleTextScreenSize(FontSize*, const char*, int);
    // 0x0006ada0
    void DrawText(ID3DXSprite*, const char*, RECT, D3DCOLOR, BOOL);
    // 0x0006af20
    void DrawLittleText(ID3DXSprite*, const char*, RECT, int, D3DCOLOR, BOOL);
    // 0x0006b090
    void DrawLicensePlateText(const char*, RECT, D3DCOLOR);
    // 0x0006b320
    void Reset();
};
typedef char FontsSize[(sizeof(CFonts) == 40) ? 1 : -1];
typedef char FontsScratchOffset[(offsetof(CFonts, m_szTempBuffer) == 28) ? 1 : -1];

// 0x00118400
extern const char g_fontMeasureProbe[2];
// 0x002aca2c
extern CFonts* g_deviceFonts;

// 0x000c6950
int FontGetHeight();
// 0x000c69a0
int FontGetDeathWindowHeight();
// 0x000c6a00
int FontGetWeight();
// 0x000c6a30
const char* FontGetFace();
// 0x000c69e0
int FontGetDialogHeight();
#endif
