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

    void SetObjective(BYTE objective);

    void SetTurretRotation(float first, float second);

    void SetHandbrakeOn(BOOL enabled);
    // Native backend pointer is unchecked; result is returned in ST0.

    float GetTurretVerticalRotation();
    // Native backend pointer is unchecked; result is returned in ST0.

    float GetTurretHorizontalRotation();

    void SetLocked(BOOL locked);

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

    CVehicle(int model, float x, float y, float z, float rotation, DWORD keepLoaded, BOOL siren);

    virtual ~CVehicle();

    virtual void Add();

    virtual void Remove();

    void EnableEngine(BOOL enabled);

    void ProcessMarkers();

    CVehicle* GetTrailer();

    CVehicle* GetTractor();

    void RemovePassengers();

    BOOL AddComponent(WORD component);

    void SetInvulnerable(BOOL enabled);

    void SetLandingGearState(int state);

    BOOL GetLandingGearState();

    void SetTires(BYTE tires);

    BYTE GetTires();

    void UpdateDamage(DWORD panels, DWORD doors, BYTE lights);

    BOOL IsTrailer();

    void OpenAutomobileDoor(DWORD first, DWORD second, DWORD third);

    void SetTrainSpeed(float speed);

    void SetBikeDesiredLeanAngle(float speed);

    void ChangeInterior(int interior);

    void UpdateColor();

    DWORD IsSunk();

    BOOL HasTurret();

    void SetVehicleAudioHornAndCounter(BYTE value);

    void Lock(BOOL locked);

    BOOL UpdateLastDrivenTime();

    void AttachTrailer();

    void DetachTrailer();

    void EnableLights(BYTE enabled);

    BYTE GetBackendLightsDisabledFlag();

    BOOL RemoveComponent(WORD component);

    void SetPaintjob(BYTE paintjob);

    void SetLicensePlateText(const char* text);

    void SetRotation(float angle);

    float GetBikeLeanAngle();

    void SetWindowOpenFlag(BYTE window);

    void ClearWindowOpenFlag(BYTE window);

    void ResetPointers();

    BOOL HasDriver();

    BOOL IsOccupied();

    void SetHealth(float health);

    BOOL DriverIsPlayerPed();

    BOOL HasPlayerPed();

    void EnableSiren(BYTE enabled);

    BOOL SirenEnabled();

    void SetAlarmState(WORD value);

    void SetHydraThrusters(DWORD value);

    DWORD GetHydraThrusters();

    float GetTrainSpeed();

    DWORD GetPanelsDamage();

    DWORD GetDoorsDamage();

    BYTE GetLightsDamage();

    void SetTrailer(CVehicle* trailer);

    BOOL IsTowtruck();

    bool DoesExist();

    void SetEngineState(DWORD value);

    DWORD GetEngineState();

    void SetLightsState(DWORD value);

    DWORD GetLightsState();

    void SetColor(int primary, int secondary);

    BOOL IsTrainPart();

    BOOL IsRC();
    // Raw wrapper DWORD at +8B, normalized by the native query.

    BOOL HasSiren();
    // Additional stage 2 wrapper dependencies; no complete allocation claim.

    BYTE GetMaxPassengers();

    float GetHealth();

    int GetSubtype();

    void ConstructLicensePlate();

    void ShutdownLicensePlate();
};
#pragma pack(pop)
static_assert(sizeof(CVehicle) == 0xb8);
static_assert(offsetof(CVehicle, m_nHandle) == 0x44);
static_assert(offsetof(CVehicle, m_pTrailer) == 0x48);
static_assert(offsetof(CVehicle, m_pGameVehicle) == 0x4c);
static_assert(offsetof(CVehicle, m_bColorChanged) == 0x7b);
static_assert(offsetof(CVehicle, m_dwField50) == 0x50);
static_assert(offsetof(CVehicle, m_dwInvulnerable) == 0x58);
static_assert(offsetof(CVehicle, m_dwLocked) == 0x60);
static_assert(offsetof(CVehicle, m_byteObjective) == 0x64);
static_assert(offsetof(CVehicle, m_dwObjectiveMarker) == 0x65);
static_assert(offsetof(CVehicle, m_dwLastDrivenTime) == 0x69);
static_assert(offsetof(CVehicle, m_bInModShop) == 0x71);
static_assert(offsetof(CVehicle, m_dwEngineOverride75) == 0x75);
static_assert(offsetof(CVehicle, m_dwLocalUnoccupiedSync) == 0x7f);
static_assert(offsetof(CVehicle, m_dwRemoteUnoccupiedSync) == 0x83);
static_assert(offsetof(CVehicle, m_dwKeepLoaded) == 0x87);
static_assert(offsetof(CVehicle, m_dwSiren) == 0x8b);
static_assert(offsetof(CVehicle, m_pLicensePlateTexture) == 0x8f);
static_assert(offsetof(CVehicle, m_szLicensePlateText) == 0x93);
static_assert(offsetof(CVehicle, m_dwMarker) == 0xb4);

void* __stdcall GamePoolVehicleGetAt(int handle);
// GTA SA vtable classification helper; values are native 0..7 tags.

int GameGetVehicleSubtype(void* vehicle);

extern DWORD g_vehicleDetachingTrailer;

extern const float g_vehicleTrainSpeedMinimum;

extern const float g_vehicleTrainSpeedMaximum;

extern const float g_vehicleMarkerDistance;

void __stdcall GameRemoveDriverByState598(void* vehicle);

extern void* g_vehicleTrainCursor;

extern DWORD g_vehicleLastCreatedHandle;

extern const float g_vehicleCreationLift;

extern const char g_vehicleBadCarriageWarning[29];

extern const char g_vehicleCreationWarning[42];
#endif

extern const Scripting::OpcodeInfo g_vehicleLockedCommand;

extern const Scripting::OpcodeInfo g_vehicleWreckedCommand;
