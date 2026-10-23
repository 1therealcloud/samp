#ifndef SAMP_RECOVERED_VEHICLE_H
#define SAMP_RECOVERED_VEHICLE_H
#include "game/entity.h"
#include "game/scripting.h"
#include <stddef.h>
// Native allocation extent B8 and CEntity prefix/three-slot virtual interface.
// Opaque bytes retain unknown meaning; lifecycle follows the recovered DL bodies.
#pragma pack(push,1)
class CVehicle : public CEntity {
public:
    // Raw native flag; no unverified higher-level flag name assigned.
    // samp.dll RVA: 0x000b6db0
    void SetBackendFlag428Bit5(BOOL enabled);
    // Native backend pointer is unchecked; result is returned in ST0.
    // samp.dll RVA: 0x000b7400
    float GetBackendFloat94C();
    // Native backend pointer is unchecked; result is returned in ST0.
    // samp.dll RVA: 0x000b7410
    float GetBackendFloat950();
    // samp.dll RVA: 0x000b6d70
    void SetLocked(BOOL locked);
    // samp.dll RVA: 0x000b6f10
    BOOL IsWrecked();

    CVehicle *m_pTrailer;
    void *m_pGameVehicle;
    BYTE unknown50[0x21];
    BOOL m_bInModShop;
    BYTE unknown75[4];
    BYTE m_nPrimaryColor;
    BYTE m_nSecondaryColor;
    BOOL m_bColorChanged;
    BYTE unknown7f[0x39];
    // Native constructor with ordinary, train and carriage paths.
    // samp.dll RVA: 0x000b7f00
    CVehicle(int model,float x,float y,float z,float rotation,DWORD keepLoaded,BOOL siren);
    // samp.dll RVA: 0x000b8420
    virtual ~CVehicle();
    // samp.dll RVA: 0x000b8540
    virtual void Add();
    // samp.dll RVA: 0x000b8570
    virtual void Remove();
    // samp.dll RVA: 0x000b85a0
    void EnableEngine(BOOL enabled);
    // samp.dll RVA: 0x000b7220
    void ProcessMarkers();
    // samp.dll RVA: 0x000b77d0
    CVehicle * GetTrailer();
    // samp.dll RVA: 0x000b7830
    CVehicle * GetTractor();
    // samp.dll RVA: 0x000b7a20
    void RemovePassengers();
    // samp.dll RVA: 0x000b7b00
    BOOL AddComponent(WORD component);
    // samp.dll RVA: 0x000b6cd0
    void SetInvulnerable(BOOL enabled);
    // samp.dll RVA: 0x000b70e0
    void SetLandingGearState(int state);
    // samp.dll RVA: 0x000b7170
    BOOL GetLandingGearState();
    // samp.dll RVA: 0x000b7480
    void SetTires(BYTE tires);
    // samp.dll RVA: 0x000b7570
    BYTE GetTires();
    // samp.dll RVA: 0x000b7600
    void UpdateDamage(DWORD panels,DWORD doors,BYTE lights);
    // samp.dll RVA: 0x000b78b0
    BOOL IsTrailer();
    // samp.dll RVA: 0x000b7d80
    void CallBackend6a6ae0(DWORD first,DWORD second,DWORD third);
    // samp.dll RVA: 0x000b7440
    void SetTrainSpeed(float speed);
    // samp.dll RVA: 0x000b7e10
    void SetBackendFloat64c(float speed);
    // samp.dll RVA: 0x000b6bd0
    void ChangeInterior(int interior);
    // samp.dll RVA: 0x000b6e70
    void UpdateColor();
    // samp.dll RVA: 0x000b6ef0
    DWORD IsSunk();
    // samp.dll RVA: 0x000b6fe0
    BOOL HasTurret();
    // samp.dll RVA: 0x000b7010
    void SetBackendBytes1f6And514(BYTE value);
    // samp.dll RVA: 0x000b7380
    void Lock(BOOL locked);
    // samp.dll RVA: 0x000b73b0
    BOOL UpdateLastDrivenTime();
    // samp.dll RVA: 0x000b7750
    void AttachTrailer();
    // samp.dll RVA: 0x000b7770
    void DetachTrailer();
    // samp.dll RVA: 0x000b7990
    void EnableLights(BYTE enabled);
    // samp.dll RVA: 0x000b79f0
    BYTE GetBackendLightsDisabledFlag();
    // samp.dll RVA: 0x000b7be0
    BOOL RemoveComponent(WORD component);
    // samp.dll RVA: 0x000b7c20
    void SetPaintjob(BYTE paintjob);
    // samp.dll RVA: 0x000b7c80
    void SetLicensePlateText(const char *text);
    // samp.dll RVA: 0x000b7ca0
    void SetRotation(float angle);
    // samp.dll RVA: 0x000b7de0
    float GetBackendFloat648();
    // samp.dll RVA: 0x000b7ea0
    void SetWindowOpenFlag(BYTE window);
    // samp.dll RVA: 0x000b7ed0
    void ClearWindowOpenFlag(BYTE window);
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
typedef char VehicleExtent[(sizeof(CVehicle)==0xb8)?1:-1];
typedef char VehicleHandleOffset[(offsetof(CVehicle,m_nHandle)==0x44)?1:-1];
typedef char VehicleTrailerOffset[(offsetof(CVehicle,m_pTrailer)==0x48)?1:-1];
typedef char VehicleBackendOffset[(offsetof(CVehicle,m_pGameVehicle)==0x4c)?1:-1];
typedef char VehicleColorOffset[(offsetof(CVehicle,m_bColorChanged)==0x7b)?1:-1];
// samp.dll RVA: 0x000b37d0
void *__stdcall GamePoolVehicleGetAt(int handle);
// GTA SA vtable classification helper; values are native 0..7 tags.
// samp.dll RVA: 0x000b5d20
int GameGetVehicleSubtype(void *vehicle);
// samp.dll RVA: 0x0018f5dc
extern DWORD g_vehicleDetachingTrailer;
// samp.dll RVA: 0x001181a0
extern const float g_vehicleTrainSpeedMinimum;
// samp.dll RVA: 0x001181a4
extern const float g_vehicleTrainSpeedMaximum;
// samp.dll RVA: 0x0011812c
extern const float g_vehicleMarkerDistance;
// samp.dll RVA: 0x000b3eb0
void __stdcall GameRemoveDriverByState598(void *vehicle);
// Raw heap callback; record must be nonnull. First argument is unused natively.
// samp.dll RVA: 0x000b86b0
DWORD __stdcall GameDeletePointerRecord(DWORD unused,void **record);
// samp.dll RVA: 0x002abe64
extern void *g_vehicleTrainCursor;
// samp.dll RVA: 0x002abe68
extern DWORD g_vehicleLastCreatedHandle;
// samp.dll RVA: 0x0011e33c
extern const float g_vehicleCreationLift;
// samp.dll RVA: 0x0011f7e0
extern const char g_vehicleBadCarriageWarning[29];
// samp.dll RVA: 0x0011f800
extern const char g_vehicleCreationWarning[42];
#endif

// samp.dll RVA: 0x0011f704
extern const Scripting::OpcodeInfo g_vehicleLockedCommand;
// samp.dll RVA: 0x0011f68c
extern const Scripting::OpcodeInfo g_vehicleWreckedCommand;
