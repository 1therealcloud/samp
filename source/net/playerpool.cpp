#include "net/playerpool.h"
#include "net/remoteplayer.h"
#include "net/actorpool.h"
#include "game/playerped.h"
#include "net/playerinfo.h"
#include "net/localplayer.h"
// samp.dll RVA: 0x00013e80
CPlayerPool::CPlayerPool() {
    for(WORD id=0;id<1004;id++){players[id]=0;connected[id]=0;}
    local=new CLocalPlayer;localScore=0;localPing=0;localId=0;largestId=0;
}
// samp.dll RVA: 0x00013650
void CPlayerPool::UpdateLargestId() {
    int largest=0;for(int id=0;id<1004;id++)if(connected[id])largest=id;largestId=largest;
}
// samp.dll RVA: 0x00013b50
void CPlayerPool::ForceCollision() {
    int highest=largestId;
    for(WORD id=0;(int)id<=highest;id++) {
        if(id>=1004||connected[id]!=1)continue;
        CRemotePlayer *remote=players[id]?players[id]->remote:0;
        CPed *ped=*(CPed**)((BYTE*)remote+4);
        if(ped&&!ped->IsInVehicle()){savedCollision[id]=ped->GetCollisionFlag();ped->SetCollisionFlag(TRUE);}
    }
}
// samp.dll RVA: 0x00013bd0
void CPlayerPool::RestoreCollision() {
    int highest=largestId;
    for(WORD id=0;(int)id<=highest;id++) {
        if(id>=1004||connected[id]!=1)continue;
        CRemotePlayer *remote=players[id]?players[id]->remote:0;
        CPed *ped=*(CPed**)((BYTE*)remote+4);
        if(ped&&!ped->IsInVehicle())ped->SetCollisionFlag(savedCollision[id]!=0);
    }
}
// samp.dll RVA: 0x00001a80
CLocalPlayer *CPlayerPool::GetLocalPlayer() { return *(CLocalPlayer**)((BYTE*)this+0x1e); }
// samp.dll RVA: 0x000138c0
int CPlayerPool::GetCount(BOOL includeFlagged) {
    int count=0;BYTE *state=(BYTE*)this;
    for(int id=0;id<1004;id++)if(*(DWORD*)(state+0xfd6+id*4)==1) {
        if(includeFlagged||!*(DWORD*)(*(BYTE**)(state+0x26+id*4)+4))count++;
    }
    return count;
}
// samp.dll RVA: 0x0006e290
int CPlayerPool::GetScore(WORD player) {
    if(player>1004)return 0;
    BYTE *entry=*(BYTE**)((BYTE*)this+0x26+player*4);
    return entry?*(int*)entry:0;
}
// samp.dll RVA: 0x0006e2b0
DWORD CPlayerPool::GetPing(WORD player) {
    if(player>1004)return 0;
    BYTE *entry=*(BYTE**)((BYTE*)this+0x26+player*4);
    return entry?*(DWORD*)(entry+0xc):0;
}
// samp.dll RVA: 0x0006e2e0
int CPlayerPool::GetLocalScore() {return *(int*)((BYTE*)this+0x2f3a);}
// samp.dll RVA: 0x0006e2f0
DWORD CPlayerPool::GetLocalPing() {return *(DWORD*)((BYTE*)this+0x2f36);}
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

// samp.dll RVA: 0x00013a40
CObject *CPlayerPool::FindAccessory(void *backend) {
    BYTE *state=(BYTE*)this;
    BYTE *local=*(BYTE**)(state+0x1e);
    CPed *ped=local?*(CPed**)local:0;
    if(ped)for(int slot=0;slot<10;slot++) {
        if(!*(volatile BOOL*)&ped->m_accessoryOccupied[slot])continue;
        CEntity *object=(CEntity*)ped->m_accessoryObjects[slot];
        if(object&&object->m_pGameEntity==backend)
            return *(volatile BOOL*)&ped->m_accessoryOccupied[slot]?(CObject*)ped->m_accessoryObjects[slot]:0;
    }
    int highest=*(int*)(state+0x22);
    for(WORD player=0;(int)player<=highest;++player) {
        if(player>=1004||*(DWORD*)(state+0xfd6+player*4)!=1)continue;
        BYTE *entry=*(BYTE**)(state+0x26+player*4);
        BYTE *remote=entry?*(BYTE**)(entry+8):0;
        // Native dereferences remote+4 even for inconsistent records.
        ped=*(CPed**)(remote+4);
        if(!ped)continue;
        for(int slot=0;slot<10;slot++) {
            if(!*(volatile BOOL*)&ped->m_accessoryOccupied[slot])continue;
            CEntity *object=(CEntity*)ped->m_accessoryObjects[slot];
            if(object&&object->m_pGameEntity==backend)
                return *(volatile BOOL*)&ped->m_accessoryOccupied[slot]?(CObject*)ped->m_accessoryObjects[slot]:0;
        }
    }
    return 0;
}

// samp.dll RVA: 0x000170d0
const char *CPlayerPool::GetName(WORD player) {
    BYTE *state=(BYTE*)this;
    if(player==*(WORD*)state)
        return *(DWORD*)(state+0x1a)>=16?*(char**)(state+6):(char*)state+6;
    if(player>1004)return 0;
    BYTE *entry=*(BYTE**)(state+0x26+player*4);
    if(!entry)return 0;
    return *(DWORD*)(entry+0x28)>=16?*(char**)(entry+0x14):(char*)entry+0x14;
}
