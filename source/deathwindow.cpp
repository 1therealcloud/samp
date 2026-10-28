#include "deathwindow.h"
// 0x0006c720
BOOL CDeathWindow::AreAuxiliaryFontsInitialized()
{
    return auxiliaryFontsInitialized;
}
#include "fontrender.h"
#include "playertags.h"
#include <string.h>
extern "C" __int64 __cdecl _ftol2();
// 0x0011c330
const char g_deathAuxFontFace[9] = "SAMPAUX3";
// 0x0011c34c
const char g_deathWeaponFontFace[11] = "GTAWEAPON3";
// 0x0011c358
const char g_deathLongestNick[25] = "LONGESTNICKNICK_NICKNICK";
// 0x00118354
const char g_deathWeaponBackgroundGlyph[2] = "G";
// 0x0011c374
const float g_deathWindowHorizontalFactor = 0.75f;
// 0x0011c378
const float g_deathWindowVerticalFactor = 0.3f;

// 0x00069580
CDeathWindow::~CDeathWindow()
{
    if (font)
    {
        font->Release();
        font = 0;
    }
    if (weaponFont)
    {
        weaponFont->Release();
        weaponFont = 0;
    }
    if (weaponBackgroundFont)
    {
        weaponBackgroundFont->Release();
        weaponBackgroundFont = 0;
    }
    if (sprite)
    {
        sprite->Release();
        sprite = 0;
    }
    // Auxiliary fonts are deliberately untouched by the native destructor.
}

// 0x00069660
void CDeathWindow::PushBack()
{
    memmove(entries, entries + 1, 4 * sizeof(DeathWindowEntry));
}

// 0x00069680
void CDeathWindow::DrawText(const char* text, RECT rect, DWORD color, int format)
{
    format |= 0x100;
    rect.top--;
    font->DrawTextA(sprite, text, -1, &rect, format, 0xff000000);
    rect.top += 2;
    font->DrawTextA(sprite, text, -1, &rect, format, 0xff000000);
    rect.top--;
    rect.left--;
    font->DrawTextA(sprite, text, -1, &rect, format, 0xff000000);
    rect.left += 2;
    font->DrawTextA(sprite, text, -1, &rect, format, 0xff000000);
    rect.left--;
    font->DrawTextA(sprite, text, -1, &rect, format, color);
}

// 0x00069780
void CDeathWindow::DrawWeaponSprite(const char* glyph, RECT rect, DWORD color)
{
    rect.top -= 5;
    weaponBackgroundFont->DrawTextA(sprite, g_deathWeaponBackgroundGlyph, -1, &rect, 0x100, 0xff000000);
    weaponBackgroundFont->DrawTextA(sprite, g_deathWeaponBackgroundGlyph, -1, &rect, 0x500, 0xff000000);
    weaponFont->DrawTextA(sprite, glyph, -1, &rect, 0x105, color);
}

// 0x00069810
SIZE* CDeathWindow::GetWeaponSpriteRectSize(SIZE* size)
{
    RECT rect = {0, 0, 100, 100};
    if (weaponBackgroundFont)
        weaponBackgroundFont->DrawTextA(0, g_deathWeaponBackgroundGlyph, -1, &rect, 0x504, 0xff000000);
    size->cx = rect.right - rect.left;
    size->cy = rect.bottom - rect.top;
    return size;
}

// 0x00069b90
void CDeathWindow::ResetFonts()
{
    if (!device)
        return;
    RECT longest = {0, 0, 0, 0};
    if (font)
    {
        font->Release();
        font = 0;
    }
    if (weaponFont)
    {
        weaponFont->Release();
        weaponFont = 0;
    }
    if (weaponBackgroundFont)
    {
        weaponBackgroundFont->Release();
        weaponBackgroundFont = 0;
    }
    if (sprite)
    {
        sprite->Release();
        sprite = 0;
    }
    int height = FontGetDeathWindowHeight();
    D3DXCreateSprite(device, &sprite);
    const char* face = FontGetFace();
    int weight = FontGetWeight();
    D3DXCreateFontA(device, height, 0, weight, 1, FALSE, 1, 0, 0, 0, face, &font);
    if (font)
        font->DrawTextA(0, g_deathLongestNick, -1, &longest, 0x400, 0xff000000);
    longestNickWidth = longest.right - longest.left;
    D3DXCreateFontA(device, height + 8, 0, 400, 1, FALSE, 2, 0, 0, 0, g_deathWeaponFontFace, &weaponFont);
    D3DXCreateFontA(device, height + 12, 0, 400, 1, FALSE, 2, 0, 0, 0, g_deathWeaponFontFace, &weaponBackgroundFont);
    SIZE first, second;
    weaponWidth = GetWeaponSpriteRectSize(&first)->cx;
    weaponHeight = GetWeaponSpriteRectSize(&second)->cy;
}

