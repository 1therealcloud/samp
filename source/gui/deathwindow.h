#ifndef DEATHWINDOW_H
#define DEATHWINDOW_H

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

    BOOL AreAuxiliaryFontsInitialized();

    ~CDeathWindow();

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

    void PushBack();

    void DrawText(const char* text, RECT rect, DWORD color, int format);

    void DrawWeaponSprite(const char* glyph, RECT rect, DWORD color);
    // Native returns the caller-provided output pointer in EAX.

    SIZE* GetWeaponSpriteRectSize(SIZE* size);

    const char* GetWeaponSpriteId(BYTE weapon);

    void AddEntry(const char* killer, const char* victim, DWORD killerColor, DWORD victimColor, BYTE weapon);

    void AddMessage(const char* killer, const char* victim, DWORD killerColor, DWORD victimColor, BYTE weapon);

    CDeathWindow(IDirect3DDevice9* device);

    void ResetFonts();

    void Draw();
};
#pragma pack(pop)
static_assert(sizeof(DeathWindowEntry) == 59);
static_assert(sizeof(CDeathWindow) == 0x157);
static_assert(offsetof(CDeathWindow, font) == 0x137);

extern const char g_deathAuxFontFace[9];

extern const char g_deathWeaponFontFace[11];

extern const char g_deathLongestNick[25];

extern const char g_deathWeaponBackgroundGlyph[2];

extern const float g_deathWindowHorizontalFactor;

extern const float g_deathWindowVerticalFactor;

#endif // DEATHWINDOW_H
