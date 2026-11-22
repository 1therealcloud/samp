#ifndef SAMP_RECOVERED_VEHICLE_H
#define SAMP_RECOVERED_VEHICLE_H
#include "game/entity.h"
#include "game/scripting.h"

#include <stddef.h>

#include <static_assert.h>

struct IDirect3DTexture9;
// Native allocation extent B8 and CEntity prefix/three-slot virtual interface.
// Opaque bytes retain unknown meaning; lifecycle follows the recovered DL bodies.
#pragma pack(push, 1)
class CVehicle : public CEntity
{
public:
    // Stores the raw byte; does not reset/create the objective blip.
    // R5 address to verify
    void SetObjective(BYTE objective);
    // R5 address to verify
    void SetBackendFloat94CAnd950(float first, float second);

    // R5 address to verify
    void SetBackendFlag428Bit5(BOOL enabled);
    // Native backend pointer is unchecked; result is returned in ST0.
    // R5 address to verify
    float GetBackendFloat94C();
    // Native backend pointer is unchecked; result is returned in ST0.
    // R5 address to verify
    float GetBackendFloat950();
    // 0x000b7230
    void SetLocked(BOOL locked);
    // 0x000b73d0
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
    // 0x000b83d0
    CVehicle(int model, float x, float y, float z, float rotation, DWORD keepLoaded, BOOL siren);
    // 0x000b88f0
    virtual ~CVehicle();
    // 0x000b8a10
    virtual void Add();
    // 0x000b8a40
    virtual void Remove();
    // 0x000b8a70
    void EnableEngine(BOOL enabled);
    // 0x000b76e0
    void ProcessMarkers();
    // 0x000b7c90
    CVehicle* GetTrailer();
    // 0x000b7cf0
    CVehicle* GetTractor();
    // 0x000b7ee0
    void RemovePassengers();
    // 0x000b7fc0
    BOOL AddComponent(WORD component);
    // 0x000b7190
    void SetInvulnerable(BOOL enabled);
    // 0x000b75a0
    void SetLandingGearState(int state);
    // 0x000b7630
    BOOL GetLandingGearState();
    // 0x000b7940
    void SetTires(BYTE tires);
    // 0x000b7a30
    BYTE GetTires();
    // 0x000b7ac0
    void UpdateDamage(DWORD panels, DWORD doors, BYTE lights);
    // 0x000b7d70
    BOOL IsTrailer();
    // 0x000b8250
    void CallBackend6a6ae0(DWORD first, DWORD second, DWORD third);
    // 0x000b7900
    void SetTrainSpeed(float speed);
    // 0x000b82e0
    void SetBackendFloat64c(float speed);
    // 0x000b7090
    void ChangeInterior(int interior);
    // 0x000b7330
    void UpdateColor();
    // 0x000b73b0
    DWORD IsSunk();
    // 0x000b74a0
    BOOL HasTurret();
    // 0x000b74d0
    void SetBackendBytes1f6And514(BYTE value);
    // 0x000b7840
    void Lock(BOOL locked);
    // 0x000b7870
    BOOL UpdateLastDrivenTime();
    // 0x000b7c10
    void AttachTrailer();
    // 0x000b7c30
    void DetachTrailer();
    // 0x000b7e50
    void EnableLights(BYTE enabled);
    // 0x000b7eb0
    BYTE GetBackendLightsDisabledFlag();
    // 0x000b80b0
    BOOL RemoveComponent(WORD component);
    // 0x000b80f0
    void SetPaintjob(BYTE paintjob);
    // 0x000b8150
    void SetLicensePlateText(const char* text);
    // 0x000b8170
    void SetRotation(float angle);
    // 0x000b82b0
    float GetBackendFloat648();
    // 0x000b8370
    void SetWindowOpenFlag(BYTE window);
    // 0x000b83a0
    void ClearWindowOpenFlag(BYTE window);
    // 0x000b70c0
    void ResetPointers();
    // 0x000b70e0
    BOOL HasDriver();
    // 0x000b7130
    BOOL IsOccupied();
    // 0x000b72c0
    void SetHealth(float health);
    // 0x000b73f0
    BOOL DriverIsPlayerPed();
    // 0x000b7420
    BOOL HasPlayerPed();
    // 0x000b7540
    void EnableSiren(BYTE enabled);
    // 0x000b7560
    BOOL SirenEnabled();
    // 0x000b7580
    void SetBackendWord45c(WORD value);
    // 0x000b76a0
    void SetHydraThrusters(DWORD value);
    // 0x000b76c0
    DWORD GetHydraThrusters();
    // 0x000b78e0
    float GetTrainSpeed();
    // 0x000b7b80
    DWORD GetPanelsDamage();
    // 0x000b7bb0
    DWORD GetDoorsDamage();
    // 0x000b7be0
    BYTE GetLightsDamage();
    // 0x000b7c80
    void SetTrailer(CVehicle* trailer);
    // 0x000b7dd0
    BOOL IsTowtruck();
    // 0x000b8140
    bool DoesExist();
    // R5 address to verify
    void SetEngineState(DWORD value);
    // R5 address to verify
    DWORD GetEngineState();
    // R5 address to verify
    void SetLightsState(DWORD value);
    // R5 address to verify
    DWORD GetLightsState();
    // 0x000b72e0
    void SetColor(int primary, int secondary);
    // 0x000b7460
    BOOL IsTrainPart();
    // 0x000b7e00
    BOOL IsRC();
    // Raw wrapper DWORD at +8B, normalized by the native query.
    // 0x000b8330
    BOOL HasSiren();
    // Additional stage 2 wrapper dependencies; no complete allocation claim.
    // 0x000b8340
    BYTE GetMaxPassengers();
    // 0x000b72a0
    float GetHealth();
    // 0x000b7390
    int GetSubtype();
    // 0x000b81a0
    void ConstructLicensePlate();
    // 0x000b81f0
    void ShutdownLicensePlate();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CVehicle) == 0xb8);
