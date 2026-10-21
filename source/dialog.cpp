#include "dialog.h"
#include "gui/chat_dependencies.h"
#include "gui/device_lifecycle.h"
#include "gui/render_dependencies.h"
#include "game/game.h"
#include "main.h"
#include "cmdwindow.h"
#include "d3d9/common/dxut_listbox_native.h"
#include "game/hooks_render.h"
#include "game/scripting.h"
#include <DXUT.h>
#include <DXUTgui.h>
#include <string.h>
#include <stdlib.h>
#include "BitStream.h"
#include "RakClientInterface.h"

// Reconstruction-only access facade for the established control vtable slot4C.
// It adds no fields, virtual methods or standalone native entries.
struct DialogControlAccess : CDXUTControl { using CDXUTControl::UpdateRects; };

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

// samp.dll RVA: 0x0006f630
BOOL CDialog::PopulateList(const char *value,SIZE *size) {
    list->RemoveAllItems();list->columnCount=0;
    int widest=0,totalHeight=0,row=0;
    while(value && *value) {
        char line[256]={0};
        value=CopyLine(value,line,256);
        if(strlen(line)) {
            int measured=GetTextWidth(line);
            if(measured>widest)widest=measured;
            totalHeight+=GetFontHeight();
            list->AddItem(line,(void*)row,-1);
            ++row;
        }
        if(!value)break;
        if(*value=='\n')++value;
    }
    if(size){size->cx=widest+20;size->cy=totalHeight;}
    return TRUE;
}

// samp.dll RVA: 0x0006f730
int CDialog::PopulateTable(const char *value,SIZE *size,BOOL header) {
    list->RemoveAllItems();list->columnCount=0;
    // Observed adjacency: the native extra-column width access aliases the
    // first DWORD of the129-byte cell buffer. No broader stack layout is claimed.
    struct TableScratch {
        int widths[4];
        char cell[129];
        BYTE padding[7];
        char line[256];
    } scratch;
    typedef char TableCellAlias[(offsetof(TableScratch,cell)==16)?1:-1];
    typedef char TableLineOffset[(offsetof(TableScratch,line)==0x98)?1:-1];
    memset(scratch.widths,0,sizeof(scratch.widths));
    memset(scratch.line,0,sizeof(scratch.line));
    int totalHeight=0;
    const char *cursor=value;
    const char *next=CopyLine(value,scratch.line,256);
    if((BYTE)header)cursor=next;
    if(!strlen(scratch.line))return -1;
    int columns=0;
    const char *column=scratch.line;
    do {
        if(columns>=4)break;
        memset(scratch.cell,0,sizeof(scratch.cell));
        column=CopyColumn(column,scratch.cell,128);
        memset(columnTitles[columns],0,129);
        if((BYTE)header) {
            strncpy(columnTitles[columns],scratch.cell,128);
            int measured=GetTextWidth(scratch.cell);
            if(measured>scratch.widths[columns])scratch.widths[columns]=measured;
        }
        ++columns;
    } while(column);
    columnCount=columns;list->columnCount=columns-1;
    int row=0;
    while(cursor && *cursor) {
        memset(scratch.line,0,sizeof(scratch.line));
        cursor=CopyLine(cursor,scratch.line,256);
        bool added=false;
        if(strlen(scratch.line)) {
            int col=0;column=scratch.line;
            do {
                if(col>columnCount)break;
                memset(scratch.cell,0,sizeof(scratch.cell));
                column=CopyColumn(column,scratch.cell,128);
                int measured=GetTextWidth(scratch.cell);
                int *width=(int*)((BYTE*)&scratch+col*4);
                if(measured>*width)*width=measured;
                if(col==0) {
                    list->AddItem(scratch.cell,(void*)row,-1);
                    added=true;totalHeight+=GetFontHeight();
                } else list->SetSubItemText(row,col-1,scratch.cell);
                ++col;
            } while(column);
        }
        if(added)++row;
        if(!cursor)break;
        if(*cursor=='\n')++cursor;
    }
    list->columnWidths[0]=scratch.widths[0]+20;
    columnWidths[0]=scratch.widths[0]+20;
    list->columnWidths[1]=scratch.widths[1]+20;
    columnWidths[1]=scratch.widths[1]+20;
    list->columnWidths[2]=scratch.widths[2]+20;
    columnWidths[2]=scratch.widths[2]+20;
    if(size){size->cx=scratch.widths[0]+scratch.widths[1]+scratch.widths[2]+scratch.widths[3]+80;size->cy=totalHeight;}
    return 1;
}

