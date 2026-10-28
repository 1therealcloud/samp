#ifndef SAMP_DXUT_LISTBOX_DL_H
#define SAMP_DXUT_LISTBOX_DL_H
#include <DXUT.h>
#include <DXUTgui.h>
#include <DXUTmisc.h>
#include <d3dx9.h>
#include <stddef.h>
#include <windows.h>
class CDXUTDialog;
#pragma pack(push, 1)
class CDXUTDialogDL;
class CDXUTButtonDL;
// ANSI factory view of the same parent, not a separately constructed dialog.
class CDXUTDialogDL
{
public:
    // samp.dll RVA: 0x0006ee60
    CDXUTButtonDL* GetButton(int id);
    // samp.dll RVA: 0x00090790
    HRESULT AddButton(int id, const char* text, int x, int y, int width, int height, UINT hotkey = 0, bool isDefault = false, CDXUTButtonDL** created = 0);
};
// Native ANSI controls share the established packed SDK base, not its wide text.
struct CDXUTStaticDL : CDXUTControl
{
    char text[260];
    // samp.dll RVA: 0x0008de50
    CDXUTStaticDL(CDXUTDialog* parent);
    // samp.dll RVA: 0x0008dec0
    virtual ~CDXUTStaticDL();
    // samp.dll RVA: 0x0008ded0
    virtual void Render(IDirect3DDevice9* device, float elapsed);
    // samp.dll RVA: 0x0008deb0
    virtual BOOL ContainsPoint(POINT point);
    // samp.dll RVA: 0x000875c0
    HRESULT SetText(const char* text);
};
class CDXUTButtonDL : public CDXUTStaticDL
{
    friend class CDXUTDialogDL;
    // samp.dll RVA: 0x0008df20
    CDXUTButtonDL(CDXUTDialog* parent);

public:
    bool pressed;
    // samp.dll RVA: 0x0008dfc0
    virtual ~CDXUTButtonDL();
    // samp.dll RVA: 0x0008dfd0
    virtual void Render(IDirect3DDevice9* device, float elapsed);
    // samp.dll RVA: 0x000848d0
    virtual bool HandleKeyboard(UINT message, WPARAM wparam, LPARAM lparam);
    // samp.dll RVA: 0x00084930
    virtual bool HandleMouse(UINT message, POINT point, WPARAM wparam, LPARAM lparam);
    // samp.dll RVA: 0x0008df80
    virtual bool CanHaveFocus();
    // samp.dll RVA: 0x0008dfa0
    virtual void OnHotkey();
    // samp.dll RVA: 0x0008df60
    virtual BOOL ContainsPoint(POINT point);
};
// Observed native storage views. Construction, virtual interfaces and rendering
// remain separate recovery work; these are not the SDK Unicode widget types.
struct DXUTListBoxItemDL
{
    char text[257];
    char subitems[3][129];
    void* data;
    RECT activeRect;
    bool selected;
    DWORD color;
    bool flag;
};

