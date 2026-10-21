#ifndef SAMP_DIALOG_H
#define SAMP_DIALOG_H
#include <windows.h>
#include <stddef.h>
struct IDirect3DDevice9;
class CDXUTDialog;
class CDXUTListBox;
class CDXUTIMEEditBox;
#pragma pack(push,1)
class CDialog {public:
    // Caller-observed ABI; full dialog implementation remains open.
    // samp.dll RVA: 0x0006fa50
    void Show(int id,int type,const char *title,const char *text,const char *button1,const char *button2,BOOL serverSide);
    // Observed allocation extent, not a complete recovered interface.
    union {
        BYTE nativeState[0x29d];
        struct {
            IDirect3DDevice9 *device;
            int x,y,width,height,buttonWidth,buttonHeight;
            CDXUTDialog *dialog;
            CDXUTListBox *list;
            CDXUTIMEEditBox *input;
            BOOL active;
            int type,id;
            char *text;
            int textWidth,textHeight;
            char title[65];
            BOOL serverSide;
            int columnCount;
            char columnTitles[4][129];
            int columnWidths[4];
        };
    };
    // samp.dll RVA: 0x0006eec0
    CDialog(IDirect3DDevice9 *device);
    // samp.dll RVA: 0x0006f460
    void ResetDialogControls(CDXUTDialog *dialog);
    // samp.dll RVA: 0x000700d0
    void Close(char response);
    // samp.dll RVA: 0x0006ef10
    bool MsgProc(UINT message,WPARAM wparam,LPARAM lparam);
    // samp.dll RVA: 0x0006ef30
    bool IsInputHidden();
    // samp.dll RVA: 0x0006f2d0
    void Draw();
    // samp.dll RVA: 0x0006ef40
    void Recalc();
    // samp.dll RVA: 0x0006f0f0
    RECT *GetRect(RECT *rect);
    // samp.dll RVA: 0x0006f120
    int GetTextWidth(const char *text);
    // samp.dll RVA: 0x0006f1e0
    int GetFontHeight();
    // samp.dll RVA: 0x0006f200
    const char *CopyColumn(const char *text,char *output,int limit);
    // samp.dll RVA: 0x0006f260
    const char *CopyLine(const char *text,char *output,int limit);
    // samp.dll RVA: 0x0006f2a0
    void Hide();
};
#pragma pack(pop)
typedef char DialogSize[(sizeof(CDialog)==0x29d)?1:-1];
typedef char DialogStateOffset[(offsetof(CDialog,active)==0x28)?1:-1];
typedef char DialogTitleOffset[(offsetof(CDialog,title)==0x40)?1:-1];
typedef char DialogServerOffset[(offsetof(CDialog,serverSide)==0x81)?1:-1];
typedef char DialogHeaderOffset[(offsetof(CDialog,columnTitles)==0x89)?1:-1];
typedef char DialogWidthsOffset[(offsetof(CDialog,columnWidths)==0x28d)?1:-1];
// samp.dll RVA: 0x0011c304
extern const float g_dialogNegativeLineScale;
// samp.dll RVA: 0x00118490
extern const float g_dialogLineScale;
#endif
