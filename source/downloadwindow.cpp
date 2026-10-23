#include "downloadwindow.h"
#include "fontrender.h"
#include "gui/device_lifecycle.h"
#include "game/gta_globals.h"
#include "dxut/dxut_listbox_native.h"
#include "GetTime.h"
#include <string.h>
// samp.dll RVA: 0x0011c37c
const char g_downloadWindowTitle[18]="Updating models..";
// samp.dll RVA: 0x0006aa10
CDownloadWindow::CDownloadWindow(IDirect3DDevice9 *device){
    DWORD *state=(DWORD*)nativeState;state[0]=0;state[1]=0;state[2]=(DWORD)device;state[10]=0;state[12]=0;
    state[5]=640;state[6]=300;state[7]=210;state[8]=30;state[13]=3;
    memcpy(nativeState+0x38,"Type",5);
    memcpy(nativeState+0xb9,"ID",3);
    memcpy(nativeState+0x13a,"Progress",9);
    state[0x8f]=180;state[0x90]=220;state[0x93]=0;state[0x94]=0xffffffff;
    DWORD started=RakNet::GetTime();state[0x97]=0;state[0x96]=0;state[0x95]=started;
}

// samp.dll RVA: 0x0006a1a0
void CDownloadWindow::ResetFonts() {
    DWORD *state=(DWORD*)nativeState;BYTE *dialog=(BYTE*)state[10];
    DWORD height=state[6],width=state[5];
    *(DWORD*)(dialog+0x11e)=width;*(DWORD*)(dialog+0x122)=height;
    RECT client;GetClientRect(g_inputGameWindow,&client);
    // Native signed SUB/CDQ/SAR halves, with wrapping subtraction.
    LONG w=(LONG)state[5];LONG halfW=(LONG)((DWORD)w-(w<0?0xffffffffUL:0))>>1;
    LONG right=client.right;LONG halfRight=(LONG)((DWORD)right-(right<0?0xffffffffUL:0))>>1;
    LONG bottom=client.bottom;LONG halfBottom=(LONG)((DWORD)bottom-(bottom<0?0xffffffffUL:0))>>1;
    LONG h=(LONG)state[6];LONG halfH=(LONG)((DWORD)h-(h<0?0xffffffffUL:0))>>1;
    state[3]=(DWORD)halfRight-(DWORD)halfW;state[4]=(DWORD)halfBottom-(DWORD)halfH;
    dialog=(BYTE*)state[10];*(DWORD*)(dialog+0x116)=state[3];*(DWORD*)(dialog+0x11a)=state[4];
    state[9]=state[6]-*(DWORD*)((BYTE*)state[10]+0x126);
    SIZE measured;g_deviceFonts->GetLittleTextScreenSize((FontSize*)&measured,g_fontMeasureProbe,0);
    CDXUTControl *list=(CDXUTControl*)state[11];
    list->SetLocation(2,(LONG)((DWORD)measured.cy+2));
    list=(CDXUTControl*)state[11];list->SetSize((LONG)(state[5]-4),(LONG)(state[9]-(DWORD)measured.cy-4));
}
// samp.dll RVA: 0x0006a280
void CDownloadWindow::GetRect(RECT *out) {
    DWORD *state=(DWORD*)nativeState;
    out->left=state[3];out->right=(LONG)(state[5]+(DWORD)out->left);
    out->top=state[4];out->bottom=(LONG)(state[6]+(DWORD)out->top);
}
// samp.dll RVA: 0x0006a2b0
void CDownloadWindow::Show(DWORD mode) {
    DWORD *state=(DWORD*)nativeState;state[0x94]=mode;
    SIZE measured;g_deviceFonts->GetTextScreenSize((FontSize*)&measured,g_fontMeasureProbe,0);
    strcpy((char*)state[10]+0x16,g_downloadWindowTitle);
    ((BYTE*)state[10])[0x14]=1;
    *(DWORD*)((BYTE*)state[10]+0x126)=(DWORD)measured.cy+4;
    ((BYTE*)state[10])[1]=0;((BYTE*)state[10])[2]=0;((BYTE*)state[10])[0x13]=1;
    if(state[11]) {
        ((CDXUTControl*)state[11])->SetEnabled(true);
        ((CDXUTControl*)state[11])->SetVisible(true);
    }
    ResetFonts();state[12]=1;
}
// samp.dll RVA: 0x0006a350
void CDownloadWindow::Draw() {
    DWORD *state=(DWORD*)nativeState;if(!state[12])return;
    CDXUTDialog *dialog=(CDXUTDialog*)state[10];if(dialog)dialog->OnRender(10.0f);
    RECT rect;GetRect(&rect);
    rect.top=(LONG)((DWORD)rect.top+*(DWORD*)((BYTE*)state[10]+0x126)+3);
    rect.left=(LONG)((DWORD)rect.left+16);
    // Native signed count is live and unbounded; three columns are initialized by construction.
    for(int column=0;column<(LONG)state[13];column++) {
        g_deviceFonts->DrawLittleText(0,(const char*)nativeState+0x38+column*129,rect,0x120,0xffa9c4e4,TRUE);
        rect.left=(LONG)((DWORD)rect.left+*(DWORD*)(nativeState+0x23c+column*4));
    }
}
// samp.dll RVA: 0x0006a500
void CDownloadWindow::ResetDialogControls(CDXUTDialog *dialog) {
    DWORD *state=(DWORD*)nativeState;state[10]=(DWORD)dialog;if(!dialog)return;
    CDXUTListBoxDL *created=new CDXUTListBoxDL((CDXUTDialog*)state[10]);
    state[11]=(DWORD)created;
    ((CDXUTDialog*)state[10])->AddControl(created);
    ((CDXUTControl*)state[11])->SetLocation(2,2);
    ((CDXUTControl*)state[11])->SetSize((LONG)(state[5]-4),(LONG)(state[6]-35));
    ((CDXUTControl*)state[11])->OnInit();
    ((CDXUTControl*)state[11])->GetElement(0)->TextureColor.Init(0xc8ffffff,0xc8808080,0);
    ((CDXUTControl*)state[11])->SetVisible(false);
    ((CDXUTControl*)state[11])->SetEnabled(false);
    created=(CDXUTListBoxDL*)state[11];created->columnCount=2;
    created=(CDXUTListBoxDL*)state[11];created->columnWidths[0]=state[0x8f];
    created=(CDXUTListBoxDL*)state[11];created->columnWidths[1]=state[0x90];
}
