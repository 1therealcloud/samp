#ifndef SAMP_NETGAME_H
#define SAMP_NETGAME_H
#include "game/scripting.h"
#include <stddef.h>
#include <windows.h>
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
class RakClientInterface;
struct Packet;
struct CVector;
#pragma pack(push, 1)
struct CNetGamePools
{
    CMenuPool* menus;
    CActorPool* actors;
    CPlayerPool* players;
    CVehiclePool* vehicles;
    CPickupPool* pickups;
    CObjectPool* objects;
    CGangZonePool* gangZones;
    CLabelPool* labels;
    CTextDrawPool* textDraws;
};
class CNetGame
{
public:
    // Raw selected client pointer; no initialization or connection-state gate.
    // samp.dll RVA: 0x00001a90
    RakClientInterface* GetRakClient();
    // samp.dll RVA: 0x00017800
    CTextDrawPool* GetTextDrawPool();
    // samp.dll RVA: 0x00017810
    CGangZonePool* GetGangZonePool();
    // Raw first settings byte, zero-extended; no settings pointer guard.
    // samp.dll RVA: 0x000a21f0
    DWORD GetSettingsByte0();

    // samp.dll RVA: 0x0000e3e0
    CLabelPool* GetLabelPool();
    // samp.dll RVA: 0x0000e3f0
    void SetLanMode(DWORD value);

