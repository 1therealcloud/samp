#include "game/hooks_prerender.h"
#include "game/game.h"
#include "game/scripting.h"
#include "net/vehiclepool.h"
#include "gui/device_objects.h"
#include "chatwindow.h"
#include "main.h"

// samp.dll RVA: 0x0018de44
CObjectPool *g_debugObjectPool;

// samp.dll RVA: 0x000a23d0
void GameHook_000a23d0() {
    if(g_pGame)g_pGame->ProcessFrameLimiter();
    if(g_pNetGame) {
        CObjectPool *objects=*(CObjectPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0x14);
        if(objects) {
            __try { objects->Process(); }
            __except(GameExceptionFilter(GetExceptionCode(),GetExceptionInformation(),"s003")) {}
        }
    }
    if(g_debugObjectPool) {
        __try { g_debugObjectPool->Process(); }
        __except(GameExceptionFilter(GetExceptionCode(),GetExceptionInformation(),"s003d")) {}
    }
    if(g_pNetGame) {
        CVehiclePool *vehicles=*(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0xc);
        if(vehicles)vehicles->ConstructLicensePlates();
    }
    if(g_pNetGame) {
        CObjectPool *objects=*(CObjectPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0x14);
        if(objects)objects->ConstructMaterials();
    }
    if(g_pChat)g_pChat->RenderToSurface();
    __try { ((void (__cdecl*)())0x53bee0)(); }
    __except(GameExceptionFilter(GetExceptionCode(),GetExceptionInformation(),"s007")) {}
    if(g_pNetGame) {
        CTextDrawPool *textdraws=*(CTextDrawPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0x20);
        if(textdraws)textdraws->Native01E330();
    }
}
