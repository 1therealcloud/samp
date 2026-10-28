#ifndef SAMP_DEATHWINDOW_H
#define SAMP_DEATHWINDOW_H
#include <d3dx9.h>
#include <stddef.h>
#pragma pack(push, 1)
struct DeathWindowEntry
{
    char killer[25], victim[25];
    DWORD killerColor, victimColor;
    BYTE weapon;
};
class CDeathWindow
{
public:
    // Raw initialization DWORD, without normalization.
    // 0x0006c720
    BOOL AreAuxiliaryFontsInitialized();
    // 0x00069580
    ~CDeathWindow();
    // 0x000695f0
    void CreateAuxiliaryFonts();
    union
    {
        BYTE nativeState[0x157];
        struct
        {
            BOOL enabled;
            DeathWindowEntry entries[5];
            int longestNickWidth, weaponWidth, weaponHeight;
            ID3DXFont *font, *weaponFont, *weaponBackgroundFont;
            ID3DXSprite* sprite;
            IDirect3DDevice9* device;
            BOOL auxiliaryFontsInitialized;
            ID3DXFont *auxiliaryFont, *auxiliaryBackgroundFont;
        };
    };
    // 0x00069660
    void PushBack();
    // 0x00069680
    void DrawText(const char* text, RECT rect, DWORD color, int format);
    // 0x00069780
    void DrawWeaponSprite(const char* glyph, RECT rect, DWORD color);
    // Native returns the caller-provided output pointer in EAX.
    // 0x00069810
    SIZE* GetWeaponSpriteRectSize(SIZE* size);
    // 0x00069890
    const char* GetWeaponSpriteId(BYTE weapon);
    // 0x0006a010
    void AddEntry(const char* killer, const char* victim, DWORD killerColor, DWORD victimColor, BYTE weapon);
    // 0x0006a0f0
    void AddMessage(const char* killer, const char* victim, DWORD killerColor, DWORD victimColor, BYTE weapon);
    // 0x0006a090
    CDeathWindow(IDirect3DDevice9* device);
    // 0x00069b90
    void ResetFonts();
    // 0x00069d20
    void Draw();
};
#pragma pack(pop)
typedef char DeathWindowEntryExtent[(sizeof(DeathWindowEntry) == 59) ? 1 : -1];
typedef char DeathWindowExtent[(sizeof(CDeathWindow) == 0x157) ? 1 : -1];
typedef char DeathWindowFontOffset[(offsetof(CDeathWindow, font) == 0x137) ? 1 : -1];
// 0x0011c330
extern const char g_deathAuxFontFace[9];
// 0x0011c34c
extern const char g_deathWeaponFontFace[11];
// 0x0011c358
extern const char g_deathLongestNick[25];
// 0x00118354
extern const char g_deathWeaponBackgroundGlyph[2];
// 0x0011c374
extern const float g_deathWindowHorizontalFactor;
// 0x0011c378
extern const float g_deathWindowVerticalFactor;
#endif
