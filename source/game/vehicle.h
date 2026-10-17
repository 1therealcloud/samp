#ifndef SAMP_RECOVERED_VEHICLE_H
#define SAMP_RECOVERED_VEHICLE_H
#include <windows.h>
#include <stddef.h>
// Observed prefix only. Full allocation extent, inheritance and remaining fields
// are not established; production code must not allocate this partial type yet.
#pragma pack(push,1)
class CVehicle {
public:
    BYTE unknown00[0x44];
    int m_nHandle;
    BYTE unknown48[4];
    void *m_pGameVehicle;
    BYTE unknown50[0x21];
    BOOL m_bInModShop;
    BYTE unknown75[4];
    BYTE m_nPrimaryColor;
    BYTE m_nSecondaryColor;
    BOOL m_bColorChanged;
    // samp.dll RVA: 0x000b6e20
    void SetColor(BYTE primary,BYTE secondary);
};
#pragma pack(pop)
typedef char VehicleBackendOffset[(offsetof(CVehicle,m_pGameVehicle)==0x4c)?1:-1];
typedef char VehicleColorOffset[(offsetof(CVehicle,m_bColorChanged)==0x7b)?1:-1];
// samp.dll RVA: 0x000b37d0
void *__stdcall GamePoolVehicleGetAt(int handle);
#endif