    // samp.dll RVA: 0x00002e50
    CActorPool* GetActorPool();
    // samp.dll RVA: 0x00002e60
    DWORD GetState();
    // Returns the raw DWORD at239, without bool normalization.
    // samp.dll RVA: 0x00002e70
    BOOL LanMode();
    // samp.dll RVA: 0x0000af40
    void UpdateNetwork();
    // samp.dll RVA: 0x00009df0
    void Packet_AuthKey(Packet* packet);
    // samp.dll RVA: 0x0000aa60
    void Packet_ConnectionSucceeded(Packet* packet);
    // samp.dll RVA: 0x0000a9d0
    void Packet_ConnectionLost(Packet* packet);
    // samp.dll RVA: 0x0000a420
    void Packet_OnfootSync(Packet* packet);
    // samp.dll RVA: 0x0000a6f0
    void Packet_IncarSync(Packet* packet);
    // samp.dll RVA: 0x00009730
    void Packet_UnoccupiedSync(Packet* packet);
    // samp.dll RVA: 0x00009b10
    void Packet_TrailerSync(Packet* packet);
    // samp.dll RVA: 0x00009830
    void Packet_BulletSync(Packet* packet);
    // samp.dll RVA: 0x00009930
    void Packet_AimSync(Packet* packet);
    // samp.dll RVA: 0x00009a20
    void Packet_PassengerSync(Packet* packet);
    // samp.dll RVA: 0x00009c00
    void Packet_MarkersSync(Packet* packet);
    // samp.dll RVA: 0x0000a230
    void ShutdownForRestart();
    // Descriptive native pool/frame pass with three separate SEH regions.
    // samp.dll RVA: 0x00008810
    void ProcessPools();
    // samp.dll RVA: 0x00008750
    void ProcessCustomModelWait();
    // samp.dll RVA: 0x00008450
    void InitialNotification();
    // samp.dll RVA: 0x000085e0
    void InitializeGameLogic();
    // samp.dll RVA: 0x00008ad0
    void Packet_DisconnectionNotification(Packet* packet);
    // samp.dll RVA: 0x00008b10
    void Packet_InvalidPassword(Packet* packet);
    // samp.dll RVA: 0x00009fb0
    void ResetMapIcons();
    // samp.dll RVA: 0x00009ff0
    void CreateMapIcon(BYTE index, CVector position, BYTE icon, DWORD color, int type);
    // Native excludes the player pool from this reset sequence.
    // samp.dll RVA: 0x0000a1e0
    void ResetPools();
    // samp.dll RVA: 0x00008630
    void Connect();
    // samp.dll RVA: 0x000086a0
    void SpawnScreen();
    // Packet arguments are deliberately ignored by these notification entries.
    // samp.dll RVA: 0x00008a40
    void Packet_RSAPublicKeyMismatch(Packet* packet);
    // samp.dll RVA: 0x00008a60
    void Packet_ConnectionBanned(Packet* packet);
    // samp.dll RVA: 0x00008a80
    void Packet_ConnectionRequestAccepted(Packet* packet);
    // samp.dll RVA: 0x00008aa0
    void Packet_NoFreeIncomingConnections(Packet* packet);
    // samp.dll RVA: 0x00008b50
    void Packet_ConnectionAttemptFailed(Packet* packet);
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
    CPickupPool* GetPickupPool();
    // Native tail adapter exposes the complete EDX:EAX counter result.
    // samp.dll RVA: 0x000085d0
    __int64 GetCounter();
    // samp.dll RVA: 0x00008c00
    void UpdatePlayersInfo();
    // samp.dll RVA: 0x000081e0
    CMenuPool* GetMenuPool();
    // samp.dll RVA: 0x00002e40
    CObjectPool* GetObjectPool();
    // Native coordinator allocation extent; internal layout and lifecycle remain open.
    union
    {
        BYTE nativeState[0x3e2];
        struct
        {
            BYTE unknown00[0x3de];
            CNetGamePools* pools;
        };
    };
    // samp.dll RVA: 0x0000b610
    CNetGame(const char* host, int port, const char* nickname, const char* password);
    // Address-only command dependency.
    // samp.dll RVA: 0x00008b80
    void ToggleNameTagStatus();
    // samp.dll RVA: 0x0000b290
    void Process();
    // Native destructor entry. The full networking lifecycle remains open.
    // samp.dll RVA: 0x00009570
    void Destroy();
    // samp.dll RVA: 0x00001180
    CVehiclePool* GetVehiclePool();
    // samp.dll RVA: 0x00001170
    CPlayerPool* GetPlayerPool();
};
#pragma pack(pop)
typedef char NetGamePoolSetExtent[(sizeof(CNetGamePools) == 0x24) ? 1 : -1];
typedef char NetGamePoolsOffset[(offsetof(CNetGame, pools) == 0x3de) ? 1 : -1];
typedef char NetGameExtent[(sizeof(CNetGame) == 0x3e2) ? 1 : -1];
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
// samp.dll RVA: 0x00117c28
extern const char g_netDefaultHostName[];
// samp.dll RVA: 0x00117b6c
extern const char g_netNoNameTagStatusOption[];
// samp.dll RVA: 0x00117af0
extern const char g_netDisconnectedMessage[];
// samp.dll RVA: 0x00117bac
extern const char g_netRestartingMessage[];
// samp.dll RVA: 0x00156b2c
extern DWORD g_netVehicleProcessCounter;
// samp.dll RVA: 0x00156b30
extern DWORD g_netPickupProcessCounter;
// samp.dll RVA: 0x00117a34
extern const char g_netVehicleException[];
// samp.dll RVA: 0x00117a2c
extern const char g_netPickupException[];
// samp.dll RVA: 0x00117a24
extern const char g_netMenuException[];
// samp.dll RVA: 0x00117a39
extern const char g_netEmptyShoppingSubsection[];
// samp.dll RVA: 0x00117b10
extern const char g_netInvalidPasswordMessage[];
// samp.dll RVA: 0x00117ba0
extern const double g_netQuaternionReciprocal;
// samp.dll RVA: 0x00117ba8
extern const float g_netCompressedVectorScale;
// samp.dll RVA: 0x00057160
int __cdecl NetHexDigit(int value);
// samp.dll RVA: 0x00057170
void __cdecl NetDigestToHex(char* output, const BYTE* digest);
// samp.dll RVA: 0x000571b0
void __cdecl NetGenerateAuthKey(char* output, const char* challenge);
// samp.dll RVA: 0x0005fa60
void __cdecl NetGenerateGpci(char* output, const char* seed, DWORD factor);
// samp.dll RVA: 0x00117bc8
extern const char g_netConnectionLostMessage[];
// samp.dll RVA: 0x00117c08
extern const char g_netJoiningMessage[];
// samp.dll RVA: 0x00117bfc
extern const char g_netClientVersion[];
// samp.dll RVA: 0x00117bf8
extern int g_netJoinRpc;
#endif
