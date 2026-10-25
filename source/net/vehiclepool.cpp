#include "net/vehiclepool.h"
#include "game/util.h"
#include "chatwindow.h"
#include "main.h"
#include "gui/device_objects.h"
#include "d3dhook/texture_binding.h"
#include <string.h>
#include "net/netgame.h"
#include "net/playerpool.h"
#include "net/localplayer.h"
#include "BitStream.h"
#include "RakClientInterface.h"
#include "game/game.h"
// samp.dll RVA: 0x001184f8
const char g_vehiclePoolNotDeletedMessage[]="Warning: vehicle %u was not deleted";
// samp.dll RVA: 0x0011851c
int g_vehiclePoolDestroyRpc=136;
// samp.dll RVA: 0x0001eb40
BOOL CVehiclePool::Create(const VehiclePoolQueueEntry *entry) {
    if(m_pObjects[entry->id]) {
        g_pChat->AddDebugMessage(g_vehiclePoolNotDeletedMessage,(unsigned)entry->id);Delete(entry->id);
    }
    m_pObjects[entry->id]=g_pGame->CreateVehicle(entry->model,entry->position.x,entry->position.y,entry->position.z,entry->rotation,entry->siren);
    if(!m_pObjects[entry->id])return FALSE;
    if(entry->primaryColor!=-1||entry->secondaryColor!=-1)m_pObjects[entry->id]->SetColor((BYTE)entry->primaryColor,(BYTE)entry->secondaryColor);
    m_pObjects[entry->id]->SetHealth(entry->health);
    m_pGameObjects[entry->id]=m_pObjects[entry->id]->m_pGameVehicle;m_bNotEmpty[entry->id]=TRUE;
    BYTE *state=(BYTE*)this;
    *(float*)(state+0x11ad4+entry->id*12)=entry->position.x;
    *(float*)(state+0x11ad8+entry->id*12)=entry->position.y;
    *(float*)(state+0x11adc+entry->id*12)=entry->position.z;
    if(entry->interior&&m_bNotEmpty[entry->id])m_pObjects[entry->id]->ChangeInterior(entry->interior);
    if(entry->panels||entry->doors||entry->lights)m_pObjects[entry->id]->UpdateDamage(entry->panels,entry->doors,entry->lights);
    m_pObjects[entry->id]->SetTires(entry->tires);stateBD14[entry->id]=1;
    *(DWORD*)(state+0xdc54+entry->id*4)=0;*(DWORD*)(state+0xfb94+entry->id*4)=0;
    UpdateLargestId();return TRUE;
}
// samp.dll RVA: 0x0001edc0
void CVehiclePool::ProcessWaitingList() {
    for(int i=0;i<100;++i)if(queueActive[i]&&g_pGame->IsModelAvailable(queue[i].model)) {
        VehiclePoolQueueEntry *entry=queue+i;Create(entry);
        if(entry->model==537) {
            ++entry->id;entry->model=569;Create(entry);++entry->id;Create(entry);++entry->id;Create(entry);
        }
        if(entry->model==538) {
            ++entry->id;entry->model=570;Create(entry);++entry->id;Create(entry);++entry->id;Create(entry);
        }
        queueActive[i]=0;
    }
}
// samp.dll RVA: 0x0001ecf0
void CVehiclePool::SendDestroyNotification(WORD id) {
    CLocalPlayer *local=((CNetGame*)g_pNetGame)->pools->players->local;
    if(*(WORD*)(local->nativeState+0xfe)==id) {
        RakNet::BitStream stream;stream.Write(id);
        (*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c))->RPC(&g_vehiclePoolDestroyRpc,&stream,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,false);
        local=((CNetGame*)g_pNetGame)->pools->players->local;*(WORD*)(local->nativeState+0xfe)=0xffff;
    }
}

