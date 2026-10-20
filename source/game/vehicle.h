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
    // samp.dll RVA: 0x000b6c00
    void ResetPointers();
    // samp.dll RVA: 0x000b6c20
    BOOL HasDriver();
    // samp.dll RVA: 0x000b6c70
    BOOL IsOccupied();
    // samp.dll RVA: 0x000b6e00
    void SetHealth(float health);
    // samp.dll RVA: 0x000b6f30
    BOOL DriverIsPlayerPed();
    // samp.dll RVA: 0x000b6f60
    BOOL HasPlayerPed();
    // samp.dll RVA: 0x000b7080
    void EnableSiren(BYTE enabled);
    // samp.dll RVA: 0x000b70a0
    bool SirenEnabled();
    // samp.dll RVA: 0x000b70c0
    void SetBackendWord45c(WORD value);
    // samp.dll RVA: 0x000b71e0
    void SetHydraThrusters(DWORD value);
    // samp.dll RVA: 0x000b7200
    DWORD GetHydraThrusters();
    // samp.dll RVA: 0x000b7420
    float GetTrainSpeed();
    // samp.dll RVA: 0x000b76c0
    DWORD GetPanelsDamage();
    // samp.dll RVA: 0x000b76f0
    DWORD GetDoorsDamage();
    // samp.dll RVA: 0x000b7720
    BYTE GetLightsDamage();
    // samp.dll RVA: 0x000b77c0
    void SetTrailer(CVehicle *trailer);
    // samp.dll RVA: 0x000b7910
    bool IsTowtruck();
    // samp.dll RVA: 0x000b7c70
    bool DoesExist();
    // samp.dll RVA: 0x000b7d40
    void SetField50(DWORD value);
    // samp.dll RVA: 0x000b7d50
    DWORD GetField50();
    // samp.dll RVA: 0x000b7d60
    void SetField54(DWORD value);
    // samp.dll RVA: 0x000b7d70
    DWORD GetField54();
    // samp.dll RVA: 0x000b6e20
    void SetColor(BYTE primary,BYTE secondary);
    // samp.dll RVA: 0x000b6fa0
    BOOL IsTrainPart();
    // samp.dll RVA: 0x000b7940
    BOOL IsRC();
    // Raw wrapper DWORD at +8B, normalized by the native query.
    // samp.dll RVA: 0x000b7e60
    BOOL HasSiren();
    // Additional stage 2 wrapper dependencies; no complete allocation claim.
    // samp.dll RVA: 0x000b7e70
    BYTE GetMaxPassengers();
    // samp.dll RVA: 0x000b6de0
    float GetHealth();
    // samp.dll RVA: 0x000b6ed0
    int GetSubtype();
    // samp.dll RVA: 0x000b7cd0
    void ConstructLicensePlate();
    // samp.dll RVA: 0x000b7d20
    void ShutdownLicensePlate();
};
#pragma pack(pop)
typedef char VehicleBackendOffset[(offsetof(CVehicle,m_pGameVehicle)==0x4c)?1:-1];
typedef char VehicleColorOffset[(offsetof(CVehicle,m_bColorChanged)==0x7b)?1:-1];
// samp.dll RVA: 0x000b37d0
void *__stdcall GamePoolVehicleGetAt(int handle);
// GTA SA vtable classification helper; values are native 0..7 tags.
// samp.dll RVA: 0x000b5d20
int GameGetVehicleSubtype(void *vehicle);
#endif
