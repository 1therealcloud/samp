#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H

#include "game/quaternion.h"
#include "game/scripting.h"

#include <stddef.h>
#include <windows.h>

#pragma pack(push, 1)
struct LocalOnfootSyncData
{
    WORD leftRight, upDown, keys;
    CVector position;
    GameQuaternion rotation;
    BYTE health, armour, weaponAim, specialAction;
    CVector speed, surfingOffset;
    WORD surfingVehicle;
    DWORD animation;
};
struct LocalIncarSyncData
{
    WORD vehicle, leftRight, upDown, keys;
    GameQuaternion rotation;
    CVector position, speed;
    float health;
    BYTE pedHealth, armour, weaponAim, siren, landingGear;
    WORD trailer;
    DWORD modelData;
};
#pragma pack(pop)
static_assert(sizeof(LocalOnfootSyncData) == 68);
static_assert(sizeof(LocalIncarSyncData) == 63);

#pragma pack(push, 1)
struct LocalAimSyncData
{
    BYTE cameraMode;
    BYTE vectors[24];
    float aimZ;
    BYTE zoomWeapon, aspect;
};
struct LocalTrailerSyncData
{
    WORD vehicle;
    CVector position;
    GameQuaternion rotation;
    CVector speed, turnSpeed;
};
struct LocalPassengerSyncData
{
    WORD vehicle;
    BYTE seatFlags, weaponAim, health, armour;
    WORD leftRight, upDown, keys;
    CVector position;
};
#pragma pack(pop)
static_assert(sizeof(LocalAimSyncData) == 31);
static_assert(sizeof(LocalTrailerSyncData) == 54);
static_assert(sizeof(LocalPassengerSyncData) == 24);

#pragma pack(push, 1)
struct UnoccupiedSyncData
{
    WORD vehicle;
    BYTE seat;
    CVector right, up, position, speed, turnSpeed;
    float health;
};
#pragma pack(pop)
static_assert(sizeof(UnoccupiedSyncData) == 67);

#pragma pack(push, 1)
struct BulletSyncData
{
    BYTE type;
    WORD targetId;
    CVector origin, target, offset;
    BYTE weapon;
};
#pragma pack(pop)
static_assert(sizeof(BulletSyncData) == 40);
static_assert(offsetof(LocalOnfootSyncData, position) == 6);
static_assert(offsetof(LocalOnfootSyncData, rotation) == 0x12);
static_assert(offsetof(LocalOnfootSyncData, health) == 0x22);
static_assert(offsetof(LocalOnfootSyncData, speed) == 0x26);
static_assert(offsetof(LocalOnfootSyncData, surfingOffset) == 0x32);
static_assert(offsetof(LocalOnfootSyncData, animation) == 0x40);
static_assert(offsetof(LocalIncarSyncData, rotation) == 8);
static_assert(offsetof(LocalIncarSyncData, position) == 0x18);
static_assert(offsetof(LocalIncarSyncData, speed) == 0x24);
static_assert(offsetof(LocalIncarSyncData, health) == 0x30);
static_assert(offsetof(LocalIncarSyncData, pedHealth) == 0x34);
static_assert(offsetof(LocalIncarSyncData, trailer) == 0x39);
static_assert(offsetof(LocalIncarSyncData, modelData) == 0x3b);
static_assert(offsetof(LocalAimSyncData, aimZ) == 25);
static_assert(offsetof(LocalAimSyncData, zoomWeapon) == 29);
static_assert(offsetof(LocalTrailerSyncData, rotation) == 14);
static_assert(offsetof(LocalTrailerSyncData, speed) == 30);
static_assert(offsetof(LocalPassengerSyncData, keys) == 10);
static_assert(offsetof(LocalPassengerSyncData, position) == 12);
static_assert(offsetof(UnoccupiedSyncData, position) == 27);
static_assert(offsetof(UnoccupiedSyncData, health) == 63);
static_assert(offsetof(BulletSyncData, targetId) == 1);
static_assert(offsetof(BulletSyncData, origin) == 3);
static_assert(offsetof(BulletSyncData, weapon) == 39);

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

