#include "menu.h"
#include "game/scripting.h"
#include <string.h>
// 0x000a7160
CMenu::CMenu(const char* title, float x, float y, BYTE columns, float width1, float width2, const void* interaction)
{
    for (int row = 0; row < 12; ++row)
    {
        items[row][0][0] = 0;
        items[row][1][0] = 0;
    }
    this->title[0] = 0;
    columnTitles[0][0] = 0;
    columnTitles[1][0] = 0;
    strcpy(this->title, title);
    memcpy(&this->x, &x, 4);
    memcpy(&this->y, &y, 4);
    memcpy(&widths[0], &width1, 4);
    memcpy(&widths[1], &width2, 4);
    this->columns = columns == 2 ? 2 : 1;
    this->interaction = *(const CMenuInteraction*)interaction;
    handle = 0;
}
// 0x000a7200
void CMenu::AddItem(BYTE column, BYTE row, const char* text)
{
    strcpy(items[row][column], text);
}
// 0x000a7230
void CMenu::SetColumnTitle(BYTE column, const char* text)
{
    strcpy(columnTitles[column], text);
}
// 0x000a7280
char* CMenu::GetItem(BYTE column, BYTE row)
{
    return items[row][column];
}
// 0x000a72a0
char* CMenu::GetTitle()
{
    return title;
}
// 0x000a72b0
char* CMenu::GetColumnTitle(BYTE column)
{
    return columnTitles[column];
}

// 0x0013c0c8
extern const char g_menuItemKeys[24][32] = {"SAMP000", "SAMP100", "SAMP001", "SAMP101", "SAMP002", "SAMP102", "SAMP003", "SAMP103", "SAMP004", "SAMP104", "SAMP005", "SAMP105",
                                            "SAMP006", "SAMP106", "SAMP007", "SAMP107", "SAMP008", "SAMP108", "SAMP009", "SAMP109", "SAMP010", "SAMP110", "SAMP011", "SAMP111"};
// 0x0011ecf4
extern const char g_menuDummy[6] = "DUMMY";
// 0x0011ecfc
const char g_menuHeaderKey1[] = "SAMPRW2";
// 0x0011ed04
const char g_menuHeaderKey0[] = "SAMPRW1";
// 0x0011ed0c
const char g_menuTitleKey[] = "SAMPHED";
// 0x0011ec7c
const Scripting::OpcodeInfo g_menuHideCommand = {0x8da, "i"};
// 0x0011ec90
const Scripting::OpcodeInfo g_menuCreateCommand = {0x8d4, "sfffiiiiv"};
// 0x0011eca4
const Scripting::OpcodeInfo g_menuItemsCommand = {0x8db, "iisssssssssssss"};
// 0x0011ecb8
const Scripting::OpcodeInfo g_menuWidthCommand = {0x9db, "iii"};
// 0x0011eccc
const Scripting::OpcodeInfo g_menuRowCommand = {0x8d9, "iii"};
// 0x0011ece0
const Scripting::OpcodeInfo g_menuSelectionCommand = {0x8d7, "iv"};
// 0x000a7260
void CMenu::Hide()
{
    Scripting::ProcessCommand(&g_menuHideCommand, handle);
}
// 0x000a72d0
const char* CMenu::MS(BYTE row, BYTE column)
{
    int index = column + row * 2;
    return ((char*)items)[index * 32] ? ((const char*)g_menuItemKeys) + index * 32 : g_menuDummy;
}
// 0x000a7300
BYTE CMenu::GetActiveRow()
{
    if (!interaction.enabled)
        return 0xff;
    DWORD row;
    Scripting::ProcessCommand(&g_menuSelectionCommand, handle, &row);
    return (BYTE)row;
}
// 0x000a7330
void CMenu::Show()
{
    Scripting::ProcessCommand(&g_menuCreateCommand, title[0] ? g_menuTitleKey : g_menuDummy, (double)x, (double)y,
                             (widths[0] + widths[1]) / columns, (int)columns, interaction.enabled, 1, 1, &handle);
    Scripting::ProcessCommand(&g_menuItemsCommand, handle, 0, columnTitles[0][0] ? g_menuHeaderKey0 : g_menuDummy,
                             MS(0, 0), MS(1, 0), MS(2, 0), MS(3, 0), MS(4, 0), MS(5, 0),
                             MS(6, 0), MS(7, 0), MS(8, 0), MS(9, 0), MS(10, 0), MS(11, 0));
    // Native uses column-zero width for both columns and VC7's _ftol2 conversion.
    Scripting::ProcessCommand(&g_menuWidthCommand, handle, 0, (int)widths[0]);
    if (columns == 2)
    {
        Scripting::ProcessCommand(&g_menuItemsCommand, handle, 1, columnTitles[1][0] ? g_menuHeaderKey1 : g_menuDummy,
                                 MS(0, 1), MS(1, 1), MS(2, 1), MS(3, 1), MS(4, 1), MS(5, 1),
                                 MS(6, 1), MS(7, 1), MS(8, 1), MS(9, 1), MS(10, 1), MS(11, 1));
        Scripting::ProcessCommand(&g_menuWidthCommand, handle, 1, (int)widths[0]);
    }
    for (BYTE row = 0; row < 12; ++row)
        Scripting::ProcessCommand(&g_menuRowCommand, handle, row, interaction.rows[row]);
}
