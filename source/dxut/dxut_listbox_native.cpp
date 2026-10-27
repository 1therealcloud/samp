#include "dxut/dxut_listbox_native.h"
#include <strsafe.h>
#include <string.h>
#include <DXUTgui.h>
#include "gui/input_text.h"
#include <new>

// samp.dll RVA: 0x0008e750
CDXUTScrollBarDL::CDXUTScrollBarDL(CDXUTDialog *parent):CDXUTControl(0) {
    m_Type=DXUT_CONTROL_SCROLLBAR;m_pDialog=parent;showThumb=true;
    SetRect(&up,0,0,0,0);SetRect(&down,0,0,0,0);
    SetRect(&track,0,0,0,0);SetRect(&thumb,0,0,0,0);
    position=0;pageSize=1;start=0;end=1;arrowState=0;arrowTimestamp=0.0;
}

// samp.dll RVA: 0x0008e800
CDXUTScrollBarDL::~CDXUTScrollBarDL() {}

// samp.dll RVA: 0x00088250
bool CDXUTScrollBarDL::HandleMouse(UINT message,POINT point,WPARAM,LPARAM) {
    lastMouse=point;
    switch(message) {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
        if(PtInRect(&up,point)) {
            SetCapture(DXUTGetHWND());
            if(position>start)--position;
            UpdateThumbRect();arrowState=1;arrowTimestamp=DXUTGetTime();return true;
        }
        if(PtInRect(&down,point)) {
            SetCapture(DXUTGetHWND());
            if(position+pageSize<end)++position;
            UpdateThumbRect();arrowState=2;arrowTimestamp=DXUTGetTime();return true;
        }
        if(PtInRect(&thumb,point)) {
            SetCapture(DXUTGetHWND());
            g_dxutScrollThumbOffsetY=point.y-thumb.top;g_dxutScrollDragging=true;return true;
        }
        if(thumb.left<=point.x && thumb.right>point.x) {
            SetCapture(DXUTGetHWND());
            if(thumb.top>point.y && track.top<=point.y){Scroll(1-pageSize);return true;}
            if(thumb.bottom<=point.y && track.bottom>point.y){Scroll(pageSize-1);return true;}
        }
        break;
    case WM_LBUTTONUP:
        g_dxutScrollDragging=false;ReleaseCapture();UpdateThumbRect();arrowState=0;break;
    case WM_MOUSEMOVE:
        if(g_dxutScrollDragging) {
            thumb.bottom+=point.y-g_dxutScrollThumbOffsetY-thumb.top;
            thumb.top=point.y-g_dxutScrollThumbOffsetY;
            if(thumb.top<track.top)OffsetRect(&thumb,0,track.top-thumb.top);
            else if(thumb.bottom>track.bottom)OffsetRect(&thumb,0,track.bottom-thumb.bottom);
            int maxFirst=end-start-pageSize;
            int maxThumb=(track.bottom-track.top)-(thumb.bottom-thumb.top);
            position=start+(thumb.top-track.top+maxThumb/(maxFirst*2))*maxFirst/maxThumb;
            return true;
        }
        break;
    }
    return false;
}

// samp.dll RVA: 0x000884d0
void CDXUTScrollBarDL::Render(IDirect3DDevice9 *,float elapsed) {
    if(arrowState) {
        double now=DXUTGetTime();
        if(PtInRect(&up,lastMouse)) {
            if(arrowState==1 && 0.33<now-arrowTimestamp){Scroll(-1);arrowState=3;arrowTimestamp=now;}
            else if(arrowState==3 && 0.05<now-arrowTimestamp){Scroll(-1);arrowTimestamp=now;}
        } else if(PtInRect(&down,lastMouse)) {
            if(arrowState==2 && 0.33<now-arrowTimestamp){Scroll(1);arrowState=4;arrowTimestamp=now;}
            else if(arrowState==4 && 0.05<now-arrowTimestamp){Scroll(1);arrowTimestamp=now;}
        }
    }
    UINT state=DXUT_STATE_NORMAL;
    if(!m_bVisible)state=DXUT_STATE_HIDDEN;
    else if(!m_bEnabled || !showThumb)state=DXUT_STATE_DISABLED;
    else if(m_bMouseOver)state=DXUT_STATE_MOUSEOVER;
    else if(m_bHasFocus)state=DXUT_STATE_FOCUS;
    CDXUTElement *element=m_Elements.GetAt(0);
    element->TextureColor.Blend(state,elapsed,0.8f);m_pDialog->DrawSprite(element,&track);
    element=m_Elements.GetAt(1);
    element->TextureColor.Blend(state,elapsed,0.8f);m_pDialog->DrawSprite(element,&up);
    element=m_Elements.GetAt(2);
    element->TextureColor.Blend(state,elapsed,0.8f);m_pDialog->DrawSprite(element,&down);
    element=m_Elements.GetAt(3);
    element->TextureColor.Blend(state,elapsed,0.8f);m_pDialog->DrawSprite(element,&thumb);
}

