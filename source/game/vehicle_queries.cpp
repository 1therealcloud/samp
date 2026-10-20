#include "game/vehicle.h"
#include "game/object_math.h"
// samp.dll RVA: 0x000b7940
BOOL CVehicle::IsRC() {
    if(m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle)) {
        short model=*(short*)((BYTE*)m_pGameVehicle+0x22);
        return model==0x1b9 || model==0x1d0 || model==0x1d1 ||
            model==0x252 || model==0x1f5 || model==0x234;
    }
    return FALSE;
}
// samp.dll RVA: 0x000b7e60
BOOL CVehicle::HasSiren() { return *(DWORD*)((BYTE*)this+0x8b)!=0; }
// samp.dll RVA: 0x000b7e70
BYTE CVehicle::GetMaxPassengers() {
    if(m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle))return *((BYTE*)m_pGameVehicle+0x488);
    return 0;
}
// samp.dll RVA: 0x000b6de0
float CVehicle::GetHealth() {
    return m_pGameVehicle?*(float*)((BYTE*)m_pGameVehicle+0x4c0):g_objectScaleLowerBound;
}
// samp.dll RVA: 0x000b6ed0
int CVehicle::GetSubtype() { return m_pGameVehicle?GameGetVehicleSubtype(m_pGameVehicle):0; }
// samp.dll RVA: 0x000b5d20
int GameGetVehicleSubtype(void *vehicle) {
    if(!vehicle)return 0;
    // GTA SA vtable virtual addresses, not samp.dll addresses.
    switch(*(DWORD*)vehicle) {
    case 0x871120:return 1;
    case 0x8721a0:return 4;
    case 0x871360:return 2;
    case 0x871948:return 5;
    case 0x871680:return 3;
    case 0x871528:return 6;
    case 0x872370:return 7;
    }
    return 0;
}

#include "game/playerped.h"

// samp.dll RVA: 0x000b6c00
void CVehicle::ResetPointers() {
    if(m_pGameVehicle) {
        m_pGameVehicle=GamePoolVehicleGetAt(m_nHandle);
        *(void**)((BYTE*)this+0x40)=m_pGameVehicle;
    }
}

// samp.dll RVA: 0x000b6c20
BOOL CVehicle::HasDriver() {
    if(m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle) && m_pGameVehicle) {
        BYTE *driver=*(BYTE**)((BYTE*)m_pGameVehicle+0x460);
        if(driver && (*(DWORD*)(driver+0x46c)&0x100) && !*(DWORD*)(driver+0x598))return TRUE;
    }
    return FALSE;
}

// samp.dll RVA: 0x000b6c70
BOOL CVehicle::IsOccupied() {
    BYTE *vehicle=(BYTE*)m_pGameVehicle;
    if(vehicle) {
        for(int slot=0;slot<8;++slot)if(*(void**)(vehicle+0x460+slot*4))return TRUE;
    }
    return FALSE;
}

// samp.dll RVA: 0x000b6e00
void CVehicle::SetHealth(float health) {
    if(m_pGameVehicle)*(DWORD*)((BYTE*)m_pGameVehicle+0x4c0)=*(DWORD*)&health;
}

// samp.dll RVA: 0x000b6f30
BOOL CVehicle::DriverIsPlayerPed() {
    if(m_pGameVehicle) {
        void *ped=GameFindLocalPed();
        if(*(void**)((BYTE*)m_pGameVehicle+0x460)==ped)return TRUE;
    }
    return FALSE;
}

// samp.dll RVA: 0x000b6f60
BOOL CVehicle::HasPlayerPed() {
    if(m_pGameVehicle) {
        BYTE *ped=(BYTE*)GameFindLocalPed();
        if(ped && (*(DWORD*)(ped+0x46c)&0x100) && m_pGameVehicle==*(void**)(ped+0x58c))return TRUE;
    }
    return FALSE;
}

// samp.dll RVA: 0x000b7080
void CVehicle::EnableSiren(BYTE enabled) {
    BYTE *flags=(BYTE*)m_pGameVehicle+0x42d;
    *flags=(BYTE)((*flags&0x7f)|(enabled<<7));
}

// samp.dll RVA: 0x000b70a0
bool CVehicle::SirenEnabled() {
    return (*(BYTE*)((BYTE*)m_pGameVehicle+0x42d)&0x80)!=0;
}

// samp.dll RVA: 0x000b70c0
void CVehicle::SetBackendWord45c(WORD value) {
    *(WORD*)((BYTE*)m_pGameVehicle+0x45c)=value;
}

