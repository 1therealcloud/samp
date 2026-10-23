#include "gui/gui.h"
#include "gui/device_lifecycle.h"
#include "gui/render_dependencies.h"
#include "chatwindow.h"
#include "cmdwindow.h"
#include "game/game.h"
#include <DXUT.h>
#include <DXUTgui.h>
// samp.dll RVA: 0x002acab8
CDXUTDialog *g_guiScoreboardDialog;
// samp.dll RVA: 0x002acac0
CDXUTDialog *g_guiServerDialog;
// samp.dll RVA: 0x002acac4
CDXUTDialog *g_guiPanelDialog;
// samp.dll RVA: 0x002acac8
CDXUTDialog *g_guiDownloadDialog;
typedef char NativeDialogExtent[(sizeof(CDXUTDialog)==0x1ea)?1:-1];
// samp.dll RVA: 0x000c6ce0
void ClientInitializeGui(){
    if(g_guiScoreboardDialog){delete g_guiScoreboardDialog;g_guiScoreboardDialog=0;}
    if(g_renderFirstDialog){delete g_renderFirstDialog;g_renderFirstDialog=0;}
    if(g_deviceCursorSurface){g_deviceCursorSurface->Release();g_deviceCursorSurface=0;}
    if(g_renderSecondDialog){delete g_renderSecondDialog;g_renderSecondDialog=0;}
    if(g_guiServerDialog){delete g_guiServerDialog;g_guiServerDialog=0;}
    if(g_guiPanelDialog){delete g_guiPanelDialog;g_guiPanelDialog=0;}
    if(g_guiDownloadDialog){delete g_guiDownloadDialog;g_guiDownloadDialog=0;}
    g_guiScoreboardDialog=new CDXUTDialog;
    g_guiScoreboardDialog->Init(g_deviceDialogResources);
    g_guiScoreboardDialog->SetCallback(ClientGuiScoreboardEvent,0);
    g_guiScoreboardDialog->SetSize(640,480);g_guiScoreboardDialog->SetLocation(0,0);
    g_guiScoreboardDialog->SetBackgroundColors(0x960a0a0a,0x960a0a0a,0x960a0a0a,0x960a0a0a);
    g_guiScoreboardDialog->EnableMouseInput(true);g_guiScoreboardDialog->EnableKeyboardInput(true);g_guiScoreboardDialog->SetVisible(false);
    if(g_deviceScoreboard)g_deviceScoreboard->ResetDialogControls(g_guiScoreboardDialog);
    g_renderFirstDialog=new CDXUTDialog;
    g_renderFirstDialog->Init(g_deviceDialogResources);g_renderFirstDialog->SetCallback(ClientGuiInputEvent,0);
    g_renderFirstDialog->SetLocation(0,0);g_renderFirstDialog->SetSize(*(int*)0xc17044,*(int*)0xc17048);
    g_renderFirstDialog->EnableMouseInput(true);g_renderFirstDialog->EnableKeyboardInput(true);
    g_renderSecondDialog=new CDXUTDialog;
    g_renderSecondDialog->Init(g_deviceDialogResources);g_renderSecondDialog->SetCallback(ClientGuiSpawnEvent,0);
    g_renderSecondDialog->SetLocation(0,0);g_renderSecondDialog->SetSize(310,40);
    g_renderSecondDialog->SetBackgroundColors(0x960a0a0a,0x960a0a0a,0x960a0a0a,0x960a0a0a);
    g_renderSecondDialog->EnableMouseInput(true);g_renderSecondDialog->EnableKeyboardInput(false);g_renderSecondDialog->SetVisible(false);
    // Selected SDK widgets expose a wide-text API; these native labels are ASCII.
    g_renderSecondDialog->AddButton(6,L"<<",10,5,90,30,0,false,0);
    g_renderSecondDialog->AddButton(5,L">>",110,5,90,30,0,false,0);
    g_renderSecondDialog->AddButton(4,L"Spawn",210,5,90,30,0,false,0);
    if(g_pChat)g_pChat->ResetDialogControls(g_renderFirstDialog);
    if(g_pInput)g_pInput->ResetDialogControls(g_renderFirstDialog);
    g_guiServerDialog=new CDXUTDialog;
    g_guiServerDialog->Init(g_deviceDialogResources);g_guiServerDialog->SetCallback(ClientGuiDialogEvent,0);
    g_guiServerDialog->SetLocation(0,0);g_guiServerDialog->SetSize(600,300);
    g_guiServerDialog->EnableMouseInput(true);g_guiServerDialog->EnableKeyboardInput(true);
    g_guiServerDialog->SetBackgroundColors(0xdc050505,0xdc050505,0xdc050505,0xdc050505);g_guiServerDialog->SetVisible(false);
    if(g_renderDialog)g_renderDialog->ResetDialogControls(g_guiServerDialog);
    g_guiPanelDialog=new CDXUTDialog;
    g_guiPanelDialog->Init(g_deviceDialogResources);g_guiPanelDialog->SetCallback(ClientGuiPanelEvent,0);
    g_guiPanelDialog->SetLocation(0,0);g_guiPanelDialog->SetSize(500,160);
    g_guiPanelDialog->EnableMouseInput(true);g_guiPanelDialog->EnableKeyboardInput(true);
    g_guiPanelDialog->SetBackgroundColors(0xdc050505,0xdc050505,0xdc050505,0xdc050505);g_guiPanelDialog->SetVisible(false);
    if(g_renderPanel)g_renderPanel->ResetDialogControls(g_guiPanelDialog);
    g_guiDownloadDialog=new CDXUTDialog;
    g_guiDownloadDialog->Init(g_deviceDialogResources);g_guiDownloadDialog->SetCallback(ClientGuiDownloadEvent,0);
    g_guiDownloadDialog->SetLocation(0,0);g_guiDownloadDialog->SetSize(500,160);
    g_guiDownloadDialog->EnableMouseInput(true);g_guiDownloadDialog->EnableKeyboardInput(false);
    g_guiDownloadDialog->SetBackgroundColors(0xdc050505,0xdc050505,0xdc050505,0xdc050505);g_guiDownloadDialog->SetVisible(false);
    if(g_renderDownloadWindow)g_renderDownloadWindow->ResetDialogControls(g_guiDownloadDialog);
    if(!g_deviceCursorSurface){
        g_gameD3DDevice->CreateOffscreenPlainSurface(32,32,D3DFMT_A8R8G8B8,D3DPOOL_SYSTEMMEM,&g_deviceCursorSurface,0);
        D3DXLoadSurfaceFromFileA(g_deviceCursorSurface,0,0,"mouse.png",0,D3DX_FILTER_NONE,0,0);
        if(g_deviceCursorSurface)g_gameD3DDevice->SetCursorProperties(0,0,g_deviceCursorSurface);
    }
}

