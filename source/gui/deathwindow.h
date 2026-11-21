#ifndef DEATHWINDOW_H
#define DEATHWINDOW_H

#include <d3dx9.h>

#include <stddef.h>

#include <static_assert.h>

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
    // R5 address to verify
    BOOL AreAuxiliaryFontsInitialized();
    // 0x00069b40
    ~CDeathWindow();
    // 0x00069bb0
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
    // 0x00069c20
    void PushBack();
    // 0x00069c40
    void DrawText(const char* text, RECT rect, DWORD color, int format);
    // 0x00069d40
    void DrawWeaponSprite(const char* glyph, RECT rect, DWORD color);
    // Native returns the caller-provided output pointer in EAX.
    // 0x00069dd0
    SIZE* GetWeaponSpriteRectSize(SIZE* size);
    // 0x00069e50
    const char* GetWeaponSpriteId(BYTE weapon);
    // 0x0006a5d0
    void AddEntry(const char* killer, const char* victim, DWORD killerColor, DWORD victimColor, BYTE weapon);
    // 0x0006a6b0
    void AddMessage(const char* killer, const char* victim, DWORD killerColor, DWORD victimColor, BYTE weapon);
    // 0x0006a650
    CDeathWindow(IDirect3DDevice9* device);
    // 0x0006a150
    void ResetFonts();
    // 0x0006a2e0
    void Draw();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(DeathWindowEntry) == 59);
STATIC_ASSERT(sizeof(CDeathWindow) == 0x157);
STATIC_ASSERT(offsetof(CDeathWindow, font) == 0x137);
// 0x000ea308
extern const char g_deathAuxFontFace[9];
// 0x000ea324
extern const char g_deathWeaponFontFace[11];
// 0x000ea330
extern const char g_deathLongestNick[25];
// 0x000e62d0
extern const char g_deathWeaponBackgroundGlyph[2];
// 0x000ea34c
extern const float g_deathWindowHorizontalFactor;
// 0x000ea350
extern const float g_deathWindowVerticalFactor;

#endif // DEATHWINDOW_H
