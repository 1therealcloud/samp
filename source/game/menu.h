#ifndef MENU_H
#define MENU_H

#include <stddef.h>
#include <windows.h>

#include <static_assert.h>

#pragma pack(push, 1)
// Sixteen native DWORDs copied verbatim; only the first thirteen have known uses.
struct CMenuInteraction
{
    DWORD enabled;
    DWORD rows[12];
    DWORD unknown[3];
};
// Native layout; unknown00 and unknown3b2 remain untouched by construction.
class CMenu
{
public:
    BYTE unknown00;
    char title[32];
    char items[12][2][32];
    char columnTitles[2][32];
    float x, y, widths[2];
    BYTE columns;
    CMenuInteraction interaction;
    BYTE unknown3b2[2];
    DWORD handle;
    // 0x000a7670
    void Hide();
    // 0x000a76e0
    const char* MS(BYTE row, BYTE column);
    // 0x000a7710
    BYTE GetActiveRow();
    // 0x000a7740
    void Show();
    // 0x000a7420
    CMenu(const char* title, float x, float y, BYTE columns, float width1, float width2, const void* interaction);
    // 0x000a7580
    void AddItem(BYTE column, BYTE row, const char* text);
    // 0x000a7600
    void SetColumnTitle(BYTE column, const char* text);
    // 0x000a7690
    char* GetItem(BYTE column, BYTE row);
    // 0x000a76b0
    char* GetTitle();
    // R5 address to verify
    char* GetColumnTitle(BYTE column);
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CMenuInteraction) == 64);
STATIC_ASSERT(sizeof(CMenu) == 0x3b8);
STATIC_ASSERT(offsetof(CMenu, title) == 1);
STATIC_ASSERT(offsetof(CMenu, items) == 0x21);
STATIC_ASSERT(offsetof(CMenu, columnTitles) == 0x321);
STATIC_ASSERT(offsetof(CMenu, x) == 0x361);
STATIC_ASSERT(offsetof(CMenu, widths) == 0x369);
STATIC_ASSERT(offsetof(CMenu, columns) == 0x371);
STATIC_ASSERT(offsetof(CMenu, interaction) == 0x372);
STATIC_ASSERT(offsetof(CMenu, handle) == 0x3b4);
// 0x000fe0c8
extern const char g_menuItemKeys[24][32];
// 0x000ecbfc
extern const char g_menuDummy[6];

#endif // MENU_H
