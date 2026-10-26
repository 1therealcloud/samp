#include "net/netrpc.h"
#include "RakClientInterface.h"
#include "events.h"
#include "net/local_rpc.h"
#include "net/netgame.h"
#include "objectedit.h"
#include "objectselection.h"
#include "BitStream.h"
#include "main.h"
#include "net/pickuppool.h"
#include "net/playerpool.h"
#include "net/localplayer.h"
#include "net/remoteplayer.h"
#include "net/playerinfo.h"
#include "game/playerped.h"
#include "gui/render_dependencies.h"
#include "svrnetstats.h"
#include "chatwindow.h"
#include "StringCompressor.h"
#include <string.h>
#include "net/vehiclepool.h"
#include "net/actorpool.h"
#include "gui/device_lifecycle.h"
#include "scoreboard.h"
#include "game/vehicle.h"
#include "GetTime.h"
#include "game/game.h"
#include "game/patches.h"
#include "game/custommodels.h"
// samp.dll RVA: 0x00011f80
void ScrCustomModelDownload(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    char url[257]={0};BYTE status=0,type=0,urlLength=0,stage=0;
    DWORD hash=0,unpackedLength=0;int length=0;BYTE chunk[4096];
    stream.ReadBits(&status,8,true);
    if(status==0) {
        g_customModels->ResumeReadyWorker();g_customModels->AbortDownload(0,6001,0);
    }else if(status==2)g_customModels->FinishDownload(0);
    else if(status==6) {
        stream.ReadBits(&type,8,true);stream.ReadBits((BYTE*)&hash,32,true);stream.ReadBits(&urlLength,8,true);
        stream.Read(url,(int)urlLength);
        g_customModels->ResumeReadyWorker();g_customModels->BeginUrlDownload(type,hash,url);
    }else if(status==5) {
        stream.ReadBits(&type,8,true);stream.ReadBits((BYTE*)&hash,32,true);
        g_customModels->ResumeReadyWorker();g_customModels->BeginDirectDownload(type,hash);
    }else if(status==1) {
        stream.ReadBits(&stage,8,true);stream.ReadBits((BYTE*)&unpackedLength,32,true);stream.ReadBits((BYTE*)&length,32,true);
        if(length<0||length>4096)return;
        stream.Read((char*)chunk,length);g_customModels->ProcessCompressedChunk(stage,unpackedLength,chunk,(DWORD)length);
    }
}
// samp.dll RVA: 0x00011d80
void ScrCustomModelRecord(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE record[94]={0};DWORD index=0,count=0;
    if(!g_customModels) {
        CCustomModels *models=new CCustomModels;g_customModels=models;models->Initialize();if(!g_customModels)return;
    }
    stream.ReadBits((BYTE*)&index,32,true);stream.ReadBits((BYTE*)&count,32,true);
    stream.ReadBits(record+7,8,true);stream.ReadBits(record+8,32,true);stream.ReadBits(record+0xc,32,true);stream.ReadBits(record+0x10,32,true);
    stream.ReadBits(record+0x46,32,true);stream.ReadBits(record+0x4a,32,true);stream.ReadBits(record+0x4e,32,true);stream.ReadBits(record+0x52,32,true);
    stream.ReadBits(record+0x56,8,true);stream.ReadBits(record+0x57,8,true);
    g_customModels->AddRecord(index,record);g_customModels->SetExpectedRecordCount(count);
}
// samp.dll RVA: 0x0000ea40
void ScrCustomModelsReset(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CCustomModels *models=g_customModels;if(models){models->Initialize();g_customModels->DeleteRecord(0);}
}
// samp.dll RVA: 0x0000ead0
void ScrCustomModelsComplete(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CCustomModels *models=g_customModels;if(models){models->nativeState[0x221]=1;models->StopWorkers();}
}
// samp.dll RVA: 0x0013c0b4
DWORD g_rpcInitValue13C0B4=2;
// samp.dll RVA: 0x001180ec
const char g_rpcConnectedFormat[]="Connected to {B9C9BF}%.64s";
// samp.dll RVA: 0x00010530
void ScrInitGame(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CPlayerPool *players=((CNetGame*)g_pNetGame)->pools->players;
    DWORD mode=0;WORD player;BYTE stunt,holdTime,length,models[212];
    stream.Read(*(bool*)(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x21));
    stream.Read(*(bool*)*(BYTE**)((BYTE*)g_pNetGame+0x3d5));
    stream.Read(*(bool*)(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x15));
    stream.Read(*(bool*)(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x39));
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x34,32,true);
    stream.Read(*(bool*)&stunt);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x27,32,true);
    stream.Read(*(bool*)(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x1a));
    stream.Read(*(bool*)(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x2f));
    (*(BYTE**)((BYTE*)g_pNetGame+0x3d5))[0x2f]=1;
    stream.Read(*(bool*)(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x2b));
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x23,32,true);
    stream.ReadBits((BYTE*)&player,16,true);
    stream.Read(*(bool*)(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x38));
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x30,32,true);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x2c,8,true);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x2e,8,true);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x16,32,true);
    stream.Read(*(bool*)&holdTime);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+1,32,true);
    stream.Read(*(bool*)(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x20));
    stream.ReadBits((BYTE*)&g_localOnFootSendRate,32,true);stream.ReadBits((BYTE*)&g_localInCarSendRate,32,true);
    stream.ReadBits((BYTE*)&g_localAimSendRate,32,true);stream.ReadBits((BYTE*)&g_rpcInitValue13C0B4,32,true);
    stream.ReadBits((BYTE*)&mode,32,true);
    if(mode==1||mode==2) {
        g_netStatsShotCompMode=mode==1?0:1;GameInstallShotHooks();
        if(g_pGame)g_pGame->DisableAutoAiming();
    }else g_netStatsShotCompMode=2;
    stream.ReadBits(&length,8,true);
    if(length){memset((BYTE*)g_pNetGame+0x131,0,257);stream.Read((char*)g_pNetGame+0x131,length);}
    *((BYTE*)g_pNetGame+0x131+length)=0;
    stream.Read((char*)models,212);g_pGame->SetRequiredVehicleModels(models);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x1b,32,true);
    players->localId=player;g_pGame->EnableStuntBonus(stunt);
    if(holdTime)*(DWORD*)((BYTE*)g_pNetGame+0x239)=1;
    ((CNetGame*)g_pNetGame)->InitializeGameLogic();
    g_pGame->SetWorldGravity(*(float*)(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x16));
    if((*(BYTE**)((BYTE*)g_pNetGame+0x3d5))[0x1a])g_pGame->DisableEnterExits();
    if(*(DWORD*)(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x1b))GameInstallVehicleDamageFilter();
    g_pGame->SetWorldWeather((*(BYTE**)((BYTE*)g_pNetGame+0x3d5))[0x2e]);
    g_pChat->AddDebugMessage(g_rpcConnectedFormat,(char*)g_pNetGame+0x131);
    *(DWORD*)((BYTE*)g_pNetGame+0x3cd)=5;players->local->PrepareForClassSelection();
}
// samp.dll RVA: 0x0000e4e0
void ScrGamemodeRestart(RPCParameters *parameters) {
    (void)parameters;((CNetGame*)g_pNetGame)->ShutdownForRestart();
}
// samp.dll RVA: 0x00118108
const char g_rpcCouldNotSpawnPlayer[]="Warning: Couldn't spawn player(%u)";
// samp.dll RVA: 0x000109d0
void ScrWorldPlayerAdd(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;
    WORD id;BYTE team=0,style=4;int model=0,customModel=0;CVector position;float rotation=0;DWORD color=0;WORD skills[11];
    stream.Read(id);stream.Read(team);stream.Read(model);stream.Read(customModel);
    stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);stream.Read(rotation);stream.Read(color);stream.Read(style);stream.Read((char*)skills,22);
    if(!pool||id>=1004||!pool->connected[id]||!pool->players[id]||!pool->players[id]->remote)return;
    CRemotePlayer *remote=pool->players[id]->remote;
    if(!remote->Spawn(team,model,customModel,&position,rotation,color,style)) {
        if(g_pChat)g_pChat->AddDebugMessage(g_rpcCouldNotSpawnPlayer,(unsigned)id);return;
    }
    CPed *ped=*(CPed**)(remote->nativeState+4);if(ped)for(int i=0;i<11;++i)ped->SetWeaponSkill(i,skills[i]);
    if(team!=0xff)remote->nativeState[0x19]=team;
    if(pool->GetPlayerInfoFlag(id)) {ped=*(CPed**)(remote->nativeState+4);if(ped)ped->SetImmunities(0,1,1,0,0);}
}
// samp.dll RVA: 0x0000e500
void ScrWorldVehicleAdd(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CVehiclePool *pool=((CNetGame*)g_pNetGame)->pools->vehicles;if(!pool)return;
    VehiclePoolQueueEntry entry={0};BYTE tuning[23]={0};
    stream.Read((char*)&entry,40);stream.Read((char*)tuning,23);RakNet::GetTimeNS();
    if(entry.model<400||entry.model>611)return;
    pool->Create(&entry);CVehicle *vehicle=pool->Get(entry.id);if(!vehicle)return;
    if(entry.model==537||entry.model==538) {
        entry.model=entry.model==537?569:570;
        ++entry.id;pool->Create(&entry);++entry.id;pool->Create(&entry);++entry.id;pool->Create(&entry);
    }
    if(vehicle->GetSubtype()==1) {
        for(int i=0;i<14;++i)if(tuning[i])vehicle->AddComponent((WORD)(tuning[i]+999));
        if(tuning[14])vehicle->SetPaintjob((BYTE)(tuning[14]-1));
    }
    int first=*(int*)(tuning+15),second=*(int*)(tuning+19);
    if(first!=-1||second!=-1)vehicle->SetColor((BYTE)first,(BYTE)second);
    RakNet::GetTimeNS();
}
// samp.dll RVA: 0x00010dc0
void ScrWorldVehicleRemove(RPCParameters *parameters) {
    CVehiclePool *vehicles=((CNetGame*)g_pNetGame)->pools->vehicles;
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    if(!vehicles)return;
    WORD id;stream.Read(id);CVehicle *vehicle=vehicles->Get(id);
    CPlayerPool *players=((CNetGame*)g_pNetGame)->pools->players;
    if(!vehicle||!players)return;
    CLocalPlayer *local=players->local;
    if(g_pGame->GetPlayerPed()->IsInVehicle()) {
        if(vehicles->Find(g_pGame->GetPlayerPed()->GetVehicle())==id) {
            CMatrix matrix;g_pGame->GetPlayerPed()->GetMatrix(&matrix);
            g_pGame->GetPlayerPed()->WarpFromVehicle(matrix.position.x,matrix.position.y,matrix.position.z);
            g_clientSkipGameFrame=1;
        }
    }
    if(local) {
        CEntity *surfing=*(CEntity**)(local->nativeState+0x2de);
        if(surfing==vehicles->Get(id))local->ClearSurfingState();
    }
    for(WORD player=0;player<1004;++player)if(players->connected[player]) {
        CRemotePlayer *remote=players->players[player]?players->players[player]->remote:0;
        if(*(CPed**)(remote->nativeState+4)&&remote->nativeState[0x1a]&&*(CVehicle**)(remote->nativeState+8)==vehicle)remote->OnExitVehicle();
    }
    for(WORD other=1;other<2000;++other)if(vehicles->m_bNotEmpty[other]&&other!=id) {
        CVehicle *car=vehicles->Get(other);
        if(car->m_pTrailer==vehicle){vehicles->Get(other)->DetachTrailer();vehicles->Get(other)->SetTrailer(0);}
    }
    int model=vehicles->Get(id)->GetModelIndex();vehicles->Delete(id);
    if(model==537||model==538){vehicles->Delete((WORD)(id+1));vehicles->Delete((WORD)(id+2));vehicles->Delete((WORD)(id+3));}
}
// samp.dll RVA: 0x0000f150
void ScmEvent(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD player;DWORD eventType,vehicle,param2,param3;
    stream.Read(player);stream.Read(eventType);stream.Read(vehicle);stream.Read(param2);stream.Read(param3);
    GameProcessIncomingEvent(player,eventType,(WORD)vehicle,param2,param3);
}
// samp.dll RVA: 0x0000e910
BYTE ClientCheckChecksum(const void *data,WORD length) {
    BYTE result=0;const BYTE *bytes=(const BYTE*)data;
    for(unsigned i=0;i<length;++i)result^=bytes[i]&0xcc;
    return result;
}
// samp.dll RVA: 0x00011860
void SendClientCheckResponse(BYTE type,DWORD argument,BYTE result) {
    RakNet::BitStream stream;stream.Write(type);stream.Write(argument);stream.Write(result);
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(const_cast<int*>(&g_rpcClientCheck),&stream,HIGH_PRIORITY,RELIABLE_ORDERED,0,false);
}
// samp.dll RVA: 0x00011930
void ClientCheck(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE type=0,result=0;DWORD argument=0,offset=0,length=0;
    stream.Read(type);stream.Read(argument);
    stream.ReadBits((unsigned char*)&offset,16,true);stream.ReadBits((unsigned char*)&length,16,true);
    if((WORD)offset>256||(WORD)length>256||(WORD)length<2)return;
    if(type==0x46) {
        if(GameHasModelInfo(argument)) {
            void *info=GameGetModelInfo(argument);
            if(info)result=ClientCheckChecksum((void*)((DWORD)info+(WORD)offset),(WORD)length);
        }
        SendClientCheckResponse(0x46,argument,result);
    }else if(type==2) {
        if(g_pGame->GetPlayerPed()) {
            if(g_pGame->GetPlayerPed()->IsInVehicle()&&!g_pGame->GetPlayerPed()->IsPassenger()) {
                BYTE *backend=*(BYTE**)((BYTE*)g_pGame->GetPlayerPed()+0x2a4);
                BYTE *vehicle=*(BYTE**)(backend+0x58c);
                if(vehicle){argument=*(DWORD*)(vehicle+0x40);result=2;}
            }else {
                BYTE *backend=*(BYTE**)((BYTE*)g_pGame->GetPlayerPed()+0x2a4);
                if(backend){argument=*(DWORD*)(backend+0x40);result=1;}
            }
        }
        SendClientCheckResponse(2,argument,result);
    }else if(type==0x47) {
        if(GameHasModelInfo(argument)) {
            int waits=0;
            if(!g_pGame->IsModelAvailable(argument)) {
                g_pGame->RequestModel(argument,2);g_pGame->LoadRequestedModels();
                while(!g_pGame->IsModelAvailable(argument)) {
                    if(++waits>1000)break;
                    Sleep(1);
                }
            }
            if(g_pGame->IsModelAvailable(argument)) {
                void *collision=GameGetModelCollision(argument);
                if(collision)result=ClientCheckChecksum((void*)((DWORD)collision+(WORD)offset),(WORD)length);
                if(!GameGetModelReferenceCount(argument))g_pGame->ReleaseModel(argument,0);
            }
        }
        SendClientCheckResponse(0x47,argument,result);
    }else if(type==5) {
        if(argument<0x400000||argument>0x856e00)return;
        if(argument)result=ClientCheckChecksum((void*)(argument+(WORD)offset),(WORD)length);
        SendClientCheckResponse(5,argument,result);
    }else if(type==0x45) {
        if(argument>800000)return;
        DWORD address=(DWORD)g_clientModule+argument;
        if(address)result=ClientCheckChecksum((void*)(address+(WORD)offset),(WORD)length);
        SendClientCheckResponse(0x45,argument,result);
    }else if(type==0x48) {
        SendClientCheckResponse(0x48,RakNet::GetTime(),0);
    }
}
// samp.dll RVA: 0x001180c4
const char g_rpcRejectedVersion[]="CONNECTION REJECTED: Incorrect Version.";
// samp.dll RVA: 0x00118098
const char g_rpcRejectedNickname[]="CONNECTION REJECTED: Unacceptable NickName";
// samp.dll RVA: 0x00118060
const char g_rpcNicknameLength[]="Please choose another nick between and 3-20 characters";
// samp.dll RVA: 0x00118040
const char g_rpcNicknameCharacters[]="Please use only a-z, A-Z, 0-9";
// samp.dll RVA: 0x0011800c
const char g_rpcNicknameQuit[]="Use /quit to exit or press ESC and select Quit Game";
// samp.dll RVA: 0x00117fe4
const char g_rpcRejectedMod[]="CONNECTION REJECTED: Bad mod version.";
// samp.dll RVA: 0x00117fac
const char g_rpcRejectedSlot[]="CONNECTION REJECTED: Unable to allocate a player slot.";
// samp.dll RVA: 0x0000fea0
void RequestClass(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;
    BYTE outcome=0;BYTE info[50];stream.Read(outcome);stream.Read((char*)info,50);
    if(outcome)local->SetSpawnInfo(info);
    local->PrepareForClassSelection_Outcome(outcome!=0);
}
// samp.dll RVA: 0x0000ff70
void RequestSpawn(RPCParameters *parameters) {
    BYTE outcome=0;
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    stream.Read(outcome);CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;
    if(outcome==2||(outcome&&*(DWORD*)(local->nativeState+0x306)))local->Spawn();
    else *(DWORD*)(local->nativeState+0x306)=0;
}
// samp.dll RVA: 0x00010410
void ConnectionRejected(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE reason;stream.Read(reason);
    if(reason==1)g_pChat->AddInfoMessage(g_rpcRejectedVersion);
    else if(reason==2){
        g_pChat->AddInfoMessage(g_rpcRejectedNickname);g_pChat->AddInfoMessage(g_rpcNicknameLength);
        g_pChat->AddInfoMessage(g_rpcNicknameCharacters);g_pChat->AddInfoMessage(g_rpcNicknameQuit);
    }else if(reason==3)g_pChat->AddInfoMessage(g_rpcRejectedMod);
    else if(reason==4)g_pChat->AddInfoMessage(g_rpcRejectedSlot);
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->Disconnect(500,0);
}
// samp.dll RVA: 0x000114a0
void EnterVehicle(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD player;DWORD vehicle=0;BYTE passenger=0;stream.Read(player);
    stream.ReadBits((unsigned char*)&vehicle,16,true);stream.Read(passenger);
    CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;
    if(player<=1004&&pool->players[player]&&pool->players[player]->remote){
        CRemotePlayer *remote=pool->players[player]->remote;
        if(remote->GetDistanceToLocalPlayer()<g_vehicleMarkerDistance)remote->EnterVehicle((WORD)vehicle,passenger!=0);
    }
}
// samp.dll RVA: 0x000115c0
void ExitVehicle(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD player;DWORD vehicle=0;stream.Read(player);stream.ReadBits((unsigned char*)&vehicle,16,true);
    CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;
    if(player<=1004&&pool->players[player]&&pool->players[player]->remote){
        CRemotePlayer *remote=pool->players[player]->remote;
        if(remote->GetDistanceToLocalPlayer()<g_vehicleMarkerDistance)remote->ExitVehicle();
    }
}
// samp.dll RVA: 0x000121c0
void Chat(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    if(*(DWORD*)((BYTE*)g_pNetGame+0x3cd)==5){
        char text[256]={0};WORD player;BYTE length;stream.Read(player);stream.Read(length);
        stream.Read(text,length);text[length]=0;
        CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;
        if(player==pool->localId){
            const char *name=pool->localName.c_str();DWORD color=pool->local->GetColorAsARGB();
            g_pChat->AddChatMessage(name,color,text);
        }else if(player<=1004&&pool->players[player]&&pool->players[player]->remote)pool->players[player]->remote->Chat(text);
    }
}
// samp.dll RVA: 0x00010c20
void ScrWorldPlayerDeath(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;WORD player;stream.Read(player);
    if(pool&&player<=1004&&pool->players[player]&&pool->players[player]->remote)pool->players[player]->remote->Kill();
}
// samp.dll RVA: 0x00010ce0
void ScrWorldPlayerRemove(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    DWORD player=0;CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;
    stream.ReadBits((unsigned char*)&player,16,true);
    if(pool&&(WORD)player<=1004){
        pool=((CNetGame*)g_pNetGame)->pools->players;
        if(pool->players[(WORD)player]&&pool->players[(WORD)player]->remote)pool->players[(WORD)player]->remote->Remove();
    }
}
// samp.dll RVA: 0x000102b0
void UpdateScoresPingsIps(RPCParameters *parameters) {
    unsigned bytes=(int)parameters->numberOfBitsOfData/8;
    RakNet::BitStream stream(parameters->input,bytes+1,false);
    if(bytes) {
        CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;
        unsigned count=bytes/10;
        WORD id;int score;DWORD ping;
        for(WORD i=0;(unsigned)i<count;i++) {
            stream.Read(id);stream.Read(score);stream.Read(ping);
            if(id<1004) {
                if(id==pool->localId)pool->localScore=score;else if(pool->players[id])pool->players[id]->score=score;
                if(id==pool->localId)pool->localPing=ping;else if(pool->players[id])pool->players[id]->ping=ping;
            }
        }
        if(g_deviceScoreboard)g_deviceScoreboard->UpdateList();
    }
}
// samp.dll RVA: 0x0000e740
void EnterEditObject(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    if(g_renderTextDrawSelection&&g_renderTextDrawSelection->enabled)g_renderTextDrawSelection->Disable();
    CObjectEdit *edit=(CObjectEdit*)g_deviceObjectEdit;if(edit&&edit->unknown80)edit->Disable(FALSE);
    if(g_renderObjectSelection)g_renderObjectSelection->Enable(TRUE);
}
// samp.dll RVA: 0x0000e800
void ScrCancelEdit(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CObjectEdit *edit=(CObjectEdit*)g_deviceObjectEdit;if(edit&&edit->unknown80)edit->Disable(FALSE);
    if(g_renderObjectSelection&&g_renderObjectSelection->enabled)g_renderObjectSelection->Enable(FALSE);
}
// samp.dll RVA: 0x0000e8b0
void ScrUpdateCameraTarget(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    bool enabled=false;stream.Read(enabled);if(g_pNetGame)((BYTE*)g_pNetGame)[0x233]=(BYTE)enabled;
}
// samp.dll RVA: 0x000116b0
void EditAttachedObject(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    int index=0;stream.Read(index);if(g_renderObjectSelection&&g_renderObjectSelection->enabled)g_renderObjectSelection->Enable(FALSE);
    ((CObjectEdit*)g_deviceObjectEdit)->EnterEditAttachedObject(index);
}
// samp.dll RVA: 0x00011770
void EditObject(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    bool playerObject=false;WORD id=0xffff;stream.Read(playerObject);stream.Read(id);
    if(g_renderObjectSelection&&g_renderObjectSelection->enabled)g_renderObjectSelection->Enable(FALSE);
    if(g_renderTextDrawSelection&&g_renderTextDrawSelection->enabled)g_renderTextDrawSelection->Disable();
    ((CObjectEdit*)g_deviceObjectEdit)->EnterEditObject(id,playerObject?TRUE:FALSE);
}
// samp.dll RVA: 0x0000e940
void ScrCreateActor(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CActorPool *pool=((CNetGame*)g_pNetGame)->pools->actors;
    if(pool){ActorCreateInfo info={0};stream.Read((char*)&info,31);
        if(GameIsPedModel(info.model)){pool->Create(&info);(void)pool->Get(info.id);}}
}
// samp.dll RVA: 0x00011cd0
void ScrDestroyActor(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CActorPool *pool=((CNetGame*)g_pNetGame)->pools->actors;if(pool){WORD id;stream.Read(id);pool->Delete(id);}
}
// samp.dll RVA: 0x00011090
void DamageVehicle(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;DWORD panels,doors;BYTE lights,tires;stream.Read(id);stream.Read(panels);stream.Read(doors);stream.Read(lights);stream.Read(tires);
    CVehiclePool *pool=((CNetGame*)g_pNetGame)->pools->vehicles;CVehicle *vehicle=pool->Get(id);
    if(vehicle){vehicle->UpdateDamage(panels,doors,lights);vehicle->SetTires(tires);}
}
// samp.dll RVA: 0x000111c0
void ScrSetVehicleParamsEx(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    if(!((CNetGame*)g_pNetGame)->pools->vehicles)return;
    WORD id;BYTE params[16];stream.Read(id);stream.Read((char*)params,16);
    CVehicle *vehicle=((CNetGame*)g_pNetGame)->pools->vehicles->Get(id);if(!vehicle)return;
    if(params[0]<=1)vehicle->SetField50(params[0]);
    if(params[1]<=1)vehicle->SetField54(params[1]);
    if(params[3]<=1)vehicle->Lock(params[3]);
    if(params[6]<=1)((BYTE*)vehicle)[0x64]=params[6];
    if(params[5]<=1)vehicle->CallBackend6a6ae0(1,0x11,params[5]?0x3f800000:0);
    if(params[4]<=1)vehicle->CallBackend6a6ae0(0,0x10,params[4]?0x3f800000:0);
    if(params[2]<=1){vehicle->EnableSiren(params[2]);vehicle->SetBackendWord45c(params[2]?20000:0);}
    if(params[7]<=1)vehicle->EnableSiren(params[7]);
    const DWORD component[]={10,8,11,9};
    for(int i=0;i<4;i++)if(params[8+i]<=1)vehicle->CallBackend6a6ae0(2+i,component[i],params[8+i]?0x3f800000:0);
    for(int j=0;j<4;j++){if(params[12+j]==0)vehicle->SetWindowOpenFlag((BYTE)component[j]);else if(params[12+j]==1)vehicle->ClearWindowOpenFlag((BYTE)component[j]);}
}
// samp.dll RVA: 0x0000f4b0
void ScrSetPlayerSkillLevel(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD player,value;int skill;stream.Read(player);stream.Read(skill);stream.Read(value);
    CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;
    if(pool) {
        CPed *ped;
        if(player==pool->localId){ped=g_pGame->GetPlayerPed();if(!ped)return;}
        else {
            if(player>=1004||!pool->connected[player]||!pool->players[player])return;
            CRemotePlayer *remote=pool->players[player]->remote;
            if(!remote||!remote->IsActive())return;ped=*(CPed**)(remote->nativeState+4);
        }
        ped->SetWeaponSkill(skill,value);
    }
}
// samp.dll RVA: 0x0000f5e0
void ScrCreate3DTextLabel(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CLabelPool *pool=((CNetGame*)g_pNetGame)->pools->labels;
    if(pool) {
        char text[2049]={0};WORD id,player,vehicle;DWORD color;CVector position;float distance;BYTE behindWalls=0;
        stream.Read(id);stream.Read(color);stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);
        stream.Read(distance);stream.Read(behindWalls);stream.Read(player);stream.Read(vehicle);
        StringCompressor::Instance()->DecodeString(text,2048,&stream,0);
        if(id<2048)pool->Create(id,text,color,position,distance,behindWalls,player,vehicle);
    }
}
// samp.dll RVA: 0x0000f790
void ScrDestroy3DTextLabel(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CLabelPool *pool=((CNetGame*)g_pNetGame)->pools->labels;
    if(pool){WORD id;stream.Read(id);if(id<2048&&pool->active[id])pool->Delete(id);}
}
// samp.dll RVA: 0x0000f850
void ScrChatBubble(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    if(!g_pNetGame)return;CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;if(!pool)return;
    char text[257]={0};WORD player;DWORD color;float distance;int lifespan;BYTE length;
    stream.Read(player);stream.Read(color);stream.Read(distance);stream.Read(lifespan);stream.Read(length);
    if(length<145) {
        stream.Read(text,length);text[length]=0;
        if(player<1004&&pool->connected[player])g_renderChatBubble->Add(player,text,color,distance,lifespan);
    }
}
// samp.dll RVA: 0x0000f9c0
void ScrShowDialog(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    char title[257]={0},button1[257]={0},button2[257]={0},text[4097]={0};short id;BYTE type,length;
    stream.Read(id);stream.Read(type);stream.Read(length);stream.Read(title,length);
    stream.Read(length);stream.Read(button1,length);stream.Read(length);stream.Read(button2,length);
    StringCompressor::Instance()->DecodeString(text,4096,&stream,0);
    if(strlen(title)&&strlen(text))g_renderDialog->Show(id,type,title,text,button1,button2,TRUE);
}
// samp.dll RVA: 0x0000ee60
void ScrClientMessage(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    char text[256]={0};DWORD color,length;stream.Read(color);stream.Read(length);
    if(length<256){stream.Read(text,length);text[length]=0;g_pChat->AddMessage(color,text);}
}
// samp.dll RVA: 0x0000fbb0
void ScrServerJoin(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;
    char name[256]={0};WORD player;DWORD color;BYTE npc=0,length=0;DWORD npcFlag=0;
    stream.Read(player);stream.Read(color);stream.Read(npc);stream.Read(length);stream.Read(name,length);name[length]=0;
    if(strlen(name)<25) {
        if(npc)npcFlag=1;pool->Create(player,name,npcFlag);
        if(color&&player<1004&&pool->connected[player])pool->GetPlayer(player)->SetColor(color);
    }
}
// samp.dll RVA: 0x0000fd40
void ScrServerQuit(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;
    WORD player;BYTE reason;stream.Read(player);stream.Read(reason);pool->Delete(player,reason);
}
// samp.dll RVA: 0x0000e450
void SrvNetStats(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    stream.Read((char*)g_srvNetStatsHistory,0x128);
}
// samp.dll RVA: 0x00010040
void ScrSetCheckpoint(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CVector position;float radius;stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);stream.Read(radius);
    CVector size={radius,radius,radius};g_pGame->SetCheckpoint(&position,&size);g_pGame->unknown4d=1;
}
// samp.dll RVA: 0x00010150
void ScrSetRaceCheckpoint(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE type;CVector point;stream.Read(type);
    stream.Read(point.x);stream.Read(point.y);stream.Read(point.z);CVector position=point;
    stream.Read(point.x);stream.Read(point.y);stream.Read(point.z);CVector next=point;
    stream.Read(point.x);
    g_pGame->SetRacingCheckpoint(type,&position,&next,point.x);g_pGame->unknown29=1;
}
// samp.dll RVA: 0x0000ef50
void ScrCreatePickup(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    int id;Pickup pickup;stream.Read(id);stream.Read((char*)&pickup,sizeof(pickup));
    CPickupPool *pool=((CNetGame*)g_pNetGame)->pools->pickups;if(pool)pool->Create(&pickup,id);
}
// samp.dll RVA: 0x0000f010
void ScrDestroyPickup(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    int id;stream.Read(id);CPickupPool *pool=((CNetGame*)g_pNetGame)->pools->pickups;if(pool)pool->Delete(id);
}
// samp.dll RVA: 0x0000f0b0
void ScrDestroyWeaponPickup(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE owner;stream.Read(owner);((CNetGame*)g_pNetGame)->pools->pickups->DeleteWeapon(owner);
}
// The original handlers use signed bits/8+1, including an extra byte for aligned data.
// Read failures are deliberately ignored; uninitialized native output locals stay so.
// samp.dll RVA: 0x0000edc0
void ScrSetWorldTime(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE hour;stream.Read(hour);
    (*(BYTE**)((BYTE*)g_pNetGame+0x3d5))[0x2c]=hour;
}
// samp.dll RVA: 0x0000f240
void ScrSetWeather(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE weather;stream.Read(weather);
    (*(BYTE**)((BYTE*)g_pNetGame+0x3d5))[0x2e]=weather;
    g_pGame->SetWorldWeather(weather);
}
// samp.dll RVA: 0x0000f2f0
void ScrSetPlayerTime(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE hour,minute;stream.Read(hour);stream.Read(minute);
    g_pGame->SetWorldTime(hour,minute);
    (*(BYTE**)((BYTE*)g_pNetGame+0x3d5))[0x2c]=hour;
    (*(BYTE**)((BYTE*)g_pNetGame+0x3d5))[0x2d]=minute;
}
// samp.dll RVA: 0x0000f3d0
void ScrToggleClock(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    bool enabled;stream.ReadBits((unsigned char*)&enabled,8,true);
    g_pGame->EnableClock(enabled);
    if(enabled)(*(BYTE**)((BYTE*)g_pNetGame+0x3d5))[0x1f]=0;
    else {
        (*(BYTE**)((BYTE*)g_pNetGame+0x3d5))[0x1f]=1;
        BYTE *settings=*(BYTE**)((BYTE*)g_pNetGame+0x3d5);
        g_pGame->GetWorldTime(settings+0x2c,settings+0x2d);
    }
}
// samp.dll RVA: 0x0000fe00
void ScrSetIngameTimer(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    DWORD timer=0;stream.Read(timer);g_pGame->SetTimer(timer);
}
// samp.dll RVA: 0x0000e430
void ScrDisableCheckpoint(RPCParameters *parameters) {(void)parameters;g_pGame->unknown4d=0;}
// samp.dll RVA: 0x0000e440
void ScrDisableRaceCheckpoint(RPCParameters *parameters) {(void)parameters;g_pGame->unknown29=0;}
// samp.dll RVA: 0x00117fa8
const int g_rpcScrSetIngameTimer=60;
// samp.dll RVA: 0x00117fa4
const int g_rpcScrWorldPlayerAdd=32;
// samp.dll RVA: 0x00117fa0
const int g_rpcScrWorldPlayerDeath=166;
// samp.dll RVA: 0x00117f9c
const int g_rpcScrWorldPlayerRemove=163;
// samp.dll RVA: 0x00117f98
const int g_rpcScrWorldVehicleAdd=164;
// samp.dll RVA: 0x00117f94
const int g_rpcScrWorldVehicleRemove=165;
// samp.dll RVA: 0x00117f90
const int g_rpcScrSetVehicleParamsEx=24;
// samp.dll RVA: 0x00117968
const int g_rpcDamageVehicle=106;
// samp.dll RVA: 0x00117f8c
const int g_rpcScrSetPlayerSkillLevel=34;
// samp.dll RVA: 0x00117f88
const int g_rpcScrCreate3DTextLabel=36;
// samp.dll RVA: 0x00117f84
const int g_rpcScrDestroy3DTextLabel=58;
// samp.dll RVA: 0x00117f80
const int g_rpcScrChatBubble=59;
// samp.dll RVA: 0x00117f7c
const int g_rpcScrShowDialog=61;
// samp.dll RVA: 0x00117f78
const int g_rpcScrServerJoin=137;
// samp.dll RVA: 0x00117f74
const int g_rpcScrServerQuit=138;
// samp.dll RVA: 0x00117f70
const int g_rpcScrInitGame=139;
// samp.dll RVA: 0x00117f6c
const int g_rpcScrSetCheckpoint=107;
// samp.dll RVA: 0x00117f68
const int g_rpcScrDisableCheckpoint=37;
// samp.dll RVA: 0x00117f64
const int g_rpcScrSetRaceCheckpoint=38;
// samp.dll RVA: 0x00117f60
const int g_rpcScrDisableRaceCheckpoint=39;
// samp.dll RVA: 0x00117f5c
const int g_rpcSrvNetStats=102;
// samp.dll RVA: 0x00117f58
const int g_rpcScrGamemodeRestart=40;
// samp.dll RVA: 0x00117f54
const int g_rpcConnectionRejected=130;
// samp.dll RVA: 0x00117f50
const int g_rpcScrClientMessage=93;
// samp.dll RVA: 0x00117f4c
const int g_rpcScrSetWorldTime=94;
// samp.dll RVA: 0x00117f48
const int g_rpcScrCreatePickup=95;
// samp.dll RVA: 0x00117f44
const int g_rpcScrDestroyPickup=63;
// samp.dll RVA: 0x00117f40
const int g_rpcScrDestroyWeaponPickup=151;
// samp.dll RVA: 0x00117f3c
const int g_rpcScrSetWeather=152;
// samp.dll RVA: 0x00117f38
const int g_rpcScrSetPlayerTime=29;
// samp.dll RVA: 0x00117f34
const int g_rpcScrToggleClock=30;
// samp.dll RVA: 0x00117f24
const int g_rpcScrCancelEdit=28;
// samp.dll RVA: 0x00117f20
const int g_rpcScrUpdateCameraTarget=170;
// samp.dll RVA: 0x00117f1c
const int g_rpcClientCheck=103;
// samp.dll RVA: 0x00117f18
const int g_rpcScrCreateActor=171;
// samp.dll RVA: 0x00117f14
const int g_rpcScrDestroyActor=172;
// Custom-model record handler RVA0x00011d80; legacy registry name retained.
// samp.dll RVA: 0x00117f10
const int g_rpcUnknown00117F10=179;
// Custom-model reset handler RVA0x0000ea40; legacy registry name retained.
// samp.dll RVA: 0x00117f0c
const int g_rpcUnknown00117F0C=180;
// Custom-model download handler RVA0x00011f80; legacy registry name retained.
// samp.dll RVA: 0x00117f08
const int g_rpcUnknown00117F08=183;
// Custom-model completion handler RVA0x0000ead0; legacy registry name retained.
// samp.dll RVA: 0x00117f04
const int g_rpcUnknown00117F04=185;
// samp.dll RVA: 0x0000eb60
void UnRegisterRPCs(RakClientInterface *client) {
    const int *ids[]={
        &g_rpcScrSetIngameTimer,&g_rpcScrWorldPlayerAdd,&g_rpcScrWorldPlayerDeath,&g_rpcScrWorldPlayerRemove,
        &g_rpcScrWorldVehicleAdd,&g_rpcScrWorldVehicleRemove,&g_rpcScrSetVehicleParamsEx,&g_localRequestClassRpc,
        &g_localSpawnRequestRpc,&g_rpcDamageVehicle,&g_localEnterVehicleRpc,&g_localExitVehicleRpc,
        &g_rpcScrSetPlayerSkillLevel,&g_rpcScrCreate3DTextLabel,&g_rpcScrDestroy3DTextLabel,&g_rpcScrChatBubble,
        &g_rpcScrShowDialog,&g_rpcScrServerJoin,&g_rpcScrServerQuit,&g_rpcScrInitGame,
        &g_localChatRpc,&g_rpcScrSetCheckpoint,&g_rpcScrDisableCheckpoint,&g_rpcScrSetRaceCheckpoint,
        &g_rpcScrDisableRaceCheckpoint,&g_netPlayersInfoRpc,&g_rpcSrvNetStats,&g_rpcScrGamemodeRestart,
        &g_rpcConnectionRejected,&g_rpcScrClientMessage,&g_rpcScrSetWorldTime,&g_rpcScrCreatePickup,
        &g_rpcScrDestroyPickup,&g_rpcScrDestroyWeaponPickup,&g_scmEventRpcId,&g_rpcScrSetWeather,
        &g_rpcScrSetPlayerTime,&g_rpcScrToggleClock,&g_objectEditAttachedRpc,&g_objectEditRpc,
        &g_objectSelectionRpc,&g_rpcScrCancelEdit,&g_rpcScrUpdateCameraTarget,&g_rpcClientCheck,
        &g_rpcScrCreateActor,&g_rpcScrDestroyActor,&g_rpcUnknown00117F10,&g_rpcUnknown00117F0C,
        &g_rpcUnknown00117F08,&g_rpcUnknown00117F04,
    };
    for(unsigned i=0;i<sizeof(ids)/sizeof(ids[0]);i++)client->UnregisterAsRemoteProcedureCall(const_cast<int*>(ids[i]));
}

