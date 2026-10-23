#ifndef SAMP_NETGAME_H
#define SAMP_NETGAME_H
#include <windows.h>
#include <stddef.h>
#include "game/scripting.h"
// The pool set and its owning field are established; other state remains opaque.
class CVehiclePool;
class CPlayerPool;
class CMenuPool;
class CObjectPool;
class CActorPool;
class CPickupPool;
class CGangZonePool;
class CLabelPool;
class CTextDrawPool;
struct Packet;
struct CVector;
#pragma pack(push,1)
struct CNetGamePools {
    CMenuPool *menus;CActorPool *actors;CPlayerPool *players;CVehiclePool *vehicles;
    CPickupPool *pickups;CObjectPool *objects;CGangZonePool *gangZones;CLabelPool *labels;CTextDrawPool *textDraws;
};
class CNetGame {
public:
    // samp.dll RVA: 0x00008450
    void InitialNotification();
    // samp.dll RVA: 0x000085e0
    void InitializeGameLogic();
    // samp.dll RVA: 0x00008ad0
    void Packet_DisconnectionNotification(Packet *packet);
    // samp.dll RVA: 0x00008b10
    void Packet_InvalidPassword(Packet *packet);
    // samp.dll RVA: 0x00009fb0
    void ResetMapIcons();
    // samp.dll RVA: 0x00009ff0
    void CreateMapIcon(BYTE index,CVector position,BYTE icon,DWORD color,int type);
    // Native excludes the player pool from this reset sequence.
    // samp.dll RVA: 0x0000a1e0
    void ResetPools();
    // samp.dll RVA: 0x00008630
    void Connect();
    // samp.dll RVA: 0x000086a0
    void SpawnScreen();
    // Packet arguments are deliberately ignored by these notification entries.
    // samp.dll RVA: 0x00008a40
    void Packet_RSAPublicKeyMismatch(Packet *packet);
    // samp.dll RVA: 0x00008a60
    void Packet_ConnectionBanned(Packet *packet);
    // samp.dll RVA: 0x00008a80
    void Packet_ConnectionRequestAccepted(Packet *packet);
    // samp.dll RVA: 0x00008aa0
    void Packet_NoFreeIncomingConnections(Packet *packet);
    // samp.dll RVA: 0x00008b50
    void Packet_ConnectionAttemptFailed(Packet *packet);
    // samp.dll RVA: 0x00008230
    void InitializePools();
    // samp.dll RVA: 0x00008cd0
    void ResetPlayerPool();
    // samp.dll RVA: 0x00008d70
    void ResetVehiclePool();
    // samp.dll RVA: 0x00008e10
    void ResetTextDrawPool();
    // samp.dll RVA: 0x00008eb0
    void ResetObjectPool();
    // samp.dll RVA: 0x00008f50
    void ResetGangZonePool();
    // samp.dll RVA: 0x00008ff0
    void ResetPickupPool();
    // samp.dll RVA: 0x00009050
    void ResetMenuPool();
    // samp.dll RVA: 0x000090e0
    void ResetLabelPool();
    // samp.dll RVA: 0x00009180
    void ResetActorPool();
    // samp.dll RVA: 0x00008ca0
    void RemoveMapIcon(BYTE index);
    // samp.dll RVA: 0x000081f0
    void SetGameState(DWORD state);
    // samp.dll RVA: 0x000081d0
    CPickupPool *GetPickupPool();
    // Native tail adapter exposes the complete EDX:EAX counter result.
    // samp.dll RVA: 0x000085d0
    __int64 GetCounter();
    // samp.dll RVA: 0x00008c00
    void UpdatePlayersInfo();
    // samp.dll RVA: 0x000081e0
    CMenuPool *GetMenuPool();
    // samp.dll RVA: 0x00002e40
    CObjectPool *GetObjectPool();
    // Native coordinator allocation extent; internal layout and lifecycle remain open.
    union {
        BYTE nativeState[0x3e2];
        struct {BYTE unknown00[0x3de];CNetGamePools *pools;};
    };
    // samp.dll RVA: 0x0000b610
    CNetGame(const char *host,int port,const char *nickname,const char *password);
    // Address-only command dependency.
    // samp.dll RVA: 0x00008b80
    void ToggleNameTagStatus();
    // samp.dll RVA: 0x0000b290
    void Process();
    // Native destructor entry. The full networking lifecycle remains open.
    // samp.dll RVA: 0x00009570
    void Destroy();
    // samp.dll RVA: 0x00001180
    CVehiclePool *GetVehiclePool();
    // samp.dll RVA: 0x00001170
    CPlayerPool *GetPlayerPool();
};
#pragma pack(pop)
typedef char NetGamePoolSetExtent[(sizeof(CNetGamePools)==0x24)?1:-1];
typedef char NetGamePoolsOffset[(offsetof(CNetGame,pools)==0x3de)?1:-1];
typedef char NetGameExtent[(sizeof(CNetGame)==0x3e2)?1:-1];
// samp.dll RVA: 0x00156b38
extern DWORD g_netPlayersInfoTick;
// samp.dll RVA: 0x00117b98
extern int g_netPlayersInfoRpc;
// samp.dll RVA: 0x001179c8
extern const Scripting::OpcodeInfo g_netRemoveMapIconCommand;
// samp.dll RVA: 0x00117a0c
extern const char g_netConnectingMessage[];
// samp.dll RVA: 0x00117a64
extern const char g_netEncryptionFailedMessage[];
// samp.dll RVA: 0x00117a88
extern const char g_netConnectionBannedMessage[];
// samp.dll RVA: 0x00117aac
extern const char g_netConnectionAcceptedMessage[];
// samp.dll RVA: 0x00117ad0
extern const char g_netServerFullMessage[];
// samp.dll RVA: 0x00117b28
extern const char g_netNoResponseMessage[];
// samp.dll RVA: 0x001179dc
extern const char g_netStartedMessage[];
// samp.dll RVA: 0x00117af0
extern const char g_netDisconnectedMessage[];
// samp.dll RVA: 0x00117b10
extern const char g_netInvalidPasswordMessage[];
#endif
