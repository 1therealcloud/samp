#include "game/playerped.h"
#include "game/ped_commands.h"
#include "game/scripting.h"
#include "game/game.h"
#include <string.h>
// samp.dll RVA: 0x002a9da8
void *g_gamePlayerPeds[210];
// samp.dll RVA: 0x002aa0f0
BYTE g_gamePlayerControllerState[210][16];
// samp.dll RVA: 0x000b3860
void *__stdcall GameFindLocalPed() { return *(void**)0xb7cd98; }
// samp.dll RVA: 0x000b3f20
void __stdcall GameAssignPlayerPed(BYTE player,void *ped) { g_gamePlayerPeds[player]=ped;memset(g_gamePlayerControllerState[player],0,16); }
// samp.dll RVA: 0x000ab690
CPed::CPed() {
    m_nHandle=1;m_pGamePed=GameFindLocalPed();m_pGameEntity=GameFindLocalPed();m_nPlayerNumber=0;
    GameAssignPlayerPed(0,m_pGamePed);
    Scripting::ProcessCommand(&g_pedCommand087e,m_nHandle,1);
    Scripting::ProcessCommand(&g_pedCommand0446,m_nHandle,0);
    unknown2a8=unknown2ac=1;m_arrow=0;m_parachuteObject=0;m_bUrinating=0;unknown2d2=0;
    m_bDancing=0;m_danceStyle=0;m_bUsingCellphone=0;m_lastDanceMove=0;
    m_stuffType=m_stuffObject=m_drunkLevel=0;unknown2f6=unknown2f7=unknown2f8=0;
    for(int i=0;i<10;i++) { m_accessoryOccupied[i]=FALSE;m_accessoryObjects[i]=0;memset(&m_accessoryInfo[i],0,52); }
    unknown2f9=0;
}
// samp.dll RVA: 0x000b0ea0
CPed::~CPed() { Destroy(); }
// samp.dll RVA: 0x000aba90
BOOL CPed::IsInVehicle() { return m_pGamePed?(*(DWORD*)((BYTE*)m_pGamePed+0x46c)>>8)&1:FALSE; }
// samp.dll RVA: 0x000abbe0
BOOL CPed::IsPassenger() {
    BYTE *ped=(BYTE*)m_pGamePed;BYTE *vehicle=*(BYTE**)(ped+0x58c);
    if(vehicle && (*(DWORD*)(ped+0x46c)&0x100) &&
        (*(void**)(vehicle+0x460)!=ped || *(short*)(vehicle+0x22)==0x23a || *(short*)(vehicle+0x22)==0x239))return TRUE;
    return FALSE;
}
// samp.dll RVA: 0x00001010
CPed *CGame::GetPlayerPed() { if(!m_pPlayerPed)m_pPlayerPed=new CPed;return m_pPlayerPed; }

#include "game/pool_helpers.h"
#include "game/object_math.h"

// samp.dll RVA: 0x000adca0
BOOL CPed::UsingCellphone() {
    return m_bUsingCellphone;
}

// samp.dll RVA: 0x000ad890
DWORD CPed::GetStuff() {
    return m_stuffType;
}

// samp.dll RVA: 0x000ade60
BOOL CPed::DoesUrinating() {
    return m_bUrinating;
}

// samp.dll RVA: 0x000aba60
BYTE CPed::GetState() {
    return *((BYTE*)m_pGamePed+0x530);
}

// samp.dll RVA: 0x000abc20
void * CPed::GetVehicle() {
    return *(void**)((BYTE*)m_pGamePed+0x58c);
}

// samp.dll RVA: 0x000adbb0
void CPed::SetDrunkLevel(DWORD level) {
    m_drunkLevel=level;
}

// samp.dll RVA: 0x000ad760
BOOL CPed::DoesDancing() {
    return unknown2d2!=0;
}

// samp.dll RVA: 0x000ab9f0
DWORD CPed::GetFlags() {
    return m_pGamePed?*(DWORD*)((BYTE*)m_pGamePed+0x46c):0;
}

// samp.dll RVA: 0x000ab7d0
void CPed::ResetPointers() {
    m_pGamePed=GamePoolPedGetAt(m_nHandle);
    m_pGameEntity=m_pGamePed;
}

// samp.dll RVA: 0x000ab990
void CPed::SetHealth(float health) {
    if(m_pGamePed) *(DWORD*)((BYTE*)m_pGamePed+0x540)=*(DWORD*)&health;
}

// samp.dll RVA: 0x000ab9d0
void CPed::SetArmour(float armour) {
    if(m_pGamePed) *(DWORD*)((BYTE*)m_pGamePed+0x548)=*(DWORD*)&armour;
}

// samp.dll RVA: 0x000aba10
void CPed::SetFlags(DWORD flags) {
    if(m_pGamePed) *(DWORD*)((BYTE*)m_pGamePed+0x46c)=flags;
}

// samp.dll RVA: 0x000ab8b0
DWORD CPed::GetVehicleRef() {
    if(!m_pGamePed) return 0;
    return GamePoolVehicleGetIndex(*(void**)((BYTE*)m_pGamePed+0x58c));
}

