#include "net/actorpool.h"
#include "game/actor.h"
// samp.dll RVA: 0x00001660
void CActorPool::UpdateLargestId(){int largest=0;for(int i=0;i<1000;++i)if(active[i])largest=i;largestId=largest;}
// samp.dll RVA: 0x000016c0
CActorPool::CActorPool(){for(int i=0;i<1000;++i){active[i]=0;actors[i]=0;peds[i]=0;}largestId=0;}
// samp.dll RVA: 0x000016f0
BOOL CActorPool::Delete(WORD id){
    if(id>=1000 || !active[id] || !actors[id])return FALSE;
    CActor *actor=actors[id];active[id]=0;delete actor;
    actors[id]=0;peds[id]=0;UpdateLargestId();return TRUE;
}
// samp.dll RVA: 0x000018e0
CActorPool::~CActorPool(){for(WORD i=0;i<1000;++i)Delete(i);}
// samp.dll RVA: 0x000018b0
DWORD CActorPool::Find(void *ped) {
    BYTE *state=(BYTE*)this;DWORD actor=0;
    if(*(LONG*)state>=0) {
        do {
            if(*(void**)(state+0x1f44+actor*4)==ped)return actor;
        } while((LONG)++actor<=*(LONG*)state);
    }
    // Native OR AX,FFFF preserves the high half of the terminating counter.
    return (actor&0xffff0000)|0xffff;
}

#include "main.h"
#include "chatwindow.h"
#include "game/custommodels.h"
// samp.dll RVA: 0x00001900
BOOL CActorPool::Create(const ActorCreateInfo *info) {
    // Native deliberately performs no ID bounds check.
    if(actors[info->id]) {
        g_pChat->AddDebugMessage("Warning: actor %u was not deleted",(unsigned)info->id);
        Delete(info->id);
    }
    actors[info->id]=new CActor(info->model,info->position,info->heading);
    if(!actors[info->id])return FALSE;
    // Actor backend is distinct from the base CEntity pointer.
    peds[info->id]=*(void**)((BYTE*)actors[info->id]+0x48);
    active[info->id]=1;
    actors[info->id]->SetHealth(info->health);
    actors[info->id]->SetInvulnerable(info->invulnerable?1:0);
    state2ee4[info->id]=1;
    state3e84[info->id]=0;
    UpdateLargestId();
    if(g_customModels && info->customModel>20000 &&
       (GameIsPedModel(info->customModel) || g_customModels->DoesExist(info->customModel)))
        actors[info->id]->SetModelIndex(info->customModel);
    return TRUE;
}
