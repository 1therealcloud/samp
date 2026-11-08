#ifndef DIALOG_H
#define DIALOG_H

#include <stddef.h>
#include <windows.h>

#include <static_assert.h>
struct IDirect3DDevice9;
class CDXUTDialog;
class CDXUTListBoxDL;
class CDXUTIMEEditBox;
#pragma pack(push, 1)
class CDialog
{
public:
    // 0x0006fa50
    void Show(int id, int type, const char* title, const char* text, const char* button1, const char* button2, BOOL serverSide);
    // Observed allocation extent, not a complete recovered interface.
    union
    {
        BYTE nativeState[0x29d];
        struct
        {
            IDirect3DDevice9* device;
            int x, y, width, height, buttonWidth, buttonHeight;
            CDXUTDialog* dialog;
            CDXUTListBoxDL* list;
            CDXUTIMEEditBox* input;
            BOOL active;
            int type, id;
            char* text;
            int textWidth, textHeight;
            char title[65];
            BOOL serverSide;
            int columnCount;
            char columnTitles[4][129];
            int columnWidths[4];
        };
    };
    // 0x0006eec0
    CDialog(IDirect3DDevice9* device);
    // 0x0006f460
    void ResetDialogControls(CDXUTDialog* dialog);
    // 0x000700d0
    void Close(char response);
    // 0x0006ef10
    bool MsgProc(UINT message, WPARAM wparam, LPARAM lparam);
    // 0x0006ef30
    bool IsInputHidden();
    // 0x0006f2d0
    void Draw();
    // 0x0006ef40
    void Recalc();
    // 0x0006f0f0
    RECT* GetRect(RECT* rect);
    // 0x0006f120
    int GetTextWidth(const char* text);
    // 0x0006f1e0
    int GetFontHeight();
    // 0x0006f200
    const char* CopyColumn(const char* text, char* output, int limit);
    // 0x0006f260
    const char* CopyLine(const char* text, char* output, int limit);
    // 0x0006f2a0
    void Hide();
    // 0x0006f630
    BOOL PopulateList(const char* text, SIZE* size);
    // 0x0006f730
    int PopulateTable(const char* text, SIZE* size, BOOL header);
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CDialog) == 0x29d);
STATIC_ASSERT(offsetof(CDialog, active) == 0x28);
STATIC_ASSERT(offsetof(CDialog, title) == 0x40);
STATIC_ASSERT(offsetof(CDialog, serverSide) == 0x81);
STATIC_ASSERT(offsetof(CDialog, columnTitles) == 0x89);
STATIC_ASSERT(offsetof(CDialog, columnWidths) == 0x28d);
// 0x0011c304
extern const float g_dialogNegativeLineScale;
// 0x00118490
extern const float g_dialogLineScale;

// 0x0011c724
extern const float g_dialogEditPadding;
// 0x0011c348
extern int g_dialogResponseRpcId;

#endif // DIALOG_H