// Reconstruction-only access to the shared base layout/nonvirtual body;
// no fields, virtual overrides or established standalone native entry.
struct DLControlAccess : CDXUTControl {
    using CDXUTControl::UpdateRects;
    using CDXUTControl::m_rcBoundingBox;
    using CDXUTControl::m_bEnabled;
};

// samp.dll RVA: 0x000881b0
void CDXUTScrollBarDL::UpdateRects() {
    DLControlAccess *base=(DLControlAccess*)this;
    base->DLControlAccess::UpdateRects();
    const RECT &bounds=base->m_rcBoundingBox;
    SetRect(&up,bounds.left,bounds.top,bounds.right,bounds.top+bounds.right-bounds.left);
    SetRect(&down,bounds.left,bounds.bottom-(bounds.right-bounds.left),bounds.right,bounds.bottom);
    SetRect(&track,up.left,up.bottom,down.right,down.top);
    thumb.left=up.left;thumb.right=up.right;
    UpdateThumbRect();
}

// samp.dll RVA: 0x00088230
void CDXUTScrollBarDL::Scroll(int delta) {position+=delta;Cap();UpdateThumbRect();}

// samp.dll RVA: 0x00084e70
bool CDXUTScrollBarDL::HandleKeyboard(UINT,WPARAM,LPARAM) {return false;}

// samp.dll RVA: 0x0008e810
CDXUTListBoxDL::CDXUTListBoxDL(CDXUTDialog *parent):CDXUTControl(0),scrollBar(parent) {
    m_pDialog=parent;style=0;selectionStart=0;drag=false;textHeight=0;
    columnCount=0;m_Type=DXUT_CONTROL_LISTBOX;scrollBarWidth=16;
    selected=-1;border=6;margin=5;
    columnWidths[0]=columnWidths[1]=columnWidths[2]=0;
}

// samp.dll RVA: 0x0008e8d0
CDXUTListBoxDL::~CDXUTListBoxDL() {RemoveAllItems();}

// samp.dll RVA: 0x0008a500
bool CDXUTListBoxDL::HandleKeyboard(UINT message,WPARAM key,LPARAM lparam) {
    if(!m_bEnabled || !m_bVisible)return false;
    if(scrollBar.HandleKeyboard(message,key,lparam))return true;
    if(message!=WM_KEYDOWN)return false;
    switch(key) {
    case VK_UP:case VK_DOWN:case VK_NEXT:case VK_PRIOR:case VK_HOME:case VK_END:
        break;
    default:return false;
    }
    int count=items.GetSize();if(!count)return true;
    int old=selected;
    switch(key) {
    case VK_UP:--selected;break;
    case VK_DOWN:++selected;break;
    case VK_NEXT:selected+=scrollBar.pageSize-1;break;
    case VK_PRIOR:selected-=scrollBar.pageSize-1;break;
    case VK_HOME:selected=0;break;
    case VK_END:selected=count-1;break;
    }
    if(selected<0)selected=0;
    if(selected>=count)selected=count-1;
    if(old!=selected) {
        if(style&1) {
            for(int i=0;i<items.GetSize();++i)items[i]->selected=false;
            if(GetKeyState(VK_SHIFT)<0) {
                int first=selectionStart,last=selected;
                if(first>last){int swap=first;first=last;last=swap;}
                for(int i=first;i<=last;++i)items[i]->selected=true;
            } else {
                items[selected]->selected=true;selectionStart=selected;
            }
        } else selectionStart=selected;
        scrollBar.ShowItem(selected);
        m_pDialog->SendEvent(0x702,true,this);
    }
    return true;
}

