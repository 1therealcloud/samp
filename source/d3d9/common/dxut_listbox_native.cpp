#include "d3d9/common/dxut_listbox_native.h"
#include <strsafe.h>
#include <string.h>

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
