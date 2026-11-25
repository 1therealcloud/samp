#ifndef MENU_H
#define MENU_H

#include <stddef.h>
#include <windows.h>

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

    void Hide();

    const char* MS(BYTE row, BYTE column);

    BYTE GetActiveRow();

    void Show();

    CMenu(const char* title, float x, float y, BYTE columns, float width1, float width2, const void* interaction);

    void AddItem(BYTE column, BYTE row, const char* text);

    void SetColumnTitle(BYTE column, const char* text);

    char* GetItem(BYTE column, BYTE row);

    char* GetTitle();

    char* GetColumnTitle(BYTE column);
};
#pragma pack(pop)
static_assert(sizeof(CMenuInteraction) == 64);
static_assert(sizeof(CMenu) == 0x3b8);
static_assert(offsetof(CMenu, title) == 1);
static_assert(offsetof(CMenu, items) == 0x21);
static_assert(offsetof(CMenu, columnTitles) == 0x321);
static_assert(offsetof(CMenu, x) == 0x361);
static_assert(offsetof(CMenu, widths) == 0x369);
static_assert(offsetof(CMenu, columns) == 0x371);
static_assert(offsetof(CMenu, interaction) == 0x372);
static_assert(offsetof(CMenu, handle) == 0x3b4);

extern const char g_menuItemKeys[24][32];

extern const char g_menuDummy[6];

#endif // MENU_H
