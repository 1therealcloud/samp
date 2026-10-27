#include "game/playerped.h"
// samp.dll RVA: 0x00013570
void *CPed::GetGtaActor() { return m_pGamePed; }
#include "game/game.h"
#include "game/scripting.h"
#include "game/debug.h"
#include "game/animation.h"
#include "game/hooks_render.h"
#include "game/hooks_control.h"
#include "game/weapon_backend.h"
#include "game/task.h"
#include "game/keystuff.h"
#include "game/building_removal.h"
#include "net/localplayer.h"
#include "label.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gui/device_lifecycle.h"
#include <string.h>
// Linker orders these BSS subsections by suffix, preserving native adjacency.
#pragma bss_seg(".pstate$a")
// samp.dll RVA: 0x002a9a50
DWORD g_gamePlayerFlags[210];
#pragma bss_seg(".pstate$b")
// Shared raw raster snapshots (+14 and +0C), between flags and matrix caches.
// samp.dll RVA: 0x002a9d98
DWORD g_gamePlayerTableUnknown2A9D98[2];
#pragma bss_seg(".pstate$c")
// samp.dll RVA: 0x002a9da0
DWORD g_gameLocalPedMatrix;
#pragma bss_seg(".pstate$d")
// samp.dll RVA: 0x002a9da4
DWORD g_gameVehicleMatrix;
#pragma bss_seg(".pstate$e")
// samp.dll RVA: 0x002a9da8
void *g_gamePlayerPeds[210];
#pragma bss_seg(".pstate$f")
// samp.dll RVA: 0x002aa0f0
BYTE g_gamePlayerControllerState[210][16];
#pragma bss_seg()



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

#include "game/util.h"

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
#include "exceptions.h"
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


// samp.dll RVA: 0x000abb30
void CPed::ForceRotation(float rotation) {
    if(m_pGamePed&&GamePoolPedGetAt(m_nHandle)) {
        *(float*)((BYTE*)m_pGamePed+0x55c)=GameHeadingToRadians(rotation);
        *(float*)((BYTE*)m_pGamePed+0x558)=GameHeadingToRadians(rotation);
    }
}

// samp.dll RVA: 0x000abb80
void CPed::SetRotation(float rotation) {
    if(m_pGamePed&&GamePoolPedGetAt(m_nHandle)) {
        *(float*)((BYTE*)m_pGamePed+0x55c)=GameHeadingToRadians(rotation);
        *(float*)((BYTE*)m_pGamePed+0x558)=GameHeadingToRadians(rotation);
        Scripting::ProcessCommand(&g_pedRotationCommand,m_nHandle,(double)rotation);
    }
}

// samp.dll RVA: 0x000aea50
void CPed::DeleteAccessory(int index) {
    if(index<0||index>=10||m_accessoryOccupied[index]!=1) return;
    if(m_accessoryObjects[index]) {
        delete (CEntity*)m_accessoryObjects[index];
        m_accessoryObjects[index]=0;
    }
    memset(&m_accessoryInfo[index],0,sizeof(m_accessoryInfo[index]));
    m_accessoryOccupied[index]=0;
}

// samp.dll RVA: 0x000abdc0
__declspec(naked) float CPed::GetDistanceToEntity(CEntity *entity) {
    // Native uninitialized matrix outputs and extended x87 return are intentional.
    __asm { sub esp,0x80
        lea eax,[esp]
        push eax
        call CEntity::GetMatrix
        lea ecx,[esp+0x40]
        push ecx
        mov ecx,[esp+0x88]
        call CEntity::GetMatrix
        fld dword ptr [esp+0x38]
        fsub dword ptr [esp+0x78]
        fld dword ptr [esp+0x34]
        fsub dword ptr [esp+0x74]
        fld dword ptr [esp+0x30]
        fsub dword ptr [esp+0x70]
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fld st(3)
        fmul st(0),st(4)
        faddp st(1),st(0)
        fsqrt
        fstp st(3)
        fstp st(0)
        fstp st(0)
        add esp,0x80
        ret 4
    }
}

// samp.dll RVA: 0x000ad000
void CPed::DestroyParachute() {
    if(m_parachuteObject) {
        Scripting::ProcessCommand(&g_pedDetachParachute,m_parachuteObject,g_pedParachuteZero,g_pedParachuteZero,g_pedParachuteZero,0);
        Scripting::ProcessCommand(&g_pedParachuteCleanupCommand,m_parachuteObject);
        m_parachuteObject=0;
    }
}

// samp.dll RVA: 0x000af330
void CPed::CreateArrow(DWORD color) {
    if(m_arrow) {
        Scripting::ProcessCommand(&g_pedDeleteArrowCommand,m_arrow);
        m_arrow=0;
    }
    Scripting::ProcessCommand(&g_pedCreateArrowCommand,m_nHandle,&m_arrow);
    Scripting::ProcessCommand(&g_pedArrowColorCommand,m_arrow,color);
    Scripting::ProcessCommand(&g_pedArrowModeCommand,m_arrow,2);
}

// samp.dll RVA: 0x000abe20
int CPed::GetVehicleSeatIndex() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(*(ped+0x530)!=50) return -1;
    BYTE *vehicle=*(BYTE**)(ped+0x58c);
    if(!vehicle) return -1;
    for(int seat=0;seat<8;++seat)
        if(*(void**)(vehicle+0x460+seat*4)==ped) return seat;
    return -1;
}


// samp.dll RVA: 0x000adde0
void CPed::StopUrinating() {
    if(m_pGamePed&&GamePoolPedGetAt(m_nHandle)&&m_bUrinating) {
        DWORD *effect=(DWORD*)unknown2bd;
        if(*effect) {
            Scripting::ProcessCommand(&g_pedStopUrinationEffectCommand,*effect);
            *effect=0;
        }
        CMatrix matrix;
        GetMatrix(&matrix);
        Teleport(matrix.position);
        m_bUrinating=0;
    }
}

// samp.dll RVA: 0x000ac100
void CPed::ExitVehicle() {
    if(!m_pGamePed||!GamePoolPedGetAt(m_nHandle)) return;
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!(*(DWORD*)(ped+0x46c)&0x100)) return;
    if(!GamePoolVehicleGetIndex(*(void**)(ped+0x58c))) return;
    DWORD reference=0;
    if(m_pGamePed) reference=GamePoolVehicleGetIndex(*(void**)((BYTE*)m_pGamePed+0x58c));
    BYTE *vehicle=(BYTE*)GamePoolVehicleGetAt(reference);
    if(!vehicle) return;
    WORD model=*(WORD*)(vehicle+0x22);
    if(model==570||model==538) return;
    Scripting::ProcessCommand(&g_pedExitVehicleCommand,m_nHandle,GetVehicleRef());
}

// samp.dll RVA: 0x000afb50
void CPed::SetModelIndex(int model) {
    void *ped=m_pGamePed;
    if(GamePoolPedGetAt(m_nHandle)) {
        if(!GameIsPedModel(model)) model=0;
        if(ped) {
            // GTA SA virtual address: persistent one-byte patch0x005a82c0.
            GameUnprotect((void*)0x5a82c0,1);
            *(BYTE*)0x5a82c0=0xc3;
            DestroyCommandTask();
            CEntity::SetModelIndex(model);
            // GTA SA virtual address: 0x004e68d0, ECX original backend+294, argument original backend.
            __asm { mov eax,ped
                lea ecx,[eax+0x294]
                push eax
                mov edx,0x4e68d0
                call edx
            }
        }
    }
}

// samp.dll RVA: 0x000ace50
void CPed::ApplyAnimation(const char *name,const char *library,float speed,DWORD loop,DWORD lockX,DWORD lockY,DWORD freeze,int time) {
    if(!m_pGamePed||!GamePoolPedGetAt(m_nHandle)) return;
    if(_stricmp(library,g_pedBlockedAnimationLibrary)==0) return;
    if(!g_pGame->IsAnimationLoaded(library)) {
        g_pGame->RequestAnimation(library);
        if(!g_pGame->IsAnimationLoaded(library)) {
            void (WINAPI *volatile sleep)(DWORD)=::Sleep;
            int attempts=0;
            do {
                sleep(1);
                if(++attempts==15) return;
            } while(!g_pGame->IsAnimationLoaded(library));
        }
    }
    Scripting::ProcessCommand(&g_pedApplyAnimationCommand,m_nHandle,name,library,(double)speed,loop,lockX,lockY,freeze,time);
}

// samp.dll RVA: 0x000acf20
BOOL CPed::IsPerformingAnimation(const char *name) {
    if(m_pGamePed) return Scripting::ProcessCommand(&g_pedIsPerformingAnimationCommand,m_nHandle,name)!=0;
    return FALSE;
}

// samp.dll RVA: 0x000ad6d0
void CPed::StartDancing(int style) {
    if(style<0||style>3) return;
    unknown2d2=1;
    *(DWORD*)unknown2d6=style;
    if(m_nPlayerNumber==0)
        ApplyAnimation(g_pedDanceNames[style],g_pedDanceLibraries[style],16.0f,1,0,0,0,-1);
}

// samp.dll RVA: 0x000abab0
__declspec(naked) float CPed::GetRotation() {
    __asm { sub esp,0x40
        push esi
        mov esi,ecx
        mov eax,[esi+0x2a4]
        test eax,eax
        jz no_rotation
        mov eax,[esi+0x44]
        push eax
        call GamePoolPedGetAt
        test eax,eax
        jnz rotation_matrix
    no_rotation:
        fld dword ptr [g_objectScaleLowerBound]
        pop esi
        add esp,0x40
        ret
    rotation_matrix:
        lea ecx,[esp+4]
        push ecx
        mov ecx,esi
        call CEntity::GetMatrix
        fld dword ptr [esp+0x14]
        fchs
        fld dword ptr [esp+0x18]
        fpatan
        fmul dword ptr [g_entityRadiansToDegrees]
        fcom dword ptr [g_objectScaleLowerBound]
        fnstsw ax
        test ah,5
        jp rotation_upper
        fadd dword ptr [g_anglePeriod]
        pop esi
        add esp,0x40
        ret
    rotation_upper:
        fcom dword ptr [g_anglePeriod]
        fnstsw ax
        test ah,1
        jnz rotation_done
        fsub dword ptr [g_anglePeriod]
    rotation_done:
        pop esi
        add esp,0x40
        ret
    }
}

// samp.dll RVA: 0x000ac6e0
__declspec(naked) void * CPed::GetCurrentWeaponInfo() {
    // Preserve the native uninitialized output slot on a null-backend path.
    __asm { mov eax,[ecx+0x2a4]
        sub esp,8
        test eax,eax
        jz weapon_info_return
        test eax,eax
        jz no_weapon_slot
        movzx ecx,byte ptr [eax+0x718]
        imul ecx,ecx,0x1c
        lea eax,[ecx+eax+0x5a0]
        jmp query_weapon_info
    no_weapon_slot:
        xor eax,eax
    query_weapon_info:
        mov edx,[eax]
        mov ecx,g_pGame
        push ebx
        push 1
        push edx
        call CGame::GetWeaponInfo
        mov [esp+8],eax
        mov ebx,[esp+8]
        lea eax,[ebx+0x24]
        mov [esp+4],eax
        pop ebx
    weapon_info_return:
        mov eax,[esp]
        add esp,8
        ret
    }
}

// samp.dll RVA: 0x000ad5c0
DWORD CPed::GetModelTable28() {
    if(!m_pGamePed) return 0xffffffff;
    return GameReadPedModelTable28(*(short*)((BYTE*)m_pGamePed+0x22));
}

// samp.dll RVA: 0x000ad5e0
DWORD CPed::GetModelTable24() {
    if(!m_pGamePed) return 0xffffffff;
    return GameReadPedModelTable24(*(short*)((BYTE*)m_pGamePed+0x22));
}

// samp.dll RVA: 0x000ad770
const char *CPed::GetAnimationForDance(int move) {
    if(unknown2d2) {
        int style=*(int*)unknown2d6;
        DWORD offset=(DWORD)move<<4;
        if(style>=0&&style<=2) {
            if((m_pGamePed&&GameReadPedModelTable28(*(short*)((BYTE*)m_pGamePed+0x22))==5)||GetModelTable28()==22)
                return (const char*)((DWORD)g_pedDanceMoveTables+offset);
            return (const char*)((DWORD)g_pedDanceMoveTables+0x100+offset);
        }
        if(style==3) return (const char*)((DWORD)g_pedDanceMoveTables+0x200+offset);
    }
    return g_pedEmptyDanceAnimation;
}

// samp.dll RVA: 0x000acf60
void CPed::LinkToInterior(BYTE interior,BOOL refresh) {
    if(!m_pGamePed) return;
    if(m_nPlayerNumber==0) {
        Scripting::ProcessCommand(&g_pedSelectInteriorCommand,(DWORD)interior);
        Scripting::ProcessCommand(&g_pedLinkInteriorCommand,m_nHandle,(DWORD)interior);
        if(refresh) {
            CMatrix matrix;
            GetMatrix(&matrix);
            Scripting::ProcessCommand(&g_pedRefreshInteriorCommand,(double)matrix.position.x,(double)matrix.position.y);
        }
    } else Scripting::ProcessCommand(&g_pedLinkInteriorCommand,m_nHandle,(DWORD)interior);
}

// samp.dll RVA: 0x000ac3b0
void CPed::SetControllable(BOOL enabled) {
    if(!GamePoolPedGetAt(m_nHandle)) return;
    if(!enabled) {
        Scripting::ProcessCommand(&g_pedControllableCommand,(DWORD)m_nPlayerNumber,0);
        Scripting::ProcessCommand(&g_pedFreezeCommand,m_nHandle,1);
    } else {
        Scripting::ProcessCommand(&g_pedControllableCommand,(DWORD)m_nPlayerNumber,1);
        Scripting::ProcessCommand(&g_pedFreezeCommand,m_nHandle,0);
        if(m_pGamePed&&(*(DWORD*)((BYTE*)m_pGamePed+0x46c)&0x100)) return;
        CMatrix matrix;
        GetMatrix(&matrix);
        Teleport(matrix.position);
    }
}

// samp.dll RVA: 0x000acd10
void CPed::CallBackend730AndVehicle490() {
    if(m_pGamePed) {
        void *state=*(void**)((BYTE*)m_pGamePed+0x730);
        if(state) {
            // GTA SA virtual address: 0x005393f0, ECX raw state/no arguments.
            __asm { mov ecx,state
                mov eax,0x5393f0
                call eax
            }
        }
        BYTE *ped=(BYTE*)m_pGamePed;
        if(*(DWORD*)(ped+0x46c)&0x100) {
            BYTE *vehicle=*(BYTE**)(ped+0x58c);
            if(vehicle) {
                state=*(void**)(vehicle+0x490);
                if(state) {
                    __asm { mov ecx,state
                        mov eax,0x5393f0
                        call eax
                    }
                }
            }
        }
    }
}

