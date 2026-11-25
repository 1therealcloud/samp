#include "game/menu.h"

#include "game/scripting.h"

#include <string.h>

extern const char g_menuItemKeys[24][32] = {"SAMP000", "SAMP100", "SAMP001", "SAMP101", "SAMP002", "SAMP102", "SAMP003", "SAMP103", "SAMP004", "SAMP104", "SAMP005", "SAMP105",
                                            "SAMP006", "SAMP106", "SAMP007", "SAMP107", "SAMP008", "SAMP108", "SAMP009", "SAMP109", "SAMP010", "SAMP110", "SAMP011", "SAMP111"};

extern const char g_menuDummy[6] = "DUMMY";

const char g_menuHeaderKey1[] = "SAMPRW2";

const char g_menuHeaderKey0[] = "SAMPRW1";

const char g_menuTitleKey[] = "SAMPHED";

const Scripting::OpcodeInfo g_menuHideCommand = {0x8da, "i"};

const Scripting::OpcodeInfo g_menuCreateCommand = {0x8d4, "sfffiiiiv"};

const Scripting::OpcodeInfo g_menuItemsCommand = {0x8db, "iisssssssssssss"};

const Scripting::OpcodeInfo g_menuWidthCommand = {0x9db, "iii"};

const Scripting::OpcodeInfo g_menuRowCommand = {0x8d9, "iii"};

const Scripting::OpcodeInfo g_menuSelectionCommand = {0x8d7, "iv"};

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
    strncpy_s(this->title, title, sizeof(this->title) - 1);
    this->title[sizeof(this->title) - 1] = 0;
    memcpy(&this->x, &x, 4);
    memcpy(&this->y, &y, 4);
    memcpy(&widths[0], &width1, 4);
    memcpy(&widths[1], &width2, 4);
    this->columns = columns == 2 ? 2 : 1;
    this->interaction = *(const CMenuInteraction*)interaction;
    handle = 0;
}

void CMenu::AddItem(BYTE column, BYTE row, const char* text)
{
    if (column >= 2 || row >= 12)
        return;
    strncpy_s(items[row][column], text, sizeof(items[row][column]) - 1);
    items[row][column][sizeof(items[row][column]) - 1] = 0;
}

void CMenu::SetColumnTitle(BYTE column, const char* text)
{
    if (column >= 2)
        return;
    strncpy_s(columnTitles[column], text, sizeof(columnTitles[column]) - 1);
    columnTitles[column][sizeof(columnTitles[column]) - 1] = 0;
}

char* CMenu::GetItem(BYTE column, BYTE row)
{
    return items[row][column];
}

char* CMenu::GetTitle()
{
    return title;
}

char* CMenu::GetColumnTitle(BYTE column)
{
    return columnTitles[column];
}

void CMenu::Hide()
{
    Scripting::ProcessCommand(&g_menuHideCommand, handle);
}

const char* CMenu::MS(BYTE row, BYTE column)
{
    int index = column + row * 2;
    return ((char*)items)[index * 32] ? ((const char*)g_menuItemKeys) + index * 32 : g_menuDummy;
}

BYTE CMenu::GetActiveRow()
{
    if (!interaction.enabled)
        return 0xff;
    DWORD row;
    Scripting::ProcessCommand(&g_menuSelectionCommand, handle, &row);
    return (BYTE)row;
}

void CMenu::Show()
{
    Scripting::ProcessCommand(&g_menuCreateCommand, title[0] ? g_menuTitleKey : g_menuDummy, (double)x, (double)y, (widths[0] + widths[1]) / columns, (int)columns,
                              interaction.enabled, 1, 1, &handle);
    Scripting::ProcessCommand(&g_menuItemsCommand, handle, 0, columnTitles[0][0] ? g_menuHeaderKey0 : g_menuDummy, MS(0, 0), MS(1, 0), MS(2, 0), MS(3, 0), MS(4, 0), MS(5, 0),
                              MS(6, 0), MS(7, 0), MS(8, 0), MS(9, 0), MS(10, 0), MS(11, 0));
    // Native uses column-zero width for both columns and VC7's _ftol2 conversion.
    Scripting::ProcessCommand(&g_menuWidthCommand, handle, 0, (int)widths[0]);
    if (columns == 2)
    {
        Scripting::ProcessCommand(&g_menuItemsCommand, handle, 1, columnTitles[1][0] ? g_menuHeaderKey1 : g_menuDummy, MS(0, 1), MS(1, 1), MS(2, 1), MS(3, 1), MS(4, 1), MS(5, 1),
                                  MS(6, 1), MS(7, 1), MS(8, 1), MS(9, 1), MS(10, 1), MS(11, 1));
        Scripting::ProcessCommand(&g_menuWidthCommand, handle, 1, (int)widths[0]);
    }
    for (BYTE row = 0; row < 12; ++row)
        Scripting::ProcessCommand(&g_menuRowCommand, handle, row, interaction.rows[row]);
}
