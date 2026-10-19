#ifndef SAMP_PLAYER_POOL_ADDRESS_INTERFACE_H
#define SAMP_PLAYER_POOL_ADDRESS_INTERFACE_H
#include <windows.h>
// Address-only dependency. No fields, allocation extent or complete interface
// are established by this declaration; never instantiate this class.
class CRemotePlayer;
class CLocalPlayer;
class CPlayerPool {
public:
    // Observed thiscall edge after GTA world processing; body not recovered.
    // samp.dll RVA: 0x00013840
    void Native013840();
    // Observed lookup prefix; no complete allocation layout.
    // samp.dll RVA: 0x000137c0
    WORD Find(void *ped);
    // Native accepts player <=1004; allocation adjacency is not established here.
    // samp.dll RVA: 0x000010f0
    CRemotePlayer *GetPlayer(WORD player);
    // samp.dll RVA: 0x00001a80
    CLocalPlayer *GetLocalPlayer();
    // Returns the raw slot DWORD, not a normalized bool.
    // samp.dll RVA: 0x000010b0
    DWORD IsConnected(WORD player);
    // Raw player-info DWORD +4; broader flag semantics not established.
    // samp.dll RVA: 0x0000e300
    DWORD GetPlayerInfoFlag(WORD player);
};
#endif
