#ifndef NETGAME_H
#define NETGAME_H

#include <stddef.h>
#include <windows.h>

#include "game/scripting.h"
#include <static_assert.h>
// Observed coordinator fields; the unused prefix remains opaque.
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
// Native settings allocation is 0x3a bytes. Names describe established users;
// fields without established semantics retain their native offsets explicitly.
struct CNetGameSettings
{
    BYTE useCJWalk;
    DWORD unknown01;
    float worldBounds[4];
    BYTE unknown15;
    float gravity;
    BYTE disableEnterExits;
    DWORD vehicleDamageFilter;
    BYTE holdWorldTime;
    BYTE instantKill;
    BYTE zoneNames;
    BYTE unknown22;
    DWORD spawnsAvailable;
    float nameTagDrawDistance;
    BYTE manualVehicleControl;
    BYTE worldHour, worldMinute, weather;
    BYTE nameTagLineOfSight;
    DWORD playerMarkersMode;
    DWORD unknown34;
    BYTE showNameTags;
    BYTE unknown39;
};
STATIC_ASSERT(sizeof(CNetGameSettings) == 0x3a);
STATIC_ASSERT(offsetof(CNetGameSettings, worldBounds) == 5);
STATIC_ASSERT(offsetof(CNetGameSettings, gravity) == 0x16);
STATIC_ASSERT(offsetof(CNetGameSettings, vehicleDamageFilter) == 0x1b);
STATIC_ASSERT(offsetof(CNetGameSettings, holdWorldTime) == 0x1f);
STATIC_ASSERT(offsetof(CNetGameSettings, spawnsAvailable) == 0x23);
STATIC_ASSERT(offsetof(CNetGameSettings, nameTagDrawDistance) == 0x27);
STATIC_ASSERT(offsetof(CNetGameSettings, worldHour) == 0x2c);
STATIC_ASSERT(offsetof(CNetGameSettings, playerMarkersMode) == 0x30);
STATIC_ASSERT(offsetof(CNetGameSettings, showNameTags) == 0x38);
STATIC_ASSERT(offsetof(CNetGameSettings, unknown39) == 0x39);
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
    // Observed state values; the raw DWORD also retains unrecognized values.
    enum
    {
        StateWaitingToConnect = 1,
        StateConnecting = 2,
        StateConnected = 5,
        StateAwaitingInit = 6,
        StateRestarting = 11
    };
    // Raw selected client pointer; no initialization or connection-state gate.
    // 0x00001a90
    RakClientInterface* GetRakClient();
    // 0x00017800
    CTextDrawPool* GetTextDrawPool();
    // 0x00017810
    CGangZonePool* GetGangZonePool();
    // Raw first settings byte, zero-extended; no settings pointer guard.
    // 0x000a21f0
    DWORD GetSettingsByte0();

    // 0x0000e3e0
    CLabelPool* GetLabelPool();
    // 0x0000e3f0
    void SetLanMode(DWORD value);

    // 0x00002e50
    CActorPool* GetActorPool();
    // 0x00002e60
    DWORD GetState();
    // Returns the raw DWORD at239, without bool normalization.
    // 0x00002e70
    BOOL LanMode();
    // 0x0000af40
    void UpdateNetwork();
    // 0x00009df0
    void Packet_AuthKey(Packet* packet);
    // 0x0000aa60
    void Packet_ConnectionSucceeded(Packet* packet);
    // 0x0000a9d0
    void Packet_ConnectionLost(Packet* packet);
    // 0x0000a420
    void Packet_OnfootSync(Packet* packet);
    // 0x0000a6f0
    void Packet_IncarSync(Packet* packet);
    // 0x00009730
    void Packet_UnoccupiedSync(Packet* packet);
    // 0x00009b10
    void Packet_TrailerSync(Packet* packet);
    // 0x00009830
    void Packet_BulletSync(Packet* packet);
    // 0x00009930
    void Packet_AimSync(Packet* packet);
    // 0x00009a20
    void Packet_PassengerSync(Packet* packet);
    // 0x00009c00
    void Packet_MarkersSync(Packet* packet);
    // 0x0000a230
    void ShutdownForRestart();
    // Descriptive native pool/frame pass with three separate SEH regions.
    // 0x00008810
    void ProcessPools();
    // 0x00008750
    void ProcessCustomModelWait();
    // 0x00008450
    void InitialNotification();
    // 0x000085e0
    void InitializeGameLogic();
    // 0x00008ad0
    void Packet_DisconnectionNotification(Packet* packet);
    // 0x00008b10
    void Packet_InvalidPassword(Packet* packet);
    // 0x00009fb0
    void ResetMapIcons();
    // 0x00009ff0
    void CreateMapIcon(BYTE index, CVector position, BYTE icon, DWORD color, int type);
    // Native excludes the player pool from this reset sequence.
    // 0x0000a1e0
    void ResetPools();
    // 0x00008630
    void Connect();
    // 0x000086a0
    void SpawnScreen();
    // Packet arguments are deliberately ignored by these notification entries.
    // 0x00008a40
    void Packet_RSAPublicKeyMismatch(Packet* packet);
    // 0x00008a60
    void Packet_ConnectionBanned(Packet* packet);
    // 0x00008a80
    void Packet_ConnectionRequestAccepted(Packet* packet);
    // 0x00008aa0
    void Packet_NoFreeIncomingConnections(Packet* packet);
    // 0x00008b50
    void Packet_ConnectionAttemptFailed(Packet* packet);
    // 0x00008230
    void InitializePools();
    // 0x00008cd0
    void ResetPlayerPool();
    // 0x00008d70
    void ResetVehiclePool();
    // 0x00008e10
    void ResetTextDrawPool();
    // 0x00008eb0
    void ResetObjectPool();
    // 0x00008f50
    void ResetGangZonePool();
    // 0x00008ff0
    void ResetPickupPool();
    // 0x00009050
    void ResetMenuPool();
    // 0x000090e0
    void ResetLabelPool();
    // 0x00009180
    void ResetActorPool();
    // 0x00008ca0
    void RemoveMapIcon(BYTE index);
    // 0x000081f0
    void SetGameState(DWORD state);
    // 0x000081d0
    CPickupPool* GetPickupPool();
    // Native tail adapter exposes the complete EDX:EAX counter result.
    // 0x000085d0
    __int64 GetCounter();
    // 0x00008c00
    void UpdatePlayersInfo();
    // 0x000081e0
    CMenuPool* GetMenuPool();
    // 0x00002e40
    CObjectPool* GetObjectPool();
    // Native coordinator allocation extent. Both strings have an extra byte;
    // construction clears only 256 bytes, while InitGame clears 257 hostname bytes.
    union
    {
        BYTE nativeState[0x3e2];
        struct
        {
            BYTE unknown00[0x2c];
            RakClientInterface* rakClient;
            char hostOrIp[257];
            char hostName[257];
            BYTE disableVehicleCollisions;
            BYTE updateCameraTarget;
            BYTE showNameTagStatus;
            int serverPort;
            DWORD lanMode;
            DWORD mapIcons[100];
            DWORD gameState;
            DWORD lastConnectAttempt;
            CNetGameSettings* settings;
            BYTE modelWaitActive;
            DWORD modelWaitTick;
            CNetGamePools* pools;
        };
    };
    // 0x0000b610
    CNetGame(const char* host, int port, const char* nickname, const char* password);
    // Address-only command dependency.
    // 0x00008b80
    void ToggleNameTagStatus();
    // 0x0000b290
    void Process();
    // Native destructor entry. The full networking lifecycle remains open.
    // 0x00009570
    void Destroy();
    // 0x00001180
    CVehiclePool* GetVehiclePool();
    // 0x00001170
    CPlayerPool* GetPlayerPool();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CNetGamePools) == 0x24);
