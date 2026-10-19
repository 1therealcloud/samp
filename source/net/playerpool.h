#ifndef SAMP_PLAYER_POOL_ADDRESS_INTERFACE_H
#define SAMP_PLAYER_POOL_ADDRESS_INTERFACE_H
// Address-only dependency. No fields, allocation extent or complete interface
// are established by this declaration; never instantiate this class.
class CPlayerPool {
public:
    // Observed thiscall edge after GTA world processing; body not recovered.
    // samp.dll RVA: 0x00013840
    void Native013840();
};
#endif
