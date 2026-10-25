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
#include "net/localplayer.h"
#include "net/pool_frame.h"
// samp.dll RVA: 0x001181fc
const float g_remoteBulletOffsetMinimum=-300.0f;
// samp.dll RVA: 0x00118200
const float g_remoteBulletOffsetMaximum=300.0f;
// samp.dll RVA: 0x00016010
void CRemotePlayer::UpdateBullet(const void *data) {
    CPed *ped=*(CPed**)(nativeState+4);if(!ped||!ped->DoesExist())return;
    const BYTE *input=(const BYTE*)data;PedBulletData bullet={0};
    memcpy(&bullet.origin,input+3,12);memcpy(&bullet.target,input+0xf,12);memcpy(&bullet.offset,input+0x1b,12);
    BYTE type=input[0];
    if(type) {
        for(int axis=0;axis<3;++axis) {
            float *component=&bullet.offset.x+axis;BYTE accepted=0;
            // Native accepts unordered offsets and includes both +/-300 endpoints.
            __asm {
                mov eax,component
                fld dword ptr [eax]
                fcomp dword ptr [g_remoteBulletOffsetMaximum]
                fnstsw ax
                test ah,41h
                jz bullet_offset_done
                mov eax,component
                fld dword ptr [eax]
                fcomp dword ptr [g_remoteBulletOffsetMinimum]
                fnstsw ax
                test ah,5
                jnp bullet_offset_done
                mov accepted,1
            bullet_offset_done:
            }
            if(!accepted)return;
        }
        if(type==1) {
            CPlayerPool *players=((CNetGame*)g_pNetGame)->pools->players;
            if(players) {
                WORD player=*(WORD*)(input+1);
                if(player==players->localId)bullet.entity=g_pGame->GetPlayerPed()->m_pGamePed;
                else {
                    if(player==*(WORD*)nativeState)return;
                    if(players->IsConnected(player)) {
                        CPed *target=*(CPed**)(players->GetPlayer(player)->nativeState+4);
                        if(target)bullet.entity=target->m_pGamePed;
                    }
                }
            }
        }else if(type==2) {
            CVehiclePool *vehicles=((CNetGame*)g_pNetGame)->pools->vehicles;
            if(vehicles) {
                WORD vehicle=*(WORD*)(input+1);
                if(vehicles->DoesExist(vehicle))bullet.entity=vehicles->Get(vehicle)->m_pGameVehicle;
            }
        }
    }
    if((*(CPed**)(nativeState+4))->DoesExist()) {
        BYTE weapon=input[0x27];
        if((BYTE)(*(CPed**)(nativeState+4))->GetCurrentWeapon()!=weapon) {
            (*(CPed**)(nativeState+4))->SetArmedWeapon(weapon,1);weapon=input[0x27];
            if((BYTE)(*(CPed**)(nativeState+4))->GetCurrentWeapon()!=weapon) {
                (*(CPed**)(nativeState+4))->GiveWeapon(weapon,9999);
                (*(CPed**)(nativeState+4))->SetArmedWeapon(input[0x27],1);
            }
        }
    }
    (*(CPed**)(nativeState+4))->SetBulletData(&bullet);(*(CPed**)(nativeState+4))->FireWeapon();
}
// samp.dll RVA: 0x00016230
void CRemotePlayer::SetState(BYTE state) {
    BYTE previous=nativeState[0x1a];if(state!=previous){ChangeState(state,previous);nativeState[0x1a]=state;}
}
// samp.dll RVA: 0x00118218
const char g_remoteVendingAnimation[]="VEND_EAT_P";
// samp.dll RVA: 0x00118224
const char g_remoteVendingLibrary[]="VENDING";
// samp.dll RVA: 0x00118204
const char g_remoteCarryAnimation[]="crry_prtial";
// samp.dll RVA: 0x00118210
const char g_remoteCarryLibrary[]="CARRY";
// samp.dll RVA: 0x00016310
void CRemotePlayer::Process() {
    if(!*(CPed**)(nativeState+4))return;
    if(!nativeState[0x1a]) {
        ResetData();g_pGame->RemovePed(*(CPed**)(nativeState+4));*(DWORD*)(nativeState+4)=0;return;
    }
    DWORD elapsed=GetTickCount()-*(DWORD*)(nativeState+0x1c5);
    if(nativeState[0x1a]==17&&nativeState[0x1c4]==16&&!(*(CPed**)(nativeState+4))->IsInVehicle()) {
        if(!HasSurfingTarget()) {
            SetOnfootTarget((CVector*)(nativeState+0x42),(CVector*)(nativeState+0x62));UpdateOnfootSpeedAndPosition();
        }else SetOnfootTarget((CVector*)(nativeState+0x42),(CVector*)(nativeState+0x62));
        if((*(CPed**)(nativeState+4))->DoesExist()) {
            BYTE weapon=nativeState[0x60]&0x3f;
            if((BYTE)(*(CPed**)(nativeState+4))->GetCurrentWeapon()!=weapon) {
                (*(CPed**)(nativeState+4))->SetArmedWeapon(weapon,0);weapon=nativeState[0x60]&0x3f;
                if((BYTE)(*(CPed**)(nativeState+4))->GetCurrentWeapon()!=weapon) {
                    (*(CPed**)(nativeState+4))->GiveWeapon(weapon,9999);
                    (*(CPed**)(nativeState+4))->SetArmedWeapon(nativeState[0x60]&0x3f,0);
                }
            }
        }
        if(nativeState[0x40]&0x80) {
            if((BYTE)(*(CPed**)(nativeState+4))->GetCurrentWeapon()==34||(BYTE)(*(CPed**)(nativeState+4))->GetCurrentWeapon()==35||(BYTE)(*(CPed**)(nativeState+4))->GetCurrentWeapon()==36)
                (*(CPed**)(nativeState+4))->SetState(12);
        }else if((*(CPed**)(nativeState+4))->GetState()==12)(*(CPed**)(nativeState+4))->SetState(1);
        DWORD animation=*(DWORD*)(nativeState+0x7c);
        if(*(DWORD*)(nativeState+0x1c0)!=animation) {
            if(animation)(*(CPed**)(nativeState+4))->ApplyPackedAnimation(animation);
            if(!*(DWORD*)(nativeState+0x7c)&&*(DWORD*)(nativeState+0x1c0)==0x1804067c) {
                (*(CPed**)(nativeState+4))->ApplyAnimation(g_remoteVendingAnimation,g_remoteVendingLibrary,4.0f,0,0,0,0,-1);
                *(DWORD*)(nativeState+0x7c)=0xffffffff;
            }
            animation=*(DWORD*)(nativeState+0x7c);*(DWORD*)(nativeState+0x1c0)=animation;*(DWORD*)(nativeState+0x1cd)=0;
        }
        if(!*(DWORD*)(nativeState+0x1cd)&&!animation&&(*(CPed**)(nativeState+4))->HasTaskType401())
            (*(CPed**)(nativeState+4))->ApplyAnimation(g_remoteCarryAnimation,g_remoteCarryLibrary,4.0f,0,0,0,0,0);
        *(DWORD*)(nativeState+8)=0;nativeState[0x1c4]=0;
    }else if(nativeState[0x1a]==19&&nativeState[0x1c4]==17&&(*(CPed**)(nativeState+4))->IsInVehicle()) {
        CVehicle *vehicle=*(CVehicle**)(nativeState+8);if(!vehicle||!vehicle->DoesExist())return;
        GameQuaternion *rotation=(GameQuaternion*)(nativeState+0x88);GameQuaternionNormalize(rotation);
        CMatrix matrix;GameQuaternionToMatrixComponents(rotation,&matrix);matrix.position=*(CVector*)(nativeState+0x98);
        if((*(CVehicle**)(nativeState+8))->GetModelIndex()==538||(*(CVehicle**)(nativeState+8))->GetModelIndex()==537||(*(CVehicle**)(nativeState+8))->GetModelIndex()==449)
            UpdateTrain(&matrix,(CVector*)(nativeState+0xa4),*(float*)(nativeState+0xbb));
        else {SetIncarTarget(&matrix,(CVector*)(nativeState+0x98),(CVector*)(nativeState+0xa4));UpdateIncarSpeedAndPosition();}
        if((*(CVehicle**)(nativeState+8))->GetModelIndex()==520)(*(CVehicle**)(nativeState+8))->SetHydraThrusters(*(DWORD*)(nativeState+0xbb));
        if((*(CVehicle**)(nativeState+8))->GetSubtype()==2||(*(CVehicle**)(nativeState+8))->GetSubtype()==6)
            (*(CVehicle**)(nativeState+8))->SetBackendFloat64c(*(float*)(nativeState+0xbb));
        (*(CVehicle**)(nativeState+8))->EnableSiren(nativeState[0xb7]);
        (*(CVehicle**)(nativeState+8))->SetLandingGearState(nativeState[0xb8]);
        (*(CVehicle**)(nativeState+8))->SetHealth(*(float*)(nativeState+0xb0));
        WORD trailer=*(WORD*)(nativeState+0xb9);
        if(!trailer||trailer>=2000) {
            if((*(CVehicle**)(nativeState+8))->GetTrailer()) {
                (*(CVehicle**)(nativeState+8))->DetachTrailer();(*(CVehicle**)(nativeState+8))->SetTrailer(0);
            }
        }
        BYTE weapon=nativeState[0xb6]&0x3f;
        if((BYTE)(*(CPed**)(nativeState+4))->GetCurrentWeapon()!=weapon) {
            (*(CPed**)(nativeState+4))->SetArmedWeapon(weapon,0);weapon=nativeState[0xb6]&0x3f;
            if((BYTE)(*(CPed**)(nativeState+4))->GetCurrentWeapon()!=weapon) {
                (*(CPed**)(nativeState+4))->GiveWeapon(weapon,9999);
                (*(CPed**)(nativeState+4))->SetArmedWeapon(nativeState[0xb6]&0x3f,0);
            }
        }
        nativeState[0x1c4]=0;
    }else if(nativeState[0x1a]==18&&nativeState[0x1c4]==18&&(*(CPed**)(nativeState+4))->IsInVehicle()) {
        CVehicle *vehicle=*(CVehicle**)(nativeState+8);if(!vehicle||!vehicle->DoesExist())return;
        if((*(CPed**)(nativeState+4))->DoesExist()) {
            BYTE weapon=nativeState[0x27]&0x3f;
            if((BYTE)(*(CPed**)(nativeState+4))->GetCurrentWeapon()!=weapon) {
                (*(CPed**)(nativeState+4))->SetArmedWeapon(weapon,0);weapon=nativeState[0x27]&0x3f;
                if((BYTE)(*(CPed**)(nativeState+4))->GetCurrentWeapon()!=weapon) {
                    (*(CPed**)(nativeState+4))->GiveWeapon(weapon,9999);
                    (*(CPed**)(nativeState+4))->SetArmedWeapon(nativeState[0x27]&0x3f,0);
                }
            }
        }
        if((*(CPed**)(nativeState+4))->GetState()==50) {
            if((nativeState[0x26]&0x40)&&!(*(CPed**)(nativeState+4))->HasTaskType1022()&&(*(CPed**)(nativeState+4))->DoesExist()&&(*(CPed**)(nativeState+4))->EnablePassengerDrivebyMode())
                *(DWORD*)(nativeState+0x20)=1;
            if(!(nativeState[0x26]&0x40)&&(*(CPed**)(nativeState+4))->HasTaskType1022()&&(*(CPed**)(nativeState+4))->DoesExist()) {
                (*(CPed**)(nativeState+4))->StopTaskType1022();*(DWORD*)(nativeState+0x20)=0;
            }
        }
        if((nativeState[0x26]&0x80)&&!(*(CPed**)(nativeState+4))->HasDanceState())(*(CPed**)(nativeState+4))->SetDancePoseState(1);
        else if(!(nativeState[0x26]&0x80)&&(*(CPed**)(nativeState+4))->HasDanceState())(*(CPed**)(nativeState+4))->SetDancePoseState(0);
        nativeState[0x1c4]=0;
    }
    if(nativeState[0x1a]==17&&!(*(CPed**)(nativeState+4))->IsInVehicle()) {
        UpdateOnfootRotation();ProcessHead();ProcessSpecialAction();*(DWORD*)(nativeState+0x20)=0;
        (*(CPed**)(nativeState+4))->SetKeys(*(WORD*)(nativeState+0x40),*(WORD*)(nativeState+0x3c),*(WORD*)(nativeState+0x3e));
        if(HasSurfingTarget()) {
            if(*(WORD*)(nativeState+0x7a)<2000){(*(CPed**)(nativeState+4))->SetCollisionProcessed(FALSE);(*(CPed**)(nativeState+4))->SetCollisionFlag(FALSE);}
        }else {
            BOOL collision=nativeState[0x18]!=3;(*(CPed**)(nativeState+4))->SetCollisionProcessed(collision);(*(CPed**)(nativeState+4))->SetCollisionFlag(collision);
        }
        CVector *speed=(CVector*)(nativeState+0x62);
        if(speed->x==0&&speed->y==0&&speed->z==0) {
            (*(CPed**)(nativeState+4))->SetSpeed(*speed);
            if(*(DWORD*)(nativeState+0x1d1)!=2&&(int)elapsed>=3000)*(DWORD*)(nativeState+0x1d1)=2;
        }else if(*(DWORD*)(nativeState+0x1d1)!=2&&(int)elapsed>=1500)*(DWORD*)(nativeState+0x1d1)=2;
        if(GetTickCount()-*(DWORD*)(nativeState+0x1c5)>3000) {
            *(WORD*)(nativeState+0x3c)=0;*(WORD*)(nativeState+0x3e)=0;CVector zero={0,0,0};(*(CPed**)(nativeState+4))->SetSpeed(zero);
            CMatrix matrix;(*(CPed**)(nativeState+4))->GetMatrix(&matrix);matrix.position=*(CVector*)(nativeState+0x42);
            (*(CPed**)(nativeState+4))->SetMatrix(matrix);
        }
    }else if(nativeState[0x1a]==19&&(*(CPed**)(nativeState+4))->IsInVehicle()) {
        if(*(CVehicle**)(nativeState+8)&&(*(CVehicle**)(nativeState+8))->GetModelIndex()!=538&&(*(CVehicle**)(nativeState+8))->GetModelIndex()!=537&&(*(CVehicle**)(nativeState+8))->GetModelIndex()!=449)UpdateIncarRotation();
        *(DWORD*)(nativeState+0x20)=0;(*(CPed**)(nativeState+4))->DestroyParachute();
        if(*(CVehicle**)(nativeState+8)&&(*(CVehicle**)(nativeState+8))->IsRC())(*(CPed**)(nativeState+4))->Remove();
        CVector *speed=(CVector*)(nativeState+0xa4);
        if(*(DWORD*)(nativeState+0x1c)&&speed->x==0&&speed->y==0&&speed->z==0&&*(CVehicle**)(nativeState+8))
            (*(CVehicle**)(nativeState+8))->SetSpeed(*speed);
        (*(CPed**)(nativeState+4))->SetKeys(*(WORD*)(nativeState+0x86),*(WORD*)(nativeState+0x82),*(WORD*)(nativeState+0x84));
        (*(CPed**)(nativeState+4))->ProcessVehicleHorn();
        if(speed->x==0&&speed->y==0&&speed->z==0) {
            if(*(DWORD*)(nativeState+0x1d1)!=2&&(int)elapsed>3000)*(DWORD*)(nativeState+0x1d1)=2;
        }else if(*(DWORD*)(nativeState+0x1d1)!=2&&(int)elapsed>1500)*(DWORD*)(nativeState+0x1d1)=2;
        ProcessHead();
    }else if(nativeState[0x1a]==18&&(*(CPed**)(nativeState+4))->IsInVehicle()) {
        (*(CPed**)(nativeState+4))->DestroyParachute();(*(CPed**)(nativeState+4))->SetKeys(*(WORD*)(nativeState+0x2e),0,0);
        if(*(DWORD*)(nativeState+0x1d1)!=2&&(int)elapsed>=3000)*(DWORD*)(nativeState+0x1d1)=2;
        ProcessHead();
    }else {
        CVector zero={0,0,0};(*(CPed**)(nativeState+4))->SetSpeed(zero);(*(CPed**)(nativeState+4))->SetKeys(0,0,0);*(DWORD*)(nativeState+0x20)=0;
    }
    if(nativeState[0x1a]!=32)(*(CPed**)(nativeState+4))->SetHealth(1000.0f);
    if(*(DWORD*)(nativeState+0x1d1)==2&&(int)elapsed<1500)*(DWORD*)(nativeState+0x1d1)=0;
}
// samp.dll RVA: 0x00117874
const float g_remoteHeadDistance=20.0f;
// samp.dll RVA: 0x00117868
const char g_remoteHeadTask[]="FollowPedSA";
// samp.dll RVA: 0x00014240
void CRemotePlayer::ProcessHead() {
    CPed *ped=*(CPed**)(nativeState+4);if(!ped||!ped->DoesExist()||*(DWORD*)(nativeState+0x1c))return;
    if(g_pGame&&!*(DWORD*)((BYTE*)g_pGame+0x59))return;
    if((*(CPed**)(nativeState+4))->GetStuff())return;
    CMatrix matrix;(*(CPed**)(nativeState+4))->GetMatrix(&matrix);DWORD now=GetTickCount();CRemotePlayer *self=this;
    if(now-*(DWORD*)(nativeState+0x1e1)>1000) {
        CameraAim *aim=AimStuff::GetPlayerAim(*((BYTE*)*(CPed**)(nativeState+4)+0x2b0));float z;
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
            mov [ecx+1e1h],edx
            fmul dword ptr [g_remoteHeadDistance]
            fadd dword ptr [matrix+56]
            fstp dword ptr [z]
            fxch st(1)
            fsub dword ptr [matrix+48]
            fstp dword ptr [ecx+1d5h]
            fsub dword ptr [matrix+52]
            fstp dword ptr [ecx+1d9h]
            fld dword ptr [z]
            fsub dword ptr [matrix+56]
            fstp dword ptr [ecx+1ddh]
        }
    }
    if(now-*(DWORD*)(nativeState+0x1e5)>200) {
        CVector target;ped=*(CPed**)(nativeState+4);
        __asm {
            mov ecx,self
            fld dword ptr [matrix+48]
            fadd dword ptr [ecx+1d5h]
            fstp dword ptr [target]
            fld dword ptr [matrix+52]
            fadd dword ptr [ecx+1d9h]
            fstp dword ptr [target+4]
            fld dword ptr [matrix+56]
            fadd dword ptr [ecx+1ddh]
            fstp dword ptr [target+8]
        }
        ped->ApplyCommandTask(g_remoteHeadTask,0,1000,-1,&target,0,0.1f,500,3,0);
        *(DWORD*)(nativeState+0x1e5)=now;
    }
}
// samp.dll RVA: 0x00014390
void CRemotePlayer::SetMarkerState(BOOL enabled) {
    if(!enabled) {
        DWORD marker=*(DWORD*)(nativeState+0x1f9);
        if(marker){g_pGame->DeleteMarker(marker);*(DWORD*)(nativeState+0x1f9)=0;}
    }
    *(BOOL*)(nativeState+0x1e9)=enabled;
}
// samp.dll RVA: 0x000143d0
void CRemotePlayer::SetMarkerPosition(short x,short y,short z) {
    *(DWORD*)(nativeState+0x1e9)=1;DWORD marker=*(DWORD*)(nativeState+0x1f9);
    if(marker){g_pGame->DeleteMarker(marker);*(DWORD*)(nativeState+0x1f9)=0;}
    if(*(CPed**)(nativeState+4))return;
    marker=g_pGame->CreateMarker(0,(float)x,(float)y,(float)z,*(WORD*)nativeState,0);
    *(int*)(nativeState+0x1f5)=z;*(int*)(nativeState+0x1ed)=x;*(int*)(nativeState+0x1f1)=y;*(DWORD*)(nativeState+0x1f9)=marker;
}
// samp.dll RVA: 0x00014470
BOOL CRemotePlayer::HasSurfingTarget() {
    WORD id=*(WORD*)(nativeState+0x7a);return nativeState[0x1a]==17&&id&&id!=0xffff&&id<4100?TRUE:FALSE;
}
// samp.dll RVA: 0x000144a0
BOOL CRemotePlayer::SurfingOnObject() {
    WORD id=*(WORD*)(nativeState+0x7a);return nativeState[0x1a]==17&&id&&id!=0xffff&&id>=2000&&id<4100?TRUE:FALSE;
}
// samp.dll RVA: 0x00014780
void CRemotePlayer::ProcessSpecialAction() {
    CPed *ped=*(CPed**)(nativeState+4);if(!ped||!ped->DoesExist())return;
    if(nativeState[0x18]==1&&!(*(CPed**)(nativeState+4))->IsDucking()){(*(CPed**)(nativeState+4))->ClearTasksIfOnFoot();return;}
    if(nativeState[0x18]!=1&&(*(CPed**)(nativeState+4))->IsDucking())(*(CPed**)(nativeState+4))->ClearDuckingFlag();
    if(nativeState[0x18]==11&&!(*(CPed**)(nativeState+4))->UsingCellphone()){(*(CPed**)(nativeState+4))->EnableCellphone(TRUE);return;}
    if(nativeState[0x18]!=11&&(*(CPed**)(nativeState+4))->UsingCellphone()){(*(CPed**)(nativeState+4))->EnableCellphone(FALSE);return;}
    if(nativeState[0x18]==2&&!(*(CPed**)(nativeState+4))->HasJetpack()) {
        *(DWORD*)(nativeState+0x14)=1;(*(CPed**)(nativeState+4))->EnableJetpack();return;
    }
    if(nativeState[0x18]!=2&&(*(CPed**)(nativeState+4))->HasJetpack()) {
        *(DWORD*)(nativeState+0x14)=0;(*(CPed**)(nativeState+4))->DisableJetpack();return;
    }
    if(nativeState[0x18]==10&&!(*(CPed**)(nativeState+4))->DoesHandsUp())(*(CPed**)(nativeState+4))->HandsUp();
    if(nativeState[0x18]!=10&&(*(CPed**)(nativeState+4))->DoesHandsUp())(*(CPed**)(nativeState+4))->StopDancing();
    if(!(*(CPed**)(nativeState+4))->DoesUrinating()&&nativeState[0x18]==68)(*(CPed**)(nativeState+4))->StartUrinating();
    if((*(CPed**)(nativeState+4))->DoesUrinating()&&nativeState[0x18]!=68)(*(CPed**)(nativeState+4))->StopUrinating();
    if(nativeState[0x18]==20&&(*(CPed**)(nativeState+4))->GetStuff()!=1)(*(CPed**)(nativeState+4))->GiveStuff(1);
    if(nativeState[0x18]==22&&(*(CPed**)(nativeState+4))->GetStuff()!=2)(*(CPed**)(nativeState+4))->GiveStuff(2);
    if(nativeState[0x18]==23&&(*(CPed**)(nativeState+4))->GetStuff()!=3)(*(CPed**)(nativeState+4))->GiveStuff(3);
    if(nativeState[0x18]==21&&(*(CPed**)(nativeState+4))->GetStuff()!=4)(*(CPed**)(nativeState+4))->GiveStuff(4);
    if((*(CPed**)(nativeState+4))->GetStuff()) {
        BYTE action=nativeState[0x18];
        if(action==20||action==22||action==23||action==21)(*(CPed**)(nativeState+4))->ApplyStuff();
        else (*(CPed**)(nativeState+4))->DropStuff();
    }
    if(!(*(CPed**)(nativeState+4))->HasDanceState()&&nativeState[0x18]==24)(*(CPed**)(nativeState+4))->SetDancePoseState(1);
    if((*(CPed**)(nativeState+4))->HasDanceState()&&nativeState[0x18]!=24&&nativeState[0x18]!=3&&nativeState[0x18]!=4)(*(CPed**)(nativeState+4))->SetDancePoseState(0);
    if(!(*(CPed**)(nativeState+4))->HasDanceStyle()&&nativeState[0x18]==25)(*(CPed**)(nativeState+4))->SetSecondaryPoseState(1);
    if((*(CPed**)(nativeState+4))->HasDanceStyle()&&nativeState[0x18]!=25&&nativeState[0x18]!=3&&nativeState[0x18]!=4)(*(CPed**)(nativeState+4))->SetSecondaryPoseState(0);
    (*(CPed**)(nativeState+4))->CheckParachuteWeapon();
}
// samp.dll RVA: 0x0013c498
float g_remoteOnfootCorrection=0.1f;
// samp.dll RVA: 0x0013c49c
float g_remoteIncarCorrection=0.06f;
// samp.dll RVA: 0x0011797c
const float g_remoteIncarTeleportDistance=8.0f;
// samp.dll RVA: 0x000149f0
void CRemotePlayer::UpdateOnfootSpeedAndPosition() {
    CPed *ped=*(CPed**)(nativeState+4);if(!ped)return;
    CMatrix matrix;ped->GetMatrix(&matrix);
    CVector *target=(CVector*)(nativeState+0x120);
    if(!(*(CPed**)(nativeState+4))->DoesExist()){matrix.position=*target;(*(CPed**)(nativeState+4))->SetMatrix(matrix);return;}
    volatile float *delta=(volatile float*)(nativeState+0x190);
    delta[0]=FloatOffset(target->x,matrix.position.x);delta[1]=FloatOffset(target->y,matrix.position.y);delta[2]=FloatOffset(target->z,matrix.position.z);
    if(delta[0]<=g_localQuaternionSyncThreshold&&delta[1]<=g_localQuaternionSyncThreshold&&delta[2]<=g_localQuaternionSyncThreshold)return;
    if(delta[0]>g_quaternionMatrixTwo||delta[1]>g_quaternionMatrixTwo||delta[2]>g_quaternionMatrixOne) {
        matrix.position=*target;(*(CPed**)(nativeState+4))->SetMatrix(matrix);return;
    }
    CVector speed;(*(CPed**)(nativeState+4))->GetSpeed(&speed);CRemotePlayer *self=this;
    __asm {
        mov ecx,self
        fld dword ptr [g_remoteOnfootCorrection]
        fld dword ptr [ecx+190h]
        fcomp dword ptr [g_localQuaternionSyncThreshold]
        fnstsw ax
        test ah,41h
        jne onfoot_y_correction
        fld dword ptr [ecx+120h]
        fsub dword ptr [matrix+48]
        fmul st(0),st(1)
        fadd dword ptr [speed]
        fstp dword ptr [speed]
    onfoot_y_correction:
        fld dword ptr [ecx+194h]
        fcomp dword ptr [g_localQuaternionSyncThreshold]
        fnstsw ax
        test ah,41h
        jne onfoot_z_correction
        fld dword ptr [ecx+124h]
        fsub dword ptr [matrix+52]
        fmul st(0),st(1)
        fadd dword ptr [speed+4]
        fstp dword ptr [speed+4]
    onfoot_z_correction:
        fld dword ptr [ecx+198h]
        fcomp dword ptr [g_localQuaternionSyncThreshold]
        fnstsw ax
        test ah,41h
        jne onfoot_correction_done
        fld dword ptr [ecx+128h]
        fsub dword ptr [matrix+56]
        fmul st(0),st(1)
        fadd dword ptr [speed+8]
        fstp dword ptr [speed+8]
    onfoot_correction_done:
        fstp st(0)
    }
    (*(CPed**)(nativeState+4))->SetSpeed(speed);
}
// samp.dll RVA: 0x00014c00
void CRemotePlayer::UpdateOnfootXYSpeedAndPosition() {
    CPed *ped=*(CPed**)(nativeState+4);if(!ped)return;
    CMatrix matrix;ped->GetMatrix(&matrix);CVector *target=(CVector*)(nativeState+0x120);
    if(!(*(CPed**)(nativeState+4))->DoesExist()) {
        matrix.position.x=target->x;matrix.position.y=target->y;(*(CPed**)(nativeState+4))->SetMatrix(matrix);return;
    }
    volatile float *delta=(volatile float*)(nativeState+0x190);delta[0]=FloatOffset(target->x,matrix.position.x);
    int action=0;CRemotePlayer *self=this;
    __asm {
        mov ecx,self
        push dword ptr [matrix+52]
        push dword ptr [ecx+124h]
        call FloatOffset
        add esp,8
        mov ecx,self
        fst dword ptr [ecx+194h]
        fld dword ptr [ecx+190h]
        fcomp dword ptr [g_objectTurnVelocityScale]
        fnstsw ax
        test ah,41h
        jp xy_significant
        fcom dword ptr [g_objectTurnVelocityScale]
        fnstsw ax
        test ah,41h
        jnp xy_discard_y
    xy_significant:
        fld dword ptr [ecx+190h]
        fcomp dword ptr [g_quaternionMatrixTwo]
        fnstsw ax
        test ah,41h
        je xy_teleport_pop
        fcomp dword ptr [g_quaternionMatrixTwo]
        fnstsw ax
        test ah,41h
        je xy_teleport
        mov action,1
        jmp xy_decided
    xy_teleport_pop:
        fstp st(0)
    xy_teleport:
        mov action,2
        jmp xy_decided
    xy_discard_y:
        fstp st(0)
    xy_decided:
    }
    if(!action)return;
    if(action==2){matrix.position.x=target->x;matrix.position.y=target->y;(*(CPed**)(nativeState+4))->SetMatrix(matrix);return;}
    CVector speed;(*(CPed**)(nativeState+4))->GetSpeed(&speed);
    for(int i=0;i<2;++i)if(delta[i]>g_objectPoolSecondsPerTick) {
        float *wanted=(float*)target+i,*current=(float*)&matrix.position+i,*value=(float*)&speed+i;
        __asm {
            mov ecx,wanted
            mov edx,current
            mov eax,value
            fld dword ptr [ecx]
            fsub dword ptr [edx]
            fmul dword ptr [g_objectMutationStep]
            fadd dword ptr [eax]
            fstp dword ptr [eax]
        }
    }
    (*(CPed**)(nativeState+4))->SetSpeed(speed);
}
// samp.dll RVA: 0x00014ec0
void CRemotePlayer::UpdateIncarSpeedAndPosition() {
    CVector speed={0,0,0};CVehicle *vehicle=*(CVehicle**)(nativeState+8);if(!vehicle)return;
    CMatrix matrix;vehicle->GetMatrix(&matrix);CVector *target=(CVector*)(nativeState+0x138);
    if(!(*(CVehicle**)(nativeState+8))->DoesExist()){matrix.position=*target;(*(CVehicle**)(nativeState+8))->SetMatrix(matrix);return;}
    volatile float *delta=(volatile float*)(nativeState+0x190);
    delta[0]=FloatOffset(target->x,matrix.position.x);delta[1]=FloatOffset(target->y,matrix.position.y);
    int significant=0;CRemotePlayer *self=this;
    __asm {
        mov ecx,self
        push dword ptr [matrix+56]
        push dword ptr [ecx+140h]
        call FloatOffset
        add esp,8
        mov ecx,self
        fst dword ptr [ecx+198h]
        fld dword ptr [ecx+190h]
        fcomp dword ptr [g_pedDrunkSpeedThreshold]
        fnstsw ax
        test ah,41h
        jp incar_significant_pop_z
        fld dword ptr [ecx+194h]
        fcomp dword ptr [g_pedDrunkSpeedThreshold]
        fnstsw ax
        test ah,41h
        jp incar_significant_pop_z
        fcomp dword ptr [g_pedDrunkSpeedThreshold]
        fnstsw ax
        test ah,41h
        jnp incar_significance_done
        jmp incar_significant
    incar_significant_pop_z:
        fstp st(0)
    incar_significant:
        mov significant,1
    incar_significance_done:
    }
    if(!significant)return;
    float zLimit=0.5f;
    if((*(CVehicle**)(nativeState+8))->GetSubtype()==4||(*(CVehicle**)(nativeState+8))->GetSubtype()==5||(*(CVehicle**)(nativeState+8))->GetSubtype()==3)zLimit=2.0f;
    if(delta[0]>g_remoteIncarTeleportDistance||delta[1]>g_remoteIncarTeleportDistance||delta[2]>zLimit) {
        matrix.position=*target;(*(CVehicle**)(nativeState+8))->SetMatrix(matrix);
        (*(CVehicle**)(nativeState+8))->SetSpeed(*(CVector*)(nativeState+0x144));return;
    }
    (*(CVehicle**)(nativeState+8))->GetSpeed(&speed);
    for(int i=0;i<3;++i)if(delta[i]>g_pedDrunkSpeedThreshold) {
        float *wanted=(float*)target+i,*current=(float*)&matrix.position+i,*value=(float*)&speed+i;
        __asm {
            mov ecx,wanted
            mov edx,current
            mov eax,value
            fld dword ptr [ecx]
            fsub dword ptr [edx]
            fmul dword ptr [g_remoteIncarCorrection]
            fadd dword ptr [eax]
            fstp dword ptr [eax]
        }
    }
    if(FloatOffset(speed.x,0)>g_objectTurnVelocityScale||FloatOffset(speed.y,0)>g_objectTurnVelocityScale||FloatOffset(speed.z,0)>g_objectTurnVelocityScale)
        (*(CVehicle**)(nativeState+8))->SetSpeed(speed);
}
// samp.dll RVA: 0x0013c4a0
DWORD g_remoteClampTurnSpeed=1;
// samp.dll RVA: 0x001181bc
const float g_remoteTurnSpeedMinimum=-0.02f;
// samp.dll RVA: 0x00014d80
void CRemotePlayer::UpdateOnfootRotation() {
    CPed *ped=*(CPed**)(nativeState+4);if(!ped)return;
    CMatrix matrix;GameQuaternion current,result;ped->GetMatrix(&matrix);
    GameQuaternionFromMatrixComponents(&matrix,&current);
    GameQuaternionSlerp(&result,(GameQuaternion*)(nativeState+0x4e),&current,0.75f);
    GameQuaternionToMatrixComponents(&result,&matrix);(*(CPed**)(nativeState+4))->SetMatrix(matrix);
    float angle;
    __asm {
        fld dword ptr [matrix+16]
        fchs
        fld dword ptr [matrix+20]
        fpatan
        fmul dword ptr [g_entityRadiansToDegrees]
        fst dword ptr [angle]
        fcomp dword ptr [g_objectScaleLowerBound]
        fld dword ptr [angle]
        fnstsw ax
        test ah,5
        jp onfoot_heading_nonnegative
        fadd dword ptr [g_anglePeriod]
        jmp onfoot_heading_store
    onfoot_heading_nonnegative:
        fcomp dword ptr [g_anglePeriod]
        fnstsw ax
        test ah,1
        jne onfoot_heading_ready
        fld dword ptr [angle]
        fsub dword ptr [g_anglePeriod]
    onfoot_heading_store:
        fstp dword ptr [angle]
    onfoot_heading_ready:
    }
    (*(CPed**)(nativeState+4))->SetRotation(angle);
}
// samp.dll RVA: 0x000151c0
void CRemotePlayer::UpdateIncarRotation() {
    CVector turnSpeed={0,0,0};CVehicle *vehicle=*(CVehicle**)(nativeState+8);if(!vehicle)return;
    if(g_remoteClampTurnSpeed) {
        vehicle->GetTurnSpeed(&turnSpeed);float *values=(float*)&turnSpeed;
        for(int i=0;i<3;++i) {
            if(values[i]>g_pedDrunkShakeScale)values[i]=g_pedDrunkShakeScale;
            else if(values[i]<g_remoteTurnSpeedMinimum)values[i]=g_remoteTurnSpeedMinimum;
        }
        (*(CVehicle**)(nativeState+8))->SetTurnSpeed(turnSpeed);
    }
    CMatrix matrix;GameQuaternion current,result;(*(CVehicle**)(nativeState+8))->GetMatrix(&matrix);
    GameQuaternionFromMatrix(&matrix,&current);
    GameQuaternionSlerp(&result,(GameQuaternion*)(nativeState+0x19c),&current,0.75f);
    GameQuaternionNormalize(&result);GameQuaternionToMatrix(&result,&matrix);
    (*(CVehicle**)(nativeState+8))->SetMatrix(matrix);
}
// samp.dll RVA: 0x000153a0
void CRemotePlayer::UpdateTrain(const CMatrix *target,const CVector *targetSpeed,float trainSpeed) {
    if(!*(CPed**)(nativeState+4)||!*(CVehicle**)(nativeState+8))return;
    CMatrix current;(*(CVehicle**)(nativeState+8))->GetMatrix(&current);
    bool moveX=FloatOffset(target->position.x,current.position.x)>g_objectRotationFactor;
    bool moveY=FloatOffset(target->position.y,current.position.y)>g_objectRotationFactor;
    if(moveX||moveY)(*(CVehicle**)(nativeState+8))->Teleport(target->position);
    CVector speed;(*(CVehicle**)(nativeState+8))->GetSpeed(&speed);speed=*targetSpeed;
    (*(CVehicle**)(nativeState+8))->SetSpeed(speed);(*(CVehicle**)(nativeState+8))->SetTrainSpeed(trainSpeed);
}
// samp.dll RVA: 0x00014e50
void CRemotePlayer::SetOnfootTarget(const CVector *position,const CVector *speed) {
    for(int i=0;i<12;i+=4)*(DWORD*)(nativeState+0x120+i)=*(const DWORD*)((const BYTE*)position+i);
    for(int i=0;i<12;i+=4)*(DWORD*)(nativeState+0x12c+i)=*(const DWORD*)((const BYTE*)speed+i);
    (*(CPed**)(nativeState+4))->SetSpeed(*(CVector*)(nativeState+0x12c));
}
// samp.dll RVA: 0x00015330
void CRemotePlayer::SetIncarTarget(const CMatrix *matrix,const CVector *position,const CVector *speed) {
    GameQuaternionFromMatrix(matrix,(GameQuaternion*)(nativeState+0x19c));
    for(int i=0;i<12;i+=4)*(DWORD*)(nativeState+0x138+i)=*(const DWORD*)((const BYTE*)position+i);
    for(int i=0;i<12;i+=4)*(DWORD*)(nativeState+0x144+i)=*(const DWORD*)((const BYTE*)speed+i);
    (*(CVehicle**)(nativeState+8))->SetSpeed(*speed);
}
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
