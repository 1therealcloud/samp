#ifndef SAMP_REMOTE_PLAYER_ADDRESS_INTERFACE_H
#define SAMP_REMOTE_PLAYER_ADDRESS_INTERFACE_H
#include "net/localplayer.h"
#include <stddef.h>
#include <windows.h>
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
    // 0x00017790
    void ShowNameTag(BYTE show);
    // Synchronization caches, not queries to the GTA ped backend.
    // 0x000739c0
    float GetReportedHealth();
    // 0x000739d0
    float GetReportedArmour();
    // 0x00008180
    void SetTeam(BYTE team);
    // 0x000a21e0
    BYTE GetTeam();
    // 0x00001070
    CPed* GetPed();
    // 0x000010a0
    CVehicle* GetVehicle();

    // 0x00002dc0
    BYTE GetState();
    // 0x000135d0
    void SetId(WORD id);
    // 0x000135e0
    WORD GetId();
    // 0x000135f0
    void SetRawFlag1C(DWORD value);
    // 0x00013600
    void ResetStateAndLabels();
    // 0x00014220
    BYTE GetSpecialAction();
    // 0x00014230
    DWORD GetRawFlag1C();

    union
    {
        RemotePlayerState m_state;
        // Existing raw view is retained for installed hooks and x87 assembly.
        BYTE nativeState[0x1fd];
    };
    // 0x00015600
    void UpdateUnoccupied(const void* data);
    // 0x00015950
    void UpdateTrailer(const void* data);
    // 0x00016010
    void UpdateBullet(const void* data);
    // 0x00016230
    void SetState(BYTE state);
    // 0x00016310
    void Process();
    // 0x00014240
    void ProcessHead();
    // Raw DWORD state is retained, not normalized to bool.
    // 0x00014390
    void SetMarkerState(BOOL enabled);
    // 0x000143d0
    void SetMarkerPosition(short x, short y, short z);
    // Descriptive name: includes encoded object IDs as well as vehicle IDs.
    // 0x00014470
    BOOL HasSurfingTarget();
    // 0x000144a0
    BOOL SurfingOnObject();
    // 0x00014780
    void ProcessSpecialAction();
    // 0x000149f0
    void UpdateOnfootSpeedAndPosition();
    // Descriptive name for native XY-only path.
    // 0x00014c00
    void UpdateOnfootXYSpeedAndPosition();
    // 0x00014ec0
    void UpdateIncarSpeedAndPosition();
    // 0x00014d80
    void UpdateOnfootRotation();
    // 0x000151c0
    void UpdateIncarRotation();
    // 0x000153a0
    void UpdateTrain(const CMatrix* matrix, const CVector* speed, float trainSpeed);
    // Descriptive names for native target-storage methods; backend is unchecked.
    // 0x00014e50
    void SetOnfootTarget(const CVector* position, const CVector* speed);
    // 0x00015330
    void SetIncarTarget(const CMatrix* matrix, const CVector* position, const CVector* speed);
    // Team argument is consumed by ABI but not read by this native body.
    // 0x00016c50
    BOOL Spawn(BYTE team, int model, int customModel, const CVector* position, float rotation, DWORD color, BYTE fightingStyle);
    // Descriptive overload names for native receive/cache entries.
    // 0x00016da0
    void UpdateOnfoot(const void* data, DWORD timestamp);
    // 0x00016e70
    void UpdateIncar(const void* data, DWORD timestamp);
    // 0x00016f60
    void UpdatePassenger(const void* data);
    // 0x000154a0
    void UpdateAim(const void* data);
    // 0x00014660
    void OnEnterVehicle();
    // 0x00014730
    void OnExitVehicle();
    // 0x00017120
    void Chat(char* text);
    // 0x00016250
    CRemotePlayer();
    // 0x000162c0
    ~CRemotePlayer();
    // 0x00015c60
    void ResetData();
    // Native distance returns extended ST0; the other ped has no extra null guard.
    // 0x00015d50
    float GetDistanceToPlayer(CRemotePlayer* other);
    // 0x00015dd0
    float GetDistanceToLocalPlayer();
    // 0x00015e00
    void SetColor(DWORD color);
    // 0x00015e20
    DWORD GetColorAsRGBA();
    // 0x00015e50
    void EnterVehicle(WORD id, BOOL passenger);
    // 0x00015ee0
    void ExitVehicle();
    // 0x00015f10
    void ChangeState(BYTE newState, BYTE oldState);
    // 0x00017080
    void Kill();
    // 0x00015e30
    DWORD GetColorAsARGB();
    // 0x00015fd0
    int GetStatus();
    // Requires a ped pointer and nonzero native state byte.
    // 0x00001080
    BOOL IsActive();
    // Native on-foot attachment/surfing matrix pass.
    // 0x000144d0
    void ApplySurfingMatrix();
    // 0x00017040
    void Remove();
};
#pragma pack(pop)
typedef char RemotePedOffset[(offsetof(RemotePlayerState, ped) == 4) ? 1 : -1];
typedef char RemoteVehicleOffset[(offsetof(RemotePlayerState, vehicle) == 8) ? 1 : -1];
typedef char RemoteTagOffset[(offsetof(RemotePlayerState, showNameTag) == 0x10) ? 1 : -1];
typedef char RemoteSpecialActionOffset[(offsetof(RemotePlayerState, specialAction) == 0x18) ? 1 : -1];
typedef char RemoteStateOffset[(offsetof(RemotePlayerState, playerState) == 0x1a) ? 1 : -1];
typedef char RemoteRawFlagOffset[(offsetof(RemotePlayerState, rawFlag1c) == 0x1c) ? 1 : -1];
typedef char RemotePassengerOffset[(offsetof(RemotePlayerState, passenger) == 0x24) ? 1 : -1];
typedef char RemoteOnfootOffset[(offsetof(RemotePlayerState, onfoot) == 0x3c) ? 1 : -1];
typedef char RemoteIncarOffset[(offsetof(RemotePlayerState, incar) == 0x80) ? 1 : -1];
typedef char RemoteUnknownF5Offset[(offsetof(RemotePlayerState, unknownf5) == 0xf5) ? 1 : -1];
typedef char RemoteOnfootTargetOffset[(offsetof(RemotePlayerState, onfootTargetPosition) == 0x120) ? 1 : -1];
typedef char RemoteIncarTargetOffset[(offsetof(RemotePlayerState, incarTargetPosition) == 0x138) ? 1 : -1];
typedef char RemoteInaccuracyOffset[(offsetof(RemotePlayerState, positionInaccuracy) == 0x190) ? 1 : -1];
typedef char RemoteRotationOffset[(offsetof(RemotePlayerState, incarTargetRotation) == 0x19c) ? 1 : -1];
typedef char RemoteArmourOffset[(offsetof(RemotePlayerState, reportedArmour) == 0x1ac) ? 1 : -1];
typedef char RemoteHealthOffset[(offsetof(RemotePlayerState, reportedHealth) == 0x1b0) ? 1 : -1];
typedef char RemoteAnimationOffset[(offsetof(RemotePlayerState, appliedAnimation) == 0x1c0) ? 1 : -1];
typedef char RemoteUpdateTypeOffset[(offsetof(RemotePlayerState, updateType) == 0x1c4) ? 1 : -1];
typedef char RemoteUpdateTickOffset[(offsetof(RemotePlayerState, lastUpdateTick) == 0x1c5) ? 1 : -1];
typedef char RemoteTimestampOffset[(offsetof(RemotePlayerState, lastNetworkTimestamp) == 0x1c9) ? 1 : -1];
typedef char RemoteScriptedAnimationOffset[(offsetof(RemotePlayerState, scriptedAnimation) == 0x1cd) ? 1 : -1];
typedef char RemoteStatusOffset[(offsetof(RemotePlayerState, status) == 0x1d1) ? 1 : -1];
typedef char RemoteHeadDirectionOffset[(offsetof(RemotePlayerState, headDirection) == 0x1d5) ? 1 : -1];
typedef char RemoteHeadTickOffset[(offsetof(RemotePlayerState, lastHeadUpdateTick) == 0x1e1) ? 1 : -1];
typedef char RemoteHeadTaskTickOffset[(offsetof(RemotePlayerState, lastHeadTaskTick) == 0x1e5) ? 1 : -1];
typedef char RemoteMarkerFlagOffset[(offsetof(RemotePlayerState, markerEnabled) == 0x1e9) ? 1 : -1];
typedef char RemoteMarkerPositionOffset[(offsetof(RemotePlayerState, markerX) == 0x1ed) ? 1 : -1];
typedef char RemoteMarkerOffset[(offsetof(RemotePlayerState, marker) == 0x1f9) ? 1 : -1];
typedef char RemoteStateExtent[(sizeof(RemotePlayerState) == 0x1fd) ? 1 : -1];
typedef char RemotePlayerExtent[(sizeof(CRemotePlayer) == 0x1fd) ? 1 : -1];
// 0x001181e4
extern const float g_remoteAbsentDistance;
// 0x001181e8
extern const char g_remoteCarJackedText[];
// 0x001181c8
extern const float g_remoteAimZoomScale;
// 0x001181cc
extern const float g_remoteAimAspectScale;
// Raw native switch, default1. Other native writers remain unreviewed.
// 0x0013c4a0
extern DWORD g_remoteClampTurnSpeed;
// 0x001181bc
extern const float g_remoteTurnSpeedMinimum;
// Raw correction coefficients; native default values, other writers unreviewed.
// 0x0013c498
extern float g_remoteOnfootCorrection;
// 0x0013c49c
extern float g_remoteIncarCorrection;
// 0x0011797c
extern const float g_remoteIncarTeleportDistance;
// 0x00117874
extern const float g_remoteHeadDistance;
// 0x00117868
extern const char g_remoteHeadTask[];
// 0x00118218
extern const char g_remoteVendingAnimation[];
// 0x00118224
extern const char g_remoteVendingLibrary[];
// 0x00118204
extern const char g_remoteCarryAnimation[];
// 0x00118210
extern const char g_remoteCarryLibrary[];
// 0x001181fc
extern const float g_remoteBulletOffsetMinimum;
// 0x00118200
extern const float g_remoteBulletOffsetMaximum;
// 0x001181dc
extern const float g_remoteVehicleAxisTeleportDistance;
// 0x001181d0
extern const double g_remoteUnoccupiedCorrectionThreshold;
// 0x001181e0
extern const float g_remoteTrailerCorrection;
#endif
