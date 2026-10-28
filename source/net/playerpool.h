#ifndef SAMP_PLAYER_POOL_H
#define SAMP_PLAYER_POOL_H
#include <stddef.h>
#include <string>
#include <windows.h>
// Native allocation extent0x2F3E; lifecycle and some field semantics remain open.
class CRemotePlayer;
class CLocalPlayer;
class CObject;
class CPlayerInfo;
struct CVector;
#pragma pack(push, 1)
class CPlayerPool
{
public:
    // 0x0000e340
    void SetScore(WORD id, int value);
    // 0x0000e380
    void SetPing(WORD id, DWORD value);

    // 0x0000e330
    void SetLocalId(WORD id);

    // 0x00002dd0
    int GetLargestId();
    // 0x00002de0
    WORD GetLocalId();

    // Descriptive name: native returns a raw player-info pointer, not a BOOL.
    // Native accepts adjacent ID1004 without checking connected state.
    // 0x000010d0
    CPlayerInfo* GetPlayerInfo(WORD player);
    // 0x0000a1d0
    const char* GetLocalPlayerName();
    // Native adjusts this by2 to the embedded VC7 string; external B580 anchor does not.
    // 0x0000b5e0
    void SetLocalPlayerName(const char* text);
    // Raw slot write, including adjacent ID1004; no ownership/connected-state change.
    // 0x00013630
    void SetAt(WORD player, CPlayerInfo* info);
    // 0x000136c0
    BOOL Process();
    // Fourth raw word is copied into both native ray records.
    // 0x00013c50
    WORD FindInRay(float maxCameraDistance, const CVector* origin, const CVector* end, DWORD rayWord);
    WORD localId;
    std::string localName;
    CLocalPlayer* local;
    int largestId;
    CPlayerInfo* players[1004];
    DWORD connected[1004];
    DWORD savedCollision[1004];
    DWORD localPing;
    int localScore;
    // Connected slots must equal1, not merely be nonzero.
    // 0x000139f0
    void Deactivate();
    // Native admits adjacent ID1004, without a connected-state requirement.
    // 0x0001da50
    void SetName(WORD player, const char* text);
    // 0x00013e80
    CPlayerPool();
    // 0x00013fd0
    ~CPlayerPool();
    // Native constructs the info before validating the inclusive ID1004.
    // 0x00014100
    BOOL Create(WORD id, const char* name, DWORD npc);
    // Native reason word is ignored; this entry pops eight bytes.
    // 0x00013f40
    BOOL Delete(WORD id, BYTE reason);
    // 0x00013650
    void UpdateLargestId();
    // 0x00013b50
    void ForceCollision();
    // 0x00013bd0
    void RestoreCollision();
    // 0x000138c0
    int GetCount(BOOL includeFlagged);
    // 0x0006e290
    int GetScore(WORD player);
    // 0x0006e2b0
    DWORD GetPing(WORD player);
    // 0x0006e2e0
    int GetLocalScore();
    // 0x0006e2f0
    DWORD GetLocalPing();
    // MSVC small-string storage; native accepts the adjacent ID1004 as well.
    // 0x000170d0
    const char* GetName(WORD player);
    // Local accessories first, then connected remote-player accessories.
    // 0x00013a40
    CObject* FindAccessory(void* backend);
    // Observed pose/accessory frame pass; complete pool layout remains unknown.
    // 0x00013840
    void Native013840();
    // Observed lookup prefix; no complete allocation layout.
    // 0x000137c0
    WORD Find(void* ped);
    // Native accepts player <=1004; allocation adjacency is not established here.
    // 0x000010f0
    CRemotePlayer* GetPlayer(WORD player);
    // 0x00001a80
    CLocalPlayer* GetLocalPlayer();
    // Returns the raw slot DWORD, not a normalized bool.
    // 0x000010b0
    DWORD IsConnected(WORD player);
    // Raw player-info DWORD +4; broader flag semantics not established.
    // 0x0000e300
    DWORD GetPlayerInfoFlag(WORD player);
};
#pragma pack(pop)
typedef char PlayerPoolLocalNameOffset[(offsetof(CPlayerPool, localName) == 2) ? 1 : -1];
typedef char PlayerPoolLocalOffset[(offsetof(CPlayerPool, local) == 0x1e) ? 1 : -1];
typedef char PlayerPoolPlayersOffset[(offsetof(CPlayerPool, players) == 0x26) ? 1 : -1];
typedef char PlayerPoolConnectedOffset[(offsetof(CPlayerPool, connected) == 0xfd6) ? 1 : -1];
typedef char PlayerPoolCollisionOffset[(offsetof(CPlayerPool, savedCollision) == 0x1f86) ? 1 : -1];
typedef char PlayerPoolExtent[(sizeof(CPlayerPool) == 0x2f3e) ? 1 : -1];
// 0x00156b84
extern DWORD g_playerPoolProcessCount;
// 0x0011817c
extern const char g_playerPoolRemoteException[];
// 0x00118174
extern const char g_playerPoolLocalException[];
#endif

#ifndef SAMP_CACHED_ACTIVE_PLAYER_COUNT_H
#define SAMP_CACHED_ACTIVE_PLAYER_COUNT_H
// 0x00156ac0
extern DWORD g_cachedActivePlayerCount;
// 0x00156ac4
extern DWORD g_activePlayerCountCountdown;
// No this pointer consumed; original class ownership not established.
// 0x00003ff0
DWORD GetCachedActivePlayerCount();
#endif
