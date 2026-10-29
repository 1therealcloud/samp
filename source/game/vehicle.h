#ifndef SAMP_RECOVERED_VEHICLE_H
#define SAMP_RECOVERED_VEHICLE_H
#include "game/entity.h"
#include "game/scripting.h"
#include <stddef.h>
struct IDirect3DTexture9;
// Native allocation extent B8 and CEntity prefix/three-slot virtual interface.
// Opaque bytes retain unknown meaning; lifecycle follows the recovered DL bodies.
#pragma pack(push, 1)
class CVehicle : public CEntity
{
public:
    // Stores the raw byte; does not reset/create the objective blip.
    // 0x0000e290
    void SetObjective(BYTE objective);
    // 0x000b73e0
    void SetBackendFloat94CAnd950(float first, float second);

    // Raw native flag; no unverified higher-level flag name assigned.
    // 0x000b6db0
    void SetBackendFlag428Bit5(BOOL enabled);
    // Native backend pointer is unchecked; result is returned in ST0.
    // 0x000b7400
    float GetBackendFloat94C();
    // Native backend pointer is unchecked; result is returned in ST0.
    // 0x000b7410
    float GetBackendFloat950();
    // 0x000b6d70
    void SetLocked(BOOL locked);
    // 0x000b6f10
    BOOL IsWrecked();

