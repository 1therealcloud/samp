#include "dialog.h"
#include "gui/chat_dependencies.h"
#include "gui/device_lifecycle.h"
#include "gui/render_dependencies.h"
#include "game/game.h"
#include "main.h"
#include "cmdwindow.h"
#include <DXUT.h>
#include <DXUTgui.h>
#include <string.h>

// samp.dll RVA: 0x0006eec0
CDialog::CDialog(IDirect3DDevice9 *device){
    DWORD *state=(DWORD*)nativeState;state[0]=(DWORD)device;state[7]=0;state[10]=0;state[13]=0;state[12]=0;state[11]=0;
    state[3]=600;state[4]=300;state[5]=100;state[6]=30;memset(nativeState+0x89,0,0x204);
}

// samp.dll RVA: 0x0006ef10
bool CDialog::MsgProc(UINT message,WPARAM wparam,LPARAM lparam) {return CDXUTIMEEditBox::StaticMsgProc(message,wparam,lparam)!=false;}

// samp.dll RVA: 0x0006ef30
bool CDialog::IsInputHidden() {if(!*(DWORD*)(nativeState+0x24))return false;return ChatImeActive();}

// samp.dll RVA: 0x0011c304
const float g_dialogNegativeLineScale=-1.5f;
// samp.dll RVA: 0x00118490
const float g_dialogLineScale=1.5f;

// samp.dll RVA: 0x0006f0f0
RECT *CDialog::GetRect(RECT *rect) {
    int left=x;rect->left=left;rect->right=width+left;
    int top=y;rect->top=top;rect->bottom=height+top;
    return rect;
}

// samp.dll RVA: 0x0006f120
int CDialog::GetTextWidth(const char *value) {
    ID3DXFont *font=dialog->GetFont(1)->pFont;
    RECT rect={0,0,0,0};
    char buffer[129];
    if(value && strlen(value) && font) {
        strcpy(buffer,value);
        ChatStripColorTags(buffer);
        font->DrawTextA(0,buffer,-1,&rect,0x540,0xffffffff);
        return rect.right-rect.left;
    }
    return -1;
}

// samp.dll RVA: 0x0006f1e0
int CDialog::GetFontHeight() { return dialog->GetFont(1)->nHeight; }

// Descriptive recovery name; the delimiter is returned without consumption.
// samp.dll RVA: 0x0006f200
const char *CDialog::CopyColumn(const char *value,char *output,int limit) {
    while(*value=='\t')++value;
    int copied=0;
    while(*value && copied<limit && *value!='\n' && *value!='\t') {
        output[copied]=*value;++copied;++value;
    }
    output[copied]=0;
    if(!*value || *value=='\n')return 0;
    return value;
}

// Descriptive recovery name; a newline is returned without consumption.
// samp.dll RVA: 0x0006f260
const char *CDialog::CopyLine(const char *value,char *output,int limit) {
    int copied=0;
    while(*value && copied<limit && *value!='\n') {
        output[copied]=*value;++copied;++value;
    }
    output[copied]=0;
    return *value?value:0;
}

// samp.dll RVA: 0x0006f2a0
void CDialog::Hide() {
    g_pGame->SetCursorMode(0,FALSE);
    dialog->SetVisible(false);
    active=FALSE;
}

// samp.dll RVA: 0x0006ef40
void CDialog::Recalc() {
    if(width<buttonWidth*2+30)width=buttonWidth*2+30;
    dialog->SetSize(width,height);
    RECT client;GetClientRect(g_inputGameWindow,&client);
    x=client.right/2-width/2;y=client.bottom/2-height/2;
    dialog->SetLocation(x,y);
    int contentHeight=height-dialog->GetCaptionHeight();
    CDXUTControl *button;
    if(dialog->GetControl(21,0)->GetVisible()) {
        button=dialog->GetControl(20,0);
        button->SetLocation(width/2-buttonWidth-10,contentHeight-buttonHeight-5);
        button=dialog->GetControl(21,0);
        button->SetLocation(width/2+10,contentHeight-buttonHeight-5);
    } else {
        button=dialog->GetControl(20,0);
        button->SetLocation(width/2-buttonWidth/2,contentHeight-buttonHeight-5);
    }
    int editHeight=14-(int)(dialog->GetFont(0)->nHeight*g_dialogNegativeLineScale);
    input->SetSize(width-10,editHeight);
    int editFontOffset=(int)(dialog->GetFont(0)->nHeight*g_dialogLineScale);
    input->SetLocation(5,contentHeight-editFontOffset-buttonHeight-24);
}

// samp.dll RVA: 0x0006f2d0
void CDialog::Draw() {
    if(!active || g_pInput->m_bEnabled || *(DWORD*)g_deviceScoreboard->nativeState)return;
    g_pGame->SetCursorMode(2,FALSE);
    if(!dialog)return;
    dialog->OnRender(10.0f);
    RECT rect;GetRect(&rect);
    if(text && (type==1 || type==0 || type==3)) {
        rect.left+=20;rect.top+=dialog->GetCaptionHeight()+5;
        g_deviceFonts->DrawLittleText(0,text,rect,0x140,0xffa9c4e4,TRUE);
        return;
    }
    if(type==5) {
        rect.top+=dialog->GetCaptionHeight()+3;
        int left=rect.left+16;
        for(int i=0;i<columnCount;++i) {
            rect.left=left;
            g_deviceFonts->DrawLittleText(0,columnTitles[i],rect,0x120,0xffa9c4e4,TRUE);
            left+=columnWidths[i];
        }
    }
}
