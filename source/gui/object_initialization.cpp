#include "gui/object_edit.h"
#include "gui/object_selection.h"
// samp.dll RVA: 0x00071600
CObjectEdit::CObjectEdit(IDirect3DDevice9 *value) {
    device=value;unknown7c=0;unknown113=0xffffffff;unknown80=0;unknown84=0;unknown78=0;
    flags[1]=0;flags[2]=0;flags[3]=0;flags[4]=0;flags[0]=0;objectId=0xffff;
    lastTick=GetTickCount();D3DXCreateLine(value,&line);
    D3DXCreateFontA(value,22,0,400,1,FALSE,SYMBOL_CHARSET,0,0,0,"SAMPAUX3",&smallFont);
    D3DXCreateFontA(value,28,0,400,1,FALSE,SYMBOL_CHARSET,0,0,0,"SAMPAUX3",&largeFont);
}
// samp.dll RVA: 0x0006d440
CObjectSelection::CObjectSelection(){enabled=0;objectId=0xffff;}
