#ifndef NETGAME_H
#define NETGAME_H

#include "game/scripting.h"

#include <stddef.h>
#include <windows.h>

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
    DWORD deathDropMoney;
    float worldBounds[4];
    BYTE allowWeapons;
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
    DWORD globalChatRadius;
    BYTE showNameTags;
    BYTE limitGlobalChatRadius;
};
static_assert(sizeof(CNetGameSettings) == 0x3a);
static_assert(offsetof(CNetGameSettings, worldBounds) == 5);
static_assert(offsetof(CNetGameSettings, gravity) == 0x16);
static_assert(offsetof(CNetGameSettings, vehicleDamageFilter) == 0x1b);
static_assert(offsetof(CNetGameSettings, holdWorldTime) == 0x1f);
static_assert(offsetof(CNetGameSettings, spawnsAvailable) == 0x23);
static_assert(offsetof(CNetGameSettings, nameTagDrawDistance) == 0x27);
static_assert(offsetof(CNetGameSettings, worldHour) == 0x2c);
static_assert(offsetof(CNetGameSettings, playerMarkersMode) == 0x30);
static_assert(offsetof(CNetGameSettings, showNameTags) == 0x38);
static_assert(offsetof(CNetGameSettings, limitGlobalChatRadius) == 0x39);
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

    RakClientInterface* GetRakClient();

    CTextDrawPool* GetTextDrawPool();

    CGangZonePool* GetGangZonePool();
    // Raw first settings byte, zero-extended; no settings pointer guard.

    DWORD GetUseCJWalk();

    CLabelPool* GetLabelPool();

    void SetLanMode(DWORD value);

    CActorPool* GetActorPool();

    DWORD GetState();
    // Returns the raw DWORD at239, without bool normalization.

    BOOL LanMode();

    void UpdateNetwork();

    void Packet_AuthKey(Packet* packet);

    void Packet_ConnectionSucceeded(Packet* packet);

    void Packet_ConnectionLost(Packet* packet);

    void Packet_OnfootSync(Packet* packet);

    void Packet_IncarSync(Packet* packet);

    void Packet_UnoccupiedSync(Packet* packet);

    void Packet_TrailerSync(Packet* packet);

    void Packet_BulletSync(Packet* packet);

    void Packet_AimSync(Packet* packet);

    void Packet_PassengerSync(Packet* packet);

    void Packet_MarkersSync(Packet* packet);

    void ShutdownForRestart();
    // Descriptive native pool/frame pass with three separate SEH regions.

    void ProcessPools();

    void ProcessCustomModelWait();

    void InitialNotification();

    void InitializeGameLogic();

    void Packet_DisconnectionNotification(Packet* packet);

    void Packet_InvalidPassword(Packet* packet);

    void ResetMapIcons();

    void CreateMapIcon(BYTE index, CVector position, BYTE icon, DWORD color, int type);
    // Native excludes the player pool from this reset sequence.

    void ResetPools();

    void Connect();

    void SpawnScreen();
    // Packet arguments are deliberately ignored by these notification entries.

    void Packet_RSAPublicKeyMismatch(Packet* packet);

    void Packet_ConnectionBanned(Packet* packet);

    void Packet_ConnectionRequestAccepted(Packet* packet);

    void Packet_NoFreeIncomingConnections(Packet* packet);

    void Packet_ConnectionAttemptFailed(Packet* packet);

    void InitializePools();

    void ResetPlayerPool();

    void ResetVehiclePool();

    void ResetTextDrawPool();

    void ResetObjectPool();

    void ResetGangZonePool();

    void ResetPickupPool();

    void ResetMenuPool();

    void ResetLabelPool();

    void ResetActorPool();

    void RemoveMapIcon(BYTE index);

    void SetGameState(DWORD state);

    CPickupPool* GetPickupPool();
    // Native tail adapter exposes the complete EDX:EAX counter result.

    __int64 GetCounter();

    void UpdatePlayersInfo();

    CMenuPool* GetMenuPool();

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

    CNetGame(const char* host, int port, const char* nickname, const char* password);
    // Address-only command dependency.

    void ToggleNameTagStatus();

    void Process();

    void Destroy();

    CVehiclePool* GetVehiclePool();

    CPlayerPool* GetPlayerPool();
};
#pragma pack(pop)
static_assert(sizeof(CNetGamePools) == 0x24);
static_assert(offsetof(CNetGame, pools) == 0x3de);
static_assert(offsetof(CNetGame, rakClient) == 0x2c);
static_assert(offsetof(CNetGame, hostOrIp) == 0x30);
static_assert(offsetof(CNetGame, hostName) == 0x131);
static_assert(offsetof(CNetGame, disableVehicleCollisions) == 0x232);
static_assert(offsetof(CNetGame, updateCameraTarget) == 0x233);
static_assert(offsetof(CNetGame, showNameTagStatus) == 0x234);
static_assert(offsetof(CNetGame, serverPort) == 0x235);
static_assert(offsetof(CNetGame, lanMode) == 0x239);
static_assert(offsetof(CNetGame, mapIcons) == 0x23d);
static_assert(offsetof(CNetGame, gameState) == 0x3cd);
static_assert(offsetof(CNetGame, lastConnectAttempt) == 0x3d1);
static_assert(offsetof(CNetGame, settings) == 0x3d5);
static_assert(offsetof(CNetGame, modelWaitActive) == 0x3d9);
static_assert(offsetof(CNetGame, modelWaitTick) == 0x3da);
static_assert(sizeof(CNetGame) == 0x3e2);

extern DWORD g_netPlayersInfoTick;

extern int g_netPlayersInfoRpc;

extern const Scripting::OpcodeInfo g_netRemoveMapIconCommand;

extern const char g_netConnectingMessage[];

extern const char g_netEncryptionFailedMessage[];

extern const char g_netConnectionBannedMessage[];

extern const char g_netConnectionAcceptedMessage[];

extern const char g_netServerFullMessage[];

extern const char g_netNoResponseMessage[];

extern const char g_netDefaultHostName[];

extern const char g_netNoNameTagStatusOption[];

extern const char g_netDisconnectedMessage[];

extern const char g_netRestartingMessage[];

extern DWORD g_netVehicleProcessCounter;

extern DWORD g_netPickupProcessCounter;

extern const char g_netVehicleException[];

extern const char g_netPickupException[];

extern const char g_netMenuException[];

extern const char g_netEmptyShoppingSubsection[];

extern const char g_netInvalidPasswordMessage[];

extern const double g_netQuaternionReciprocal;

extern const float g_netCompressedVectorScale;

int __cdecl NetHexDigit(int value);

void __cdecl NetDigestToHex(char* output, const BYTE* digest);

void __cdecl NetGenerateAuthKey(char* output, const char* challenge);

void __cdecl NetGenerateGpci(char (&output)[101], const char* seed, DWORD factor);

extern const char g_netConnectionLostMessage[];

extern const char g_netJoiningMessage[];

extern const char g_netClientVersion[];

extern int g_netJoinRpc;

// Native entries ignore ECX and pop one pointer argument.

BYTE __stdcall NetGetPacketId(Packet* packet);

BYTE __stdcall NetIsTimestampedPacket(Packet* packet);

#endif // NETGAME_H
