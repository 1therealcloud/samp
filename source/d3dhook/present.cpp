#include "d3dhook/IDirect3DDevice9Hook.h"
#include "game/device_integrity.h"
#include "game/input.h"
#include "game/playerped.h"
#include "game/vehicle.h"
#include "gui/device_lifecycle.h"
#include "gui/render_dependencies.h"
#include "chatwindow.h"
#include "cmdwindow.h"
#include "BitStream.h"
#include "RakClientInterface.h"
#include <DXUT.h>
#include <DXUTgui.h>
#include <stdio.h>
#include <string.h>
// samp.dll RVA: 0x002aca1c
CAudioStream *g_renderAudioStream;
// samp.dll RVA: 0x002aca08
CChatBubble *g_renderChatBubble;
// samp.dll RVA: 0x002ac9e0
CDialog *g_renderDialog;
// samp.dll RVA: 0x002ac9e4
CRenderPanel *g_renderPanel;
// samp.dll RVA: 0x002ac9e8
CDownloadWindow *g_renderDownloadWindow;
// samp.dll RVA: 0x002ac9f4
CObjectSelection *g_renderObjectSelection;
// samp.dll RVA: 0x002ac9f8
CTextDrawSelection *g_renderTextDrawSelection;
// samp.dll RVA: 0x002ac9fc
CNetStats *g_renderNetStats;
// samp.dll RVA: 0x002aca00
CSrvNetStats *g_renderServerNetStats;
// samp.dll RVA: 0x002aca04
CHelpDialog *g_renderHelp;
// samp.dll RVA: 0x002acab4
CDXUTDialog *g_renderFirstDialog;
// samp.dll RVA: 0x002acabc
CDXUTDialog *g_renderSecondDialog;
// samp.dll RVA: 0x0016be7c
DWORD g_renderScreenshotRequested;
// samp.dll RVA: 0x002aca48
BYTE g_renderShowVehicleInfo;
// samp.dll RVA: 0x00140bdc
GroundHeightFunction g_renderGroundHeight=(GroundHeightFunction)0x569660; // GTA SA virtual address.
// samp.dll RVA: 0x0011bbd4
int g_renderMapClickRpc=119;
// samp.dll RVA: 0x000751b0
HRESULT __stdcall IDirect3DDevice9Hook::Present(CONST RECT *source,CONST RECT *dest,HWND window,CONST RGNDATA *dirty) {
    if(g_renderScreenshotRequested)ClientCaptureScreenshot();
    ClientProcessMenuTransition();
    if(g_pGame->IsMenuVisible()) {
        if(g_pGame->IsMenuVisible()&&g_pNetGame&&*(DWORD*)0xba6774&&(GetAsyncKeyState(VK_RBUTTON)&1)&&!*(BYTE*)0xba8298) {
            // Native leaves both coordinates uninitialized if no marker matches.
            // No deterministic behavior is claimed for that invalid-marker path.
            float x,y;
            for(int i=0;i<175;i++) {
                BYTE *marker=(BYTE*)(0xba873d+i*40);
                if(*(WORD*)marker==0x1203){memcpy(&x,marker-0x1d,4);memcpy(&y,marker-0x19,4);}
            }
            RakNet::BitStream stream;stream.Write(x);stream.Write(y);
            float z=g_renderGroundHeight(x,y)+2.0f;stream.Write(z);
            (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(&g_renderMapClickRpc,&stream,HIGH_PRIORITY,RELIABLE,0,false);
        }
        goto present;
    }
    if(g_pNetGame) {
        BYTE *vehicles=*(BYTE**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0xc);
        ClientDrawPlayerTags();ClientDrawLabels();
        if(g_renderAudioStream)g_renderAudioStream->DrawInfo();
        if(g_renderShowVehicleInfo) {
            if(g_deviceLabel)g_deviceLabel->Begin();
            for(int i=0;i<2000;i++) {
                if(*(DWORD*)(vehicles+0x3074+i*4)!=1)continue;
                CVehicle *vehicle=*(CVehicle**)(vehicles+0x1134+i*4);
                if(!vehicle)continue;
                CEntity *entity=(CEntity*)vehicle;
                if(entity->GetDistanceToLocalPlayer()<=20.0f) {
                    CMatrix matrix;entity->GetMatrix(&matrix);CVector position=matrix.position;
                    CVector *sync=(CVector*)(vehicles+0x11ad4+i*12);
                    // Native promotes these six values before the following getters.
                    double cx=position.x,cy=position.y,cz=position.z,sx=sync->x,sy=sync->y,sz=sync->z;
                    unsigned seats=vehicle->GetMaxPassengers();
                    double distance=entity->GetDistanceToLocalPlayer();
                    int preloadModel=entity->GetModelIndex();
                    unsigned preloaded=*((BYTE*)g_pGame+preloadModel-0x122);
                    double health=vehicle->GetHealth();int subtype=vehicle->GetSubtype();int model=entity->GetModelIndex();
                    char text[256];
                    sprintf(text,"[id: %d, type: %d subtype: %d Health: %.1f preloaded: %u]\nDistance: %.2fm\nPassengerSeats: %u\ncPos: %.3f,%.3f,%.3f\nsPos: %.3f,%.3f,%.3f",i,model,subtype,health,preloaded,distance,seats,cx,cy,cz,sx,sy,sz);
                    g_deviceLabel->Draw(&position,text,0xff358bd4,TRUE,FALSE);
                }
            }
            if(g_deviceLabel)g_deviceLabel->End();
        }
        if(g_pNetGame) {
            if(*(DWORD*)g_deviceScoreboard){g_pGame->EnableHUD(FALSE);g_deviceScoreboard->Draw();goto present;}
            SHORT key=GetAsyncKeyState(VK_F5);
            if(key&&*(DWORD*)((BYTE*)g_pNetGame+0x3cd)==5) {
                g_pGame->EnableHUD(FALSE);if(g_renderNetStats)g_renderNetStats->Draw();goto present;
            }
            if(g_pNetGame) {
                key=GetAsyncKeyState(VK_F10);
                if(key){g_pGame->EnableHUD(FALSE);if(g_renderServerNetStats)g_renderServerNetStats->Draw();goto present;}
                if(g_pNetGame&&GetAsyncKeyState(VK_F1)&&g_renderHelp)g_renderHelp->Show();
            }
        }
    }
    g_pGame->EnableHUD(g_pChat->m_nMode!=0);
    if(g_pGame&&g_pGame->GetPlayerPed()&&(int)g_pGame->GetPlayerPed()->GetDrunkLevel()>5000)g_pGame->EnableHUD(FALSE);
    if(g_renderChatBubble)g_renderChatBubble->Draw();
    if(g_pNetGame&&*(DWORD*)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0x1c))
        (*(CLabelPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0x1c))->Draw();
    if(g_pChat)g_pChat->Draw();
    if(g_pInput)g_pInput->EnableCursor();
    if(g_deviceDeathWindow)((CDeathWindow*)g_deviceDeathWindow)->Draw();
    if(g_pNetGame) {
        if(g_renderDialog)g_renderDialog->Draw();
        if(g_pNetGame) {
            if(g_renderPanel)g_renderPanel->Draw();
            if(g_pNetGame&&g_renderDownloadWindow)g_renderDownloadWindow->Draw();
        }
    }
    if(g_renderFirstDialog)g_renderFirstDialog->OnRender(10.0f);
    if(g_renderSecondDialog)g_renderSecondDialog->OnRender(10.0f);
    if(g_deviceObjectEdit)((CObjectEdit*)g_deviceObjectEdit)->Render();
    if(g_renderObjectSelection)g_renderObjectSelection->Draw();
    if(g_renderTextDrawSelection)g_renderTextDrawSelection->Process();
present:
    if(g_clientSettings.debug)GameDebugDraw();
    GameDeviceIntegrityCycle();
    HRESULT result=g_gameD3DDevice->Present(source,dest,window,dirty);
    GameDeviceIntegrityCycle();return result;
}
