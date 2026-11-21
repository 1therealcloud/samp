#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include "net/localplayer.h"

#include <stddef.h>
#include <windows.h>

#include <static_assert.h>

struct CVector;
class CMatrix;
#pragma pack(push, 1)
class CPed;
class CVehicle;
// Packed observed storage, not a separately constructed native object.
// Unused cache regions retain offset names rather than inferred meanings.
struct RemotePlayerState
{
    WORD id, vehicleId;
    CPed* ped;
    CVehicle* vehicle;
    DWORD unknown0c, showNameTag, jetpackState;
    BYTE specialAction, team, playerState, seat;
    DWORD rawFlag1c, passengerDriveby;
    LocalPassengerSyncData passenger;
    LocalOnfootSyncData onfoot;
    LocalIncarSyncData incar;
    BYTE unknownbf[54];
    BYTE unknownf5[31];
    BYTE unknown114[12];
    CVector onfootTargetPosition, onfootTargetSpeed;
    CVector incarTargetPosition, incarTargetSpeed;
    BYTE unknown150[0x40];
    CVector positionInaccuracy;
    GameQuaternion incarTargetRotation;
    float reportedArmour, reportedHealth;
    BYTE unknown1b4[12];
    DWORD appliedAnimation;
    BYTE updateType;
    DWORD lastUpdateTick, lastNetworkTimestamp;
    // Script ApplyAnimation/ClearAnimations RPC flag, stored even without a ped.
    DWORD scriptedAnimation;
    DWORD status;
    CVector headDirection;
    DWORD lastHeadUpdateTick, lastHeadTaskTick;
    DWORD markerEnabled;
    int markerX, markerY, markerZ;
    DWORD marker;
};
class CRemotePlayer
{
public:
    // Any nonzero input byte becomes DWORD1; this is not a raw BOOL copy.
    // R5 address to verify
    void ShowNameTag(BYTE show);
    // Synchronization caches, not queries to the GTA ped backend.
    // R5 address to verify
    float GetReportedHealth();
    // R5 address to verify
    float GetReportedArmour();
    // R5 address to verify
    void SetTeam(BYTE team);
    // R5 address to verify
    BYTE GetTeam();
    // R5 address to verify
    CPed* GetPed();
    // R5 address to verify
    CVehicle* GetVehicle();

    // R5 address to verify
    BYTE GetState();
    // R5 address to verify
    void SetId(WORD id);
    // R5 address to verify
    WORD GetId();
    // R5 address to verify
    void SetRawFlag1C(DWORD value);
    // R5 address to verify
    void ResetStateAndLabels();
    // R5 address to verify
    BYTE GetSpecialAction();
    // R5 address to verify
    DWORD GetRawFlag1C();

