#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H

#include "game/quaternion.h"
#include "game/scripting.h"
#include "main.h"
#include "synchronization.h"

#include <stddef.h>
#include <windows.h>

class CPed;
class CEntity;
class CVehicle;
struct CVector;
struct PedBulletData;
namespace Scripting
{
struct OpcodeInfo;
}
#pragma pack(push, 1)
struct LocalSpawnInfo
{
    BYTE team;
    int model, customModel;
    BYTE unknown09;
    CVector position;
    float rotation;
    int weapons[3], ammo[3];
};

inline DWORD GetLocalSpawnInfoWireSize()
{
    return sizeof(LocalSpawnInfo) - (g_DLClient ? 0 : sizeof(int));
}

struct LocalPlayerState
{
    CPed* ped;
    LocalTrailerSyncData trailer;
    LocalOnfootSyncData onfoot;
    LocalPassengerSyncData passenger;
    LocalIncarSyncData incar;
    LocalAimSyncData aim;
    DWORD active, wasted;
    WORD currentVehicle, lastVehicle;
    DWORD animation, animationSent;
    WORD cameraTargets[4];
    DWORD cameraTargetTick;
    CVector headDirection;
    DWORD headUpdateTick, headTaskTick, lastChangedDataTick;
    DWORD spectating;
    BYTE team;
    WORD unknown131;
    DWORD lastSendTick, lastSpectatorSendTick, lastAimSendTick, lastStatsSendTick;
    DWORD spawnPermission, classSelectionTick, initialSelectionTick;
    LocalSpawnInfo spawnInfo;
    DWORD hasSpawnInfo;
    DWORD weaponsUpdateTick;
    WORD aimedPlayer, aimedActor;
    BYTE currentWeapon, lastWeapons[13];
    DWORD lastAmmo[13];
    DWORD passengerDriveby;
    BYTE interior;
    DWORD rcMode;
    BYTE unknown1d8[0x100];
    WORD surfingId;
    DWORD surfingTick;
    CEntity* surfingEntity;
    DWORD surfingLocked, surfingMode;
    CVector surfingOffset;
    DWORD unknown2f6, surfingContactState;
    DWORD wantsAnotherClass;
    int selectedClass;
    DWORD waitingForSpawnReply, classSelectionActive, zoneDisplayTick;
    BYTE spectateCameraMode, spectateType;
    DWORD spectateId, spectateProcessed;
    WORD damageVehicle;
    DWORD panelsDamage, doorsDamage;
    BYTE lightsDamage, tiresDamage;
};
class CLocalPlayer
{
public:
    // Requests next class selection; does not spawn or change active state.

    void ReturnToClassSelection();
    // Raw DWORD flag used while the local ped is removed for RC driving.

    DWORD GetRCModeState();
    // Returns the raw DWORD, without bool normalization.

    DWORD GetActiveState();

    CEntity* GetSurfingEntity();
    // Clears only the entity and mode, retaining lock and offset state.

    void ClearSurfingEntityAndMode();

    void SetTeam(BYTE team);

    BYTE GetTeam();

    BYTE GetAimTargetOffset(WORD* id, CVector* offset);

    DWORD GetSpawnPermission();

    DWORD IsSpectating();

    union
    {
        LocalPlayerState m_state;
        BYTE nativeState[0x328];
    };

    BOOL Process();

    void ProcessHead();

    void ProcessWorldBoundaries();

    void ProcessVehicleWorldBoundaries();

    void PrepareForClassSelection();

    void ProcessVehicleDamage(WORD vehicle);

    void ProcessPassengerEntry();

    WORD GetAimedPlayer();

    WORD GetAimedActor();

    void ProcessClassSelection();

    void UpdateWeapons();

    void ProcessSpectating();
    // Native returns true in AL; this pointer is unused.

    bool GetCameraTarget(WORD* targets);

    void UpdateCameraTarget();

    void DrawCameraTargetLabels();

    BYTE GetSpecialAction();

    void SendOnfootData();

    void SendIncarData();

    DWORD NeedsToSendOnfootData(WORD keys, WORD upDown, WORD leftRight);

    DWORD NeedsToSendIncarData(WORD keys, WORD upDown, WORD leftRight);

    void SendAimData();

    void SendTrailerData(WORD vehicle);

    void SendPassengerData();

    void WastedNotification();

    void SendStats();

