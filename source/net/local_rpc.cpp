#include "net/local_rpc.h"
#include "net/localplayer.h"
#include "net/vehiclepool.h"
#include "net/playerpool.h"
#include "net/objectpool.h"
#include "game/util.h"
#include <string.h>
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

// samp.dll RVA: 0x000069d0
void CLocalPlayer::SendBullet(PedBulletData *data) {
    CPed *ped=*(CPed**)this;
    if(!ped)return;
    if((BYTE)ped->GetCurrentWeapon()==34&&!data->entity)return;
    BYTE type=0;WORD id=0xffff;
    if(data->entity) {
        BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);
        CPlayerPool *players=*(CPlayerPool**)(pools+8);
        CVehiclePool *vehicles=*(CVehiclePool**)(pools+0xc);
        CObjectPool *objects=*(CObjectPool**)(pools+0x14);
        // Native requires player and vehicle pools, but does not check objects.
        if(players&&vehicles) {
            id=players->Find(data->entity);
            if(id!=0xffff)type=1;
            else {
                id=vehicles->Find(data->entity);
                if(id!=0xffff)type=2;
                else {
                    id=objects->GetId(data->entity);
                    if(id!=0xffff)type=3;
                    else {
                        BYTE *backend=(BYTE*)data->entity;
                        CMatrix *matrix=*(CMatrix**)(backend+0x14);
                        if(matrix) {
                            CVector world={0,0,0};
                            GameTransformPoint(&world,matrix,&data->offset);
                            memcpy(&data->offset,&world,12);
                        } else {
                            data->offset.x=*(float*)(backend+4)+data->offset.x;
                            data->offset.y=*(float*)(backend+8)+data->offset.y;
                            data->offset.z=*(float*)(backend+0xc)+data->offset.z;
                        }
                    }
                }
            }
        }
    }
    BYTE payload[40]={0};
    payload[0]=type;memcpy(payload+1,&id,2);
    memcpy(payload+3,&data->origin,12);memcpy(payload+15,&data->target,12);
    memcpy(payload+27,&data->offset,12);
    payload[39]=(BYTE)(*(CPed**)this)->GetCurrentWeapon();
    RakNet::BitStream stream;
    BYTE packet=206;stream.WriteBits(&packet,8,true);
    stream.Write((const char*)payload,40);
    typedef char BulletReliability[(HIGH_PRIORITY==1&&UNRELIABLE_SEQUENCED==7)?1:-1];
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->Send(&stream,HIGH_PRIORITY,UNRELIABLE_SEQUENCED,0);
}

#include "game/keystuff.h"
// Shared native surfing work buffers; matrix.position aliases are not new globals.
// samp.dll RVA: 0x001569a8
CVector g_localSurfPedTurnSpeed;
// samp.dll RVA: 0x001569b8
CMatrix g_localSurfPedMatrix;
// samp.dll RVA: 0x001569f8
CVector g_localSurfEntitySpeed;
// samp.dll RVA: 0x00156a08
CMatrix g_localSurfInverseMatrix;
// samp.dll RVA: 0x00156a48
CVector g_localSurfPedSpeed;
// samp.dll RVA: 0x00156a58
CMatrix g_localSurfEntityMatrix;
// samp.dll RVA: 0x00156aa4
CVector g_localSurfEntityTurnSpeed;

// samp.dll RVA: 0x000034b0
void CLocalPlayer::ApplySurfingPhysics() {
    BYTE *state=(BYTE*)this;
    if(!*(CEntity**)(state+0x2de))return;
    (*(CEntity**)(state+0x2de))->GetMatrix(&g_localSurfEntityMatrix);
    (*(CPed**)state)->GetMatrix(&g_localSurfPedMatrix);
    (*(CEntity**)(state+0x2de))->GetSpeed(&g_localSurfEntitySpeed);
    (*(CEntity**)(state+0x2de))->GetTurnSpeed(&g_localSurfEntityTurnSpeed);
    WORD leftRight,upDown;
    (*(CPed**)state)->GetKeys(&leftRight,&upDown);
    KeyStuff::GetInternalKeys();
    if(*(DWORD*)(state+0x2e2)&&!leftRight&&!upDown&&!(*(CPed**)state)->HasTaskType211()) {
        GameTransformPoint(&g_localSurfPedMatrix.position,&g_localSurfEntityMatrix,(CVector*)(state+0x2ea));
        (*(CPed**)state)->ApplyMatrix(g_localSurfPedMatrix);
        (*(CPed**)state)->GetSpeed(&g_localSurfPedSpeed);
        memcpy(&g_localSurfPedSpeed.x,&g_localSurfEntitySpeed.x,8);
        (*(CPed**)state)->SetSpeed(g_localSurfPedSpeed);
        (*(CPed**)state)->GetTurnSpeed(&g_localSurfPedTurnSpeed);
        memcpy(&g_localSurfPedTurnSpeed.z,&g_localSurfEntityTurnSpeed.z,4);
        (*(CPed**)state)->SetTurnSpeed(g_localSurfPedTurnSpeed);
    } else {
        GameInvertRwMatrix(&g_localSurfInverseMatrix,&g_localSurfEntityMatrix);
        GameTransformPoint((CVector*)(state+0x2ea),&g_localSurfInverseMatrix,&g_localSurfPedMatrix.position);
    }
}
// samp.dll RVA: 0x00003630
void CLocalPlayer::SetSurfingEntity(CEntity *entity,DWORD locked) {
    *(CEntity**)((BYTE*)this+0x2de)=entity;
    *(DWORD*)((BYTE*)this+0x2e2)=locked;
}