// samp.dll RVA: 0x000b71e0
void CVehicle::SetHydraThrusters(DWORD value) {
    if(m_pGameVehicle)*(DWORD*)((BYTE*)m_pGameVehicle+0x86c)=value;
}

// samp.dll RVA: 0x000b7200
DWORD CVehicle::GetHydraThrusters() {
    return m_pGameVehicle?*(DWORD*)((BYTE*)m_pGameVehicle+0x86c):0;
}

// samp.dll RVA: 0x000b7420
float CVehicle::GetTrainSpeed() {
    return m_pGameVehicle?*(float*)((BYTE*)m_pGameVehicle+0x5a4):g_objectScaleLowerBound;
}

// samp.dll RVA: 0x000b76c0
DWORD CVehicle::GetPanelsDamage() {
    return m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle)==1?*(DWORD*)((BYTE*)m_pGameVehicle+0x5b4):0;
}

// samp.dll RVA: 0x000b76f0
DWORD CVehicle::GetDoorsDamage() {
    return m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle)==1?*(DWORD*)((BYTE*)m_pGameVehicle+0x5a9):0;
}

// samp.dll RVA: 0x000b7720
BYTE CVehicle::GetLightsDamage() {
    return m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle)==1?*(BYTE*)((BYTE*)m_pGameVehicle+0x5b0):0;
}

// samp.dll RVA: 0x000b77c0
void CVehicle::SetTrailer(CVehicle *trailer) {
    *(CVehicle**)((BYTE*)this+0x48)=trailer;
}

// samp.dll RVA: 0x000b7910
bool CVehicle::IsTowtruck() {
    return m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle) && *(short*)((BYTE*)m_pGameVehicle+0x22)==0x20d;
}

// samp.dll RVA: 0x000b7c70
bool CVehicle::DoesExist() {
    return GamePoolVehicleGetAt(m_nHandle)!=0;
}

// samp.dll RVA: 0x000b7d40
void CVehicle::SetField50(DWORD value) {
    *(DWORD*)((BYTE*)this+0x50)=value;
}

// samp.dll RVA: 0x000b7d50
DWORD CVehicle::GetField50() {
    return *(DWORD*)((BYTE*)this+0x50);
}

// samp.dll RVA: 0x000b7d60
void CVehicle::SetField54(DWORD value) {
    *(DWORD*)((BYTE*)this+0x54)=value;
}

// samp.dll RVA: 0x000b7d70
DWORD CVehicle::GetField54() {
    return *(DWORD*)((BYTE*)this+0x54);
}

#include "game/entity.h"
#include "game/wrapper_commands.h"
#include <string.h>

// samp.dll RVA: 0x0018f5dc
DWORD g_vehicleDetachingTrailer;

// samp.dll RVA: 0x000b6bd0
void CVehicle::ChangeInterior(int interior) {
    if(GamePoolVehicleGetAt(m_nHandle))Scripting::ProcessCommand(&g_vehicleChangeInteriorCommand,m_nHandle,interior);
}

// samp.dll RVA: 0x000b6e70
void CVehicle::UpdateColor() {
    if(m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle) && m_bColorChanged && !m_bInModShop) {
        if(m_nPrimaryColor!=*((BYTE*)m_pGameVehicle+0x434) || m_nSecondaryColor!=*((BYTE*)m_pGameVehicle+0x435)) {
            *((BYTE*)m_pGameVehicle+0x434)=m_nPrimaryColor;
            *((BYTE*)m_pGameVehicle+0x435)=m_nSecondaryColor;
        }
    }
}

// samp.dll RVA: 0x000b6ef0
DWORD CVehicle::IsSunk() {
    return m_pGameVehicle?Scripting::ProcessCommand(&g_vehicleIsSunkCommand,m_nHandle):0;
}

// samp.dll RVA: 0x000b6fe0
BOOL CVehicle::HasTurret() {
    int model=((CEntity*)this)->GetModelIndex();
    return model==0x1b0 || model==0x234 || model==0x197 || model==0x259;
}

// samp.dll RVA: 0x000b7010
void CVehicle::SetBackendBytes1f6And514(BYTE value) {
    if(m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle)) {
        if(m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle)==4)return;
        if(m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle)==5)return;
        if(m_pGameVehicle && GameGetVehicleSubtype(m_pGameVehicle)==3)return;
        *((BYTE*)m_pGameVehicle+0x1f6)=value;
        *((BYTE*)m_pGameVehicle+0x514)=value;
    }
}

