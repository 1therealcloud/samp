#include "client/device_setup.h"
#include "client/frame.h"
#include "main.h"
#include "game/input.h"
#include "cmdwindow.h"
#include "chatwindow.h"
#include "gui/device_lifecycle.h"
#include "gui/render_dependencies.h"
#include "gui/gui.h"
#include <DXUT.h>
#include <DXUTgui.h>
// samp.dll RVA: 0x0016be84
DWORD g_clientInputOutsideClick;
// samp.dll RVA: 0x0016be88
DWORD g_clientScoreboardOutsideClick;
// samp.dll RVA: 0x000610d0
LRESULT CALLBACK ClientWindowProcedure(HWND window,UINT message,WPARAM wparam,LPARAM lparam) {
    if(g_clientIntegrityFailures>10)return 0;
    if(message==WM_ACTIVATEAPP&&wparam==1&&g_pGame&&g_gameD3DDevice&&!g_pGame->IsMenuVisible()&&!g_pGame->m_nCursorMode)g_gameD3DDevice->ShowCursor(FALSE);
    if(g_pInput)g_pInput->MsgProc(message,wparam,lparam);
    if(g_renderDialog)g_renderDialog->MsgProc(message,wparam,lparam);
    if(g_renderFirstDialog&&g_pInput&&g_pInput->m_bEnabled&&!g_pGame->IsMenuVisible()) {
        if(g_renderFirstDialog->MsgProc(window,message,wparam,lparam))return 0;
        if((message==WM_KEYUP||message==WM_KEYDOWN)&&wparam==VK_RETURN)return 0;
        if(message==WM_CHAR&&wparam==10)return 0;
    } else if(g_deviceScoreboard&&g_guiScoreboardDialog) {
        if(!g_pGame->IsMenuVisible()&&message==WM_KEYUP&&wparam==VK_TAB) {
            if(!*(DWORD*)g_deviceScoreboard){if(!*(DWORD*)(g_renderPanel->nativeState+0x24))g_deviceScoreboard->Enable();}
            else g_deviceScoreboard->Close(true);
        }
        if(*(DWORD*)g_deviceScoreboard&&!g_pGame->IsMenuVisible())g_guiScoreboardDialog->MsgProc(window,message,wparam,lparam);
    }
    if(g_renderSecondDialog&&*((BYTE*)g_renderSecondDialog+0x13)&&!g_pGame->IsMenuVisible())g_renderSecondDialog->MsgProc(window,message,wparam,lparam);
    if(g_renderDialog&&g_guiServerDialog&&*(DWORD*)(g_renderDialog->nativeState+0x28)&&!*(DWORD*)g_deviceScoreboard&&!g_pInput->m_bEnabled&&!g_pGame->IsMenuVisible()&&g_guiServerDialog->MsgProc(window,message,wparam,lparam)) {
        if(message==WM_CHAR&&wparam==VK_ESCAPE&&!g_renderDialog->IsInputHidden())g_renderDialog->Close(0);
        return 0;
    }
    if(g_deviceObjectEdit) {
        if(((CObjectEdit*)g_deviceObjectEdit)->MsgProc(message,wparam,lparam))return 0;
        if(*(DWORD*)(g_deviceObjectEdit+0x80)&&message==WM_CHAR&&wparam==VK_ESCAPE){((CObjectEdit*)g_deviceObjectEdit)->Disable(FALSE);return 0;}
    }
    if(g_renderObjectSelection) {
        if(g_renderObjectSelection->MsgProc(message,wparam,lparam))return 0;
        if(g_renderObjectSelection->enabled&&message==WM_CHAR&&wparam==VK_ESCAPE){g_renderObjectSelection->Enable(FALSE);return 0;}
    }
    if(g_renderTextDrawSelection) {
        if(g_renderTextDrawSelection->MsgProc(message,wparam,lparam))return 0;
        if(g_renderTextDrawSelection->enabled&&message==WM_CHAR&&wparam==VK_ESCAPE){g_renderTextDrawSelection->Disable();return 0;}
    }
    if(g_renderPanel&&g_guiPanelDialog&&*(DWORD*)(g_renderPanel->nativeState+0x24)&&!*(DWORD*)g_deviceScoreboard&&!g_pInput->m_bEnabled&&!g_pGame->IsMenuVisible()&&g_guiPanelDialog->MsgProc(window,message,wparam,lparam))return 0;
    // Native snapshots these five pointers before its message switch.
    CInput *input=g_pInput;CScoreboard *scoreboard=g_deviceScoreboard;BYTE *edit=g_deviceObjectEdit;
    CObjectSelection *selection=g_renderObjectSelection;CTextDrawSelection *text=g_renderTextDrawSelection;
    if(message<=WM_CHAR) {
        switch(message) {
        case WM_KILLFOCUS:
            if(scoreboard&&*(DWORD*)scoreboard)scoreboard->Close(true);
            if(g_pInput&&g_pInput->m_bEnabled)g_pInput->Close();
            if(g_renderSecondDialog&&*((BYTE*)g_renderSecondDialog+0x13))g_pGame->SetCursorMode(0,TRUE);
            g_pGame->ProcessInputEnabling();break;
        case WM_SETCURSOR:
            if(!g_deviceResetMarker){SetCursor(0);g_deviceResetMarker=1;}return 1;
        case WM_KEYDOWN:
            if(wparam==VK_ESCAPE&&((input&&input->m_bEnabled)||(scoreboard&&*(DWORD*)scoreboard)||(g_renderDialog&&*(DWORD*)(g_renderDialog->nativeState+0x28))||(g_renderPanel&&*(DWORD*)(g_renderPanel->nativeState+0x24))||(g_renderSecondDialog&&*((BYTE*)g_renderSecondDialog+0x13))||(edit&&*(DWORD*)(edit+0x80))||(selection&&selection->enabled)||(text&&text->enabled)))return 0;
            break;
        // samp.dll RVA: 0x00060de0
        case WM_KEYUP:ClientHandleKeyRelease(wparam);break;
        // samp.dll RVA: 0x00061010
        case WM_CHAR:ClientHandleCharacter(wparam);break;
        }
    } else if(message<=WM_LBUTTONUP) {
        if(message==WM_LBUTTONUP){
            if(g_clientInputOutsideClick){if(input&&input->m_bEnabled)input->Close();g_clientInputOutsideClick=0;return 0;}
            if(g_clientScoreboardOutsideClick){if(scoreboard&&*(DWORD*)scoreboard)scoreboard->Close(true);g_clientScoreboardOutsideClick=0;return 0;}
        } else if(message==WM_SYSKEYUP){if(wparam==VK_RETURN)ClientToggleWindowedMode();}
        else if(message==WM_LBUTTONDOWN){
            int x=(short)LOWORD(lparam),y=(short)HIWORD(lparam);RECT rect;
            if(scoreboard&&*(DWORD*)scoreboard){scoreboard->GetRect(&rect);if(x<rect.left||x>rect.right||y<rect.top||y>rect.bottom){g_clientScoreboardOutsideClick=1;return 0;}}
            else if(input&&input->m_bEnabled&&!*(DWORD*)(edit+0x80)&&!selection->enabled){InputRect inputRect;input->GetRect(&inputRect);rect=inputRect;if(x<rect.left||x>rect.right||y<rect.top||y>rect.bottom){g_clientInputOutsideClick=1;return 0;}}
        }
    } else if(message==WM_MOUSEWHEEL&&g_pInput->m_bEnabled&&g_pChat->m_nMode){g_pChat->Scroll((short)HIWORD(wparam)/120);return 0;}
    return CallWindowProcA(g_clientPreviousWindowProcedure,window,message,wparam,lparam);
}
