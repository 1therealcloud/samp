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
