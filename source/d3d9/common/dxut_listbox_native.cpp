#include "d3d9/common/dxut_listbox_native.h"
#include <strsafe.h>
#include <string.h>
#include <DXUTgui.h>
#include "gui/input_text.h"

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
