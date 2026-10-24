#ifndef SAMP_PLAYER_POOL_H
#define SAMP_PLAYER_POOL_H
#include <windows.h>
#include <string>
#include <stddef.h>
// Native allocation extent0x2F3E; lifecycle and some field semantics remain open.
class CRemotePlayer;
class CLocalPlayer;
class CObject;
class CPlayerInfo;
#pragma pack(push,1)
class CPlayerPool {
public:
    WORD localId;std::string localName;CLocalPlayer *local;
    int largestId;CPlayerInfo *players[1004];DWORD connected[1004];
    DWORD savedCollision[1004];DWORD localPing;int localScore;
    // Native admits adjacent ID1004, without a connected-state requirement.
    // samp.dll RVA: 0x0001da50
    void SetName(WORD player,const char *text);
    // samp.dll RVA: 0x00013e80
    CPlayerPool();
    // samp.dll RVA: 0x00013fd0
    ~CPlayerPool();
    // Native constructs the info before validating the inclusive ID1004.
    // samp.dll RVA: 0x00014100
    BOOL Create(WORD id,const char *name,DWORD npc);
    // Native reason word is ignored; this entry pops eight bytes.
    // samp.dll RVA: 0x00013f40
    BOOL Delete(WORD id,BYTE reason);
    // samp.dll RVA: 0x00013650
    void UpdateLargestId();
    // samp.dll RVA: 0x00013b50
    void ForceCollision();
    // samp.dll RVA: 0x00013bd0
    void RestoreCollision();
    // samp.dll RVA: 0x000138c0
    int GetCount(BOOL includeFlagged);
    // samp.dll RVA: 0x0006e290
    int GetScore(WORD player);
    // samp.dll RVA: 0x0006e2b0
    DWORD GetPing(WORD player);
    // samp.dll RVA: 0x0006e2e0
    int GetLocalScore();
    // samp.dll RVA: 0x0006e2f0
    DWORD GetLocalPing();
    // MSVC small-string storage; native accepts the adjacent ID1004 as well.
    // samp.dll RVA: 0x000170d0
    const char *GetName(WORD player);
    // Local accessories first, then connected remote-player accessories.
    // samp.dll RVA: 0x00013a40
    CObject *FindAccessory(void *backend);
    // Observed pose/accessory frame pass; complete pool layout remains unknown.
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
#pragma pack(pop)
typedef char PlayerPoolLocalNameOffset[(offsetof(CPlayerPool,localName)==2)?1:-1];
typedef char PlayerPoolLocalOffset[(offsetof(CPlayerPool,local)==0x1e)?1:-1];
typedef char PlayerPoolPlayersOffset[(offsetof(CPlayerPool,players)==0x26)?1:-1];
typedef char PlayerPoolConnectedOffset[(offsetof(CPlayerPool,connected)==0xfd6)?1:-1];
typedef char PlayerPoolCollisionOffset[(offsetof(CPlayerPool,savedCollision)==0x1f86)?1:-1];
typedef char PlayerPoolExtent[(sizeof(CPlayerPool)==0x2f3e)?1:-1];
#endif
