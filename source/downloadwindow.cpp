#include "downloadwindow.h"
// samp.dll RVA: 0x0006a410
CDownloadWindow *CDownloadWindow::InitializeRecordStorage() {
    *(DWORD*)nativeState=0;*(DWORD*)(nativeState+4)=0;
    return this;
}
// samp.dll RVA: 0x0006aa00
bool CDownloadWindow::ReleaseRecordStorage() {return ResizeRecords(0);}
// samp.dll RVA: 0x0006a440
int CDownloadWindow::GetFilesCount() {return *(int*)(nativeState+4);}
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

#include "objectedit.h"
#include "game/vehicle.h"
#include <stdlib.h>
#include <stdio.h>
// samp.dll RVA: 0x0011c414
const char g_downloadModel[6]="Model";
// samp.dll RVA: 0x0011c40c
const char g_downloadTexture[8]="Texture";
// samp.dll RVA: 0x0011c404
const char g_downloadIdFormat[5]="0x%X";
// samp.dll RVA: 0x0011c400
const char g_downloadZeroFormat[4]="0%%";
// samp.dll RVA: 0x0011c3f8
const char g_downloadPercentFormat[7]="%.0f%%";
// samp.dll RVA: 0x0011c3e0
const char g_downloadDetailFormat[21]="{CC8888}(%d) %.16s..";
// samp.dll RVA: 0x0011c3c8
const char g_downloadErrorFormat[24]="{CC8888}Error(%d) %d/%d";
// samp.dll RVA: 0x0011c3b8
const char g_downloadCompleteFormat[15]="{77AA77}%.0f%%";
// samp.dll RVA: 0x0011c390
const char g_downloadCaptionFormat[38]="Updating models.. [%d/%d] (%.2f KB/s)";

