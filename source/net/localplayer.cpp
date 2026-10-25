#include "net/local_rpc.h"
#include "net/localplayer.h"
#include "net/vehiclepool.h"
#include "net/playerpool.h"
#include "net/actorpool.h"
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
#include "net/playerinfo.h"
#include "net/remoteplayer.h"
#include "game/aimstuff.h"
#include "game/quaternion.h"
#include "gui/device_lifecycle.h"
#include "cmdwindow.h"
#include "game/gta_globals.h"
// VC7 runtime helper; library entry, not a recovered SA-MP method.
extern "C" __int64 __cdecl _ftol2();
#pragma pack(push,1)
struct LocalOnfootSyncData {
    WORD leftRight,upDown,keys;CVector position;GameQuaternion rotation;
    BYTE health,armour,weaponAim,specialAction;CVector speed,surfingOffset;WORD surfingVehicle;DWORD animation;
};
struct LocalIncarSyncData {
    WORD vehicle,leftRight,upDown,keys;GameQuaternion rotation;CVector position,speed;float health;
    BYTE pedHealth,armour,weaponAim,siren,landingGear;WORD trailer;DWORD modelData;
};
#pragma pack(pop)
typedef char LocalOnfootSyncExtent[(sizeof(LocalOnfootSyncData)==68)?1:-1];
typedef char LocalIncarSyncExtent[(sizeof(LocalIncarSyncData)==63)?1:-1];
// samp.dll RVA: 0x00117944
const float g_localQuaternionSyncThreshold=0.00001f;
// samp.dll RVA: 0x00003390
BYTE CLocalPlayer::GetSpecialAction() {
    if(GetPed()->HasJetpack())return 2;
    if(GetPed()->DoesDancing()) {DWORD style=*(DWORD*)((BYTE*)GetPed()+0x2d6);if(style<=3)return (BYTE)(style+5);}
    if(GetPed()->DoesHandsUp())return 10;
    if(GetPed()->UsingCellphone())return 11;
    if(GetPed()->DoesUrinating())return 68;
    if(GetPed()->IsDucking())return 1;
    if(GetPed()->GetVehicleTransitionTaskState())return 3;
    if(GetPed()->HasTaskType704())return 4;
    if(GetPed()->GetStuff()) {
        if(GetPed()->GetStuff()==1)return 20;
        if(GetPed()->GetStuff()==2)return 22;
        if(GetPed()->GetStuff()==3)return 23;
        if(GetPed()->GetStuff()==4)return 21;
    }
    if(GetPed()->HasDanceState())return 24;
    return GetPed()->HasDanceStyle()?25:0;
}
// samp.dll RVA: 0x00004db0
void CLocalPlayer::SendOnfootData() {
    RakNet::BitStream stream;WORD leftRight,upDown;WORD keys=GetPed()->GetKeys(&leftRight,&upDown);
    BYTE aim=GetPed()->GetAim();CMatrix matrix;CVector speed;GetPed()->GetMatrix(&matrix);GetPed()->GetSpeed(&speed);
    LocalOnfootSyncData data;data.leftRight=leftRight;data.upDown=upDown;data.keys=keys;data.position=matrix.position;
    data.weaponAim=(BYTE)(aim<<6);GameQuaternionFromMatrixComponents(&matrix,&data.rotation);GameQuaternionNormalize(&data.rotation);
    float *current=(float*)&data.rotation;float *previous=(float*)(nativeState+0x4c);int keepPrevious=1;
    for(int i=0;i<4;++i) {
        // Push the native float words unchanged, including exceptional bit patterns.
        DWORD first=((DWORD*)current)[i],second=((DWORD*)previous)[i];int close=0;
        __asm {
            push second
            push first
            call FloatOffset
            add esp,8
            fcomp dword ptr [g_localQuaternionSyncThreshold]
            fnstsw ax
            test ah,5
            jp onfoot_component_different
            mov close,1
        onfoot_component_different:
        }
        if(!close){keepPrevious=0;break;}
    }
    if(keepPrevious)memcpy(&data.rotation,previous,16);
    CLocalPlayer *self=this;DWORD health,armour;
    __asm {
        mov ecx,self
        mov ecx,[ecx]
        call CPed::GetHealth
        call _ftol2
        mov health,eax
        mov ecx,self
        mov ecx,[ecx]
        call CPed::GetArmour
        call _ftol2
        mov armour,eax
    }
    data.health=(BYTE)health;data.armour=(BYTE)armour;data.weaponAim|=(BYTE)(GetPed()->GetCurrentWeapon()&0x3f);
    data.specialAction=GetSpecialAction();data.speed=speed;
    if(*(DWORD*)(nativeState+0x2e6)){memcpy(&data.surfingOffset,nativeState+0x2ea,12);data.surfingVehicle=*(WORD*)(nativeState+0x2d8);}
    else {data.surfingOffset.x=0;data.surfingOffset.y=0;data.surfingOffset.z=0;data.surfingVehicle=0;}
    data.animation=*(DWORD*)(nativeState+0x100);*(DWORD*)(nativeState+0x104)=1;
    if(HasDataChanged(nativeState+0x3a,&data,68)) {
        stream.Write((BYTE)207);stream.Write((char*)&data,68);
        (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->Send(&stream,HIGH_PRIORITY,UNRELIABLE_SEQUENCED,1);
        memcpy(nativeState+0x3a,&data,68);
    }
}
// samp.dll RVA: 0x00006e80
void CLocalPlayer::SendIncarData() {
    RakNet::BitStream stream;CVehiclePool *vehicles=((CNetGame*)g_pNetGame)->pools->vehicles;
    WORD leftRight,upDown;WORD keys=GetPed()->GetKeys(&leftRight,&upDown);BYTE aim=GetPed()->GetAim();
    LocalIncarSyncData data={0};if(!GetPed())return;
    data.vehicle=vehicles->Find(GetPed()->GetVehicle());if(data.vehicle==0xffff)return;
    data.leftRight=leftRight;data.upDown=upDown;data.keys=keys;data.weaponAim=(BYTE)(aim<<6);
    CVehicle *vehicle=vehicles->Get(data.vehicle);if(!vehicle)return;
    CMatrix matrix;CVector speed;GameQuaternion rotation;vehicle->GetMatrix(&matrix);vehicle->GetSpeed(&speed);
    GameQuaternionFromMatrixComponents(&matrix,&rotation);GameQuaternionNormalize(&rotation);data.rotation=rotation;
    float *current=(float*)&data.rotation;float *previous=(float*)(nativeState+0x9e);int keepPrevious=1;
    for(int i=0;i<4;++i) {
        // Push the native float words unchanged, including exceptional bit patterns.
        DWORD first=((DWORD*)current)[i],second=((DWORD*)previous)[i];int close=0;
        __asm {
            push second
            push first
            call FloatOffset
            add esp,8
            fcomp dword ptr [g_localQuaternionSyncThreshold]
            fnstsw ax
            test ah,5
            jp incar_component_different
            mov close,1
        incar_component_different:
        }
        if(!close){keepPrevious=0;break;}
    }
    if(keepPrevious)memcpy(&data.rotation,previous,16);
    data.position=matrix.position;data.speed=speed;data.health=vehicle->GetHealth();
    CLocalPlayer *self=this;DWORD health,armour;
    __asm {
        mov ecx,self
        mov ecx,[ecx]
        call CPed::GetHealth
        call _ftol2
        mov health,eax
        mov ecx,self
        mov ecx,[ecx]
        call CPed::GetArmour
        call _ftol2
        mov armour,eax
    }
    data.pedHealth=(BYTE)health;data.armour=(BYTE)armour;data.weaponAim|=(BYTE)(GetPed()->GetCurrentWeapon()&0x3f);
    data.trailer=0;CVehicle *trailer=vehicle->GetTrailer();
    if(trailer&&trailer->m_pGameVehicle&&trailer->GetTractor()==vehicle) {
        vehicle->SetTrailer(trailer);data.trailer=vehicles->Find(trailer->m_pGameVehicle);
    }else vehicle->SetTrailer(0);
    if(vehicle->GetModelIndex()==520)data.modelData=vehicle->GetHydraThrusters();
    else if(vehicle->GetModelIndex()==538||vehicle->GetModelIndex()==537||vehicle->GetModelIndex()==449)*(float*)&data.modelData=vehicle->GetTrainSpeed();
    else if(vehicle->GetSubtype()==2||vehicle->GetSubtype()==6)*(float*)&data.modelData=vehicle->GetBackendFloat648();
    else data.modelData=0;
    data.siren=vehicle->SirenEnabled()!=0;data.landingGear=vehicle->GetLandingGearState()!=0;
    if(HasDataChanged(nativeState+0x96,&data,63)) {
        stream.Write((BYTE)200);stream.Write((char*)&data,63);
        (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->Send(&stream,HIGH_PRIORITY,UNRELIABLE_SEQUENCED,1);
        memcpy(nativeState+0x96,&data,63);
    }
    if(data.trailer&&data.trailer<2000)SendTrailerData(data.trailer);
    if(vehicle->HasTurret()||GetTickCount()-*(DWORD*)(nativeState+0x13b)>1000){*(DWORD*)(nativeState+0x13b)=GetTickCount();SendAimData();}
}
#pragma pack(push,1)
struct LocalAimSyncData {BYTE cameraMode;BYTE vectors[24];float aimZ;BYTE zoomWeapon,aspect;};
struct LocalTrailerSyncData {WORD vehicle;CVector position;GameQuaternion rotation;CVector speed,turnSpeed;};
struct LocalPassengerSyncData {WORD vehicle;BYTE seatFlags,weaponAim,health,armour;WORD leftRight,upDown,keys;CVector position;};
#pragma pack(pop)
typedef char LocalAimSyncExtent[(sizeof(LocalAimSyncData)==31)?1:-1];
typedef char LocalTrailerSyncExtent[(sizeof(LocalTrailerSyncData)==54)?1:-1];
typedef char LocalPassengerSyncExtent[(sizeof(LocalPassengerSyncData)==24)?1:-1];
// samp.dll RVA: 0x00117948
const float g_localAimZoomScale=63.0f;
// samp.dll RVA: 0x0011794c
const float g_localAimAspectScale=255.0f;
// samp.dll RVA: 0x00117950
int g_localWastedRpc=53;
// samp.dll RVA: 0x000041c0
DWORD CLocalPlayer::NeedsToSendOnfootData(WORD keys,WORD upDown,WORD leftRight) {
    return keys!=*(WORD*)(nativeState+0x3e)||upDown!=*(WORD*)(nativeState+0x3c)||leftRight!=*(WORD*)(nativeState+0x3a)?1:0;
}
// samp.dll RVA: 0x000041f0
DWORD CLocalPlayer::NeedsToSendIncarData(WORD keys,WORD upDown,WORD leftRight) {
    return keys!=*(WORD*)(nativeState+0x9c)||upDown!=*(WORD*)(nativeState+0x9a)||leftRight!=*(WORD*)(nativeState+0x98)?1:0;
}
// samp.dll RVA: 0x00005090
void CLocalPlayer::SendAimData() {
    RakNet::BitStream stream;CameraAim *aim=GetPed()->GetCameraAim();LocalAimSyncData data;
    BYTE player=*((BYTE*)GetPed()+0x2b0);
    data.cameraMode=player?AimStuff::GetPlayerCameraMode(player):AimStuff::GetCameraMode();
    memcpy(data.vectors,aim,24);data.aimZ=GetPed()->GetAimZ();
    float zoom=AimStuff::GetCameraExtZoom();DWORD aspectValue,zoomValue;
    __asm {
        call AimStuff::GetAspectRatio
        fmul dword ptr [g_localAimAspectScale]
        call _ftol2
        mov aspectValue,eax
        fld dword ptr [zoom]
        fmul dword ptr [g_localAimZoomScale]
        call _ftol2
        mov zoomValue,eax
    }
    data.aspect=(BYTE)aspectValue;data.zoomWeapon=(BYTE)(zoomValue&0x3f);
    DWORD *slot=(DWORD*)GetPed()->GetCurrentWeaponSlot();DWORD weapon;
    if(slot[1]==2)weapon=3;else {weapon=slot[2];if(weapon>1)weapon=2;}
    data.zoomWeapon|=(BYTE)(weapon<<6);
    if(HasDataChanged(nativeState+0xd5,&data,31)) {
        stream.Write((BYTE)203);stream.Write((char*)&data,31);
        (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->Send(&stream,HIGH_PRIORITY,UNRELIABLE_SEQUENCED,1);
        memcpy(nativeState+0xd5,&data,31);
    }
}
// samp.dll RVA: 0x00005240
void CLocalPlayer::SendTrailerData(WORD id) {
    CVehiclePool *vehicles=((CNetGame*)g_pNetGame)->pools->vehicles;
    if(id>=2000||!vehicles->m_bNotEmpty[id])return;
    CVehicle *vehicle=vehicles->m_pObjects[id];if(!vehicle)return;
    CMatrix matrix;GameQuaternion rotation;vehicle->GetMatrix(&matrix);
    GameQuaternionFromMatrixComponents(&matrix,&rotation);GameQuaternionNormalize(&rotation);
    LocalTrailerSyncData data;data.rotation=rotation;data.position=matrix.position;data.vehicle=id;
    vehicle->GetSpeed(&data.speed);vehicle->GetTurnSpeed(&data.turnSpeed);
    if(HasDataChanged(nativeState+4,&data,54)) {
        RakNet::BitStream stream;stream.Write((BYTE)210);stream.Write((char*)&data,54);
        (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->Send(&stream,HIGH_PRIORITY,UNRELIABLE_SEQUENCED,1);
        memcpy(nativeState+4,&data,54);
    }
}
// samp.dll RVA: 0x00005400
void CLocalPlayer::SendPassengerData() {
    RakNet::BitStream stream;CVehiclePool *vehicles=((CNetGame*)g_pNetGame)->pools->vehicles;
    WORD leftRight,upDown;WORD keys=GetPed()->GetKeys(&leftRight,&upDown);BYTE aim=GetPed()->GetAim();
    LocalPassengerSyncData data={0};data.vehicle=vehicles->Find(GetPed()->GetVehicle());if(data.vehicle==0xffff)return;
    data.weaponAim=(BYTE)(aim<<6);data.leftRight=leftRight;data.upDown=upDown;data.keys=keys;
    CLocalPlayer *self=this;DWORD health,armour;
    __asm {
        mov ecx,self
        mov ecx,[ecx]
        call CPed::GetHealth
        call _ftol2
        mov health,eax
        mov ecx,self
        mov ecx,[ecx]
        call CPed::GetArmour
        call _ftol2
        mov armour,eax
    }
    data.health=(BYTE)health;data.armour=(BYTE)armour;
    data.seatFlags=(BYTE)(GetPed()->GetVehicleSeatIndex()&0x3f);
    if(GetPed()->HasDanceState())data.seatFlags|=0x80;
    data.seatFlags|=(BYTE)((nativeState[0x1cf]&1)<<6);
    data.weaponAim|=(BYTE)(GetPed()->GetCurrentWeapon()&0x3f);
    CMatrix matrix;GetPed()->GetMatrix(&matrix);data.position=matrix.position;
    if(HasDataChanged(nativeState+0x7e,&data,24)) {
        stream.Write((BYTE)211);stream.Write((char*)&data,24);
        (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->Send(&stream,HIGH_PRIORITY,UNRELIABLE_SEQUENCED,1);
        memcpy(nativeState+0x7e,&data,24);
    }
    DWORD delay=*(DWORD*)(nativeState+0x1cf)?100:1000;
    if(GetTickCount()-*(DWORD*)(nativeState+0x13b)>delay){*(DWORD*)(nativeState+0x13b)=GetTickCount();SendAimData();}
}
// samp.dll RVA: 0x00005660
void CLocalPlayer::WastedNotification() {
    RakNet::BitStream stream;WORD killer;BYTE reason=GetPed()->GetDeathInfo(&killer);
    stream.Write(reason);stream.Write(killer);
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(&g_localWastedRpc,&stream,HIGH_PRIORITY,RELIABLE_ORDERED,0,false);
}
// samp.dll RVA: 0x00005b50
void CLocalPlayer::SendStats() {
    RakNet::BitStream stream;int money=g_pGame->GetPlayerMoney();DWORD drunk=GetPed()->GetDrunkLevel();
    stream.Write((BYTE)205);stream.Write(money);stream.Write(drunk);
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->Send(&stream,HIGH_PRIORITY,UNRELIABLE,0);
}
#pragma pack(push,1)
struct UnoccupiedSyncData {
    WORD vehicle;BYTE seat;CVector right,up,position,speed,turnSpeed;float health;
};
#pragma pack(pop)
typedef char UnoccupiedSyncExtent[(sizeof(UnoccupiedSyncData)==67)?1:-1];
// samp.dll RVA: 0x00117978
const float g_localUnoccupiedRange=90.0f;
// samp.dll RVA: 0x00004bd0
void CLocalPlayer::SendUnoccupiedData(WORD id,BYTE seat) {
    CNetGamePools *pools=((CNetGame*)g_pNetGame)->pools;
    CPlayerPool *players=pools->players;CVehiclePool *vehicles=pools->vehicles;
    if(id>=2000||!vehicles->m_bNotEmpty[id])return;
    CVehicle *vehicle=vehicles->m_pObjects[id];RakNet::BitStream stream;
    CMatrix matrix;CVector speed,turnSpeed;vehicle->GetMatrix(&matrix);vehicle->GetSpeed(&speed);vehicle->GetTurnSpeed(&turnSpeed);
    UnoccupiedSyncData data;data.right=matrix.right;data.up=matrix.up;data.position=matrix.position;
    data.speed=speed;data.turnSpeed=turnSpeed;data.vehicle=id;data.seat=seat;data.health=vehicle->GetHealth();
    stream.Write((BYTE)209);stream.Write((char*)&data,67);
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->Send(&stream,HIGH_PRIORITY,UNRELIABLE_SEQUENCED,1);
    *(WORD*)((BYTE*)vehicles+0x8e34+id*2)=players->localId;*(DWORD*)((BYTE*)vehicle+0x7f)=1;
}
// samp.dll RVA: 0x00006c10
bool CLocalPlayer::ProcessUnoccupiedSync(WORD id,CVehicle *vehicle) {
    GetTickCount();BYTE *backend=(BYTE*)vehicle->m_pGameVehicle;
    CPlayerPool *players=((CNetGame*)g_pNetGame)->pools->players;
    if(!backend||!*(CPed**)nativeState||vehicle->IsTrainPart())return false;
    BYTE *driver=*(BYTE**)(backend+0x460);
    bool clear=driver&&(*(DWORD*)(driver+0x46c)&0x100);
    if(!clear) {
        int outsideRange=0;
        __asm {
            mov ecx,vehicle
            call CEntity::GetDistanceToLocalPlayer
            fcomp dword ptr [g_localUnoccupiedRange]
            fnstsw ax
            test ah,41h
            jne distance_accepted
            mov outsideRange,1
        distance_accepted:
        }
        clear=outsideRange||vehicle->IsStationary()||vehicle->GetTractor()!=0;
    }
    if(clear){*(DWORD*)((BYTE*)vehicle+0x7f)=0;*(DWORD*)((BYTE*)vehicle+0x83)=0;return false;}
    for(int seat=0;seat<8;++seat) {
        BYTE *passenger=*(BYTE**)(backend+0x464+seat*4);
        if(passenger&&!*(DWORD*)(passenger+0x598)) {
            if(passenger==*(BYTE**)((BYTE*)*(CPed**)nativeState+0x2a4)) {
                SendUnoccupiedData(id,(BYTE)(seat+1));return true;
            }
            return false;
        }
    }
    int closest=0,largest=players->largestId;
    if(largest<players->localId)largest=players->localId;
    float best=100000.0f;
    for(int index=0;index<=largest;++index) {
        CPed *ped;
        if(index==players->localId)ped=*(CPed**)nativeState;
        else {
            WORD player=(WORD)index;if(player>1004||!players->players[player]||!players->players[player]->remote)continue;
            ped=*(CPed**)(players->players[player]->remote->nativeState+4);
        }
        if(!ped||!ped->DoesExist())continue;
        int better=0;
        __asm {
            push vehicle
            mov ecx,ped
            call CPed::GetDistanceToEntity
            cmp index,0
            je seed_distance
            fcom dword ptr [best]
            fnstsw ax
            test ah,5
            jp reject_distance
            fstp dword ptr [best]
            mov better,1
            jmp distance_done
        seed_distance:
            fstp dword ptr [best]
            jmp distance_done
        reject_distance:
            fstp st(0)
        distance_done:
        }
        if(better)closest=index;
    }
    if(closest==players->localId&&best<=g_localUnoccupiedRange){SendUnoccupiedData(id,0);return true;}
    return false;
}
// samp.dll RVA: 0x0000e2b0
void CLocalPlayer::ClearSurfingState() {
    *(DWORD*)(nativeState+0x2e6)=0;SetSurfingEntity(0,0);
}
// samp.dll RVA: 0x00003fb0
void CLocalPlayer::PrepareForClassSelection_Outcome(BOOL outcome) {
    if(outcome) {
        CPed *ped=*(CPed**)nativeState;
        if(ped){ped->ClearWeapons();(*(CPed**)nativeState)->SetModelIndex(*(int*)(nativeState+0x150));}
        *(DWORD*)(nativeState+0x143)=1;
    }
}
// samp.dll RVA: 0x00003af0
void CLocalPlayer::SetSpawnInfo(const void *info) {
    const BYTE *data=(const BYTE*)info;
    for(int i=0;i<48;i+=4)*(DWORD*)(nativeState+0x14f+i)=*(const DWORD*)(data+i);
    *(WORD*)(nativeState+0x17f)=*(const WORD*)(data+48);*(DWORD*)(nativeState+0x181)=1;
}
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

// samp.dll RVA: 0x00117888
const char g_localWorldBoundaryText[]="Stay within the ~r~world boundries";
// samp.dll RVA: 0x00117968
int g_localVehicleDamageRpc=106;
// samp.dll RVA: 0x0011797c
const float g_localPassengerEntryDistance=8.0f;
// samp.dll RVA: 0x00002fd0
void CLocalPlayer::ProcessHead() {
    CPed *ped=*(CPed**)nativeState;if(!ped)return;
    if(g_pGame&&!*(DWORD*)((BYTE*)g_pGame+0x59))return;
    if(ped->GetStuff())return;
    if(g_deviceObjectEdit&&*(DWORD*)(g_deviceObjectEdit+0x80))return;
    CMatrix matrix;(*(CPed**)nativeState)->GetMatrix(&matrix);DWORD now=GetTickCount();CLocalPlayer *self=this;
    if(now-*(DWORD*)(nativeState+0x120)>1000) {
        CameraAim *aim=AimStuff::GetInternalAim();float z;
        __asm {
            mov eax,aim
            fld dword ptr [eax]
            fmul dword ptr [g_remoteHeadDistance]
            fadd dword ptr [matrix+48]
            fld dword ptr [eax+4]
            fmul dword ptr [g_remoteHeadDistance]
            fadd dword ptr [matrix+52]
            fld dword ptr [eax+8]
            mov ecx,self
            mov edx,now
            mov [ecx+120h],edx
            fmul dword ptr [g_remoteHeadDistance]
            fadd dword ptr [matrix+56]
            fstp dword ptr [z]
            fxch st(1)
            fsub dword ptr [matrix+48]
            fstp dword ptr [ecx+114h]
            fsub dword ptr [matrix+52]
            fstp dword ptr [ecx+118h]
            fld dword ptr [z]
            fsub dword ptr [matrix+56]
            fstp dword ptr [ecx+11ch]
        }
    }
    if(now-*(DWORD*)(nativeState+0x124)>200) {
        CVector target;ped=*(CPed**)nativeState;
        __asm {
            mov ecx,self
            fld dword ptr [matrix+48]
            fadd dword ptr [ecx+114h]
            fstp dword ptr [target]
            fld dword ptr [matrix+52]
            fadd dword ptr [ecx+118h]
            fstp dword ptr [target+4]
            fld dword ptr [matrix+56]
            fadd dword ptr [ecx+11ch]
            fstp dword ptr [target+8]
        }
        ped->ApplyCommandTask(g_remoteHeadTask,0,2000,-1,&target,0,0.1f,500,3,0);
        *(DWORD*)(nativeState+0x124)=now;
    }
}
// samp.dll RVA: 0x00003e40
void CLocalPlayer::ProcessWorldBoundaries() {
    if(g_pGame->GetCurrentInterior())return;
    BYTE *settings=*(BYTE**)((BYTE*)g_pNetGame+0x3d5);
    if((*(CPed**)nativeState)->EnforceWorldBoundries(*(float*)(settings+5),*(float*)(settings+9),*(float*)(settings+0xd),*(float*)(settings+0x11))) {
        (*(CPed**)nativeState)->SetArmedWeapon(0,0);
        g_pGame->DisplayGameText(g_localWorldBoundaryText,1000,5);
    }
}
// samp.dll RVA: 0x00003ea0
void CLocalPlayer::ProcessVehicleWorldBoundaries() {
    CVehiclePool *vehicles=((CNetGame*)g_pNetGame)->pools->vehicles;
    WORD id=vehicles->Find((*(CPed**)nativeState)->GetVehicle());
    if(g_pGame->GetCurrentInterior()||id==0xffff||id>=2000||!vehicles->m_bNotEmpty[id])return;
    CVehicle *vehicle=vehicles->m_pObjects[id];if(!vehicle)return;
    BYTE *settings=*(BYTE**)((BYTE*)g_pNetGame+0x3d5);
    if(vehicle->EnforceWorldBoundries(*(float*)(settings+5),*(float*)(settings+9),*(float*)(settings+0xd),*(float*)(settings+0x11)))
        g_pGame->DisplayGameText(g_localWorldBoundaryText,1000,5);
}
// samp.dll RVA: 0x00003f60
void CLocalPlayer::PrepareForClassSelection() {
    CPed *ped=*(CPed**)nativeState;*(DWORD*)(nativeState+0x143)=0;
    if(ped) {
        ped->SetInitialState();(*(CPed**)nativeState)->SetHealth(100.0f);(*(CPed**)nativeState)->SetControllable(FALSE);
    }
    DWORD (WINAPI *tick)()=GetTickCount;
    *(DWORD*)(nativeState+0x14b)=tick();*(DWORD*)(nativeState+0x147)=tick();
}
// samp.dll RVA: 0x00005c20
void CLocalPlayer::ProcessVehicleDamage(WORD id) {
    CVehiclePool *vehicles=((CNetGame*)g_pNetGame)->pools->vehicles;
    if(id>=2000||!vehicles->m_bNotEmpty[id])return;
    CVehicle *vehicle=vehicles->m_pObjects[id];if(!vehicle)return;
    if(id!=*(WORD*)(nativeState+0x31c)) {
        *(DWORD*)(nativeState+0x31e)=vehicle->GetPanelsDamage();
        *(DWORD*)(nativeState+0x322)=vehicle->GetDoorsDamage();
        nativeState[0x326]=vehicle->GetLightsDamage();nativeState[0x327]=vehicle->GetTires();
        *(WORD*)(nativeState+0x31c)=id;return;
    }
    if(*(DWORD*)(nativeState+0x31e)==vehicle->GetPanelsDamage()&&*(DWORD*)(nativeState+0x322)==vehicle->GetDoorsDamage()&&nativeState[0x326]==vehicle->GetLightsDamage()&&nativeState[0x327]==vehicle->GetTires())return;
    *(DWORD*)(nativeState+0x31e)=vehicle->GetPanelsDamage();
    *(DWORD*)(nativeState+0x322)=vehicle->GetDoorsDamage();
    nativeState[0x326]=vehicle->GetLightsDamage();nativeState[0x327]=vehicle->GetTires();
    RakNet::BitStream stream;
    WORD sentId=*(WORD*)(nativeState+0x31c);stream.WriteBits((const unsigned char*)&sentId,16,true);
    DWORD panels=*(DWORD*)(nativeState+0x31e);stream.WriteBits((const unsigned char*)&panels,32,true);
    DWORD doors=*(DWORD*)(nativeState+0x322);stream.WriteBits((const unsigned char*)&doors,32,true);
    BYTE lights=nativeState[0x326];stream.WriteBits(&lights,8,true);
    BYTE tires=nativeState[0x327];stream.WriteBits(&tires,8,true);
    typedef char VehicleDamageReliability[(HIGH_PRIORITY==1&&RELIABLE==8)?1:-1];
    (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(&g_localVehicleDamageRpc,&stream,HIGH_PRIORITY,RELIABLE,0,false);
}
// samp.dll RVA: 0x00006de0
void CLocalPlayer::ProcessPassengerEntry() {
    BYTE *keys=(BYTE*)KeyStuff::GetInternalKeys();WORD enter=*(WORD*)(keys+0x10);
    CVehiclePool *vehicles=((CNetGame*)g_pNetGame)->pools->vehicles;
    if(!enter||*(WORD*)(keys+0x40))return;
    WORD id=vehicles->FindNearestToLocalPlayer();if(id>=2000||!vehicles->m_bNotEmpty[id])return;
    CVehicle *vehicle=vehicles->Get(id);BYTE nearEnough=0;
    __asm {
        mov ecx,vehicle
        call CEntity::GetDistanceToLocalPlayer
        fcomp dword ptr [g_localPassengerEntryDistance]
        fnstsw ax
        test ah,5
        jp passenger_too_far
        mov nearEnough,1
    passenger_too_far:
    }
    if(!nearEnough)return;
    if((BYTE)(*(CPed**)nativeState)->GetCurrentWeapon()==46)(*(CPed**)nativeState)->SetArmedWeapon(0,0);
    (*(CPed**)nativeState)->EnterVehicle(vehicle->m_nHandle,TRUE);EnterVehicle(id,TRUE);
}
// samp.dll RVA: 0x00004a30
WORD CLocalPlayer::GetAimedPlayer() {
    CPed *ped=*(CPed**)nativeState;if(!ped)return 0xffff;
    void *target=ped->GetAimedPed();if(!target)return 0xffff;
    CPlayerPool *players=((CNetGame*)g_pNetGame)->pools->players;
    return players?players->Find(target):0xffff;
}
// samp.dll RVA: 0x00004a60
WORD CLocalPlayer::GetAimedActor() {
    CPed *ped=*(CPed**)nativeState;if(!ped)return 0xffff;
    void *target=ped->GetAimedPed();if(!target)return 0xffff;
    CActorPool *actors=((CNetGame*)g_pNetGame)->pools->actors;
    return actors?(WORD)actors->Find(target):0xffff;
}
// samp.dll RVA: 0x00117854
const Scripting::OpcodeInfo g_localPedCameraCommand={0x159,"iii"};
// samp.dll RVA: 0x00005f40
void CLocalPlayer::ProcessClassSelection() {
    if(!g_pGame->IsMenuVisible()) {g_pGame->EnableHUD(FALSE);g_pGame->SetCursorMode(2,FALSE);}
    if(g_renderDialog&&g_renderDialog->active)return;
    if(!*(DWORD*)(nativeState+0x30a)) {
        *((BYTE*)g_renderSecondDialog+0x13)=1;RECT rectangle;
        GetClientRect(g_inputGameWindow,&rectangle);
        BYTE *dialog=(BYTE*)g_renderSecondDialog;
        int y=rectangle.bottom-*(int*)(dialog+0x122)-50;
        int x=rectangle.right/2-*(int*)(dialog+0x11e)/2;
        *(int*)(dialog+0x11a)=y;*(int*)(dialog+0x116)=x;
        *(DWORD*)(nativeState+0x30a)=1;RequestClass(*(int*)(nativeState+0x302));return;
    }
    DWORD (WINAPI *tick)()=GetTickCount;
    if(tick()-*(DWORD*)(nativeState+0x14b)<2000)return;
    SHORT (WINAPI *keyState)(int)=GetAsyncKeyState;
    if(!*(DWORD*)(nativeState+0x306)) {
        if(!*(DWORD*)(nativeState+0x143))return;
        if(keyState(VK_SHIFT)<0&&!g_pInput->m_bEnabled){RequestSpawn();return;}
    }
    if(!*(DWORD*)(nativeState+0x143))return;
    DWORD elapsed=tick()-*(DWORD*)(nativeState+0x147);
    if(keyState(VK_LEFT)<0&&elapsed>250){*(DWORD*)(nativeState+0x147)=tick();PrevClass();return;}
    if(keyState(VK_RIGHT)<0&&elapsed>250){*(DWORD*)(nativeState+0x147)=tick();NextClass();}
}
// samp.dll RVA: 0x000060d0
void CLocalPlayer::UpdateWeapons() {
    BYTE weapon=(BYTE)(*(CPed**)nativeState)->GetCurrentWeapon();bool changed=false;
    WORD player=GetAimedPlayer();WORD actor=GetAimedActor();
    if(player!=*(WORD*)(nativeState+0x189)||actor!=*(WORD*)(nativeState+0x18b)) {
        *(WORD*)(nativeState+0x189)=player;*(WORD*)(nativeState+0x18b)=actor;changed=true;
    }else if(weapon==nativeState[0x18d]&&GetTickCount()-*(DWORD*)(nativeState+0x185)<100)return;
    if(weapon!=nativeState[0x18d])nativeState[0x18d]=weapon;
    RakNet::BitStream stream;BYTE packet=204;stream.WriteBits(&packet,8,true);
    WORD target=*(WORD*)(nativeState+0x189);stream.WriteBits((const unsigned char*)&target,16,true);
    target=*(WORD*)(nativeState+0x18b);stream.WriteBits((const unsigned char*)&target,16,true);
    for(BYTE index=0;index<13;++index) {
        BYTE *backend=(BYTE*)(*(CPed**)nativeState)->m_pGamePed;
        BYTE *slot=backend+index*0x1c;
        if((DWORD)nativeState[0x18e+index]!=*(DWORD*)(slot+0x5a0)||*(DWORD*)(nativeState+0x19b+index*4)!=*(DWORD*)(slot+0x5ac)) {
            nativeState[0x18e+index]=*(BYTE*)(slot+0x5a0);
            backend=(BYTE*)(*(CPed**)nativeState)->m_pGamePed;
            *(DWORD*)(nativeState+0x19b+index*4)=*(DWORD*)(backend+index*0x1c+0x5ac);
            BYTE sentIndex=index;stream.WriteBits(&sentIndex,8,true);
            BYTE type=nativeState[0x18e+index];stream.WriteBits(&type,8,true);
            WORD ammo=*(WORD*)(nativeState+0x19b+index*4);stream.WriteBits((const unsigned char*)&ammo,16,true);
            changed=true;
        }
    }
    if(changed) {
        *(DWORD*)(nativeState+0x185)=GetTickCount();
        (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->Send(&stream,HIGH_PRIORITY,UNRELIABLE,0);
    }
}
// samp.dll RVA: 0x00006360
void CLocalPlayer::ProcessSpectating() {
    RakNet::BitStream stream;WORD leftRight,upDown;
    WORD keys=(*(CPed**)nativeState)->GetKeys(&leftRight,&upDown);
    CMatrix matrix;g_pGame->m_pCamera->GetMatrix(&matrix);
    CNetGamePools *pools=((CNetGame*)g_pNetGame)->pools;
    CPlayerPool *players=pools->players;CVehiclePool *vehicles=pools->vehicles;
    if(!players||!vehicles)return;
    BYTE data[18];*(WORD*)data=leftRight;*(WORD*)(data+2)=upDown;*(WORD*)(data+4)=keys;
    memcpy(data+6,&matrix.position,12);
    DWORD rate=GetOnFootSendRate();DWORD (WINAPI *tick)()=GetTickCount;
    if(tick()-*(DWORD*)(nativeState+0x137)>rate) {
        *(DWORD*)(nativeState+0x137)=tick();BYTE packet=212;stream.WriteBits(&packet,8,true);
        stream.Write((const char*)data,18);
        (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->Send(&stream,HIGH_PRIORITY,UNRELIABLE,1);
        rate=GetOnFootSendRate()*2;
        if(tick()-*(DWORD*)(nativeState+0x13b)>rate){*(DWORD*)(nativeState+0x13b)=tick();SendAimData();}
    }
    g_pGame->EnableHUD(FALSE);(*(CPed**)nativeState)->SetHealth(100.0f);
    CVector position=*(CVector*)(data+6);
    // Native loads received Z before adding20; retain operand order for NaNs.
    __asm {
        fld dword ptr [position+8]
        fadd dword ptr [g_remoteHeadDistance]
        fstp dword ptr [position+8]
    }
    (*(CPed**)nativeState)->Teleport(position);
    if(nativeState[0x313]==1) {
        WORD id=*(WORD*)(nativeState+0x314);
        if(id>=1004||!players->connected[id]){nativeState[0x313]=0;*(DWORD*)(nativeState+0x318)=0;}
    }
    if(nativeState[0x313]==1) {
        WORD id=*(WORD*)(nativeState+0x314);
        if(id<1004&&players->connected[id]) {
            CRemotePlayer *remote=players->GetPlayer(id);
            if(!*(CPed**)(remote->nativeState+4)||!remote->nativeState[0x1a]||players->GetPlayer(id)->nativeState[0x1a]==32) {
                nativeState[0x313]=0;*(DWORD*)(nativeState+0x318)=0;
            }
        }
    }
    if(*(DWORD*)(nativeState+0x318))return;
    if(!nativeState[0x313]) {
        (*(CPed**)nativeState)->WarpFromVehicle(0.0f,0.0f,10.0f);
        CVector cameraPosition={50.0f,50.0f,50.0f},rotation={0,0,0};
        g_pGame->m_pCamera->Set(cameraPosition,rotation);
        CVector target={60.0f,60.0f,50.0f};g_pGame->m_pCamera->PointAt(target,2);
        *(DWORD*)(nativeState+0x318)=1;
    }else if(nativeState[0x313]==1) {
        WORD id=*(WORD*)(nativeState+0x314);if(!players->IsConnected(id))return;
        CPed *ped=*(CPed**)(players->GetPlayer(id)->nativeState+4);if(!ped)return;
        Scripting::ProcessCommand(&g_localPedCameraCommand,ped->m_nHandle,(DWORD)nativeState[0x312],2);
        *(DWORD*)(nativeState+0x318)=1;
    }else if(nativeState[0x313]==2) {
        WORD id=*(WORD*)(nativeState+0x314);if(!vehicles->DoesExist(id))return;
        CVehicle *vehicle=vehicles->Get(id);if(!vehicle)return;
        Scripting::ProcessCommand(&g_localTrainCameraCommand,vehicle->m_nHandle,(DWORD)nativeState[0x312],2);
        *(DWORD*)(nativeState+0x318)=1;
    }
}
