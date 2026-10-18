#include "main.h"
// samp.dll RVA: 0x0018dc38
DWORD g_clientStartupMarker;
// samp.dll RVA: 0x0009db90
void GameMarkStartupStage() { g_clientStartupMarker=1; }
// samp.dll RVA: 0x000c5150
void __cdecl ClientStartupThread(void *argument) {
    (void)argument;
    if(g_pGame)g_pGame->InitGame();
    while(*(volatile DWORD*)0xc8d4c0!=7)Sleep(5);
    GameMarkStartupStage();g_pGame->StartGame();ExitThread(0);
}
