#ifndef DIALOG_H
#define DIALOG_H

#include <stddef.h>
#include <windows.h>

struct IDirect3DDevice9;
class CDXUTDialog;
class CDXUTListBoxDL;
class CDXUTIMEEditBox;
#pragma pack(push, 1)
class CDialog
{
public:
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

    CDialog(IDirect3DDevice9* device);

    void ResetDialogControls(CDXUTDialog* dialog);

    void Close(char response);

    bool MsgProc(UINT message, WPARAM wparam, LPARAM lparam);

    bool IsInputHidden();

    void Draw();

    void Recalc();

    RECT* GetRect(RECT* rect);

    int GetTextWidth(const char* text);

    int GetFontHeight();

    const char* CopyColumn(const char* text, char* output, int limit);

    const char* CopyLine(const char* text, char* output, int limit);

    void Hide();

    BOOL PopulateList(const char* text, SIZE* size);

    int PopulateTable(const char* text, SIZE* size, BOOL header);
};
#pragma pack(pop)
static_assert(sizeof(CDialog) == 0x29d);
static_assert(offsetof(CDialog, active) == 0x28);
static_assert(offsetof(CDialog, title) == 0x40);
static_assert(offsetof(CDialog, serverSide) == 0x81);
static_assert(offsetof(CDialog, columnTitles) == 0x89);
static_assert(offsetof(CDialog, columnWidths) == 0x28d);

extern const float g_dialogNegativeLineScale;

extern const float g_dialogLineScale;

extern const float g_dialogEditPadding;

extern int g_dialogResponseRpcId;

#endif // DIALOG_H