// samp.dll RVA: 0x0011c724
const float g_dialogEditPadding=14.0f;

// samp.dll RVA: 0x0006fa50
void CDialog::Show(int requestId,int style,const char *caption,const char *value,const char *firstButton,const char *secondButton,BOOL remote) {
    if(requestId<0){if(active)Hide();return;}
    if(g_pInput && g_pInput->m_bEnabled)g_pInput->Close();
    id=requestId;type=style;serverSide=remote;
    memset(title,0,sizeof(title));strncpy(title,caption,64);
    if(text)free(text);
    text=(char*)calloc(1,strlen(value)+64);
    strcpy(text,value);
    // Native hidden-result scratch; do not invoke the reconstruction FontSize ctor.
    SIZE bodySize;
    FontSize *measured=g_deviceFonts->GetLittleTextScreenSize((FontSize*)&bodySize,text,0x40);
    textWidth=measured->cx;textHeight=measured->cy;
    if(strlen(caption)) {
        SIZE titleSize;g_deviceFonts->GetTextScreenSize((FontSize*)&titleSize,g_fontMeasureProbe,0);
        // Native copies the original, untruncated caption and leaves stale caption
        // state untouched when the input caption is empty.
        strcpy((char*)dialog+0x16,caption);
        *(BYTE*)((BYTE*)dialog+0x14)=1;
        dialog->SetCaptionHeight(titleSize.cy+4);
    }
    SIZE size={0,0};
    switch(type) {
    case 0:
        width=textWidth+40;
        height=dialog->GetCaptionHeight()+textHeight+buttonHeight+25;
        input->SetVisible(false);input->SetEnabled(false);
        ((CDXUTControl*)list)->SetVisible(false);((CDXUTControl*)list)->SetEnabled(false);
        break;
    case 1:
    case 3: {
        width=textWidth+40;
        bool password=type==3;
        int captionHeight=dialog->GetCaptionHeight();
        int editHeight=(int)(dialog->GetFont(1)->nHeight*g_hookHudAspectThreshold+g_dialogEditPadding);
        height=editHeight+textHeight+buttonHeight+captionHeight+25;
        input->SetVisible(true);input->SetEnabled(true);
        BYTE *flags=(BYTE*)input;
        flags[0x126]=password;flags[0x5f]=password;
        input->SetText(g_pedEmptyDanceAnimation,false);
        ((CDXUTControl*)list)->SetVisible(false);((CDXUTControl*)list)->SetEnabled(false);
        break;
    }
    case 2: {
        PopulateList(text,&size);
        width=size.cx+40;
        if(width<400)width=400;
        if(width>800)width=800;
        if(width<buttonWidth*2+30)width=buttonWidth*2+30;
        int listHeight=size.cy+list->border*2;
        if(listHeight<200)listHeight=200;else if(listHeight>400)listHeight=400;
        height=dialog->GetCaptionHeight()+buttonHeight+listHeight+15;
        ((CDXUTControl*)list)->SetLocation(5,5);
        ((CDXUTControl*)list)->SetSize(width-10,listHeight);
        input->SetVisible(false);input->SetEnabled(false);
        ((CDXUTControl*)list)->SetVisible(true);((CDXUTControl*)list)->SetEnabled(true);
        ((DialogControlAccess*)list)->UpdateRects();
        break;
    }
    case 4:
    case 5: {
        bool header=type==5;
        PopulateTable(text,&size,header);
        width=size.cx+40;
        if(width<400) {
            width=400;
            int columns=list->columnCount;
            if(columns>0) {
                int extra=(360-size.cx)/columns;
                int i=0;
                do {
                    int own=columnWidths[i];
                    int widget=list->columnWidths[i];
                    columnWidths[i]=own+extra;
                    list->columnWidths[i]=widget+extra;
                    ++i;
                } while(i!=list->columnCount);
            }
        }
        if(width<buttonWidth*2+30)width=buttonWidth*2+30;
        int listHeight=size.cy+list->border*2;
        if(listHeight<200)listHeight=200;else if(listHeight>400)listHeight=400;
        if(header) {
            SIZE headerSize;g_deviceFonts->GetLittleTextScreenSize((FontSize*)&headerSize,g_fontMeasureProbe,0);
            height=dialog->GetCaptionHeight()+buttonHeight+headerSize.cy+listHeight+15;
            ((CDXUTControl*)list)->SetLocation(5,headerSize.cy+5);
        } else {
            height=dialog->GetCaptionHeight()+buttonHeight+listHeight+15;
            ((CDXUTControl*)list)->SetLocation(5,5);
        }
        ((CDXUTControl*)list)->SetSize(width-10,listHeight);
        input->SetVisible(false);input->SetEnabled(false);
        ((CDXUTControl*)list)->SetVisible(true);((CDXUTControl*)list)->SetEnabled(true);
        ((DialogControlAccess*)list)->UpdateRects();
        break;
    }
    }
    bool second=strlen(secondButton)!=0;
    ((CDXUTStaticDL*)dialog->GetControl(20,0))->SetText(firstButton);
    ((CDXUTStaticDL*)dialog->GetControl(21,0))->SetText(second?secondButton:g_pedEmptyDanceAnimation);
    dialog->GetControl(21,0)->SetVisible(second);
    dialog->SetVisible(true);
    if(type==1 || type==3)dialog->RequestFocus(input);
    if(type==2 || type==4 || type==5) {
        dialog->RequestFocus((CDXUTControl*)list);
        list->SelectItem(0);
    }
    Recalc();active=TRUE;
}

