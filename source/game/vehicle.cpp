#include "game/vehicle.h"
// samp.dll RVA: 0x000b37d0
void *__stdcall GamePoolVehicleGetAt(int handle) {
    void *result;
    __asm {
        mov ecx,dword ptr ds:[0xb74494]
        push handle
        mov eax,0x4048e0
        call eax
        mov result,eax
    }
    return result;
}
// samp.dll RVA: 0x000b6e20
void CVehicle::SetColor(BYTE primary,BYTE secondary) {
    if(m_pGameVehicle && GamePoolVehicleGetAt(m_nHandle)) {
        ((BYTE*)m_pGameVehicle)[0x434]=primary;((BYTE*)m_pGameVehicle)[0x435]=secondary;
    }
    m_nSecondaryColor=secondary;m_nPrimaryColor=primary;m_bColorChanged=TRUE;
}
