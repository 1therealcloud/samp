#include "net/netgame.h"
#include "BitStream.h"
#include "RakClientInterface.h"
#include "main.h"
#include "chatwindow.h"
#include "config.h"
#include "GetTime.h"
#include "net/objectpool.h"
#include "net/menupool.h"
#include "net/labelpool.h"
#include "net/textdrawpool.h"
#include "net/gangzonepool.h"
#include "net/playerpool.h"
#include "net/vehiclepool.h"
#include "net/pickuppool.h"
#include "net/actorpool.h"
#include <stdlib.h>
// samp.dll RVA: 0x00008230
void CNetGame::InitializePools() {
    pools=(CNetGamePools*)calloc(1,sizeof(CNetGamePools));
    CObjectPool *objects=new CObjectPool;pools->objects=objects;
    CMenuPool *menus=new CMenuPool;pools->menus=menus;
    CLabelPool *labels=new CLabelPool;pools->labels=labels;
    CTextDrawPool *textDraws=new CTextDrawPool;pools->textDraws=textDraws;
    CGangZonePool *gangZones=new CGangZonePool;pools->gangZones=gangZones;
    CPlayerPool *players=new CPlayerPool;pools->players=players;
    CVehiclePool *vehicles=new CVehiclePool;pools->vehicles=vehicles;
    CPickupPool *pickups=new CPickupPool;pools->pickups=pickups;
    CActorPool *actors=new CActorPool;pools->actors=actors;
}
// samp.dll RVA: 0x00008cd0
void CNetGame::ResetPlayerPool() {
    CPlayerPool *old=pools->players;if(old)delete old;
    CPlayerPool *created=new CPlayerPool;pools->players=created;
}
// samp.dll RVA: 0x00008d70
void CNetGame::ResetVehiclePool() {
    CVehiclePool *old=pools->vehicles;if(old)delete old;
    CVehiclePool *created=new CVehiclePool;pools->vehicles=created;
}
// samp.dll RVA: 0x00008e10
void CNetGame::ResetTextDrawPool() {
    CTextDrawPool *old=pools->textDraws;if(old)delete old;
    CTextDrawPool *created=new CTextDrawPool;pools->textDraws=created;
}
// samp.dll RVA: 0x00008eb0
void CNetGame::ResetObjectPool() {
    CObjectPool *old=pools->objects;if(old)delete old;
    CObjectPool *created=new CObjectPool;pools->objects=created;
}
// samp.dll RVA: 0x00008f50
void CNetGame::ResetGangZonePool() {
    CGangZonePool *old=pools->gangZones;if(old)delete old;
    CGangZonePool *created=new CGangZonePool;pools->gangZones=created;
}
// samp.dll RVA: 0x00008ff0
void CNetGame::ResetPickupPool() {
    CPickupPool *old=pools->pickups;if(old)delete old;
    CPickupPool *created=new CPickupPool;pools->pickups=created;
}
// samp.dll RVA: 0x00009050
void CNetGame::ResetMenuPool() {
    CMenuPool *old=pools->menus;if(old)delete old;
    CMenuPool *created=new CMenuPool;pools->menus=created;
}
// samp.dll RVA: 0x000090e0
void CNetGame::ResetLabelPool() {
    CLabelPool *old=pools->labels;if(old)delete old;
    CLabelPool *created=new CLabelPool;pools->labels=created;
}
// samp.dll RVA: 0x00009180
void CNetGame::ResetActorPool() {
    CActorPool *old=pools->actors;if(old)delete old;
    CActorPool *created=new CActorPool;pools->actors=created;
}
// samp.dll RVA: 0x001179c8
const Scripting::OpcodeInfo g_netRemoveMapIconCommand={0x164,"i"};
// samp.dll RVA: 0x00008ca0
void CNetGame::RemoveMapIcon(BYTE index) {
    if(index<100){DWORD *icon=(DWORD*)(nativeState+0x23d+index*4);Scripting::ProcessCommand(&g_netRemoveMapIconCommand,*icon);*icon=0;}
}
// samp.dll RVA: 0x000081f0
void CNetGame::SetGameState(DWORD state) {*(DWORD*)(nativeState+0x3cd)=state;}
// samp.dll RVA: 0x000081d0
CPickupPool *CNetGame::GetPickupPool() {return pools->pickups;}
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
