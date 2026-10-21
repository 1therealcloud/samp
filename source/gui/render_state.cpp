#include "gui/render_dependencies.h"
#include "main.h"
#include "game/playerped.h"
#include "game/scripting.h"
// samp.dll RVA: 0x0017ebe0
BYTE g_renderMenuWasVisible;
// samp.dll RVA: 0x0018e538
IUnknown *g_gameLastReleasedResource;
// samp.dll RVA: 0x0018dde8
DWORD g_gameDebugStage;



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
