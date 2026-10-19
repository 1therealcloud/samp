#include "game/playerped.h"
#include "game/ped_commands.h"
#include "game/aimstuff.h"
#include "client/exceptions.h"
// samp.dll RVA: 0x0011ef54
const Scripting::OpcodeInfo g_pedGetCurrentWeaponCommand={0x470,"iv"};
// samp.dll RVA: 0x0011ef40
const Scripting::OpcodeInfo g_pedSetArmedWeaponCommand={0x1b9,"ii"};
// samp.dll RVA: 0x000acb10
BOOL CPed::HasDanceState() { return m_bDancing!=0; }
// samp.dll RVA: 0x000aca60
BOOL CPed::HasDanceStyle() { return m_danceStyle!=0; }
// samp.dll RVA: 0x000ab870
DWORD CPed::GetCurrentWeapon() {
    if(m_pGamePed && GamePoolPedGetAt(m_nHandle)) {
        DWORD weapon=(DWORD)this;
        Scripting::ProcessCommand(&g_pedGetCurrentWeaponCommand,m_nHandle,&weapon);
        return weapon&0xff;
    }
    return 0;
}
// samp.dll RVA: 0x000abc80
void CPed::SetArmedWeapon(DWORD weapon,BYTE direct) {
    if(m_pGamePed && GamePoolPedGetAt(m_nHandle)) {
        *g_clientCrashPedContext=m_nPlayerNumber;
        AimStuff::UpdateLocalSkills();AimStuff::ApplyPlayerSkills(m_nPlayerNumber);
        void *ped=m_pGamePed;
        if((!ped || !(*(DWORD*)((BYTE*)ped+0x46c)&0x100)) && !direct) {
            Scripting::ProcessCommand(&g_pedSetArmedWeaponCommand,m_nHandle,weapon);
            AimStuff::ApplyLocalSkills();*g_clientCrashPedContext=0;
            return;
        }
        if(ped)((void(__fastcall*)(void*,void*,DWORD))0x5e6280)(ped,0,weapon);
        AimStuff::ApplyLocalSkills();*g_clientCrashPedContext=0;
    }
}