// samp.dll RVA: 0x000afbd0
void CPed::Kill() {
    if(!m_nHandle||!m_pGamePed) return;
    if(HasJetpack()) DisableJetpack();
    CallBackend730AndVehicle490();
    CMatrix matrix;
    GetMatrix(&matrix);
    Teleport(matrix.position);
    *(DWORD*)((BYTE*)m_pGamePed+0x540)=0;
    *g_clientCrashPedContext=m_nPlayerNumber;
    Scripting::ProcessCommand(&g_pedKillCommand,m_nHandle);
    *g_clientCrashPedContext=0;
}

#include "game/entity.h"
// samp.dll RVA: 0x0011f258
const float g_pedFloorRayLength=1.75f;

// samp.dll RVA: 0x000ac630
__declspec(naked) void *CPed::GetFloor() {
    // Preserve native float spills, x87 stack lifetime and unchecked query output.
    __asm { sub esp,0x50
        push esi
        mov esi,ecx
        mov eax,[esi+0x2a4]
        test eax,eax
        jz floor_rejected
        mov ecx,[eax+0x46c]
        test ch,1
        jnz floor_rejected
        mov eax,[esi+0x44]
        push eax
        call GamePoolPedGetAt
        test eax,eax
        jz floor_rejected
        mov eax,[esi+0x2a4]
        mov ecx,[eax+0x14]
        fld dword ptr [ecx+0x30]
        push 0
        fst dword ptr [esp+0xc]
        mov edx,[eax+0x14]
        fld dword ptr [edx+0x34]
        push 0
        fst dword ptr [esp+0x14]
        mov eax,[eax+0x14]
        fld dword ptr [eax+0x38]
        push 0
        fsub dword ptr [g_vehicleCreationLift]
        push 0
        push 1
        push 0
        fstp dword ptr [esp+0x28]
        push 1
        fxch st(1)
        push 0
        fstp dword ptr [esp+0x34]
        lea ecx,[esp+0x24]
        push ecx
        fstp dword ptr [esp+0x3c]
        lea edx,[esp+0x44]
        fld dword ptr [esp+0x34]
        push edx
        fsub dword ptr [g_pedFloorRayLength]
        lea eax,[esp+0x3c]
        push eax
        lea ecx,[esp+0x34]
        push ecx
        fstp dword ptr [esp+0x4c]
        call GameProcessLineOfSightRaw
        mov eax,[esp+0x34]
        add esp,0x30
        pop esi
        add esp,0x50
        ret
    floor_rejected:
        xor eax,eax
        pop esi
        add esp,0x50
        ret
    }
}

// samp.dll RVA: 0x0018f4a4
DWORD g_pedVehicleEntryFlag;

// samp.dll RVA: 0x000ac030
void CPed::EnterVehicle(int vehicle,BOOL passenger) {
    if(!m_pGamePed) return;
    BYTE *backend=(BYTE*)GamePoolVehicleGetAt(vehicle);
    if(!backend||!GamePoolPedGetAt(m_nHandle)) return;
    g_pedVehicleEntryFlag=1;
    if(GetCurrentWeapon()==46) SetArmedWeapon(0,0);
    if(passenger) {
        if(*(WORD*)(backend+0x22)==570&&m_pGamePed==GameFindLocalPed())
            Scripting::ProcessCommand(&g_pedWarpPassengerCommand,m_nHandle,vehicle,-1);
        else Scripting::ProcessCommand(&g_pedEnterPassengerCommand,m_nHandle,vehicle,3000,-1);
    } else Scripting::ProcessCommand(&g_pedEnterDriverCommand,m_nHandle,vehicle,3000);
}

// samp.dll RVA: 0x000add00
void CPed::StartUrinating() {
    if(!m_pGamePed||!GamePoolPedGetAt(m_nHandle)||m_bUrinating) return;
    if(m_nPlayerNumber==0) ApplyAnimation(g_pedUrinationAnimation,g_pedUrinationLibrary,4.0f,1,0,0,0,-1);
    Scripting::ProcessCommand(&g_pedCreateUrinationEffectCommand,g_pedUrinationEffect,m_nHandle,
        g_pedParachuteZero,g_pedUrinationOffsetY,g_pedUrinationOffsetZ,
        g_pedParachuteZero,g_pedUrinationDirectionY,g_pedParachuteZero,1,(DWORD*)unknown2bd);
    Scripting::ProcessCommand(&g_pedStartUrinationEffectCommand,*(DWORD*)unknown2bd);
    m_bUrinating=1;
}

#include "net/vehiclepool.h"
// samp.dll RVA: 0x000abeb0
void CPed::PutIntoVehicle(int vehicle,int seat) {
    if(!m_pGamePed||!GamePoolVehicleGetAt(vehicle)||!GamePoolPedGetAt(m_nHandle)) return;
    if(GetCurrentWeapon()==46) SetArmedWeapon(0,0);
    BYTE *backend=(BYTE*)GamePoolVehicleGetAt(vehicle);
    DWORD reject;
    __asm { fld dword ptr [g_objectScaleLowerBound]
        mov eax,backend
        fld dword ptr [eax+0x4c0]
        fucompp
        fnstsw ax
        test ah,0x44
        setnp al
        movzx eax,al
        mov reject,eax
    }
    // GTA SA virtual address: excluded vehicle vtable0x00863c40.
    if(reject||*(DWORD*)backend==0x863c40) return;
    if((GameGetVehicleSubtype(backend)==1||GameGetVehicleSubtype(backend)==2)&&seat>(int)*(backend+0x488)) return;
    if(seat==0) {
        BYTE *driver=*(BYTE**)(backend+0x460);
        if(driver&&(*(DWORD*)(driver+0x46c)&0x100)) return;
        Scripting::ProcessCommand(&g_pedWarpDriverCommand,m_nHandle,vehicle);
    } else Scripting::ProcessCommand(&g_pedWarpPassengerCommand,m_nHandle,vehicle,(DWORD)seat-1u);
    void *ped=m_pGamePed;
    if(ped==GameFindLocalPed()&&(*(DWORD*)((BYTE*)ped+0x46c)&0x100)) g_pGame->m_pCamera->Restore();
    if(g_pNetGame) {
        BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);
        CVehiclePool *pool=*(CVehiclePool**)(pools+0xc);
        WORD id=pool->Find(backend);
        if(id==0xffff||id>2000) return;
        CVehicle *wrapper=pool->Get(id);
        if(wrapper&&wrapper->IsTrainPart()&&m_pGamePed==GameFindLocalPed())
            Scripting::ProcessCommand(&g_pedTrainCameraCommand,wrapper->m_nHandle,3,2);
    }
}

// samp.dll RVA: 0x000ad8a0
BOOL CPed::ApplyStuff() {
    if(!m_stuffType||!m_pGamePed||!CEntity::DoesExist()) return FALSE;
    KeyPad *keys=m_nPlayerNumber?KeyStuff::GetPlayerKeys(m_nPlayerNumber):KeyStuff::GetInternalKeys();
    SetArmedWeapon(0,0);
    if(m_nPlayerNumber||PerformingCustomAnimation()||!keys->keys1[17]) return FALSE;
    DWORD type=m_stuffType;
    if(type==4) {
        ApplyAnimation(g_pedSmokeAnimation,g_pedSmokeLibrary,700.0f,0,0,0,0,2750);
        return TRUE;
    }
    if(type==1||type==2||type==3) {
        if(GetModelTable28()==5||GetModelTable28()==22)
            ApplyAnimation(g_pedDrinkAnimationF,g_pedDrinkLibrary,4.0f,0,0,0,0,-1);
        else ApplyAnimation(g_pedDrinkAnimationM,g_pedDrinkLibrary,4.0f,0,0,0,0,-1);
        // Native reload after ApplyAnimation, modular add followed by signed upper bound.
        if(m_stuffType!=3) {
            DWORD level=m_drunkLevel+1250u;
            m_drunkLevel=level;
            if((int)level>50000) m_drunkLevel=50000;
        }
        return TRUE;
    }
    return FALSE;
}

// samp.dll RVA: 0x000af1d0
WORD CPed::GetKeys(WORD *leftRight,WORD *upDown) {
    KeyPad *keys=KeyStuff::GetInternalKeys();
    *leftRight=keys->keys1[0];
    *upDown=keys->keys1[1];
    short horizontal=(short)keys->keys1[2];
    short vertical=(short)keys->keys1[3];
    DWORD bits=0;
    if(horizontal>0) bits|=0x4000;
    if(horizontal<0) bits|=0x2000;
    if(vertical>0) bits|=0x1000;
    if(vertical<0) bits|=0x0800;
    if(keys->keys1[21]) bits|=0x0400;
    if(keys->keys1[19]) bits|=0x0200;
    BYTE *backend=(BYTE*)m_pGamePed;
    if(backend&&(*(volatile DWORD*)(backend+0x46c)&0x100)&&keys->keys1[5]) bits|=0x0100;
    if(keys->keys1[6]) bits|=0x0080;
    if(backend&&(*(volatile DWORD*)(backend+0x46c)&0x100)&&keys->keys1[7]) bits|=0x0040;
    if(keys->keys1[14]) bits|=0x0020;
    BOOL suppress=FALSE;
    if(backend&&!(*(volatile DWORD*)(backend+0x46c)&0x100)) {
        BYTE *manager=*(BYTE**)(backend+0x47c);
        BYTE *task=*(BYTE**)(manager+0x10);
        // GTA SA virtual address: native jetpack task vtable0x008705c4.
        suppress=task&&*(DWORD*)task==0x8705c4;
    }
    if(!suppress&&keys->keys1[15]) bits|=0x0010;
    if(keys->keys1[16]) bits|=0x0008;
    BOOL fire=TRUE;
    if(backend) {
        // Native reload is separate from the earlier cached nonnull gate.
        BYTE *current=(BYTE*)*(void *volatile*)&m_pGamePed;
        BYTE *slot=(BYTE*)((DWORD)current+0x5a0+*(current+0x718)*0x1c);
        if(slot) {
            DWORD weapon=*(DWORD*)slot;
            if(weapon>15&&weapon!=46&&!*(DWORD*)(slot+0x0c)) fire=FALSE;
        }
    }
    if(fire&&keys->keys1[17]) bits|=0x0004;
    if(keys->keys1[18]) bits|=0x0002;
    if(keys->keys1[4]) bits|=0x0001;
    return (WORD)bits;
}

// samp.dll RVA: 0x000aef40
void CPed::SetKeys(WORD mask,WORD leftRight,WORD upDown) {
    KeyPad *keys=KeyStuff::GetPlayerKeys(m_nPlayerNumber);
    BOOL held=keys->keys1[0]!=0;
    keys->keys1[0]=leftRight;
    keys->keys2[0]=held&&leftRight?leftRight:0;
    held=keys->keys1[1]!=0;
    keys->keys1[1]=upDown;
    keys->keys2[1]=held&&upDown?upDown:0;
    if((mask&0x0001)) {
        WORD previous=keys->keys1[4];
        keys->keys1[4]=255;
        keys->keys2[4]=previous!=0?255:0;
    } else {
        keys->keys1[4]=0;
        keys->keys2[4]=0;
    }
    BYTE *backend=(BYTE*)*(void *volatile*)&m_pGamePed;
    if(backend&&(*(volatile DWORD*)(backend+0x46c)&0x100)&&(mask&0x0002)) {
        WORD previous=keys->keys1[18];
        keys->keys1[18]=255;
        keys->keys2[18]=previous!=0?255:0;
    } else {
        keys->keys1[18]=0;
        keys->keys2[18]=0;
    }
    if((mask&0x0004)) {
        WORD previous=keys->keys1[17];
        keys->keys1[17]=255;
        keys->keys2[17]=previous!=0?255:0;
    } else {
        keys->keys1[17]=0;
        keys->keys2[17]=0;
    }
    if((mask&0x0008)) {
        WORD previous=keys->keys1[16];
        keys->keys1[16]=255;
        keys->keys2[16]=previous!=0?255:0;
    } else {
        keys->keys1[16]=0;
        keys->keys2[16]=0;
    }
    BOOL down=(mask&0x0010)!=0;
    if(down) {
        BYTE *current=(BYTE*)*(void *volatile*)&m_pGamePed;
        if(current&&!(*(volatile DWORD*)(current+0x46c)&0x100)) {
            BYTE *manager=*(BYTE**)(current+0x47c);
            BYTE *task=*(BYTE**)(manager+0x10);
            // GTA SA virtual address: jetpack task vtable0x008705c4.
            if(task&&*(DWORD*)task==0x8705c4) down=FALSE;
        }
    }
    if(down) {
        WORD previous=keys->keys1[15];
        keys->keys1[15]=255;
        keys->keys2[15]=previous!=0?255:0;
    } else {
        keys->keys1[15]=0;
        keys->keys2[15]=0;
    }
    if((mask&0x0020)) {
        WORD previous=keys->keys1[14];
        keys->keys1[14]=255;
        keys->keys2[14]=previous!=0?255:0;
    } else {
        keys->keys1[14]=0;
        keys->keys2[14]=0;
    }
    if((mask&0x0040)) {
        WORD previous=keys->keys1[7];
        keys->keys1[7]=255;
        keys->keys2[7]=previous!=0?255:0;
    } else {
        keys->keys1[7]=0;
        keys->keys2[7]=0;
    }
    if((mask&0x0080)) {
        WORD previous=keys->keys1[6];
        keys->keys1[6]=255;
        keys->keys2[6]=previous!=0?255:0;
    } else {
        keys->keys1[6]=0;
        keys->keys2[6]=0;
    }
    if((mask&0x0100)) {
        WORD previous=keys->keys1[5];
        keys->keys1[5]=255;
        keys->keys2[5]=previous!=0?255:0;
    } else {
        keys->keys1[5]=0;
        keys->keys2[5]=0;
    }
    if((mask&0x0200)) {
        WORD previous=keys->keys1[19];
        keys->keys1[19]=255;
        keys->keys2[19]=previous==255?255:0;
    } else {
        keys->keys1[19]=0;
        keys->keys2[19]=0;
    }
    if((mask&0x0400)) {
        WORD previous=keys->keys1[21];
        keys->keys1[21]=255;
        keys->keys2[21]=previous==255?255:0;
    } else {
        keys->keys1[21]=0;
        keys->keys2[21]=0;
    }
    WORD direction=(mask>>11)&3;
    WORD axis=direction==2?0x0080:direction==1?0xff80:0;
    keys->keys1[3]=axis;
    keys->keys2[3]=axis;
    direction=(mask>>13)&3;
    axis=direction==2?0x0080:direction==1?0xff80:0;
    keys->keys1[2]=axis;
    keys->keys2[2]=axis;
    KeyStuff::SetKeys(m_nPlayerNumber,keys);
}

#include <stdlib.h>
extern "C" {
// MSVC CRT x87 conversion helper, provided by selected runtime.
// samp.dll RVA: 0x000f9324
__int64 __cdecl _ftol2();
}
// samp.dll RVA: 0x002a92a4
DWORD g_pedDrunkVehicleTick;
// samp.dll RVA: 0x001181c0
const float g_pedDrunkShakeScale=0.02f;
// samp.dll RVA: 0x00117668
const float g_pedDrunkSpeedThreshold=0.05f;

