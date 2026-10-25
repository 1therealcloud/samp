#include "net/scriptrpc.h"
#include "RakClientInterface.h"
#include "textdrawselection.h"
#include "BitStream.h"
#include "main.h"
#include "net/netgame.h"
#include "net/playerpool.h"
#include "net/localplayer.h"
#include "game/playerped.h"
#include "game/camera.h"
#include "net/gangzonepool.h"
#include "net/menupool.h"
#include "net/textdrawpool.h"
#include "game/audio.h"
#include "gui/render_dependencies.h"
#include "chatwindow.h"
#include "net/playerinfo.h"
#include "net/remoteplayer.h"
#include "gui/device_lifecycle.h"
#include "net/vehiclepool.h"
#include "net/objectpool.h"
#include "net/actorpool.h"
#include "game/actor.h"
#include "game/custommodels.h"
#include "game/building_removal.h"
#include "StringCompressor.h"
#include <string.h>
// samp.dll RVA: 0x00118288
const Scripting::OpcodeInfo g_rpcAttachObjectToPlayerCommand={0x69b,"iiffffff"};
// samp.dll RVA: 0x0001aba0
void ObjectReadMaterials(BYTE count,CObject *object,RakNet::BitStream *stream) {
    DWORD color=0;BYTE length=0,type,index,size,fontSize,bold,alignment;WORD model;
    DWORD fontColor,backgroundColor;
    char dictionary[257],texture[257],font[257],text[2049];
    for(unsigned left=count;left;--left) {
        memset(dictionary,0,sizeof(dictionary));memset(texture,0,sizeof(texture));
        memset(font,0,sizeof(font));memset(text,0,sizeof(text));
        stream->Read(type);
        if(type==1) {
            stream->Read(index);stream->Read(model);stream->Read(length);stream->Read(dictionary,length);
            stream->Read(length);stream->Read(texture,length);stream->Read(color);
            if(strlen(dictionary)<32&&strlen(texture)<32) {
                int materialModel=model;if(model==0xffff||model>20000)materialModel=-1;
                if(object)object->SetMaterial(materialModel,index,dictionary,texture,color);
            }
        }else if(type==2) {
            stream->Read(index);stream->Read(size);stream->Read(length);stream->Read(font,length);
            stream->Read(fontSize);stream->Read(bold);stream->Read(fontColor);stream->Read(backgroundColor);stream->Read(alignment);
            StringCompressor::Instance()->DecodeString(text,2048,stream,0);
            if(strlen(font)<32&&strlen(text)&&object)object->SetMaterialText(index,text,size,font,fontSize,bold,fontColor,backgroundColor,alignment);
        }
    }
}
// samp.dll RVA: 0x0001ae70
void ScrCreateObject(RPCParameters *parameters) {
    WORD id=0xffff,vehicle=0xffff,attachedObject=0xffff;int model=-1;
    CVector position={0,0,0},rotation={0,0,0},offset={0,0,0},attachedRotation={0,0,0};
    float distance=0;BYTE noCameraCollision=0,syncRotation=0,materials=0;
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    stream.Read(id);stream.Read(model);stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);
    stream.Read(rotation.x);stream.Read(rotation.y);stream.Read(rotation.z);stream.Read(distance);
    stream.Read(noCameraCollision);stream.Read(vehicle);stream.Read(attachedObject);
    if(vehicle!=0xffff||attachedObject!=0xffff) {
        stream.Read(offset.x);stream.Read(offset.y);stream.Read(offset.z);
        stream.Read(attachedRotation.x);stream.Read(attachedRotation.y);stream.Read(attachedRotation.z);stream.Read(syncRotation);
    }
    stream.Read(materials);CObjectPool *pool=((CNetGame*)g_pNetGame)->pools->objects;
    pool->Create(id,model,position,rotation,distance);CObject *object=(CObject*)pool->Get(id);
    if(vehicle!=0xffff){if(object)object->SetAttachedVehicle(vehicle,&offset,&attachedRotation);}
    else if(attachedObject!=0xffff&&object)object->SetAttachedObject(attachedObject,&offset,&attachedRotation,syncRotation);
    if(materials)ObjectReadMaterials(materials,object,&stream);
    if(object&&noCameraCollision)((BYTE*)object)[0x53]=1;
}
// samp.dll RVA: 0x0001b1d0
void ScrSetObjectMaterial(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CObjectPool *pool=((CNetGame*)g_pNetGame)->pools->objects;WORD id;stream.Read(id);
    CObject *object=(CObject*)pool->Get(id);
    char dictionary[257]={0},texture[257]={0},font[257]={0},text[2049]={0};
    BYTE length=0,type,index,size,fontSize,bold,alignment;DWORD color=0,fontColor,backgroundColor;WORD model;
    stream.Read(type);
    if(type==1) {
        stream.Read(index);stream.Read(model);stream.Read(length);stream.Read(dictionary,length);
        stream.Read(length);stream.Read(texture,length);stream.Read(color);
        if(strlen(dictionary)<32&&strlen(texture)<32) {
            int materialModel=model;if(model==0xffff)materialModel=-1;
            if(object)object->SetMaterial(materialModel,index,dictionary,texture,color);
        }
    }else if(type==2) {
        stream.Read(index);stream.Read(size);stream.Read(length);stream.Read(font,length);
        stream.Read(fontSize);stream.Read(bold);stream.Read(fontColor);stream.Read(backgroundColor);stream.Read(alignment);
        StringCompressor::Instance()->DecodeString(text,2048,&stream,0);
        if(strlen(font)<32&&strlen(text)&&object)object->SetMaterialText(index,text,size,font,fontSize,bold,fontColor,backgroundColor,alignment);
    }
}
// samp.dll RVA: 0x0001c1d0
void ScrAttachObjectToPlayer(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id,player;CVector offset,rotation;stream.Read(id);stream.Read(player);
    stream.Read(offset.x);stream.Read(offset.y);stream.Read(offset.z);
    stream.Read(rotation.x);stream.Read(rotation.y);stream.Read(rotation.z);
    CNetGamePools *pools=((CNetGame*)g_pNetGame)->pools;
    CObject *object=(CObject*)pools->objects->Get(id);if(!object)return;
    CPlayerPool *players=pools->players;CPed *ped;
    if(player==players->localId)ped=*(CPed**)players->local->nativeState;
    else {
        if(player>1004||!players->players[player]||!players->players[player]->remote)return;
        ped=*(CPed**)(players->players[player]->remote->nativeState+4);
    }
    Scripting::ProcessCommand(&g_rpcAttachObjectToPlayerCommand,*(DWORD*)((BYTE*)object+0x44),*(DWORD*)((BYTE*)ped+0x44),
        (double)offset.x,(double)offset.y,(double)offset.z,(double)rotation.x,(double)rotation.y,(double)rotation.z);
}
// samp.dll RVA: 0x00018a00
void ScrSetPlayerAttachedObject(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    PedAccessoryState info={0};WORD player;int index;bool create;
    stream.Read(player);stream.Read(index);stream.Read(create);if(create)stream.Read((char*)&info,52);
    CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;if(!pool)return;
    CPed *ped;
    if(player==pool->localId)ped=*(CPed**)pool->local->nativeState;
    else {
        if(player>=1004||!pool->connected[player])return;
        CRemotePlayer *remote=pool->players[player]?pool->players[player]->remote:0;
        ped=*(CPed**)(remote->nativeState+4);
    }
    if(ped){if(create)ped->AddAccessory(index,&info);else ped->DeleteAccessory(index);}
}
// samp.dll RVA: 0x0001d780
void ScrRpc48(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    DWORD value=0;stream.Read(value);BYTE *models=(BYTE*)g_customModels;
    if(models&&*(DWORD*)(models+0x217)!=value){*(DWORD*)(models+0x217)=value;models[0x221]=0;models[0x220]=0;}
}
// samp.dll RVA: 0x001182b0
const Scripting::OpcodeInfo g_rpcPlayerOpcode052cCommand={0x52c,"ii"};
// samp.dll RVA: 0x001182c4
const Scripting::OpcodeInfo g_rpcPlayerOpcode03fdCommand={0x3fd,"ii"};
// samp.dll RVA: 0x0001da90
void ScrSetPlayerName(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;WORD id;BYTE length,status;char name[25];stream.Read(id);stream.Read(length);
    if(length<25){stream.Read(name,length);stream.Read(status);name[length]=0;
        if(status==1)pool->SetName(id,name);if(pool->localId==id)pool->localName.assign(name,strlen(name));}
}
// samp.dll RVA: 0x000188b0
void ScrSetPlayerDrunkLevel(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    DWORD level;stream.Read(level);CPed *ped=g_pGame->GetPlayerPed();if(ped)ped->SetDrunkLevel(level);
}
// samp.dll RVA: 0x00018950
void ScrSetPlayerArmedWeapon(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CPed *ped=g_pGame->GetPlayerPed();if(ped){int weapon;stream.Read(weapon);if(weapon>=0&&weapon<47)ped->SetArmedWeapon(weapon,0);}
}
// samp.dll RVA: 0x00019b20
void ScrAttachCameraToObject(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;stream.Read(id);CObjectPool *pool=((CNetGame*)g_pNetGame)->pools->objects;
    if(id<=2100&&pool->active[id]) {
        CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;*(DWORD*)(local->nativeState+0x318)=1;
        CEntity *object=((CNetGame*)g_pNetGame)->pools->objects->Get(id);g_pGame->m_pCamera->Attach(object);
    }
}
// samp.dll RVA: 0x00019c20
void ScrInterpolateCamera(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    bool move=true;CVector from,to;int time;BYTE mode;stream.Read(move);
    stream.Read(from.x);stream.Read(from.y);stream.Read(from.z);stream.Read(to.x);stream.Read(to.y);stream.Read(to.z);stream.Read(time);stream.Read(mode);
    if(mode<1||mode>2)mode=2;
    if(time>0){CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;*(DWORD*)(local->nativeState+0x318)=1;
        if(move)g_pGame->m_pCamera->SetMoveVector(&from,&to,time,mode);else g_pGame->m_pCamera->SetTrackVector(&from,&to,time,mode);}
}
// samp.dll RVA: 0x0001d110
void ClickTextDraw(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    bool enabled=false;DWORD color=0;stream.Read(enabled);stream.Read(color);
    if(g_renderObjectSelection&&g_renderObjectSelection->enabled)g_renderObjectSelection->Enable(FALSE);
    CObjectEdit *edit=(CObjectEdit*)g_deviceObjectEdit;if(edit&&edit->unknown80)edit->Disable(FALSE);
    if(g_renderTextDrawSelection){if(enabled)g_renderTextDrawSelection->Enable(color);else g_renderTextDrawSelection->Disable();}
}
// samp.dll RVA: 0x0001bca0
void ScrStopObjectCameraCollision(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;stream.Read(id);CEntity *object=((CNetGame*)g_pNetGame)->pools->objects->Get(id);if(object)((BYTE*)object)[0x53]=1;
}
// samp.dll RVA: 0x0001cff0
void ScrRemoveBuildingForPlayer(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    int model=0;CVector position={0,0,0};float radius=0;stream.Read(model);stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);stream.Read(radius);
    GameRemoveBuilding(model,position.x,position.y,position.z,radius);
}
// samp.dll RVA: 0x00018b40
void ScrPlayCrimeReport(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD unused;BOOL useVehicle=0;int model=0;DWORD color=0,first=0;CVector position;
    stream.Read(unused);stream.Read(useVehicle);stream.Read(model);stream.Read(color);stream.Read(first);stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);
    // Native passes a pointer to the three-word vector, not its first DWORD.
    g_pGame->ApplyPreviewAnimation(first,(DWORD)&position,useVehicle,model,color);
}
// samp.dll RVA: 0x00018750
void ScrPlayerOpcode052C(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    DWORD value;stream.Read(value);if(g_pGame->GetPlayerPed())Scripting::ProcessCommand(&g_rpcPlayerOpcode052cCommand,0,value);
}
// samp.dll RVA: 0x00018800
void ScrPlayerOpcode03FD(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    DWORD value;stream.Read(value);if(g_pGame->GetPlayerPed())Scripting::ProcessCommand(&g_rpcPlayerOpcode03fdCommand,0,value);
}
// Native is a bare RET; this empty body is recovered behavior, not a stub.
// samp.dll RVA: 0x00017bd0
void ScrRpc125(RPCParameters *parameters) {(void)parameters;}
// samp.dll RVA: 0x0001a230
void ScrRpc64(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);WORD ignored;stream.Read(ignored);
}
// samp.dll RVA: 0x00118274
const Scripting::OpcodeInfo g_rpcWidescreenCommand={0x2a3,"i"};
// samp.dll RVA: 0x0011844c
const char g_rpcWidescreenMessage[]="Widescreen = %i";
// samp.dll RVA: 0x0011845c
const char g_rpcInvalidSkinWarning[]="Warning: SetPlayerSkin %d isn't a valid ped model.";
// samp.dll RVA: 0x00017a60
void ScrSetSpawnInfo(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;BYTE info[50];stream.Read((char*)info,50);pool->local->SetSpawnInfo(info);
}
// samp.dll RVA: 0x00018250
void ScrSetPlayerFightingStyle(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;BYTE style=0;stream.Read(id);stream.Read(style);CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;
    if(pool) {
        CPed *ped;
        if(id==pool->localId)ped=*(CPed**)pool->local->nativeState;
        else {if(id>=1004||!pool->connected[id])return;CRemotePlayer *remote=pool->GetPlayer(id);ped=*(CPed**)(remote->nativeState+4);}
        if(ped)ped->SetFightingStyle(style);
    }
}
// samp.dll RVA: 0x0001a740
void ScrSetPlayerAmmo(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE weapon;WORD ammo;stream.Read(weapon);stream.Read(ammo);
    CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;(*(CPed**)local->nativeState)->SetWeaponAmmo(weapon,ammo);
}
// samp.dll RVA: 0x00019240
void ScrSetPlayerTeam(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;WORD id;BYTE team;stream.Read(id);stream.Read(team);
    if(id==pool->localId)pool->local->nativeState[0x130]=team;
    else if(id<=1004){CPlayerInfo *info=pool->players[id];if(info&&info->remote)info->remote->nativeState[0x19]=team;}
}
// samp.dll RVA: 0x00018c80
void ScrSetPlayerSkin(RPCParameters *parameters) {
    int model=0,customModel=0;
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;if(!pool)return;
    WORD id;stream.Read(id);stream.Read(model);stream.Read(customModel);
    if(customModel>20000&&(GameIsPedModel(customModel)||(g_customModels&&g_customModels->DoesExist(customModel))))model=customModel;
    if(!GameIsPedModel(model)){if(g_pChat)g_pChat->AddInfoMessage(g_rpcInvalidSkinWarning,model);return;}
    if(id==pool->localId)(*(CPed**)pool->local->nativeState)->SetModelIndex(model);
    else if(id<1004&&pool->connected[id]) {
        CRemotePlayer *remote=pool->GetPlayer(id);
        if(*(CPed**)(remote->nativeState+4)&&remote->nativeState[0x1a]) {
            remote=pool->GetPlayer(id);CPed *ped=*(CPed**)(remote->nativeState+4);if(ped)ped->SetModelIndex(model);
        }
    }
}
// samp.dll RVA: 0x0001a480
void ScrApplyAnimation(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    char library[256]={0},name[256]={0};WORD id;BYTE libraryLength,nameLength;float speed;bool loop,lockX,lockY,freeze;int time;
    stream.Read(id);stream.Read(libraryLength);stream.Read(library,libraryLength);stream.Read(nameLength);stream.Read(name,nameLength);
    stream.Read(speed);stream.Read(loop);stream.Read(lockX);stream.Read(lockY);stream.Read(freeze);stream.Read(time);
    library[libraryLength]=0;name[nameLength]=0;CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;
    if(pool) {
        CPed *ped;
        if(id==pool->localId)ped=*(CPed**)pool->local->nativeState;
        else {
            if(id>=1004||!pool->connected[id]||!pool->players[id]||!pool->players[id]->remote)return;
            CRemotePlayer *remote=pool->players[id]->remote;ped=*(CPed**)(remote->nativeState+4);*(DWORD*)(remote->nativeState+0x1cd)=1;
        }
        if(ped)ped->ApplyAnimation(name,library,speed,loop,lockX,lockY,freeze,time);
    }
}
// samp.dll RVA: 0x00018090
void ScrClearAnimations(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;stream.Read(id);CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;
    if(pool) {
        CPed *ped;
        if(id==pool->localId)ped=*(CPed**)pool->local->nativeState;
        else {
            if(id>=1004||!pool->connected[id]||!pool->players[id]||!pool->players[id]->remote)return;
            CRemotePlayer *remote=pool->players[id]->remote;ped=*(CPed**)(remote->nativeState+4);*(DWORD*)(remote->nativeState+0x1cd)=0;
        }
        if(ped){CMatrix matrix;ped->GetMatrix(&matrix);ped->Teleport(matrix.position);}
    }
}
// samp.dll RVA: 0x0001c0f0
void ScrMoveVehicleComponent(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CVehiclePool *pool=((CNetGame*)g_pNetGame)->pools->vehicles;
    if(pool){WORD id,component;stream.Read(id);stream.Read(component);if(id<2000&&pool->m_bNotEmpty[id])pool->m_pObjects[id]->RemoveComponent(component);}
}
// samp.dll RVA: 0x000185c0
void ScrSetPlayerWidescreen(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE enabled;stream.Read(enabled);g_pChat->AddDebugMessage(g_rpcWidescreenMessage,(int)enabled);
    Scripting::ProcessCommand(&g_rpcWidescreenCommand,(int)enabled);
}
// samp.dll RVA: 0x00018670
void ScrSetVehicleTires(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;BYTE tires;stream.Read(id);stream.Read(tires);CVehiclePool *pool=((CNetGame*)g_pNetGame)->pools->vehicles;
    CVehicle *vehicle=pool->Get(id);if(pool&&vehicle)vehicle->SetTires(tires);
}
// samp.dll RVA: 0x0011829c
const Scripting::OpcodeInfo g_rpcExplosionCommand={0x948,"fffii"};
// samp.dll RVA: 0x0001b540
void ScrSetObjectPosition(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;CVector position;float unusedRotation;stream.Read(id);stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);stream.Read(unusedRotation);
    CEntity *object=((CNetGame*)g_pNetGame)->pools->objects->Get(id);if(object)object->Teleport(position);
}
// samp.dll RVA: 0x0001b650
void ScrSetObjectRotation(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;CVector rotation;stream.Read(id);stream.Read(rotation.x);stream.Read(rotation.y);stream.Read(rotation.z);
    CObjectPool *pool=((CNetGame*)g_pNetGame)->pools->objects;if(pool){CObject *object=(CObject*)pool->Get(id);if(object)object->SetRotation(&rotation);}
}
// samp.dll RVA: 0x0001b750
void ScrDestroyObject(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;stream.Read(id);CObjectPool *pool=((CNetGame*)g_pNetGame)->pools->objects;
    CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;CEntity *object=pool->Get(id);
    if(object){if(local&&*(CEntity**)(local->nativeState+0x2de)==object){*(CEntity**)(local->nativeState+0x2de)=0;*(DWORD*)(local->nativeState+0x2e6)=0;}pool->Delete(id);}
}
// samp.dll RVA: 0x0001ba30
void ScrMoveObject(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;CVector origin,position,rotation;float speed;stream.Read(id);
    stream.Read(origin.x);stream.Read(origin.y);stream.Read(origin.z);
    stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);stream.Read(speed);
    stream.Read(rotation.x);stream.Read(rotation.y);stream.Read(rotation.z);
    CObjectEdit *edit=(CObjectEdit*)g_deviceObjectEdit;if(edit&&edit->unknown80&&edit->objectId==id)return;
    CObject *object=(CObject*)((CNetGame*)g_pNetGame)->pools->objects->Get(id);if(object)object->Move(position,speed,rotation);
}
// samp.dll RVA: 0x0001bbe0
void ScrStopObject(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;stream.Read(id);CObject *object=(CObject*)((CNetGame*)g_pNetGame)->pools->objects->Get(id);if(object)object->Stop();
}
// samp.dll RVA: 0x0001d210
void ScrSetActorAnimation(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CActorPool *pool=((CNetGame*)g_pNetGame)->pools->actors;
    if(pool) {
        char library[256]={0},name[256]={0};BYTE libraryLength,nameLength;WORD id;float speed;bool loop,lockX,lockY,freeze;int time;
        stream.Read(id);stream.Read(libraryLength);stream.Read(library,libraryLength);stream.Read(nameLength);stream.Read(name,nameLength);
        stream.Read(speed);stream.Read(loop);stream.Read(lockX);stream.Read(lockY);stream.Read(freeze);stream.Read(time);
        library[libraryLength]=0;name[nameLength]=0;CActor *actor=pool->Get(id);
        if(actor)actor->PerformAnimation(name,library,speed,loop,lockX,lockY,freeze,time);
    }
}
// samp.dll RVA: 0x0001d3f0
void ScrClearActorAnimations(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;stream.Read(id);CActorPool *pool=((CNetGame*)g_pNetGame)->pools->actors;if(pool){CActor *actor=pool->Get(id);if(actor)actor->ClearTasks();}
}
// samp.dll RVA: 0x0001d4b0
void ScrSetActorRotation(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;float angle;stream.Read(id);stream.Read(angle);CActorPool *pool=((CNetGame*)g_pNetGame)->pools->actors;
    if(pool){CActor *actor=pool->Get(id);if(actor)actor->SetRotation(angle);}
}
// samp.dll RVA: 0x0001d590
void ScrSetActorPosition(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;CVector position;stream.Read(id);stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);
    CActorPool *pool=((CNetGame*)g_pNetGame)->pools->actors;if(pool){CActor *actor=pool->Get(id);if(actor)actor->Teleport(position);}
}
// samp.dll RVA: 0x0001d6a0
void ScrSetActorHealth(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;float health;stream.Read(id);stream.Read(health);CActorPool *pool=((CNetGame*)g_pNetGame)->pools->actors;
    if(pool){CActor *actor=pool->Get(id);if(actor)actor->SetHealth(health);}
}
// samp.dll RVA: 0x0001a2c0
void ScrSetPlayerMapIcon(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE id,icon,type;CVector position;DWORD color;stream.Read(id);stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);stream.Read(icon);stream.Read(color);stream.Read(type);
    ((CNetGame*)g_pNetGame)->CreateMapIcon(id,position,icon,color,type);
}
// samp.dll RVA: 0x0001a3e0
void ScrRemovePlayerMapIcon(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE id;stream.Read(id);((CNetGame*)g_pNetGame)->RemoveMapIcon(id);
}
// samp.dll RVA: 0x0001b840
void ScrCreateExplosion(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CVector position;int type;float radius;stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);stream.Read(type);stream.Read(radius);
    // Preserve native descriptor "fffii" despite its double-promoted radius argument.
    Scripting::ProcessCommand(&g_rpcExplosionCommand,(double)position.x,(double)position.y,(double)position.z,type,(double)radius);
}
// samp.dll RVA: 0x0001b950
void ScrShowPlayerNametagForPlayer(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;BYTE show;stream.Read(id);stream.Read(show);CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;
    if(id<1004&&pool->connected[id]){CRemotePlayer *remote=pool->GetPlayer(id);*(DWORD*)(remote->nativeState+0x10)=show!=0;}
}
// samp.dll RVA: 0x000197a0
void ScrSetVehiclePosition(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;CVector position;stream.Read(id);stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);
    if(g_pNetGame){CVehiclePool *pool=((CNetGame*)g_pNetGame)->pools->vehicles;
        if(pool&&id<2000&&pool->m_bNotEmpty[id])pool->m_pObjects[id]->Teleport(position);}
}
// samp.dll RVA: 0x000198b0
void ScrSetVehicleZAngle(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;float angle;stream.Read(id);stream.Read(angle);CVehiclePool *pool=((CNetGame*)g_pNetGame)->pools->vehicles;
    if(pool&&id<2000&&pool->m_bNotEmpty[id])pool->m_pObjects[id]->SetRotation(angle);
}
// samp.dll RVA: 0x00019990
void ScrSetVehicleParamsForPlayer(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;BYTE objective,locked;stream.Read(id);stream.Read(objective);stream.Read(locked);
    CVehiclePool *pool=((CNetGame*)g_pNetGame)->pools->vehicles;if(pool)pool->AssignSpecialParams(id,objective,locked);
}
// samp.dll RVA: 0x00019a60
void ScrLinkVehicleToInterior(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;BYTE interior;stream.Read(id);stream.Read(interior);((CNetGame*)g_pNetGame)->pools->vehicles->ChangeInterior(id,interior);
}
// samp.dll RVA: 0x0001a8a0
void ScrSetVehicleHealth(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;float health;stream.Read(id);stream.Read(health);CVehiclePool *pool=((CNetGame*)g_pNetGame)->pools->vehicles;
    if(pool&&id<2000&&pool->m_bNotEmpty[id]){CVehicle *vehicle=pool->Get(id);if(vehicle)vehicle->SetHealth(health);}
}
// samp.dll RVA: 0x0001a980
void ScrAttachTrailerToVehicle(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    if(!((CNetGame*)g_pNetGame)->pools->vehicles)return;
    WORD trailerId,tractorId;stream.Read(trailerId);stream.Read(tractorId);
    CVehiclePool *pool=((CNetGame*)g_pNetGame)->pools->vehicles;
    CVehicle *trailer=pool->Get(trailerId),*tractor=pool->Get(tractorId);
    if(trailer&&tractor){tractor->SetTrailer(trailer);tractor->AttachTrailer();}
}
// samp.dll RVA: 0x0001aac0
void ScrDetachTrailerFromVehicle(RPCParameters *parameters) {
    unsigned char *input=parameters->input;unsigned bits=parameters->numberOfBitsOfData;
    if(!((CNetGame*)g_pNetGame)->pools->vehicles)return;
    RakNet::BitStream stream(input,(int)bits/8+1,false);WORD id;stream.Read(id);
    CVehicle *vehicle=((CNetGame*)g_pNetGame)->pools->vehicles->Get(id);
    if(vehicle){vehicle->DetachTrailer();vehicle->SetTrailer(0);}
}
// samp.dll RVA: 0x00018450
void ScrSetVehicleVelocity(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE type=0;CVector velocity;stream.Read(type);stream.Read(velocity.x);stream.Read(velocity.y);stream.Read(velocity.z);
    CVehiclePool *pool=((CNetGame*)g_pNetGame)->pools->vehicles;CPed *ped=g_pGame->GetPlayerPed();
    if(ped&&ped->IsInVehicle()) {
        WORD id=pool->Find(ped->GetVehicle());
        if(id!=0xffff){CVehicle *vehicle=pool->Get(id);if(vehicle){if(type==0)vehicle->SetSpeed(velocity);else if(type==1)vehicle->SetTurnSpeed(velocity);}}
    }
}
// samp.dll RVA: 0x0001bd60
void ScrSetNumberPlate(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    char text[33]={0};BYTE length=0;WORD id;stream.Read(id);stream.Read(length);
    if(length<=32){stream.Read(text,length);CVehiclePool *pool=((CNetGame*)g_pNetGame)->pools->vehicles;
        if(pool&&id<2000&&pool->m_bNotEmpty[id])pool->m_pObjects[id]->SetLicensePlateText(text);}
}
// samp.dll RVA: 0x0001be80
void ScrTogglePlayerSpectating(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BOOL enabled;stream.Read(enabled);((CNetGame*)g_pNetGame)->pools->players->local->ToggleSpectating(enabled);
}
// samp.dll RVA: 0x0001bf30
void ScrPlayerSpectatePlayer(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;BYTE mode;stream.Read(id);stream.Read(mode);mode=mode==2?15:mode==3?14:4;
    CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;local->nativeState[0x312]=mode;local->SpectatePlayer(id);
}
// samp.dll RVA: 0x0001c010
void ScrPlayerSpectateVehicle(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;BYTE mode;stream.Read(id);stream.Read(mode);mode=mode==2?15:mode==3?14:3;
    CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;local->nativeState[0x312]=mode;local->SpectateVehicle(id);
}
// samp.dll RVA: 0x00019130
void ScrPutPlayerInVehicle(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD id;BYTE seat;stream.Read(id);stream.Read(seat);
    DWORD ref=((CNetGame*)g_pNetGame)->pools->vehicles->GetRef((int)id);
    CVehicle *vehicle=((CNetGame*)g_pNetGame)->pools->vehicles->Get(id);
    if(ref&&vehicle)g_pGame->GetPlayerPed()->PutIntoVehicle(ref,seat);
}
// samp.dll RVA: 0x00017b00
void ScrRemovePlayerFromVehicle(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;(*(CPed**)local->nativeState)->ExitVehicle();
}
// samp.dll RVA: 0x00019420
void ScrDisplayGametext(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    char text[256]={0};int style,time,length;stream.Read(style);stream.Read(time);stream.Read(length);
    if(length>0&&length<201){stream.Read(text,length);text[length]=0;g_pGame->DisplayGameText(text,time,style);}
}
// samp.dll RVA: 0x00017960
void ScrSetPlayerShopName(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    char name[33]={0};stream.Read(name,32);CPed *ped=g_pGame->GetPlayerPed();
    if(ped){if(strlen(name)){ped->SetBackendShoppingName(name);ped->LoadShoppingDataSubsection(name);}
        else {ped->SetBackendShoppingName(0);ped->LoadShoppingDataSubsection("");}}
}
// samp.dll RVA: 0x00019330
void ScrSetPlayerColor(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;WORD player;DWORD color;stream.Read(player);stream.Read(color);
    if(player==pool->localId)pool->local->SetColor(color);
    // Native admits1004; adjacency beyond players[] remains unproven.
    else if(player<=1004){CPlayerInfo *info=pool->players[player];if(info&&info->remote)info->remote->SetColor(color);}
}
// samp.dll RVA: 0x0001d830
void ScrDeathMessage(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CPlayerPool *pool=((CNetGame*)g_pNetGame)->pools->players;WORD killer,victim;BYTE weapon;
    stream.Read(killer);stream.Read(victim);stream.Read(weapon);if(victim==0xffff)return;
    const char *killerName=0,*victimName=0;DWORD killerColor=0,victimColor=0;
    if(killer!=0xffff) {
        if(killer==pool->localId){killerName=pool->localName.c_str();killerColor=pool->local->GetColorAsARGB();}
        else if(killer<1004&&pool->connected[killer]){killerName=pool->GetName(killer);killerColor=pool->GetPlayer(killer)->GetColorAsARGB();}
    }
    if(victim==pool->localId){victimName=pool->localName.c_str();victimColor=pool->local->GetColorAsARGB();}
    else if(victim<1004&&pool->connected[victim]) {
        victimName=pool->GetName(victim);CPlayerInfo *info=pool->players[victim];
        CRemotePlayer *remote=info?info->remote:0;victimColor=remote->GetColorAsARGB();
    }
    if(g_deviceDeathWindow)((CDeathWindow*)g_deviceDeathWindow)->AddMessage(killerName,victimName,killerColor,victimColor,weapon);
}
// samp.dll RVA: 0x00118494
const char g_rpcLargeTextDrawWarning[]="Warning: ignoring large TextDraw size=%u";
// samp.dll RVA: 0x0001c390
void ScrInitMenu(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    if(!g_pNetGame)return;
    CMenuPool *pool=((CNetGame*)g_pNetGame)->pools->menus;
    BYTE id;int twoColumns;char text[32];float x,y,width1,width2=0;DWORD interaction[13];
    stream.Read(id);stream.Read(twoColumns);stream.Read(text,32);
    stream.Read(x);stream.Read(y);stream.Read(width1);if(twoColumns)stream.Read(width2);
    stream.Read(interaction[0]);for(int i=1;i<13;i++)stream.Read(interaction[i]);
    // Native includes ID128 in this probe; adjacent allocation bytes are unproven.
    if(id<=128&&pool->active[id])pool->Delete(id);
    CMenu *menu=pool->Create(id,text,x,y,(BYTE)((BYTE)twoColumns+1),width1,width2,interaction);
    if(menu) {
        stream.Read(text,32);menu->SetColumnTitle(0,text);BYTE count;stream.Read(count);
        for(BYTE row=0;row<count;row++){stream.Read(text,32);menu->AddItem(0,row,text);}
        if(twoColumns) {
            stream.Read(text,32);menu->SetColumnTitle(1,text);stream.Read(count);
            for(BYTE row=0;row<count;row++){stream.Read(text,32);menu->AddItem(1,row,text);}
        }
    }
}
// samp.dll RVA: 0x0001c650
void ScrShowMenu(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    if(g_pNetGame){BYTE id;stream.Read(id);((CNetGame*)g_pNetGame)->pools->menus->Show(id);}
}
// samp.dll RVA: 0x0001c700
void ScrHideMenu(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    if(g_pNetGame){BYTE id;stream.Read(id);((CNetGame*)g_pNetGame)->pools->menus->Hide(id);}
}
// samp.dll RVA: 0x0001c850
void ScrShowTextDraw(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD length=0;CTextDrawPool *pool=((CNetGame*)g_pNetGame)->pools->textDraws;
    if(pool) {
        WORD id;TextDrawTransmit info;char text[800];stream.Read(id);stream.Read((char*)&info,63);stream.Read(length);
        if(length<800){stream.Read(text,length);text[length]=0;pool->Create(id,&info,text);}
        else if(g_pChat)g_pChat->AddInfoMessage(g_rpcLargeTextDrawWarning,(unsigned)length);
    }
}
// samp.dll RVA: 0x0001c980
void ScrHideTextDrawForPlayer(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CTextDrawPool *pool=((CNetGame*)g_pNetGame)->pools->textDraws;
    if(pool){WORD id;stream.Read(id);pool->Delete(id);}
}
// samp.dll RVA: 0x0001ca30
void ScrTextDrawSetString(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    WORD length=0;CTextDrawPool *pool=((CNetGame*)g_pNetGame)->pools->textDraws;
    if(pool) {
        WORD id;char text[800];stream.Read(id);stream.Read(length);
        // Native accepts800 and writes its terminator one byte beyond this buffer.
        if(length<=800){stream.Read(text,length);text[length]=0;if(id<2304&&pool->active[id]&&pool->draws[id])pool->draws[id]->SetText(text);}
    }
}
// samp.dll RVA: 0x00019e60
void ScrPlaySound(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    int sound;CVector position;stream.Read(sound);stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);
    g_pGame->m_pAudio->Play(sound,position);
}
// samp.dll RVA: 0x0001ce80
void ScrPlayAudioStream(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    char url[257]={0};BYTE length=0;CVector position={0,0,0};float radius=0;bool spatial=false;
    stream.Read(length);stream.Read(url,length);stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);stream.Read(radius);
    stream.ReadBits((BYTE*)&spatial,8,true);
    if(g_renderAudioStream)g_renderAudioStream->Play(url,position.x,position.y,position.z,radius,spatial);
}
// samp.dll RVA: 0x00017c00
void ScrStopAudioStream(RPCParameters *parameters) {(void)parameters;if(g_renderAudioStream)g_renderAudioStream->Stop(false);}
// samp.dll RVA: 0x000195d0
void ScrSetPlayerCameraPosition(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CVector position,rotation={0,0,0};stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);
    g_pGame->m_pCamera->Set(position,rotation);
}
// samp.dll RVA: 0x000196a0
void ScrSetPlayerCameraLookAt(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CVector position;BYTE style=0;stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);stream.Read(style);
    if(style<1||style>2)style=2;
    g_pGame->m_pCamera->PointAt(position,style);
}
// samp.dll RVA: 0x00017b90
void ScrSetCameraBehindPlayer(RPCParameters *parameters) {(void)parameters;g_pGame->m_pCamera->Restore();}
// samp.dll RVA: 0x00018e50
void ScrSetPlayerPosition(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CPlayerPool *players=((CNetGame*)g_pNetGame)->pools->players;
    if(players) {
        CLocalPlayer *local=players->local;
        if(local) {
            CVector position;stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);
            *(DWORD*)(local->nativeState+0x2e6)=0;local->SetSurfingEntity(0,0);
            (*(CPed**)local->nativeState)->Teleport(position);
        }
    }
}
// samp.dll RVA: 0x00018f70
void ScrSetPlayerPositionFindZ(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;
    CVector position;stream.Read(position.x);stream.Read(position.y);stream.Read(position.z);
    position.z=g_pGame->FindGroundZ(position.x,position.y,position.z)+1.5f;
    *(DWORD*)(local->nativeState+0x2e6)=0;local->SetSurfingEntity(0,0);
    (*(CPed**)local->nativeState)->Teleport(position);
}
// samp.dll RVA: 0x00018350
void ScrSetPlayerVelocity(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CVector velocity;stream.Read(velocity.x);stream.Read(velocity.y);stream.Read(velocity.z);
    CPed *ped=g_pGame->GetPlayerPed();
    if(ped){if(ped->IsOnGround())ped->SetFlags(ped->GetFlags()^3);ped->SetSpeed(velocity);}
}
// samp.dll RVA: 0x00019f40
void ScrSetPlayerWorldBounds(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+5,32,true);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+9,32,true);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0xd,32,true);
    stream.ReadBits(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x11,32,true);
}
// samp.dll RVA: 0x0001a0d0
void ScrSetPlayerFacingAngle(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    float angle;stream.Read(angle);g_pGame->GetPlayerPed()->SetRotation(angle);
}
// samp.dll RVA: 0x00017860
void ScrEnableStuntBonusForPlayer(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    bool enabled;stream.Read(enabled);g_pGame->EnableStuntBonus((BYTE)enabled);
}
// samp.dll RVA: 0x00017be0
void ScrForceClassSelection(RPCParameters *parameters) {
    (void)parameters;CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;
    *(DWORD*)(local->nativeState+0x2fe)=1;
}
// samp.dll RVA: 0x00017bb0
void ScrResetPlayerWeapons(RPCParameters *parameters) {
    (void)parameters;CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;
    (*(CPed**)local->nativeState)->ClearWeapons();
}
// samp.dll RVA: 0x0001a170
void ScrGivePlayerWeapon(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    int weapon,ammo;stream.Read(weapon);stream.Read(ammo);
    CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;
    (*(CPed**)local->nativeState)->GiveWeapon(weapon,ammo);
}
// samp.dll RVA: 0x0001cb40
void ScrGangZoneCreate(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CGangZonePool *zones=((CNetGame*)g_pNetGame)->pools->gangZones;
    if(zones) {
        WORD id;float minX,minY,maxX,maxY;DWORD color;
        stream.Read(id);stream.Read(minX);stream.Read(minY);stream.Read(maxX);stream.Read(maxY);stream.Read(color);
        zones->Create(id,minX,minY,maxX,maxY,color);
    }
}
// samp.dll RVA: 0x0001cc60
void ScrGangZoneDestroy(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CGangZonePool *zones=((CNetGame*)g_pNetGame)->pools->gangZones;
    if(zones){WORD id;stream.Read(id);zones->Delete(id);}
}
// samp.dll RVA: 0x0001cd10
void ScrGangZoneFlash(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CGangZonePool *zones=((CNetGame*)g_pNetGame)->pools->gangZones;
    if(zones){WORD id;DWORD color;stream.Read(id);stream.Read(color);zones->StartFlashing(id,color);}
}
// samp.dll RVA: 0x0001cdd0
void ScrGangZoneStopFlash(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    CGangZonePool *zones=((CNetGame*)g_pNetGame)->pools->gangZones;
    if(zones){WORD id;stream.Read(id);zones->StopFlashing(id);}
}
// samp.dll RVA: 0x000178f0
void ScrDisableVehicleCollisions(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    bool disabled=false;stream.Read(disabled);
    if(g_pNetGame)((BYTE*)g_pNetGame)[0x232]=(BYTE)disabled;
}
// samp.dll RVA: 0x0001a800
void ScrSetGravity(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    float gravity;stream.Read(gravity);g_pGame->SetWorldGravity(gravity);
}
// samp.dll RVA: 0x0001c7b0
void ScrSetPlayerWantedLevel(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    if(g_pGame){BYTE level;stream.Read(level);g_pGame->SetWantedLevel(level);}
}
// samp.dll RVA: 0x00019080
void ScrSetPlayerHealth(RPCParameters *parameters) {
    CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    float health;stream.Read(health);(*(CPed**)local->nativeState)->SetHealth(health);
}
// samp.dll RVA: 0x0001a690
void ScrSetPlayerArmor(RPCParameters *parameters) {
    CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    float armour;stream.Read(armour);(*(CPed**)local->nativeState)->SetArmour(armour);
}
// samp.dll RVA: 0x00019530
void ScrSetPlayerInterior(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE interior;stream.Read(interior);g_pGame->GetPlayerPed()->LinkToInterior(interior,TRUE);
}
// samp.dll RVA: 0x0001a030
void ScrGivePlayerMoney(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    int amount;stream.Read(amount);g_pGame->IncreasePlayerMoney(amount);
}
// samp.dll RVA: 0x00017ba0
void ScrResetPlayerMoney(RPCParameters *parameters) {(void)parameters;g_pGame->ResetMoney();}
// samp.dll RVA: 0x000181a0
void ScrSetPlayerSpecialAction(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE action;stream.Read(action);CPlayerPool *players=((CNetGame*)g_pNetGame)->pools->players;
    if(players)players->local->SetSpecialAction(action);
}
// samp.dll RVA: 0x00019dc0
void ScrTogglePlayerControllable(RPCParameters *parameters) {
    RakNet::BitStream stream(parameters->input,(int)parameters->numberOfBitsOfData/8+1,false);
    BYTE enabled;stream.Read(enabled);g_pGame->GetPlayerPed()->SetControllable(enabled);
}
// samp.dll RVA: 0x00118448
const int g_rpcScrDisableVehicleCollisions=167;
// samp.dll RVA: 0x00118444
const int g_rpcScrForceClassSelection=74;
// samp.dll RVA: 0x00118440
const int g_rpcScrAttachObjectToPlayer=75;
// samp.dll RVA: 0x0011843c
const int g_rpcScrInitMenu=76;
// samp.dll RVA: 0x00118438
const int g_rpcScrShowMenu=77;
// samp.dll RVA: 0x00118434
const int g_rpcScrHideMenu=78;
// samp.dll RVA: 0x00118430
const int g_rpcScrSetPlayerWantedLevel=133;
// samp.dll RVA: 0x0011842c
const int g_rpcScrShowTextDraw=134;
// samp.dll RVA: 0x00118428
const int g_rpcScrHideTextDrawForPlayer=135;
// samp.dll RVA: 0x00118424
const int g_rpcScrTextDrawSetString=105;
// samp.dll RVA: 0x00118420
const int g_rpcScrGangZoneCreate=108;
// samp.dll RVA: 0x0011841c
const int g_rpcScrGangZoneDestroy=120;
// samp.dll RVA: 0x00118418
const int g_rpcScrGangZoneFlash=121;
// samp.dll RVA: 0x00118414
const int g_rpcScrGangZoneStopFlash=85;
// samp.dll RVA: 0x00118410
const int g_rpcScrApplyAnimation=86;
// samp.dll RVA: 0x0011840c
const int g_rpcScrClearAnimations=87;
// samp.dll RVA: 0x00118408
const int g_rpcScrSetPlayerSpecialAction=88;
// samp.dll RVA: 0x00118404
const int g_rpcScrEnableStuntBonusForPlayer=104;
// samp.dll RVA: 0x00118400
const int g_rpcScrSetPlayerFightingStyle=89;
// samp.dll RVA: 0x001183fc
const int g_rpcScrSetPlayerVelocity=90;
// samp.dll RVA: 0x001183f8
const int g_rpcScrSetVehicleVelocity=91;
// samp.dll RVA: 0x001183f4
const int g_rpcScrSetPlayerWidescreen=111;
// samp.dll RVA: 0x001183f0
const int g_rpcScrSetVehicleTires=98;
// Handler RVA0x00018750; semantic name not established.
// samp.dll RVA: 0x001183ec
const int g_rpcUnknown001183EC=92;
// Handler RVA0x00018800; semantic name not established.
// samp.dll RVA: 0x001183e8
const int g_rpcUnknown001183E8=150;
// samp.dll RVA: 0x001183e4
const int g_rpcScrPlayCrimeReport=112;
// samp.dll RVA: 0x001183e0
const int g_rpcScrSetPlayerMapIcon=56;
// samp.dll RVA: 0x001183dc
const int g_rpcScrRemovePlayerMapIcon=144;
// samp.dll RVA: 0x001183d8
const int g_rpcScrSetPlayerAmmo=145;
// samp.dll RVA: 0x001183d4
const int g_rpcScrSetGravity=146;
// samp.dll RVA: 0x001183d0
const int g_rpcScrSetVehicleHealth=147;
// samp.dll RVA: 0x001183cc
const int g_rpcScrAttachTrailerToVehicle=148;
// samp.dll RVA: 0x001183c8
const int g_rpcScrDetachTrailerFromVehicle=149;
// samp.dll RVA: 0x001183c4
const int g_rpcScrCreateObject=44;
// samp.dll RVA: 0x001183c0
const int g_rpcScrSetObjectPosition=45;
// samp.dll RVA: 0x001183bc
const int g_rpcScrSetObjectRotation=46;
// samp.dll RVA: 0x00117694
const int g_rpcScrDestroyObject=47;
// samp.dll RVA: 0x001183b8
const int g_rpcScrCreateExplosion=79;
// samp.dll RVA: 0x001183b4
const int g_rpcScrShowPlayerNametagForPlayer=80;
// samp.dll RVA: 0x001183b0
const int g_rpcScrMoveObject=99;
// samp.dll RVA: 0x001183ac
const int g_rpcScrStopObject=122;
// samp.dll RVA: 0x001183a8
const int g_rpcScrSetNumberPlate=123;
// samp.dll RVA: 0x001183a4
const int g_rpcScrTogglePlayerSpectating=124;
// Handler RVA0x00017bd0; semantic name not established.
// samp.dll RVA: 0x001183a0
const int g_rpcUnknown001183A0=125;
// samp.dll RVA: 0x0011839c
const int g_rpcScrPlayerSpectatePlayer=126;
// samp.dll RVA: 0x00118398
const int g_rpcScrPlayerSpectateVehicle=127;
// samp.dll RVA: 0x00118394
const int g_rpcScrMoveVehicleComponent=57;
// samp.dll RVA: 0x00118390
const int g_rpcScrSetPlayerShopName=33;
// samp.dll RVA: 0x0011838c
const int g_rpcScrSetPlayerDrunkLevel=35;
// samp.dll RVA: 0x00118388
const int g_rpcScrSetPlayerArmedWeapon=67;
// samp.dll RVA: 0x00118384
const int g_rpcScrSetPlayerAttachedObject=113;
// samp.dll RVA: 0x00118380
const int g_rpcScrPlayAudioStream=41;
// samp.dll RVA: 0x0011837c
const int g_rpcScrStopAudioStream=42;
// samp.dll RVA: 0x00118378
const int g_rpcScrRemoveBuildingForPlayer=43;
// samp.dll RVA: 0x00118374
const int g_rpcScrSetSpawnInfo=68;
// samp.dll RVA: 0x00118370
const int g_rpcScrSetPlayerTeam=69;
// samp.dll RVA: 0x0011836c
const int g_rpcScrSetPlayerName=11;
// samp.dll RVA: 0x00118368
const int g_rpcScrSetPlayerSkin=153;
// samp.dll RVA: 0x00118364
const int g_rpcScrSetPlayerPosition=12;
// samp.dll RVA: 0x00118360
const int g_rpcScrSetPlayerPositionFindZ=13;
// samp.dll RVA: 0x0011835c
const int g_rpcScrSetPlayerHealth=14;
// samp.dll RVA: 0x00118358
const int g_rpcScrPutPlayerInVehicle=70;
// samp.dll RVA: 0x00118354
const int g_rpcScrRemovePlayerFromVehicle=71;
// samp.dll RVA: 0x00118350
const int g_rpcScrSetPlayerColor=72;
// samp.dll RVA: 0x0011834c
const int g_rpcScrDisplayGametext=73;
// samp.dll RVA: 0x00118348
const int g_rpcScrSetPlayerInterior=156;
// samp.dll RVA: 0x00118344
const int g_rpcScrSetPlayerCameraPosition=157;
// samp.dll RVA: 0x00118340
const int g_rpcScrSetPlayerCameraLookAt=158;
// samp.dll RVA: 0x0011833c
const int g_rpcScrSetVehiclePosition=159;
// samp.dll RVA: 0x00118338
const int g_rpcScrSetVehicleZAngle=160;
// samp.dll RVA: 0x00118334
const int g_rpcScrSetVehicleParamsForPlayer=161;
// samp.dll RVA: 0x00118330
const int g_rpcScrSetCameraBehindPlayer=162;
// samp.dll RVA: 0x0011832c
const int g_rpcScrTogglePlayerControllable=15;
// samp.dll RVA: 0x00118328
const int g_rpcScrPlaySound=16;
// samp.dll RVA: 0x00118324
const int g_rpcScrSetPlayerWorldBounds=17;
// samp.dll RVA: 0x00118320
const int g_rpcScrGivePlayerMoney=18;
// samp.dll RVA: 0x0011831c
const int g_rpcScrSetPlayerFacingAngle=19;
// samp.dll RVA: 0x00118318
const int g_rpcScrResetPlayerMoney=20;
// samp.dll RVA: 0x00118314
const int g_rpcScrResetPlayerWeapons=21;
// samp.dll RVA: 0x00118310
const int g_rpcScrGivePlayerWeapon=22;
// Handler RVA0x0001a230; semantic name not established.
// samp.dll RVA: 0x0011830c
const int g_rpcUnknown0011830C=64;
// samp.dll RVA: 0x00118308
const int g_rpcScrLinkVehicleToInterior=65;
// samp.dll RVA: 0x00118304
const int g_rpcScrDeathMessage=55;
// samp.dll RVA: 0x00118300
const int g_rpcScrAttachCameraToObject=81;
// samp.dll RVA: 0x001182fc
const int g_rpcScrInterpolateCamera=82;
// samp.dll RVA: 0x001182f4
const int g_rpcScrSetObjectMaterial=84;
// samp.dll RVA: 0x001182f0
const int g_rpcScrStopObjectCameraCollision=169;
// samp.dll RVA: 0x001182ec
const int g_rpcScrSetActorAnimation=173;
// samp.dll RVA: 0x001182e8
const int g_rpcScrClearActorAnimations=174;
// samp.dll RVA: 0x001182e4
const int g_rpcScrSetActorRotation=175;
// samp.dll RVA: 0x001182e0
const int g_rpcScrSetActorPosition=176;
// samp.dll RVA: 0x001182dc
const int g_rpcScrSetActorHealth=178;
// Handler RVA0x0001d780; semantic name not established.
// samp.dll RVA: 0x001182d8
const int g_rpcUnknown001182D8=48;
// samp.dll RVA: 0x001184c0
const int g_rpcScrSetPlayerArmor=66;
// samp.dll RVA: 0x0001dbf0
void RegisterScriptRPCs(RakClientInterface *client) {
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDisableVehicleCollisions),ScrDisableVehicleCollisions);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerMapIcon),ScrSetPlayerMapIcon);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrRemovePlayerMapIcon),ScrRemovePlayerMapIcon);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerAmmo),ScrSetPlayerAmmo);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetGravity),ScrSetGravity);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetVehicleHealth),ScrSetVehicleHealth);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrAttachTrailerToVehicle),ScrAttachTrailerToVehicle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDetachTrailerFromVehicle),ScrDetachTrailerFromVehicle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrCreateObject),ScrCreateObject);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetObjectPosition),ScrSetObjectPosition);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetObjectRotation),ScrSetObjectRotation);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDestroyObject),ScrDestroyObject);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrCreateExplosion),ScrCreateExplosion);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrShowPlayerNametagForPlayer),ScrShowPlayerNametagForPlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrMoveObject),ScrMoveObject);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrStopObject),ScrStopObject);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetNumberPlate),ScrSetNumberPlate);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrTogglePlayerSpectating),ScrTogglePlayerSpectating);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcUnknown001183A0),ScrRpc125);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrPlayerSpectatePlayer),ScrPlayerSpectatePlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrPlayerSpectateVehicle),ScrPlayerSpectateVehicle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrMoveVehicleComponent),ScrMoveVehicleComponent);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrForceClassSelection),ScrForceClassSelection);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrAttachObjectToPlayer),ScrAttachObjectToPlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrInitMenu),ScrInitMenu);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrShowMenu),ScrShowMenu);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrHideMenu),ScrHideMenu);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerWantedLevel),ScrSetPlayerWantedLevel);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrShowTextDraw),ScrShowTextDraw);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrHideTextDrawForPlayer),ScrHideTextDrawForPlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrTextDrawSetString),ScrTextDrawSetString);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrGangZoneCreate),ScrGangZoneCreate);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrGangZoneDestroy),ScrGangZoneDestroy);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrGangZoneFlash),ScrGangZoneFlash);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrGangZoneStopFlash),ScrGangZoneStopFlash);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrApplyAnimation),ScrApplyAnimation);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrClearAnimations),ScrClearAnimations);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerSpecialAction),ScrSetPlayerSpecialAction);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrEnableStuntBonusForPlayer),ScrEnableStuntBonusForPlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerFightingStyle),ScrSetPlayerFightingStyle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerVelocity),ScrSetPlayerVelocity);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetVehicleVelocity),ScrSetVehicleVelocity);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerWidescreen),ScrSetPlayerWidescreen);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetVehicleTires),ScrSetVehicleTires);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcUnknown001183EC),ScrPlayerOpcode052C);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcUnknown001183E8),ScrPlayerOpcode03FD);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrPlayCrimeReport),ScrPlayCrimeReport);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetSpawnInfo),ScrSetSpawnInfo);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerTeam),ScrSetPlayerTeam);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerSkin),ScrSetPlayerSkin);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerName),ScrSetPlayerName);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerPosition),ScrSetPlayerPosition);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerPositionFindZ),ScrSetPlayerPositionFindZ);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerHealth),ScrSetPlayerHealth);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrPutPlayerInVehicle),ScrPutPlayerInVehicle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrRemovePlayerFromVehicle),ScrRemovePlayerFromVehicle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerColor),ScrSetPlayerColor);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDisplayGametext),ScrDisplayGametext);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerInterior),ScrSetPlayerInterior);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerCameraPosition),ScrSetPlayerCameraPosition);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerCameraLookAt),ScrSetPlayerCameraLookAt);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetVehiclePosition),ScrSetVehiclePosition);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetVehicleZAngle),ScrSetVehicleZAngle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetVehicleParamsForPlayer),ScrSetVehicleParamsForPlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetCameraBehindPlayer),ScrSetCameraBehindPlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrTogglePlayerControllable),ScrTogglePlayerControllable);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrPlaySound),ScrPlaySound);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerWorldBounds),ScrSetPlayerWorldBounds);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrGivePlayerMoney),ScrGivePlayerMoney);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerFacingAngle),ScrSetPlayerFacingAngle);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrResetPlayerMoney),ScrResetPlayerMoney);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrResetPlayerWeapons),ScrResetPlayerWeapons);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrGivePlayerWeapon),ScrGivePlayerWeapon);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcUnknown0011830C),ScrRpc64);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrLinkVehicleToInterior),ScrLinkVehicleToInterior);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerArmor),ScrSetPlayerArmor);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrDeathMessage),ScrDeathMessage);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerShopName),ScrSetPlayerShopName);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerDrunkLevel),ScrSetPlayerDrunkLevel);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerArmedWeapon),ScrSetPlayerArmedWeapon);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetPlayerAttachedObject),ScrSetPlayerAttachedObject);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrPlayAudioStream),ScrPlayAudioStream);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrStopAudioStream),ScrStopAudioStream);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrRemoveBuildingForPlayer),ScrRemoveBuildingForPlayer);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrAttachCameraToObject),ScrAttachCameraToObject);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrInterpolateCamera),ScrInterpolateCamera);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_textDrawClickRpc),ClickTextDraw);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetObjectMaterial),ScrSetObjectMaterial);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrStopObjectCameraCollision),ScrStopObjectCameraCollision);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetActorAnimation),ScrSetActorAnimation);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrClearActorAnimations),ScrClearActorAnimations);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetActorRotation),ScrSetActorRotation);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetActorPosition),ScrSetActorPosition);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcScrSetActorHealth),ScrSetActorHealth);
    client->RegisterAsRemoteProcedureCall(const_cast<int*>(&g_rpcUnknown001182D8),ScrRpc48);
}
// samp.dll RVA: 0x00017c20
void UnRegisterScriptRPCs(RakClientInterface *client) {
    const int *ids[]={
        &g_rpcScrDisableVehicleCollisions,&g_rpcScrForceClassSelection,&g_rpcScrAttachObjectToPlayer,&g_rpcScrInitMenu,
        &g_rpcScrShowMenu,&g_rpcScrHideMenu,&g_rpcScrSetPlayerWantedLevel,&g_rpcScrShowTextDraw,
        &g_rpcScrHideTextDrawForPlayer,&g_rpcScrTextDrawSetString,&g_rpcScrGangZoneCreate,&g_rpcScrGangZoneDestroy,
        &g_rpcScrGangZoneFlash,&g_rpcScrGangZoneStopFlash,&g_rpcScrApplyAnimation,&g_rpcScrClearAnimations,
        &g_rpcScrSetPlayerSpecialAction,&g_rpcScrEnableStuntBonusForPlayer,&g_rpcScrSetPlayerFightingStyle,&g_rpcScrSetPlayerVelocity,
        &g_rpcScrSetVehicleVelocity,&g_rpcScrSetPlayerWidescreen,&g_rpcScrSetVehicleTires,&g_rpcUnknown001183EC,
        &g_rpcUnknown001183E8,&g_rpcScrPlayCrimeReport,&g_rpcScrSetPlayerMapIcon,&g_rpcScrRemovePlayerMapIcon,
        &g_rpcScrSetPlayerAmmo,&g_rpcScrSetGravity,&g_rpcScrSetVehicleHealth,&g_rpcScrAttachTrailerToVehicle,
        &g_rpcScrDetachTrailerFromVehicle,&g_rpcScrCreateObject,&g_rpcScrSetObjectPosition,&g_rpcScrSetObjectRotation,
        &g_rpcScrDestroyObject,&g_rpcScrCreateExplosion,&g_rpcScrShowPlayerNametagForPlayer,&g_rpcScrMoveObject,
        &g_rpcScrStopObject,&g_rpcScrSetNumberPlate,&g_rpcScrTogglePlayerSpectating,&g_rpcUnknown001183A0,
        &g_rpcScrPlayerSpectatePlayer,&g_rpcScrPlayerSpectateVehicle,&g_rpcScrMoveVehicleComponent,&g_rpcScrSetPlayerShopName,
        &g_rpcScrSetPlayerDrunkLevel,&g_rpcScrSetPlayerArmedWeapon,&g_rpcScrSetPlayerAttachedObject,&g_rpcScrPlayAudioStream,
        &g_rpcScrStopAudioStream,&g_rpcScrRemoveBuildingForPlayer,&g_rpcScrSetSpawnInfo,&g_rpcScrSetPlayerTeam,
        &g_rpcScrSetPlayerName,&g_rpcScrSetPlayerSkin,&g_rpcScrSetPlayerPosition,&g_rpcScrSetPlayerPositionFindZ,
        &g_rpcScrSetPlayerHealth,&g_rpcScrPutPlayerInVehicle,&g_rpcScrRemovePlayerFromVehicle,&g_rpcScrSetPlayerColor,
        &g_rpcScrDisplayGametext,&g_rpcScrSetPlayerInterior,&g_rpcScrSetPlayerCameraPosition,&g_rpcScrSetPlayerCameraLookAt,
        &g_rpcScrSetVehiclePosition,&g_rpcScrSetVehicleZAngle,&g_rpcScrSetVehicleParamsForPlayer,&g_rpcScrSetCameraBehindPlayer,
        &g_rpcScrTogglePlayerControllable,&g_rpcScrPlaySound,&g_rpcScrSetPlayerWorldBounds,&g_rpcScrGivePlayerMoney,
        &g_rpcScrSetPlayerFacingAngle,&g_rpcScrResetPlayerMoney,&g_rpcScrResetPlayerWeapons,&g_rpcScrGivePlayerWeapon,
        &g_rpcUnknown0011830C,&g_rpcScrLinkVehicleToInterior,&g_rpcScrDeathMessage,&g_rpcScrAttachCameraToObject,
        &g_rpcScrInterpolateCamera,&g_textDrawClickRpc,&g_rpcScrSetObjectMaterial,&g_rpcScrStopObjectCameraCollision,
        &g_rpcScrSetActorAnimation,&g_rpcScrClearActorAnimations,&g_rpcScrSetActorRotation,&g_rpcScrSetActorPosition,
        &g_rpcScrSetActorHealth,&g_rpcUnknown001182D8,
    };
    for(unsigned i=0;i<sizeof(ids)/sizeof(ids[0]);i++)client->UnregisterAsRemoteProcedureCall(const_cast<int*>(ids[i]));
}
