#ifndef SAMP_MENU_H
#define SAMP_MENU_H
#include <static_assert.h>
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
    // 0x000a7260
    void Hide();
    // 0x000a72d0
    const char* MS(BYTE row, BYTE column);
    // 0x000a7300
    BYTE GetActiveRow();
    // 0x000a7330
    void Show();
    // 0x000a7160
    CMenu(const char* title, float x, float y, BYTE columns, float width1, float width2, const void* interaction);
    // 0x000a7200
    void AddItem(BYTE column, BYTE row, const char* text);
    // 0x000a7230
    void SetColumnTitle(BYTE column, const char* text);
    // 0x000a7280
    char* GetItem(BYTE column, BYTE row);
    // 0x000a72a0
    char* GetTitle();
    // 0x000a72b0
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
// 0x0013c0c8
extern const char g_menuItemKeys[24][32];
// 0x0011ecf4
extern const char g_menuDummy[6];
#endif
