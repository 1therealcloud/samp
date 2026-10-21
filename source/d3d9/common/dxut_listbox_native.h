#ifndef SAMP_DXUT_LISTBOX_DL_H
#define SAMP_DXUT_LISTBOX_DL_H
#include <windows.h>
#include <stddef.h>
#include <d3dx9.h>
#include <DXUT.h>
#include <DXUTmisc.h>
class CDXUTDialog;
#pragma pack(push,1)
class CDXUTDialogDL;
// Native ANSI button allocation view; lifecycle/vtable are still dependencies.
class CDXUTButtonDL {
    BYTE storage[0x152];
    friend class CDXUTDialogDL;
    // samp.dll RVA: 0x0008df20
    CDXUTButtonDL(CDXUTDialog *parent);
};
// ANSI factory view of the same parent, not a separately constructed dialog.
class CDXUTDialogDL {public:
    // samp.dll RVA: 0x00090790
    HRESULT AddButton(int id,const char *text,int x,int y,int width,int height,
        UINT hotkey=0,bool isDefault=false,CDXUTButtonDL **created=0);
};
struct CDXUTEditBoxDL {
    BYTE control[0x4d];
    const wchar_t *wideText;
    // samp.dll RVA: 0x000850d0
    const char *GetText();
};
// ANSI static-text prefix only, not a constructed or complete virtual control.
struct CDXUTStaticDL {
    BYTE control[0x4d];
    char text[260];
    // samp.dll RVA: 0x000875c0
    HRESULT SetText(const char *text);
};
// Observed native storage views. Construction, virtual interfaces and rendering
// remain separate recovery work; these are not the SDK Unicode widget types.
struct DXUTListBoxItemDL {
    char text[257];
    char subitems[3][129];
    void *data;
    RECT activeRect;
    bool selected;
    int row;
    bool flag;
};
class CDXUTScrollBarDL {
public:
    BYTE control[0x4d];
    bool showThumb;
    RECT up,down,track,thumb;
    int position,pageSize,start,end;
    BYTE unknown9e[0x34];
    // samp.dll RVA: 0x00084e80
    void Cap();
    // samp.dll RVA: 0x00084da0
    void UpdateThumbRect();
    // samp.dll RVA: 0x000886d0
    void SetTrackRange(int first,int last);
    // samp.dll RVA: 0x00084e30
    void ShowItem(int index);
private:
    // Declaration-only native lifecycle; the observed view must not be constructed.
    // samp.dll RVA: 0x0008e750
    CDXUTScrollBarDL(CDXUTDialog *parent);
    // samp.dll RVA: 0x0008e800
    ~CDXUTScrollBarDL();
};
class CDXUTListBoxDL {
    friend class CDialog;
public:
    BYTE control[0x4d];
    int columnCount;
    int columnWidths[3];
    CDXUTScrollBarDL scrollBar;
    int scrollBarWidth,border,margin,textHeight;
    DWORD style;
    int selected,selectionStart;
    bool drag;
    CGrowableArray<DXUTListBoxItemDL*> items;
    // samp.dll RVA: 0x0008a270
    HRESULT AddItem(const char *text,void *data,int row);
    // samp.dll RVA: 0x00088880
    void SetSubItemText(int row,int column,const char *text);
    // samp.dll RVA: 0x0008a440
    DXUTListBoxItemDL *GetItem(int row);
    // samp.dll RVA: 0x0008c270
    void RemoveAllItems();
    // samp.dll RVA: 0x0008a470
    void SelectItem(int index);
    // samp.dll RVA: 0x000888f0
    int GetSelectedIndex(int previous=-1);
private:
    // Declaration-only native lifecycle; the observed view must not be constructed.
    // samp.dll RVA: 0x0008e810
    CDXUTListBoxDL(CDXUTDialog *parent);
    // samp.dll RVA: 0x0008e8d0
    ~CDXUTListBoxDL();
};
#pragma pack(pop)
typedef char DLListItemExtent[(sizeof(DXUTListBoxItemDL)==0x29e)?1:-1];
typedef char DLListItemData[(offsetof(DXUTListBoxItemDL,data)==0x284)?1:-1];
typedef char DLScrollExtent[(sizeof(CDXUTScrollBarDL)==0xd2)?1:-1];
typedef char DLScrollPosition[(offsetof(CDXUTScrollBarDL,position)==0x8e)?1:-1];
typedef char DLListExtent[(sizeof(CDXUTListBoxDL)==0x158)?1:-1];
typedef char DLListArrayOffset[(offsetof(CDXUTListBoxDL,items)==0x14c)?1:-1];
#endif
