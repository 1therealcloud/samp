#include "menu.h"
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
    char* destination = this->title;
    char value;
    do
    {
        value = *title++;
        *destination++ = value;
    }
    while (value);
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
    char* destination = items[row][column];
    char value;
    do
    {
        value = *text++;
        *destination++ = value;
    }
    while (value);
}
// 0x000a7230
void CMenu::SetColumnTitle(BYTE column, const char* text)
{
    char* destination = columnTitles[column];
    char value;
    do
    {
        value = *text++;
        *destination++ = value;
    }
    while (value);
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

#include "game/scripting.h"
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
    const char* title = this->title[0] ? g_menuTitleKey : g_menuDummy;
    int columns = this->columns;
    double width;
    float* widthValues = widths;
    // clang-format off
    __asm {mov eax,widthValues}
    __asm {fld dword ptr [eax+4]}
    __asm {fadd dword ptr [eax]}
    __asm {fidiv columns}
    __asm {fstp qword ptr width}
    // clang-format on
    Scripting::ProcessCommand(&g_menuCreateCommand, title, (double)x, (double)y, width, columns, interaction.enabled, 1, 1, &handle);
    for (int column = 0; column < 2; ++column)
    {
        if (column && this->columns != 2)
            break;
        const char* heading = columnTitles[column][0] ? (column ? g_menuHeaderKey1 : g_menuHeaderKey0) : g_menuDummy;
        const char* items[12];
        for (int row = 11; row >= 0; --row)
            items[row] = MS((BYTE)row, (BYTE)column);
        Scripting::ProcessCommand(&g_menuItemsCommand, handle, column, heading, items[0], items[1], items[2], items[3], items[4], items[5], items[6], items[7], items[8], items[9],
                                  items[10], items[11]);
        // VC7 uses its native _ftol2 conversion here, matching the original.
        Scripting::ProcessCommand(&g_menuWidthCommand, handle, column, (int)widths[0]);
    }
    for (int row = 0; row < 12; ++row)
        Scripting::ProcessCommand(&g_menuRowCommand, handle, row, interaction.rows[row]);
}
