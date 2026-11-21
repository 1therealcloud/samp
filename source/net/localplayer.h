#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H

#include "game/quaternion.h"
#include "game/scripting.h"

#include <stddef.h>
#include <windows.h>

#include <static_assert.h>

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
STATIC_ASSERT(sizeof(LocalOnfootSyncData) == 68);
STATIC_ASSERT(sizeof(LocalIncarSyncData) == 63);

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
STATIC_ASSERT(sizeof(LocalAimSyncData) == 31);
STATIC_ASSERT(sizeof(LocalTrailerSyncData) == 54);
STATIC_ASSERT(sizeof(LocalPassengerSyncData) == 24);

#pragma pack(push, 1)
struct UnoccupiedSyncData
{
    WORD vehicle;
    BYTE seat;
    CVector right, up, position, speed, turnSpeed;
    float health;
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(UnoccupiedSyncData) == 67);

#pragma pack(push, 1)
struct BulletSyncData
{
    BYTE type;
    WORD targetId;
    CVector origin, target, offset;
    BYTE weapon;
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(BulletSyncData) == 40);
STATIC_ASSERT(offsetof(LocalOnfootSyncData, position) == 6);
STATIC_ASSERT(offsetof(LocalOnfootSyncData, rotation) == 0x12);
STATIC_ASSERT(offsetof(LocalOnfootSyncData, health) == 0x22);
STATIC_ASSERT(offsetof(LocalOnfootSyncData, speed) == 0x26);
STATIC_ASSERT(offsetof(LocalOnfootSyncData, surfingOffset) == 0x32);
STATIC_ASSERT(offsetof(LocalOnfootSyncData, animation) == 0x40);
STATIC_ASSERT(offsetof(LocalIncarSyncData, rotation) == 8);
STATIC_ASSERT(offsetof(LocalIncarSyncData, position) == 0x18);
STATIC_ASSERT(offsetof(LocalIncarSyncData, speed) == 0x24);
STATIC_ASSERT(offsetof(LocalIncarSyncData, health) == 0x30);
STATIC_ASSERT(offsetof(LocalIncarSyncData, pedHealth) == 0x34);
STATIC_ASSERT(offsetof(LocalIncarSyncData, trailer) == 0x39);
STATIC_ASSERT(offsetof(LocalIncarSyncData, modelData) == 0x3b);
STATIC_ASSERT(offsetof(LocalAimSyncData, aimZ) == 25);
STATIC_ASSERT(offsetof(LocalAimSyncData, zoomWeapon) == 29);
STATIC_ASSERT(offsetof(LocalTrailerSyncData, rotation) == 14);
STATIC_ASSERT(offsetof(LocalTrailerSyncData, speed) == 30);
STATIC_ASSERT(offsetof(LocalPassengerSyncData, keys) == 10);
STATIC_ASSERT(offsetof(LocalPassengerSyncData, position) == 12);
STATIC_ASSERT(offsetof(UnoccupiedSyncData, position) == 27);
STATIC_ASSERT(offsetof(UnoccupiedSyncData, health) == 63);
STATIC_ASSERT(offsetof(BulletSyncData, targetId) == 1);
STATIC_ASSERT(offsetof(BulletSyncData, origin) == 3);
STATIC_ASSERT(offsetof(BulletSyncData, weapon) == 39);

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
    LocalSpawnInfoWireSize = sizeof(LocalSpawnInfo) - sizeof(int)
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
    DWORD unknown2f6, rawSurfState2fa;
    DWORD wantsAnotherClass;
    int selectedClass;
    DWORD waitingForSpawnReply, rawFlag30a, zoneDisplayTick;
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
    // 0x00017c60
    void ReturnToClassSelection();
    // Raw DWORD flag used while the local ped is removed for RC driving.
    // R5 address to verify
    DWORD GetRCModeState();
    // Returns the raw DWORD, without bool normalization.
    // R5 address to verify
    DWORD GetActiveState();
    // R5 address to verify
    CEntity* GetSurfingEntity();
    // Clears only the entity and mode, retaining lock and offset state.
    // R5 address to verify
    void ClearSurfingEntityAndMode();
    // R5 address to verify
    void SetTeam(BYTE team);
    // R5 address to verify
    BYTE GetTeam();
    // R5 address to verify
    BYTE GetAimTargetOffset(WORD* id, CVector* offset);

    // R5 address to verify
    DWORD GetSpawnPermission();
    // R5 address to verify
    DWORD IsSpectating();

    union
    {
        LocalPlayerState m_state;
        BYTE nativeState[0x328];
    };
    // 0x000074c0
    BOOL Process();
    // 0x00002fa0
    void ProcessHead();
    // 0x00003f50
    void ProcessWorldBoundaries();
    // 0x00003fc0
    void ProcessVehicleWorldBoundaries();
    // 0x00004080
    void PrepareForClassSelection();
    // 0x00005de0
    void ProcessVehicleDamage(WORD vehicle);
    // 0x00006fe0
    void ProcessPassengerEntry();
    // R5 address to verify
    WORD GetAimedPlayer();
    // R5 address to verify
    WORD GetAimedActor();
    // 0x00006100
    void ProcessClassSelection();
    // 0x00006290
    void UpdateWeapons();
    // 0x00006540
    void ProcessSpectating();
    // Native returns true in AL; this pointer is unused.
    // 0x00004440
    bool GetCameraTarget(WORD* targets);
    // 0x00004700
    void UpdateCameraTarget();
    // 0x00004850
    void DrawCameraTargetLabels();
    // 0x00003410
    BYTE GetSpecialAction();
    // 0x00004f00
    void SendOnfootData();
    // 0x00007080
    void SendIncarData();
    // 0x00004300
    DWORD NeedsToSendOnfootData(WORD keys, WORD upDown, WORD leftRight);
    // 0x00004340
    DWORD NeedsToSendIncarData(WORD keys, WORD upDown, WORD leftRight);
    // 0x00005210
    void SendAimData();
    // 0x000053d0
    void SendTrailerData(WORD vehicle);
    // 0x00005590
    void SendPassengerData();
    // 0x00005810
    void WastedNotification();
    // 0x00005d00
    void SendStats();
    // 0x00004d30
    void SendUnoccupiedData(WORD vehicle, BYTE seat);
    // Native bool result in AL; deliberately retains player0 selection behavior.
    // 0x00006e00
    bool ProcessUnoccupiedSync(WORD id, CVehicle* vehicle);
    // R5 address to verify
    void ClearSurfingState();
    // 0x000040e0
    void PrepareForClassSelection_Outcome(BOOL outcome);
    // Copies the raw50-byte spawn record forward and marks it available.
    // 0x00003be0
    void SetSpawnInfo(const void* info);
    // 0x00004c50
    CLocalPlayer();
    // 0x00002e90
    void ResetData();
    // 0x000030f0
    void SetSpecialAction(BYTE action);
    // 0x00003c20
    BOOL Spawn();
    // Returns the raw DWORD state.
    // 0x000041c0
    void ToggleSpectating(BOOL enabled);
    // 0x00004230
    void SpectateVehicle(WORD vehicle);
    // 0x00004280
    void SpectatePlayer(WORD player);
    // 0x00003ed0
    void SetColor(DWORD color);
    // 0x00003f00
    DWORD GetColorAsRGBA();
    // Raw DWORD result: returns1 after500ms or when any byte differs.
    // 0x00003a60
    DWORD HasDataChanged(const void* previous, const void* current, DWORD length);
    // R5 address to verify
    void SendSpawnRequest();
    // 0x000058d0
    void RequestClass(int selection);
    // 0x00005970
    void SendInteriorChange(BYTE interior);
    // 0x00003f20
    DWORD GetColorAsARGB();
    // 0x00003ab0
    DWORD GetInCarSendRate();
    // 0x00003af0
    DWORD GetOnFootSendRate();
    // 0x00003b30
    DWORD GetSendRate();
    // 0x00003570
    void ApplySurfingPhysics();
    // 0x00003710
    void SetSurfingEntity(CEntity* entity, DWORD locked);
    // May transform data->offset in place for an unclassified backend target.
    // R5 address to verify
    void SendBullet(PedBulletData* data);
    // 0x00002d70
    CPed* GetPed();
    // Damage amount is a native float; take/give apply scaling before serialization.
    // 0x00005ad0
    void EnterVehicle(WORD vehicle, BOOL passenger);
    // 0x000068a0
    void SendTakeDamage(WORD player, float damage, DWORD weapon, DWORD bodyPart);
    // 0x000069b0
    void SendGiveDamage(WORD player, float damage, DWORD weapon, DWORD bodyPart);
    // R5 address to verify
    void SendActorDamage(WORD actor, float damage, DWORD weapon, DWORD bodyPart);
    // 0x00005bf0
    void ExitVehicle(int vehicle);
    // Network/pool dependency used only when Move finds a player pool.
    // 0x00003730
    void ProcessSurfing();
    // 0x00005a10
    void Chat(char* text);
    // 0x00004060
    void RequestSpawn();
    // 0x00005ff0
    void NextClass();
    // 0x00006080
    void PrevClass();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(LocalSpawnInfo) == 50);
