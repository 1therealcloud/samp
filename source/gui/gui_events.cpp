#include "gui/gui.h"
#include "gui/device_lifecycle.h"
#include "gui/render_dependencies.h"
#include "net/localplayer.h"
#include "cmdwindow.h"
#include "main.h"
// samp.dll RVA: 0x000c6c30
void CALLBACK ClientGuiScoreboardEvent(UINT event,int id,CDXUTControl *control,void *context){
    if(g_guiScoreboardDialog&&event==0x701&&g_deviceScoreboard)g_deviceScoreboard->SendNotification();
}
// samp.dll RVA: 0x000c6bf0
void CALLBACK ClientGuiInputEvent(UINT event,int id,CDXUTControl *control,void *context){
    if(id==1&&event==0x601&&g_pInput&&!g_pInput->IsActive())g_pInput->ProcessInput();
}
// samp.dll RVA: 0x000c6c60
void CALLBACK ClientGuiSpawnEvent(UINT event,int id,CDXUTControl *control,void *context){
    // Native resolves these pointers even for events that do not dispatch a command.
    BYTE *pools=*(BYTE* volatile*)((BYTE*)g_pNetGame+0x3de);
    BYTE *players=*(BYTE* volatile*)(pools+8);
    CLocalPlayer *local=*(CLocalPlayer* volatile*)(players+0x1e);
    if(id==4){if(event==0x101)local->RequestSpawn();}
    else if(id==5){if(event==0x101)local->NextClass();}
    else if(id==6&&event==0x101)local->PrevClass();
}
// samp.dll RVA: 0x000c6b60
void CALLBACK ClientGuiDialogEvent(UINT event,int id,CDXUTControl *control,void *context){
    if(id==19){if(event==0x601){if(!g_renderDialog||g_pInput->IsActive())return;g_renderDialog->Close(1);return;}}
    else if(id==20){if(event==0x101){if(g_renderDialog)g_renderDialog->Close(1);return;}}
    else if(id==21&&event==0x101){if(g_renderDialog)g_renderDialog->Close(0);return;}
    if(event==0x701&&g_renderDialog)g_renderDialog->Close(1);
}
// Both empty callbacks are confirmed native RET 16 bodies.
// samp.dll RVA: 0x000c6cc0
void CALLBACK ClientGuiPanelEvent(UINT event,int id,CDXUTControl *control,void *context){}
// samp.dll RVA: 0x000c6cd0
void CALLBACK ClientGuiDownloadEvent(UINT event,int id,CDXUTControl *control,void *context){}
