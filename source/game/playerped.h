#ifndef SAMP_RECOVERED_PED_H
#define SAMP_RECOVERED_PED_H
#include "game/entity.h"
#pragma pack(push,1)
struct PedAccessoryState { BYTE opaque[52]; };
// samp.dll vtable RVA: 0x0011f24c
class CPed : public CEntity {
public:
    BOOL m_bUsingCellphone;
    BOOL m_accessoryOccupied[10];
    PedAccessoryState m_accessoryInfo[10];
    void *m_accessoryObjects[10];
    void *m_pGamePed;
    DWORD unknown2a8,unknown2ac;
    BYTE m_nPlayerNumber;
    BYTE unknown2b1[8];
    DWORD m_parachuteObject;
    BYTE unknown2bd[4];
    DWORD m_stuffType,m_stuffObject,m_drunkLevel,m_arrow;
    BYTE unknown2d1;
    DWORD unknown2d2;
    BYTE unknown2d6[8];
    BOOL m_bDancing;
    DWORD m_danceStyle,m_lastDanceMove;
    BYTE unknown2ea[8];
    BOOL m_bUrinating;
    BYTE unknown2f6,unknown2f7,unknown2f8;
    DWORD unknown2f9;
    BYTE unknown2fd[48];
    // samp.dll RVA: 0x000ab690
    CPed();
    // samp.dll RVA: 0x000abd60
    void * GetCurrentWeaponSlot();
    // samp.dll RVA: 0x000ad480
    BOOL IsOnGround();
    // samp.dll RVA: 0x000aba70
    void SetState(BYTE state);
    // samp.dll RVA: 0x000ab9b0
    float GetArmour();
    // samp.dll RVA: 0x000ab970
    float GetHealth();
    // samp.dll RVA: 0x000ab8b0
    DWORD GetVehicleRef();
    // samp.dll RVA: 0x000aba10
    void SetFlags(DWORD flags);
    // samp.dll RVA: 0x000ab9d0
    void SetArmour(float armour);
    // samp.dll RVA: 0x000ab990
    void SetHealth(float health);
    // samp.dll RVA: 0x000ab7d0
    void ResetPointers();
    // samp.dll RVA: 0x000ab9f0
    DWORD GetFlags();
    // samp.dll RVA: 0x000ad760
    BOOL DoesDancing();
    // samp.dll RVA: 0x000adbb0
    void SetDrunkLevel(DWORD level);
    // samp.dll RVA: 0x000abc20
    void * GetVehicle();
    // samp.dll RVA: 0x000aba60
    BYTE GetState();
    // samp.dll RVA: 0x000ade60
    BOOL DoesUrinating();
    // samp.dll RVA: 0x000ad890
    DWORD GetStuff();
    // samp.dll RVA: 0x000adca0
    BOOL UsingCellphone();
    // samp.dll RVA: 0x000b0ea0
    virtual ~CPed();
    // samp.dll RVA: 0x000b0ba0
    void Destroy();
    // samp.dll RVA: 0x000aea30
    BOOL HasAccessory();
    // samp.dll RVA: 0x000b06b0
    void DeleteAllAccessories();
    // samp.dll RVA: 0x000ad800
    void DropStuff();
    // samp.dll RVA: 0x000ac190
    void WarpFromVehicle(float x,float y,float z);
    // samp.dll RVA: 0x000aba90
    BOOL IsInVehicle();
    // samp.dll RVA: 0x000abbe0
    BOOL IsPassenger();
    // samp.dll RVA: 0x000adba0
    DWORD GetDrunkLevel();
    // Observed task-type test; does not establish a complete GTA task interface.
    // samp.dll RVA: 0x000ac8f0
    BOOL HasTaskType268();
    // Descriptive predicates for native dance state/style fields.
    // samp.dll RVA: 0x000acb10
    BOOL HasDanceState();
    // samp.dll RVA: 0x000aca60
    BOOL HasDanceStyle();
    // samp.dll RVA: 0x000ab870
    DWORD GetCurrentWeapon();
    // samp.dll RVA: 0x000abc80
    void SetArmedWeapon(DWORD weapon,BYTE direct);
};
#pragma pack(pop)
typedef char PedExtent[(sizeof(CPed)==0x32d)?1:-1];
typedef char PedBackendOffset[(offsetof(CPed,m_pGamePed)==0x2a4)?1:-1];
typedef char PedPlayerOffset[(offsetof(CPed,m_nPlayerNumber)==0x2b0)?1:-1];
// samp.dll RVA: 0x002a9da8
extern void *g_gamePlayerPeds[210];
// samp.dll RVA: 0x002aa0f0
extern BYTE g_gamePlayerControllerState[210][16];
// samp.dll RVA: 0x000b3860
void *__stdcall GameFindLocalPed();
// samp.dll RVA: 0x000b3f20
void __stdcall GameAssignPlayerPed(BYTE player,void *ped);
// samp.dll RVA: 0x000b3770
void *__stdcall GamePoolPedGetAt(int handle);
#endif
