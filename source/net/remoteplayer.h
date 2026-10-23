#ifndef SAMP_REMOTE_PLAYER_ADDRESS_INTERFACE_H
#define SAMP_REMOTE_PLAYER_ADDRESS_INTERFACE_H
// Address-only dependency. No allocation or complete interface established.
class CRemotePlayer {public:
    // Native on-foot attachment/surfing matrix pass.
    // samp.dll RVA: 0x000144d0
    void ApplySurfingMatrix();
    // samp.dll RVA: 0x00017040
    void Remove();
};
#endif