// samp.dll RVA: 0x000ab970
float CPed::GetHealth() {
    if(!m_pGamePed) return g_objectScaleLowerBound;
    return *(float*)((BYTE*)m_pGamePed+0x540);
}

// samp.dll RVA: 0x000ab9b0
float CPed::GetArmour() {
    if(!m_pGamePed) return g_objectScaleLowerBound;
    return *(float*)((BYTE*)m_pGamePed+0x548);
}

// samp.dll RVA: 0x000aba70
void CPed::SetState(BYTE state) {
    if(m_pGamePed) *(DWORD*)((BYTE*)m_pGamePed+0x530)=state;
}

// samp.dll RVA: 0x000ad480
BOOL CPed::IsOnGround() {
    return m_pGamePed&&(*((BYTE*)m_pGamePed+0x46c)&3)?TRUE:FALSE;
}

// samp.dll RVA: 0x000abd60
void * CPed::GetCurrentWeaponSlot() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped) return 0;
    return ped+0x5a0+*(ped+0x718)*0x1c;
}

#include "game/aimstuff.h"

// samp.dll RVA: 0x000ab850
void CPed::SetAim(const CameraAim *aim) {
    AimStuff::SetAim(m_nPlayerNumber,aim);
}

// samp.dll RVA: 0x000ab7f0
void CPed::SetInitialState() {
    // GTA SA virtual address: 0x0060cd20, backend ECX, one callee-popped argument.
    void *ped=m_pGamePed;
    __asm { push 0
        mov ecx,ped
        mov edx,0x60cd20
        call edx
    }
}

// samp.dll RVA: 0x000adee0
const char * CPed::GetLoadedShoppingDataSubsection() {
    if(!m_pGamePed) return 0;
    // GTA SA virtual address: 0x00a9a7d8; returns the address, not a pointer loaded there.
    return GamePoolPedGetAt(m_nHandle)?(const char*)0xa9a7d8:0;
}

// samp.dll RVA: 0x000abd30
void CPed::RemoveWeaponWhenEnteringVehicle() {
    void *ped=m_pGamePed;
    if(ped) {
        // GTA SA virtual address: 0x005e6370, backend ECX, argument0.
        __asm { push 0
            mov ecx,ped
            mov edx,0x5e6370
            call edx
        }
    }
}

// samp.dll RVA: 0x000aeb60
void * CPed::GetAimedPed() {
    BYTE *ped=(BYTE*)m_pGamePed;
    // GTA SA virtual address: excluded vtable0x00863c40.
    if(!ped||*(DWORD*)ped==0x863c40||!*(DWORD*)(ped+0x18)) return 0;
    return *(void**)(ped+0x79c);
}

// samp.dll RVA: 0x00013580
void * CPed::GetAccessory(int index) {
    if(index<0||index>=10||!m_accessoryOccupied[index]) return 0;
    return m_accessoryObjects[index];
}

// samp.dll RVA: 0x000ad4a0
void CPed::ResetDamageEntity() {
    if(m_pGamePed) {
        *(DWORD*)((BYTE*)m_pGamePed+0x764)=0;
        *(DWORD*)((BYTE*)m_pGamePed+0x760)=255;
    }
}

// samp.dll RVA: 0x000adcd0
void CPed::SetFightingStyle(int style) {
    if(m_pGamePed) Scripting::ProcessCommand(&g_pedFightingStyleCommand,m_nHandle,style,6);
}

// samp.dll RVA: 0x000aeab0
BOOL CPed::GetAccessoryState(int index) {
    if(index<0||index>=10) return FALSE;
    return m_accessoryOccupied[index]==1;
}

// samp.dll RVA: 0x000adc70
void CPed::EnableCellphone(BOOL enabled) {
    if(m_pGamePed) {
        m_bUsingCellphone=enabled;
        Scripting::ProcessCommand(&g_pedCellphoneCommand,m_nHandle,enabled);
    }
}

// samp.dll RVA: 0x000ab8d0
void CPed::DeleteArrow() {
    if(m_arrow) {
        Scripting::ProcessCommand(&g_pedDeleteArrowCommand,m_arrow);
    }
    m_arrow=0;
}

// samp.dll RVA: 0x000adf00
void CPed::LoadShoppingDataSubsection(const char *name) {
    const char *end=name;
    while(*end) ++end;
    if((DWORD)(end-name)<=8) {
        // GTA SA virtual address: 0x0049bbe0, cdecl.
        ((void (__cdecl *)(const char*))0x49bbe0)(name);
    }
}

// samp.dll RVA: 0x000ac370
void CPed::SetSpawnInfo(const CVector *position,float rotation) {
    Scripting::ProcessCommand(&g_pedSpawnInfoCommand,(double)position->x,(double)position->y,(double)position->z,(double)rotation,0);
}

// samp.dll RVA: 0x000aba30
BOOL CPed::IsDead() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped) return TRUE;
    BOOL dead;
    __asm { mov edx,ped
        fld dword ptr [edx+0x540]
        fcomp dword ptr [g_objectScaleLowerBound]
        fnstsw ax
        test ah,0x41
        setne al
        movzx eax,al
        mov dead,eax
    }
    return dead;
}
