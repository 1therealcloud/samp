#ifndef SAMP_NETGAME_H
#define SAMP_NETGAME_H
#include <windows.h>
// The network constructor extent is known; its internal layout and lifecycle remain open.
class CVehiclePool;
class CPlayerPool;
class CMenuPool;
class CObjectPool;
class CNetGame {
public:
    // samp.dll RVA: 0x00008c00
    void UpdatePlayersInfo();
    // samp.dll RVA: 0x000081e0
    CMenuPool *GetMenuPool();
    // samp.dll RVA: 0x00002e40
    CObjectPool *GetObjectPool();
    // Native coordinator allocation extent; internal layout and lifecycle remain open.
    BYTE nativeState[0x3e2];
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
// samp.dll RVA: 0x00156b38
extern DWORD g_netPlayersInfoTick;
// samp.dll RVA: 0x00117b98
extern int g_netPlayersInfoRpc;
#endif
