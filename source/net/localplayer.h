#ifndef SAMP_LOCAL_PLAYER_INTERFACE_H
#define SAMP_LOCAL_PLAYER_INTERFACE_H
#include <windows.h>
// Address-only interface; full class layout/networking lifecycle remains open.
class CPed;
class CEntity;
struct PedBulletData;
class CLocalPlayer {public:
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
#endif
