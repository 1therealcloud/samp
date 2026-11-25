#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

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
    DWORD isNpc, passengerDriveby;
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

    void ShowNameTag(BYTE show);
    // Synchronization caches, not queries to the GTA ped backend.

    float GetReportedHealth();

    float GetReportedArmour();

    void SetTeam(BYTE team);

    BYTE GetTeam();

    CPed* GetPed();

    CVehicle* GetVehicle();

    BYTE GetState();

    void SetId(WORD id);

    WORD GetId();

    void SetNpc(DWORD value);

    void ResetStateAndLabels();

    BYTE GetSpecialAction();

    DWORD IsNpc();

    union
    {
        RemotePlayerState m_state;
        // Existing raw view is retained for installed hooks and x87 assembly.
        BYTE nativeState[0x1fd];
    };

    void UpdateUnoccupied(const void* data);

    void UpdateTrailer(const void* data);

    void UpdateBullet(const void* data);

    void SetState(BYTE state);

    void Process();

    void ProcessHead();
    // Raw DWORD state is retained, not normalized to bool.

    void SetMarkerState(BOOL enabled);

    void SetMarkerPosition(short x, short y, short z);

    BOOL HasSurfingTarget();

    BOOL SurfingOnObject();

    void ProcessSpecialAction();

    void UpdateOnfootSpeedAndPosition();

    void UpdateOnfootXYSpeedAndPosition();

    void UpdateIncarSpeedAndPosition();

    void UpdateOnfootRotation();

    void UpdateIncarRotation();

    void UpdateTrain(const CMatrix* matrix, const CVector* speed, float trainSpeed);

    void SetOnfootTarget(const CVector* position, const CVector* speed);

    void SetIncarTarget(const CMatrix* matrix, const CVector* position, const CVector* speed);
    // Team argument is consumed by ABI but not read by this native body.

    BOOL Spawn(BYTE team, int model, int customModel, const CVector* position, float rotation, DWORD color, BYTE fightingStyle);

    void UpdateOnfoot(const void* data, DWORD timestamp);

    void UpdateIncar(const void* data, DWORD timestamp);

    void UpdatePassenger(const void* data);

    void UpdateAim(const void* data);

    void OnEnterVehicle();

    void OnExitVehicle();

    void Chat(char* text);

    CRemotePlayer();

    ~CRemotePlayer();

    void ResetData();
    // Native distance returns extended ST0; the other ped has no extra null guard.

    float GetDistanceToPlayer(CRemotePlayer* other);

    float GetDistanceToLocalPlayer();

    void SetColor(DWORD color);

    DWORD GetColorAsRGBA();

    void EnterVehicle(WORD id, BOOL passenger);

    void ExitVehicle();

    void ChangeState(BYTE newState, BYTE oldState);

    void Kill();

    DWORD GetColorAsARGB();

    int GetStatus();
    // Requires a ped pointer and nonzero native state byte.

    BOOL IsActive();
    // Native on-foot attachment/surfing matrix pass.

    void ApplySurfingMatrix();

    void Remove();
};
#pragma pack(pop)
static_assert(offsetof(RemotePlayerState, ped) == 4);
static_assert(offsetof(RemotePlayerState, vehicle) == 8);
static_assert(offsetof(RemotePlayerState, showNameTag) == 0x10);
static_assert(offsetof(RemotePlayerState, specialAction) == 0x18);
static_assert(offsetof(RemotePlayerState, playerState) == 0x1a);
static_assert(offsetof(RemotePlayerState, isNpc) == 0x1c);
static_assert(offsetof(RemotePlayerState, passenger) == 0x24);
static_assert(offsetof(RemotePlayerState, onfoot) == 0x3c);
static_assert(offsetof(RemotePlayerState, incar) == 0x80);
static_assert(offsetof(RemotePlayerState, unknownf5) == 0xf5);
static_assert(offsetof(RemotePlayerState, onfootTargetPosition) == 0x120);
static_assert(offsetof(RemotePlayerState, incarTargetPosition) == 0x138);
static_assert(offsetof(RemotePlayerState, positionInaccuracy) == 0x190);
static_assert(offsetof(RemotePlayerState, incarTargetRotation) == 0x19c);
static_assert(offsetof(RemotePlayerState, reportedArmour) == 0x1ac);
static_assert(offsetof(RemotePlayerState, reportedHealth) == 0x1b0);
static_assert(offsetof(RemotePlayerState, appliedAnimation) == 0x1c0);
static_assert(offsetof(RemotePlayerState, updateType) == 0x1c4);
static_assert(offsetof(RemotePlayerState, lastUpdateTick) == 0x1c5);
static_assert(offsetof(RemotePlayerState, lastNetworkTimestamp) == 0x1c9);
static_assert(offsetof(RemotePlayerState, scriptedAnimation) == 0x1cd);
static_assert(offsetof(RemotePlayerState, status) == 0x1d1);
static_assert(offsetof(RemotePlayerState, headDirection) == 0x1d5);
static_assert(offsetof(RemotePlayerState, lastHeadUpdateTick) == 0x1e1);
static_assert(offsetof(RemotePlayerState, lastHeadTaskTick) == 0x1e5);
static_assert(offsetof(RemotePlayerState, markerEnabled) == 0x1e9);
static_assert(offsetof(RemotePlayerState, markerX) == 0x1ed);
static_assert(offsetof(RemotePlayerState, marker) == 0x1f9);
static_assert(sizeof(RemotePlayerState) == 0x1fd);
static_assert(sizeof(CRemotePlayer) == 0x1fd);

extern const float g_remoteAbsentDistance;

extern const char g_remoteCarJackedText[];

extern const float g_remoteAimZoomScale;

extern const float g_remoteAimAspectScale;
// Raw native switch, default1. Other native writers remain unreviewed.

extern DWORD g_remoteClampTurnSpeed;

extern const float g_remoteTurnSpeedMinimum;
// Raw correction coefficients; native default values, other writers unreviewed.

extern float g_remoteOnfootCorrection;

extern float g_remoteIncarCorrection;

extern const float g_remoteIncarTeleportDistance;

extern const float g_remoteHeadDistance;

extern const char g_remoteHeadTask[];

extern const char g_remoteVendingAnimation[];

extern const char g_remoteVendingLibrary[];

extern const char g_remoteCarryAnimation[];

extern const char g_remoteCarryLibrary[];

extern const float g_remoteBulletOffsetMinimum;

extern const float g_remoteBulletOffsetMaximum;

extern const float g_remoteVehicleAxisTeleportDistance;

extern const double g_remoteUnoccupiedCorrectionThreshold;

extern const float g_remoteTrailerCorrection;

#endif // REMOTEPLAYER_H