// samp.dll RVA: 0x0008a6e0
bool CDXUTListBoxDL::HandleMouse(UINT message,POINT point,WPARAM keys,LPARAM lparam) {
    if(!m_bEnabled || !m_bVisible)return false;
    if(message==WM_LBUTTONDOWN && !m_bHasFocus)m_pDialog->RequestFocus(this);
    if(scrollBar.HandleMouse(message,point,keys,lparam))return true;
    switch(message) {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
        if(items.GetSize()>0 && PtInRect(&selectionRect,point)) {
            int clicked=textHeight?scrollBar.position+(point.y-textRect.top)/textHeight:-1;
            if(clicked<scrollBar.position || clicked>=items.GetSize() ||
                clicked>=scrollBar.position+scrollBar.pageSize)return true;
            SetCapture(DXUTGetHWND());
            if(message==WM_LBUTTONDBLCLK){m_pDialog->SendEvent(0x701,true,this);return true;}
            selected=clicked;if(!(keys&MK_SHIFT))selectionStart=clicked;
            if(style&1) {
                DXUTListBoxItemDL *item=items[clicked];
                int modifiers=keys&(MK_SHIFT|MK_CONTROL);
                if(modifiers==MK_CONTROL)item->selected=!item->selected;
                else if(modifiers==MK_SHIFT) {
                    int first=selectionStart,last=clicked;
                    if(first>last){int swap=first;first=last;last=swap;}
                    for(int i=0;i<first;++i)items[i]->selected=false;
                    for(int i=last+1;i<items.GetSize();++i)items[i]->selected=false;
                    for(int i=first;i<=last;++i)items[i]->selected=true;
                } else if(modifiers==(MK_SHIFT|MK_CONTROL)) {
                    int first=selectionStart,last=clicked;
                    if(first>last){int swap=first;first=last;last=swap;}
                    bool state=items[selectionStart]->selected;
                    for(int i=first+1;i<last;++i)items[i]->selected=state;
                    item->selected=true;selected=selectionStart;
                } else {
                    for(int i=0;i<items.GetSize();++i)items[i]->selected=false;
                    item->selected=true;
                }
            }
            m_pDialog->SendEvent(0x702,true,this);return true;
        }
        break;
    case WM_LBUTTONUP:
        ReleaseCapture();drag=false;
        if(selected<items.GetSize() && selected!=-1) {
            int first=selectionStart,last=selected;
            if(first>last){int swap=first;first=last;last=swap;}
            for(int i=first+1;i<last;++i)items[i]->selected=items[selectionStart]->selected;
            items[selected]->selected=items[selectionStart]->selected;
            if(selectionStart!=selected)m_pDialog->SendEvent(0x702,true,this);
        }
        break;
    case WM_MOUSEMOVE:
        if(drag) {
            int item=textHeight?scrollBar.position+(point.y-textRect.top)/textHeight:-1;
            if(item>=scrollBar.position && item<items.GetSize() &&
                item<scrollBar.position+scrollBar.pageSize)selected=item;
            else if(item<scrollBar.position){scrollBar.Scroll(-1);selected=scrollBar.position;}
            else if(item>=scrollBar.position+scrollBar.pageSize) {
                scrollBar.Scroll(1);int last=scrollBar.position+scrollBar.pageSize;
                if(items.GetSize()<last)last=items.GetSize();selected=last-1;
            } else break;
            m_pDialog->SendEvent(0x702,true,this);
        }
        break;
    case WM_MOUSEWHEEL: {
        UINT lines;SystemParametersInfoA(SPI_GETWHEELSCROLLLINES,0,&lines,0);
        int amount=(short)HIWORD(keys)/WHEEL_DELTA*lines;
        scrollBar.Scroll(-amount);return true;
    }
    }
    return false;
}

// samp.dll RVA: 0x0008e900
void CDXUTListBoxDL::Render(IDirect3DDevice9 *device,float elapsed) {
    if(!m_bVisible)return;
    CDXUTElement *normal=m_Elements.GetAt(0);
    normal->TextureColor.Blend(DXUT_STATE_NORMAL,elapsed,0.7f);
    normal->FontColor.Blend(DXUT_STATE_NORMAL,elapsed,0.7f);
    CDXUTElement *selection=m_Elements.GetAt(1);
    selection->TextureColor.Blend(DXUT_STATE_NORMAL,elapsed,0.7f);
    selection->FontColor.Blend(DXUT_STATE_NORMAL,elapsed,0.7f);
    m_pDialog->DrawSprite(normal,&m_rcBoundingBox);
    if(items.GetSize()>0) {
        RECT text=textRect,highlight=selectionRect;
        text.bottom=text.top+m_pDialog->GetFont(normal->iFont)->nHeight;
        textHeight=text.bottom-text.top;
        if(scrollBar.position<0){scrollBar.position=0;scrollBar.Cap();scrollBar.UpdateThumbRect();}
        if(!g_dxutListScrollInitialized) {
            scrollBar.pageSize=textHeight?(textRect.bottom-textRect.top)/textHeight:textRect.bottom-textRect.top;
            scrollBar.Cap();scrollBar.UpdateThumbRect();g_dxutListScrollInitialized=true;
        }
        text.right=textRect.right;
        for(int i=scrollBar.position;i<items.GetSize();++i) {
            if(text.bottom>textRect.bottom)break;
            DXUTListBoxItemDL *item=items[i];bool selectedStyle=false;
            if(!(style&1) && i==selected)selectedStyle=true;
            else if(style&1) {
                if(drag && ((i>=selected && i<selectionStart)||(i<=selected && i>selectionStart)))
                    selectedStyle=items[selectionStart]->selected;
                else if(item->selected)selectedStyle=true;
            }
            CDXUTElement *element=normal;
            if(!item->flag && selectedStyle) {
                highlight.top=text.top;highlight.bottom=text.bottom;
                m_pDialog->DrawSprite(selection,&highlight);element=selection;
            } else if(item->color) {
                const float scale=1.0f/255.0f;
                normal->FontColor.Current.r=(float)((item->color>>16)&255)*scale;
                normal->FontColor.Current.g=(float)((item->color>>8)&255)*scale;
                normal->FontColor.Current.b=(float)(item->color&255)*scale;
                normal->FontColor.Current.a=(float)(item->color>>24)*scale;
            }
            m_pDialog->DrawTextA(item->text,element,&text,false,-1);
            if(columnCount>0) {
                int left=text.left;
                for(int column=0;column<columnCount;++column) {
                    text.left+=columnWidths[column];
                    m_pDialog->DrawTextA(item->subitems[column],element,&text,false,-1);
                }
                text.left=left;
            }
            OffsetRect(&text,0,textHeight);
        }
    }
    scrollBar.Render(device,elapsed);
}

