#ifndef SAMP_ACTOR_POOL_ADDRESS_INTERFACE_H
#define SAMP_ACTOR_POOL_ADDRESS_INTERFACE_H
#include <windows.h>
// Observed lookup prefix only; never allocate this address interface.
class CActorPool {public:
    // samp.dll RVA: 0x000018b0
    DWORD Find(void *ped);
};
#endif
