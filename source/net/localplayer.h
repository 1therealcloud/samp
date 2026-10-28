#ifndef SAMP_LOCAL_PLAYER_INTERFACE_H
#define SAMP_LOCAL_PLAYER_INTERFACE_H
#include <windows.h>
// Native allocation extent0x328; unreviewed fields/networking lifecycle remain open.
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

    BYTE nativeState[0x328];
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
    // The raw DWORD state is retained; this is not normalized to bool.
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
    // Descriptive native surfing helpers; complete local-player layout is open.
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
typedef char LocalPlayerExtent[(sizeof(CLocalPlayer) == 0x328) ? 1 : -1];
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
#endif
