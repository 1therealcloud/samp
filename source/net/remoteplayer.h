#ifndef SAMP_REMOTE_PLAYER_ADDRESS_INTERFACE_H
#define SAMP_REMOTE_PLAYER_ADDRESS_INTERFACE_H
#include <windows.h>
struct CVector;
// Native allocation extent0x1FD; unreviewed state remains opaque.
#pragma pack(push,1)
class CRemotePlayer {public:
    BYTE nativeState[0x1fd];
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
#endif
