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
#include "net/netrpc.h"
#include "net/scriptrpc.h"
#include "RakNetworkFactory.h"
#include "game/camera.h"
#include "game/playerped.h"
#include "game/patches.h"
#include "gui/render_dependencies.h"
// samp.dll RVA: 0x001179dc
const char g_netStartedMessage[]="{FFFFFF}SA-MP {B9C9BF}0.3.DL-R1 {FFFFFF}Started";
// samp.dll RVA: 0x00117af0
const char g_netDisconnectedMessage[]="Server closed the connection.";
// samp.dll RVA: 0x00117b10
const char g_netInvalidPasswordMessage[]="Wrong server password.";
// samp.dll RVA: 0x00008450
void CNetGame::InitialNotification() {
    if(g_pChat)g_pChat->AddDebugMessage(g_netStartedMessage);
}
// samp.dll RVA: 0x000085e0
void CNetGame::InitializeGameLogic() {
    if((*(BYTE**)(nativeState+0x3d5))[0x2b])GameApplyManualVehicleControlPatches();
    *(DWORD*)(*(BYTE**)(nativeState+0x3d5)+5)=0x469c4000;
    *(DWORD*)(*(BYTE**)(nativeState+0x3d5)+9)=0xc69c4000;
    *(DWORD*)(*(BYTE**)(nativeState+0x3d5)+0xd)=0x469c4000;
    *(DWORD*)(*(BYTE**)(nativeState+0x3d5)+0x11)=0xc69c4000;
}
// samp.dll RVA: 0x00008ad0
void CNetGame::Packet_DisconnectionNotification(Packet *packet) {
    (void)packet;g_pChat->AddDebugMessage(g_netDisconnectedMessage);
    if(g_renderAudioStream)g_renderAudioStream->Stop(true);
    (*(RakClientInterface**)(nativeState+0x2c))->Disconnect(2000,0);
}
// samp.dll RVA: 0x00008b10
void CNetGame::Packet_InvalidPassword(Packet *packet) {
    (void)packet;g_pChat->AddDebugMessage(g_netInvalidPasswordMessage);
    (*(RakClientInterface**)(nativeState+0x2c))->Disconnect(0,0);
}
// samp.dll RVA: 0x00009fb0
void CNetGame::ResetMapIcons() {
    for(BYTE index=0;index<100;index++) {
        DWORD *icon=(DWORD*)(nativeState+0x23d+index*4);
        if(*icon){Scripting::ProcessCommand(&g_netRemoveMapIconCommand,*icon);*icon=0;}
    }
}
// samp.dll RVA: 0x00009ff0
void CNetGame::CreateMapIcon(BYTE index,CVector position,BYTE icon,DWORD color,int type) {
    if(index<100) {
        DWORD *slot=(DWORD*)(nativeState+0x23d+index*4);
        if(*slot){Scripting::ProcessCommand(&g_netRemoveMapIconCommand,*slot);*slot=0;}
        *slot=g_pGame->CreateMarker(icon,position.x,position.y,position.z,color,type);
    }
}
// samp.dll RVA: 0x0000a1e0
void CNetGame::ResetPools() {
    ResetVehiclePool();ResetActorPool();ResetTextDrawPool();ResetGangZonePool();
    ResetLabelPool();ResetMapIcons();ResetPickupPool();ResetObjectPool();ResetMenuPool();
}
// samp.dll RVA: 0x00009570
void CNetGame::Destroy() {
    (*(RakClientInterface**)(nativeState+0x2c))->Disconnect(0,0);
    UnRegisterRPCs(*(RakClientInterface**)(nativeState+0x2c));
    UnRegisterScriptRPCs(*(RakClientInterface**)(nativeState+0x2c));
    RakNetworkFactory::DestroyRakClientInterface(*(RakClientInterface**)(nativeState+0x2c));
    CPlayerPool *players=pools->players;if(players){delete players;pools->players=0;}
    CVehiclePool *vehicles=pools->vehicles;if(vehicles){delete vehicles;pools->vehicles=0;}
    CActorPool *actors=pools->actors;if(actors){delete actors;pools->actors=0;}
    CTextDrawPool *textDraws=pools->textDraws;if(textDraws){delete textDraws;pools->textDraws=0;}
    CGangZonePool *gangZones=pools->gangZones;if(gangZones){delete gangZones;pools->gangZones=0;}
    CLabelPool *labels=pools->labels;if(labels){delete labels;pools->labels=0;}
    CPickupPool *pickups=pools->pickups;if(pickups){delete pickups;pools->pickups=0;}
    CObjectPool *objects=pools->objects;if(objects){delete objects;pools->objects=0;}
    CMenuPool *menus=pools->menus;if(menus){delete menus;pools->menus=0;}
    if(pools){operator delete(pools);pools=0;}
    void *settings=*(void**)(nativeState+0x3d5);
    if(settings){operator delete(settings);*(void**)(nativeState+0x3d5)=0;}
}
// samp.dll RVA: 0x00117a0c
const char g_netConnectingMessage[]="Connecting to %s:%d...";
// samp.dll RVA: 0x00117a64
const char g_netEncryptionFailedMessage[]="Failed to initialize encryption.";
// samp.dll RVA: 0x00117a88
const char g_netConnectionBannedMessage[]="You are banned from this server.";
// samp.dll RVA: 0x00117aac
const char g_netConnectionAcceptedMessage[]="Server has accepted the connection.";
// samp.dll RVA: 0x00117ad0
const char g_netServerFullMessage[]="The server is full. Retrying...";
// samp.dll RVA: 0x00117b28
const char g_netNoResponseMessage[]="The server didn't respond. Retrying..";
// samp.dll RVA: 0x00008630
void CNetGame::Connect() {
    DWORD (WINAPI *tick)()=GetTickCount;
    if(tick()-*(DWORD*)(nativeState+0x3d1)>3000) {
        if(g_pChat)g_pChat->AddDebugMessage(g_netConnectingMessage,(char*)nativeState+0x30,*(int*)(nativeState+0x235));
        (*(RakClientInterface**)(nativeState+0x2c))->Connect((char*)nativeState+0x30,*(WORD*)(nativeState+0x235),0,0,2);
        *(DWORD*)(nativeState+0x3d1)=tick();*(DWORD*)(nativeState+0x3cd)=2;
    }
}
// samp.dll RVA: 0x000086a0
void CNetGame::SpawnScreen() {
    if(g_pGame->GetPlayerPed()->IsInVehicle())g_pGame->GetPlayerPed()->WarpFromVehicle(1093.4000244140625f,-2036.5f,82.710601806640625f);
    else {CVector position={1133.0504150390625f,-2038.4034423828125f,69.09999847412109375f};g_pGame->GetPlayerPed()->Teleport(position);}
    CVector position={1093.0f,-2036.0f,90.0f},rotation={0,0,0};
    g_pGame->m_pCamera->Set(position,rotation);CVector point={384.0f,-1557.0f,20.0f};g_pGame->m_pCamera->PointAt(point,2);
    g_pGame->SetWorldWeather(1);g_pGame->EnableHUD(FALSE);
}
// samp.dll RVA: 0x00008a40
void CNetGame::Packet_RSAPublicKeyMismatch(Packet *packet) {(void)packet;g_pChat->AddDebugMessage(g_netEncryptionFailedMessage);}
// samp.dll RVA: 0x00008a60
void CNetGame::Packet_ConnectionBanned(Packet *packet) {(void)packet;g_pChat->AddDebugMessage(g_netConnectionBannedMessage);}
// samp.dll RVA: 0x00008a80
void CNetGame::Packet_ConnectionRequestAccepted(Packet *packet) {(void)packet;g_pChat->AddDebugMessage(g_netConnectionAcceptedMessage);}
// samp.dll RVA: 0x00008aa0
void CNetGame::Packet_NoFreeIncomingConnections(Packet *packet) {(void)packet;g_pChat->AddDebugMessage(g_netServerFullMessage);*(DWORD*)(nativeState+0x3cd)=1;}
// samp.dll RVA: 0x00008b50
void CNetGame::Packet_ConnectionAttemptFailed(Packet *packet) {(void)packet;g_pChat->AddDebugMessage(g_netNoResponseMessage);*(DWORD*)(nativeState+0x3cd)=1;}
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