    void SendUnoccupiedData(WORD vehicle, BYTE seat);
    // Native bool result in AL; deliberately retains player0 selection behavior.

    bool ProcessUnoccupiedSync(WORD id, CVehicle* vehicle);

    void ClearSurfingState();

    void PrepareForClassSelection_Outcome(BOOL outcome);
    // Copies the raw50-byte spawn record forward and marks it available.

    void SetSpawnInfo(const void* info);

    CLocalPlayer();

    void ResetData();

    void SetSpecialAction(BYTE action);

    BOOL Spawn();
    // Returns the raw DWORD state.

    void ToggleSpectating(BOOL enabled);

    void SpectateVehicle(WORD vehicle);

    void SpectatePlayer(WORD player);

    void SetColor(DWORD color);

    DWORD GetColorAsRGBA();
    // Raw DWORD result: returns1 after500ms or when any byte differs.

    DWORD HasDataChanged(const void* previous, const void* current, DWORD length);

    void SendSpawnRequest();

    void RequestClass(int selection);

    void SendInteriorChange(BYTE interior);

    DWORD GetColorAsARGB();

    DWORD GetInCarSendRate();

    DWORD GetOnFootSendRate();

    DWORD GetSendRate();

    void ApplySurfingPhysics();

    void SetSurfingEntity(CEntity* entity, DWORD locked);
    // May transform data->offset in place for an unclassified backend target.

    void SendBullet(PedBulletData* data);

    CPed* GetPed();
    // Damage amount is a native float; take/give apply scaling before serialization.

    void EnterVehicle(WORD vehicle, BOOL passenger);

    void SendTakeDamage(WORD player, float damage, DWORD weapon, DWORD bodyPart);

    void SendGiveDamage(WORD player, float damage, DWORD weapon, DWORD bodyPart);

    void SendActorDamage(WORD actor, float damage, DWORD weapon, DWORD bodyPart);

    void ExitVehicle(int vehicle);
    // Network/pool dependency used only when Move finds a player pool.

    void ProcessSurfing();

    void Chat(char* text);

    void RequestSpawn();

    void NextClass();

