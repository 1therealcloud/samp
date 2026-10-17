#include "gui/input_native.h"
// samp.dll RVA: 0x00068e10
CInput::CInput(IDirect3DDevice9 *device) {
    m_bEnabled=FALSE;
    m_pDevice=device;
    m_nCommandCount=0; m_nTotalRecall=0;
    m_pEditbox=0; m_nCurrentRecall=-1;
    memset(m_szRecallBuffer,0,sizeof(m_szRecallBuffer));
    memset(m_szInput,0,sizeof(m_szInput));
    memset(m_szCurrentBuffer,0,sizeof(m_szCurrentBuffer));
}
// samp.dll RVA: 0x00068e80
InputRect *CInput::GetRect(InputRect *rect) {
    memset(rect,0,sizeof(*rect));
    if(m_pGameUi) { rect->right=m_pGameUi->GetWidth();rect->bottom=m_pGameUi->GetHeight(); }
    return rect;
}
// samp.dll RVA: 0x00068ec0
void CInput::Open() {
    if(!m_bEnabled) {
        if(m_pEditbox) {
            RECT rect; GetClientRect(g_inputGameWindow,&rect);
            int width=(int)(rect.right*0.6f);
            if(width>800) width=800;
            m_pEditbox->SetEnabled(true);m_pEditbox->SetVisible(true);
            m_pEditbox->SetLocation(40,g_pChat->m_nWindowBottom);
            m_pEditbox->SetSize(width,14-(int)(m_pGameUi->GetFont(0)->nHeight*-1.5f));
            m_pGameUi->RequestFocus(m_pEditbox);m_pEditbox->OnFocusIn();
            int bottom=g_pChat->m_nWindowBottom;
            m_pGameUi->SetSize(width+100,bottom+50);
        }
        m_bEnabled=TRUE;
    }
}
// samp.dll RVA: 0x00069010
void CInput::AddRecall(const char *text) {
    for(int i=8;i>=0;--i) strcpy(m_szRecallBuffer[i+1],m_szRecallBuffer[i]);
    strcpy(m_szRecallBuffer[0],text);
    if(m_nTotalRecall<10) ++m_nTotalRecall;
}
// samp.dll RVA: 0x00069070
void CInput::RecallUp() {
    if(m_nCurrentRecall<m_nTotalRecall-1) {
        if(m_nCurrentRecall==-1) { strncpy(m_szCurrentBuffer,m_pEditbox->GetText(),128);m_szCurrentBuffer[128]=0; }
        ++m_nCurrentRecall;
        m_pEditbox->SetText(m_szRecallBuffer[m_nCurrentRecall],false);
    }
}
// samp.dll RVA: 0x000690e0
void CInput::RecallDown() {
    --m_nCurrentRecall;
    if(m_nCurrentRecall>=0) { m_pEditbox->SetText(m_szRecallBuffer[m_nCurrentRecall],false);return; }
    if(m_nCurrentRecall==-1) m_pEditbox->SetText(m_szCurrentBuffer,false);
    m_nCurrentRecall=-1;
}
// samp.dll RVA: 0x00069130
void CInput::EnableCursor() { if(m_bEnabled) g_pGame->SetCursorMode(2,FALSE); }
// samp.dll RVA: 0x00069150
InputCommand CInput::GetCommandHandler(const char *name) {
    for(int i=0;i!=m_nCommandCount;++i) if(!_stricmp(name,m_szCommandName[i])) return m_pCommandProc[i];
    return 0;
}
// samp.dll RVA: 0x000691a0
void CInput::SetDefaultCommand(InputCommand command) { m_pDefaultCommand=command; }
// samp.dll RVA: 0x000691b0
void CInput::AddCommand(const char *name,InputCommand command) {
    if(m_nCommandCount<144 && strlen(name)<32) {
        m_pCommandProc[m_nCommandCount]=command;
        strcpy(m_szCommandName[m_nCommandCount],name);
        ++m_nCommandCount;
    }
}
// samp.dll RVA: 0x00069210
BOOL CInput::MsgProc(int message,int wparam,int lparam) {
    if(m_bEnabled && m_pEditbox && CDXUTIMEEditBox::StaticMsgProc(message,wparam,lparam)) return TRUE;
    return FALSE;
}
// samp.dll RVA: 0x00069280
void CInput::ResetDialogControls(CDXUTDialog *dialog) {
    m_pGameUi=dialog;
    if(dialog) {
        dialog->AddIMEEditBox(1,"",10,175,570,40,true,(CDXUTIMEEditBox **)&m_pEditbox);
        if(g_pConfig->GetIntValue("ime")) { InputEnableIme(true);InputResetIme(); }
        m_pEditbox->m_Elements[0]->TextureColor.Init(0xf0050505,0xc8808080,0);
        m_pEditbox->SetTextColor(0xffffffff);
        ((InputEditColors*)m_pEditbox)->caret=0xff969696;
        ((InputEditColors*)m_pEditbox)->selectedBackground=0xffb92228;
        ((InputEditColors*)m_pEditbox)->selectedText=0xff0a0a0f;
        m_pEditbox->SetEnabled(false);m_pEditbox->SetVisible(false);
    }
}

// samp.dll RVA: 0x00068fc0
void CInput::Close() {
    if(m_bEnabled) {
        if(m_pEditbox) { m_pEditbox->OnFocusOut();m_pEditbox->SetEnabled(false);m_pEditbox->SetVisible(false); }
        g_pGame->SetCursorMode(0,TRUE);
        m_bEnabled=FALSE;
    }
}

