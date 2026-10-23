#ifndef SAMP_NETGAME_H
#define SAMP_NETGAME_H
#include <windows.h>
#include <stddef.h>
#include "game/scripting.h"
// The pool set and its owning field are established; other state remains opaque.
class CVehiclePool;
class CPlayerPool;
class CMenuPool;
class CObjectPool;
class CActorPool;
class CPickupPool;
class CGangZonePool;
class CLabelPool;
class CTextDrawPool;
#pragma pack(push,1)
struct CNetGamePools {
    CMenuPool *menus;CActorPool *actors;CPlayerPool *players;CVehiclePool *vehicles;
    CPickupPool *pickups;CObjectPool *objects;CGangZonePool *gangZones;CLabelPool *labels;CTextDrawPool *textDraws;
};
class CNetGame {
public:
    // samp.dll RVA: 0x00008230
    void InitializePools();
    // samp.dll RVA: 0x00008cd0
    void ResetPlayerPool();
    // samp.dll RVA: 0x00008d70
    void ResetVehiclePool();
    // samp.dll RVA: 0x00008e10
    void ResetTextDrawPool();
    // samp.dll RVA: 0x00008eb0
    void ResetObjectPool();
    // samp.dll RVA: 0x00008f50
    void ResetGangZonePool();
    // samp.dll RVA: 0x00008ff0
    void ResetPickupPool();
    // samp.dll RVA: 0x00009050
    void ResetMenuPool();
    // samp.dll RVA: 0x000090e0
    void ResetLabelPool();
    // samp.dll RVA: 0x00009180
    void ResetActorPool();
    // samp.dll RVA: 0x00008ca0
    void RemoveMapIcon(BYTE index);
    // samp.dll RVA: 0x000081f0
    void SetGameState(DWORD state);
    // samp.dll RVA: 0x000081d0
    CPickupPool *GetPickupPool();
    // Native tail adapter exposes the complete EDX:EAX counter result.
    // samp.dll RVA: 0x000085d0
    __int64 GetCounter();
    // samp.dll RVA: 0x00008c00
    void UpdatePlayersInfo();
    // samp.dll RVA: 0x000081e0
    CMenuPool *GetMenuPool();
    // samp.dll RVA: 0x00002e40
    CObjectPool *GetObjectPool();
    // Native coordinator allocation extent; internal layout and lifecycle remain open.
    union {
        BYTE nativeState[0x3e2];
        struct {BYTE unknown00[0x3de];CNetGamePools *pools;};
    };
    // samp.dll RVA: 0x0000b610
    CNetGame(const char *host,int port,const char *nickname,const char *password);
    // Address-only command dependency.
    // samp.dll RVA: 0x00008b80
    void ToggleNameTagStatus();
    // samp.dll RVA: 0x0000b290
    void Process();
    // Native destructor entry. The full networking lifecycle remains open.
    // samp.dll RVA: 0x00009570
    void Destroy();
    // samp.dll RVA: 0x00001180
    CVehiclePool *GetVehiclePool();
    // samp.dll RVA: 0x00001170
    CPlayerPool *GetPlayerPool();
};
#pragma pack(pop)
typedef char NetGamePoolSetExtent[(sizeof(CNetGamePools)==0x24)?1:-1];
typedef char NetGamePoolsOffset[(offsetof(CNetGame,pools)==0x3de)?1:-1];
typedef char NetGameExtent[(sizeof(CNetGame)==0x3e2)?1:-1];
// samp.dll RVA: 0x00156b38
extern DWORD g_netPlayersInfoTick;
// samp.dll RVA: 0x00117b98
extern int g_netPlayersInfoRpc;
// samp.dll RVA: 0x001179c8
extern const Scripting::OpcodeInfo g_netRemoveMapIconCommand;
#endif