STATIC_ASSERT(offsetof(CVehicle, m_nHandle) == 0x44);
STATIC_ASSERT(offsetof(CVehicle, m_pTrailer) == 0x48);
STATIC_ASSERT(offsetof(CVehicle, m_pGameVehicle) == 0x4c);
STATIC_ASSERT(offsetof(CVehicle, m_bColorChanged) == 0x7b);
STATIC_ASSERT(offsetof(CVehicle, m_dwField50) == 0x50);
STATIC_ASSERT(offsetof(CVehicle, m_dwInvulnerable) == 0x58);
STATIC_ASSERT(offsetof(CVehicle, m_dwLocked) == 0x60);
STATIC_ASSERT(offsetof(CVehicle, m_byteObjective) == 0x64);
STATIC_ASSERT(offsetof(CVehicle, m_dwObjectiveMarker) == 0x65);
STATIC_ASSERT(offsetof(CVehicle, m_dwLastDrivenTime) == 0x69);
STATIC_ASSERT(offsetof(CVehicle, m_bInModShop) == 0x71);
STATIC_ASSERT(offsetof(CVehicle, m_dwEngineOverride75) == 0x75);
STATIC_ASSERT(offsetof(CVehicle, m_dwLocalUnoccupiedSync) == 0x7f);
STATIC_ASSERT(offsetof(CVehicle, m_dwRemoteUnoccupiedSync) == 0x83);
STATIC_ASSERT(offsetof(CVehicle, m_dwKeepLoaded) == 0x87);
STATIC_ASSERT(offsetof(CVehicle, m_dwSiren) == 0x8b);
STATIC_ASSERT(offsetof(CVehicle, m_pLicensePlateTexture) == 0x8f);
STATIC_ASSERT(offsetof(CVehicle, m_szLicensePlateText) == 0x93);
STATIC_ASSERT(offsetof(CVehicle, m_dwMarker) == 0xb4);
// 0x000b3bd0
void* __stdcall GamePoolVehicleGetAt(int handle);
// GTA SA vtable classification helper; values are native 0..7 tags.
// 0x000b61e0
int GameGetVehicleSubtype(void* vehicle);
// R5 address to verify
extern DWORD g_vehicleDetachingTrailer;
// 0x000e6110
extern const float g_vehicleTrainSpeedMinimum;
// 0x000e6114
extern const float g_vehicleTrainSpeedMaximum;
// 0x000e60a0
extern const float g_vehicleMarkerDistance;
// 0x000b42b0
void __stdcall GameRemoveDriverByState598(void* vehicle);
// R5 address to verify
extern void* g_vehicleTrainCursor;
// R5 address to verify
extern DWORD g_vehicleLastCreatedHandle;
// 0x000ec314
extern const float g_vehicleCreationLift;
// 0x000ed6dc
extern const char g_vehicleBadCarriageWarning[29];
// 0x000ed6fc
extern const char g_vehicleCreationWarning[42];
#endif

// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleLockedCommand;
// R5 address to verify
extern const Scripting::OpcodeInfo g_vehicleWreckedCommand;
