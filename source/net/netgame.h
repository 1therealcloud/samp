#ifndef NETGAME_H
#define NETGAME_H

#include "game/scripting.h"

#include <stddef.h>
#include <windows.h>

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
    // R5 address to verify
    RakClientInterface* GetRakClient();
    // R5 address to verify
    CTextDrawPool* GetTextDrawPool();
    // R5 address to verify
    CGangZonePool* GetGangZonePool();
    // Raw first settings byte, zero-extended; no settings pointer guard.
    // R5 address to verify
    DWORD GetUseCJWalk();

    // R5 address to verify
    CLabelPool* GetLabelPool();
    // R5 address to verify
    void SetLanMode(DWORD value);

    // 0x00002e20
    CActorPool* GetActorPool();
    // 0x00002e30
    DWORD GetState();
    // Returns the raw DWORD at239, without bool normalization.
    // 0x00002e40
    BOOL LanMode();
    // 0x0000b260
    void UpdateNetwork();
    // 0x0000a100
    void Packet_AuthKey(Packet* packet);
    // 0x0000ad80
    void Packet_ConnectionSucceeded(Packet* packet);
    // 0x0000acf0
    void Packet_ConnectionLost(Packet* packet);
    // 0x0000a740
    void Packet_OnfootSync(Packet* packet);
    // 0x0000aa10
    void Packet_IncarSync(Packet* packet);
    // 0x00009a40
    void Packet_UnoccupiedSync(Packet* packet);
    // 0x00009e20
    void Packet_TrailerSync(Packet* packet);
    // 0x00009b40
    void Packet_BulletSync(Packet* packet);
    // 0x00009c40
    void Packet_AimSync(Packet* packet);
    // 0x00009d30
    void Packet_PassengerSync(Packet* packet);
    // 0x00009f10
    void Packet_MarkersSync(Packet* packet);
    // 0x0000a540
    void ShutdownForRestart();
    // Descriptive native pool/frame pass with three separate SEH regions.
    // 0x00008b20
    void ProcessPools();
    // 0x00008a60
    void ProcessCustomModelWait();
    // 0x00008760
    void InitialNotification();
    // 0x000088f0
    void InitializeGameLogic();
    // 0x00008de0
    void Packet_DisconnectionNotification(Packet* packet);
    // 0x00008e20
    void Packet_InvalidPassword(Packet* packet);
    // 0x0000a2c0
    void ResetMapIcons();
    // 0x0000a300
    void CreateMapIcon(BYTE index, CVector position, BYTE icon, DWORD color, int type);
    // Native excludes the player pool from this reset sequence.
    // 0x0000a4f0
    void ResetPools();
    // 0x00008940
    void Connect();
    // 0x000089b0
    void SpawnScreen();
    // Packet arguments are deliberately ignored by these notification entries.
    // 0x00008d50
    void Packet_RSAPublicKeyMismatch(Packet* packet);
    // 0x00008d70
    void Packet_ConnectionBanned(Packet* packet);
    // 0x00008d90
    void Packet_ConnectionRequestAccepted(Packet* packet);
    // 0x00008db0
    void Packet_NoFreeIncomingConnections(Packet* packet);
    // 0x00008e60
    void Packet_ConnectionAttemptFailed(Packet* packet);
    // 0x00008540
    void InitializePools();
    // 0x00008fe0
    void ResetPlayerPool();
    // 0x00009080
    void ResetVehiclePool();
    // 0x00009110
    void ResetTextDrawPool();
    // 0x000091b0
    void ResetObjectPool();
    // 0x00009250
    void ResetGangZonePool();
    // 0x000092f0
    void ResetPickupPool();
    // 0x00009350
    void ResetMenuPool();
    // 0x000093f0
    void ResetLabelPool();
    // 0x00009490
    void ResetActorPool();
    // 0x00008fb0
    void RemoveMapIcon(BYTE index);
    // 0x00008500
    void SetGameState(DWORD state);
    // 0x000084e0
    CPickupPool* GetPickupPool();
    // Native tail adapter exposes the complete EDX:EAX counter result.
    // 0x000088e0
    __int64 GetCounter();
    // 0x00008f10
    void UpdatePlayersInfo();
    // 0x000084f0
    CMenuPool* GetMenuPool();
    // 0x00002e10
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
    // 0x0000b930
    CNetGame(const char* host, int port, const char* nickname, const char* password);
    // Address-only command dependency.
    // 0x00008e90
    void ToggleNameTagStatus();
    // 0x0000b5b0
    void Process();
    // 0x00009880
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
// 0x00118a18
extern DWORD g_netPlayersInfoTick;
// 0x000e5b28
extern int g_netPlayersInfoRpc;
// 0x000e5958
extern const Scripting::OpcodeInfo g_netRemoveMapIconCommand;
// 0x000e599c
extern const char g_netConnectingMessage[];
// 0x000e59f4
extern const char g_netEncryptionFailedMessage[];
// 0x000e5a18
extern const char g_netConnectionBannedMessage[];
// 0x000e5a3c
extern const char g_netConnectionAcceptedMessage[];
// 0x000e5a60
extern const char g_netServerFullMessage[];
// 0x000e5ab8
extern const char g_netNoResponseMessage[];
// 0x000e596c
extern const char g_netStartedMessage[];
// 0x000e5bb8
extern const char g_netDefaultHostName[];
// 0x000e5afc
extern const char g_netNoNameTagStatusOption[];
// 0x000e5a80
extern const char g_netDisconnectedMessage[];
// 0x000e5b3c
extern const char g_netRestartingMessage[];
// R5 address to verify
extern DWORD g_netVehicleProcessCounter;
// 0x00118a10
extern DWORD g_netPickupProcessCounter;
// R5 address to verify
extern const char g_netVehicleException[];
// 0x000e59bc
extern const char g_netPickupException[];
// 0x000e59b4
extern const char g_netMenuException[];
// 0x000e59c9
extern const char g_netEmptyShoppingSubsection[];
// 0x000e5aa0
extern const char g_netInvalidPasswordMessage[];
// 0x000e5b30
extern const double g_netQuaternionReciprocal;
// 0x000e5b38
extern const float g_netCompressedVectorScale;
// 0x000576a0
int __cdecl NetHexDigit(int value);
// 0x000576b0
void __cdecl NetDigestToHex(char* output, const BYTE* digest);
// 0x000576f0
void __cdecl NetGenerateAuthKey(char* output, const char* challenge);
// 0x0005ffa0
void __cdecl NetGenerateGpci(char* output, const char* seed, DWORD factor);
// 0x000e5b58
extern const char g_netConnectionLostMessage[];
// 0x000e5b98
extern const char g_netJoiningMessage[];
// R5 address to verify
extern const char g_netClientVersion[];
// R5 address to verify
extern int g_netJoinRpc;

// Native entries ignore ECX and pop one pointer argument.
// R5 address to verify
BYTE __stdcall NetGetPacketId(Packet* packet);
// R5 address to verify
BYTE __stdcall NetIsTimestampedPacket(Packet* packet);

#endif // NETGAME_H
