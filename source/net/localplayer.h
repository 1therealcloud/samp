#ifndef SAMP_LOCAL_PLAYER_INTERFACE_H
#define SAMP_LOCAL_PLAYER_INTERFACE_H
#include <windows.h>
// Native allocation extent0x328; unreviewed fields/networking lifecycle remain open.
class CPed;
class CEntity;
class CVehicle;
struct PedBulletData;
#pragma pack(push,1)
class CLocalPlayer {public:
    BYTE nativeState[0x328];
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
#endif
