#include "net/playerpool.h"
#include "net/remoteplayer.h"
#include "net/actorpool.h"
#include "game/playerped.h"
// samp.dll RVA: 0x000010f0
CRemotePlayer *CPlayerPool::GetPlayer(WORD player) {
    if(player<1005) {
        BYTE *entry=*(BYTE**)((BYTE*)this+0x26+player*4);
        if(entry)return *(CRemotePlayer**)(entry+8);
    }
    return 0;
}
// samp.dll RVA: 0x000010b0
DWORD CPlayerPool::IsConnected(WORD player) {
    if(player>1003)return 0;
    return *(DWORD*)((BYTE*)this+0xfd6+player*4);
}
// samp.dll RVA: 0x0000e300
DWORD CPlayerPool::GetPlayerInfoFlag(WORD player) {
    if(player<1005) {
        BYTE *entry=*(BYTE**)((BYTE*)this+0x26+player*4);
        if(entry)return *(DWORD*)(entry+4);
    }
    return 0;
}
// samp.dll RVA: 0x000137c0
WORD CPlayerPool::Find(void *ped) {
    BYTE *state=(BYTE*)this;
    for(WORD player=0;(int)player<=*(int*)(state+0x22);++player) {
        if(player<1004 && *(DWORD*)(state+0xfd6+player*4)==1) {
            BYTE *entry=*(BYTE**)(state+0x26+player*4);
            BYTE *remote=entry?*(BYTE**)(entry+8):0;
            // Native dereferences remote+4 even for inconsistent/null records.
            CPed *wrapper=*(CPed**)(remote+4);
            if(wrapper && wrapper->m_pGamePed && wrapper->m_pGamePed==ped) {
                entry=*(BYTE**)(state+0x26+player*4);
                if(!entry)return *(WORD*)0;
                return **(WORD**)(entry+8);
            }
        }
    }
    return 0xffff;
}

// samp.dll RVA: 0x00013840
void CPlayerPool::Native013840() {
    BYTE *state=(BYTE*)this;
    BYTE *local=*(BYTE**)(state+0x1e);
    if(local&&*(CPed**)local) {
        (*(CPed**)local)->ProcessAnimationPose();
        local=*(BYTE**)(state+0x1e);
        (*(CPed**)local)->ProcessAccessories();
    }
    // Native captures the highest ID once; the WORD counter wraps naturally.
    int highest=*(int*)(state+0x22);
    for(WORD player=0;(int)player<=highest;++player) {
        if(player>=1004||*(DWORD*)(state+0xfd6+player*4)!=1)continue;
        BYTE *entry=*(BYTE**)(state+0x26+player*4);
        BYTE *remote=entry?*(BYTE**)(entry+8):0;
        // Preserve the native unchecked remote+4 read for inconsistent records.
        if(*(CPed**)(remote+4)) {
            (*(CPed**)(remote+4))->ProcessAnimationPose();
            (*(CPed**)(remote+4))->ProcessAccessories();
        }
    }
}