    union
    {
        RemotePlayerState m_state;
        // Existing raw view is retained for installed hooks and x87 assembly.
        BYTE nativeState[0x1fd];
    };
    // 0x000158d0
    void UpdateUnoccupied(const void* data);
    // 0x00015c90
    void UpdateTrailer(const void* data);
    // 0x00016370
    void UpdateBullet(const void* data);
    // R5 address to verify
    void SetState(BYTE state);
    // 0x000166b0
    void Process();
    // 0x000143a0
    void ProcessHead();
    // Raw DWORD state is retained, not normalized to bool.
    // 0x00014500
    void SetMarkerState(BOOL enabled);
    // 0x00014540
    void SetMarkerPosition(short x, short y, short z);
    // 0x000145f0
    BOOL HasSurfingTarget();
    // 0x00014620
    BOOL SurfingOnObject();
    // 0x00014950
    void ProcessSpecialAction();
    // 0x00014c40
    void UpdateOnfootSpeedAndPosition();
    // R5 address to verify
    void UpdateOnfootXYSpeedAndPosition();
    // 0x00015140
    void UpdateIncarSpeedAndPosition();
    // 0x00014ff0
    void UpdateOnfootRotation();
    // 0x00015460
    void UpdateIncarRotation();
    // 0x00015650
    void UpdateTrain(const CMatrix* matrix, const CVector* speed, float trainSpeed);
    // 0x000150d0
    void SetOnfootTarget(const CVector* position, const CVector* speed);
    // 0x000155e0
    void SetIncarTarget(const CMatrix* matrix, const CVector* position, const CVector* speed);
    // Team argument is consumed by ABI but not read by this native body.
    // 0x00017130
    BOOL Spawn(BYTE team, int model, int customModel, const CVector* position, float rotation, DWORD color, BYTE fightingStyle);
    // 0x00017260
    void UpdateOnfoot(const void* data, DWORD timestamp);
    // 0x00017340
    void UpdateIncar(const void* data, DWORD timestamp);
    // 0x00017440
    void UpdatePassenger(const void* data);
    // 0x00015760
    void UpdateAim(const void* data);
    // 0x00014800
    void OnEnterVehicle();
    // 0x000148f0
    void OnExitVehicle();
    // 0x00017610
    void Chat(char* text);
    // 0x000165e0
    CRemotePlayer();
    // 0x00016660
    ~CRemotePlayer();
    // 0x00015fa0
    void ResetData();
    // Native distance returns extended ST0; the other ped has no extra null guard.
    // 0x000160a0
    float GetDistanceToPlayer(CRemotePlayer* other);
    // 0x00016120
    float GetDistanceToLocalPlayer();
    // 0x00016150
    void SetColor(DWORD color);
    // 0x00016170
    DWORD GetColorAsRGBA();
    // 0x000161a0
    void EnterVehicle(WORD id, BOOL passenger);
    // 0x00016230
    void ExitVehicle();
    // 0x00016270
    void ChangeState(BYTE newState, BYTE oldState);
    // 0x00017570
    void Kill();
    // 0x00016180
    DWORD GetColorAsARGB();
    // 0x00016330
    int GetStatus();
    // Requires a ped pointer and nonzero native state byte.
    // 0x00001080
    BOOL IsActive();
    // Native on-foot attachment/surfing matrix pass.
    // 0x00014650
    void ApplySurfingMatrix();
    // 0x00017530
    void Remove();
};
#pragma pack(pop)
STATIC_ASSERT(offsetof(RemotePlayerState, ped) == 4);
STATIC_ASSERT(offsetof(RemotePlayerState, vehicle) == 8);
STATIC_ASSERT(offsetof(RemotePlayerState, showNameTag) == 0x10);
STATIC_ASSERT(offsetof(RemotePlayerState, specialAction) == 0x18);
STATIC_ASSERT(offsetof(RemotePlayerState, playerState) == 0x1a);
STATIC_ASSERT(offsetof(RemotePlayerState, rawFlag1c) == 0x1c);
STATIC_ASSERT(offsetof(RemotePlayerState, passenger) == 0x24);
STATIC_ASSERT(offsetof(RemotePlayerState, onfoot) == 0x3c);
STATIC_ASSERT(offsetof(RemotePlayerState, incar) == 0x80);
STATIC_ASSERT(offsetof(RemotePlayerState, unknownf5) == 0xf5);
STATIC_ASSERT(offsetof(RemotePlayerState, onfootTargetPosition) == 0x120);
STATIC_ASSERT(offsetof(RemotePlayerState, incarTargetPosition) == 0x138);
STATIC_ASSERT(offsetof(RemotePlayerState, positionInaccuracy) == 0x190);
STATIC_ASSERT(offsetof(RemotePlayerState, incarTargetRotation) == 0x19c);
STATIC_ASSERT(offsetof(RemotePlayerState, reportedArmour) == 0x1ac);
STATIC_ASSERT(offsetof(RemotePlayerState, reportedHealth) == 0x1b0);
STATIC_ASSERT(offsetof(RemotePlayerState, appliedAnimation) == 0x1c0);
STATIC_ASSERT(offsetof(RemotePlayerState, updateType) == 0x1c4);
STATIC_ASSERT(offsetof(RemotePlayerState, lastUpdateTick) == 0x1c5);
STATIC_ASSERT(offsetof(RemotePlayerState, lastNetworkTimestamp) == 0x1c9);
STATIC_ASSERT(offsetof(RemotePlayerState, scriptedAnimation) == 0x1cd);
STATIC_ASSERT(offsetof(RemotePlayerState, status) == 0x1d1);
STATIC_ASSERT(offsetof(RemotePlayerState, headDirection) == 0x1d5);
STATIC_ASSERT(offsetof(RemotePlayerState, lastHeadUpdateTick) == 0x1e1);
STATIC_ASSERT(offsetof(RemotePlayerState, lastHeadTaskTick) == 0x1e5);
STATIC_ASSERT(offsetof(RemotePlayerState, markerEnabled) == 0x1e9);
STATIC_ASSERT(offsetof(RemotePlayerState, markerX) == 0x1ed);
STATIC_ASSERT(offsetof(RemotePlayerState, marker) == 0x1f9);
STATIC_ASSERT(sizeof(RemotePlayerState) == 0x1fd);
STATIC_ASSERT(sizeof(CRemotePlayer) == 0x1fd);
// R5 address to verify
extern const float g_remoteAbsentDistance;
// 0x000e6158
extern const char g_remoteCarJackedText[];
// R5 address to verify
extern const float g_remoteAimZoomScale;
// 0x000e613c
extern const float g_remoteAimAspectScale;
// Raw native switch, default1. Other native writers remain unreviewed.
// R5 address to verify
extern DWORD g_remoteClampTurnSpeed;
// R5 address to verify
extern const float g_remoteTurnSpeedMinimum;
// Raw correction coefficients; native default values, other writers unreviewed.
// R5 address to verify
extern float g_remoteOnfootCorrection;
// R5 address to verify
extern float g_remoteIncarCorrection;
// 0x000e590c
extern const float g_remoteIncarTeleportDistance;
// 0x000e5804
extern const float g_remoteHeadDistance;
// 0x000e57f8
extern const char g_remoteHeadTask[];
// 0x000e6188
extern const char g_remoteVendingAnimation[];
// 0x000e6194
extern const char g_remoteVendingLibrary[];
// 0x000e6174
extern const char g_remoteCarryAnimation[];
// 0x000e6180
extern const char g_remoteCarryLibrary[];
// R5 address to verify
extern const float g_remoteBulletOffsetMinimum;
// 0x000e6170
extern const float g_remoteBulletOffsetMaximum;
// R5 address to verify
extern const float g_remoteVehicleAxisTeleportDistance;
// 0x000e6140
extern const double g_remoteUnoccupiedCorrectionThreshold;
// R5 address to verify
extern const float g_remoteTrailerCorrection;

#endif // REMOTEPLAYER_H