// samp.dll RVA: 0x000b7380
void CVehicle::Lock(BOOL locked) {
    if(locked) {
        *(DWORD*)((BYTE*)m_pGameVehicle+0x4f8)=2;
        *(DWORD*)((BYTE*)this+0x60)=1;
    } else {
        *(DWORD*)((BYTE*)m_pGameVehicle+0x4f8)=0;
        *(DWORD*)((BYTE*)this+0x60)=0;
    }
}

// samp.dll RVA: 0x000b73b0
BOOL CVehicle::UpdateLastDrivenTime() {
    if(m_pGameVehicle && *(void**)((BYTE*)m_pGameVehicle+0x460)) {
        *(DWORD*)((BYTE*)this+0x6d)=1;
        *(DWORD*)((BYTE*)this+0x69)=GetTickCount();
        return TRUE;
    }
    return FALSE;
}

// samp.dll RVA: 0x000b7750
void CVehicle::AttachTrailer() {
    CVehicle *trailer=*(CVehicle**)((BYTE*)this+0x48);
    if(trailer)Scripting::ProcessCommand(&g_vehicleAttachTrailerCommand,trailer->m_nHandle,m_nHandle);
}

// samp.dll RVA: 0x000b7770
void CVehicle::DetachTrailer() {
    g_vehicleDetachingTrailer=1;
    if(*(CVehicle**)((BYTE*)this+0x48) && (*(CVehicle**)((BYTE*)this+0x48))->m_nHandle &&
        GamePoolVehicleGetAt((*(CVehicle**)((BYTE*)this+0x48))->m_nHandle) &&
        (*(CVehicle**)((BYTE*)this+0x48))->m_pGameVehicle) {
        Scripting::ProcessCommand(&g_vehicleDetachTrailerCommand,(*(CVehicle**)((BYTE*)this+0x48))->m_nHandle,m_nHandle);
    }
    g_vehicleDetachingTrailer=0;
}

// samp.dll RVA: 0x000b7990
void CVehicle::EnableLights(BYTE enabled) {
    if(m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle)) {
        if(enabled) {
            *((BYTE*)m_pGameVehicle+0x4a8)&=0xf7;
            *((BYTE*)m_pGameVehicle+0x428)|=0x40;
        } else {
            *((BYTE*)m_pGameVehicle+0x4a8)|=8;
            *((BYTE*)m_pGameVehicle+0x428)&=0xbf;
        }
    }
}

// samp.dll RVA: 0x000b79f0
BYTE CVehicle::GetBackendLightsDisabledFlag() {
    if(m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle))return (*((BYTE*)m_pGameVehicle+0x4a8)>>3)&1;
    return 1;
}

// samp.dll RVA: 0x000b7be0
BOOL CVehicle::RemoveComponent(WORD component) {
    if(m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle)) {
        Scripting::ProcessCommand(&g_vehicleRemoveComponentCommand,m_nHandle,(unsigned int)component);
        return TRUE;
    }
    return FALSE;
}

// samp.dll RVA: 0x000b7c20
void CVehicle::SetPaintjob(BYTE paintjob) {
    if(m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle) && m_pGameVehicle &&
        GameGetVehicleSubtype(m_pGameVehicle)==1 && paintjob<4) {
        Scripting::ProcessCommand(&g_vehiclePaintjobCommand,m_nHandle,(unsigned int)paintjob);
    }
}

// samp.dll RVA: 0x000b7c80
void CVehicle::SetLicensePlateText(const char *text) {
    strncpy((char*)this+0x93,text,32);
}

// samp.dll RVA: 0x000b7ca0
void CVehicle::SetRotation(float angle) {
    if(GamePoolVehicleGetAt(m_nHandle))Scripting::ProcessCommand(&g_vehicleRotationCommand,m_nHandle,(double)angle);
}

// samp.dll RVA: 0x000b7de0
float CVehicle::GetBackendFloat648() {
    return m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle)?*(float*)((BYTE*)m_pGameVehicle+0x648):g_objectScaleLowerBound;
}

// samp.dll RVA: 0x000b7ea0
void CVehicle::SetWindowOpenFlag(BYTE window) {
    void *backend=m_pGameVehicle;
    if(backend) {
        unsigned int value=window;
        __asm {
            mov ecx,backend
            push value
            mov edx,0x6d3080
            call edx
        }
    }
}

// samp.dll RVA: 0x000b7ed0
void CVehicle::ClearWindowOpenFlag(BYTE window) {
    void *backend=m_pGameVehicle;
    if(backend) {
        unsigned int value=window;
        __asm {
            mov ecx,backend
            push value
            mov edx,0x6d30b0
            call edx
        }
    }
}