// 0x0006a010
void CDeathWindow::AddEntry(const char* killer, const char* victim, DWORD killerColor, DWORD victimColor, BYTE weapon)
{
    PushBack();
    DeathWindowEntry* entry = entries + 4;
    entry->weapon = weapon;
    entry->killerColor = killerColor;
    entry->victimColor = victimColor;
    // Native copies are unbounded, and null names clear only their first byte.
    if (killer)
    {
        char* out = entry->killer;
        do
        {
            *out++ = *killer;
        }
        while (*killer++);
    }
    else
        entry->killer[0] = 0;
    if (victim)
    {
        char* out = entry->victim;
        do
        {
            *out++ = *victim;
        }
        while (*victim++);
    }
    else
        entry->victim[0] = 0;
}

// 0x0006a0f0
void CDeathWindow::AddMessage(const char* killer, const char* victim, DWORD killerColor, DWORD victimColor, BYTE weapon)
{
    AddEntry(killer, victim, killerColor, victimColor, weapon);
}

// 0x00069d20
void CDeathWindow::Draw()
{
    int y, x;
    // clang-format off
    // GTA SA virtual addresses: screen height 0x00c17048, width 0x00c17044.
    __asm {
        fild dword ptr ds:[0c17048h]
        fmul g_deathWindowVerticalFactor
        call _ftol2
        mov y,eax
        fild dword ptr ds:[0c17044h]
        fmul g_deathWindowHorizontalFactor
        call _ftol2
        mov x,eax
    }
    // clang-format on
    int totalWidth = weaponWidth + longestNickWidth * 2;
    if (x + totalWidth > *(int*)0xc17044)
        x = *(int*)0xc17044 - totalWidth;
    int base = x, right = x + 60, bottom = y + 30;
    if (!font || !weaponFont || !enabled)
        return;
    sprite->Begin(0x10);
    RECT measured; // Native leaves this metric scratch untouched before DrawTextA.
    for (int i = 0; i < 5; i++)
    {
        DeathWindowEntry* entry = entries + i;
        if (strlen(entry->killer) && strlen(entry->victim))
        {
            font->DrawTextA(0, entry->killer, -1, &measured, 0x400, 0xffffffff);
            RECT rect = {x + longestNickWidth - (measured.right - measured.left), y, right, bottom};
            DrawText(entry->killer, rect, entry->killerColor, 0);
            x += longestNickWidth + 3;
            right = x + 35;
            RECT icon = {x, y, right, bottom};
            const char* glyph = GetWeaponSpriteId(entry->weapon);
            DrawWeaponSprite(glyph, icon, 0xffffffff);
            right += weaponWidth;
            x += weaponWidth;
            RECT victim = {x, y, right, bottom};
            DrawText(entry->victim, victim, entry->victimColor, 0);
            x = base;
        }
        else if (!strlen(entry->killer) && strlen(entry->victim))
        {
            font->DrawTextA(0, entry->victim, -1, &measured, 0x400, 0xff000000);
            RECT rect = {x + longestNickWidth - (measured.right - measured.left), y, right, bottom};
            DrawText(entry->victim, rect, entry->victimColor, 0);
            int left = x + longestNickWidth + 3;
            right = left + 35;
            DWORD color = 0xffffffff;
            if (entry->weapon == 200)
                color = 0xff1111aa;
            else if (entry->weapon == 201)
                color = 0xffaa1111;
            RECT icon = {left, y, right, bottom};
            const char* glyph = GetWeaponSpriteId(entry->weapon);
            DrawWeaponSprite(glyph, icon, color);
        }
        y += weaponHeight + 5;
        bottom += weaponHeight + 5;
        right = x + 60;
    }
    sprite->End();
}
// 0x0006a090
CDeathWindow::CDeathWindow(IDirect3DDevice9* device)
{
    *(DWORD*)(nativeState + 0x137) = 0;
    *(DWORD*)(nativeState + 0x13b) = 0;
    *(DWORD*)(nativeState + 0x13f) = 0;
    *(DWORD*)(nativeState + 0x143) = 0;
    *(DWORD*)(nativeState + 0x14b) = 0;
    *(DWORD*)(nativeState + 0x14f) = 0;
    *(DWORD*)(nativeState + 0x153) = 0;
    *(DWORD*)nativeState = 1;
    *(IDirect3DDevice9**)(nativeState + 0x147) = device;
    ResetFonts();
    memset(nativeState + 4, 0, 0x127);
}

