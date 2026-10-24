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
// Handler RVA0x000185c0; semantic name not established.
// samp.dll RVA: 0x001183f4
const int g_rpcUnknown001183F4=111;
// Handler RVA0x00018670; semantic name not established.
// samp.dll RVA: 0x001183f0
const int g_rpcUnknown001183F0=98;
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
// Handler RVA0x0001d3f0; semantic name not established.
// samp.dll RVA: 0x001182e8
const int g_rpcUnknown001182E8=174;
// samp.dll RVA: 0x001182e4
const int g_rpcScrSetActorRotation=175;
// samp.dll RVA: 0x001182e0
const int g_rpcScrSetActorPosition=176;
// samp.dll RVA: 0x001182dc
const int g_rpcScrSetActorHealth=178;
// Handler RVA0x0001d780; semantic name not established.
// samp.dll RVA: 0x001182d8
const int g_rpcUnknown001182D8=48;
// samp.dll RVA: 0x00017c20
void UnRegisterScriptRPCs(RakClientInterface *client) {
    const int *ids[]={
        &g_rpcScrDisableVehicleCollisions,&g_rpcScrForceClassSelection,&g_rpcScrAttachObjectToPlayer,&g_rpcScrInitMenu,
        &g_rpcScrShowMenu,&g_rpcScrHideMenu,&g_rpcScrSetPlayerWantedLevel,&g_rpcScrShowTextDraw,
        &g_rpcScrHideTextDrawForPlayer,&g_rpcScrTextDrawSetString,&g_rpcScrGangZoneCreate,&g_rpcScrGangZoneDestroy,
        &g_rpcScrGangZoneFlash,&g_rpcScrGangZoneStopFlash,&g_rpcScrApplyAnimation,&g_rpcScrClearAnimations,
        &g_rpcScrSetPlayerSpecialAction,&g_rpcScrEnableStuntBonusForPlayer,&g_rpcScrSetPlayerFightingStyle,&g_rpcScrSetPlayerVelocity,
        &g_rpcScrSetVehicleVelocity,&g_rpcUnknown001183F4,&g_rpcUnknown001183F0,&g_rpcUnknown001183EC,
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
        &g_rpcScrSetActorAnimation,&g_rpcUnknown001182E8,&g_rpcScrSetActorRotation,&g_rpcScrSetActorPosition,
        &g_rpcScrSetActorHealth,&g_rpcUnknown001182D8,
    };
    for(unsigned i=0;i<sizeof(ids)/sizeof(ids[0]);i++)client->UnregisterAsRemoteProcedureCall(const_cast<int*>(ids[i]));
}