// These public SDK aliases resolve to its original function-local storage.
// samp.dll RVA: 0x00181a8d
extern bool g_dxutScrollDragging;
// samp.dll RVA: 0x00181a90
extern int g_dxutScrollThumbOffsetY;
// samp.dll RVA: 0x00181a95
extern bool g_dxutListScrollInitialized;
class CDXUTListBoxDL : public CDXUTControl
{
    typedef char ScrollPositionOffset[(offsetof(CDXUTScrollBar, m_nPosition) == 0x8e) ? 1 : -1];
    typedef char ScrollArrowOffset[(offsetof(CDXUTScrollBar, m_dArrowTS) == 0xaa) ? 1 : -1];
    friend class CDialog;

public:
    int columnCount;
    int columnWidths[3];
    CDXUTScrollBar scrollBar;
    RECT textRect, selectionRect;
    int scrollBarWidth, border, margin, textHeight;
    DWORD style;
    int selected, selectionStart;
    bool drag;
    CGrowableArray<DXUTListBoxItemDL*> items;
    // Native column accessors are unchecked, including negative indices.
    // samp.dll RVA: 0x0006a170
    void SetColumnCount(int value);
    // samp.dll RVA: 0x0006ee80
    int GetColumnCount();
    // samp.dll RVA: 0x0006a180
    void SetColumnWidth(int column, int value);
    // samp.dll RVA: 0x0006ee90
    int GetColumnWidth(int column);
    // samp.dll RVA: 0x0008e810
    CDXUTListBoxDL(CDXUTDialog* parent);
    // samp.dll RVA: 0x0008e8d0
    virtual ~CDXUTListBoxDL();
    // samp.dll RVA: 0x0008e900
    virtual void Render(IDirect3DDevice9* device, float elapsed);
    // samp.dll RVA: 0x0008a500
    virtual bool HandleKeyboard(UINT message, WPARAM wparam, LPARAM lparam);
    // samp.dll RVA: 0x0008a6e0
    virtual bool HandleMouse(UINT message, POINT point, WPARAM wparam, LPARAM lparam);
    // samp.dll RVA: 0x00088700
    virtual void UpdateRects();
    // samp.dll RVA: 0x0008e8c0
    virtual HRESULT OnInit();
    // samp.dll RVA: 0x0008e8a0
    virtual bool CanHaveFocus();
    // samp.dll RVA: 0x0008a270
    HRESULT AddItem(const char* text, void* data, int row);
    // Unlike AddItem, native insertion leaves subitems and the final flag untouched.
    // samp.dll RVA: 0x0008a370
    HRESULT InsertItem(int index, const char* text, void* data, DWORD color);
    // samp.dll RVA: 0x00088880
    void SetSubItemText(int row, int column, const char* text);
    // samp.dll RVA: 0x0008a440
    DXUTListBoxItemDL* GetItem(int row);
    // samp.dll RVA: 0x0008c270
    void RemoveAllItems();
    // samp.dll RVA: 0x00088800
    void RemoveItem(int index);
    // samp.dll RVA: 0x0008a470
    void SelectItem(int index);
    // samp.dll RVA: 0x000888f0
    int GetSelectedIndex(int previous = -1);
    // samp.dll RVA: 0x0006e240
    DXUTListBoxItemDL* GetSelectedItem(int previous = -1);
    // samp.dll RVA: 0x0006e260
    CDXUTScrollBar* GetScrollBar();
};
#pragma pack(pop)
typedef char DLListItemExtent[(sizeof(DXUTListBoxItemDL) == 0x29e) ? 1 : -1];
typedef char DLControlExtent[(sizeof(CDXUTControl) == 0x4d) ? 1 : -1];
typedef char DLStaticExtent[(sizeof(CDXUTStaticDL) == 0x151) ? 1 : -1];
typedef char DLButtonExtent[(sizeof(CDXUTButtonDL) == 0x152) ? 1 : -1];
typedef char DLListItemData[(offsetof(DXUTListBoxItemDL, data) == 0x284) ? 1 : -1];
typedef char DLScrollExtent[(sizeof(CDXUTScrollBar) == 0xb2) ? 1 : -1];
typedef char DLListExtent[(sizeof(CDXUTListBoxDL) == 0x158) ? 1 : -1];
typedef char DLListScroll[(offsetof(CDXUTListBoxDL, scrollBar) == 0x5d) ? 1 : -1];
typedef char DLListTextRect[(offsetof(CDXUTListBoxDL, textRect) == 0x10f) ? 1 : -1];
typedef char DLListSelectionRect[(offsetof(CDXUTListBoxDL, selectionRect) == 0x11f) ? 1 : -1];
typedef char DLListArrayOffset[(offsetof(CDXUTListBoxDL, items) == 0x14c) ? 1 : -1];
#endif
