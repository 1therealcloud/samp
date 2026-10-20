#include "game/model.h"
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

#include "game/rw_helpers.h"

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

#include "game/input.h"

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

#include "game/entity_collision.h"
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
