#include "gui/render_dependencies.h"
#include "gui/device_lifecycle.h"
#include "gui/input_activity.h"
#include "main.h"
#include "cmdwindow.h"
#include <DXUT.h>
#include <DXUTgui.h>
// samp.dll RVA: 0x0006e3c0
void CScoreboard::GetRect(RECT *rect) {
    int left=(int)*(float*)(nativeState+8);rect->left=left;
    rect->right=left+(int)*(float*)(nativeState+0x14);
    int top=(int)*(float*)(nativeState+12);rect->top=top;
    rect->bottom=top+(int)*(float*)(nativeState+0x18);
}
// samp.dll RVA: 0x0006e410
void CScoreboard::Close(bool restoreCursor) {
    if(*(DWORD*)nativeState&&*(CDXUTDialog**)(nativeState+0x34)) {
        (*(CDXUTDialog**)(nativeState+0x34))->SetVisible(false);
        (*(CDXUTListBox**)(nativeState+0x38))->SetEnabled(false);
        (*(CDXUTListBox**)(nativeState+0x38))->SetVisible(false);
        if(restoreCursor)g_pGame->SetCursorMode(0,FALSE);
        *(DWORD*)nativeState=0;
    }
}
// samp.dll RVA: 0x0006ee10
void CScoreboard::Enable() {
    if(!*(DWORD*)nativeState&&*(CDXUTDialog**)(nativeState+0x34)) {
        (*(CDXUTDialog**)(nativeState+0x34))->SetVisible(true);
        (*(CDXUTListBox**)(nativeState+0x38))->SetEnabled(true);
        (*(CDXUTListBox**)(nativeState+0x38))->UpdateRects();
        (*(CDXUTListBox**)(nativeState+0x38))->SetVisible(true);
        UpdateList();g_pGame->SetCursorMode(3,FALSE);*(DWORD*)nativeState=1;
    }
}
// samp.dll RVA: 0x0006ef10
bool CDialog::MsgProc(UINT message,WPARAM wparam,LPARAM lparam) {return CDXUTIMEEditBox::StaticMsgProc(message,wparam,lparam)!=false;}
// samp.dll RVA: 0x0006ef30
bool CDialog::IsInputHidden() {if(!*(DWORD*)(nativeState+0x24))return false;return ChatImeActive();}
// samp.dll RVA: 0x0006d880
BOOL CObjectSelection::MsgProc(UINT message,WPARAM,LPARAM) {if(enabled&&message==WM_LBUTTONUP){SendNotification();return TRUE;}return FALSE;}
// samp.dll RVA: 0x00071010
BOOL CTextDrawSelection::MsgProc(UINT message,WPARAM,LPARAM) {if(enabled&&message==WM_LBUTTONUP){if(unknown08!=0xffff)SendNotification();return TRUE;}return FALSE;}
// samp.dll RVA: 0x0006d5c0
void CObjectSelection::Enable(BOOL on) {
    if(enabled){if(!on){g_pGame->SetCursorMode(0,TRUE);if(g_pInput->m_bEnabled){g_pInput->Close();g_pInput->Open();}}if(enabled){enabled=on;return;}}
    if(on){g_pGame->SetCursorMode(0,TRUE);g_pGame->m_nInputEnableWaitFrames=0;g_pGame->ProcessInputEnabling();}
    enabled=on;
}
// samp.dll RVA: 0x00070fc0
void CTextDrawSelection::Disable() {
    enabled=0;g_pGame->SetCursorMode(0,FALSE);
    if(g_pInput->m_bEnabled){g_pInput->Close();g_pInput->Open();}
    unknown08=0xffff;SendNotification();ResetTextDraws();
}
// samp.dll RVA: 0x00072660
void CObjectEdit::Disable(BOOL send) {
    BOOL notify=send!=0;unknown80=0;g_pGame->SetCursorMode(0,TRUE);
    if(g_pInput->m_bEnabled){g_pInput->Close();g_pInput->Open();}
    if(unknown78==2)SendAttachedEditEndNotification(notify);
    if(unknown78==1)SendEditEndNotification(notify);
}