enum
{
#if defined(DL_BUILD)
    LocalSpawnInfoWireSize = sizeof(LocalSpawnInfo)
#else  // DL_BUILD
    LocalSpawnInfoWireSize = sizeof(LocalSpawnInfo) - sizeof(int)
#endif // DL_BUILD
};

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
    // 0x00017770
    void ReturnToClassSelection();
    // Raw DWORD flag used while the local ped is removed for RC driving.
    // 0x0009e940
    DWORD GetRCModeState();
    // Returns the raw DWORD, without bool normalization.
    // 0x00008170
    DWORD GetActiveState();
    // 0x0000e2a0
    CEntity* GetSurfingEntity();
    // Clears only the entity and mode, retaining lock and offset state.
    // 0x00017760
    void ClearSurfingEntityAndMode();
    // 0x00017780
    void SetTeam(BYTE team);
    // 0x000a21d0
    BYTE GetTeam();
    // 0x00004230
    BYTE GetAimTargetOffset(WORD* id, CVector* offset);

    // 0x00002db0
    DWORD GetSpawnPermission();
    // 0x000135c0
    DWORD IsSpectating();

    union
    {
        LocalPlayerState m_state;
        BYTE nativeState[0x328];
    };
    // 0x000072d0
    BOOL Process();
    // 0x00002fd0
    void ProcessHead();
    // 0x00003e40
    void ProcessWorldBoundaries();
    // 0x00003ea0
    void ProcessVehicleWorldBoundaries();
    // 0x00003f60
    void PrepareForClassSelection();
    // 0x00005c20
    void ProcessVehicleDamage(WORD vehicle);
    // 0x00006de0
    void ProcessPassengerEntry();
    // 0x00004a30
    WORD GetAimedPlayer();
    // 0x00004a60
    WORD GetAimedActor();
    // 0x00005f40
    void ProcessClassSelection();
    // 0x000060d0
    void UpdateWeapons();
    // 0x00006360
    void ProcessSpectating();
    // Native returns true in AL; this pointer is unused.
    // 0x00004300
    bool GetCameraTarget(WORD* targets);
    // 0x000045c0
    void UpdateCameraTarget();
    // 0x00004710
    void DrawCameraTargetLabels();
    // 0x00003390
    BYTE GetSpecialAction();
    // 0x00004db0
    void SendOnfootData();
    // 0x00006e80
    void SendIncarData();
    // 0x000041c0
    DWORD NeedsToSendOnfootData(WORD keys, WORD upDown, WORD leftRight);
    // 0x000041f0
    DWORD NeedsToSendIncarData(WORD keys, WORD upDown, WORD leftRight);
    // 0x00005090
    void SendAimData();
    // 0x00005240
    void SendTrailerData(WORD vehicle);
    // 0x00005400
    void SendPassengerData();
    // 0x00005660
    void WastedNotification();
    // 0x00005b50
    void SendStats();
    // 0x00004bd0
    void SendUnoccupiedData(WORD vehicle, BYTE seat);
    // Native bool result in AL; deliberately retains player0 selection behavior.
    // 0x00006c10
    bool ProcessUnoccupiedSync(WORD id, CVehicle* vehicle);
    // 0x0000e2b0
    void ClearSurfingState();
    // 0x00003fb0
    void PrepareForClassSelection_Outcome(BOOL outcome);
    // Copies the raw50-byte spawn record forward and marks it available.
    // 0x00003af0
    void SetSpawnInfo(const void* info);
    // 0x00004af0
    CLocalPlayer();
    // 0x00002ec0
    void ResetData();
    // 0x00003110
    void SetSpecialAction(BYTE action);
    // 0x00003b20
    BOOL Spawn();
    // Returns the raw DWORD state.
    // 0x00004080
    void ToggleSpectating(BOOL enabled);
    // 0x000040f0
    void SpectateVehicle(WORD vehicle);
    // 0x00004140
    void SpectatePlayer(WORD player);
    // 0x00003de0
    void SetColor(DWORD color);
    // 0x00003e00
    DWORD GetColorAsRGBA();
    // Raw DWORD result: returns1 after500ms or when any byte differs.
    // 0x00003970
    DWORD HasDataChanged(const void* previous, const void* current, DWORD length);
    // 0x00003a70
    void SendSpawnRequest();
    // 0x00005720
    void RequestClass(int selection);
    // 0x000057c0
    void SendInteriorChange(BYTE interior);
    // 0x00003e20
    DWORD GetColorAsARGB();
    // 0x000039c0
    DWORD GetInCarSendRate();
    // 0x00003a00
    DWORD GetOnFootSendRate();
    // 0x00003a40
    DWORD GetSendRate();
    // 0x000034b0
    void ApplySurfingPhysics();
    // 0x00003630
    void SetSurfingEntity(CEntity* entity, DWORD locked);
    // May transform data->offset in place for an unclassified backend target.
    // 0x000069d0
    void SendBullet(PedBulletData* data);
    // 0x00002da0
    CPed* GetPed();
    // Damage amount is a native float; take/give apply scaling before serialization.
    // 0x00005920
    void EnterVehicle(WORD vehicle, BOOL passenger);
    // 0x000066b0
    void SendTakeDamage(WORD player, float damage, DWORD weapon, DWORD bodyPart);
    // 0x000067c0
    void SendGiveDamage(WORD player, float damage, DWORD weapon, DWORD bodyPart);
    // 0x000068d0
    void SendActorDamage(WORD actor, float damage, DWORD weapon, DWORD bodyPart);
    // 0x00005a40
    void ExitVehicle(int vehicle);
    // Network/pool dependency used only when Move finds a player pool.
    // 0x00003650
    void ProcessSurfing();
    // 0x00005860
    void Chat(char* text);
    // 0x00003f40
    void RequestSpawn();
    // 0x00005e30
    void NextClass();
    // 0x00005ec0
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
// 0x00117978
extern const float g_localUnoccupiedRange;
// 0x00117948
extern const float g_localAimZoomScale;
// 0x0011794c
extern const float g_localAimAspectScale;
// 0x00117950
extern int g_localWastedRpc;
// 0x00117968
extern int g_localVehicleDamageRpc;
// 0x0011797c
extern const float g_localPassengerEntryDistance;
// 0x00117888
extern const char g_localWorldBoundaryText[];
// 0x00117854
extern const Scripting::OpcodeInfo g_localPedCameraCommand;
// 0x001178ac
extern const float g_localCameraRayLength;
// 0x001178b0
extern int g_localCameraTargetRpc;
// 0x00117920
extern const char g_localCameraTargetObjectText[];
// 0x001178fc
extern const char g_localCameraTargetVehicleText[];
// 0x001178d8
extern const char g_localCameraTargetPlayerText[];
// 0x001178b4
extern const char g_localCameraTargetActorText[];
// 0x0013c0b0
extern DWORD g_localAimSendRate;
// 0x00117980
extern const char g_localClassSelectionMessage[];
// 0x00117944
extern const float g_localQuaternionSyncThreshold;
// 0x0011787c
extern int g_localSpawnRequestRpc;
// 0x00117880
extern int g_localSpawnRpc;
// Skips one SetInitialState call when nonzero.
// 0x0013c0a0
extern DWORD g_localSkipInitialState;
// 0x00117954
extern int g_localRequestClassRpc;
// 0x00117958
extern int g_localInteriorChangeRpc;
// 0x0011795c
extern int g_localChatRpc;
// 0x00117960
extern int g_localEnterVehicleRpc;
// 0x00117964
extern int g_localExitVehicleRpc;
// 0x0011796c
extern int g_localPlayerDamageRpc;
// 0x00117974
extern int g_localActorDamageRpc;
// 0x00117970
extern const float g_localDamageScale;
// 0x00117840
extern const Scripting::OpcodeInfo g_localTrainCameraCommand;
// 0x0013c0a4
extern DWORD g_localTrainCameraTick;

struct CVector;
class CMatrix;
// 0x001569a8
extern CVector g_localSurfPedTurnSpeed;
// 0x001569b8
extern CMatrix g_localSurfPedMatrix;
// 0x001569f8
extern CVector g_localSurfEntitySpeed;
// 0x00156a08
extern CMatrix g_localSurfInverseMatrix;
// 0x00156a48
extern CVector g_localSurfPedSpeed;
// 0x00156a58
extern CMatrix g_localSurfEntityMatrix;
// 0x00156aa4
extern CVector g_localSurfEntityTurnSpeed;

// 0x0013c0a8
extern DWORD g_localOnFootSendRate;
// 0x0013c0ac
extern DWORD g_localInCarSendRate;

#endif // LOCALPLAYER_H