// samp.dll RVA: 0x0001e470
void CVehiclePool::UpdateLargestId() {
    int largest=0;for(int i=0;i<2000;i++)if(m_bNotEmpty[i])largest=i;m_nLargestId=largest;
}
// samp.dll RVA: 0x0001e4d0
CVehiclePool::CVehiclePool() {
    DWORD (WINAPI *tick)()=GetTickCount;
    for(int i=0;i<2000;i++) {
        m_bNotEmpty[i]=FALSE;m_pObjects[i]=0;m_pGameObjects[i]=0;lastTick[i]=tick();
    }
    memset(queueActive,0,sizeof(queueActive));m_nLargestId=0;licensePlatesReady=1;
}
// samp.dll RVA: 0x0001e540
BOOL CVehiclePool::Delete(WORD id) {
    if(id>=2000||!m_bNotEmpty[id]||!m_pObjects[id])return FALSE;
    m_pObjects[id]->ShutdownLicensePlate();
    CVehicle *vehicle=m_pObjects[id];m_bNotEmpty[id]=FALSE;delete vehicle;
    m_pObjects[id]=0;m_pGameObjects[id]=0;UpdateLargestId();return TRUE;
}
// samp.dll RVA: 0x0001eb20
CVehiclePool::~CVehiclePool() {for(WORD id=0;id<2000;id++)Delete(id);}
// samp.dll RVA: 0x0001e5c0
void CVehiclePool::ChangeInterior(WORD id,int interior) {
    if(m_bNotEmpty[id])m_pObjects[id]->ChangeInterior(interior);
}
// samp.dll RVA: 0x0001e5f0
void CVehiclePool::AssignSpecialParams(WORD id,BYTE objective,BYTE locked) {
    if(id>=2000||!m_bNotEmpty[id])return;
    CVehicle *vehicle=m_pObjects[id];if(!vehicle||!stateBD14[id])return;
    if(objective){*((BYTE*)vehicle+0x64)=1;*(DWORD*)((BYTE*)vehicle+0x65)=0;}
    vehicle->Lock(locked);
}
// samp.dll RVA: 0x0001e6c0
WORD CVehiclePool::FindNearestToLocalPlayer() {
    WORD selected=0xffff;float best=10000.0f;
    if(m_nLargestId<0)return selected;
    WORD id=0;
    do {
        if(id<2000&&m_bNotEmpty[id]&&stateBD14[id]) {
            CVehicle *vehicle=m_pObjects[id];int better=0;
            __asm {
                mov ecx,vehicle
                call CEntity::GetDistanceToLocalPlayer
                fcom dword ptr [best]
                fnstsw ax
                test ah,5
                jp reject
                fstp dword ptr [best]
                mov better,1
                jmp compared
            reject:
                fstp st(0)
            compared:
            }
            if(better)selected=id;
        }
    } while((int)(WORD)++id<=m_nLargestId);
    return selected;
}
// samp.dll RVA: 0x0001e730
WORD CVehiclePool::FindNearestToPoint(float x,float y,float z) {
    WORD selected=0xffff;float best=20000.0f;
    if(m_nLargestId<0)return selected;
    WORD id=0;
    do {
        if(m_bNotEmpty[id]&&stateBD14[id]) {
            CVehicle *vehicle=m_pObjects[id];int better=0;
            __asm {
                push z
                push y
                push x
                mov ecx,vehicle
                call CEntity::GetDistanceToPoint
                fcom dword ptr [best]
                fnstsw ax
                test ah,5
                jp reject
                fstp dword ptr [best]
                mov better,1
                jmp compared
            reject:
                fstp st(0)
            compared:
            }
            if(better)selected=id;
        }
    } while((int)(WORD)++id<=m_nLargestId);
    return selected;
}
// samp.dll RVA: 0x001184c4
const char g_vehiclePoolQueueFullMessage[]="All vehicle waiting slots are consumed!";
// samp.dll RVA: 0x0001e7d0
void CVehiclePool::Queue(const VehiclePoolQueueEntry *entry) {
    int slot=0;while(slot<100&&queueActive[slot])slot++;
    if(slot==100){g_pChat->AddDebugMessage(g_vehiclePoolQueueFullMessage);return;}
    queueActive[slot]=1;memcpy(queue+slot,entry,sizeof(*entry));
}
// samp.dll RVA: 0x0001e680
DWORD CVehiclePool::GetRef(int id) {return m_pGameObjects[id]?GamePoolVehicleGetIndex(m_pGameObjects[id]):0xffff;}
// samp.dll RVA: 0x0001e6a0
DWORD CVehiclePool::GetRef(void *backend) {return backend?GamePoolVehicleGetIndex(backend):0xffff;}
// samp.dll RVA: 0x0001e650
WORD CVehiclePool::Find(void *gameObject) {
    for(int i=1;i<=m_nLargestId;i++)if(m_pGameObjects[i]==gameObject)return (WORD)i;
    return 0xffff;
}
// samp.dll RVA: 0x00001120
CVehicle *CVehiclePool::Get(WORD id) { return id<2000 && m_bNotEmpty[id]?m_pObjects[id]:0; }

// Native returns the raw state DWORD; do not normalize it to bool.
// samp.dll RVA: 0x00001150
BOOL CVehiclePool::DoesExist(WORD id) { return id<2000?m_bNotEmpty[id]:FALSE; }

#include "game/object.h"
// samp.dll RVA: 0x0001e8e0
bool CVehiclePool::TestCollisionShapes(const GameEntityCollisionShape *first,const GameEntityCollisionShape *second) {
    for(int index=0;index<=m_nLargestId;index++) {
        if(!m_bNotEmpty[index])continue;
        CVehicle *vehicle=m_pObjects[index];
        if(!vehicle||!vehicle->DoesExist())continue;
        BYTE nearby;
        __asm {
            mov ecx,vehicle
            call CEntity::GetDistanceToCamera
            fcomp g_objectStreamingNearDistance
            fnstsw ax
            test ah,5
            setnp nearby
        }
        if(!nearby||vehicle->IsIgnored())continue;
        if(vehicle->TestCollisionShapes(first,second,1))return true;
    }
    return false;
}

// samp.dll RVA: 0x0001e830
void CVehiclePool::ConstructLicensePlates() {
    BYTE *state=(BYTE*)this;
    if(!*(DWORD*)(state+0x17894))return;
    if(g_devicePlateManager && !((CLicensePlate*)g_devicePlateManager)->defaultTexture) {
        IDirect3DTexture9 *created=((CLicensePlate*)g_devicePlateManager)->Create("XYZSR998");
        ((CLicensePlate*)g_devicePlateManager)->defaultTexture=created;
    }
    if(m_nLargestId>=0) {
        DWORD counter=0;
        do {
            WORD slot=(WORD)counter;
            if(*(DWORD*)(state+0x3074+slot*4))(*(CVehicle**)(state+0x1134+slot*4))->ConstructLicensePlate();
        } while((int)(WORD)++counter<=m_nLargestId);
    }
}
// samp.dll RVA: 0x0001e8a0
void CVehiclePool::ShutdownLicensePlates() {
    BYTE *state=(BYTE*)this;
    if(m_nLargestId>=0) {
        DWORD counter=0;
        do {
            WORD slot=(WORD)counter;
            if(*(DWORD*)(state+0x3074+slot*4))(*(CVehicle**)(state+0x1134+slot*4))->ShutdownLicensePlate();
        } while((int)(WORD)++counter<=m_nLargestId);
    }
}
