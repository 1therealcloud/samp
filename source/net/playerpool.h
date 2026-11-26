#ifndef PLAYERPOOL_H
#define PLAYERPOOL_H

#include <stddef.h>
#include <string>
#include <windows.h>

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
    void SetScore(WORD id, int value);

    void SetPing(WORD id, DWORD value);

    void SetLocalId(WORD id);

    int GetLargestId();

    WORD GetLocalId();

    // Native accepts adjacent ID1004 without checking connected state.

    CPlayerInfo* GetPlayerInfo(WORD player);

    const char* GetLocalPlayerName();
    // Native adjusts this by2 to the embedded VC7 string; external B580 anchor does not.

    void SetLocalPlayerName(const char* text);
    // Raw slot write, including adjacent ID1004; no ownership/connected-state change.

    void SetAt(WORD player, CPlayerInfo* info);

    BOOL Process();
    // Fourth raw word is copied into both native ray records.

    WORD FindInRay(float maxCameraDistance, const CVector* origin, const CVector* end, DWORD rayWord);
    WORD localId;
    std::string localName;
#ifndef _DEBUG
    BYTE localNamePadding[4]; // Preserve the offsets used by native hooks.
#endif
    CLocalPlayer* local;
    int largestId;
    CPlayerInfo* players[1004];
    DWORD connected[1004];
    DWORD savedCollision[1004];
    DWORD localPing;
    int localScore;
    // Connected slots must equal1, not merely be nonzero.

    void Deactivate();
    // Native admits adjacent ID1004, without a connected-state requirement.

    void SetName(WORD player, const char* text);

    CPlayerPool();

    ~CPlayerPool();
    // Native constructs the info before validating the inclusive ID1004.

    BOOL Create(WORD id, const char* name, DWORD npc);
    // Native reason word is ignored; this entry pops eight bytes.

    BOOL Delete(WORD id, BYTE reason);

    void UpdateLargestId();

    void ForceCollision();

    void RestoreCollision();

    int GetCount(BOOL includeFlagged);

    int GetScore(WORD player);

    DWORD GetPing(WORD player);

    int GetLocalScore();

    DWORD GetLocalPing();
    // MSVC small-string storage; native accepts the adjacent ID1004 as well.

    const char* GetName(WORD player);
    // Local accessories first, then connected remote-player accessories.

    CObject* FindAccessory(void* backend);
    // Local pose/accessories first, then remote slots whose connected DWORD equals1.

    void ProcessPlayerPosesAndAccessories();
    // Matches the GTA ped backend and retains live pool reloads after backend calls.

    WORD Find(void* ped);
    // The original also accepts player ID 1004.

    CRemotePlayer* GetPlayer(WORD player);

    CLocalPlayer* GetLocalPlayer();
    // Returns the raw slot DWORD, not a normalized bool.

    DWORD IsConnected(WORD player);
    // Raw player-info DWORD +4; broader flag semantics not established.

    DWORD GetPlayerInfoFlag(WORD player);
};
#pragma pack(pop)
static_assert(offsetof(CPlayerPool, localName) == 2);
static_assert(offsetof(CPlayerPool, local) == 0x1e);
static_assert(offsetof(CPlayerPool, players) == 0x26);
static_assert(offsetof(CPlayerPool, connected) == 0xfd6);
static_assert(offsetof(CPlayerPool, savedCollision) == 0x1f86);
static_assert(sizeof(CPlayerPool) == 0x2f3e);

extern DWORD g_playerPoolProcessCount;

extern const char g_playerPoolRemoteException[];

extern const char g_playerPoolLocalException[];
#endif

#ifndef SAMP_CACHED_ACTIVE_PLAYER_COUNT_H
#define SAMP_CACHED_ACTIVE_PLAYER_COUNT_H

extern DWORD g_cachedActivePlayerCount;

extern DWORD g_activePlayerCountCountdown;
// No this pointer consumed; original class ownership not established.

DWORD GetCachedActivePlayerCount();

#endif // PLAYERPOOL_H
