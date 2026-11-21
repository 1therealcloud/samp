#ifndef PLAYERPOOL_H
#define PLAYERPOOL_H

#include <stddef.h>
#include <string>
#include <windows.h>

#include <static_assert.h>

// Packed native pool; inclusive ID1004 accesses retain observed adjacent storage behavior.
class CRemotePlayer;
class CLocalPlayer;
class CObject;
class CPlayerInfo;
struct CVector;
#pragma pack(push, 1)
class CPlayerPool
{
public:
    // R5 address to verify
    void SetScore(WORD id, int value);
    // R5 address to verify
    void SetPing(WORD id, DWORD value);

    // R5 address to verify
    void SetLocalId(WORD id);

    // R5 address to verify
    int GetLargestId();
    // R5 address to verify
    WORD GetLocalId();

    // Native accepts adjacent ID1004 without checking connected state.
    // 0x000010d0
    CPlayerInfo* GetPlayerInfo(WORD player);
    // 0x0000a4e0
    const char* GetLocalPlayerName();
    // Native adjusts this by2 to the embedded VC7 string; external B580 anchor does not.
    // R5 address to verify
    void SetLocalPlayerName(const char* text);
    // Raw slot write, including adjacent ID1004; no ownership/connected-state change.
    // 0x00013730
    void SetAt(WORD player, CPlayerInfo* info);
    // 0x000137c0
    BOOL Process();
    // Fourth raw word is copied into both native ray records.
    // 0x00013d90
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
    // 0x00013b10
    void Deactivate();
    // Native admits adjacent ID1004, without a connected-state requirement.
    // R5 address to verify
    void SetName(WORD player, const char* text);
    // 0x00013fd0
    CPlayerPool();
    // 0x00014120
    ~CPlayerPool();
    // Native constructs the info before validating the inclusive ID1004.
    // 0x00014250
    BOOL Create(WORD id, const char* name, DWORD npc);
    // Native reason word is ignored; this entry pops eight bytes.
    // 0x00014090
    BOOL Delete(WORD id, BYTE reason);
    // 0x00013750
    void UpdateLargestId();
    // 0x00013c90
    void ForceCollision();
    // 0x00013d10
    void RestoreCollision();
    // 0x000139f0
    int GetCount(BOOL includeFlagged);
    // 0x0006e850
    int GetScore(WORD player);
    // 0x0006e880
    DWORD GetPing(WORD player);
    // 0x0006e8b0
    int GetLocalScore();
    // 0x0006e8c0
    DWORD GetLocalPing();
    // MSVC small-string storage; native accepts the adjacent ID1004 as well.
    // 0x000175c0
    const char* GetName(WORD player);
    // Local accessories first, then connected remote-player accessories.
    // 0x00013b70
    CObject* FindAccessory(void* backend);
    // Local pose/accessories first, then remote slots whose connected DWORD equals1.
    // R5 address to verify
    void Native013840();
    // Matches the GTA ped backend and retains live pool reloads after backend calls.
    // 0x000138c0
    WORD Find(void* ped);
    // The original also accepts player ID 1004.
    // 0x000010f0
    CRemotePlayer* GetPlayer(WORD player);
    // 0x00001a40
    CLocalPlayer* GetLocalPlayer();
    // Returns the raw slot DWORD, not a normalized bool.
    // 0x000010b0
    DWORD IsConnected(WORD player);
    // Raw player-info DWORD +4; broader flag semantics not established.
    // R5 address to verify
    DWORD GetPlayerInfoFlag(WORD player);
};
#pragma pack(pop)
STATIC_ASSERT(offsetof(CPlayerPool, localName) == 2);
STATIC_ASSERT(offsetof(CPlayerPool, local) == 0x1e);
STATIC_ASSERT(offsetof(CPlayerPool, players) == 0x26);
STATIC_ASSERT(offsetof(CPlayerPool, connected) == 0xfd6);
STATIC_ASSERT(offsetof(CPlayerPool, savedCollision) == 0x1f86);
STATIC_ASSERT(sizeof(CPlayerPool) == 0x2f3e);
// R5 address to verify
extern DWORD g_playerPoolProcessCount;
// 0x000e60f0
extern const char g_playerPoolRemoteException[];
// 0x000e60e8
extern const char g_playerPoolLocalException[];
#endif

#ifndef SAMP_CACHED_ACTIVE_PLAYER_COUNT_H
#define SAMP_CACHED_ACTIVE_PLAYER_COUNT_H
// R5 address to verify
extern DWORD g_cachedActivePlayerCount;
// R5 address to verify
extern DWORD g_activePlayerCountCountdown;
// No this pointer consumed; original class ownership not established.
// R5 address to verify
DWORD GetCachedActivePlayerCount();

#endif // PLAYERPOOL_H
