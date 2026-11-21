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
    // 0x0006ffb0
    void Show(int id, int type, const char* title, const char* text, const char* button1, const char* button2, BOOL serverSide);
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
    // 0x0006f480
    CDialog(IDirect3DDevice9* device);
    // 0x0006fa20
    void ResetDialogControls(CDXUTDialog* dialog);
    // 0x00070630
    void Close(char response);
    // R5 address to verify
    bool MsgProc(UINT message, WPARAM wparam, LPARAM lparam);
    // R5 address to verify
    bool IsInputHidden();
    // 0x0006f890
    void Draw();
    // 0x0006f500
    void Recalc();
    // 0x0006f6b0
    RECT* GetRect(RECT* rect);
    // 0x0006f6e0
    int GetTextWidth(const char* text);
    // 0x0006f7a0
    int GetFontHeight();
    // 0x0006f7c0
    const char* CopyColumn(const char* text, char* output, int limit);
    // 0x0006f820
    const char* CopyLine(const char* text, char* output, int limit);
    // 0x0006f860
    void Hide();
    // R5 address to verify
    BOOL PopulateList(const char* text, SIZE* size);
    // R5 address to verify
    int PopulateTable(const char* text, SIZE* size, BOOL header);
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CDialog) == 0x29d);
STATIC_ASSERT(offsetof(CDialog, active) == 0x28);
STATIC_ASSERT(offsetof(CDialog, title) == 0x40);
STATIC_ASSERT(offsetof(CDialog, serverSide) == 0x81);
STATIC_ASSERT(offsetof(CDialog, columnTitles) == 0x89);
STATIC_ASSERT(offsetof(CDialog, columnWidths) == 0x28d);
// 0x000ea2dc
extern const float g_dialogNegativeLineScale;
// 0x000e6400
extern const float g_dialogLineScale;

// 0x000ea6fc
extern const float g_dialogEditPadding;
// 0x000ea320
extern int g_dialogResponseRpcId;

#endif // DIALOG_H
