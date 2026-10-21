#include "d3d9/common/dxut_listbox_native.h"
#include <strsafe.h>
#include <string.h>
#include <DXUTgui.h>
#include "gui/input_text.h"
#include <new>

// samp.dll RVA: 0x0008de50
CDXUTStaticDL::CDXUTStaticDL(CDXUTDialog *parent):CDXUTControl(0) {
    m_pDialog=parent;m_Type=DXUT_CONTROL_STATIC;
    memset(text,0,sizeof(text));
    for(int i=0;i<m_Elements.GetSize();++i)if(m_Elements[i])delete m_Elements[i];
    m_Elements.RemoveAll();
}

// samp.dll RVA: 0x0008dec0
CDXUTStaticDL::~CDXUTStaticDL() {}

// samp.dll RVA: 0x0008deb0
BOOL CDXUTStaticDL::ContainsPoint(POINT) {return FALSE;}

// samp.dll RVA: 0x0008ded0
void CDXUTStaticDL::Render(IDirect3DDevice9 *,float elapsed) {
    if(!m_bVisible)return;
    UINT state=m_bEnabled?DXUT_STATE_NORMAL:DXUT_STATE_DISABLED;
    CDXUTElement *element=m_Elements.GetAt(0);
    element->FontColor.Blend(state,elapsed,0.7f);
    m_pDialog->DrawTextA(text,element,&m_rcBoundingBox,true,-1);
}

// samp.dll RVA: 0x0008df20
CDXUTButtonDL::CDXUTButtonDL(CDXUTDialog *parent):CDXUTStaticDL(0) {
    m_pDialog=parent;m_Type=DXUT_CONTROL_BUTTON;pressed=false;m_nHotkey=0;
}

// samp.dll RVA: 0x0008dfc0
CDXUTButtonDL::~CDXUTButtonDL() {}

// samp.dll RVA: 0x0008df60
BOOL CDXUTButtonDL::ContainsPoint(POINT point) {return PtInRect(&m_rcBoundingBox,point);}

// samp.dll RVA: 0x0008df80
bool CDXUTButtonDL::CanHaveFocus() {return m_bVisible && m_bEnabled;}

// samp.dll RVA: 0x0008dfa0
void CDXUTButtonDL::OnHotkey() {
    m_pDialog->RequestFocus(this);
    m_pDialog->SendEvent(0x101,true,this);
}

// samp.dll RVA: 0x000848d0
bool CDXUTButtonDL::HandleKeyboard(UINT message,WPARAM wparam,LPARAM) {
    if(!m_bEnabled || !m_bVisible)return false;
    if(message==WM_KEYDOWN && wparam==VK_SPACE){pressed=true;return true;}
    if(message==WM_KEYUP && wparam==VK_SPACE) {
        if(pressed==true){pressed=false;m_pDialog->SendEvent(0x101,true,this);}
        return true;
    }
    return false;
}

// samp.dll RVA: 0x00084930
bool CDXUTButtonDL::HandleMouse(UINT message,POINT point,WPARAM,LPARAM) {
    if(!m_bEnabled || !m_bVisible)return false;
    if(message==WM_LBUTTONDOWN || message==WM_LBUTTONDBLCLK) {
        if(!ContainsPoint(point))return false;
        pressed=true;
        SetCapture(DXUTGetHWND());
        m_pDialog->SendEvent(0x102,true,this);
        if(!m_bHasFocus)m_pDialog->RequestFocus(this);
        return true;
    }
    if(message==WM_LBUTTONUP && pressed) {
        pressed=false;ReleaseCapture();
        if(!m_pDialog->m_bKeyboardInput)CDXUTDialog::ClearFocus();
        if(ContainsPoint(point))m_pDialog->SendEvent(0x101,true,this);
        return true;
    }
    return false;
}

// samp.dll RVA: 0x0008dfd0
void CDXUTButtonDL::Render(IDirect3DDevice9 *,float elapsed) {
    int offsetX=0,offsetY=0;
    UINT state=DXUT_STATE_NORMAL;
    if(!m_bVisible)state=DXUT_STATE_HIDDEN;
    else if(!m_bEnabled)state=DXUT_STATE_DISABLED;
    else if(pressed){state=DXUT_STATE_PRESSED;offsetX=1;offsetY=2;}
    else if(m_bMouseOver){state=DXUT_STATE_MOUSEOVER;offsetX=-1;offsetY=-2;}
    else if(m_bHasFocus)state=DXUT_STATE_FOCUS;
    CDXUTElement *element=m_Elements.GetAt(0);
    float rate=state==DXUT_STATE_PRESSED?0.0f:0.8f;
    RECT rect=m_rcBoundingBox;OffsetRect(&rect,offsetX,offsetY);
    element->TextureColor.Blend(state,elapsed,rate);
    element->FontColor.Blend(state,elapsed,rate);
    m_pDialog->DrawSprite(element,&rect);
    m_pDialog->DrawTextA(text,element,&rect,false,-1);
    element=m_Elements.GetAt(1);
    element->TextureColor.Blend(state,elapsed,rate);
    element->FontColor.Blend(state,elapsed,rate);
    m_pDialog->DrawSprite(element,&rect);
    m_pDialog->DrawTextA(text,element,&rect,false,-1);
}

