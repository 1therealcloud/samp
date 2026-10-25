#include "net/remoteplayer.h"
#include "net/vehiclepool.h"
#include "net/objectpool.h"
#include "game/playerped.h"
#include "game/util.h"
#include "main.h"
#include "net/netgame.h"
#include "game/quaternion.h"
#include "game/player_colors.h"
#include <string.h>
#include "net/playerpool.h"
#include "chatwindow.h"
#include "game/game.h"
#include "game/custommodels.h"
#include "game/aimstuff.h"
// samp.dll RVA: 0x001181c8
const float g_remoteAimZoomScale=0.01587301678955555f;
// samp.dll RVA: 0x001181cc
const float g_remoteAimAspectScale=0.003921568859368563f;
// samp.dll RVA: 0x00016c50
BOOL CRemotePlayer::Spawn(BYTE team,int model,int customModel,const CVector *position,float rotation,DWORD color,BYTE fightingStyle) {
    (void)team;if(!g_pGame->IsStarted())return FALSE;
    CPed *ped=*(CPed**)(nativeState+4);
    if(ped){g_pGame->RemovePed(ped);*(DWORD*)(nativeState+4)=0;}
    ped=g_pGame->CreatePed(model,*position,rotation,1,*(BOOL*)(nativeState+0x1c));
    if(!ped){if(nativeState[0x1a])nativeState[0x1a]=0;return FALSE;}
    if(g_customModels&&customModel>20000) {
        if(GameIsPedModel(customModel)||g_customModels->DoesExist(customModel))ped->SetModelIndex(customModel);
    }
    if(color)PlayerColors::Set(*(WORD*)nativeState,color);
    DWORD marker=*(DWORD*)(nativeState+0x1f9);
    if(marker){g_pGame->DeleteMarker(marker);*(DWORD*)(nativeState+0x1f9)=0;}
    if(*(DWORD*)(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+0x30))ped->CreateArrow(*(WORD*)nativeState);
    *(CPed**)(nativeState+4)=ped;*(float*)(nativeState+0x1b0)=100.0f;ped->SetKeys(0,0,0);
    if(fightingStyle!=4)ped->SetFightingStyle(fightingStyle);
    if(nativeState[0x1a]!=33)nativeState[0x1a]=33;
    return TRUE;
}
// samp.dll RVA: 0x00014660
void CRemotePlayer::OnEnterVehicle() {
    CVehiclePool *vehicles=((CNetGame*)g_pNetGame)->pools->vehicles;
    CPed *ped=*(CPed**)(nativeState+4);if(!ped||ped->IsInVehicle())return;
    CVehicle *vehicle=vehicles->Get(*(WORD*)(nativeState+2));if(!vehicle)return;
    BYTE weapon=(nativeState[0x1b]?nativeState[0x27]:nativeState[0xb6])&0x3f;
    if((BYTE)(*(CPed**)(nativeState+4))->GetCurrentWeapon()!=weapon) {
        (*(CPed**)(nativeState+4))->SetArmedWeapon(weapon,0);
        if((BYTE)(*(CPed**)(nativeState+4))->GetCurrentWeapon()!=weapon) {
            (*(CPed**)(nativeState+4))->GiveWeapon(weapon,9999);(*(CPed**)(nativeState+4))->SetArmedWeapon(weapon,0);
        }
    }
    DWORD ref=vehicles->GetRef((int)*(WORD*)(nativeState+2));BYTE seat=nativeState[0x1b];
    (*(CPed**)(nativeState+4))->PutIntoVehicle(ref,seat);
}
// samp.dll RVA: 0x00016da0
void CRemotePlayer::UpdateOnfoot(const void *data,DWORD timestamp) {
    const BYTE *input=(const BYTE*)data;*(DWORD*)(nativeState+8)=0;
    if(timestamp&&(int)(timestamp-*(DWORD*)(nativeState+0x1c9))<0)return;
    *(DWORD*)(nativeState+0x1c9)=timestamp;
    for(int i=0;i<68;i+=4)*(DWORD*)(nativeState+0x3c+i)=*(const DWORD*)(input+i);
    *(float*)(nativeState+0x1b0)=(float)input[0x22];*(float*)(nativeState+0x1ac)=(float)input[0x23];
    nativeState[0x1c4]=16;*(DWORD*)(nativeState+0x1c5)=GetTickCount();nativeState[0x18]=input[0x25];
    if(*(int*)(nativeState+0x7c)<0)*(DWORD*)(nativeState+0x7c)=0;
    CPed *ped=*(CPed**)(nativeState+4);
    if(ped&&ped->IsInVehicle()&&nativeState[0x18]!=3&&nativeState[0x18]!=4&&!(*(CPed**)(nativeState+4))->HasTaskType704())OnExitVehicle();
    if(nativeState[0x1a]!=17)nativeState[0x1a]=17;
}
// samp.dll RVA: 0x00016e70
void CRemotePlayer::UpdateIncar(const void *data,DWORD timestamp) {
    const BYTE *input=(const BYTE*)data;
    if(timestamp&&(int)(timestamp-*(DWORD*)(nativeState+0x1c9))<0)return;
    *(DWORD*)(nativeState+0x1c9)=timestamp;
    for(int i=0;i<60;i+=4)*(DWORD*)(nativeState+0x80+i)=*(const DWORD*)(input+i);
    *(WORD*)(nativeState+0xbc)=*(const WORD*)(input+60);nativeState[0xbe]=input[62];
    *(WORD*)(nativeState+2)=*(const WORD*)input;
    *(CVehicle**)(nativeState+8)=((CNetGame*)g_pNetGame)->pools->vehicles->Get(*(const WORD*)input);
    nativeState[0x1b]=0;*(float*)(nativeState+0x1b0)=(float)input[0x34];*(float*)(nativeState+0x1ac)=(float)input[0x35];
    nativeState[0x1c4]=17;*(DWORD*)(nativeState+0x1c5)=GetTickCount();nativeState[0x18]=0;
    CPed *ped=*(CPed**)(nativeState+4);if(ped&&!ped->IsInVehicle())OnEnterVehicle();
    BYTE oldState=nativeState[0x1a];if(oldState!=19){ChangeState(19,oldState);nativeState[0x1a]=19;}
}
// samp.dll RVA: 0x00016f60
void CRemotePlayer::UpdatePassenger(const void *data) {
    const BYTE *input=(const BYTE*)data;
    for(int i=0;i<24;i+=4)*(DWORD*)(nativeState+0x24+i)=*(const DWORD*)(input+i);
    *(WORD*)(nativeState+2)=*(const WORD*)input;nativeState[0x1b]=input[2]&0x3f;
    *(CVehicle**)(nativeState+8)=((CNetGame*)g_pNetGame)->pools->vehicles->Get(*(WORD*)(nativeState+2));
    *(float*)(nativeState+0x1b0)=(float)input[4];*(float*)(nativeState+0x1ac)=(float)input[5];
    nativeState[0x1c4]=18;*(DWORD*)(nativeState+0x1c5)=GetTickCount();nativeState[0x18]=0;
    CPed *ped=*(CPed**)(nativeState+4);if(ped&&!ped->IsInVehicle())OnEnterVehicle();
    if(nativeState[0x1a]!=18)nativeState[0x1a]=18;
}
// samp.dll RVA: 0x000154a0
void CRemotePlayer::UpdateAim(const void *data) {
    const BYTE *input=(const BYTE*)data;CPed *ped=*(CPed**)(nativeState+4);if(!ped)return;
    AimStuff::SetPlayerCameraMode(input[0],*((BYTE*)ped+0x2b0));
    CameraAim aim;memcpy(&aim.front,input+1,12);CVector direction;memcpy(&direction,input+1,12);CVector up={0,0,0};
    GameBuildAimUpVector(&direction,&up);memcpy(&aim.up,&up,12);
    memcpy(&aim.source,input+13,12);memcpy(&aim.sourceBeforeLookBehind,input+13,12);
    (*(CPed**)(nativeState+4))->SetAim(&aim);(*(CPed**)(nativeState+4))->SetAimZ(*(const float*)(input+25));
    int aspect=input[30],zoom=input[29]&0x3f;float aspectValue,zoomValue;
    __asm {
        fild dword ptr [aspect]
        fmul dword ptr [g_remoteAimAspectScale]
        fstp dword ptr [aspectValue]
        fild dword ptr [zoom]
        fmul dword ptr [g_remoteAimZoomScale]
        fstp dword ptr [zoomValue]
    }
    ped=*(CPed**)(nativeState+4);AimStuff::SetPlayerCamera(*((BYTE*)ped+0x2b0),zoomValue,aspectValue);
    DWORD *slot=(DWORD*)(*(CPed**)(nativeState+4))->GetCurrentWeaponSlot();BYTE weapon=input[29]>>6;
    if(weapon==3)slot[1]=2;else if(weapon!=2)slot[2]=weapon;else if(slot[2]<2)slot[2]=2;
}
// samp.dll RVA: 0x00014730
void CRemotePlayer::OnExitVehicle() {
    CPed *ped=*(CPed**)(nativeState+4);
    if(ped&&ped->IsInVehicle()) {
        CMatrix matrix;(*(CPed**)(nativeState+4))->GetMatrix(&matrix);
        (*(CPed**)(nativeState+4))->WarpFromVehicle(matrix.position.x,matrix.position.y,matrix.position.z);
        *(DWORD*)(nativeState+8)=0;
    }
}
// samp.dll RVA: 0x00017120
void CRemotePlayer::Chat(char *text) {
    WORD id=*(WORD*)nativeState;
    const char *name=((CNetGame*)g_pNetGame)->pools->players->GetName(id);
    DWORD color=(PlayerColors::Get(id)>>8)|0xff000000;
    g_pChat->AddChatMessage(name,color,text);
}
// samp.dll RVA: 0x001181e4
const float g_remoteAbsentDistance=10000.0f;
// samp.dll RVA: 0x001181e8
const char g_remoteCarJackedText[]="~r~Car Jacked~w~!";
// samp.dll RVA: 0x00016250
CRemotePlayer::CRemotePlayer() {
    BYTE *state=(BYTE*)this;DWORD (WINAPI *tick)()=GetTickCount;
    *(WORD*)(state+2)=0;state[0x1a]=0;*(WORD*)state=0xffff;*(DWORD*)(state+4)=0;
    state[0x1c4]=0;state[0x19]=0xff;*(DWORD*)(state+0x10)=1;
    *(DWORD*)(state+0xc)=0;*(DWORD*)(state+0x14)=0;*(DWORD*)(state+0x1f9)=0;
    state[0x18]=0;*(DWORD*)(state+0x1c)=0;*(DWORD*)(state+0x1cd)=0;
    *(DWORD*)(state+0x1e1)=tick();*(DWORD*)(state+0x1e5)=tick();*(DWORD*)(state+0x1c5)=tick();
    ResetData();
}
// samp.dll RVA: 0x000162c0
CRemotePlayer::~CRemotePlayer() {
    BYTE *state=(BYTE*)this;DWORD marker=*(DWORD*)(state+0x1f9);
    if(marker){g_pGame->DeleteMarker(marker);*(DWORD*)(state+0x1f9)=0;}
    CPed *ped=*(CPed**)(state+4);*(DWORD*)(state+0x1e9)=0;
    if(ped){g_pGame->RemovePed(ped);*(DWORD*)(state+4)=0;}
}
// samp.dll RVA: 0x00015c60
void CRemotePlayer::ResetData() {
    BYTE *state=(BYTE*)this;
    *(WORD*)(state+2)=0;
    *(DWORD*)(state+0x1b0)=0;*(DWORD*)(state+0x1ac)=0;*(DWORD*)(state+8)=0;
    state[0x1b]=0;*(DWORD*)(state+0x20)=0;
    *(DWORD*)(state+0x1ed)=0;*(DWORD*)(state+0x1f1)=0;*(DWORD*)(state+0x1f5)=0;
    state[0x18]=0;*(DWORD*)(state+0x1c0)=0;*(DWORD*)(state+0x1d1)=2;
    memset(state+0x3c,0,68);memset(state+0x80,0,63);
    memset(state+0x24,0,24);memset(state+0xf5,0,31);memset(state+0x1d5,0,12);
    DWORD marker=*(DWORD*)(state+0x1f9);
    if(marker){g_pGame->DeleteMarker(marker);*(DWORD*)(state+0x1f9)=0;}
    *(DWORD*)(state+0x1e9)=0;
    if(g_pNetGame)*(DWORD*)(state+0x1c9)=(DWORD)((CNetGame*)g_pNetGame)->GetCounter();
}
// samp.dll RVA: 0x00017040
void CRemotePlayer::Remove() {
    BYTE *state=(BYTE*)this;
    if(*(CPed**)(state+4)) {
        ResetData();g_pGame->RemovePed(*(CPed**)(state+4));*(DWORD*)(state+4)=0;
    }
    state[0x1a]=0;*(DWORD*)(state+0x10)=1;
}
// samp.dll RVA: 0x00017080
void CRemotePlayer::Kill() {
    BYTE *state=(BYTE*)this;
    if(*(CPed**)(state+4)) {
        (*(CPed**)(state+4))->SetKeys(0,0,0);
        (*(CPed**)(state+4))->Kill();
    }
    if(state[0x1a]!=32)state[0x1a]=32;
    ResetData();
}
// samp.dll RVA: 0x00015e00
void CRemotePlayer::SetColor(DWORD color) {PlayerColors::Set(*(WORD*)this,color);}
// samp.dll RVA: 0x00015e20
DWORD CRemotePlayer::GetColorAsRGBA() {return PlayerColors::Get(*(WORD*)this);}
// samp.dll RVA: 0x00015d50
__declspec(naked) float CRemotePlayer::GetDistanceToPlayer(CRemotePlayer *other) {
    __asm {
        mov ecx,dword ptr [ecx+4]
        sub esp,80h
        test ecx,ecx
        jnz have_ped
        fld g_remoteAbsentDistance
        add esp,80h
        ret 4
    have_ped:
        lea eax,[esp]
        push eax
        call CEntity::GetMatrix
        mov edx,dword ptr [esp+84h]
        lea ecx,[esp+40h]
        push ecx
        mov ecx,dword ptr [edx+4]
        call CEntity::GetMatrix
        fld dword ptr [esp+38h]
        fsub dword ptr [esp+78h]
        fld dword ptr [esp+34h]
        fsub dword ptr [esp+74h]
        fld dword ptr [esp+30h]
        fsub dword ptr [esp+70h]
        fld st(0)
        fmul st,st(1)
        fld st(2)
        fmul st,st(3)
        faddp st(1),st
        fld st(3)
        fmul st,st(4)
        faddp st(1),st
        fsqrt
        fstp st(3)
        fstp st(0)
        fstp st(0)
        add esp,80h
        ret 4
    }
}
// samp.dll RVA: 0x00015dd0
__declspec(naked) float CRemotePlayer::GetDistanceToLocalPlayer() {
    __asm {
        mov eax,ecx
        mov ecx,dword ptr [eax+4]
        test ecx,ecx
        jnz select_entity
        fld g_remoteAbsentDistance
        ret
    select_entity:
        cmp byte ptr [eax+1ah],13h
        jne selected
        mov eax,dword ptr [eax+8]
        test eax,eax
        jz selected
        mov ecx,eax
    selected:
        jmp CEntity::GetDistanceToLocalPlayer
    }
}
// samp.dll RVA: 0x00015e50
void CRemotePlayer::EnterVehicle(WORD id,BOOL passenger) {
    BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);
    CVehiclePool *vehicles=*(CVehiclePool**)(pools+0xc);
    CVehicle *vehicle=vehicles->Get(id);BYTE *state=(BYTE*)this;
    CPed *ped=*(CPed**)(state+4);
    if(!ped||!vehicle||ped->IsInVehicle())return;
    DWORD ref=vehicles->GetRef((int)id);
    if(!ref||ref==0xffff)return;
    (*(CPed**)(state+4))->SetKeys(0,0,0);
    (*(CPed**)(state+4))->EnterVehicle(ref,passenger);
}
// samp.dll RVA: 0x00015ee0
void CRemotePlayer::ExitVehicle() {
    BYTE *state=(BYTE*)this;CPed *ped=*(CPed**)(state+4);
    if(!ped||!ped->IsInVehicle())return;
    (*(CPed**)(state+4))->SetKeys(0,0,0);
    (*(CPed**)(state+4))->ExitVehicle();
}
// samp.dll RVA: 0x00015f10
void CRemotePlayer::ChangeState(BYTE newState,BYTE oldState) {
    if(newState!=19||oldState!=17)return;
    CPed *ped=g_pGame->GetPlayerPed();
    if(!ped||!ped->IsInVehicle()||ped->IsPassenger())return;
    CVehiclePool *vehicles=*(CVehiclePool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+0xc);
    WORD id=vehicles->Find(ped->GetVehicle());
    if(id!=*(WORD*)((BYTE*)this+2))return;
    CMatrix matrix;ped->GetMatrix(&matrix);
    float z=matrix.position.z+g_quaternionMatrixOne;
    ped->WarpFromVehicle(matrix.position.x,matrix.position.y,z);
    g_pGame->DisplayGameText(g_remoteCarJackedText,1000,5);
}
// samp.dll RVA: 0x000144d0
void CRemotePlayer::ApplySurfingMatrix() {
    BYTE *state=(BYTE*)this;
    if(state[0x1a]!=17)return;
    WORD id=*(WORD*)(state+0x7a);
    if(!id||id==0xffff||id>=4100)return;
    BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);
    CVehiclePool *vehicles=*(CVehiclePool**)(pools+0xc);
    CObjectPool *objects=*(CObjectPool**)(pools+0x14);
    CEntity *entity;
    if(id<2000)entity=vehicles->Get(id);
    else entity=objects->Get((WORD)(id-2000));
    if(!entity)return;
    for(int axis=0;axis<3;axis++) {
        float *component=(float*)(state+0x6e)+axis;BYTE accepted;
        // Preserve FCOMP unordered acceptance and native +/-100 bounds.
        __asm {
            mov edx,component
            fld dword ptr [edx]
            fcomp g_vehicleTrainSpeedMaximum
            fnstsw ax
            test ah,41h
            jz invalid_offset
            fld dword ptr [edx]
            fcomp g_vehicleTrainSpeedMinimum
            fnstsw ax
            test ah,5
            jnp invalid_offset
            mov accepted,1
            jmp checked_offset
        invalid_offset:
            mov accepted,0
        checked_offset:
        }
        if(!accepted)return;
    }
    CMatrix entityMatrix,pedMatrix;CVector speed,turnSpeed,position;
    entity->GetMatrix(&entityMatrix);
    entity->GetSpeed(&speed);entity->GetTurnSpeed(&turnSpeed);
    (*(CPed**)(state+4))->GetMatrix(&pedMatrix);
    GameTransformPoint(&position,&entityMatrix,(CVector*)(state+0x6e));
    memcpy(&pedMatrix.position,&position,12);
    (*(CPed**)(state+4))->ApplyMatrix(pedMatrix);
}

// samp.dll RVA: 0x00001080
BOOL CRemotePlayer::IsActive() {
    BYTE *state=(BYTE*)this;
    return *(CPed**)(state+4)&&state[0x1a]!=0;
}

#include "game/player_colors.h"
// samp.dll RVA: 0x00015e30
DWORD CRemotePlayer::GetColorAsARGB() {
    return (PlayerColors::Get(*(WORD*)this)>>8)|0xff000000;
}
// samp.dll RVA: 0x00015fd0
int CRemotePlayer::GetStatus() {
    BYTE *state=(BYTE*)this;
    if(!*(CPed**)(state+4))return 0;
    if(!(*(CPed**)(state+4))->DoesExist())return 0;
    if((*(CPed**)(state+4))->GetVehicleTransitionTaskState())return 0;
    if((*(CPed**)(state+4))->HasTaskType704())return 0;
    return *(int*)(state+0x1d1);
}
