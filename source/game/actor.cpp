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