// samp.dll RVA: 0x000ad9f0
void CPed::ProcessDrunk() {
    int level=(int)m_drunkLevel;
    DWORD deltaBits=0;
    if(level>2000) {
        if(m_nPlayerNumber) return;
        int shake;
        __asm { fild level
            fmul dword ptr [g_pedDrunkShakeScale]
            call _ftol2
            mov shake,eax
        }
        if(shake>250) shake=250;
        else if(shake<5) shake=0;
        Scripting::ProcessCommand(&g_pedDrunkShakeCommand,0,shake);
        if((int)m_drunkLevel>2000) {
            BYTE *ped=(BYTE*)m_pGamePed;
            if(ped&&(*(DWORD*)(ped+0x46c)&0x100)&&!IsPassenger()) {
                BYTE *vehicle=*(BYTE**)(ped+0x58c);
                if(vehicle) {
                    DWORD previous=g_pedDrunkVehicleTick;
                    DWORD (WINAPI *volatile tick)()=::GetTickCount;
                    BOOL process=TRUE;
                    if(previous) {
                        DWORD now=tick();
                        process=now-g_pedDrunkVehicleTick>200u;
                    }
                    if(process) {
                        int outcome=rand()%40;
                        if(outcome>=20) {
                            if(outcome>30) deltaBits=(int)m_drunkLevel<5000?0x3c449ba6:0x3c75c28f;
                            else deltaBits=(int)m_drunkLevel<5000?0xbc449ba6:0xbc75c28f;
                        }
                        __asm { mov edx,vehicle
                            push 0
                            push dword ptr [edx+0x44]
                            call FloatOffset
                            fcomp dword ptr [g_pedDrunkSpeedThreshold]
                            add esp,8
                            fnstsw ax
                            test ah,0x41
                            jz drunk_apply_delta
                            mov edx,vehicle
                            push 0
                            push dword ptr [edx+0x48]
                            call FloatOffset
                            fcomp dword ptr [g_pedDrunkSpeedThreshold]
                            add esp,8
                            fnstsw ax
                            test ah,0x41
                            jnz drunk_delta_done
                        drunk_apply_delta:
                            mov edx,vehicle
                            fld dword ptr deltaBits
                            fadd dword ptr [edx+0x58]
                            fstp dword ptr [edx+0x58]
                        drunk_delta_done:
                        }
                        g_pedDrunkVehicleTick=tick();
                    }
                }
            }
        }
        --m_drunkLevel;
        return;
    }
    if(!m_nPlayerNumber) {
        if(level>0) m_drunkLevel=(DWORD)(level-1);
        Scripting::ProcessCommand(&g_pedDrunkShakeCommand,0,0);
    }
}

// samp.dll RVA: 0x0011f15c
const Scripting::OpcodeInfo g_pedDetachParachute={0x682,"ifffi"};
// samp.dll RVA: 0x0011f148
const Scripting::OpcodeInfo g_pedDestroyParachute={0x9a2,"i"};
// samp.dll RVA: 0x0011f238
const Scripting::OpcodeInfo g_pedDropStuff={0x70a,"iifffiissi"};
// samp.dll RVA: 0x0011efcc
const Scripting::OpcodeInfo g_pedWarpFromVehicle={0x362,"ifff"};
// samp.dll RVA: 0x0011f320
const char g_pedNullAnimation[5]="NULL";
// samp.dll RVA: 0x000b3770
void *__stdcall GamePoolPedGetAt(int handle) {
    // GTA SA virtual pool-pointer and thiscall backend addresses.
    return ((void*(__fastcall*)(void*,void*,int))0x404910)(*(void**)0xb74490,0,handle);
}
// samp.dll RVA: 0x000aea30
BOOL CPed::HasAccessory() {
    for(int i=0;i<10;++i)if(m_accessoryOccupied[i]==1)return TRUE;
    return FALSE;
}
// samp.dll RVA: 0x000b06b0
void CPed::DeleteAllAccessories() {
    for(int i=0;i<10;++i)if(m_accessoryOccupied[i]==1) {
        void *object=m_accessoryObjects[i];
        if(object) {
            // Observed scalar-deleting virtual entry; no complete CObject ABI assumed.
            ((void(__fastcall*)(void*,void*,int))(*(DWORD**)object)[0])(object,0,1);
            m_accessoryObjects[i]=0;
        }
        memset(&m_accessoryInfo[i],0,52);
        m_accessoryOccupied[i]=FALSE;
    }
}
// samp.dll RVA: 0x000ad800
void CPed::DropStuff() {
    if(m_stuffObject) {
        Scripting::ProcessCommand(&g_pedDropStuff,m_nHandle,m_stuffObject,
            0.0,0.0,0.0,6,16,g_pedNullAnimation,g_pedNullAnimation,0);
        m_stuffObject=0;
    }
    CMatrix matrix;GetMatrix(&matrix);
    Teleport(matrix.position);
    m_stuffType=0;
}
// samp.dll RVA: 0x000ac190
void CPed::WarpFromVehicle(float x,float y,float z) {
    if(GamePoolPedGetAt(m_nHandle) && m_pGamePed && (*(DWORD*)((BYTE*)m_pGamePed+0x46c)&0x100))
        Scripting::ProcessCommand(&g_pedWarpFromVehicle,m_nHandle,(double)x,(double)y,(double)z);
}
// samp.dll RVA: 0x000b0ba0
void CPed::Destroy() {
    BYTE *original=(BYTE*)m_pGamePed;
    KeyStuff::ResetKeys(m_nPlayerNumber);
    GameAssignPlayerPed(m_nPlayerNumber,0);
    if(m_pGamePed && GamePoolPedGetAt(m_nHandle) && *(DWORD*)m_pGamePed!=0x863c40) {
        if(m_parachuteObject) {
            Scripting::ProcessCommand(&g_pedDetachParachute,m_parachuteObject,0.0,0.0,0.0,0);
            Scripting::ProcessCommand(&g_pedDestroyParachute,m_parachuteObject);
            m_parachuteObject=0;
        }
        if(HasAccessory())DeleteAllAccessories();
        if(m_stuffType)DropStuff();
        if(*(DWORD*)((BYTE*)m_pGamePed+0x46c)&0x100)WarpFromVehicle(100.0f,100.0f,10.0f);
        *(DWORD*)(*(BYTE**)(original+0x480)+0x4c)=0;
        ((void(__fastcall*)(void*,void*,int))(*(DWORD**)original)[0])(original,0,1);
        m_pGamePed=0;m_pGameEntity=0;
        // Native successful deletion deliberately retains m_nHandle.
        return;
    }
    m_pGamePed=0;m_pGameEntity=0;m_nHandle=0;
}

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

#include "game/ped_control.h"
#include "chatwindow.h"
// samp.dll RVA: 0x0018e8c0
DWORD g_hookPedControlField540;
// samp.dll RVA: 0x0018f378
CPlayerPool *g_hookFaultPlayerPool;
// samp.dll RVA: 0x00151970
WORD g_hookFaultPlayerId=0xffff;
// samp.dll RVA: 0x0018e824
CRemotePlayer *g_hookFaultRemotePlayer;
// samp.dll RVA: 0x00151a70
const BYTE g_hookPedRestoreA[6]={0xd9,0x96,0x5c,0x05,0,0};
// samp.dll RVA: 0x00151a88
const BYTE g_hookPedRestoreB[6]={0xd9,0x96,0x5c,0x05,0,0};
// samp.dll RVA: 0x00151a90
const BYTE g_hookPedRemoteRestore[5]={0xe8,0x17,0x88,0xff,0xff};
// samp.dll RVA: 0x0011e7b8
const Scripting::OpcodeInfo g_gameCurrentInteriorCommand={0x77e,"v"};
// samp.dll RVA: 0x0011ebcc
const char g_hookPedExceptionLabel[10]="playerped";
// samp.dll RVA: 0x0011ebac
const char g_hookPedRemovedMessage[32]="Removed player %u due to error.";
// samp.dll RVA: 0x000a0d00
BYTE CGame::GetCurrentInterior() {
    DWORD interior;Scripting::ProcessCommand(&g_gameCurrentInteriorCommand,&interior);
    return (BYTE)interior;
}
// samp.dll RVA: 0x000a2530
void GameRunPedControlBackend() {
    __try {
        // GTA SA thiscall backend, ECX=the current saved ped.
        ((void(__fastcall*)(void*,void*))0x60ea90)((void*)g_hookCameraPed,0);
    } __except(GameExceptionFilter(GetExceptionCode(),GetExceptionInformation(),g_hookPedExceptionLabel)) {
        if(*g_clientCrashPedContext && g_pNetGame) {
            g_hookFaultPlayerPool=*(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+8);
            if(g_hookFaultPlayerPool) {
                WORD id=g_hookFaultPlayerPool->Find((void*)g_hookCameraPed);
                g_hookFaultPlayerId=id;
                if(id!=0xffff) {
                    g_hookFaultRemotePlayer=g_hookFaultPlayerPool->GetPlayer(id);
                    if(g_hookFaultRemotePlayer) {
                        g_hookFaultRemotePlayer->Remove();id=g_hookFaultPlayerId;
                    }
                    if(g_pChat)g_pChat->AddDebugMessage(g_hookPedRemovedMessage,(unsigned)id);
                }
            }
        }
    }
}
// Extracted readable body; outer native entry preserves PUSHAD state.
// samp.dll RVA: unknown (reconstruction-only body helper extracted from A26D0; no standalone native entry)
void GameProcessPedControlBody() {
    g_hookCollisionPed=g_hookCameraPed;
    g_hookSavedPlayer=*g_clientCrashPedContext;
    g_hookPlayerIndex=GameFindPlayerPedIndex((void*)g_hookCameraPed);
    if(g_hookCameraPed && g_hookPlayerIndex && !g_hookSavedPlayer) {
        KeyStuff::UpdateKeys();KeyStuff::ApplyPlayerKeys(g_hookPlayerIndex);
        g_hookSavedCameraMode=*g_aimInternalCameraMode;
        *g_aimInternalCameraMode=AimStuff::GetPlayerCameraMode(g_hookPlayerIndex);
        g_hookSavedCameraModeWord=*g_hookCameraModeWord;
        *g_hookCameraModeWord=AimStuff::GetPlayerCameraMode(g_hookPlayerIndex);
        if(*g_hookCameraModeWord==4)*g_hookCameraModeWord=0;
        AimStuff::UpdateCamera();AimStuff::ApplyPlayerCamera(g_hookPlayerIndex);
        AimStuff::UpdateAim();AimStuff::ApplyPlayerAim(g_hookPlayerIndex);
        AimStuff::UpdateLocalSkills();AimStuff::ApplyPlayerSkills(g_hookPlayerIndex);
        *g_clientCrashPedContext=g_hookPlayerIndex;
        g_hookPedControlField540=*(DWORD*)(g_hookCollisionPed+0x540);
        GameUnprotect((void*)0x5e92f4,5);
        *(DWORD*)0x5e92f4=0x90909090;*(BYTE*)0x5e92f8=0x90;
        GameRunPedControlBackend();
        memcpy((void*)0x5e92f4,g_hookPedRemoteRestore,5);
        AimStuff::ApplyLocalSkills();
        *g_aimInternalCameraMode=g_hookSavedCameraMode;
        *g_hookCameraModeWord=g_hookSavedCameraModeWord;
        AimStuff::ApplyLocalCamera();*g_clientCrashPedContext=0;
        KeyStuff::ApplyLocalKeys();AimStuff::ApplyLocalAim();
    } else {
        if(g_pNetGame && *(DWORD*)(*(BYTE**)(*(BYTE**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+8)+0x1e)+0xf4)) {
            BYTE *settings=*(BYTE**)((BYTE*)g_pNetGame+0x3d5);
            float positiveX=*(float*)(settings+5),negativeX=*(float*)(settings+9);
            float positiveY=*(float*)(settings+0xd),negativeY=*(float*)(settings+0x11);
            if(g_pGame->GetPlayerPed()->HasExceededWorldBoundries(positiveX,negativeX,positiveY,negativeY) &&
                !g_pGame->GetCurrentInterior() && !(*(DWORD*)(g_hookCollisionPed+0x46c)&0x100) &&
                (*(DWORD*)(g_hookCollisionPed+0x46c)&3)) {
                g_hookVehicleKeys=(DWORD)KeyStuff::GetInternalKeys();
                *(WORD*)(g_hookVehicleKeys+0x1c)=0xff;*(WORD*)(g_hookVehicleKeys+0x4c)=0;
            }
        }
        if(g_pGame->GetPlayerPed()) {
            if(g_pGame->GetPlayerPed()->unknown2f7) {
                g_hookVehicleKeys=(DWORD)KeyStuff::GetInternalKeys();
                *(WORD*)(g_hookVehicleKeys+0x24)=0;*(WORD*)(g_hookVehicleKeys+0x54)=0;
            }
            if(g_pGame->GetPlayerPed()->unknown2f6) {
                g_hookVehicleKeys=(DWORD)KeyStuff::GetInternalKeys();
                *(WORD*)(g_hookVehicleKeys+0x1c)=0;*(WORD*)(g_hookVehicleKeys+0x4c)=0;
            }
            if(g_pGame->GetPlayerPed()->unknown2f8) {
                g_hookVehicleKeys=(DWORD)KeyStuff::GetInternalKeys();
                *(WORD*)(g_hookVehicleKeys+0x20)=0;*(WORD*)(g_hookVehicleKeys+0x50)=0;
            }
        }
        memcpy((void*)0x6884c4,g_hookPedRestoreA,6);memcpy((void*)0x688200,g_hookPedRestoreB,6);
        ((void(__fastcall*)(void*,void*))0x60ea90)((void*)g_hookCameraPed,0);
        memset((void*)0x6884c4,0x90,6);memset((void*)0x688200,0x90,6);
    }
}
// Raw entry: ECX=GTA ped; original preserves all PUSHAD registers and returns.
// samp.dll RVA: 0x000a26d0
__declspec(naked) void GameHook_000a26d0() {
    __asm {
        mov g_hookCameraPed,ecx
        pushad
        call GameProcessPedControlBody
        popad
        ret
    }
}

