#ifndef SAMP_REMOTE_PLAYER_ADDRESS_INTERFACE_H
#define SAMP_REMOTE_PLAYER_ADDRESS_INTERFACE_H
#include <windows.h>
struct CVector;
class CMatrix;
// Native allocation extent0x1FD; unreviewed state remains opaque.
#pragma pack(push,1)
class CRemotePlayer {public:
    BYTE nativeState[0x1fd];
    // samp.dll RVA: 0x00014240
    void ProcessHead();
    // Raw DWORD state is retained, not normalized to bool.
    // samp.dll RVA: 0x00014390
    void SetMarkerState(BOOL enabled);
    // samp.dll RVA: 0x000143d0
    void SetMarkerPosition(short x,short y,short z);
    // Descriptive name: includes encoded object IDs as well as vehicle IDs.
    // samp.dll RVA: 0x00014470
    BOOL HasSurfingTarget();
    // samp.dll RVA: 0x000144a0
    BOOL SurfingOnObject();
    // samp.dll RVA: 0x00014780
    void ProcessSpecialAction();
    // samp.dll RVA: 0x000149f0
    void UpdateOnfootSpeedAndPosition();
    // Descriptive name for native XY-only path.
    // samp.dll RVA: 0x00014c00
    void UpdateOnfootXYSpeedAndPosition();
    // samp.dll RVA: 0x00014ec0
    void UpdateIncarSpeedAndPosition();
    // samp.dll RVA: 0x00014d80
    void UpdateOnfootRotation();
    // samp.dll RVA: 0x000151c0
    void UpdateIncarRotation();
    // samp.dll RVA: 0x000153a0
    void UpdateTrain(const CMatrix *matrix,const CVector *speed,float trainSpeed);
    // Descriptive names for native target-storage methods; backend is unchecked.
    // samp.dll RVA: 0x00014e50
    void SetOnfootTarget(const CVector *position,const CVector *speed);
    // samp.dll RVA: 0x00015330
    void SetIncarTarget(const CMatrix *matrix,const CVector *position,const CVector *speed);
    // Team argument is consumed by ABI but not read by this native body.
    // samp.dll RVA: 0x00016c50
    BOOL Spawn(BYTE team,int model,int customModel,const CVector *position,float rotation,DWORD color,BYTE fightingStyle);
    // Descriptive overload names for native receive/cache entries.
    // samp.dll RVA: 0x00016da0
    void UpdateOnfoot(const void *data,DWORD timestamp);
    // samp.dll RVA: 0x00016e70
    void UpdateIncar(const void *data,DWORD timestamp);
    // samp.dll RVA: 0x00016f60
    void UpdatePassenger(const void *data);
    // samp.dll RVA: 0x000154a0
    void UpdateAim(const void *data);
    // samp.dll RVA: 0x00014660
    void OnEnterVehicle();
    // samp.dll RVA: 0x00014730
    void OnExitVehicle();
    // samp.dll RVA: 0x00017120
    void Chat(char *text);
    // samp.dll RVA: 0x00016250
    CRemotePlayer();
    // samp.dll RVA: 0x000162c0
    ~CRemotePlayer();
    // samp.dll RVA: 0x00015c60
    void ResetData();
    // Native distance returns extended ST0; the other ped has no extra null guard.
    // samp.dll RVA: 0x00015d50
    float GetDistanceToPlayer(CRemotePlayer *other);
    // samp.dll RVA: 0x00015dd0
    float GetDistanceToLocalPlayer();
    // samp.dll RVA: 0x00015e00
    void SetColor(DWORD color);
    // samp.dll RVA: 0x00015e20
    DWORD GetColorAsRGBA();
    // samp.dll RVA: 0x00015e50
    void EnterVehicle(WORD id,BOOL passenger);
    // samp.dll RVA: 0x00015ee0
    void ExitVehicle();
    // samp.dll RVA: 0x00015f10
    void ChangeState(BYTE newState,BYTE oldState);
    // samp.dll RVA: 0x00017080
    void Kill();
    // samp.dll RVA: 0x00015e30
    DWORD GetColorAsARGB();
    // samp.dll RVA: 0x00015fd0
    int GetStatus();
    // Requires a ped pointer and nonzero native state byte.
    // samp.dll RVA: 0x00001080
    BOOL IsActive();
    // Native on-foot attachment/surfing matrix pass.
    // samp.dll RVA: 0x000144d0
    void ApplySurfingMatrix();
    // samp.dll RVA: 0x00017040
    void Remove();
};
#pragma pack(pop)
typedef char RemotePlayerExtent[(sizeof(CRemotePlayer)==0x1fd)?1:-1];
// samp.dll RVA: 0x001181e4
extern const float g_remoteAbsentDistance;
// samp.dll RVA: 0x001181e8
extern const char g_remoteCarJackedText[];
// samp.dll RVA: 0x001181c8
extern const float g_remoteAimZoomScale;
// samp.dll RVA: 0x001181cc
extern const float g_remoteAimAspectScale;
// Raw native switch, default1. Other native writers remain unreviewed.
// samp.dll RVA: 0x0013c4a0
extern DWORD g_remoteClampTurnSpeed;
// samp.dll RVA: 0x001181bc
extern const float g_remoteTurnSpeedMinimum;
// Raw correction coefficients; native default values, other writers unreviewed.
// samp.dll RVA: 0x0013c498
extern float g_remoteOnfootCorrection;
// samp.dll RVA: 0x0013c49c
extern float g_remoteIncarCorrection;
// samp.dll RVA: 0x0011797c
extern const float g_remoteIncarTeleportDistance;
// samp.dll RVA: 0x00117874
extern const float g_remoteHeadDistance;
// samp.dll RVA: 0x00117868
extern const char g_remoteHeadTask[];
#endif