    void PrevClass();
};
#pragma pack(pop)
static_assert(sizeof(LocalSpawnInfo) == 50);
static_assert(offsetof(LocalSpawnInfo, position) == 10);
static_assert(offsetof(LocalSpawnInfo, weapons) == 26);
static_assert(offsetof(LocalSpawnInfo, ammo) == 38);
static_assert(offsetof(LocalPlayerState, ped) == 0x0);
static_assert(offsetof(LocalPlayerState, trailer) == 0x4);
static_assert(offsetof(LocalPlayerState, onfoot) == 0x3a);
static_assert(offsetof(LocalPlayerState, passenger) == 0x7e);
static_assert(offsetof(LocalPlayerState, incar) == 0x96);
static_assert(offsetof(LocalPlayerState, aim) == 0xd5);
static_assert(offsetof(LocalPlayerState, active) == 0xf4);
static_assert(offsetof(LocalPlayerState, wasted) == 0xf8);
static_assert(offsetof(LocalPlayerState, currentVehicle) == 0xfc);
static_assert(offsetof(LocalPlayerState, cameraTargets) == 0x108);
static_assert(offsetof(LocalPlayerState, headDirection) == 0x114);
static_assert(offsetof(LocalPlayerState, headUpdateTick) == 0x120);
static_assert(offsetof(LocalPlayerState, spectating) == 0x12c);
static_assert(offsetof(LocalPlayerState, team) == 0x130);
static_assert(offsetof(LocalPlayerState, unknown131) == 0x131);
static_assert(offsetof(LocalPlayerState, lastSendTick) == 0x133);
static_assert(offsetof(LocalPlayerState, spawnPermission) == 0x143);
static_assert(offsetof(LocalPlayerState, classSelectionTick) == 0x147);
static_assert(offsetof(LocalPlayerState, initialSelectionTick) == 0x14b);
static_assert(offsetof(LocalPlayerState, spawnInfo) == 0x14f);
static_assert(offsetof(LocalPlayerState, hasSpawnInfo) == 0x181);
static_assert(offsetof(LocalPlayerState, weaponsUpdateTick) == 0x185);
static_assert(offsetof(LocalPlayerState, aimedPlayer) == 0x189);
static_assert(offsetof(LocalPlayerState, lastWeapons) == 0x18e);
static_assert(offsetof(LocalPlayerState, lastAmmo) == 0x19b);
static_assert(offsetof(LocalPlayerState, passengerDriveby) == 0x1cf);
static_assert(offsetof(LocalPlayerState, interior) == 0x1d3);
static_assert(offsetof(LocalPlayerState, rcMode) == 0x1d4);
static_assert(offsetof(LocalPlayerState, unknown1d8) == 0x1d8);
static_assert(offsetof(LocalPlayerState, surfingId) == 0x2d8);
static_assert(offsetof(LocalPlayerState, surfingTick) == 0x2da);
static_assert(offsetof(LocalPlayerState, surfingEntity) == 0x2de);
static_assert(offsetof(LocalPlayerState, surfingLocked) == 0x2e2);
static_assert(offsetof(LocalPlayerState, surfingMode) == 0x2e6);
static_assert(offsetof(LocalPlayerState, surfingOffset) == 0x2ea);
static_assert(offsetof(LocalPlayerState, unknown2f6) == 0x2f6);
static_assert(offsetof(LocalPlayerState, surfingContactState) == 0x2fa);
static_assert(offsetof(LocalPlayerState, wantsAnotherClass) == 0x2fe);
static_assert(offsetof(LocalPlayerState, selectedClass) == 0x302);
static_assert(offsetof(LocalPlayerState, waitingForSpawnReply) == 0x306);
static_assert(offsetof(LocalPlayerState, classSelectionActive) == 0x30a);
static_assert(offsetof(LocalPlayerState, zoneDisplayTick) == 0x30e);
static_assert(offsetof(LocalPlayerState, spectateCameraMode) == 0x312);
static_assert(offsetof(LocalPlayerState, spectateType) == 0x313);
static_assert(offsetof(LocalPlayerState, spectateId) == 0x314);
static_assert(offsetof(LocalPlayerState, spectateProcessed) == 0x318);
static_assert(offsetof(LocalPlayerState, damageVehicle) == 0x31c);
static_assert(offsetof(LocalPlayerState, panelsDamage) == 0x31e);
static_assert(offsetof(LocalPlayerState, doorsDamage) == 0x322);
static_assert(offsetof(LocalPlayerState, lightsDamage) == 0x326);
static_assert(offsetof(LocalPlayerState, tiresDamage) == 0x327);
static_assert(sizeof(LocalPlayerState) == 0x328);
static_assert(sizeof(CLocalPlayer) == 0x328);

extern const float g_localUnoccupiedRange;

extern const float g_localAimZoomScale;

extern const float g_localAimAspectScale;

extern int g_localWastedRpc;

extern int g_localVehicleDamageRpc;

extern const float g_localPassengerEntryDistance;

extern const char g_localWorldBoundaryText[];

extern const Scripting::OpcodeInfo g_localPedCameraCommand;

extern const float g_localCameraRayLength;

extern int g_localCameraTargetRpc;

extern const char g_localCameraTargetObjectText[];

extern const char g_localCameraTargetVehicleText[];

extern const char g_localCameraTargetPlayerText[];

extern const char g_localCameraTargetActorText[];

extern DWORD g_localAimSendRate;

extern const char g_localClassSelectionMessage[];

extern const float g_localQuaternionSyncThreshold;

extern int g_localSpawnRequestRpc;

extern int g_localSpawnRpc;
// Skips one SetInitialState call when nonzero.

extern DWORD g_localSkipInitialState;

extern int g_localRequestClassRpc;

extern int g_localInteriorChangeRpc;

extern int g_localChatRpc;

extern int g_localEnterVehicleRpc;

extern int g_localExitVehicleRpc;

extern int g_localPlayerDamageRpc;

extern int g_localActorDamageRpc;

extern const float g_localDamageScale;

extern const Scripting::OpcodeInfo g_localTrainCameraCommand;

extern DWORD g_localTrainCameraTick;

struct CVector;
class CMatrix;

extern CVector g_localSurfPedTurnSpeed;

extern CMatrix g_localSurfPedMatrix;

extern CVector g_localSurfEntitySpeed;

extern CMatrix g_localSurfInverseMatrix;

extern CVector g_localSurfPedSpeed;

extern CMatrix g_localSurfEntityMatrix;

extern CVector g_localSurfEntityTurnSpeed;

extern DWORD g_localOnFootSendRate;

extern DWORD g_localInCarSendRate;

#endif // LOCALPLAYER_H