// samp.dll RVA: 0x0011f0d0
const Scripting::OpcodeInfo g_pedCommand087e={0x87e,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011ef90
const Scripting::OpcodeInfo g_pedCommand0446={0x446,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
// samp.dll RVA: 0x0011e604
const Scripting::OpcodeInfo g_entityTeleportCommand={0x7c7,{0x69,0x66,0x66,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f1c0
const Scripting::OpcodeInfo g_pedFightingStyleCommand={0x7fe,{0x69,0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f198
const Scripting::OpcodeInfo g_pedCellphoneCommand={0x729,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011eef0
const Scripting::OpcodeInfo g_pedDeleteArrowCommand={0x164,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011ef18
const Scripting::OpcodeInfo g_pedSpawnInfoCommand={0x16c,{0x66,0x66,0x66,0x66,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f1ac
const Scripting::OpcodeInfo g_pedHandsUpCommand={0x5c4,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011ef7c
const Scripting::OpcodeInfo g_pedImmunitiesCommand={0x2ab,{0x69,0x69,0x69,0x69,0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f01c
const Scripting::OpcodeInfo g_pedRotationCommand={0x173,{0x69,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f1d4
const Scripting::OpcodeInfo g_pedParachuteCleanupCommand={0x108,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011ee8c
const Scripting::OpcodeInfo g_pedCreateArrowCommand={0x187,{0x69,0x76,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011ef04
const Scripting::OpcodeInfo g_pedArrowColorCommand={0x165,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f184
const Scripting::OpcodeInfo g_pedArrowModeCommand={0x18b,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011ae68
const double g_pedParachuteZero=0.0;

// samp.dll RVA: 0x0011f1e8
const Scripting::OpcodeInfo g_pedStopUrinationEffectCommand={0x650,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f008
const Scripting::OpcodeInfo g_pedExitVehicleCommand={0x5cd,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f0a8
const Scripting::OpcodeInfo g_pedApplyAnimationCommand={0x812,{0x69,0x73,0x73,0x66,0x69,0x69,0x69,0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f0bc
const Scripting::OpcodeInfo g_pedIsPerformingAnimationCommand={0x611,{0x69,0x73,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011e324
const char g_pedBlockedAnimationLibrary[4]={0x53,0x45,0x58,0x0};

// samp.dll RVA: 0x00154db0
char g_pedDanceLibraries[4][16]={0x57,0x4f,0x50,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x47,0x46,0x55,0x4e,0x4b,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x52,0x55,0x4e,0x4e,0x49,0x4e,0x47,0x4d,0x41,0x4e,0x0,0x0,0x0,0x0,0x0,0x0,0x53,0x54,0x52,0x49,0x50,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

// samp.dll RVA: 0x00154df0
char g_pedDanceNames[4][16]={0x44,0x41,0x4e,0x43,0x45,0x5f,0x4c,0x4f,0x4f,0x50,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x4c,0x4f,0x4f,0x50,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x4c,0x4f,0x4f,0x50,0x0,0x0,0x0,0x0,0x0,0x0,0x53,0x54,0x52,0x5f,0x4c,0x6f,0x6f,0x70,0x5f,0x42,0x0,0x0,0x0,0x0,0x0,0x0};

// samp.dll RVA: 0x00154e30
char g_pedDanceMoveTables[3][16][16]={0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x31,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x32,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x33,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x34,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x35,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x36,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x37,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x38,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x39,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x31,0x30,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x31,0x31,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x31,0x32,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x31,0x33,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x31,0x34,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x31,0x35,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x47,0x31,0x36,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x31,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x32,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x33,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x34,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x35,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x36,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x37,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x38,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x39,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x31,0x30,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x31,0x31,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x31,0x32,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x31,0x33,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x31,0x34,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x31,0x35,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x44,0x41,0x4e,0x43,0x45,0x5f,0x42,0x31,0x36,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x73,0x74,0x72,0x69,0x70,0x5f,0x41,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x73,0x74,0x72,0x69,0x70,0x5f,0x42,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x73,0x74,0x72,0x69,0x70,0x5f,0x43,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x73,0x74,0x72,0x69,0x70,0x5f,0x44,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x73,0x74,0x72,0x69,0x70,0x5f,0x45,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x73,0x74,0x72,0x69,0x70,0x5f,0x46,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x73,0x74,0x72,0x69,0x70,0x5f,0x47,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x53,0x54,0x52,0x5f,0x41,0x32,0x42,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x53,0x54,0x52,0x5f,0x42,0x32,0x41,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x53,0x54,0x52,0x5f,0x42,0x32,0x43,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x53,0x54,0x52,0x5f,0x43,0x31,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x53,0x54,0x52,0x5f,0x43,0x32,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x53,0x54,0x52,0x5f,0x43,0x32,0x42,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x53,0x54,0x52,0x5f,0x41,0x32,0x42,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x53,0x54,0x52,0x5f,0x42,0x32,0x43,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x53,0x54,0x52,0x5f,0x43,0x32,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

// samp.dll RVA: 0x00117a39
const char g_pedEmptyDanceAnimation[1]={0x0};

// samp.dll RVA: 0x0011eec8
const Scripting::OpcodeInfo g_pedSelectInteriorCommand={0x4bb,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f0f8
const Scripting::OpcodeInfo g_pedLinkInteriorCommand={0x860,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f058
const Scripting::OpcodeInfo g_pedRefreshInteriorCommand={0x4e4,{0x66,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011ef2c
const Scripting::OpcodeInfo g_pedControllableCommand={0x1b4,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f044
const Scripting::OpcodeInfo g_pedFreezeCommand={0x4d7,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f080
const Scripting::OpcodeInfo g_pedKillCommand={0x321,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f06c
const Scripting::OpcodeInfo g_pedWarpPassengerCommand={0x430,{0x69,0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011eff4
const Scripting::OpcodeInfo g_pedEnterPassengerCommand={0x5ca,{0x69,0x69,0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011efe0
const Scripting::OpcodeInfo g_pedEnterDriverCommand={0x5cb,{0x69,0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f1fc
const Scripting::OpcodeInfo g_pedCreateUrinationEffectCommand={0x66a,{0x73,0x69,0x66,0x66,0x66,0x66,0x66,0x66,0x69,0x76,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011f210
const Scripting::OpcodeInfo g_pedStartUrinationEffectCommand={0x64c,{0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011eacc
const char g_pedUrinationLibrary[9]={0x50,0x41,0x55,0x4c,0x4e,0x4d,0x41,0x43,0x0};

// samp.dll RVA: 0x0011f378
const char g_pedUrinationAnimation[10]={0x50,0x49,0x53,0x53,0x5f,0x4c,0x4f,0x4f,0x50,0x0};

// samp.dll RVA: 0x0011f354
const char g_pedUrinationEffect[10]={0x50,0x45,0x54,0x52,0x4f,0x4c,0x43,0x41,0x4e,0x0};

// samp.dll RVA: 0x0011f360
const double g_pedUrinationOffsetY=0.57999998331069946;

// samp.dll RVA: 0x0011f368
const double g_pedUrinationOffsetZ=-0.079999998211860657;

// samp.dll RVA: 0x0011f370
const double g_pedUrinationDirectionY=0.0099999997764825821;

// samp.dll RVA: 0x0011ef68
const Scripting::OpcodeInfo g_pedWarpDriverCommand={0x36a,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011eedc
const Scripting::OpcodeInfo g_pedTrainCameraCommand={0x158,{0x69,0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x0011ead8
const char g_pedSmokeLibrary[6]={0x47,0x41,0x4e,0x47,0x53,0x0};

// samp.dll RVA: 0x0011f348
const char g_pedSmokeAnimation[12]={0x73,0x6d,0x6b,0x63,0x69,0x67,0x5f,0x70,0x72,0x74,0x6c,0x0};

// samp.dll RVA: 0x0011eae0
const char g_pedDrinkLibrary[4]={0x42,0x41,0x52,0x0};

// samp.dll RVA: 0x0011f338
const char g_pedDrinkAnimationM[15]={0x64,0x6e,0x6b,0x5f,0x73,0x74,0x6e,0x64,0x4d,0x5f,0x6c,0x6f,0x6f,0x70,0x0};

// samp.dll RVA: 0x0011f328
const char g_pedDrinkAnimationF[15]={0x64,0x6e,0x6b,0x5f,0x73,0x74,0x6e,0x64,0x46,0x5f,0x6c,0x6f,0x6f,0x70,0x0};

// samp.dll RVA: 0x0011f224
const Scripting::OpcodeInfo g_pedDrunkShakeCommand={0x52c,{0x69,0x69,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

// samp.dll RVA: 0x000adba0
DWORD CPed::GetDrunkLevel() { return m_drunkLevel; }

// samp.dll RVA: 0x000715d0
PedAccessoryState *CPed::GetAccessoryInfo(int index) {
    if(index<0||index>=10||!m_accessoryOccupied[index])return 0;
    return &m_accessoryInfo[index];
}
// samp.dll RVA: 0x000ae680
void CPed::GetBoneMatrixById(CMatrix *out,int bone) {
    BYTE *ped=(BYTE*)m_pGamePed;
    // GTA SA virtual address: excluded vtable 0x00863c40.
    if(!ped||*(DWORD*)ped==0x863c40)return;
    void *clump=*(void**)(ped+0x18);
    if(!clump)return;
    typedef BYTE *(__cdecl *GetHierarchy)(void*);
    typedef int (__cdecl *GetBoneIndex)(void*,int);
    // GTA SA virtual addresses: hierarchy lookup and US/EU bone-index lookup.
    GetBoneIndex getIndex=(GetBoneIndex)(g_gameVersion==1?0x7c51a0:0x7c51e0);
    BYTE *hierarchy=((GetHierarchy)0x734a40)(clump);
    int index=getIndex(hierarchy,bone);
    const DWORD *source=(const DWORD*)(*(BYTE**)(hierarchy+8)+(DWORD)index*64);
    // Native REP MOVSD copies forward, including overlapping output.
    volatile DWORD *destination=(volatile DWORD*)out;
    for(int i=0;i<16;i++)destination[i]=source[i];
}
// samp.dll RVA: 0x000ae810
void CPed::GetBoneMatrix(CMatrix *out,int bone) {
    BYTE *ped=(BYTE*)m_pGamePed;
    BYTE *entry=*(BYTE**)(ped+0x488+(DWORD)bone*4);
    GetBoneMatrixById(out,*(int*)(entry+0x14));
}

// samp.dll RVA: 0x000ab840
CameraAim *CPed::GetCameraAim() {return AimStuff::GetInternalAim();}

// samp.dll RVA: 0x000b3dc0
BOOL __stdcall GameIsBackendOnScreen(void *backend) {
    // GTA SA virtual address: thiscall backend, raw EAX normalized to BOOL.
    int result;
    __asm {
        mov ecx,backend
        mov edx,534540h
        call edx
        mov result,eax
    }
    return result!=0;
}
// samp.dll RVA: 0x000ab900
BOOL CPed::IsOnScreen() {return m_pGamePed?GameIsBackendOnScreen(m_pGamePed):FALSE;}
// samp.dll RVA: 0x000ad5a0
void *CPed::GetContactEntity() {return *(void**)((BYTE*)m_pGamePed+0x584);}
// samp.dll RVA: 0x000ad5b0
void *CPed::GetContactVehicle() {return *(void**)((BYTE*)m_pGamePed+0x568);}
// samp.dll RVA: 0x000141e0
void CPed::SetCameraMode(BYTE mode) {AimStuff::SetPlayerCameraMode(mode,m_nPlayerNumber);}
// samp.dll RVA: 0x00014200
void CPed::SetCameraExtZoomAndAspectRatio(float zoom,float aspect) {AimStuff::SetPlayerCamera(m_nPlayerNumber,zoom,aspect);}
// samp.dll RVA: 0x00002d00
BYTE CPed::GetCameraMode() {return m_nPlayerNumber?AimStuff::GetPlayerCameraMode(m_nPlayerNumber):AimStuff::GetCameraMode();}


// samp.dll RVA: 0x000b40c0
void __stdcall GameSetPlayerFlag(BYTE player,DWORD value) {g_gamePlayerFlags[player]=value;}
// samp.dll RVA: 0x0011eea0
const Scripting::OpcodeInfo g_pedCreatePlayerCommand={0x53,"vfffv"};
// samp.dll RVA: 0x0011eeb4
const Scripting::OpcodeInfo g_pedGetPlayerHandleCommand={0x1f5,"vv"};
// samp.dll RVA: 0x0011f0e4
const Scripting::OpcodeInfo g_pedCommand03fe={0x3fe,"ii"};
// samp.dll RVA: 0x0011f388
const float g_pedSpawnLift=0.15f;
// samp.dll RVA: 0x000b08e0
CPed::CPed(int player,int model,CVector position,float rotation) {
    DWORD handle=0,scriptPlayer=(BYTE)player;
    m_pGamePed=0;m_nHandle=0;
    Scripting::ProcessCommand(&g_pedCreatePlayerCommand,&scriptPlayer,
        (double)position.x,(double)position.y,(double)position.z,&handle);
    Scripting::ProcessCommand(&g_pedGetPlayerHandleCommand,&scriptPlayer,&handle);
    m_nHandle=handle;
    m_pGamePed=GamePoolPedGetAt(handle);m_pGameEntity=m_pGamePed;
    m_nPlayerNumber=(BYTE)player;GameAssignPlayerPed((BYTE)player,m_pGamePed);
    Scripting::ProcessCommand(&g_pedCommand087e,m_nHandle,1);
    Scripting::ProcessCommand(&g_pedImmunitiesCommand,m_nHandle,0,0,1,0,0);
    Scripting::ProcessCommand(&g_pedCommand0446,m_nHandle,0);
    if(g_pNetGame) {
        DWORD setting=*(DWORD*)(*(BYTE**)((BYTE*)g_pNetGame+0x3d5)+1);
        Scripting::ProcessCommand(&g_pedCommand03fe,m_nHandle,0);
        Scripting::ProcessCommand(&g_pedCommand03fe,m_nHandle,setting);
    }
    unknown2a8=1;unknown2ac=1;m_arrow=0;m_parachuteObject=0;m_bUsingCellphone=0;
    m_danceStyle=0;unknown2d2=0;m_bUrinating=0;m_bDancing=0;m_lastDanceMove=0;
    SetModelIndex(model);
    ForceRotation(rotation);
    KeyStuff::ResetKeys(m_nPlayerNumber);
    m_stuffType=0;m_stuffObject=0;m_drunkLevel=0;unknown2f6=0;unknown2f7=0;unknown2f8=0;
    *(DWORD*)((BYTE*)this+0x329)=0;memset(unknown2fd,0,44);
    CMatrix matrix;GetMatrix(&matrix);
    memcpy(&matrix.position.x,&position.x,4);memcpy(&matrix.position.y,&position.y,4);
    float pz=position.z,liftedZ;
    __asm {
        fld pz
        fadd g_pedSpawnLift
        fstp liftedZ
    }
    memcpy(&matrix.position.z,&liftedZ,4);SetMatrix(matrix);
    for(int i=0;i<10;i++) {
        m_accessoryOccupied[i]=0;m_accessoryObjects[i]=0;memset(&m_accessoryInfo[i],0,52);
    }
    unknown2f9=0;*(DWORD*)((BYTE*)this+0x329)=0;memset(unknown2fd,0,44);
}

#include "game/object.h"
#include "game/custommodels.h"
// samp.dll RVA: 0x000b0710
void CPed::AddAccessory(int index,const PedAccessoryState *info) {
    BYTE *ped=(BYTE*)m_pGamePed;
    // GTA SA virtual address: excluded backend vtable0x00863c40.
    if(!ped||*(DWORD*)ped==0x863c40||!*(void**)(ped+0x18))return;
    if(index<0||index>=10||info->bone<=0||info->bone>18)return;
    if(GetAccessoryState(index))DeleteAccessory(index);
    CMatrix matrix;GetMatrix(&matrix);
    PedAccessoryState *stored=&m_accessoryInfo[index];
    // Native REP MOVSD runs after deletion and forwards overlapping input/output.
    for(int i=0;i<13;i++)((volatile DWORD*)stored)[i]=((const DWORD*)info)[i];
    if(stored->model<0&&g_customModels&&g_customModels->IsObjectEntry(stored->model)&&
        !g_customModels->GetLoadedFlag(stored->model)&&!g_customModels->LoadObjectEntry(stored->model))
        stored->model=18631;
    CObject *object=new CObject(stored->model,matrix.position,stored->rotation,200.0f,1,0,0);
    m_accessoryObjects[index]=object;
    if(info->materialColor1)object->SetMaterial(-1,0,0,0,info->materialColor1);
    if(info->materialColor2)((CObject*)m_accessoryObjects[index])->SetMaterial(-1,1,0,0,info->materialColor2);
    ((CObject*)m_accessoryObjects[index])->SetCollisionFlag(FALSE);
    m_accessoryOccupied[index]=1;
}

#include "net/playerpool.h"
#include "net/vehiclepool.h"
// samp.dll RVA: 0x0011f094
const Scripting::OpcodeInfo g_pedBehaviorWalkStyleCommand={0x245,"is"};
// samp.dll RVA: 0x0011f030
const Scripting::OpcodeInfo g_pedBehaviorDrivebyCommand={0x713,"iiiffffiii"};
// samp.dll RVA: 0x0011efa4
const Scripting::OpcodeInfo g_pedBehaviorCreateObjectCommand={0x107,"ifffv"};
// samp.dll RVA: 0x0011f120
const Scripting::OpcodeInfo g_pedBehaviorAttachCommand={0x69b,"iiffffff"};
// samp.dll RVA: 0x0011f134
const Scripting::OpcodeInfo g_pedBehaviorObjectFlagCommand={0x750,"ii"};
// samp.dll RVA: 0x0011f170
const Scripting::OpcodeInfo g_pedBehaviorObjectScalarCommand={0x8d2,"if"};
// samp.dll RVA: 0x0011f10c
const Scripting::OpcodeInfo g_pedBehaviorObjectAnimationCommand={0x75a,"issfii"};
// samp.dll RVA: 0x0011efb8
const Scripting::OpcodeInfo g_pedBehaviorEffectCommand={0x18c,"fffi"};
// samp.dll RVA: 0x0011f314
const char g_pedParachuteOpen[10]="PARA_OPEN";
// samp.dll RVA: 0x0011f308
const char g_pedParachuteDecel[11]="PARA_DECEL";
// samp.dll RVA: 0x0011f2ec
const char g_pedParachuteFloat[11]="PARA_FLOAT";
// samp.dll RVA: 0x0011f2d0
const char g_pedParachuteSteerRight[12]="PARA_STEERR";
// samp.dll RVA: 0x0011f2b4
const char g_pedParachuteSteerLeft[12]="PARA_STEERL";
// samp.dll RVA: 0x0011f294
const char g_pedParachuteLandWater[16]="PARA_LAND_WATER";
// samp.dll RVA: 0x0011f268
const char g_pedParachuteOpenObject[12]="PARA_OPEN_O";
// samp.dll RVA: 0x0011f2f8
const char g_pedParachuteDecelObject[13]="PARA_DECEL_O";
// samp.dll RVA: 0x0011f2dc
const char g_pedParachuteFloatObject[13]="PARA_FLOAT_O";
// samp.dll RVA: 0x0011f2c0
const char g_pedParachuteSteerRightObject[14]="PARA_STEERR_O";
// samp.dll RVA: 0x0011f2a4
const char g_pedParachuteSteerLeftObject[14]="PARA_STEERL_O";
// samp.dll RVA: 0x0011f280
const char g_pedParachuteLandWaterObject[18]="PARA_LAND_WATER_O";
// samp.dll RVA: 0x0011a928
const double g_pedParachuteOne=1.0;
// samp.dll RVA: 0x0011f278
const double g_pedParachuteOpeningSpeed=1000.0;
// samp.dll RVA: 0x0011f260
const double g_pedDrivebyDistance=300.0;

// samp.dll RVA: 0x000ac470
BYTE CPed::GetDeathInfo(WORD *killer) {
    if(g_pNetGame) {
        BYTE *pools=*(BYTE**)((BYTE*)g_pNetGame+0x3de);
        CVehiclePool *vehicles=*(CVehiclePool**)(pools+0xc);
        CPlayerPool *players=*(CPlayerPool**)(pools+8);
        BYTE *ped=(BYTE*)m_pGamePed;
        if(ped) {
            BYTE reason=ped[0x760];
            if(reason==53){*killer=0xffff;return reason;}
            bool direct=reason<43||reason==50||reason==51;
            if(direct||reason==49||reason==54) {
                void *damager=*(void**)(ped+0x764);
                if(damager) {
                    if(direct) {
                        WORD id=players->Find(damager);
                        if(id!=0xffff){*killer=id;return reason;}
                        damager=*(void**)((BYTE*)m_pGamePed+0x764);
                    }
                    if(vehicles->Find(damager)!=0xffff) {
                        BYTE *vehicle=*(BYTE**)((BYTE*)m_pGamePed+0x764);
                        WORD id=players->Find(*(void**)(vehicle+0x460));
                        if(id!=0xffff){*killer=id;return reason;}
                    } else if(reason==54){*killer=0xffff;return reason;}
                }
            }
        }
    }
    *killer=0xffff;return 0xff;
}
// samp.dll RVA: 0x000acbb0
BOOL CPed::EnablePassengerDrivebyMode() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped||!(*(DWORD*)(ped+0x46c)&0x100)||!*(void**)(ped+0x58c))return FALSE;
    DWORD weapon=GetCurrentWeapon()&0xff;
    if(weapon==46){SetArmedWeapon(0,0);return FALSE;}
    SetArmedWeapon(weapon,0);
    Scripting::ProcessCommand(&g_pedBehaviorDrivebyCommand,m_nHandle,-1,-1,
        g_pedParachuteZero,g_pedParachuteZero,g_pedParachuteZero,g_pedDrivebyDistance,8,1,20);
    return TRUE;
}
// samp.dll RVA: 0x000acdf0
void CPed::SetWalkStyle(const char *name) {
    if(m_pGamePed)Scripting::ProcessCommand(&g_pedBehaviorWalkStyleCommand,m_nHandle,name);
}
// samp.dll RVA: 0x000ad0f0
BOOL CPed::OpenParachute() {
    if(m_parachuteObject)return TRUE;
    CMatrix matrix;GetMatrix(&matrix);
    Scripting::ProcessCommand(&g_pedBehaviorCreateObjectCommand,3131,
        (double)matrix.position.x,(double)matrix.position.y,(double)matrix.position.z,&m_parachuteObject);
    if(!GamePoolObjectGetAt(m_parachuteObject)){m_parachuteObject=0;return FALSE;}
    Scripting::ProcessCommand(&g_pedBehaviorAttachCommand,m_parachuteObject,m_nHandle,
        g_pedParachuteZero,g_pedParachuteZero,g_pedParachuteZero,
        g_pedParachuteZero,g_pedParachuteZero,g_pedParachuteZero);
    Scripting::ProcessCommand(&g_pedBehaviorObjectFlagCommand,m_parachuteObject,0);
    Scripting::ProcessCommand(&g_pedBehaviorObjectScalarCommand,m_parachuteObject,g_pedParachuteZero);
    Scripting::ProcessCommand(&g_pedBehaviorObjectAnimationCommand,m_parachuteObject,
        g_pedParachuteOpenObject,g_gameParachuteLibrary,g_pedParachuteOpeningSpeed,0,1);
    Scripting::ProcessCommand(&g_pedBehaviorObjectFlagCommand,m_parachuteObject,1);
    Scripting::ProcessCommand(&g_pedBehaviorObjectScalarCommand,m_parachuteObject,g_pedParachuteOne);
    return TRUE;
}
// samp.dll RVA: 0x000ad240
void CPed::ProcessParachuteEvent(const char *name) {
    if(!m_pGamePed||!GamePoolPedGetAt(m_nHandle))return;
    DWORD weapon;Scripting::ProcessCommand(&g_pedGetCurrentWeaponCommand,m_nHandle,&weapon);
    if((BYTE)weapon!=46)return;
    if(!strcmp(name,g_pedParachuteOpen)) {
        if(!OpenParachute())return;
        CMatrix matrix;GetMatrix(&matrix);
        if(!GameFindLocalPed())return;
        const float *local=(const float*)(*(BYTE**)((BYTE*)GameFindLocalPed()+0x14)+0x30);
        const float *position=(const float*)&matrix.position;
        double effectX,effectY,effectZ;
        __asm {
            mov eax,position
            mov edx,local
            fld dword ptr [eax]
            fsub dword ptr [edx]
            fmul g_objectMutationStep
            fadd dword ptr [edx]
            fld dword ptr [eax+4]
            fsub dword ptr [edx+4]
            fmul g_objectMutationStep
            fadd dword ptr [edx+4]
            fld dword ptr [eax+8]
            fsub dword ptr [edx+8]
            fmul g_objectMutationStep
            fadd dword ptr [edx+8]
            fstp effectZ
            fstp effectY
            fstp effectX
        }
        Scripting::ProcessCommand(&g_pedBehaviorEffectCommand,effectX,effectY,effectZ,1039);
        return;
    }
    const char *animation=0;int loop=1;
    if(!strcmp(name,g_pedParachuteDecel))animation=g_pedParachuteDecelObject;
    else if(!strcmp(name,g_pedParachuteFloat))animation=g_pedParachuteFloatObject;
    else if(!strcmp(name,g_pedParachuteSteerRight))animation=g_pedParachuteSteerRightObject;
    else if(!strcmp(name,g_pedParachuteSteerLeft))animation=g_pedParachuteSteerLeftObject;
    else if(!strcmp(name,g_pedParachuteLandWater)){animation=g_pedParachuteLandWaterObject;loop=0;}
    if(animation&&OpenParachute())Scripting::ProcessCommand(&g_pedBehaviorObjectAnimationCommand,
        m_parachuteObject,animation,g_gameParachuteLibrary,g_pedParachuteOne,loop,1);
}

// samp.dll RVA: 0x0011f3a8
const double g_pedStuffOffset1X=0.05000000074505806;
// samp.dll RVA: 0x0011f3b0
const double g_pedStuffOffset1Y=0.029999999329447746;
// samp.dll RVA: 0x0011f3b8
const double g_pedStuffOffset1Z=-0.30000001192092896;
// samp.dll RVA: 0x0011f3a0
const double g_pedStuffOffset2Z=-0.05000000074505806;
// samp.dll RVA: 0x0011f390
const double g_pedStuffOffset3X=0.03999999910593033;
// samp.dll RVA: 0x0011c988
const double g_pedStuffOffset3Y=0.10000000149011612;
// samp.dll RVA: 0x0011f398
const double g_pedStuffOffset3Z=-0.019999999552965164;

// samp.dll RVA: 0x000afcb0
void CPed::ProcessDancing() {
    if(!unknown2d2||!m_pGamePed||!CEntity::DoesExist()||m_nPlayerNumber)return;
    KeyPad *keys=KeyStuff::GetInternalKeys();
    short vertical=(short)keys->keys2[1],horizontal=(short)keys->keys2[0];
    if(!PerformingCustomAnimation()) {
        DWORD style=*(DWORD*)unknown2d6;
        ApplyAnimation(g_pedDanceNames[style],g_pedDanceLibraries[style],4.0f,1,0,0,0,0);
    }
    bool alternate=keys->keys1[16]!=0;int move;
    if(vertical>0)move=horizontal<0?4:horizontal>0?5:0;
    else if(vertical<0)move=horizontal<0?6:horizontal>0?7:1;
    else if(horizontal<0)move=2;
    else if(horizontal>0)move=3;
    else return;
    if(alternate)move+=8;
    DWORD *last=(DWORD*)(unknown2d6+4);if(*last==(DWORD)move)return;
    *last=move;const char *animation=GetAnimationForDance(move);
    if(animation) {
        DWORD style=*(DWORD*)unknown2d6;
        ApplyAnimation(animation,g_pedDanceLibraries[style],4.0f,0,0,0,0,0);
    }
}
// samp.dll RVA: 0x000afed0
void CPed::GiveStuff(int type) {
    if(m_stuffObject)DropStuff();
    CMatrix matrix;GetMatrix(&matrix);
    int model;double x,y,z;
    if(type==4){model=1485;x=g_pedParachuteZero;y=g_pedParachuteZero;z=g_pedParachuteZero;}
    else if(type==1){model=1543;x=g_pedStuffOffset1X;y=g_pedStuffOffset1Y;z=g_pedStuffOffset1Z;}
    else if(type==2){model=1486;x=g_pedStuffOffset1X;y=g_pedStuffOffset1Y;z=g_pedStuffOffset2Z;}
    else if(type==3){model=1546;x=g_pedStuffOffset3X;y=g_pedStuffOffset3Y;z=g_pedStuffOffset3Z;}
    else return;
    Scripting::ProcessCommand(&g_pedBehaviorCreateObjectCommand,model,
        (double)matrix.position.x,(double)matrix.position.y,(double)matrix.position.z,&m_stuffObject);
    if(!GamePoolObjectGetAt(m_stuffObject)){m_stuffObject=0;return;}
    Scripting::ProcessCommand(&g_pedDropStuff,m_nHandle,m_stuffObject,x,y,z,
        6,16,g_pedNullAnimation,g_pedNullAnimation,-1);
    m_stuffType=type;
}

// Raw quaternion DWORDs from the native animation-frame tables.
// samp.dll RVA: 0x00155298
static const DWORD g_pedDancePose[9][5]={
    {0x00000009,0xbf2b14fd,0x3e38cda7,0xbf38b8e1,0xbc4c0bdd},
    {0x0000000a,0x3d2bd2b7,0x3f07455a,0x3d9ce0fc,0x3f583090},
    {0x0000000b,0xbdcc5543,0x3f0e6245,0x3e959429,0x3f4586fd},
    {0x0000000c,0xbf05aa93,0xbec5dfa8,0xbee184a1,0x3f1ea37b},
    {0x0000000d,0xb97fda40,0x00000000,0x3e9b074a,0x3f73fb8b},
    {0x0000000f,0x3f23e3c1,0xbe08e326,0xbf41388b,0xbd510d39},
    {0x00000010,0x3c69ff0d,0xbf01e00d,0x3de86163,0x3f5aa876},
    {0x00000011,0x3e027aea,0xbf0cee39,0x3ea91eeb,0x3f418bc6},
    {0x00000012,0x3f1470da,0x3eede311,0xbe784a0e,0x3f1fb0a3},
};
// samp.dll RVA: 0x00155130
static const DWORD g_pedSecondaryPose[12][5]={
    {0x00000007,0x3d440e17,0xbd80895d,0x3f147b03,0x3f4f8f69},
    {0x00000009,0xbf24ab2e,0xbd978a2b,0xbf3f8cfc,0xbe140a28},
    {0x0000000e,0x00000000,0x00000000,0x3dee03b4,0x3f7e43ee},
    {0x00000013,0x397fda40,0x00000000,0x3e2d883c,0x3f7c4c16},
    {0x0000000a,0xbe05c422,0x3f0c8440,0xbde2070c,0x3f517657},
    {0x0000000b,0x00000000,0x00000000,0xbf1f0175,0x3f48a1cf},
    {0x0000000c,0xbf7ca3d7,0xbd98023a,0x3cdc0336,0xbe104253},
    {0x0000000d,0xb97fda40,0x00000000,0x3e2d883c,0x3f7c4c16},
    {0x0000000f,0x3f25b888,0x3db5097d,0xbf3d89be,0xbe21c865},
    {0x00000010,0x3e368944,0xbf13e76d,0xbd8b06c4,0x3f4b2a39},
    {0x00000011,0x00000000,0x00000000,0xbf1228dd,0x3f522cba},
    {0x00000012,0xbf768134,0xbd170a3d,0xbc68101f,0x3e88a99b},
};

// samp.dll RVA: 0x000ae7b0
void *CPed::GetBoneQuaternion(int bone) {
    BYTE *ped=(BYTE*)m_pGamePed;
    // GTA SA virtual address: excluded vtable 0x00863c40.
    if(!ped||*(DWORD*)ped==0x863c40)return 0;
    void *clump=*(void**)(ped+0x18);
    if(!clump)return 0;
    // GTA SA virtual address: clump animation hierarchy lookup.
    BYTE *hierarchy=((BYTE*(__cdecl*)(void*))0x734a40)(clump);
    BYTE *animation=*(BYTE**)(hierarchy+0x20);
    return (void*)((DWORD)animation+0x4c+*(DWORD*)(animation+0x24)*(DWORD)bone);
}

// samp.dll RVA: 0x000ae8b0
void CPed::ApplySecondaryPose() {
    for(int i=0;i<12;i++) {
        volatile DWORD *destination=(volatile DWORD*)GetBoneQuaternion(g_pedSecondaryPose[i][0]);
        if(destination)for(int j=0;j<4;j++)destination[j]=g_pedSecondaryPose[i][j+1];
    }
    void *backend=m_pGamePed;
    if(backend) {
        // GTA SA virtual address: update ped bone matrices, thiscall.
        __asm {
            mov ecx,backend
            mov eax,532b20h
            call eax
        }
    }
}

// samp.dll RVA: 0x000b0360
void CPed::ApplyDancePose() {
    for(int i=0;i<9;i++) {
        volatile DWORD *destination=(volatile DWORD*)GetBoneQuaternion(g_pedDancePose[i][0]);
        if(destination)for(int j=0;j<4;j++)destination[j]=g_pedDancePose[i][j+1];
    }
    float factor=0.0f;
    void *root=GetBoneQuaternion(0);
    if(root) {
        CVector axis={0,0,0};float angle=0.0f;
        GameQueryRwQuaternionAngle(root,&axis,&angle);
        factor=1.0f-axis.z;
    }
    // Keep repeated backend/model lookups: each native helper may change state.
    if((m_pGamePed&&GameReadPedModelTable28(*(short*)((BYTE*)m_pGamePed+0x22))==5)||
       (m_pGamePed&&GameReadPedModelTable28(*(short*)((BYTE*)m_pGamePed+0x22))==22)) {
        void *quaternion=GetBoneQuaternion(9);
        if(quaternion) {
            GameRotateRwQuaternion(quaternion,1,-6.25f,1);
            GameRotateRwQuaternion(quaternion,0,12.5f,1);
        }
        quaternion=GetBoneQuaternion(15);
        if(quaternion) {
            GameRotateRwQuaternion(quaternion,1,6.25f,1);
            GameRotateRwQuaternion(quaternion,0,-12.5f,1);
        }
    } else if(m_pGamePed&&GameReadPedModelTable24(*(short*)((BYTE*)m_pGamePed+0x22))==124) {
        void *quaternion=GetBoneQuaternion(9);
        if(quaternion) {
            GameRotateRwQuaternion(quaternion,1,10.0f,1);
            GameRotateRwQuaternion(quaternion,0,25.0f,1);
        }
        quaternion=GetBoneQuaternion(15);
        if(quaternion) {
            GameRotateRwQuaternion(quaternion,1,-10.0f,1);
            GameRotateRwQuaternion(quaternion,0,-25.0f,1);
        }
    } else {
        // Native float constants: RVAs 0011e33c/0011f3c8/0011f3c4/
        // 0011c194/0011f3c0. Every product is spilled to a float argument.
        factor=factor+0.25f;
        void *quaternion=GetBoneQuaternion(9);
        if(quaternion) {
            GameRotateRwQuaternion(quaternion,1,factor*-5.0f,1);
            GameRotateRwQuaternion(quaternion,0,factor*25.0f,1);
        }
        quaternion=GetBoneQuaternion(15);
        if(quaternion) {
            GameRotateRwQuaternion(quaternion,1,factor*5.0f,1);
            GameRotateRwQuaternion(quaternion,0,factor*-25.0f,1);
        }
    }
    void *backend=m_pGamePed;
    if(backend) {
        // GTA SA virtual address: update ped bone matrices, thiscall.
        __asm {
            mov ecx,backend
            mov eax,532b20h
            call eax
        }
    }
}

// samp.dll RVA: 0x000b0e70
void CPed::ProcessAnimationPose() {
    if(m_bDancing)ApplyDancePose();
    if(m_danceStyle)ApplySecondaryPose();
}

// samp.dll RVA: 0x000b0180
void CPed::ProcessAccessories() {
    bool bonesUpdated=false;
    for(int i=0;i<10;i++) {
        if(m_accessoryOccupied[i]!=1)continue;
        CEntity *object=(CEntity*)m_accessoryObjects[i];
        if(!object||!object->m_pGameEntity)continue;
        // The native ECX is this ped, not the accessory wrapper.
        if(!DoesExist()) {
            CVector zero={0,0,0};
            ((CEntity*)m_accessoryObjects[i])->Teleport(zero);
            continue;
        }
        if(!bonesUpdated) {
            void *ped=m_pGamePed;
            if(ped) {
                // GTA SA virtual address: update ped bone matrices, thiscall.
                __asm {
                    mov ecx,ped
                    mov eax,532b20h
                    call eax
                }
            }
            bonesUpdated=true;
        }
        void *backend=((CEntity*)m_accessoryObjects[i])->m_pGameEntity;
        DWORD *vtable=*(DWORD**)backend;
        // Cache the original backend and vtable across all following calls.
        __asm {
            mov ecx,backend
            mov eax,vtable
            call dword ptr [eax+0ch]
        }
        CMatrix matrix;
        GetBoneMatrix(&matrix,m_accessoryInfo[i].bone);
        CVector position;
        GameTransformPoint(&position,&matrix,&m_accessoryInfo[i].offset);
        memcpy(&matrix.position,&position,sizeof(position));
        for(int axis=0;axis<3;axis++) {
            float *rotation=(float*)&m_accessoryInfo[i].rotation+axis;
            BYTE rotate;
            // Skip ordered +/-zero; unordered values take the rotate branch.
            __asm {
                fld g_objectScaleLowerBound
                mov edx,rotation
                fld dword ptr [edx]
                fucompp
                fnstsw ax
                test ah,44h
                setp rotate
            }
            if(rotate)GameRotateRwMatrixOnAxis(&matrix,axis,*rotation);
        }
        GameScaleRwMatrix(&matrix,&m_accessoryInfo[i].scale);
        ((CEntity*)m_accessoryObjects[i])->SetMatrix(matrix);
        ((CEntity*)m_accessoryObjects[i])->UpdateRwFrame();
        __asm {
            mov ecx,backend
            mov eax,vtable
            call dword ptr [eax+8]
        }
    }
}

// samp.dll RVA: 0x000ac8f0
BOOL CPed::HasTaskType268() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(ped && !(*(DWORD*)(ped+0x46c)&0x100)) {
        void *task=*(void**)(*(BYTE**)(ped+0x47c)+0xc);
        if(task)return GameGetTaskType(task)==0x10c;
    }
    return FALSE;
}

// samp.dll RVA: 0x000ac1f0
int CPed::GetVehicleTransitionTaskState() {
    if(!GamePoolPedGetAt(m_nHandle)||!m_pGamePed)return 0;
    BYTE *tasks=*(BYTE**)((BYTE*)m_pGamePed+0x47c);
    if(!tasks)return 0;
    void *task=*(void**)(tasks+0x10);
    if(!task)return 0;
    DWORD type=GameGetTaskType(task);
    if(type==700||type==712)return 2;
    if(type==701||type==713)return 1;
    return 0;
}

// samp.dll RVA: 0x000ac260
BOOL CPed::HasTaskType704() {
    if(!GamePoolPedGetAt(m_nHandle)||!m_pGamePed)return FALSE;
    BYTE *tasks=*(BYTE**)((BYTE*)m_pGamePed+0x47c);
    if(!tasks)return FALSE;
    void *task=*(void**)(tasks+0x10);
    return task?GameGetTaskType(task)==704:FALSE;
}

// samp.dll RVA: 0x000ac2b0
BOOL CPed::IsDucking() {
    if(!GamePoolPedGetAt(m_nHandle)||!m_pGamePed)return FALSE;
    return (*(DWORD*)((BYTE*)m_pGamePed+0x46c)>>26)&1;
}

// samp.dll RVA: 0x000ac2e0
void CPed::ClearTasksIfOnFoot() {
    if(!GamePoolPedGetAt(m_nHandle)||!m_pGamePed)return;
    if(*(DWORD*)((BYTE*)m_pGamePed+0x46c)&0x100)return;
    if(IsDucking())return;
    void *tasks=*(void**)((BYTE*)m_pGamePed+0x47c);
    if(tasks) {
        // GTA SA virtual address: task-manager thiscall, argument 0.
        __asm {
            push 0
            mov ecx,tasks
            mov edx,601230h
            call edx
        }
    }
}

// samp.dll RVA: 0x000ac340
void CPed::ClearDuckingFlag() {
    if(!GamePoolPedGetAt(m_nHandle)||!m_pGamePed)return;
    *(DWORD*)((BYTE*)m_pGamePed+0x46c)&=0xfbffffff;
}

// samp.dll RVA: 0x000ac830
BOOL CPed::HasTaskType211() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped||(*(DWORD*)(ped+0x46c)&0x100))return FALSE;
    // Native task-manager pointer is unchecked here.
    void *task=*(void**)(*(BYTE**)(ped+0x47c)+0x10);
    return task?GameGetTaskType(task)==211:FALSE;
}

// samp.dll RVA: 0x000ac870
BOOL CPed::HasTaskType1016() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped||(*(DWORD*)(ped+0x46c)&0x100))return FALSE;
    // Native task-manager pointer is unchecked here.
    void *task=*(void**)(*(BYTE**)(ped+0x47c)+0x18);
    return task?GameGetTaskType(task)==1016:FALSE;
}

// samp.dll RVA: 0x000ac8b0
BOOL CPed::HasTaskType208() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped||(*(DWORD*)(ped+0x46c)&0x100))return FALSE;
    // Native task-manager pointer is unchecked here.
    void *task=*(void**)(*(BYTE**)(ped+0x47c)+0x4);
    return task?GameGetTaskType(task)==208:FALSE;
}

// samp.dll RVA: 0x000aca20
BOOL CPed::HasAimTarget() {
    BYTE *ped=(BYTE*)m_pGamePed;
    return ped&&(*(DWORD*)(ped+0x500)||m_lastDanceMove);
}

// samp.dll RVA: 0x000aca70
void CPed::SetSecondaryPoseState(DWORD state) {
    m_danceStyle=state;
    unknown2f7=unknown2f6=unknown2f8=state!=0;
}

// samp.dll RVA: 0x000acab0
void CPed::ProcessSecondaryPoseWeapon() {
    if(!m_danceStyle||!m_pGamePed||!GamePoolPedGetAt(m_nHandle))return;
    DWORD weapon;
    Scripting::ProcessCommand(&g_pedGetCurrentWeaponCommand,m_nHandle,&weapon);
    if((BYTE)weapon)SetArmedWeapon(0,0);
}

// samp.dll RVA: 0x000acb20
void CPed::SetDancePoseState(DWORD state) {
    m_bDancing=state;
    unknown2f8=state!=0;
}

// samp.dll RVA: 0x000acb40
void CPed::ProcessDancePoseWeapon() {
    if(!m_bDancing)return;
    if(g_deviceObjectEdit&&*(DWORD*)(g_deviceObjectEdit+0x80))unknown2f8=0;
    if(!m_pGamePed||!GamePoolPedGetAt(m_nHandle))return;
    DWORD weapon;
    Scripting::ProcessCommand(&g_pedGetCurrentWeaponCommand,m_nHandle,&weapon);
    if((BYTE)weapon)SetArmedWeapon(0,0);
}

// samp.dll RVA: 0x000acc50
BOOL CPed::HasTaskType1022() {
    if(!m_pGamePed||!GamePoolPedGetAt(m_nHandle))return FALSE;
    void *task=*(void**)(*(BYTE**)((BYTE*)m_pGamePed+0x47c)+0x10);
    return task?GameGetTaskType(task)==1022:FALSE;
}

// samp.dll RVA: 0x000acca0
void CPed::StopTaskType1022() {
    if(!m_pGamePed||!GamePoolPedGetAt(m_nHandle))return;
    void *task=*(void**)(*(BYTE**)((BYTE*)m_pGamePed+0x47c)+0x10);
    if(!task||GameGetTaskType(task)!=1022)return;
    // GTA SA virtual address: original task teardown, thiscall/1.
    __asm {
        push 1
        mov ecx,task
        mov edx,6275e0h
        call edx
    }
    // Native reloads the backend and task manager after teardown.
    *(DWORD*)(*(BYTE**)((BYTE*)m_pGamePed+0x47c)+0x10)=0;
}

// samp.dll RVA: 0x000ace20
void CPed::SetMoney(int amount) {
    Scripting::ProcessCommand(&g_pedCommand03fe,m_nHandle,0);
    Scripting::ProcessCommand(&g_pedCommand03fe,m_nHandle,amount);
}

// samp.dll RVA: 0x000ad060
void CPed::CheckParachuteWeapon() {
    if(!GamePoolPedGetAt(m_nHandle)||!DoesExist()||!m_parachuteObject)return;
    if((BYTE)GetCurrentWeapon()==46)return;
    Scripting::ProcessCommand(&g_pedDetachParachute,m_parachuteObject,
        g_pedParachuteZero,g_pedParachuteZero,g_pedParachuteZero,0);
    Scripting::ProcessCommand(&g_pedDestroyParachute,m_parachuteObject);
    m_parachuteObject=0;
}

// samp.dll RVA: 0x000ad640
BOOL CPed::HasTaskType401() {
    BYTE *ped=(BYTE*)m_pGamePed;
    // GTA SA virtual address: excluded backend vtable.
    if(!ped||*(DWORD*)ped==0x863c40||!*(void**)(ped+0x18))return FALSE;
    void *task=*(void**)(*(BYTE**)(ped+0x47c)+0x10);
    if(!task)return FALSE;
    if(GameGetTaskType(task)==401)return TRUE;
    task=GameGetTaskChild(task);
    while(task) {
        if(GameGetTaskType(task)==401) {
            // Native checks the matching child again, retaining both calls.
            return task&&GameGetTaskType(task)==401;
        }
        task=GameGetTaskChild(task);
    }
    return FALSE;
}

// samp.dll RVA: 0x000adcb0
DWORD CPed::GetBackendByte72D() {
    return m_pGamePed?*(BYTE*)((BYTE*)m_pGamePed+0x72d):0;
}

// samp.dll RVA: 0x000ae0d0
DWORD CPed::CallBackend5E01C0(DWORD first,DWORD second) {
    void *ped=m_pGamePed;
    if(!ped||*(DWORD*)ped==0x863c40)return 0;
    DWORD result;
    // GTA SA virtual address: thiscall backend(second,first,0).
    __asm {
        push 0
        push first
        push second
        mov ecx,ped
        mov edx,5e01c0h
        call edx
        mov result,eax
    }
    return result;
}

// samp.dll RVA: 0x000ae120
void CPed::ResetLocalControlWord() {
    // GTA SA virtual address: local control word.
    *(WORD*)0xb7cedc=0;
}

// samp.dll RVA: 0x000ae160
void CPed::ApplyClumpAnimation(int mode,DWORD first,DWORD second,DWORD third) {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped||*(DWORD*)ped==0x863c40)return;
    void *clump=*(void**)(ped+0x18);
    if(!clump)return;
    // GTA SA virtual addresses: native clump animation backends.
    if(mode==1)((void(__cdecl*)(void*,DWORD,DWORD))0x4d3aa0)(clump,first,second);
    else if(mode==2)((void(__cdecl*)(void*,DWORD,DWORD,DWORD))0x4d4610)(clump,first,second,third);
}

// samp.dll RVA: 0x000ae5e0
int CPed::GetBoneCount() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped||*(DWORD*)ped==0x863c40)return 0;
    void *clump=*(void**)(ped+0x18);
    if(!clump)return 0;
    // GTA SA virtual address: clump animation hierarchy lookup.
    BYTE *hierarchy=((BYTE*(__cdecl*)(void*))0x734a40)(clump);
    return *(int*)(hierarchy+4);
}

// samp.dll RVA: 0x000ae630
CMatrix * CPed::GetBoneMatrices() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped||*(DWORD*)ped==0x863c40)return 0;
    void *clump=*(void**)(ped+0x18);
    if(!clump)return 0;
    // GTA SA virtual address: clump animation hierarchy lookup.
    BYTE *hierarchy=((BYTE*(__cdecl*)(void*))0x734a40)(clump);
    return *(CMatrix **)(hierarchy+8);
}

// samp.dll RVA: 0x000ae710
void CPed::SetBoneMatrixById(const CMatrix *matrix,int bone) {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped||*(DWORD*)ped==0x863c40)return;
    void *clump=*(void**)(ped+0x18);
    if(!clump)return;
    // GTA SA virtual addresses: US/EU index helper selected before hierarchy call.
    typedef int (__cdecl *GetIndex)(void*,int);
    GetIndex getIndex=(GetIndex)(g_gameVersion==1?0x7c51a0:0x7c51e0);
    BYTE *hierarchy=((BYTE*(__cdecl*)(void*))0x734a40)(clump);
    int index=getIndex(hierarchy,bone);
    volatile DWORD *destination=(volatile DWORD*)(*(BYTE**)(hierarchy+8)+(DWORD)index*64);
    const volatile DWORD *source=(const volatile DWORD*)matrix;
    // Forward DWORD copy, including overlap, then clear native RW matrix flags.
    for(int i=0;i<16;i++)destination[i]=source[i];
    destination[3]&=0xfffdfffc;
}

// samp.dll RVA: 0x000ae850
void CPed::SetBoneMatrix(const CMatrix *matrix,int bone) {
    BYTE *entry=*(BYTE**)((BYTE*)m_pGamePed+0x488+(DWORD)bone*4);
    SetBoneMatrixById(matrix,*(int*)(entry+0x14));
}

// samp.dll RVA: 0x000aee20
float CPed::GetRootPoseFactor() {
    void *quaternion=GetBoneQuaternion(0);
    if(!quaternion)return 0.0f;
    CVector axis={0,0,0};float angle=0.0f;
    GameQueryRwQuaternionAngle(quaternion,&axis,&angle);
    // Native returns the subtraction in extended ST0, without a float spill.
    return 1.0f-axis.z;
}

// samp.dll RVA: 0x002a9280
char g_pedShoppingName[32];

// samp.dll RVA: 0x000ae1f0
DWORD CPed::PackAnimationTask(const void *task) {
    const BYTE *data=(const BYTE*)task;
    WORD index=GameFindAnimation((const char*)data+0x28,(const char*)data+0x10);
    BYTE speed=(BYTE)(int)*(const float*)(data+0x38);
    DWORD flags=*(const DWORD*)(data+0x5c);
    int time=*(const int*)(data+0x40);
    DWORD direction=time>0?2:(time<0?1:0);
    DWORD packedFlags=(direction<<4)|((flags&8)?0:8)|
        ((flags&0x80)?4:0)|((flags&0x40)?2:0)|((flags&2)?1:0);
    // Native sign-extends the index before OR, even for a failed lookup.
    return (DWORD)(int)(short)index|((DWORD)speed<<16)|(packedFlags<<24);
}

// samp.dll RVA: 0x000ae2a0
DWORD CPed::PackQueuedAnimation() {
    DWORD key=GameGetAnimationGroupKey(g_hookQueuedAnimationGroup,g_hookQueuedAnimationId);
    short index=(short)GameFindAnimationByKey(key);
    BYTE speed=(BYTE)(int)*(float*)&g_hookQueuedAnimationBlend;
    return (DWORD)(int)index|((0xffff8000u|(DWORD)speed)<<16);
}

// samp.dll RVA: 0x000ae2e0
void *CPed::FindAnimationTask() {
    void *task=*(void**)(*(BYTE**)((BYTE*)m_pGamePed+0x47c)+0x10);
    if(!task)return 0;
    if(GameGetTaskType(task)==401||GameGetTaskType(task)==934)return task;
    task=GameGetTaskChild(task);
    while(task) {
        if(GameGetTaskType(task)==401||GameGetTaskType(task)==934) {
            // Native repeats the matching-child type tests before returning it.
            if(task&&(GameGetTaskType(task)==401||GameGetTaskType(task)==934))return task;
            return 0;
        }
        task=GameGetTaskChild(task);
    }
    return 0;
}

// samp.dll RVA: 0x000ae390
DWORD CPed::GetPackedAnimation() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped||*(DWORD*)ped==0x863c40||!*(void**)(ped+0x18))return 0;
    void *task=FindAnimationTask();
    return task?PackAnimationTask(task):PackQueuedAnimation();
}

// samp.dll RVA: 0x000ae3d0
void CPed::ApplyPackedAnimation(DWORD packed) {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped||*(DWORD*)ped==0x863c40||!*(void**)(ped+0x18))return;
    float speed=(float)((packed>>16)&0xff);
    DWORD flags=packed>>24;
    DWORD direction=(flags>>4)&3;
    int time=direction==2?1:(direction==1?-1:0);
    char *combined=GameGetAnimationName((short)packed);
    char library[32]={0},name[32]={0};
    if(combined) {
        char *separator=strchr(combined,':');
        if(separator) {
            // Retain native unbounded split/copy behavior; do not add truncation.
            strncpy(library,combined,separator-combined);
            strcpy(name,separator+1);
            ApplyAnimation(name,library,speed,flags&1,(flags>>1)&1,
                (flags>>2)&1,(flags>>3)&1,time);
        }
    }
    if(!memcmp(library,g_gameParachuteLibrary,10))ProcessParachuteEvent(name);
}

// samp.dll RVA: 0x000ae560
int CPed::GetBoneId(int bone) {
    BYTE *entry=*(BYTE**)((BYTE*)m_pGamePed+0x488+(DWORD)bone*4);
    return *(int*)(entry+0x14);
}

// samp.dll RVA: 0x000ae590
DWORD CPed::GetBoneHierarchyFlags() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped||*(DWORD*)ped==0x863c40)return 0;
    void *clump=*(void**)(ped+0x18);
    if(!clump)return 0;
    // GTA SA virtual address: clump animation hierarchy lookup.
    BYTE *hierarchy=((BYTE*(__cdecl*)(void*))0x734a40)(clump);
    return *(DWORD*)hierarchy;
}

// samp.dll RVA: 0x000ae890
void CPed::UpdateBoneMatrices() {
    void *ped=m_pGamePed;
    if(ped) {
        // GTA SA virtual address: native thiscall backend.
        __asm {
            mov ecx,ped
            mov eax,532b20h
            call eax
        }
    }
}

// samp.dll RVA: 0x000adf30
const char *CPed::GetBackendShoppingName() {
    if(!m_pGamePed||!GamePoolPedGetAt(m_nHandle))return 0;
    return *(const char**)((BYTE*)m_pGamePed+0x78c);
}

// samp.dll RVA: 0x000adf80
void CPed::SetBackendShoppingName(const char *name) {
    if(!m_pGamePed||!GamePoolPedGetAt(m_nHandle))return;
    BYTE *ped=(BYTE*)m_pGamePed;
    if(name&&strlen(name)) {
        if(strlen(name)>31)return;
        memset(g_pedShoppingName,0,32);
        strncpy(g_pedShoppingName,name,32);
        *(char**)(ped+0x78c)=g_pedShoppingName;
    } else {
        // Native clears only the low byte of the field, not the whole pointer.
        *(BYTE*)(ped+0x78c)=0;
    }
}

// samp.dll RVA: 0x000aeae0
void CPed::GetAimOffsetMatrix(CMatrix *matrix) {
    GetBoneMatrix(matrix,2);
    CameraAim *aim=AimStuff::GetInternalAim();
    // Native float constants at RVAs 0011b31c and 0011f384.
    matrix->position.x=aim->front.x*0.04f+matrix->position.x;
    matrix->position.y=aim->front.y*0.04f+matrix->position.y;
    matrix->position.z=matrix->position.z+0.08f;
}

// samp.dll RVA: 0x000af5c0
BOOL CPed::GetWeaponAimOffsets(BOOL alternate,CVector *position,CVector *weaponOffsetOut) {
    int bone=alternate?34:24;
    CVector *weaponOffset=(CVector*)GetCurrentWeaponInfo();
    // Preserve read-before-backend-validation and raw three-word copy.
    memcpy(weaponOffsetOut,weaponOffset,12);
    void *ped=m_pGamePed;
    if(ped&&*(DWORD*)ped!=0x863c40) {
        // GTA SA virtual address: backend(position,bone,0), thiscall.
        __asm {
            push 0
            push bone
            push position
            mov ecx,ped
            mov edx,5e4280h
            call edx
        }
    }
    position->z=(weaponOffset->z+position->z)+g_pedSpawnLift;
    ped=m_pGamePed;
    if(ped&&*(DWORD*)ped!=0x863c40) {
        // GTA SA virtual address: backend(weaponOffsetOut,bone,0), thiscall.
        __asm {
            push 0
            push bone
            push weaponOffsetOut
            mov ecx,ped
            mov edx,5e01c0h
            call edx
        }
    }
    return TRUE;
}

// samp.dll RVA: 0x0018f664
CPed *g_pedFiringContext;
// samp.dll RVA: 0x0018f668
void *g_pedFiringData;
// samp.dll RVA: 0x0011c6d4
const char g_pedBoneIdFormat[3]="%d";

// samp.dll RVA: 0x000af490
void *CPed::GetAimHit(CVector *position) {
    if(!m_pGamePed||GetCameraMode()!=4)return 0;
    CMatrix unused;GetMatrix(&unused);
    CameraAim *aim=AimStuff::GetInternalAim();
    CVector bone;GetBonePosition(8,&bone);
    CVector origin,target;
    // Native float constants at RVAs 0011bcc8 and 0011c664.
    origin.x=aim->front.x*0.2f+bone.x;
    origin.y=aim->front.y*0.2f+bone.y;
    origin.z=aim->front.z*0.2f+bone.z;
    target.x=aim->front.x*50.0f+bone.x;
    target.y=aim->front.y*50.0f+bone.y;
    target.z=aim->front.z*50.0f+bone.z;
    BYTE collision[44];void *entity=0;
    GameProcessLineOfSightRaw(&origin,&target,collision,&entity,1,1,1,1,0,1,0,1);
    // Native ignores AL and tests the returned entity pointer instead.
    if(entity) {
        __asm {
            mov eax,position
            fld dword ptr collision
            fstp dword ptr [eax]
        }
        memcpy(&position->y,collision+4,8);
    }
    return entity;
}

// samp.dll RVA: 0x000af870
void CPed::GetCameraAimRay(CVector *origin,CVector *target) {
    CVector source={0,0,0},destination={0,0,0};
    // Native saves these only on the nonlocal path. A later changed player byte
    // still controls restoration, so do not invent initialized fallback values.
    BYTE savedMode;WORD savedModeWord;
    if(m_nPlayerNumber) {
        savedMode=*g_aimInternalCameraMode;
        *g_aimInternalCameraMode=AimStuff::GetPlayerCameraMode(m_nPlayerNumber);
        savedModeWord=*g_hookCameraModeWord;
        *g_hookCameraModeWord=AimStuff::GetPlayerCameraMode(m_nPlayerNumber);
        if(*g_hookCameraModeWord==4)*g_hookCameraModeWord=0;
        AimStuff::UpdateCamera();AimStuff::ApplyPlayerCamera(m_nPlayerNumber);
        AimStuff::UpdateAim();AimStuff::ApplyPlayerAim(m_nPlayerNumber);
        AimStuff::UpdateLocalSkills();AimStuff::ApplyPlayerSkills(m_nPlayerNumber);
    }
    // This camera-mode query is present even though its return is unused.
    GetCameraMode();
    CVector *weaponOffset=(CVector*)GetCurrentWeaponInfo();
    if(*g_aimInternalCameraMode==55) {
        CMatrix matrix;GetBoneMatrix(&matrix,6);
        GameTransformRwPoint(origin,weaponOffset,&matrix);
    } else {
        void *ped=m_pGamePed;
        if(ped&&*(DWORD*)ped!=0x863c40) {
            // GTA SA virtual address: backend(origin,24,0), thiscall.
            __asm {
                push 0
                push 24
                push origin
                mov ecx,ped
                mov edx,5e4280h
                call edx
            }
        }
        origin->z=(weaponOffset->z+origin->z)+g_pedSpawnLift;
    }
    CVector copied;memcpy(&copied,origin,12);
    // GTA SA virtual addresses: camera 00b6f028, thiscall 00514970.
    __asm {
        lea eax,destination
        push eax
        lea eax,source
        push eax
        push copied.z
        push copied.y
        push copied.x
        push 43960000h
        mov ecx,0b6f028h
        mov eax,514970h
        call eax
    }
    __asm {
        mov eax,target
        fld destination.x
        fstp dword ptr [eax]
    }
    memcpy(&target->y,&destination.y,8);
    if(m_nPlayerNumber) {
        *g_aimInternalCameraMode=savedMode;*g_hookCameraModeWord=savedModeWord;
        AimStuff::ApplyLocalSkills();AimStuff::ApplyLocalCamera();AimStuff::ApplyLocalAim();
    }
}

// samp.dll RVA: 0x000b0ce0
void *CPed::GetCameraAimHit(CVector *position) {
    if(!m_pGamePed)return 0;
    BYTE mode=GetCameraMode();
    if(mode!=53&&mode!=55&&mode!=65)return 0;
    CVector origin,target;GetCameraAimRay(&origin,&target);
    BYTE collision[44];void *entity=0;
    GameProcessLineOfSightRaw(&origin,&target,collision,&entity,0,1,1,1,0,0,0,0);
    if(entity) {
        __asm {
            mov eax,position
            fld dword ptr collision
            fstp dword ptr [eax]
        }
        memcpy(&position->y,collision+4,8);
    }
    return entity;
}

// samp.dll RVA: 0x000af670
BOOL CPed::FireWeapon() {
    BYTE savedMode;WORD savedModeWord;
    if(m_nPlayerNumber) {
        savedMode=*g_aimInternalCameraMode;
        *g_aimInternalCameraMode=AimStuff::GetPlayerCameraMode(m_nPlayerNumber);
        savedModeWord=*g_hookCameraModeWord;
        *g_hookCameraModeWord=AimStuff::GetPlayerCameraMode(m_nPlayerNumber);
        if(*g_hookCameraModeWord==4)*g_hookCameraModeWord=0;
        AimStuff::UpdateCamera();AimStuff::ApplyPlayerCamera(m_nPlayerNumber);
        AimStuff::UpdateAim();AimStuff::ApplyPlayerAim(m_nPlayerNumber);
        AimStuff::UpdateLocalSkills();AimStuff::ApplyPlayerSkills(m_nPlayerNumber);
    }
    g_pedFiringContext=this;
    g_pedFiringData=*(DWORD*)(unknown2fd+44)?unknown2fd:0;
    CVector origin={0,0,0},muzzle={0,0,0};
    DWORD weapon;
    bool sniper=false;
    if(m_pGamePed&&GamePoolPedGetAt(m_nHandle)) {
        Scripting::ProcessCommand(&g_pedGetCurrentWeaponCommand,m_nHandle,&weapon);
        sniper=(BYTE)weapon==34;
    }
    if(sniper)GameWeaponFireSniper(GetCurrentWeaponSlot(),m_pGamePed);
    else {
        GetWeaponAimOffsets(0,&origin,&muzzle);
        GameWeaponFireInstantHit(GetCurrentWeaponSlot(),m_pGamePed,&origin,&muzzle,0,0,0,0,1);
    }
    g_pedFiringContext=0;g_pedFiringData=0;
    if(m_nPlayerNumber) {
        *g_aimInternalCameraMode=savedMode;*g_hookCameraModeWord=savedModeWord;
        AimStuff::ApplyLocalCamera();AimStuff::ApplyLocalAim();AimStuff::ApplyLocalSkills();
    }
    return TRUE;
}

// samp.dll RVA: 0x000aeb90
void CPed::DrawBoneIds() {
    if(!g_deviceLabel)return;
    g_deviceLabel->Begin();
    CMatrix matrix;
    for(int bone=0;bone<64;bone++) {
        GetBoneMatrixById(&matrix,bone);
        CVector position;memcpy(&position,&matrix.position,12);
        char text[256];sprintf(text,g_pedBoneIdFormat,bone);
        g_deviceLabel->Draw(&position,text,0xffffffff,1,0);
    }
    g_deviceLabel->End();
}

// samp.dll RVA: 0x000aec20
void CPed::DrawMappedBoneIds() {
    if(!g_deviceLabel)return;
    g_deviceLabel->Begin();
    CMatrix matrix;
    for(int bone=1;bone<=18;bone++) {
        GetBoneMatrix(&matrix,bone);
        CVector position;memcpy(&position,&matrix.position,12);
        char text[256];sprintf(text,g_pedBoneIdFormat,bone);
        g_deviceLabel->Draw(&position,text,0xffffffff,1,0);
    }
    g_deviceLabel->End();
}

// samp.dll RVA: 0x000aecf0
void CPed::DrawAnimationFrameIds() {
    if(!g_deviceLabel)return;
    CMatrix matrix;GetMatrix(&matrix);
    g_deviceLabel->Begin();
    for(int bone=0;bone<30;bone++) {
        BYTE *frame=(BYTE*)GetBoneQuaternion(bone);
        if(!frame)continue;
        CVector position;GameTransformPoint(&position,&matrix,(CVector*)(frame+0x10));
        char text[256];sprintf(text,g_pedBoneIdFormat,bone);
        g_deviceLabel->Draw(&position,text,0xffffffff,1,0);
    }
    g_deviceLabel->End();
}

// samp.dll RVA: 0x000aedf0
void CPed::UpdateAndDrawBoneIds() {
    UpdateBoneMatrices();
    DrawBoneIds();
}

// samp.dll RVA: 0x000b0d90
void CPed::ProcessAimMarker() {
    CVector position;
    if(GetAimHit(&position)) {
        if(!unknown2f9) {
            CVector rotation={0,0,0};srand((unsigned)time(0));
            CObject *object=g_pGame->NewObject(19282,0,0,0,rotation,200.0f,0,0);
            unknown2f9=(DWORD)object;
            object->SetCollisionFlag(0);
            ((CEntity*)unknown2f9)->Add();
        }
        ((CEntity*)unknown2f9)->Teleport(position);
    } else if(unknown2f9) {
        delete (CObject*)unknown2f9;
        unknown2f9=0;
    }
}

// samp.dll RVA: 0x000afb10
void CPed::ClearAimTarget() {
    void *ped=m_pGamePed;
    if(!ped)return;
    if(!*(DWORD*)((BYTE*)ped+0x500)&&!m_lastDanceMove)return;
    m_lastDanceMove=0;
    // GTA SA virtual address: native thiscall backend, no arguments.
    __asm {
        mov ecx,ped
        mov eax,5e6010h
        call eax
    }
}

// samp.dll RVA: 0x000ade70
void CPed::ProcessVehicleHorn() {
    if(!m_pGamePed||!GamePoolPedGetAt(m_nHandle))return;
    KeyPad *keys=m_nPlayerNumber?KeyStuff::GetPlayerKeys(m_nPlayerNumber):KeyStuff::GetInternalKeys();
    BYTE *ped=(BYTE*)m_pGamePed;
    BYTE *vehicle=*(BYTE**)(ped+0x58c);
    if(vehicle&&(*(DWORD*)(ped+0x46c)&0x100)&&*(void**)(vehicle+0x460)==ped)
        *(BYTE*)(vehicle+0x514)=*(WORD*)((BYTE*)keys+0x24)!=0;
}

// samp.dll RVA: 0x000ae050
void CPed::SetWeaponSkill(int skill,WORD value) {
    if(m_nPlayerNumber)AimStuff::SetPlayerSkill(m_nPlayerNumber,skill,value);
    else AimStuff::SetLocalSkill(skill,value);
}

// samp.dll RVA: 0x000ae130
void CPed::UpdateClumpAnimations(float timeStep,DWORD onScreen) {
    if(!m_pGamePed)return;
    void *clump=GetRwObject();
    if(clump)GameRwUpdateClumpAnimations(clump,timeStep,onScreen);
}

// samp.dll RVA: 0x000ae1d0
BYTE *CPed::GetControlFlags() {
    return m_nPlayerNumber?GameGetPlayerControlFlags(m_nPlayerNumber):0;
}

// samp.dll RVA: 0x000ae960
float CPed::GetArmBoneDistance() {
    CMatrix first,second;memset(&first,0,64);memset(&second,0,64);
    GetBoneMatrix(&first,5);GetBoneMatrix(&second,6);
    return GameDistanceBetweenPoints(first.position.x,first.position.y,first.position.z,
        second.position.x,second.position.y,second.position.z);
}

// samp.dll RVA: 0x000aee80
void CPed::SetBulletData(PedBulletData *data) {
    volatile DWORD *destination=(volatile DWORD*)unknown2fd;
    if(!data) {
        destination[11]=0;
        for(int i=0;i<11;i++)destination[i]=0;
        return;
    }
    const volatile DWORD *source=(const volatile DWORD*)data;
    destination[11]=1;
    destination[10]=source[10];
    // Preserve forward word reads/stores and the untouched leading DWORD.
    for(int i=1;i<10;i++)destination[i]=source[i];
    if(m_nPlayerNumber||!g_pNetGame)return;
    CPlayerPool *players=*(CPlayerPool**)(*(BYTE**)((BYTE*)g_pNetGame+0x3de)+8);
    if(players)(*(CLocalPlayer**)((BYTE*)players+0x1e))->SendBullet(data);
}

// samp.dll RVA: 0x000af390
void CPed::GiveWeapon(int weapon,int ammo) {
    if(!m_pGamePed||!GamePoolPedGetAt(m_nHandle))return;
    int model=GameGetWeaponModel(weapon);
    if(model==-1)return;
    if(!g_pGame->IsModelAvailable(model)) {
        g_pGame->RequestModel(model,2);g_pGame->LoadRequestedModels();
        while(!g_pGame->IsModelAvailable(model))Sleep(1);
    }
    *g_clientCrashPedContext=m_nPlayerNumber;
    AimStuff::UpdateLocalSkills();AimStuff::ApplyPlayerSkills(m_nPlayerNumber);
    void *ped=m_pGamePed;
    // GTA SA virtual address: backend GiveWeapon(weapon,ammo,1), thiscall.
    __asm {
        push 1
        push ammo
        push weapon
        mov ecx,ped
        mov edx,5e6080h
        call edx
    }
    AimStuff::ApplyLocalSkills();
    SetArmedWeapon(weapon,0);
    *g_clientCrashPedContext=0;
}

// samp.dll RVA: 0x000afa60
void CPed::EnableGoggles() {
    if(m_pGamePed&&(*(DWORD*)((BYTE*)m_pGamePed+0x500)||m_lastDanceMove))return;
    if(!GetWeaponSlot(44)&&!GetWeaponSlot(45))GiveWeapon(44,1);
    CTaskUseGoggles *task=new CTaskUseGoggles;
    // The native allocation/constructor path does not handle a null result here.
    task->ApplyToPed(this);
    m_lastDanceMove=1;
}

// samp.dll RVA: 0x000b3fd0
BYTE *__stdcall GameGetPlayerControlFlags(BYTE player) {
    return player<210?g_gamePlayerControllerState[player]:0;
}
// samp.dll RVA: 0x000b3a20
int __stdcall GameGetWeaponModel(int weapon) {
    // Reconstruction-only lookup storage for the native switch/jump table.
    // No established standalone native data entry for this compact table.
    static const int models[46]={
        331, 333, 334, 335, 336, 337, 338, 339, 341, 321, 322, 323, 324, 325, 326, 342, 343, 344, -1, -1, -1, 346, 347, 348, 349, 350, 351, 352, 353, 355, 356, 372, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 371
    };
    return (DWORD)weapon-1u<46?models[(DWORD)weapon-1u]:-1;
}

// samp.dll RVA: 0x000acd70
WORD CPed::GetCurrentWeaponTotalAmmo() {
    BYTE *ped=(BYTE*)m_pGamePed;
    if(!ped)return 0;
    DWORD slot=(DWORD)ped+0x5a0+(DWORD)ped[0x718]*28;
    if(!slot)return 0xffff;
    DWORD weapon=*(DWORD*)slot;
    if(weapon<=15||weapon==46)return 0xffff;
    return *(WORD*)(slot+0xc);
}
// samp.dll RVA: 0x000b0ef0
void CPed::ProcessCameraAimMarker() {
    CVector position;
    if(GetCameraAimHit(&position)) {
        if(!unknown2f9) {
            CVector rotation={0,0,0};srand((unsigned)time(0));
            CObject *object=g_pGame->NewObject(3065,0,0,0,rotation,200.0f,0,0);
            unknown2f9=(DWORD)object;object->SetCollisionFlag(0);
            ((CEntity*)unknown2f9)->Add();
        }
        ((CEntity*)unknown2f9)->Teleport(position);
    } else if(unknown2f9) {
        delete (CObject*)unknown2f9;unknown2f9=0;
    }
}

// samp.dll RVA: 0x000b3f00
void GameInitializePlayerPedTables() {
    memset(g_gamePlayerPeds,0,sizeof(g_gamePlayerPeds));
    memset(g_gamePlayerControllerState,0,sizeof(g_gamePlayerControllerState));
}
// Native byte index is unchecked;210..255 reaches the adjacent controller state.
// samp.dll RVA: 0x000b3f50
void *__stdcall GameGetPlayerPedUnchecked(BYTE player) { return g_gamePlayerPeds[player]; }
// samp.dll RVA: 0x000b3f90
void __stdcall GameSetPlayerControllerWords(BYTE player,DWORD first,DWORD second,DWORD third,DWORD fourth) {
    if(player<210) {
        DWORD *words=(DWORD*)g_gamePlayerControllerState[player];
        words[0]=first;words[1]=second;words[2]=third;words[3]=fourth;
    }
}
// samp.dll RVA: 0x000b3ff0
BYTE __stdcall GameFindPlayerIndexByClump(void *clump) {
    BYTE player=0;
    do {
        void *ped=g_gamePlayerPeds[player];
        if(ped) {
            void *object=*(void**)((BYTE*)ped+0x18);
            if(object&&object==clump)return player;
        }
        ++player;
    }while(player!=210);
    return 0;
}
// Native byte index is unchecked, unlike the existing bounded flags getter.
// samp.dll RVA: 0x000b40b0
DWORD __stdcall GameGetPlayerFlagsUnchecked(BYTE player) { return g_gamePlayerFlags[player]; }

// samp.dll RVA: 0x000ab670
PedBulletData *CPed::GetBulletData() {
    return *(DWORD*)(unknown2fd+44)?(PedBulletData*)unknown2fd:0;
}