// samp.dll RVA: 0x0011b918
const float g_downloadKilobytesPerByte=0.0009765625f;
// samp.dll RVA: 0x0006a420
DownloadRecord *CDownloadWindow::GetRecord(DWORD index) {
    DWORD *state=(DWORD*)nativeState;
    return index<state[1]?((DownloadRecord**)state[0])[index]:0;
}
// samp.dll RVA: 0x0006a450
bool CDownloadWindow::ResizeRecords(DWORD count) {
    DWORD *state=(DWORD*)nativeState;
    if(!count) {
        if(state[0]){free((void*)state[0]);state[0]=0;}
        state[1]=0;return true;
    }
    DownloadRecord **records=(DownloadRecord**)realloc((void*)state[0],count*4);
    if(!records)return false;
    state[0]=(DWORD)records;
    for(DWORD i=state[1];i<count;i++)((DownloadRecord**)state[0])[i]=0;
    state[1]=count;return true;
}
// samp.dll RVA: 0x0006a4c0
void CDownloadWindow::TrimRecords() {
    DWORD *state=(DWORD*)nativeState;DWORD count=state[1],last=0xffffffff;
    for(DWORD i=0;i<count;i++)if(((DownloadRecord**)state[0])[i])last=i;
    ResizeRecords(last==0xffffffff?0:last+1);
}
// samp.dll RVA: 0x0006a610
int CDownloadWindow::FindRecord(DWORD id) {
    DWORD *state=(DWORD*)nativeState;DWORD count=state[1];
    // Native assumes every visited record is non-null; holes are not skipped.
    for(DWORD i=0;i<count;i++)if(((DownloadRecord**)state[0])[i]->id==id)return (int)i;
    return -1;
}
// samp.dll RVA: 0x0006a640
void CDownloadWindow::ResetRecordStorage() {ResizeRecords(0);}
// samp.dll RVA: 0x0006a650
int CDownloadWindow::AddRecord(DownloadRecord *record) {
    DWORD *state=(DWORD*)nativeState;DWORD count=state[1];
    for(DWORD i=0;i<count;i++)if(!((DownloadRecord**)state[0])[i]) {
        if((LONG)i<0)break;
        ((DownloadRecord**)state[0])[i]=record;return (int)i;
    }
    if(!ResizeRecords(count+1))return -1;
    ((DownloadRecord**)state[0])[count]=record;return (int)count;
}
// samp.dll RVA: 0x0006a6b0
bool CDownloadWindow::RemoveRecord(DWORD index) {
    DWORD *state=(DWORD*)nativeState;if(index>=state[1])return false;
    ((DownloadRecord**)state[0])[index]=0;TrimRecords();return true;
}
// samp.dll RVA: 0x0006a6e0
void CDownloadWindow::ClearRecords() {
    DWORD *state=(DWORD*)nativeState;int count=(int)state[1];
    for(int i=0;i<count;i++)if((DWORD)i<state[1]) {
        DownloadRecord *record=((DownloadRecord**)state[0])[i];
        if(record){free(record);RemoveRecord(i);}
    }
    ((CDXUTListBoxDL*)state[11])->RemoveAllItems();
}
// samp.dll RVA: 0x0006ab10
void CDownloadWindow::Hide() {
    DWORD *state=(DWORD*)nativeState;((BYTE*)state[10])[0x13]=0;state[12]=0;
    ClearRecords();state[0x93]=0;state[0x94]=0xffffffff;
}
// samp.dll RVA: 0x0006a720
void CDownloadWindow::UpdateRecord(BYTE type,DWORD id,BYTE status,DWORD downloaded,DWORD total,int error,const char *detail) {
    DWORD *state=(DWORD*)nativeState;int index=FindRecord(id);
    DownloadRecord *record;DWORD delta;BYTE currentStatus=status;
    char text[64];
    if(index==-1) {
        record=(DownloadRecord*)calloc(1,18);
        record->type=type;record->id=id;record->status=status;
        record->downloaded=downloaded;record->total=total;delta=downloaded;
        _snprintf(text,64,type==1?g_downloadModel:g_downloadTexture);
        ((CDXUTListBoxDL*)state[11])->AddItem(text,(void*)0xffffffff,-1);
        record->row=state[1];AddRecord(record);
        _snprintf(text,64,g_downloadIdFormat,id);
        ((CDXUTListBoxDL*)state[11])->SetSubItemText(record->row,0,text);
        ((CDXUTListBoxDL*)state[11])->SelectItem(record->row);
        ((CDXUTListBoxDL*)state[11])->GetItem(record->row)->flag=true;
    } else {
        record=GetRecord(index);delta=downloaded-record->downloaded;
        record->status=status;record->downloaded=downloaded;
    }
    float percentage=0.0f;
    if(downloaded>0&&record->total>0) {
        DWORD denominator=record->total;
        __asm {
            mov eax,downloaded
            test eax,eax
            fild downloaded
            jge numerator_signed
            fadd g_cursorRayUnsignedBias
numerator_signed:
            fmul g_vehicleTrainSpeedMaximum
            mov eax,denominator
            test eax,eax
            fild denominator
            jge denominator_signed
            fadd g_cursorRayUnsignedBias
denominator_signed:
            fdivp st(1),st(0)
            fstp percentage
        }
    }
    state[0x96]+=delta;
    if((LONG)(RakNet::GetTime()-state[0x95])>1000) {
        state[0x95]=RakNet::GetTime();DWORD bytes=state[0x96];state[0x97]=0;
        if(bytes) {
            float speed;
            __asm {
                mov eax,bytes
                test eax,eax
                fild bytes
                jge bytes_signed
                fadd g_cursorRayUnsignedBias
bytes_signed:
                fmul g_downloadKilobytesPerByte
                fstp speed
            }
            memcpy(&state[0x97],&speed,4);
        }
        state[0x96]=0;
    }
    if(currentStatus==0)_snprintf(text,64,g_downloadZeroFormat);
    else if(currentStatus==1)_snprintf(text,64,g_downloadPercentFormat,(double)percentage);
    else if(currentStatus==3) {
        if(detail)_snprintf(text,64,g_downloadDetailFormat,error,detail);
        else _snprintf(text,64,g_downloadErrorFormat,error,record->downloaded,record->total);
    } else if(currentStatus==4) {
        state[0x93]++;_snprintf(text,64,g_downloadCompleteFormat,(double)percentage);
    }
    // Other native statuses leave the previous or untouched local text in place.
    ((CDXUTListBoxDL*)state[11])->SetSubItemText(record->row,1,text);
    if((LONG)state[0x94]>0) {
        char title[64];float speed;memcpy(&speed,&state[0x97],4);
        _snprintf(title,64,g_downloadCaptionFormat,state[0x93],state[0x94],(double)speed);
        strcpy((char*)state[10]+0x16,title);
    }
}
