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
#include "game/player_colors.h"
#include "game/camera.h"
#include "game/custommodels.h"
#include "gui/render_dependencies.h"
#include "gui/textdraw_texture.h"
// samp.dll RVA: 0x00117880
int g_localSpawnRpc=52;
// samp.dll RVA: 0x0013c0a0
DWORD g_localSkipInitialState=1;
// samp.dll RVA: 0x00003110
void CLocalPlayer::SetSpecialAction(BYTE action) {
    switch(action) {
    case 0:
        if(GetPed()->DoesDancing())GetPed()->StopDancing();
        if(GetPed()->UsingCellphone())GetPed()->EnableCellphone(FALSE);
        if(GetPed()->DoesUrinating())GetPed()->StopUrinating();
        if(GetPed()->GetStuff())GetPed()->DropStuff();
        if(GetPed()->HasJetpack())GetPed()->DisableJetpack();
        if(GetPed()->HasDanceState())GetPed()->SetDancePoseState(0);
        if(GetPed()->HasDanceStyle())GetPed()->SetSecondaryPoseState(0);
        break;
    case 2:if(!GetPed()->HasJetpack())GetPed()->EnableJetpack();break;
    case 5:case 6:case 7:case 8:GetPed()->StartDancing(action-5);break;
    case 10:GetPed()->HandsUp();break;
    case 11:if(!GetPed()->IsInVehicle())GetPed()->EnableCellphone(TRUE);break;
    case 13:if(GetPed()->UsingCellphone())GetPed()->EnableCellphone(FALSE);break;
    case 20:if(!GetPed()->IsInVehicle())GetPed()->GiveStuff(1);break;
    case 21:if(!GetPed()->IsInVehicle())GetPed()->GiveStuff(4);break;
    case 22:if(!GetPed()->IsInVehicle())GetPed()->GiveStuff(2);break;
    case 23:if(!GetPed()->IsInVehicle())GetPed()->GiveStuff(3);break;
    case 24:if(!GetPed()->HasDanceState())GetPed()->SetDancePoseState(1);break;
    case 25:if(!GetPed()->HasDanceStyle())GetPed()->SetSecondaryPoseState(1);break;
    case 68:if(!GetPed()->IsInVehicle())GetPed()->StartUrinating();break;
    }
}
// samp.dll RVA: 0x00003b20
BOOL CLocalPlayer::Spawn() {
    BYTE *state=(BYTE*)this;if(!*(DWORD*)(state+0x181))return FALSE;
    if(*(DWORD*)(state+0x30a)==1) {
        *(DWORD*)(state+0x30a)=0;
        if(g_renderSecondDialog)*((BYTE*)g_renderSecondDialog+0x13)=0;
        g_pGame->SetCursorMode(0,FALSE);
    }
    CCamera *camera=g_pGame->m_pCamera;camera->Detach();camera->Restore();g_pGame->EnableHUD(TRUE);
    GetPed()->SetControllable(TRUE);
    if(!g_localSkipInitialState)GetPed()->SetInitialState();else g_localSkipInitialState=0;
    g_pGame->RefreshRenderer(*(float*)(state+0x159),*(float*)(state+0x15d));
    if(GetPed()->HasDanceState())GetPed()->SetDancePoseState(0);
    GetPed()->SetSpawnInfo((CVector*)(state+0x159),*(float*)(state+0x165));
    if(g_customModels&&*(int*)(state+0x154)>20000) {
        if(GameIsPedModel(*(int*)(state+0x154)))GetPed()->SetModelIndex(*(int*)(state+0x154));
        else if(g_customModels->DoesExist(*(int*)(state+0x154)))GetPed()->SetModelIndex(*(int*)(state+0x154));
        else GetPed()->SetModelIndex(0);
    } else GetPed()->SetModelIndex(*(int*)(state+0x150));
    GetPed()->ClearWeapons();GetPed()->ResetDamageEntity();SetSpecialAction(0);
    int weapon=*(int*)(state+0x171);
    *(DWORD*)(state+0x2e6)=0;*(DWORD*)(state+0x2ea)=0;*(DWORD*)(state+0x2ee)=0;*(DWORD*)(state+0x2f2)=0;
    *(WORD*)(state+0x2d8)=0xffff;*(DWORD*)(state+0x2de)=0;*(DWORD*)(state+0x2e2)=0;
    if(weapon!=-1)GetPed()->GiveWeapon(weapon,*(int*)(state+0x17d));
    weapon=*(int*)(state+0x16d);if(weapon!=-1)GetPed()->GiveWeapon(weapon,*(int*)(state+0x179));
    weapon=*(int*)(state+0x169);if(weapon!=-1)GetPed()->GiveWeapon(weapon,*(int*)(state+0x175));
    g_pGame->DisableTrainTraffic();GameUnprotect((void*)0x50ac20,3);
    // GTA SA virtual addresses: native three-byte permanent RET8 patch.
    *(BYTE*)0x50ac20=0xc2;*(BYTE*)0x50ac21=8;*(BYTE*)0x50ac22=0;
    CVector position={*(float*)(state+0x159),*(float*)(state+0x15d),*(float*)(state+0x161)+g_textDrawHalf};
    GetPed()->Teleport(position);GetPed()->SetRotation(*(float*)(state+0x165));
    *(DWORD*)(state+0xf8)=0;*(DWORD*)(state+0xf4)=1;*(DWORD*)(state+0x306)=0;
    RakNet::BitStream stream;
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(&g_localSpawnRpc,&stream,HIGH_PRIORITY,RELIABLE_ORDERED,0,false);
    *(DWORD*)(state+0x30e)=GetTickCount()+1000;return TRUE;
}
// samp.dll RVA: 0x00004080
void CLocalPlayer::ToggleSpectating(BOOL enabled) {
    BYTE *state=(BYTE*)this;
    if(*(DWORD*)(state+0x12c)&&!*(DWORD*)(state+0x2fe)) {if(!enabled)Spawn();}
    else if(*(DWORD*)(state+0x2fe)&&!enabled){*(DWORD*)(state+0xf4)=0;*(DWORD*)(state+0x181)=0;}
    *(DWORD*)(state+0x12c)=enabled;state[0x313]=0;*(DWORD*)(state+0x318)=0;*(DWORD*)(state+0x314)=0xffffffff;
}
// samp.dll RVA: 0x000040f0
void CLocalPlayer::SpectateVehicle(WORD id) {
    CVehiclePool *pool=*(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0xc);
    if(pool&&id<2000&&pool->m_bNotEmpty[id]){nativeState[0x313]=2;*(DWORD*)(nativeState+0x314)=id;*(DWORD*)(nativeState+0x318)=0;}
}
// samp.dll RVA: 0x00004140
void CLocalPlayer::SpectatePlayer(WORD id) {
    CPlayerPool *pool=*(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+8);
    if(!pool||id>=1004||!pool->connected[id])return;
    CRemotePlayer *remote=pool->GetPlayer(id);
    BYTE state=*((BYTE*)remote+0x1a);
    if(state&&*((BYTE*)pool->GetPlayer(id)+0x1a)!=32){nativeState[0x313]=1;*(DWORD*)(nativeState+0x314)=id;*(DWORD*)(nativeState+0x318)=0;}
}
// samp.dll RVA: 0x00003de0
void CLocalPlayer::SetColor(DWORD color) {
    CPlayerPool *pool=*(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+8);PlayerColors::Set(*(WORD*)pool,color);
}
// samp.dll RVA: 0x00003e00
DWORD CLocalPlayer::GetColorAsRGBA() {
    CPlayerPool *pool=*(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+8);return PlayerColors::Get(*(WORD*)pool);
}
// samp.dll RVA: 0x00002ec0
void CLocalPlayer::ResetData() {
    BYTE *state=(BYTE*)this;
    *(DWORD*)(state+0x181)=0;*(DWORD*)(state+0x2fe)=0;
    *(DWORD*)(state+0x306)=0;*(DWORD*)(state+0x302)=0;state[0x1d3]=0;
    *(WORD*)(state+0xfe)=0xffff;*(DWORD*)(state+0x1d4)=0;*(WORD*)(state+0x31c)=0xffff;
    *(WORD*)(state+0x108)=0xffff;*(WORD*)(state+0x10a)=0xffff;*(WORD*)(state+0x10c)=0xffff;
    memset(state+0x114,0,12);memset(state+0xd5,0,31);
    memset(state+4,0,54);memset(state+0x3a,0,68);memset(state+0x96,0,63);
    DWORD (WINAPI *tick)()=GetTickCount;
    memset(state+0x7e,0,24);*(DWORD*)(state+0x100)=0;*(DWORD*)(state+0x104)=0;
    *(WORD*)(state+0x189)=0xffff;*(WORD*)(state+0x18b)=0xffff;
    *(DWORD*)(state+0x185)=tick();state[0x18d]=0;
    DWORD last=tick();*(DWORD*)(state+0x2de)=0;*(DWORD*)(state+0x2e2)=0;*(DWORD*)(state+0x110)=last;
}
// samp.dll RVA: 0x00004af0
CLocalPlayer::CLocalPlayer() {
    BYTE *state=(BYTE*)this;DWORD (WINAPI *tick)()=GetTickCount;
    *(DWORD*)(state+0x147)=tick();*(DWORD*)(state+0x120)=tick();*(DWORD*)(state+0x124)=tick();
    *(DWORD*)(state+0x1d4)=0;*(DWORD*)(state+0x2e6)=0;*(DWORD*)(state+0x2da)=tick();
    *(DWORD*)(state+0x2fa)=0;*(CPed**)state=g_pGame->GetPlayerPed();
    *(DWORD*)(state+0xf4)=0;*(DWORD*)(state+0xf8)=0;*(WORD*)(state+0x131)=0;*(DWORD*)(state+0x30e)=0;
    *(DWORD*)(state+0x133)=tick();*(DWORD*)(state+0x137)=tick();*(DWORD*)(state+0x128)=tick();
    *(DWORD*)(state+0x13b)=*(DWORD*)(state+0x133);*(DWORD*)(state+0x13f)=*(DWORD*)(state+0x133);
    *(DWORD*)(state+0x12c)=0;state[0x313]=0;*(DWORD*)(state+0x314)=0xffffffff;*(DWORD*)(state+0x30a)=0;
    ResetData();
    for(int slot=0;slot<13;slot++){state[0x18e+slot]=0;*(DWORD*)(state+0x19b+slot*4)=0;}
    state[0x130]=0xff;
}
// samp.dll RVA: 0x00002da0
CPed *CLocalPlayer::GetPed() { return *(CPed**)this; }
// samp.dll RVA: 0x00003e20
DWORD CLocalPlayer::GetColorAsARGB() {
    CPlayerPool *players=*(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+8);
    return (PlayerColors::Get(*(WORD*)players)>>8)|0xff000000;
}
// The selected RakNet dependency uses SA-MP's shifted reliability enumeration.
typedef char LocalRpcReliability[(HIGH_PRIORITY==1 && RELIABLE_ORDERED==9 && RELIABLE_SEQUENCED==10)?1:-1];
// samp.dll RVA: 0x0011787c
int g_localSpawnRequestRpc=129;
// samp.dll RVA: 0x00117954
int g_localRequestClassRpc=128;
// samp.dll RVA: 0x00117958
int g_localInteriorChangeRpc=118;
// samp.dll RVA: 0x0011795c
int g_localChatRpc=101;
// samp.dll RVA: 0x00003970
DWORD CLocalPlayer::HasDataChanged(const void *previous,const void *current,DWORD length) {
    BYTE *state=(BYTE*)this;
    if(GetTickCount()-*(DWORD*)(state+0x128)>500||memcmp(previous,current,length)) {
        *(DWORD*)(state+0x128)=GetTickCount();return 1;
    }
    return 0;
}
// samp.dll RVA: 0x00003650
void CLocalPlayer::ProcessSurfing() {
    BYTE *state=(BYTE*)this;
    void *floor=(*(CPed**)state)->GetFloor();
    BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);
    CVehiclePool *vehicles=*(CVehiclePool**)(pools+0xc);
    CObjectPool *objects=*(CObjectPool**)(pools+0x14);
    WORD vehicleId=0xffff,objectId=0xffff;int mode=0;
    if(floor) {
        vehicleId=vehicles->Find(floor);
        if(vehicleId&&vehicleId!=0xffff) {
            if(vehicleId<2000&&vehicles->m_bNotEmpty[vehicleId]) {
                CVehicle *vehicle=vehicles->m_pObjects[vehicleId];
                if(vehicle&&(vehicle->HasDriver()||vehicle->GetModelIndex()==569||vehicle->GetModelIndex()==570)) {
                    BYTE nearby;
                    __asm {
                        mov ecx,vehicle
                        call CEntity::GetDistanceToLocalPlayer
                        fcomp g_objectStreamingVehicleMargin
                        fnstsw ax
                        test ah,5
                        setnp nearby
                    }
                    if(nearby)mode=1;
                }
            }
        } else {
            objectId=objects->GetId(floor);
            if(objectId&&objectId!=0xffff) {
                CObject *object=(CObject*)objects->Get(objectId);
                if(object&&object->IsMoving())mode=2;
            }
        }
    }
    if((*(CPed**)state)->HasTaskType208()||(*(CPed**)state)->HasTaskType704()||(*(CPed**)state)->GetVehicleTransitionTaskState()) {
        *(DWORD*)(state+0x2fa)=0;*(DWORD*)(state+0x2de)=0;*(DWORD*)(state+0x2e2)=0;return;
    }
    if(!mode) {
        if(!*(DWORD*)(state+0x2e6)) {
            *(DWORD*)(state+0x2fa)=0;*(DWORD*)(state+0x2de)=0;*(DWORD*)(state+0x2e2)=0;return;
        }
        if(*(DWORD*)(state+0x2e6)==1&&GetTickCount()-*(DWORD*)(state+0x2da)>=2000) {
            *(DWORD*)(state+0x2e6)=0;
            memset(state+0x2ea,0,12);*(WORD*)(state+0x2d8)=0xffff;
            *(DWORD*)(state+0x2de)=0;*(DWORD*)(state+0x2e2)=0;*(DWORD*)(state+0x2fa)=0;
        }
        if(*(DWORD*)(state+0x2e6)==1&&GetTickCount()-*(DWORD*)(state+0x2da)<2000) {
            WORD id=*(WORD*)(state+0x2d8);
            CEntity *entity=id<2000?(CEntity*)vehicles->Get(id):objects->Get((WORD)(id-2000));
            if(entity){*(CEntity**)(state+0x2de)=entity;*(DWORD*)(state+0x2e2)=1;}
            *(DWORD*)(state+0x2fa)=1;
        } else *(DWORD*)(state+0x2fa)=0xffffffff;
        return;
    }
    if(mode==1) {
        *(WORD*)(state+0x2d8)=vehicleId;*(DWORD*)(state+0x2da)=GetTickCount();
        CEntity *entity=vehicles->Get(*(WORD*)(state+0x2d8));
        if(!*(DWORD*)(state+0x2e6)) {
            *(CEntity**)(state+0x2de)=entity;*(DWORD*)(state+0x2e2)=0;ApplySurfingPhysics();
        }
        *(CEntity**)(state+0x2de)=entity;*(DWORD*)(state+0x2e6)=1;*(DWORD*)(state+0x2e2)=1;
    } else if(mode==2) {
        *(WORD*)(state+0x2d8)=(WORD)(objectId+2000);*(DWORD*)(state+0x2da)=GetTickCount();
        CEntity *entity=objects->Get((WORD)(*(WORD*)(state+0x2d8)-2000));
        if(entity) {
            if(!*(DWORD*)(state+0x2e6)) {
                *(CEntity**)(state+0x2de)=entity;*(DWORD*)(state+0x2e2)=0;ApplySurfingPhysics();
            }
            *(CEntity**)(state+0x2de)=entity;*(DWORD*)(state+0x2e6)=1;*(DWORD*)(state+0x2e2)=1;
        }
    }
    *(DWORD*)(state+0x2fa)=2;
}
// samp.dll RVA: 0x00003a70
void CLocalPlayer::SendSpawnRequest() {
    RakNet::BitStream stream;
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(&g_localSpawnRequestRpc,&stream,HIGH_PRIORITY,RELIABLE,0,false);
}
// samp.dll RVA: 0x00003f40
void CLocalPlayer::RequestSpawn() {
    BYTE *state=(BYTE*)this;
    if(*(DWORD*)(state+0x30a)){SendSpawnRequest();*(DWORD*)(state+0x306)=1;}
}
// samp.dll RVA: 0x00005720
void CLocalPlayer::RequestClass(int selection) {
    RakNet::BitStream stream;stream.WriteBits((const unsigned char*)&selection,32,true);
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(&g_localRequestClassRpc,&stream,HIGH_PRIORITY,RELIABLE,0,false);
}
// samp.dll RVA: 0x000057c0
void CLocalPlayer::SendInteriorChange(BYTE interior) {
    ((BYTE*)this)[0x1d3]=interior;
    RakNet::BitStream stream;stream.WriteBits(&interior,8,true);
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(&g_localInteriorChangeRpc,&stream,HIGH_PRIORITY,RELIABLE,0,false);
}
// samp.dll RVA: 0x00005860
void CLocalPlayer::Chat(char *text) {
    BYTE length=(BYTE)strlen(text);
    RakNet::BitStream stream;stream.WriteBits(&length,8,true);stream.Write(text,length);
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(&g_localChatRpc,&stream,HIGH_PRIORITY,RELIABLE,0,false);
}
// samp.dll RVA: 0x00005e30
void CLocalPlayer::NextClass() {
    BYTE *state=(BYTE*)this;
    if(!*(DWORD*)(state+0x30a))return;
    *(DWORD*)(state+0x143)=0;
    CMatrix matrix;(*(CPed**)state)->GetMatrix(&matrix);
    BYTE *settings=*(BYTE**)((BYTE*)g_pNetGame+0x3d5);
    DWORD selection=*(DWORD*)(state+0x302);
    if(selection==*(DWORD*)(settings+0x23)-1)selection=0;else selection++;
    *(DWORD*)(state+0x302)=selection;
    g_pGame->m_pAudio->Play(1052,matrix.position);
    RequestClass(*(int*)(state+0x302));
}
// samp.dll RVA: 0x00005ec0
void CLocalPlayer::PrevClass() {
    BYTE *state=(BYTE*)this;
    if(!*(DWORD*)(state+0x30a))return;
    *(DWORD*)(state+0x143)=0;
    CMatrix matrix;(*(CPed**)state)->GetMatrix(&matrix);
    DWORD selection=*(DWORD*)(state+0x302);
    if(!selection)selection=*(DWORD*)(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x23);
    *(DWORD*)(state+0x302)=selection-1;
    g_pGame->m_pAudio->Play(1053,matrix.position);
    RequestClass(*(int*)(state+0x302));
}
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

// samp.dll RVA: 0x0013c0a8
DWORD g_localOnFootSendRate=30;
// samp.dll RVA: 0x0013c0ac
DWORD g_localInCarSendRate=30;
// samp.dll RVA: 0x000039c0
DWORD CLocalPlayer::GetInCarSendRate() {
    if(!*(CPed**)this)return 1000;
    if(*(DWORD*)((BYTE*)g_pNetGame+0x239))return 15;
    return g_pGame->CountUsedPlayerSlots()+g_localInCarSendRate;
}
// samp.dll RVA: 0x00003a00
DWORD CLocalPlayer::GetOnFootSendRate() {
    if(!*(CPed**)this)return 1000;
    if(*(DWORD*)((BYTE*)g_pNetGame+0x239))return 15;
    return g_pGame->CountUsedPlayerSlots()+g_localOnFootSendRate;
}
// samp.dll RVA: 0x00003a40
DWORD CLocalPlayer::GetSendRate() {
    CPed *ped=*(CPed**)this;
    if(!ped)return 1000;
    return ped->IsInVehicle()?GetInCarSendRate():GetOnFootSendRate();
}