#include "gui/render_dependencies.h"
#include "gui/device_objects.h"
#include "game/game.h"
#include "chatwindow.h"
#include <GetTime.h>
#include <string.h>
// Constructor-only opaque storage is intentional: the full UI interfaces remain open.
typedef char CDeathWindowObservedExtent[(sizeof(CDeathWindow)==0x157)?1:-1];
typedef char CScoreboardObservedExtent[(sizeof(CScoreboard)==0x44)?1:-1];
typedef char CDialogObservedExtent[(sizeof(CDialog)==0x29d)?1:-1];
typedef char CRenderPanelObservedExtent[(sizeof(CRenderPanel)==0x28)?1:-1];
typedef char CDownloadWindowObservedExtent[(sizeof(CDownloadWindow)==0x260)?1:-1];
typedef char CNetStatsObservedExtent[(sizeof(CNetStats)==0x18)?1:-1];
typedef char CSrvNetStatsObservedExtent[(sizeof(CSrvNetStats)==0x18)?1:-1];
typedef char CHelpDialogObservedExtent[(sizeof(CHelpDialog)==0x4)?1:-1];
typedef char CChatBubbleObservedExtent[(sizeof(CChatBubble)==0x44a20)?1:-1];
// samp.dll RVA: 0x0016c7f8
DWORD g_srvNetStatsHistory[74];
// samp.dll RVA: 0x0006a090
CDeathWindow::CDeathWindow(IDirect3DDevice9 *device){
    *(DWORD*)(nativeState+0x137)=0;*(DWORD*)(nativeState+0x13b)=0;
    *(DWORD*)(nativeState+0x13f)=0;*(DWORD*)(nativeState+0x143)=0;
    *(DWORD*)(nativeState+0x14b)=0;*(DWORD*)(nativeState+0x14f)=0;
    *(DWORD*)(nativeState+0x153)=0;*(DWORD*)nativeState=1;
    *(IDirect3DDevice9**)(nativeState+0x147)=device;
    ResetFonts();memset(nativeState+4,0,0x127);
}
// samp.dll RVA: 0x0006e460
CScoreboard::CScoreboard(IDirect3DDevice9 *device){
    DWORD *state=(DWORD*)nativeState;
    state[12]=(DWORD)device;state[13]=0;state[14]=0;state[15]=0;state[16]=0;state[1]=0;
    if(!g_pGame||g_chatScreenWidth<801){state[5]=0x44200000;state[6]=0x43f00000;}
    else {state[5]=0x44480000;state[6]=0x44160000;}
    state[7]=0x42700000;state[8]=0;state[9]=0x3dae147b;state[10]=0x3e880000;state[11]=0x3ee00000;
    Recalc();state[0]=0;
}
// samp.dll RVA: 0x0006e360
void CScoreboard::Recalc(){
    RECT rect;GetClientRect(g_inputGameWindow,&rect);
    *(float*)(nativeState+0x10)=1.0f;
    *(float*)(nativeState+8)=(float)rect.right*0.5f-*(float*)(nativeState+0x14)*0.5f;
    *(float*)(nativeState+12)=(float)rect.bottom*0.5f-*(float*)(nativeState+0x18)*0.5f;
}

// samp.dll RVA: 0x000607c0
CNetStats::CNetStats(IDirect3DDevice9 *device){
    DWORD *state=(DWORD*)nativeState;state[2]=GetTickCount();state[1]=0;state[0]=0;state[4]=0;state[3]=0;state[5]=(DWORD)device;
}
// samp.dll RVA: 0x00070cc0
CSrvNetStats::CSrvNetStats(IDirect3DDevice9 *device){
    DWORD *state=(DWORD*)nativeState;state[2]=GetTickCount();state[1]=0;state[0]=0;state[4]=0;state[3]=0;state[5]=(DWORD)device;
    memset(g_srvNetStatsHistory,0,sizeof(g_srvNetStatsHistory));
}

// samp.dll RVA: 0x00066860
CChatBubble::CChatBubble(){memset(nativeState,0,sizeof(nativeState));}

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


#include "gui/dialog_resources.h"
#include "gui/device_lifecycle.h"
#include "game/game.h"
#include <DXUT.h>
#include <DXUTgui.h>
// Allocation extent and packed manager layout observed in the original constructor.
typedef char DialogManagerExtent[(sizeof(CDXUTDialogResourceManager)==36)?1:-1];
// samp.dll RVA: 0x000c6a60
void ClientInitializeDialogResources() {
    g_deviceDialogResources=new CDXUTDialogResourceManager;
    g_deviceDialogResources->OnCreateDevice(g_gameD3DDevice);
    g_deviceDialogResources->OnResetDevice();
}
