#include "game/vehicle.h"
// samp.dll RVA: 0x000b7940
BOOL CVehicle::IsRC() {
    if(m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle)) {
        short model=*(short*)((BYTE*)m_pGameVehicle+0x22);
        return model==0x1b9 || model==0x1d0 || model==0x1d1 ||
            model==0x252 || model==0x1f5 || model==0x234;
    }
    return FALSE;
}
