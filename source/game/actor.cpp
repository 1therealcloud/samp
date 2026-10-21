#include "game/actor.h"
#include "game/scripting.h"
#include <string.h>
// samp.dll RVA: 0x0011e2d0
const Scripting::OpcodeInfo g_actorKillCommand={0x321,"i"};
// samp.dll RVA: 0x0011e294
const Scripting::OpcodeInfo g_actorImmunityCommand={0x2ab,"iiiiii"};
// samp.dll RVA: 0x0009c030
float CActor::GetHealth(){return actorBackend?*(float*)((BYTE*)actorBackend+0x540):0.0f;}
// samp.dll RVA: 0x0009c050
void CActor::SetHealth(float health){
    if(!actorBackend)return;
    memcpy((BYTE*)actorBackend+0x540,&health,4);
    if(*(float*)((BYTE*)actorBackend+0x540)<=0.0f)Scripting::ProcessCommand(&g_actorKillCommand,m_nHandle);
}
// samp.dll RVA: 0x0009c090
float CActor::GetArmour(){return actorBackend?*(float*)((BYTE*)actorBackend+0x548):0.0f;}
// samp.dll RVA: 0x0009c0b0
void CActor::SetArmour(float armour){if(actorBackend)memcpy((BYTE*)actorBackend+0x548,&armour,4);}
// samp.dll RVA: 0x0009c0d0
DWORD CActor::GetBackendFlags(){return actorBackend?*(DWORD*)((BYTE*)actorBackend+0x46c):0;}
// samp.dll RVA: 0x0009c0e0
void CActor::SetBackendFlags(DWORD flags){if(actorBackend)*(DWORD*)((BYTE*)actorBackend+0x46c)=flags;}
// samp.dll RVA: 0x0009c100
BOOL CActor::IsDead(){return !actorBackend || !(*(float*)((BYTE*)actorBackend+0x540)>0.0f);}
// samp.dll RVA: 0x0009c130
BYTE CActor::GetBackendByte530(){return *((BYTE*)actorBackend+0x530);}
// samp.dll RVA: 0x0009c140
void CActor::SetBackendByte530(BYTE value){if(actorBackend)*(DWORD*)((BYTE*)actorBackend+0x530)=value;}
// samp.dll RVA: 0x0009c160
DWORD CActor::HasBackendFlag100(){return actorBackend?(*(DWORD*)((BYTE*)actorBackend+0x46c)>>8)&1:0;}
// samp.dll RVA: 0x0009c180
void CActor::SetInvulnerable(BYTE enabled){
    if(!actorBackend)return;((BYTE*)this)[0x55]=enabled;
    if(enabled)Scripting::ProcessCommand(&g_actorImmunityCommand,m_nHandle,1,1,1,1,1);
    else Scripting::ProcessCommand(&g_actorImmunityCommand,m_nHandle,0,0,0,1,0);
}

#include "game/playerped.h"
#include "game/util.h"
// samp.dll RVA: 0x0009be80
void CActor::Destroy() {
    void *saved=actorBackend;
    // GTA SA virtual address: excluded backend vtable.
    if(saved && GamePoolPedGetAt(m_nHandle) && *(DWORD*)actorBackend!=0x863c40) {
        DWORD *table=*(DWORD**)saved;
        __asm {mov ecx,saved}
        __asm {mov eax,table}
        __asm {push 1}
        __asm {call dword ptr [eax]}
        actorBackend=0;m_pGameEntity=0;
        return;
    }
    m_nHandle=0;actorBackend=0;m_pGameEntity=0;
}
// samp.dll RVA: 0x0009bff0
void CActor::SetRotation(float heading) {
    if(!actorBackend || !GamePoolPedGetAt(m_nHandle))return;
    // Preserve helper's extended ST0 until the native final store; reload backend.
    __asm {push heading}
    __asm {call GameHeadingToRadians}
    __asm {add esp,4}
    __asm {mov ecx,this}
    __asm {mov edx,dword ptr [ecx+0x48]}
    __asm {fstp dword ptr [edx+0x55c]}
}
// samp.dll RVA: 0x0009c1d0
CActor::~CActor(){Destroy();}

#include "game/game.h"
// samp.dll RVA: 0x0011e2e4
const Scripting::OpcodeInfo g_actorAnimationCommand={0x812,"issfiiiii"};
// samp.dll RVA: 0x0009bee0
void CActor::PerformAnimation(const char *name,const char *library,float speed,DWORD loop,DWORD lockX,DWORD lockY,DWORD freeze,int time) {
    if(!actorBackend || !GamePoolPedGetAt(m_nHandle))return;
    if(_stricmp(library,g_pedBlockedAnimationLibrary)==0)return;
    if(!g_pGame->IsAnimationLoaded(library)) {
        g_pGame->RequestAnimation(library);
        if(!g_pGame->IsAnimationLoaded(library)) {
            void (WINAPI *volatile sleep)(DWORD)=::Sleep;int attempts=0;
            do {sleep(1);if(++attempts==15)return;}while(!g_pGame->IsAnimationLoaded(library));
        }
    }
    Scripting::ProcessCommand(&g_actorAnimationCommand,m_nHandle,name,library,(double)speed,loop,lockX,lockY,freeze,time);
}
// samp.dll RVA: 0x00181e5c
void *g_actorTaskBackend;
// samp.dll RVA: 0x0009bfb0
DWORD GameClearActorTasks() {
    void *tasks=*(void**)((BYTE*)g_actorTaskBackend+0x47c);
    // GTA SA virtual address: task clearing backend, thiscall with argument1.
    ((void(__fastcall*)(void*,void*,int))0x601640)(tasks,0,1);return 1;
}
// samp.dll RVA: 0x0009bfd0
void CActor::ClearTasks(){g_actorTaskBackend=actorBackend;if(actorBackend)GameClearActorTasks();}
