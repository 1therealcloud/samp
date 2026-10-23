#include "net/netgame.h"
#include "BitStream.h"
#include "RakClientInterface.h"
#include "main.h"
#include "chatwindow.h"
#include "config.h"
#include "GetTime.h"
// samp.dll RVA: 0x000085d0
__declspec(naked) __int64 CNetGame::GetCounter() {
    // Do not zero-extend a typed32-bit return: native forwards both registers.
    __asm jmp RakNet::GetTime
}
// samp.dll RVA: 0x00008b80
void CNetGame::ToggleNameTagStatus() {
    if(!nativeState[0x234]) {
        g_pChat->AddInfoMessage("NameTag Player Status: ON");
        if(g_pConfig)g_pConfig->WriteIntValue("nonametagstatus",0,FALSE);
        nativeState[0x234]=1;
    } else {
        g_pChat->AddInfoMessage("NameTag Player Status: OFF");
        if(g_pConfig)g_pConfig->WriteIntValue("nonametagstatus",1,FALSE);
        nativeState[0x234]=0;
    }
}
// samp.dll RVA: 0x00156b38
DWORD g_netPlayersInfoTick=0;
// samp.dll RVA: 0x00117b98
int g_netPlayersInfoRpc=155;
// samp.dll RVA: 0x00008c00
void CNetGame::UpdatePlayersInfo() {
    if(GetTickCount()-g_netPlayersInfoTick>3000) {
        g_netPlayersInfoTick=GetTickCount();
        RakNet::BitStream stream;
        RakClientInterface *client=*(RakClientInterface**)(nativeState+0x2c);
        if(client)client->RPC(&g_netPlayersInfoRpc,&stream,HIGH_PRIORITY,RELIABLE,0,false);
    }
}
// samp.dll RVA: 0x000081e0
CMenuPool *CNetGame::GetMenuPool() {return *(CMenuPool**)(*(BYTE**)(nativeState+0x3de));}
// samp.dll RVA: 0x00002e40
CObjectPool *CNetGame::GetObjectPool() {return *(CObjectPool**)(*(BYTE**)(nativeState+0x3de)+0x14);}
// samp.dll RVA: 0x00001180
CVehiclePool *CNetGame::GetVehiclePool() { return *(CVehiclePool**)(*(BYTE**)(nativeState+0x3de)+0xc); }
// samp.dll RVA: 0x00001170
CPlayerPool *CNetGame::GetPlayerPool() { return *(CPlayerPool**)(*(BYTE**)(nativeState+0x3de)+8); }
