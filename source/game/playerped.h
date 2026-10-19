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
    // samp.dll RVA: 0x000b0ea0
    virtual ~CPed();
    // Declaration only: complete destruction/accessory path remains open.
    // samp.dll RVA: 0x000b0ba0
    void Destroy();
    // samp.dll RVA: 0x000aba90
    BOOL IsInVehicle();
    // samp.dll RVA: 0x000abbe0
    BOOL IsPassenger();
    // samp.dll RVA: 0x000adba0
    DWORD GetDrunkLevel();
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
#endif