    CVehicle* m_pTrailer;
    void* m_pGameVehicle;
    DWORD m_dwField50, m_dwField54;
    DWORD m_dwInvulnerable, m_dwUnknown5c, m_dwLocked;
    BYTE m_byteObjective;
    DWORD m_dwObjectiveMarker, m_dwLastDrivenTime, m_dwHasBeenDriven;
    BOOL m_bInModShop;
    DWORD m_dwEngineOverride75;
    BYTE m_nPrimaryColor;
    BYTE m_nSecondaryColor;
    BOOL m_bColorChanged;
    DWORD m_dwLocalUnoccupiedSync, m_dwRemoteUnoccupiedSync;
    DWORD m_dwKeepLoaded, m_dwSiren;
    IDirect3DTexture9* m_pLicensePlateTexture;
    char m_szLicensePlateText[33];
    DWORD m_dwMarker;
    // Native constructor with ordinary, train and carriage paths.
    // 0x000b7f00
    CVehicle(int model, float x, float y, float z, float rotation, DWORD keepLoaded, BOOL siren);
    // 0x000b8420
    virtual ~CVehicle();
    // 0x000b8540
    virtual void Add();
    // 0x000b8570
    virtual void Remove();
    // 0x000b85a0
    void EnableEngine(BOOL enabled);
    // 0x000b7220
    void ProcessMarkers();
    // 0x000b77d0
    CVehicle* GetTrailer();
    // 0x000b7830
    CVehicle* GetTractor();
    // 0x000b7a20
    void RemovePassengers();
    // 0x000b7b00
    BOOL AddComponent(WORD component);
    // 0x000b6cd0
    void SetInvulnerable(BOOL enabled);
    // 0x000b70e0
    void SetLandingGearState(int state);
    // 0x000b7170
    BOOL GetLandingGearState();
    // 0x000b7480
    void SetTires(BYTE tires);
    // 0x000b7570
    BYTE GetTires();
    // 0x000b7600
    void UpdateDamage(DWORD panels, DWORD doors, BYTE lights);
    // 0x000b78b0
    BOOL IsTrailer();
    // 0x000b7d80
    void CallBackend6a6ae0(DWORD first, DWORD second, DWORD third);
    // 0x000b7440
    void SetTrainSpeed(float speed);
    // 0x000b7e10
    void SetBackendFloat64c(float speed);
    // 0x000b6bd0
    void ChangeInterior(int interior);
    // 0x000b6e70
    void UpdateColor();
    // 0x000b6ef0
    DWORD IsSunk();
    // 0x000b6fe0
    BOOL HasTurret();
    // 0x000b7010
    void SetBackendBytes1f6And514(BYTE value);
    // 0x000b7380
    void Lock(BOOL locked);
    // 0x000b73b0
    BOOL UpdateLastDrivenTime();
    // 0x000b7750
    void AttachTrailer();
    // 0x000b7770
    void DetachTrailer();
    // 0x000b7990
    void EnableLights(BYTE enabled);
    // 0x000b79f0
    BYTE GetBackendLightsDisabledFlag();
    // 0x000b7be0
    BOOL RemoveComponent(WORD component);
    // 0x000b7c20
    void SetPaintjob(BYTE paintjob);
    // 0x000b7c80
    void SetLicensePlateText(const char* text);
    // 0x000b7ca0
    void SetRotation(float angle);
    // 0x000b7de0
    float GetBackendFloat648();
    // 0x000b7ea0
    void SetWindowOpenFlag(BYTE window);
    // 0x000b7ed0
    void ClearWindowOpenFlag(BYTE window);
    // 0x000b6c00
    void ResetPointers();
    // 0x000b6c20
    BOOL HasDriver();
    // 0x000b6c70
    BOOL IsOccupied();
    // 0x000b6e00
    void SetHealth(float health);
    // 0x000b6f30
    BOOL DriverIsPlayerPed();
    // 0x000b6f60
    BOOL HasPlayerPed();
    // 0x000b7080
    void EnableSiren(BYTE enabled);
    // 0x000b70a0
    BOOL SirenEnabled();
    // 0x000b70c0
    void SetBackendWord45c(WORD value);
    // 0x000b71e0
    void SetHydraThrusters(DWORD value);
    // 0x000b7200
    DWORD GetHydraThrusters();
    // 0x000b7420
    float GetTrainSpeed();
    // 0x000b76c0
    DWORD GetPanelsDamage();
    // 0x000b76f0
    DWORD GetDoorsDamage();
    // 0x000b7720
    BYTE GetLightsDamage();
    // 0x000b77c0
    void SetTrailer(CVehicle* trailer);
    // 0x000b7910
    bool IsTowtruck();
    // 0x000b7c70
    bool DoesExist();
    // 0x000b7d40
    void SetField50(DWORD value);
    // 0x000b7d50
    DWORD GetField50();
    // 0x000b7d60
    void SetField54(DWORD value);
    // 0x000b7d70
    DWORD GetField54();
    // 0x000b6e20
    void SetColor(BYTE primary, BYTE secondary);
    // 0x000b6fa0
    BOOL IsTrainPart();
    // 0x000b7940
    BOOL IsRC();
    // Raw wrapper DWORD at +8B, normalized by the native query.
    // 0x000b7e60
    BOOL HasSiren();
    // Additional stage 2 wrapper dependencies; no complete allocation claim.
    // 0x000b7e70
    BYTE GetMaxPassengers();
    // 0x000b6de0
    float GetHealth();
    // 0x000b6ed0
    int GetSubtype();
    // 0x000b7cd0
    void ConstructLicensePlate();
    // 0x000b7d20
    void ShutdownLicensePlate();
};
#pragma pack(pop)
typedef char VehicleExtent[(sizeof(CVehicle) == 0xb8) ? 1 : -1];
typedef char VehicleHandleOffset[(offsetof(CVehicle, m_nHandle) == 0x44) ? 1 : -1];
typedef char VehicleTrailerOffset[(offsetof(CVehicle, m_pTrailer) == 0x48) ? 1 : -1];
typedef char VehicleBackendOffset[(offsetof(CVehicle, m_pGameVehicle) == 0x4c) ? 1 : -1];
typedef char VehicleColorOffset[(offsetof(CVehicle, m_bColorChanged) == 0x7b) ? 1 : -1];
typedef char VehicleField50Offset[(offsetof(CVehicle, m_dwField50) == 0x50) ? 1 : -1];
typedef char VehicleInvulnerableOffset[(offsetof(CVehicle, m_dwInvulnerable) == 0x58) ? 1 : -1];
typedef char VehicleLockOffset[(offsetof(CVehicle, m_dwLocked) == 0x60) ? 1 : -1];
typedef char VehicleObjectiveOffset[(offsetof(CVehicle, m_byteObjective) == 0x64) ? 1 : -1];
typedef char VehicleObjectiveMarkerOffset[(offsetof(CVehicle, m_dwObjectiveMarker) == 0x65) ? 1 : -1];
typedef char VehicleDrivenTickOffset[(offsetof(CVehicle, m_dwLastDrivenTime) == 0x69) ? 1 : -1];
typedef char VehicleModShopOffset[(offsetof(CVehicle, m_bInModShop) == 0x71) ? 1 : -1];
typedef char VehicleEngineOverrideOffset[(offsetof(CVehicle, m_dwEngineOverride75) == 0x75) ? 1 : -1];
typedef char VehicleLocalUnoccupiedOffset[(offsetof(CVehicle, m_dwLocalUnoccupiedSync) == 0x7f) ? 1 : -1];
typedef char VehicleRemoteUnoccupiedOffset[(offsetof(CVehicle, m_dwRemoteUnoccupiedSync) == 0x83) ? 1 : -1];
typedef char VehicleKeepLoadedOffset[(offsetof(CVehicle, m_dwKeepLoaded) == 0x87) ? 1 : -1];
typedef char VehicleSirenOffset[(offsetof(CVehicle, m_dwSiren) == 0x8b) ? 1 : -1];
typedef char VehiclePlateTextureOffset[(offsetof(CVehicle, m_pLicensePlateTexture) == 0x8f) ? 1 : -1];
typedef char VehiclePlateTextOffset[(offsetof(CVehicle, m_szLicensePlateText) == 0x93) ? 1 : -1];
typedef char VehicleMarkerOffset[(offsetof(CVehicle, m_dwMarker) == 0xb4) ? 1 : -1];
// 0x000b37d0
void* __stdcall GamePoolVehicleGetAt(int handle);
// GTA SA vtable classification helper; values are native 0..7 tags.
// 0x000b5d20
int GameGetVehicleSubtype(void* vehicle);
// 0x0018f5dc
extern DWORD g_vehicleDetachingTrailer;
// 0x001181a0
extern const float g_vehicleTrainSpeedMinimum;
// 0x001181a4
extern const float g_vehicleTrainSpeedMaximum;
// 0x0011812c
extern const float g_vehicleMarkerDistance;
// 0x000b3eb0
void __stdcall GameRemoveDriverByState598(void* vehicle);
// 0x002abe64
extern void* g_vehicleTrainCursor;
// 0x002abe68
extern DWORD g_vehicleLastCreatedHandle;
// 0x0011e33c
extern const float g_vehicleCreationLift;
// 0x0011f7e0
extern const char g_vehicleBadCarriageWarning[29];
// 0x0011f800
extern const char g_vehicleCreationWarning[42];
#endif

// 0x0011f704
extern const Scripting::OpcodeInfo g_vehicleLockedCommand;
// 0x0011f68c
extern const Scripting::OpcodeInfo g_vehicleWreckedCommand;