STATIC_ASSERT(offsetof(LocalSpawnInfo, position) == 10);
STATIC_ASSERT(offsetof(LocalSpawnInfo, weapons) == 26);
STATIC_ASSERT(offsetof(LocalSpawnInfo, ammo) == 38);
STATIC_ASSERT(offsetof(LocalPlayerState, ped) == 0x0);
STATIC_ASSERT(offsetof(LocalPlayerState, trailer) == 0x4);
STATIC_ASSERT(offsetof(LocalPlayerState, onfoot) == 0x3a);
STATIC_ASSERT(offsetof(LocalPlayerState, passenger) == 0x7e);
STATIC_ASSERT(offsetof(LocalPlayerState, incar) == 0x96);
STATIC_ASSERT(offsetof(LocalPlayerState, aim) == 0xd5);
STATIC_ASSERT(offsetof(LocalPlayerState, active) == 0xf4);
STATIC_ASSERT(offsetof(LocalPlayerState, wasted) == 0xf8);
STATIC_ASSERT(offsetof(LocalPlayerState, currentVehicle) == 0xfc);
STATIC_ASSERT(offsetof(LocalPlayerState, cameraTargets) == 0x108);
STATIC_ASSERT(offsetof(LocalPlayerState, headDirection) == 0x114);
STATIC_ASSERT(offsetof(LocalPlayerState, headUpdateTick) == 0x120);
STATIC_ASSERT(offsetof(LocalPlayerState, spectating) == 0x12c);
STATIC_ASSERT(offsetof(LocalPlayerState, team) == 0x130);
STATIC_ASSERT(offsetof(LocalPlayerState, unknown131) == 0x131);
STATIC_ASSERT(offsetof(LocalPlayerState, lastSendTick) == 0x133);
STATIC_ASSERT(offsetof(LocalPlayerState, spawnPermission) == 0x143);
STATIC_ASSERT(offsetof(LocalPlayerState, classSelectionTick) == 0x147);
STATIC_ASSERT(offsetof(LocalPlayerState, initialSelectionTick) == 0x14b);
STATIC_ASSERT(offsetof(LocalPlayerState, spawnInfo) == 0x14f);
STATIC_ASSERT(offsetof(LocalPlayerState, hasSpawnInfo) == 0x181);
STATIC_ASSERT(offsetof(LocalPlayerState, weaponsUpdateTick) == 0x185);
STATIC_ASSERT(offsetof(LocalPlayerState, aimedPlayer) == 0x189);
STATIC_ASSERT(offsetof(LocalPlayerState, lastWeapons) == 0x18e);
STATIC_ASSERT(offsetof(LocalPlayerState, lastAmmo) == 0x19b);
STATIC_ASSERT(offsetof(LocalPlayerState, passengerDriveby) == 0x1cf);
STATIC_ASSERT(offsetof(LocalPlayerState, interior) == 0x1d3);
STATIC_ASSERT(offsetof(LocalPlayerState, rcMode) == 0x1d4);
STATIC_ASSERT(offsetof(LocalPlayerState, unknown1d8) == 0x1d8);
STATIC_ASSERT(offsetof(LocalPlayerState, surfingId) == 0x2d8);
STATIC_ASSERT(offsetof(LocalPlayerState, surfingTick) == 0x2da);
STATIC_ASSERT(offsetof(LocalPlayerState, surfingEntity) == 0x2de);
STATIC_ASSERT(offsetof(LocalPlayerState, surfingLocked) == 0x2e2);
STATIC_ASSERT(offsetof(LocalPlayerState, surfingMode) == 0x2e6);
STATIC_ASSERT(offsetof(LocalPlayerState, surfingOffset) == 0x2ea);
STATIC_ASSERT(offsetof(LocalPlayerState, unknown2f6) == 0x2f6);
STATIC_ASSERT(offsetof(LocalPlayerState, rawSurfState2fa) == 0x2fa);
STATIC_ASSERT(offsetof(LocalPlayerState, wantsAnotherClass) == 0x2fe);
STATIC_ASSERT(offsetof(LocalPlayerState, selectedClass) == 0x302);
STATIC_ASSERT(offsetof(LocalPlayerState, waitingForSpawnReply) == 0x306);
STATIC_ASSERT(offsetof(LocalPlayerState, rawFlag30a) == 0x30a);
STATIC_ASSERT(offsetof(LocalPlayerState, zoneDisplayTick) == 0x30e);
STATIC_ASSERT(offsetof(LocalPlayerState, spectateCameraMode) == 0x312);
STATIC_ASSERT(offsetof(LocalPlayerState, spectateType) == 0x313);
STATIC_ASSERT(offsetof(LocalPlayerState, spectateId) == 0x314);
STATIC_ASSERT(offsetof(LocalPlayerState, spectateProcessed) == 0x318);
STATIC_ASSERT(offsetof(LocalPlayerState, damageVehicle) == 0x31c);
STATIC_ASSERT(offsetof(LocalPlayerState, panelsDamage) == 0x31e);
STATIC_ASSERT(offsetof(LocalPlayerState, doorsDamage) == 0x322);
STATIC_ASSERT(offsetof(LocalPlayerState, lightsDamage) == 0x326);
STATIC_ASSERT(offsetof(LocalPlayerState, tiresDamage) == 0x327);
STATIC_ASSERT(sizeof(LocalPlayerState) == 0x328);
STATIC_ASSERT(sizeof(CLocalPlayer) == 0x328);
// R5 address to verify
extern const float g_localUnoccupiedRange;
// R5 address to verify
extern const float g_localAimZoomScale;
// 0x000e58dc
extern const float g_localAimAspectScale;
// 0x000e58e0
extern int g_localWastedRpc;
// 0x000e58f8
extern int g_localVehicleDamageRpc;
// 0x000e590c
extern const float g_localPassengerEntryDistance;
// 0x000e5818
extern const char g_localWorldBoundaryText[];
// R5 address to verify
extern const Scripting::OpcodeInfo g_localPedCameraCommand;
// R5 address to verify
extern const float g_localCameraRayLength;
// R5 address to verify
extern int g_localCameraTargetRpc;
// 0x000e58b0
extern const char g_localCameraTargetObjectText[];
// 0x000e588c
extern const char g_localCameraTargetVehicleText[];
// 0x000e5868
extern const char g_localCameraTargetPlayerText[];
// 0x000e5844
extern const char g_localCameraTargetActorText[];
// 0x000fe0b0
extern DWORD g_localAimSendRate;
// 0x000e5910
extern const char g_localClassSelectionMessage[];
// 0x000e58d4
extern const float g_localQuaternionSyncThreshold;
// 0x000e580c
extern int g_localSpawnRequestRpc;
// 0x000e5810
extern int g_localSpawnRpc;
// Skips one SetInitialState call when nonzero.
// R5 address to verify
extern DWORD g_localSkipInitialState;
// 0x000e58e4
extern int g_localRequestClassRpc;
// 0x000e58e8
extern int g_localInteriorChangeRpc;
// 0x000e58ec
extern int g_localChatRpc;
// 0x000e58f0
extern int g_localEnterVehicleRpc;
// 0x000e58f4
extern int g_localExitVehicleRpc;
// R5 address to verify
extern int g_localPlayerDamageRpc;
// R5 address to verify
extern int g_localActorDamageRpc;
// R5 address to verify
extern const float g_localDamageScale;
// R5 address to verify
extern const Scripting::OpcodeInfo g_localTrainCameraCommand;
// R5 address to verify
extern DWORD g_localTrainCameraTick;

struct CVector;
class CMatrix;
// R5 address to verify
extern CVector g_localSurfPedTurnSpeed;
// R5 address to verify
extern CMatrix g_localSurfPedMatrix;
// R5 address to verify
extern CVector g_localSurfEntitySpeed;
// R5 address to verify
extern CMatrix g_localSurfInverseMatrix;
// R5 address to verify
extern CVector g_localSurfPedSpeed;
// R5 address to verify
extern CMatrix g_localSurfEntityMatrix;
// R5 address to verify
extern CVector g_localSurfEntityTurnSpeed;

// 0x000fe0a8
extern DWORD g_localOnFootSendRate;
// 0x000fe0ac
extern DWORD g_localInCarSendRate;

#endif // LOCALPLAYER_H
