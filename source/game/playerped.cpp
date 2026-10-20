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

#include "game/keystuff.h"
#include "client/exceptions.h"
#include "main.h"

// samp.dll RVA: 0x000acdb0
void * CPed::GetWeaponSlot(DWORD weapon) {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(ped) for(BYTE index=0;index<13;++index) {
        BYTE *slot=ped+0x5a0+index*0x1c;
        if(*(DWORD*)slot==weapon) return slot;
    }
    return 0;
}

// samp.dll RVA: 0x000afc80
void CPed::SetWeaponAmmo(BYTE weapon,WORD ammo) {
    if(m_pGamePed) {
        BYTE *slot=(BYTE*)GetWeaponSlot(weapon);
        if(slot) *(DWORD*)(slot+0x0c)=ammo;
    }
}

// samp.dll RVA: 0x000ab810
BYTE CPed::GetAim() {
    KeyPad *keys=KeyStuff::GetInternalKeys();
    if(keys->keys1[11]) return 1;
    if(keys->keys1[10]) return 2;
    return keys->keys1[9]?3:0;
}

// samp.dll RVA: 0x000ad600
BOOL CPed::PerformingCustomAnimation() {
    if(!m_pGamePed||!CEntity::DoesExist()) return FALSE;
    BYTE *tasks=*(BYTE**)((BYTE*)m_pGamePed+0x47c);
    return *(DWORD*)(tasks+0x10)!=0;
}

// samp.dll RVA: 0x000ac9e0
BOOL CPed::HasJetpack() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped||(*(DWORD*)(ped+0x46c)&0x100)) return FALSE;
    BYTE *tasks=*(BYTE**)(ped+0x47c);
    BYTE *task=*(BYTE**)(tasks+0x10);
    // GTA SA virtual address: 0x008705c4, native task vtable comparison.
    return task&&*(DWORD*)task==0x8705c4?TRUE:FALSE;
}

// samp.dll RVA: 0x000ad720
void CPed::StopDancing() {
    CMatrix matrix;
    unknown2d2=0;
    GetMatrix(&matrix);
    Teleport(matrix.position);
}

// samp.dll RVA: 0x000ad510
float CPed::GetAimZ() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped) return g_objectScaleLowerBound;
    BYTE *state=*(BYTE**)(ped+0x480);
    return *(float*)(state+0x54);
}

// samp.dll RVA: 0x000abd80
BOOL CPed::CurrentWeaponHasAmmo() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped) return TRUE;
    BYTE *slot=(BYTE*)((DWORD)ped+0x5a0+*(ped+0x718)*0x1c);
    if(!slot) return TRUE;
    DWORD weapon=*(DWORD*)slot;
    if(weapon<=15||weapon==46||*(DWORD*)(slot+0x0c)) return TRUE;
    return FALSE;
}

// samp.dll RVA: 0x000ad4d0
void CPed::RemoveWeaponModel(int weapon) {
    void *ped=m_pGamePed;
    if(ped) {
        BYTE *info=(BYTE*)g_pGame->GetWeaponInfo(weapon,1);
        DWORD model=*(DWORD*)(info+0x0c);
        // GTA SA virtual address: 0x005e3990, ECX original backend, argument model.
        __asm { push model
            mov ecx,ped
            mov edx,0x5e3990
            call edx
        }
    }
}

// samp.dll RVA: 0x000ae080
DWORD CPed::GetBonePosition(int bone,CVector *position) {
    void *ped=m_pGamePed;
    // GTA SA virtual addresses: excluded vtable0x00863c40, callee0x005e4280.
    if(!ped||*(DWORD*)ped==0x863c40) return 0;
    DWORD result;
    __asm { push 0
        push bone
        push position
        mov ecx,ped
        mov edx,0x5e4280
        call edx
        mov result,eax
    }
    return result;
}

// samp.dll RVA: 0x000abc30
void CPed::ClearWeapons() {
    void *ped=m_pGamePed;
    *g_clientCrashPedContext=m_nPlayerNumber;
    if(ped) {
        // GTA SA virtual address: 0x005e6320, unchecked backend ECX, no stack arguments.
        __asm { mov ecx,ped
            mov eax,0x5e6320
            call eax
        }
    }
    *g_clientCrashPedContext=0;
}

#include "game/vehicle.h"

// samp.dll RVA: 0x000ac930
void CPed::EnableJetpack() {
    if(m_pGamePed) {
        *g_clientCrashPedContext=m_nPlayerNumber;
        CMatrix *matrix=*(CMatrix**)((BYTE*)m_pGamePed+0x14);
        Teleport(matrix->position);
        // GTA SA virtual address: 0x00439600, no stack arguments.
        ((void (__cdecl *)())0x439600)();
        *g_clientCrashPedContext=0;
    }
}

