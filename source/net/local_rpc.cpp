#include "net/local_rpc.h"
#include "net/localplayer.h"
#include "net/vehiclepool.h"
#include "game/game.h"
#include "game/playerped.h"
#include "main.h"
#include "BitStream.h"
#include "RakClientInterface.h"
// The selected RakNet dependency uses SA-MP's shifted reliability enumeration.
typedef char LocalRpcReliability[(HIGH_PRIORITY==1 && RELIABLE_ORDERED==9 && RELIABLE_SEQUENCED==10)?1:-1];
// samp.dll RVA: 0x00117960
int g_localEnterVehicleRpc=26;
// samp.dll RVA: 0x00117964
int g_localExitVehicleRpc=154;
// samp.dll RVA: 0x0011796c
int g_localPlayerDamageRpc=115;
// samp.dll RVA: 0x00117974
int g_localActorDamageRpc=177;
// samp.dll RVA: 0x00117970
const float g_localDamageScale=0.33f;
// samp.dll RVA: 0x00117840
const Scripting::OpcodeInfo g_localTrainCameraCommand={0x158,"iii"};
// samp.dll RVA: 0x0013c0a4
DWORD g_localTrainCameraTick=0xffffffff;
// samp.dll RVA: 0x00005920
void CLocalPlayer::EnterVehicle(WORD vehicle,BOOL passenger) {
    RakNet::BitStream stream;
    stream.WriteBits((const unsigned char*)&vehicle,16,true);
    BYTE flag=passenger!=0;
    stream.WriteBits(&flag,8,true);
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(&g_localEnterVehicleRpc,&stream,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,false);
    // Native looks up the vehicle only after sending, even for invalid IDs.
    CVehiclePool *vehicles=*(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0xc);
    if(vehicle<2000 && vehicles->m_bNotEmpty[vehicle]) {
        CVehicle *object=vehicles->m_pObjects[vehicle];
        if(object && object->IsTrainPart()) {
            Scripting::ProcessCommand(&g_localTrainCameraCommand,object->m_nHandle,3,2);
            g_localTrainCameraTick=GetTickCount();
        }
    }
}
// samp.dll RVA: 0x00005a40
void CLocalPlayer::ExitVehicle(int vehicle) {
    RakNet::BitStream stream;
    CVehiclePool *vehicles=*(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0xc);
    WORD id=(WORD)vehicle;
    if(id>=2000 || !vehicles->m_bNotEmpty[id])return;
    CVehicle *object=vehicles->m_pObjects[id];
    if(!object)return;
    if(!GetPed()->IsPassenger())*(WORD*)((BYTE*)this+0xfe)=id;
    if(object->IsTrainPart())g_pGame->m_pCamera->Restore();
    if(!object->IsRC()) {
        stream.WriteBits((const unsigned char*)&vehicle,16,true);
        (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(&g_localExitVehicleRpc,&stream,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,false);
    }
}
// samp.dll RVA: 0x000066b0
void CLocalPlayer::SendTakeDamage(WORD player,float damage,DWORD weapon,DWORD bodyPart) {
    // Native scales and stores the float before constructing the stream.
    damage*=g_localDamageScale;
    RakNet::BitStream stream;
    stream.Write1();
    stream.WriteBits((const unsigned char*)&player,16,true);
    stream.WriteBits((const unsigned char*)&damage,32,true);
    stream.WriteBits((const unsigned char*)&weapon,32,true);
    stream.WriteBits((const unsigned char*)&bodyPart,32,true);
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(&g_localPlayerDamageRpc,&stream,HIGH_PRIORITY,RELIABLE_ORDERED,0,false);
}
// samp.dll RVA: 0x000067c0
void CLocalPlayer::SendGiveDamage(WORD player,float damage,DWORD weapon,DWORD bodyPart) {
    damage*=g_localDamageScale;
    RakNet::BitStream stream;
    stream.Write0();
    stream.WriteBits((const unsigned char*)&player,16,true);
    stream.WriteBits((const unsigned char*)&damage,32,true);
    stream.WriteBits((const unsigned char*)&weapon,32,true);
    stream.WriteBits((const unsigned char*)&bodyPart,32,true);
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(&g_localPlayerDamageRpc,&stream,HIGH_PRIORITY,RELIABLE_ORDERED,0,false);
}
// samp.dll RVA: 0x000068d0
void CLocalPlayer::SendActorDamage(WORD actor,float damage,DWORD weapon,DWORD bodyPart) {
    RakNet::BitStream stream;
    stream.Write0();
    stream.WriteBits((const unsigned char*)&actor,16,true);
    stream.WriteBits((const unsigned char*)&damage,32,true);
    stream.WriteBits((const unsigned char*)&weapon,32,true);
    stream.WriteBits((const unsigned char*)&bodyPart,32,true);
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(&g_localActorDamageRpc,&stream,HIGH_PRIORITY,RELIABLE_ORDERED,0,false);
}
