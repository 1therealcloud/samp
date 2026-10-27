#include "net/labelpool.h"
#include "net/playerpool.h"
#include "net/remoteplayer.h"
#include "net/vehiclepool.h"
#include "game/game.h"
#include "game/playerped.h"
#include "game/util.h"
#include "gui/chat_dependencies.h"
#include "gui/device_lifecycle.h"
#include "label.h"
#include "main.h"
#include <stdlib.h>
// Native label text uses the CRT calloc/free entries.
#include <string.h>
// samp.dll RVA: 0x0000e3c0
DWORD CLabelPool::IsActive(WORD id) {
    return id<2048?active[id]:0;
}
// samp.dll RVA: 0x00001190
CLabelPool::CLabelPool() {
    for(int id=0;id<2048;id++) {memset(&labels[id],0,sizeof(TextLabel));active[id]=0;}
}
// samp.dll RVA: 0x000015e0
CLabelPool::~CLabelPool() {
    for(WORD id=0;id<2048;id++)if(active[id])Delete(id);
}
// samp.dll RVA: 0x000011d0
void CLabelPool::Create(WORD id,const char *text,DWORD color,CVector position,float drawDistance,
    BYTE behindWalls,WORD player,WORD vehicle) {
    if(id>=2048)return;
    if(active[id]) {
        if(labels[id].text)free(labels[id].text);
        memset(&labels[id],0,sizeof(TextLabel));
    }
    DWORD converted=(color<<24)|(color>>8);
    char *allocated=(char*)calloc(1,strlen(text)+1);
    TextLabel *entry=&labels[id];entry->text=allocated;
    // Native copies after freeing the old text and does not recover from OOM.
    do {*allocated++=*text;}while(*text++);
    if(strlen(entry->text)<400)ChatPrepareMessage(entry->text,399);
    entry->color=converted;memcpy(&entry->position,&position,12);
    memcpy(&entry->drawDistance,&drawDistance,4);
    entry->behindWalls=behindWalls;
    entry->attachedPlayer=player;entry->attachedVehicle=vehicle;
    active[id]=1;
}
// samp.dll RVA: 0x000012e0
BOOL CLabelPool::Delete(WORD id) {
    if(id>=2048||!active[id])return FALSE;
    if(labels[id].text)free(labels[id].text);
    memset(&labels[id],0,sizeof(TextLabel));active[id]=0;
    return TRUE;
}
// samp.dll RVA: 0x00001350
void CLabelPool::Draw() {
    CPed *local=g_pGame->GetPlayerPed();
    if(!local||!g_pNetGame)return;
    BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);
    CPlayerPool *players=*(CPlayerPool**)(pools+8);
    CVehiclePool *vehicles=*(CVehiclePool**)(pools+0xc);
    if(g_deviceLabel)g_deviceLabel->Begin();
    for(int id=0;id<2048;id++) {
        if(!active[id])continue;
        TextLabel *entry=&labels[id];
        BOOL shadow=(entry->color&0xff000000)==0xff000000;
        if(!entry->text||!*entry->text)continue;
        CVector position;
        if(entry->attachedPlayer!=0xffff) {
            WORD player=entry->attachedPlayer;
            if(player>=1004||!players->IsConnected(player))continue;
            CRemotePlayer *remote=players->GetPlayer(player);
            // Native calls the predicate before a separate live lookup.
            if(!remote->IsActive())continue;
            remote=players->GetPlayer(player);
            CPed *ped=*(CPed**)((BYTE*)remote+4);
            if(!ped)continue;
            BYTE nearby;float *limit=&entry->drawDistance;
            __asm {
                mov ecx,ped
                call CEntity::GetDistanceToLocalPlayer
                mov edx,limit
                fcomp dword ptr [edx]
                fnstsw ax
                test ah,41h
                setnp nearby
            }
            if(!nearby)continue;
            CVector bone;ped->GetBonePosition(8,&bone);
            position.x=bone.x+entry->position.x;
            position.y=bone.y+entry->position.y;
            memcpy(&position.z,&bone.z,4);
            float *zOffset=&entry->position.z,*height=&position.z;
            __asm {
                mov ecx,ped
                call CEntity::GetDistanceToCamera
                fmul g_pedDrunkSpeedThreshold
                mov edx,zOffset
                fadd dword ptr [edx]
                mov edx,height
                fadd dword ptr [edx]
                fstp dword ptr [edx]
            }
        } else if(entry->attachedVehicle!=0xffff) {
            WORD vehicle=entry->attachedVehicle;
            if(!vehicles->DoesExist(vehicle))continue;
            CVehicle *object=vehicles->Get(vehicle);
            if(!object)continue;
            BYTE nearby;float *limit=&entry->drawDistance;
            __asm {
                mov ecx,object
                call CEntity::GetDistanceToLocalPlayer
                mov edx,limit
                fcomp dword ptr [edx]
                fnstsw ax
                test ah,41h
                setnp nearby
            }
            if(!nearby)continue;
            CMatrix matrix;object->GetMatrix(&matrix);
            CVector offset;memcpy(&offset,&entry->position,12);
            GameTransformPoint(&position,&matrix,&offset);
        } else {
            BYTE nearby;float *point=&entry->position.x,*limit=&entry->drawDistance;
            __asm {
                mov edx,point
                push dword ptr [edx+8]
                push dword ptr [edx+4]
                push dword ptr [edx]
                mov ecx,local
                call CEntity::GetDistanceToPoint
                mov edx,limit
                fcomp dword ptr [edx]
                fnstsw ax
                test ah,41h
                setnp nearby
            }
            if(!nearby)continue;
            memcpy(&position,&entry->position,12);
        }
        // Per-label Draw remains unconditional even when Begin was skipped.
        g_deviceLabel->Draw(&position,entry->text,entry->color,shadow,entry->behindWalls);
    }
    if(g_deviceLabel)g_deviceLabel->End();
}