// samp.dll RVA: 0x0011c348
int g_dialogResponseRpcId=62;

// samp.dll RVA: 0x000700d0
void CDialog::Close(char response) {
    if(!active)return;
    WORD dialogId=(WORD)id;
    char reply[129]={0};
    int selection=-1;
    BYTE length=0;
    if(type==1 || type==3) {
        strncpy(reply,((CDXUTEditBoxDL*)input)->GetText(),128);
        length=(BYTE)strlen(reply);
    } else if(type==2 || type==4 || type==5) {
        selection=list->GetSelectedIndex(-1);
        CDXUTListBoxDL *selectedList=list;
        DXUTListBoxItemDL *item=selectedList->GetItem(selectedList->GetSelectedIndex(-1));
        if(item)strncpy(reply,item->text,128);
        ChatStripColorTags(reply);
        length=(BYTE)strlen(reply);
    }
    if(serverSide) {
        RakNet::BitStream stream;
        stream.WriteBits((const unsigned char*)&dialogId,16,true);
        BYTE button=(BYTE)response;
        stream.WriteBits(&button,8,true);
        stream.WriteBits((const unsigned char*)&selection,16,true);
        stream.WriteBits(&length,8,true);
        if(length)stream.Write(reply,length);
        (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(&g_dialogResponseRpcId,&stream,HIGH_PRIORITY,RELIABLE_ORDERED,0,false);
    }
    Hide();
}

typedef char DialogRpcEnums[(HIGH_PRIORITY==1 && RELIABLE_ORDERED==9)?1:-1];
