#include "gui/device_lifecycle.h"
#include "d3dhook/texture_binding.h"
#include "game/hooks_independent.h"
#include "game/input.h"
#include "net/vehiclepool.h"
#include "chatwindow.h"
#include "cmdwindow.h"
#include <DXUT.h>
#include <DXUTgui.h>
// samp.dll RVA: 0x002ac9d8
CPlayerTags *g_devicePlayerTags;
// samp.dll RVA: 0x002aca20
CSpawnScreen *g_deviceSpawnScreen;
// samp.dll RVA: 0x002ac9ec
CLabel *g_deviceLabel;
// samp.dll RVA: 0x002aca34
CObjectMaterialText *g_deviceMaterialText;
// samp.dll RVA: 0x002aca18
BYTE *g_deviceDeathWindow;
// samp.dll RVA: 0x002ac9f0
BYTE *g_deviceObjectEdit;
// samp.dll RVA: 0x002ac9dc
CScoreboard *g_deviceScoreboard;
// samp.dll RVA: 0x002acab0
CDXUTDialogResourceManager *g_deviceDialogResources;
// samp.dll RVA: 0x002acacc
IDirect3DSurface9 *g_deviceCursorSurface;
// samp.dll RVA: 0x0016be80
DWORD g_deviceResetMarker;
// samp.dll RVA: 0x000c5620
void ClientDeviceObjectsLost() {
    if(g_devicePlayerTags)g_devicePlayerTags->OnLostDevice();
    if(g_deviceFonts)g_deviceFonts->OnLostDevice();
    if(g_deviceSpawnScreen)g_deviceSpawnScreen->OnLostDevice();
    if(g_deviceLabel)g_deviceLabel->OnLostDevice();
    if(g_devicePlateManager)((CLicensePlate*)g_devicePlateManager)->OnLostDevice();
    if(g_deviceMaterialText)g_deviceMaterialText->OnLostDevice();
    if(g_pNetGame&&*(DWORD*)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0x14))
        (*(CObjectPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0x14))->ShutdownMaterials();
    if(g_deviceDeathWindow&&*(DWORD*)(g_deviceDeathWindow+0x137))(*(ID3DXFont**)(g_deviceDeathWindow+0x137))->OnLostDevice();
    if(g_deviceDeathWindow&&*(DWORD*)(g_deviceDeathWindow+0x13b))(*(ID3DXFont**)(g_deviceDeathWindow+0x13b))->OnLostDevice();
    if(g_deviceDeathWindow&&*(DWORD*)(g_deviceDeathWindow+0x13f))(*(ID3DXFont**)(g_deviceDeathWindow+0x13f))->OnLostDevice();
    if(g_deviceDeathWindow&&*(DWORD*)(g_deviceDeathWindow+0x14f))(*(ID3DXFont**)(g_deviceDeathWindow+0x14f))->OnLostDevice();
    if(g_deviceDeathWindow&&*(DWORD*)(g_deviceDeathWindow+0x153))(*(ID3DXFont**)(g_deviceDeathWindow+0x153))->OnLostDevice();
    if(g_deviceDeathWindow&&*(DWORD*)(g_deviceDeathWindow+0x143))(*(ID3DXSprite**)(g_deviceDeathWindow+0x143))->OnLostDevice();
    if(g_deviceObjectEdit&&*(DWORD*)(g_deviceObjectEdit+0x10b))(*(ID3DXFont**)(g_deviceObjectEdit+0x10b))->OnLostDevice();
    if(g_deviceObjectEdit&&*(DWORD*)(g_deviceObjectEdit+0x10f))(*(ID3DXFont**)(g_deviceObjectEdit+0x10f))->OnLostDevice();
    if(g_deviceObjectEdit&&*(DWORD*)(g_deviceObjectEdit+0x107))(*(ID3DXLine**)(g_deviceObjectEdit+0x107))->OnLostDevice();
    if(g_pChat)g_pChat->OnLostDevice();
    if(g_pChat&&*(DWORD*)((BYTE*)g_pChat+0x63a6))(*(ID3DXSprite**)((BYTE*)g_pChat+0x63a6))->OnLostDevice();
    if(g_pChat&&*(DWORD*)((BYTE*)g_pChat+0x63aa))(*(ID3DXSprite**)((BYTE*)g_pChat+0x63aa))->OnLostDevice();
    if(g_pInput&&g_pInput->m_bEnabled)g_pInput->Close();
    if(g_deviceScoreboard&&*(DWORD*)g_deviceScoreboard)g_deviceScoreboard->Close(true);
    if(g_deviceDialogResources)g_deviceDialogResources->OnLostDevice();
    if(g_deviceCursorSurface){g_deviceCursorSurface->Release();g_deviceCursorSurface=0;}
    g_deviceResetMarker=0;g_hookKeepBuffer=0;
    if(g_pNetGame) {
        BYTE *pool=*(BYTE**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0xc);
        if(pool){*(DWORD*)(pool+0x17894)=0;((CVehiclePool*)pool)->ShutdownLicensePlates();}
        if(g_pNetGame&&*(DWORD*)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0x20))
            (*(CTextDrawPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0x20))->OnLostDevice();
    }
}
// samp.dll RVA: 0x000c5880
void ClientDeviceObjectsReset() {
    SetCursor(0);
    if(g_deviceDialogResources)g_deviceDialogResources->OnResetDevice();
    if(g_devicePlayerTags)g_devicePlayerTags->OnResetDevice();
    if(g_deviceFonts)g_deviceFonts->OnResetDevice();
    if(g_deviceSpawnScreen)g_deviceSpawnScreen->OnResetDevice();
    if(g_deviceLabel)g_deviceLabel->OnResetDevice();
    if(g_devicePlateManager)((CLicensePlate*)g_devicePlateManager)->OnResetDevice();
    if(g_deviceMaterialText)g_deviceMaterialText->OnResetDevice();
    if(g_deviceDeathWindow&&*(DWORD*)(g_deviceDeathWindow+0x137))(*(ID3DXFont**)(g_deviceDeathWindow+0x137))->OnResetDevice();
    if(g_deviceDeathWindow&&*(DWORD*)(g_deviceDeathWindow+0x13b))(*(ID3DXFont**)(g_deviceDeathWindow+0x13b))->OnResetDevice();
    if(g_deviceDeathWindow&&*(DWORD*)(g_deviceDeathWindow+0x13f))(*(ID3DXFont**)(g_deviceDeathWindow+0x13f))->OnResetDevice();
    if(g_deviceDeathWindow&&*(DWORD*)(g_deviceDeathWindow+0x14f))(*(ID3DXFont**)(g_deviceDeathWindow+0x14f))->OnResetDevice();
    if(g_deviceDeathWindow&&*(DWORD*)(g_deviceDeathWindow+0x153))(*(ID3DXFont**)(g_deviceDeathWindow+0x153))->OnResetDevice();
    if(g_deviceDeathWindow&&*(DWORD*)(g_deviceDeathWindow+0x143))(*(ID3DXSprite**)(g_deviceDeathWindow+0x143))->OnResetDevice();
    if(g_deviceObjectEdit&&*(DWORD*)(g_deviceObjectEdit+0x10b))(*(ID3DXFont**)(g_deviceObjectEdit+0x10b))->OnResetDevice();
    if(g_deviceObjectEdit&&*(DWORD*)(g_deviceObjectEdit+0x10f))(*(ID3DXFont**)(g_deviceObjectEdit+0x10f))->OnResetDevice();
    if(g_deviceObjectEdit&&*(DWORD*)(g_deviceObjectEdit+0x107))(*(ID3DXLine**)(g_deviceObjectEdit+0x107))->OnResetDevice();
    if(g_pChat)g_pChat->OnResetDevice();
    if(g_pChat&&*(DWORD*)((BYTE*)g_pChat+0x63a6))(*(ID3DXSprite**)((BYTE*)g_pChat+0x63a6))->OnResetDevice();
    if(g_pChat&&*(DWORD*)((BYTE*)g_pChat+0x63aa))(*(ID3DXSprite**)((BYTE*)g_pChat+0x63aa))->OnResetDevice();
    g_gameD3DDevice->CreateOffscreenPlainSurface(32,32,D3DFMT_A8R8G8B8,D3DPOOL_SYSTEMMEM,&g_deviceCursorSurface,0);
    D3DXLoadSurfaceFromFileA(g_deviceCursorSurface,0,0,"mouse.png",0,D3DX_FILTER_NONE,0,0);
    if(g_deviceCursorSurface)g_gameD3DDevice->SetCursorProperties(0,0,g_deviceCursorSurface);
    if(g_deviceScoreboard)g_deviceScoreboard->Recalc();
    if(!g_pGame->IsMenuVisible()) {
        g_pGame->SetCursorMode(2,FALSE);g_pGame->SetCursorMode(0,TRUE);g_pGame->ProcessInputEnabling();
    }
    g_hookKeepBuffer=1;
    if(g_pNetGame) {
        BYTE *pool=*(BYTE**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0xc);
        if(pool)*(DWORD*)(pool+0x17894)=1;
    }
}
