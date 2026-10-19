#ifndef SAMP_PLAYER_POOL_ADDRESS_INTERFACE_H
#define SAMP_PLAYER_POOL_ADDRESS_INTERFACE_H
#include <windows.h>
// Address-only dependency. No fields, allocation extent or complete interface
// are established by this declaration; never instantiate this class.
class CRemotePlayer;
class CPlayerPool {
public:
    // Observed thiscall edge after GTA world processing; body not recovered.
    // samp.dll RVA: 0x00013840
    void Native013840();
    // SEH recovery dependencies, declaration-only.
    // samp.dll RVA: 0x000137c0
    WORD Find(void *ped);
    // Native accepts player <=1004; allocation adjacency is not established here.
    // samp.dll RVA: 0x000010f0
    CRemotePlayer *GetPlayer(WORD player);
};
#endif