STATIC_ASSERT(offsetof(CNetGame, pools) == 0x3de);
STATIC_ASSERT(offsetof(CNetGame, rakClient) == 0x2c);
STATIC_ASSERT(offsetof(CNetGame, hostOrIp) == 0x30);
STATIC_ASSERT(offsetof(CNetGame, hostName) == 0x131);
STATIC_ASSERT(offsetof(CNetGame, disableVehicleCollisions) == 0x232);
STATIC_ASSERT(offsetof(CNetGame, updateCameraTarget) == 0x233);
STATIC_ASSERT(offsetof(CNetGame, showNameTagStatus) == 0x234);
STATIC_ASSERT(offsetof(CNetGame, serverPort) == 0x235);
STATIC_ASSERT(offsetof(CNetGame, lanMode) == 0x239);
STATIC_ASSERT(offsetof(CNetGame, mapIcons) == 0x23d);
STATIC_ASSERT(offsetof(CNetGame, gameState) == 0x3cd);
STATIC_ASSERT(offsetof(CNetGame, lastConnectAttempt) == 0x3d1);
STATIC_ASSERT(offsetof(CNetGame, settings) == 0x3d5);
STATIC_ASSERT(offsetof(CNetGame, modelWaitActive) == 0x3d9);
STATIC_ASSERT(offsetof(CNetGame, modelWaitTick) == 0x3da);
STATIC_ASSERT(sizeof(CNetGame) == 0x3e2);
// 0x00156b38
extern DWORD g_netPlayersInfoTick;
// 0x00117b98
extern int g_netPlayersInfoRpc;
// 0x001179c8
extern const Scripting::OpcodeInfo g_netRemoveMapIconCommand;
// 0x00117a0c
extern const char g_netConnectingMessage[];
// 0x00117a64
extern const char g_netEncryptionFailedMessage[];
// 0x00117a88
extern const char g_netConnectionBannedMessage[];
// 0x00117aac
extern const char g_netConnectionAcceptedMessage[];
// 0x00117ad0
extern const char g_netServerFullMessage[];
// 0x00117b28
extern const char g_netNoResponseMessage[];
// 0x001179dc
extern const char g_netStartedMessage[];
// 0x00117c28
extern const char g_netDefaultHostName[];
// 0x00117b6c
extern const char g_netNoNameTagStatusOption[];
// 0x00117af0
extern const char g_netDisconnectedMessage[];
// 0x00117bac
extern const char g_netRestartingMessage[];
// 0x00156b2c
extern DWORD g_netVehicleProcessCounter;
// 0x00156b30
extern DWORD g_netPickupProcessCounter;
// 0x00117a34
extern const char g_netVehicleException[];
// 0x00117a2c
extern const char g_netPickupException[];
// 0x00117a24
extern const char g_netMenuException[];
// 0x00117a39
extern const char g_netEmptyShoppingSubsection[];
// 0x00117b10
extern const char g_netInvalidPasswordMessage[];
// 0x00117ba0
extern const double g_netQuaternionReciprocal;
// 0x00117ba8
extern const float g_netCompressedVectorScale;
// 0x00057160
int __cdecl NetHexDigit(int value);
// 0x00057170
void __cdecl NetDigestToHex(char* output, const BYTE* digest);
// 0x000571b0
void __cdecl NetGenerateAuthKey(char* output, const char* challenge);
// 0x0005fa60
void __cdecl NetGenerateGpci(char* output, const char* seed, DWORD factor);
// 0x00117bc8
extern const char g_netConnectionLostMessage[];
// 0x00117c08
extern const char g_netJoiningMessage[];
// 0x00117bfc
extern const char g_netClientVersion[];
// 0x00117bf8
extern int g_netJoinRpc;

// Descriptive names; original class membership is not established.
// Native entries ignore ECX and pop one pointer argument.
// 0x00008590
BYTE __stdcall NetGetPacketId(Packet* packet);
// 0x000085b0
BYTE __stdcall NetIsTimestampedPacket(Packet* packet);

#endif // NETGAME_H
