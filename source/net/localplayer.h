#ifndef SAMP_LOCAL_PLAYER_INTERFACE_H
#define SAMP_LOCAL_PLAYER_INTERFACE_H
#include <windows.h>
// Native allocation extent0x328; unreviewed fields/networking lifecycle remain open.
class CPed;
class CEntity;
class CVehicle;
struct PedBulletData;
namespace Scripting {struct OpcodeInfo;}
#pragma pack(push,1)
class CLocalPlayer {public:
    // samp.dll RVA: 0x00002db0
    DWORD GetSpawnPermission();
    // samp.dll RVA: 0x000135c0
    DWORD IsSpectating();

    BYTE nativeState[0x328];
    // samp.dll RVA: 0x000072d0
    BOOL Process();
    // samp.dll RVA: 0x00002fd0
    void ProcessHead();
    // samp.dll RVA: 0x00003e40
    void ProcessWorldBoundaries();
    // samp.dll RVA: 0x00003ea0
    void ProcessVehicleWorldBoundaries();
    // samp.dll RVA: 0x00003f60
    void PrepareForClassSelection();
    // samp.dll RVA: 0x00005c20
    void ProcessVehicleDamage(WORD vehicle);
    // samp.dll RVA: 0x00006de0
    void ProcessPassengerEntry();
    // samp.dll RVA: 0x00004a30
    WORD GetAimedPlayer();
    // samp.dll RVA: 0x00004a60
    WORD GetAimedActor();
    // samp.dll RVA: 0x00005f40
    void ProcessClassSelection();
    // samp.dll RVA: 0x000060d0
    void UpdateWeapons();
    // samp.dll RVA: 0x00006360
    void ProcessSpectating();
    // Native returns true in AL; this pointer is unused.
    // samp.dll RVA: 0x00004300
    bool GetCameraTarget(WORD *targets);
    // samp.dll RVA: 0x000045c0
    void UpdateCameraTarget();
    // samp.dll RVA: 0x00004710
    void DrawCameraTargetLabels();
    // samp.dll RVA: 0x00003390
    BYTE GetSpecialAction();
    // samp.dll RVA: 0x00004db0
    void SendOnfootData();
    // samp.dll RVA: 0x00006e80
    void SendIncarData();
    // samp.dll RVA: 0x000041c0
    DWORD NeedsToSendOnfootData(WORD keys,WORD upDown,WORD leftRight);
    // samp.dll RVA: 0x000041f0
    DWORD NeedsToSendIncarData(WORD keys,WORD upDown,WORD leftRight);
    // samp.dll RVA: 0x00005090
    void SendAimData();
    // samp.dll RVA: 0x00005240
    void SendTrailerData(WORD vehicle);
    // samp.dll RVA: 0x00005400
    void SendPassengerData();
    // samp.dll RVA: 0x00005660
    void WastedNotification();
    // samp.dll RVA: 0x00005b50
    void SendStats();
    // samp.dll RVA: 0x00004bd0
    void SendUnoccupiedData(WORD vehicle,BYTE seat);
    // Native bool result in AL; deliberately retains player0 selection behavior.
    // samp.dll RVA: 0x00006c10
    bool ProcessUnoccupiedSync(WORD id,CVehicle *vehicle);
    // samp.dll RVA: 0x0000e2b0
    void ClearSurfingState();
    // samp.dll RVA: 0x00003fb0
    void PrepareForClassSelection_Outcome(BOOL outcome);
    // Copies the raw50-byte spawn record forward and marks it available.
    // samp.dll RVA: 0x00003af0
    void SetSpawnInfo(const void *info);
    // samp.dll RVA: 0x00004af0
    CLocalPlayer();
    // samp.dll RVA: 0x00002ec0
    void ResetData();
    // samp.dll RVA: 0x00003110
    void SetSpecialAction(BYTE action);
    // samp.dll RVA: 0x00003b20
    BOOL Spawn();
    // The raw DWORD state is retained; this is not normalized to bool.
    // samp.dll RVA: 0x00004080
    void ToggleSpectating(BOOL enabled);
    // samp.dll RVA: 0x000040f0
    void SpectateVehicle(WORD vehicle);
    // samp.dll RVA: 0x00004140
    void SpectatePlayer(WORD player);
    // samp.dll RVA: 0x00003de0
    void SetColor(DWORD color);
    // samp.dll RVA: 0x00003e00
    DWORD GetColorAsRGBA();
    // Raw DWORD result: returns1 after500ms or when any byte differs.
    // samp.dll RVA: 0x00003970
    DWORD HasDataChanged(const void *previous,const void *current,DWORD length);
    // samp.dll RVA: 0x00003a70
    void SendSpawnRequest();
    // samp.dll RVA: 0x00005720
    void RequestClass(int selection);
    // samp.dll RVA: 0x000057c0
    void SendInteriorChange(BYTE interior);
    // samp.dll RVA: 0x00003e20
    DWORD GetColorAsARGB();
    // samp.dll RVA: 0x000039c0
    DWORD GetInCarSendRate();
    // samp.dll RVA: 0x00003a00
    DWORD GetOnFootSendRate();
    // samp.dll RVA: 0x00003a40
    DWORD GetSendRate();
    // Descriptive native surfing helpers; complete local-player layout is open.
    // samp.dll RVA: 0x000034b0
    void ApplySurfingPhysics();
    // samp.dll RVA: 0x00003630
    void SetSurfingEntity(CEntity *entity,DWORD locked);
    // May transform data->offset in place for an unclassified backend target.
    // samp.dll RVA: 0x000069d0
    void SendBullet(PedBulletData *data);
    // samp.dll RVA: 0x00002da0
    CPed *GetPed();
    // Damage amount is a native float; take/give apply scaling before serialization.
    // samp.dll RVA: 0x00005920
    void EnterVehicle(WORD vehicle,BOOL passenger);
    // samp.dll RVA: 0x000066b0
    void SendTakeDamage(WORD player,float damage,DWORD weapon,DWORD bodyPart);
    // samp.dll RVA: 0x000067c0
    void SendGiveDamage(WORD player,float damage,DWORD weapon,DWORD bodyPart);
    // samp.dll RVA: 0x000068d0
    void SendActorDamage(WORD actor,float damage,DWORD weapon,DWORD bodyPart);
    // samp.dll RVA: 0x00005a40
    void ExitVehicle(int vehicle);
    // Network/pool dependency used only when Move finds a player pool.
    // samp.dll RVA: 0x00003650
    void ProcessSurfing();
    // samp.dll RVA: 0x00005860
    void Chat(char *text);
    // samp.dll RVA: 0x00003f40
    void RequestSpawn();
    // samp.dll RVA: 0x00005e30
    void NextClass();
    // samp.dll RVA: 0x00005ec0
    void PrevClass();
};
#pragma pack(pop)
typedef char LocalPlayerExtent[(sizeof(CLocalPlayer)==0x328)?1:-1];
// samp.dll RVA: 0x00117978
extern const float g_localUnoccupiedRange;
// samp.dll RVA: 0x00117948
extern const float g_localAimZoomScale;
// samp.dll RVA: 0x0011794c
extern const float g_localAimAspectScale;
// samp.dll RVA: 0x00117950
extern int g_localWastedRpc;
// samp.dll RVA: 0x00117968
extern int g_localVehicleDamageRpc;
// samp.dll RVA: 0x0011797c
extern const float g_localPassengerEntryDistance;
// samp.dll RVA: 0x00117888
extern const char g_localWorldBoundaryText[];
// samp.dll RVA: 0x00117854
extern const Scripting::OpcodeInfo g_localPedCameraCommand;
// samp.dll RVA: 0x001178ac
extern const float g_localCameraRayLength;
// samp.dll RVA: 0x001178b0
extern int g_localCameraTargetRpc;
// samp.dll RVA: 0x00117920
extern const char g_localCameraTargetObjectText[];
// samp.dll RVA: 0x001178fc
extern const char g_localCameraTargetVehicleText[];
// samp.dll RVA: 0x001178d8
extern const char g_localCameraTargetPlayerText[];
// samp.dll RVA: 0x001178b4
extern const char g_localCameraTargetActorText[];
// samp.dll RVA: 0x0013c0b0
extern DWORD g_localAimSendRate;
// samp.dll RVA: 0x00117980
extern const char g_localClassSelectionMessage[];
// samp.dll RVA: 0x00117944
extern const float g_localQuaternionSyncThreshold;
#endif
