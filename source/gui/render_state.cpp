#include "gui/render_dependencies.h"
#include "main.h"
#include "game/playerped.h"
#include "game/wrapper_commands.h"
// samp.dll RVA: 0x0017ebe0
BYTE g_renderMenuWasVisible;
// samp.dll RVA: 0x0018e538
IUnknown *g_gameLastReleasedResource;
// samp.dll RVA: 0x0018dde8
DWORD g_gameDebugStage;
// samp.dll RVA: 0x000a1bd0
void CGame::EnableHUD(BOOL enabled) {
    if(enabled){*(BYTE*)0xba6769=1;*(BYTE*)0xbaa3fb=0;}
    else {*(BYTE*)0xba6769=0;*(BYTE*)0xbaa3fb=1;}
}
// samp.dll RVA: 0x000adba0
DWORD CPed::GetDrunkLevel() { return m_drunkLevel; }
// samp.dll RVA: 0x000a1ff0
void CAudio::Process() {
    if(m_bSoundLoaded==1&&Scripting::ProcessCommand(&g_audioIsLoaded,1)) {
        Scripting::ProcessCommand(&g_audioStart,1);m_bSoundLoaded=2;
    }
}
// samp.dll RVA: 0x000a0280
void GameReleaseRenderResource() {
    g_gameLastReleasedResource=*(IUnknown**)0xc8cfa4;
    if(*(IUnknown**)0xc8cfa4){(*(IUnknown**)0xc8cfa4)->Release();*(IUnknown**)0xc8cfa4=0;}
}
// samp.dll RVA: 0x00073f10
void ClientProcessMenuTransition() {
    if(g_pGame) {
        if(g_pGame->IsMenuVisible()&&!g_renderMenuWasVisible) {
            g_renderMenuWasVisible=1;GameReleaseRenderResource();return;
        }
        if(!g_pGame->IsMenuVisible()&&g_renderMenuWasVisible) {
            g_renderMenuWasVisible=0;GameReleaseRenderResource();
        }
    }
}
// samp.dll RVA: 0x0009e2d0
void GameDebugDraw() { if(g_gameDebugStage==10)GameDrawDebugEntities(); }