// samp.dll RVA: 0x00090790
HRESULT CDXUTDialogDL::AddButton(int id,const char *text,int x,int y,int width,
    int height,UINT hotkey,bool isDefault,CDXUTButtonDL **created) {
    // This native factory has no constructor-failure cleanup frame.
    CDXUTButtonDL *button=(CDXUTButtonDL*)operator new(sizeof(CDXUTButtonDL));
    if(button)new(button) CDXUTButtonDL((CDXUTDialog*)this);
    if(created)*created=button;
    if(!button)return E_OUTOFMEMORY;
    HRESULT result=((CDXUTDialog*)this)->AddControl((CDXUTControl*)button);
    if(FAILED(result))return result;
    ((CDXUTControl*)button)->SetID(id);
    ((CDXUTStaticDL*)button)->SetText(text);
    ((CDXUTControl*)button)->SetLocation(x,y);
    ((CDXUTControl*)button)->SetSize(width,height);
    ((CDXUTControl*)button)->SetHotkey(hotkey);
    *((BYTE*)button+7)=(BYTE)isDefault;
    return S_OK;
}

// samp.dll RVA: 0x000850d0
const char *CDXUTEditBoxDL::GetText() { return InputWideToAnsi(wideText); }

// samp.dll RVA: 0x000875c0
HRESULT CDXUTStaticDL::SetText(const char *value) {
    if(!value){text[0]=0;return S_OK;}
    StringCchCopyA(text,260,value);
    return S_OK;
}

// samp.dll RVA: 0x00084e80
void CDXUTScrollBarDL::Cap() {
    if(position<start || end-start<=pageSize)position=start;
    else if(position+pageSize>end)position=end-pageSize;
}

// samp.dll RVA: 0x00084da0
void CDXUTScrollBarDL::UpdateThumbRect() {
    int range=end-start;
    if(range>pageSize) {
        int size=__max((track.bottom-track.top)*pageSize/range,8);
        int top=(track.bottom-track.top-size)*(position-start)/(range-pageSize)+track.top;
        showThumb=true;thumb.top=top;thumb.bottom=top+size;
    } else {
        showThumb=false;thumb.bottom=thumb.top;
    }
}

// samp.dll RVA: 0x000886d0
void CDXUTScrollBarDL::SetTrackRange(int first,int last) {
    start=first;end=last;Cap();UpdateThumbRect();
}

// samp.dll RVA: 0x00084e30
void CDXUTScrollBarDL::ShowItem(int index) {
    if(index<0)index=0;
    if(index>=end)index=end-1;
    if(position>index)position=index;
    else if(position+pageSize<=index)position=index-pageSize+1;
    UpdateThumbRect();
}

// samp.dll RVA: 0x0008a270
HRESULT CDXUTListBoxDL::AddItem(const char *value,void *data,int row) {
    DXUTListBoxItemDL *item=new DXUTListBoxItemDL;
    if(!item)return E_OUTOFMEMORY;
    StringCchCopyA(item->text,256,value);
    item->data=data;SetRect(&item->activeRect,0,0,0,0);
    item->selected=false;item->row=row;item->flag=false;
    // Native initializes only64 bytes of each129-byte subitem buffer.
    for(int i=0;i<3;++i)memset(item->subitems[i],0,64);
    HRESULT result=items.Add(item);
    if(FAILED(result)){delete item;return result;}
    scrollBar.SetTrackRange(0,items.GetSize());
    return result;
}

// samp.dll RVA: 0x00088880
void CDXUTListBoxDL::SetSubItemText(int row,int column,const char *value) {
    if(row<0 || row>=items.GetSize() || column<0 || column>=columnCount)return;
    memset(items[row]->subitems[column],0,128);
    strncpy(items[row]->subitems[column],value,128);
}

// samp.dll RVA: 0x0008a440
DXUTListBoxItemDL *CDXUTListBoxDL::GetItem(int row) {
    if(row<0 || row>=items.GetSize())return 0;
    return items[row];
}

// samp.dll RVA: 0x0008c270
void CDXUTListBoxDL::RemoveAllItems() {
    for(int i=0;i<items.GetSize();++i)delete items[i];
    items.RemoveAll();scrollBar.SetTrackRange(0,1);
}

// samp.dll RVA: 0x0008a470
void CDXUTListBoxDL::SelectItem(int index) {
    int count=items.GetSize();if(!count)return;
    int old=selected;selected=index;
    if(selected<0)selected=0;
    if(selected>=count)selected=count-1;
    if(old!=selected) {
        if(style&1)items[selected]->selected=true;
        selectionStart=selected;scrollBar.ShowItem(selected);
    }
    CDXUTDialog *parent=*(CDXUTDialog**)(control+0x18);
    parent->SendEvent(0x702,true,(CDXUTControl*)this);
}

// samp.dll RVA: 0x000888f0
int CDXUTListBoxDL::GetSelectedIndex(int previous) {
    if(previous<-1)return -1;
    if(style&1) {
        int count=items.GetSize();
        for(int i=previous+1;i<count;++i)if(items[i]->selected)return i;
        return -1;
    }
    return selected;
}