// 0x000695f0
void CDeathWindow::CreateAuxiliaryFonts()
{
    IDirect3DDevice9* device = *(IDirect3DDevice9**)(nativeState + 0x147);
    D3DXCreateFontA(device, 20, 0, 400, 1, FALSE, 2, 0, 0, 0, g_deathAuxFontFace, (ID3DXFont**)(nativeState + 0x14f));
    device = *(IDirect3DDevice9**)(nativeState + 0x147);
    D3DXCreateFontA(device, 22, 0, 400, 1, FALSE, 2, 0, 0, 0, g_deathAuxFontFace, (ID3DXFont**)(nativeState + 0x153));
    *(DWORD*)(nativeState + 0x14b) = 1;
}

// 0x00069890
const char* CDeathWindow::GetWeaponSpriteId(BYTE weapon)
{
    switch (weapon)
    {
        case 0:
            // Native glyph literal RVA: 0x00117f68
            return "%";
        case 1:
            // Native glyph literal RVA: 0x001184c0
            return "B";
        case 2:
            // Native glyph literal RVA: 0x0011c348
            return ">";
        case 3:
            // Native glyph literal RVA: 0x00117f58
            return "(";
        case 4:
            // Native glyph literal RVA: 0x00118388
            return g_playerTagStatusBack;
        case 5:
            // Native glyph literal RVA: 0x00117f44
            return "?";
        case 6:
            // Native glyph literal RVA: 0x00117f64
            return "&";
        case 7:
            // Native glyph literal RVA: 0x00117f8c
            return "\"";
        case 8:
            // Native glyph literal RVA: 0x00118390
            return "!";
        case 9:
            // Native glyph literal RVA: 0x0011c344
            return "1";
        case 10:
        case 11:
        case 12:
        case 13:
            // Native glyph literal RVA: 0x00118370
            return g_playerTagStatusFront;
        case 14:
            // Native glyph literal RVA: 0x00117f88
            return "$";
        case 15:
            // Native glyph literal RVA: 0x0011838c
            return "#";
        case 16:
            // Native glyph literal RVA: 0x0011830c
            return "@";
        case 17:
            // Native glyph literal RVA: 0x00118374
            return "D";
        case 22:
            // Native glyph literal RVA: 0x0011c340
            return "6";
        case 23:
            // Native glyph literal RVA: 0x0011c310
            return "2";
        case 24:
            // Native glyph literal RVA: 0x0011c33c
            return "3";
        case 25:
            // Native glyph literal RVA: 0x00117f7c
            return "=";
        case 26:
            // Native glyph literal RVA: 0x001182d8
            return "0";
        case 27:
            // Native glyph literal RVA: 0x00118378
            return "+";
        case 28:
            // Native glyph literal RVA: 0x0011834c
            return "I";
        case 29:
            // Native glyph literal RVA: 0x001183e0
            return "8";
        case 30:
            // Native glyph literal RVA: 0x00118350
            return "H";
        case 31:
            // Native glyph literal RVA: 0x00117950
            return "5";
        case 32:
            // Native glyph literal RVA: 0x00118304
            return "7";
        case 33:
            // Native glyph literal RVA: 0x001183bc
            return ".";
        case 34:
            // Native glyph literal RVA: 0x00118308
            return "A";
        case 35:
            // Native glyph literal RVA: 0x00117880
            return "4";
        case 36:
            // Native glyph literal RVA: 0x00118380
            return ")";
        case 37:
            // Native glyph literal RVA: 0x001183b4
            return "P";
        case 38:
            // Native glyph literal RVA: 0x00118358
            return "F";
        case 39:
            // Native glyph literal RVA: 0x00117fa8
            return "<";
        case 40:
            // Native glyph literal RVA: 0x00117f80
            return ";";
        case 41:
            // Native glyph literal RVA: 0x00117694
            return "/";
        case 42:
            // Native glyph literal RVA: 0x001183c4
            return ",";
        case 46:
            // Native glyph literal RVA: 0x00117f84
            return ":";
        case 49:
            // Native glyph literal RVA: 0x0011843c
            return "L";
        case 50:
            // Native glyph literal RVA: 0x001182fc
            return "R";
        case 51:
            // Native glyph literal RVA: 0x00118300
            return "Q";
        case 54:
            // Native glyph literal RVA: 0x00118440
            return "K";
        case 200:
        case 201:
            // Native glyph literal RVA: 0x00118434
            return "N";
        // Native default glyph literal RVA: 0x00118444
        default:
            return "J";
    }
}
