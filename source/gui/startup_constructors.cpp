#include "gui/render_dependencies.h"
#include "gui/device_objects.h"
#include "game/input.h"
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
// samp.dll RVA: 0x0006eec0
CDialog::CDialog(IDirect3DDevice9 *device){
    DWORD *state=(DWORD*)nativeState;state[0]=(DWORD)device;state[7]=0;state[10]=0;state[13]=0;state[12]=0;state[11]=0;
    state[3]=600;state[4]=300;state[5]=100;state[6]=30;memset(nativeState+0x89,0,0x204);
}
// samp.dll RVA: 0x00071100
CRenderPanel::CRenderPanel(IDirect3DDevice9 *device){
    DWORD *state=(DWORD*)nativeState;state[0]=(DWORD)device;state[8]=0;state[9]=0;state[3]=280;state[4]=150;state[5]=210;state[6]=30;state[7]=38;
}
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
// samp.dll RVA: 0x000607c0
CNetStats::CNetStats(IDirect3DDevice9 *device){
    DWORD *state=(DWORD*)nativeState;state[2]=GetTickCount();state[1]=0;state[0]=0;state[4]=0;state[3]=0;state[5]=(DWORD)device;
}
// samp.dll RVA: 0x00070cc0
CSrvNetStats::CSrvNetStats(IDirect3DDevice9 *device){
    DWORD *state=(DWORD*)nativeState;state[2]=GetTickCount();state[1]=0;state[0]=0;state[4]=0;state[3]=0;state[5]=(DWORD)device;
    memset(g_srvNetStatsHistory,0,sizeof(g_srvNetStatsHistory));
}
// samp.dll RVA: 0x0006b560
CHelpDialog::CHelpDialog(IDirect3DDevice9 *device){*(IDirect3DDevice9**)nativeState=device;}
// samp.dll RVA: 0x00066860
CChatBubble::CChatBubble(){memset(nativeState,0,sizeof(nativeState));}
