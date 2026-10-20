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