// samp.dll RVA: 0x00012310
void RegisterRPCs(RakClientInterface *client) {
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerSkillLevel),ScrSetPlayerSkillLevel);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrCreate3DTextLabel),ScrCreate3DTextLabel);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDestroy3DTextLabel),ScrDestroy3DTextLabel);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrChatBubble),ScrChatBubble);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrShowDialog),ScrShowDialog);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetCheckpoint),ScrSetCheckpoint);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDisableCheckpoint),ScrDisableCheckpoint);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetRaceCheckpoint),ScrSetRaceCheckpoint);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDisableRaceCheckpoint),ScrDisableRaceCheckpoint);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_netPlayersInfoRpc),UpdateScoresPingsIps);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcSrvNetStats),SrvNetStats);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrGamemodeRestart),ScrGamemodeRestart);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcConnectionRejected),ConnectionRejected);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrClientMessage),ScrClientMessage);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetWorldTime),ScrSetWorldTime);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrCreatePickup),ScrCreatePickup);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDestroyPickup),ScrDestroyPickup);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDestroyWeaponPickup),ScrDestroyWeaponPickup);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_scmEventRpcId),ScmEvent);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetWeather),ScrSetWeather);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerTime),ScrSetPlayerTime);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrToggleClock),ScrToggleClock);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetIngameTimer),ScrSetIngameTimer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrWorldPlayerAdd),ScrWorldPlayerAdd);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrWorldPlayerDeath),ScrWorldPlayerDeath);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrWorldPlayerRemove),ScrWorldPlayerRemove);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrWorldVehicleAdd),ScrWorldVehicleAdd);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrWorldVehicleRemove),ScrWorldVehicleRemove);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcDamageVehicle),DamageVehicle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetVehicleParamsEx),ScrSetVehicleParamsEx);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_localEnterVehicleRpc),EnterVehicle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_localExitVehicleRpc),ExitVehicle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrServerJoin),ScrServerJoin);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrServerQuit),ScrServerQuit);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrInitGame),ScrInitGame);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_localChatRpc),Chat);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_localRequestClassRpc),RequestClass);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_localSpawnRequestRpc),RequestSpawn);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_objectEditAttachedRpc),EditAttachedObject);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_objectEditRpc),EditObject);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_objectSelectionRpc),EnterEditObject);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrCancelEdit),ScrCancelEdit);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrUpdateCameraTarget),ScrUpdateCameraTarget);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcClientCheck),ClientCheck);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrCreateActor),ScrCreateActor);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDestroyActor),ScrDestroyActor);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcUnknown00117F10),ScrCustomModelRecord);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcUnknown00117F0C),ScrCustomModelsReset);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcUnknown00117F08),ScrCustomModelDownload);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcUnknown00117F04),ScrCustomModelsComplete);
}