// samp.dll RVA: 0x0008e8c0
HRESULT CDXUTListBoxDL::OnInit() {
    return ((CDXUTControl*)this)->m_pDialog->InitControl((CDXUTControl*)&scrollBar);
}

// samp.dll RVA: 0x0008e8a0
bool CDXUTListBoxDL::CanHaveFocus() {
    DLControlAccess *base=(DLControlAccess*)this;
    return base->m_bVisible && base->m_bEnabled;
}

// samp.dll RVA: 0x00088700
void CDXUTListBoxDL::UpdateRects() {
    DLControlAccess *base=(DLControlAccess*)this;
    base->DLControlAccess::UpdateRects();
    selectionRect=base->m_rcBoundingBox;selectionRect.right-=scrollBarWidth;
    InflateRect(&selectionRect,-border,-border);
    textRect=selectionRect;InflateRect(&textRect,-margin,0);
    ((CDXUTControl*)&scrollBar)->SetLocation(base->m_rcBoundingBox.right-scrollBarWidth,base->m_rcBoundingBox.top);
    ((CDXUTControl*)&scrollBar)->SetSize(scrollBarWidth,base->m_height);
    DXUTFontNode *font=base->m_pDialog->GetFont(base->GetElement(0)->iFont);
    if(font && font->nHeight) {
        scrollBar.pageSize=(textRect.bottom-textRect.top)/font->nHeight;
        scrollBar.Cap();scrollBar.UpdateThumbRect();scrollBar.ShowItem(selected);
    }
}

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

// samp.dll RVA: 0x00066b50
void CDXUTScrollBarDL::SetTrackPos(int value) {
    position=value;Cap();UpdateThumbRect();
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
    item->selected=false;item->color=row;item->flag=false;
    // Native initializes only64 bytes of each129-byte subitem buffer.
    for(int i=0;i<3;++i)memset(item->subitems[i],0,64);
    HRESULT result=items.Add(item);
    if(FAILED(result)){delete item;return result;}
    scrollBar.SetTrackRange(0,items.GetSize());
    return result;
}

// samp.dll RVA: 0x0008a370
HRESULT CDXUTListBoxDL::InsertItem(int index,const char *value,void *data,DWORD color) {
    DXUTListBoxItemDL *item=new DXUTListBoxItemDL;
    if(!item)return E_OUTOFMEMORY;
    StringCchCopyA(item->text,256,value);
    item->data=data;SetRect(&item->activeRect,0,0,0,0);
    item->selected=false;item->color=color;
    HRESULT result=items.Insert(index,item);
    if(SUCCEEDED(result))scrollBar.SetTrackRange(0,items.GetSize());
    else delete item;
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

// samp.dll RVA: 0x00088800
void CDXUTListBoxDL::RemoveItem(int index) {
    if(index<0 || index>=items.GetSize())return;
    delete items[index];
    items.Remove(index);
    scrollBar.SetTrackRange(0,items.GetSize());
    if(selected>=items.GetSize())selected=items.GetSize()-1;
    m_pDialog->SendEvent(EVENT_LISTBOX_SELECTION,true,this);
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
    CDXUTDialog *parent=m_pDialog;
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