// samp.dll RVA: 0x000ac730
void CPed::HandsUp() {
    if(m_pGamePed&&!(*(DWORD*)((BYTE*)m_pGamePed+0x46c)&0x100)&&CEntity::DoesExist()&&GamePoolPedGetAt(m_nHandle))
        Scripting::ProcessCommand(&g_pedHandsUpCommand,m_nHandle,-2);
}

// samp.dll RVA: 0x000ab920
void CPed::SetImmunities(DWORD bullet,DWORD fire,DWORD explosion,DWORD collision,DWORD melee) {
    if(m_pGamePed&&GamePoolPedGetAt(m_nHandle))
        Scripting::ProcessCommand(&g_pedImmunitiesCommand,m_nHandle,bullet,fire,explosion,collision,melee);
}

// samp.dll RVA: 0x000ac7e0
void CPed::HoldObject(DWORD object) {
    if(m_pGamePed&&CEntity::DoesExist()&&GamePoolPedGetAt(m_nHandle)) {
        void *ped=m_pGamePed;
        // GTA SA virtual address: 0x005e4390, ECX backend, arguments object/1.
        __asm { push 1
            push object
            mov ecx,ped
            mov edx,0x5e4390
            call edx
        }
    }
}

// samp.dll RVA: 0x000ad550
void CPed::SetAimZ(float value) {
    DWORD accepted;
    __asm { fld value
        fcomp dword ptr [g_vehicleTrainSpeedMaximum]
        fnstsw ax
        test ah,0x41
        jz reject
        fld value
        fcomp dword ptr [g_vehicleTrainSpeedMinimum]
        fnstsw ax
        test ah,5
        jnp reject
        mov accepted,1
        jmp compared
    reject:
        mov accepted,0
    compared:
    }
    if(accepted&&m_pGamePed) {
        BYTE *state=*(BYTE**)((BYTE*)m_pGamePed+0x480);
        *(DWORD*)(state+0x54)=*(DWORD*)&value;
    }
}

// samp.dll RVA: 0x002a92a0
void *g_pedCommandTaskSnapshot;

// samp.dll RVA: 0x000adbd0
void CPed::ApplyCommandTask(const char *name,int p1,int p2,int p3,const CVector *p4,int p5,float p6,int p7,int p8,int p9) {
    void *ped=m_pGamePed;
    if(ped) {
        // GTA SA virtual addresses: receiver0x00c15448, callee0x00618970.
        __asm { push p9
            push p8
            push p7
            push p6
            push p5
            push p4
            push p3
            push p2
            push p1
            push ped
            push name
            mov ecx,0xc15448
            mov edx,0x618970
            call edx
        }
    }
}

// samp.dll RVA: 0x000adc20
void CPed::DestroyCommandTask() {
    if(m_pGamePed) {
        BYTE *manager=*(BYTE**)((BYTE*)m_pGamePed+0x47c);
        void *task=*(void**)(manager+0x2c);
        g_pedCommandTaskSnapshot=task;
        if(task) {
            // GTA SA virtual address: 0x00639330, ECX published task, argument1.
            __asm { mov ecx,g_pedCommandTaskSnapshot
                mov edx,0x639330
                push 1
                call edx
            }
            manager=*(BYTE**)((BYTE*)m_pGamePed+0x47c);
            *(DWORD*)(manager+0x2c)=0;
        }
    }
}

// samp.dll RVA: 0x000ac780
BOOL CPed::DoesHandsUp() {
    if(m_pGamePed&&!(*(DWORD*)((BYTE*)m_pGamePed+0x46c)&0x100)&&CEntity::DoesExist()&&GamePoolPedGetAt(m_nHandle)) {
        BYTE *manager=*(BYTE**)((BYTE*)m_pGamePed+0x47c);
        BYTE *task=*(BYTE**)(manager+0x10);
        // GTA SA virtual address: task vtable0x0085a29c.
        return task&&*(DWORD*)task==0x85a29c?TRUE:FALSE;
    }
    return FALSE;
}

// samp.dll RVA: 0x000ac980
void CPed::DisableJetpack() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(ped&&!(*(DWORD*)(ped+0x46c)&0x100)) {
        BYTE *manager=*(BYTE**)(ped+0x47c);
        void *task=*(void**)(manager+0x10);
        // GTA SA virtual addresses: vtable0x008705c4 and callee0x006801d0.
        if(task&&*(DWORD*)task==0x8705c4) {
            __asm { mov ecx,task
                mov edx,0x6801d0
                push 1
                call edx
            }
            manager=*(BYTE**)((BYTE*)m_pGamePed+0x47c);
            *(DWORD*)(manager+0x10)=0;
        }
    }
}
