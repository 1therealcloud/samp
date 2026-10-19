#include "client/commands.h"
#include "client/frame.h"
#include "main.h"
#include "RakClientInterface.h"
// samp.dll RVA: 0x000c5550
void ClientRequestQuit(){
    if(g_pNetGame&&*(DWORD*)((BYTE*)g_pNetGame+0x3cd)==5)
        (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->Disconnect(500,0);
    g_clientQuitRequested=1;g_clientQuitStarted=GetTickCount();
}
