#include "net/netgame.h"

#include "archive/ArchiveFS.h"
#include "config.h"
#include "filechecks.h"
#include "game/aimstuff.h"
#include "game/camera.h"
#include "game/custommodels.h"
#include "game/patches.h"
#include "game/playerped.h"
#include "game/quaternion.h"
#include "game/util.h"
#include "gui/chatwindow.h"
#include "gui/gui.h"
#include "main.h"
#include "net/actorpool.h"
#include "net/gangzonepool.h"
#include "net/labelpool.h"
#include "net/localplayer.h"
#include "net/menupool.h"
#include "net/netrpc.h"
#include "net/objectpool.h"
#include "net/pickuppool.h"
#include "net/playerpool.h"
#include "net/remoteplayer.h"
#include "net/scriptrpc.h"
#include "net/textdrawpool.h"
#include "net/vehiclepool.h"

#include <BitStream.h>
#include <GetTime.h>
#include <NetworkTypes.h>
#include <RakClientInterface.h>
#include <RakNetworkFactory.h>
#include <SocketLayer.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

const char g_netDefaultHostName[] = "SA-MP";

const char g_netNoNameTagStatusOption[] = "nonametagstatus";

const char g_netConnectionLostMessage[] = "Lost connection to the server. Reconnecting..";

const char g_netJoiningMessage[] = "Connected. Joining the game...";

int g_netJoinRpc = 25;

const double g_netQuaternionReciprocal = 1.0 / 65535.0;

const float g_netCompressedVectorScale = 1.0f / 32767.5f;

const char g_netRestartingMessage[] = "The server is restarting..";

DWORD g_netVehicleProcessCounter;

DWORD g_netPickupProcessCounter;

const char g_netVehicleException[] = "s004";

const char g_netPickupException[] = "s005";

const char g_netMenuException[] = "s006";

const char g_netEmptyShoppingSubsection[] = "";

const std::string g_netStartedMessage = "{FFFFFF}SA-MP {B9C9BF}" + g_ClientVersionString + " {FFFFFF}Started";

const char g_netDisconnectedMessage[] = "Server closed the connection.";

const char g_netInvalidPasswordMessage[] = "Wrong server password.";

const char g_netConnectingMessage[] = "Connecting to %s:%d...";

const char g_netEncryptionFailedMessage[] = "Failed to initialize encryption.";

const char g_netConnectionBannedMessage[] = "You are banned from this server.";

const char g_netConnectionAcceptedMessage[] = "Server has accepted the connection.";

const char g_netServerFullMessage[] = "The server is full. Retrying...";

const char g_netNoResponseMessage[] = "The server didn't respond. Retrying..";

const Scripting::OpcodeInfo g_netRemoveMapIconCommand = {0x164, "i"};

DWORD g_netPlayersInfoTick = 0;

int g_netPlayersInfoRpc = 155;

RakClientInterface* CNetGame::GetRakClient()
{
    return rakClient;
}

CTextDrawPool* CNetGame::GetTextDrawPool()
{
    return pools->textDraws;
}

CGangZonePool* CNetGame::GetGangZonePool()
{
    return pools->gangZones;
}

CActorPool* CNetGame::GetActorPool()
{
    return pools->actors;
}

DWORD CNetGame::GetState()
{
    return gameState;
}

BOOL CNetGame::LanMode()
{
    return (BOOL)lanMode;
}

CNetGame::CNetGame(const char* host, int port, const char* nickname, const char* password)
{
    settings = (CNetGameSettings*)calloc(1, sizeof(CNetGameSettings));
    memset(hostName, 0, 256);
    memset(hostOrIp, 0, 256);
    memcpy(hostName, g_netDefaultHostName, 6);
    strncpy_s(hostOrIp, host, _TRUNCATE);
    serverPort = port;
    rakClient = RakNetworkFactory::GetRakClientInterface();
    InitializePools();
    pools->players->localName.assign(nickname, strlen(nickname));
    // Native helper RVA 0x0001f1d0 sets the datagram port key to port XOR 0xCCCC.
    SocketLayer::Instance()->SetDatagramPort((unsigned short)port);
    RegisterRPCs(rakClient);
    RegisterScriptRPCs(rakClient);
    rakClient->SetPassword(password);
    memset(mapIcons, 0, 100 * 4);
    g_pGame->EnableClock(false);
    g_pGame->EnableZoneDisplaying(0);
    settings->deathDropMoney = 0;
    settings->spawnsAvailable = 0;
    settings->nameTagLineOfSight = 1;
    settings->holdWorldTime = 1;
    settings->useCJWalk = 0;
    settings->disableEnterExits = 0;
    settings->zoneNames = 0;
    settings->instantKill = 0;
    *(DWORD*)&settings->nameTagDrawDistance = 0x42700000;
    settings->unknown22 = 1;
    settings->worldHour = 12;
    settings->worldMinute = 0;
    settings->weather = 1;
    *(DWORD*)&settings->gravity = 0x3c03126f;
    showNameTagStatus = 1;
    if (g_pConfig && g_pConfig->GetIntValue(g_netNoNameTagStatusOption))
        showNameTagStatus = 0;
    disableVehicleCollisions = 0;
    updateCameraTarget = 0;
    lastConnectAttempt = GetTickCount();
    gameState = StateWaitingToConnect;
    lanMode = 0;
    modelWaitActive = 0;
    if (g_pChat)
        g_pChat->AddDebugMessage(g_netStartedMessage.c_str());
}

void CNetGame::Process()
{
    UpdateNetwork();
    CNetGameSettings* settings = this->settings;
    if (settings->holdWorldTime)
        g_pGame->SetWorldTime(settings->worldHour, settings->worldMinute);
    g_pGame->LoadAnimationsAndModels();
    if (gameState == StateConnected)
    {
        if (g_customModels && !((BYTE*)g_customModels)[0x221])
        {
            g_customModels->Process();
            ProcessCustomModelWait();
        }
        else
        {
            if (g_renderDownloadWindow && *(DWORD*)(g_renderDownloadWindow->nativeState + 0x30))
                g_renderDownloadWindow->Hide();
            ProcessPools();
        }
    }
    else
        SpawnScreen();
    if (gameState == StateWaitingToConnect)
        Connect();
}

int __cdecl NetHexDigit(int value)
{
    value += 0x30;
    if (value > 0x39)
        value += 7;
    return value;
}

void __cdecl NetDigestToHex(char* output, const BYTE* digest)
{
    for (int offset = 0; offset < 40; offset += 2, ++digest)
    {
        output[offset] = (char)NetHexDigit(*digest >> 4);
        output[offset + 1] = (char)NetHexDigit(*digest & 15);
    }
    output[40] = 0;
}

void __cdecl NetGenerateGpci(char (&output)[101], const char* seed, DWORD factor)
{
    DWORD digest[5] = {0};
    ArchiveComputeNameDigest(seed, (DWORD)strlen(seed), digest);
    BYTE transformed[20];
    memcpy(transformed, digest, 20);
    for (int i = 0; i < 20; ++i)
    {
        BYTE value = transformed[i], a = value & 3, b = (value >> 2) & 3, c = (value >> 4) & 3, d = value >> 6;
        BYTE low = a > b ? (BYTE)((a << 2) | b) : (BYTE)((b << 2) | a);
        BYTE high = c > d ? (BYTE)((c << 2) | d) : (BYTE)((d << 2) | c);
        transformed[i] = (BYTE)((high << 4) | low);
    }
    if (!g_pNetGame)
        --factor;

    BYTE product[24] = {0};
    unsigned __int64 carry = 0;
    for (int i = 19; i >= 0; --i)
    {
        unsigned __int64 current = (unsigned __int64)transformed[i] * factor + carry;
        product[i + 4] = (BYTE)(current & 0xFF);
        carry = current >> 8;
    }
    for (int i = 3; i >= 0; --i)
    {
        product[i] = (BYTE)(carry & 0xFF);
        carry >>= 8;
    }

    static const char hexDigits[] = "0123456789ABCDEF";
    char* out = output;
    bool started = false;
    for (int i = 0; i < 24; ++i)
    {
        BYTE high = (product[i] >> 4) & 0x0F;
        BYTE low = product[i] & 0x0F;
        if (started || high != 0)
        {
            *out++ = hexDigits[high];
            started = true;
        }
        if (started || low != 0)
        {
            *out++ = hexDigits[low];
            started = true;
        }
    }
    if (!started)
        *out++ = '0';
    *out = '\0';
}

void __cdecl NetGenerateAuthKey(char* output, const char* challenge)
{
    if (!g_pGame || !GameFindLocalPed())
        return;
    CVector position = {1093.0f, -2036.0f, 90.0f}, rotation = {0, 0, 0}, target = {384.0f, -1557.0f, 20.0f};
    g_pGame->GetCamera()->Set(position, rotation);
    g_pGame->GetCamera()->PointAt(target, 2);
    DWORD digest[5] = {0};
    ArchiveComputeNameDigest(challenge, (DWORD)strlen(challenge), digest);
    BYTE result[20];
    memcpy(result, digest, 20);
    g_pGame->GetPlayerPed()->GetDistanceToCamera();
    DWORD index = g_pArchive->GetFileIndex((DWORD)0xbaddea6b);
    if (index != 0xffffffff)
    {
        BYTE* data = g_pArchive->GetFileData(index);
        for (int group = 0; group < 4; ++group)
            for (int i = group * 5; i < (group + 1) * 5; ++i)
            {
                BYTE value = result[i];
                BYTE key = group == 0 ? 0x2f : group == 1 ? 0x45 : group == 2 ? 0x6f : 0xdb;
                for (int j = 0; j < 100; ++j)
                    value = (BYTE)(data[j] ^ value ^ key);
                result[i] = value;
            }
        // GTA SA virtual address: 0x004d3aa0; native changes protection and leaves it changed.
        UnFuck((void*)0x4d3aa0, 20);
        BYTE code[20];
        memcpy(code, (const void*)0x4d3aa0, 20);
        for (int i = 0; i < 20; ++i)
            result[i] ^= code[i];
    }
    NetDigestToHex(output, result);
}

void CNetGame::Packet_AuthKey(Packet* packet)
{
    RakNet::BitStream input(packet->data, (int)packet->length, false);
    BYTE packetId = 0;
    int state = (int)gameState;
    if (state < 1 || state > 6)
        return;
    BYTE length = 0;
    char challenge[260], response[256];
    input.ReadBits(&packetId, 8, true);
    input.ReadBits(&length, 8, true);
    input.Read(challenge, length);
    challenge[length] = 0;
    NetGenerateAuthKey(response, challenge);
    BYTE responseLength = (BYTE)strlen(response);
    RakNet::BitStream output;
    packetId = 12;
    output.WriteBits(&packetId, 8, true);
    output.WriteBits(&responseLength, 8, true);
    output.Write(response, responseLength);
    rakClient->Send(&output, (PacketPriority)0, (PacketReliability)8, 0);
}

void CNetGame::Packet_ConnectionSucceeded(Packet* packet)
{
    RakNet::BitStream input(packet->data, (int)packet->length, true);
    char serial[101] = {0};
    BYTE packetId = 0;
    DWORD address = 0, port = 0, player = 0, challenge = 0;
    input.ReadBits(&packetId, 8, true);
    input.ReadBits((BYTE*)&address, 32, true);
    input.ReadBits((BYTE*)&port, 16, true);
    input.ReadBits((BYTE*)&player, 16, true);
    input.ReadBits((BYTE*)&challenge, 32, true);
    challenge ^= g_DLClient ? 4062 : 4057; // 0xfde : 0xfd9
    if (g_pChat)
        g_pChat->AddDebugMessage(g_netJoiningMessage);
    gameState = StateAwaitingInit;
    size_t length = strlen(pools->players->localName.c_str());
    RakNet::BitStream output;
    DWORD version = g_DLClient ? 4062 : 4057; // 0xfde : 0xfd9
    BYTE byte = 1;
    output.WriteBits((BYTE*)&version, 32, true);
    output.WriteBits(&byte, 8, true);
    byte = (BYTE)length;
    output.WriteBits(&byte, 8, true);
    output.Write(pools->players->localName.c_str(), byte);
    output.WriteBits((BYTE*)&challenge, 32, true);
    // GTA SA virtual address: 0x00c9236c, original serial seed string.
    NetGenerateGpci(serial, (const char*)0xc9236c, 1001);
    byte = (BYTE)strlen(serial);
    output.WriteBits(&byte, 8, true);
    output.Write(serial, byte);
    byte = g_DLClient ? 9 : 8;
    output.WriteBits(&byte, 8, true);
    output.Write(g_ClientVersionString.c_str(), g_DLClient ? 9 : 8);
    output.WriteBits((BYTE*)&challenge, 32, true);
    rakClient->RPC(&g_netJoinRpc, &output, (PacketPriority)1, (PacketReliability)8, 0, false);
}

void CNetGame::Packet_ConnectionLost(Packet* packet)
{
    (void)packet;
    RakClientInterface* client = rakClient;
    if (client)
        client->Disconnect(0, 0);
    g_pChat->AddDebugMessage(g_netConnectionLostMessage);
    ShutdownForRestart();
    if (pools->players)
        for (DWORD i = 0; i != 1004; ++i)
        {
            CPlayerPool* players = pools->players;
            if (players->connected[(WORD)i])
                players->Delete((WORD)i, 0);
        }
    if (g_renderAudioStream)
        g_renderAudioStream->Stop(true);
    gameState = StateWaitingToConnect;
}

// Descriptive free-function names; no native receiver use was observed.

BYTE __stdcall NetGetPacketId(Packet* packet)
{
    if (!packet)
        return 0xff;
    BYTE id = packet->data[0];
    if (id == 40)
        return packet->data[5];
    return id;
}

BYTE __stdcall NetIsTimestampedPacket(Packet* packet)
{
    if (!packet)
        return 0;
    return packet->data[0] == 40;
}

void CNetGame::UpdateNetwork()
{
    Packet* packet = rakClient->Receive();
    while (packet)
    {
        BYTE id = packet->data[0];
        if (id == 40)
            id = packet->data[5];
        switch (id)
        {
            case 12:
                Packet_AuthKey(packet);
                break;
            case 29:
                g_pChat->AddDebugMessage(g_netNoResponseMessage);
                gameState = StateWaitingToConnect;
                break;
            case 31:
                g_pChat->AddDebugMessage(g_netServerFullMessage);
                gameState = StateWaitingToConnect;
                break;
            case 32:
                Packet_DisconnectionNotification(packet);
                break;
            case 33:
                Packet_ConnectionLost(packet);
                break;
            case 34:
                Packet_ConnectionSucceeded(packet);
                break;
            case 35:
                g_pChat->AddDebugMessage(g_netEncryptionFailedMessage);
                break;
            case 36:
                g_pChat->AddDebugMessage(g_netConnectionBannedMessage);
                break;
            case 37:
                g_pChat->AddDebugMessage(g_netInvalidPasswordMessage);
                rakClient->Disconnect(0, 0);
                break;
            case 200:
                Packet_IncarSync(packet);
                break;
            case 203:
                Packet_AimSync(packet);
                break;
            case 206:
                Packet_BulletSync(packet);
                break;
            case 207:
                Packet_OnfootSync(packet);
                break;
            case 208:
                Packet_MarkersSync(packet);
                break;
            case 209:
                Packet_UnoccupiedSync(packet);
                break;
            case 210:
                Packet_TrailerSync(packet);
                break;
            case 211:
                Packet_PassengerSync(packet);
                break;
        }
        rakClient->DeallocatePacket(packet);
        packet = rakClient->Receive();
    }
}
// Source-only wrapper for the two native BitStream<float> instantiations.
// No fields or virtual interface are added: native ECX remains the base stream.
class NativeSyncBitStream : public RakNet::BitStream
{
public:
    NativeSyncBitStream(unsigned char* data, int length) : RakNet::BitStream(data, length, false) {}

    bool ReadQuaternion(float& w, float& x, float& y, float& z);

    bool ReadVelocity(float& x, float& y, float& z);
};

bool NativeSyncBitStream::ReadQuaternion(float& w, float& x, float& y, float& z)
{
    bool wNegative, xNegative, yNegative, zNegative;
    unsigned short cx, cy, cz;
    Read(wNegative);
    Read(xNegative);
    Read(yNegative);
    Read(zNegative);
    Read(cx);
    Read(cy);
    if (!Read(cz))
        return false;
    x = (float)(cx * g_netQuaternionReciprocal);
    y = (float)(cy * g_netQuaternionReciprocal);
    z = (float)(cz * g_netQuaternionReciprocal);
    if (xNegative)
        x = -x;
    if (yNegative)
        y = -y;
    if (zNegative)
        z = -z;
    double residual = ((double)g_quaternionMatrixOne - (double)x * x) - (double)y * y - (double)z * z;
    if (residual < g_objectScaleLowerBound)
        residual = g_objectScaleLowerBound;
    const double root = sqrt(residual);
    volatile float& outputW = w;
    outputW = (float)root;
    if (wNegative)
        outputW = (float)-root;
    return true;
}

bool NativeSyncBitStream::ReadVelocity(float& x, float& y, float& z)
{
    float magnitude;
    if (!Read(magnitude))
        return false;
    if (!(magnitude > g_localQuaternionSyncThreshold))
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        return true;
    }
    unsigned short compressed;
    float normalizedX, normalizedY;
    if (Read(compressed))
        normalizedX = (float)compressed * g_netCompressedVectorScale - g_quaternionMatrixOne;
    if (Read(compressed))
        normalizedY = (float)compressed * g_netCompressedVectorScale - g_quaternionMatrixOne;
    if (!Read(compressed))
        return false;
    const double normalizedZ = (double)compressed * g_netCompressedVectorScale - g_quaternionMatrixOne;
    x = normalizedX;
    y = normalizedY;
    z = (float)normalizedZ;
    x = magnitude * x;
    y = magnitude * y;
    z = magnitude * z;
    return true;
}

void CNetGame::Packet_OnfootSync(Packet* packet)
{
    NativeSyncBitStream stream(packet->data, (int)packet->length);
    BYTE packetId = 0;
    bool surfing = false, animation = false;
    if (gameState != StateConnected)
        return;
    LocalOnfootSyncData data = {0};
    DWORD timestamp = 0;
    WORD player;
    bool present;
    BYTE health, weapon = 0;
    if (packet->data[0] == 40)
    {
        stream.ReadBits(&packetId, 8, true);
        stream.ReadBits((BYTE*)&timestamp, 32, true);
    }
    stream.ReadBits(&packetId, 8, true);
    stream.ReadBits((BYTE*)&player, 16, true);
    stream.Read(present);
    if (present)
        stream.ReadBits((BYTE*)&data.leftRight, 16, true);
    stream.Read(present);
    if (present)
        stream.ReadBits((BYTE*)&data.upDown, 16, true);
    stream.ReadBits((BYTE*)&data.keys, 16, true);
    stream.Read((char*)&data.position, 12);
    stream.ReadQuaternion(data.rotation.w, data.rotation.x, data.rotation.y, data.rotation.z);
    stream.ReadBits(&health, 8, true);
    BYTE armour = health & 15, pedHealth = health >> 4;
    data.armour = armour == 15 ? 100 : armour * 7;
    data.health = pedHealth == 15 ? 100 : pedHealth * 7;
    stream.ReadBits(&weapon, 8, true);
    data.weaponAim = (data.weaponAim & 0xc0) | (weapon & 0x3f);
    stream.ReadBits((BYTE*)&data.specialAction, 8, true);
    stream.ReadVelocity(data.speed.x, data.speed.y, data.speed.z);
    stream.Read(surfing);
    if (surfing)
    {
        stream.ReadBits((BYTE*)&data.surfingVehicle, 16, true);
        stream.Read((char*)&data.surfingOffset, 12);
    }
    else
        data.surfingVehicle = 0xffff;
    stream.Read(animation);
    if (animation)
        stream.ReadBits((BYTE*)&data.animation, 32, true);
    else
        data.animation = 0;
    CRemotePlayer* remote = pools->players->GetPlayer(player);
    if (remote)
        remote->UpdateOnfoot(&data, timestamp);
}

void CNetGame::Packet_IncarSync(Packet* packet)
{
    NativeSyncBitStream stream(packet->data, (int)packet->length);
    BYTE packetId = 0;
    if (gameState != StateConnected)
        return;
    LocalIncarSyncData data = {0};
    DWORD timestamp = 0;
    WORD player, vehicleHealth;
    BYTE health, weapon = 0;
    bool present;
    if (packet->data[0] == 40)
    {
        stream.ReadBits(&packetId, 8, true);
        stream.ReadBits((BYTE*)&timestamp, 32, true);
    }
    stream.ReadBits(&packetId, 8, true);
    stream.ReadBits((BYTE*)&player, 16, true);
    stream.ReadBits((BYTE*)&data.vehicle, 16, true);
    stream.ReadBits((BYTE*)&data.leftRight, 16, true);
    stream.ReadBits((BYTE*)&data.upDown, 16, true);
    stream.ReadBits((BYTE*)&data.keys, 16, true);
    stream.ReadQuaternion(data.rotation.w, data.rotation.x, data.rotation.y, data.rotation.z);
    stream.Read((char*)&data.position, 12);
    stream.ReadVelocity(data.speed.x, data.speed.y, data.speed.z);
    stream.ReadBits((BYTE*)&vehicleHealth, 16, true);
    data.health = (float)vehicleHealth;
    stream.ReadBits(&health, 8, true);
    BYTE armour = health & 15, pedHealth = health >> 4;
    data.armour = armour == 15 ? 100 : armour * 7;
    data.pedHealth = pedHealth == 15 ? 100 : pedHealth * 7;
    stream.ReadBits(&weapon, 8, true);
    data.weaponAim = (data.weaponAim & 0xc0) | (weapon & 0x3f);
    stream.Read(present);
    if (present)
        data.siren = 1;
    stream.Read(present);
    if (present)
        data.landingGear = 1;
    stream.Read(present);
    if (present)
        stream.ReadBits((BYTE*)&data.modelData, 32, true);
    stream.Read(present);
    if (present)
        stream.ReadBits((BYTE*)&data.trailer, 16, true);
    CRemotePlayer* remote = pools->players->GetPlayer(player);
    if (remote)
        remote->UpdateIncar(&data, timestamp);
}
static_assert(offsetof(Packet, length) == 8);
static_assert(offsetof(Packet, data) == 16);

void CNetGame::Packet_UnoccupiedSync(Packet* packet)
{
    RakNet::BitStream stream(packet->data, (int)packet->length, false);
    BYTE packetId = 0;
    if (gameState != StateConnected)
        return;
    WORD player;
    UnoccupiedSyncData data = {0};
    stream.ReadBits(&packetId, 8, true);
    stream.ReadBits((BYTE*)&player, 16, true);
    stream.Read((char*)&data, 67);
    CRemotePlayer* remote = pools->players->GetPlayer(player);
    if (remote)
        remote->UpdateUnoccupied(&data);
}

void CNetGame::Packet_TrailerSync(Packet* packet)
{
    RakNet::BitStream stream(packet->data, (int)packet->length, false);
    if (gameState != StateConnected)
        return;
    BYTE packetId = 0;
    LocalTrailerSyncData data;
    WORD player;
    stream.ReadBits(&packetId, 8, true);
    stream.ReadBits((BYTE*)&player, 16, true);
    stream.Read((char*)&data, 54);
    CRemotePlayer* remote = pools->players->GetPlayer(player);
    if (remote)
        remote->UpdateTrailer(&data);
}

void CNetGame::Packet_BulletSync(Packet* packet)
{
    RakNet::BitStream stream(packet->data, (int)packet->length, false);
    if (gameState != StateConnected)
        return;
    BYTE packetId = 0;
    BulletSyncData data;
    DWORD player = 0;
    stream.ReadBits(&packetId, 8, true);
    stream.ReadBits((BYTE*)&player, 16, true);
    stream.Read((char*)&data, 40);
    CPlayerPool* players = pools->players;
    if (player < 1004 && players->connected[player])
    {
        CRemotePlayer* remote = players->GetPlayer((WORD)player);
        if (remote)
            remote->UpdateBullet(&data);
    }
}

void CNetGame::Packet_AimSync(Packet* packet)
{
    RakNet::BitStream stream(packet->data, (int)packet->length, false);
    BYTE packetId = 0;
    if (gameState != StateConnected)
        return;
    WORD player;
    LocalAimSyncData data;
    stream.ReadBits(&packetId, 8, true);
    stream.ReadBits((BYTE*)&player, 16, true);
    stream.Read((char*)&data, 31);
    CRemotePlayer* remote = pools->players->GetPlayer(player);
    if (remote)
        remote->UpdateAim(&data);
}

void CNetGame::Packet_PassengerSync(Packet* packet)
{
    RakNet::BitStream stream(packet->data, (int)packet->length, false);
    BYTE packetId = 0;
    if (gameState != StateConnected)
        return;
    WORD player;
    LocalPassengerSyncData data;
    stream.ReadBits(&packetId, 8, true);
    stream.ReadBits((BYTE*)&player, 16, true);
    stream.Read((char*)&data, 24);
    CRemotePlayer* remote = pools->players->GetPlayer(player);
    if (remote)
        remote->UpdatePassenger(&data);
}

void CNetGame::Packet_MarkersSync(Packet* packet)
{
    RakNet::BitStream stream(packet->data, (int)packet->length, false);
    if (gameState != StateConnected)
        return;
    CPlayerPool* players = pools->players;
    BYTE packetId = 0;
    DWORD count = 0, index = 0;
    stream.ReadBits(&packetId, 8, true);
    stream.ReadBits((BYTE*)&count, 32, true);
    BYTE active = packetId;
    WORD player;
    short x, y, z;
    if (count)
        do
        {
            stream.ReadBits((BYTE*)&player, 16, true);
            stream.Read(*(bool*)&active);
            if (active)
            {
                stream.ReadBits((BYTE*)&x, 16, true);
                stream.ReadBits((BYTE*)&y, 16, true);
                stream.ReadBits((BYTE*)&z, 16, true);
            }
            if (player < 1004 && players->connected[player])
            {
                CRemotePlayer* remote = players->GetPlayer(player);
                if (active)
                    remote->SetMarkerPosition(x, y, z);
                else
                    remote->SetMarkerState(FALSE);
            }
        }
        while (++index != count);
}

void CNetGame::ShutdownForRestart()
{
    for (WORD id = 0; id < 1004; ++id)
    {
        CRemotePlayer* remote = pools->players->GetPlayer(id);
        if (remote)
        {
            remote->m_state.team = 0xff;
            remote->ResetData();
        }
    }
    pools->players->local->ResetData();
    pools->players->local->ToggleSpectating(FALSE);
    AimStuff::ResetLocalSkills();
    gameState = StateRestarting;
    pools->players->Deactivate();
    pools->players->Process();
    ResetVehiclePool();
    ResetActorPool();
    ResetTextDrawPool();
    ResetGangZonePool();
    ResetLabelPool();
    ResetMapIcons();
    ResetPickupPool();
    ResetObjectPool();
    ResetMenuPool();
    settings->disableEnterExits = 0;
    *(DWORD*)&settings->nameTagDrawDistance = 0x42700000;
    settings->worldHour = 12;
    settings->worldMinute = 0;
    settings->weather = 1;
    settings->holdWorldTime = 1;
    settings->nameTagLineOfSight = 1;
    settings->useCJWalk = 0;
    *(DWORD*)&settings->gravity = 0x3c03126f;
    settings->deathDropMoney = 0;
    *(DWORD*)((BYTE*)g_pGame + 0x4d) = 0;
    *(DWORD*)((BYTE*)g_pGame + 0x29) = 0;
    g_pGame->GetPlayerPed()->LinkToInterior(0, TRUE);
    g_pGame->ResetMoney();
    g_pGame->GetPlayerPed()->Kill();
    g_pGame->GetPlayerPed()->SetArmour(0.0f);
    g_pGame->EnableZoneDisplaying(0);
    settings->zoneNames = 0;
    PlayerColors::Initialize();
    g_pGame->SetWorldGravity(settings->gravity);
    g_pGame->SetWantedLevel(0);
    g_pGame->EnableClock(false);
    g_pChat->AddInfoMessage(g_netRestartingMessage);
}

void CNetGame::ProcessPools()
{
    if (modelWaitActive)
    {
        if ((DWORD)RakNet::GetTime() - modelWaitTick <= 1000)
            return;
        modelWaitActive = 0;
        if (pools->players->local->m_state.active)
        {
            g_pGame->GetPlayerPed()->SetControllable(TRUE);
            g_pGame->GetPlayerPed()->SetCollisionFlag(TRUE);
            g_pGame->GetPlayerPed()->SetCollisionProcessed(TRUE);
        }
    }
    if (settings->disableEnterExits)
    {
        CPed* ped = g_pGame->GetPlayerPed();
        if (ped && !ped->GetBackendShoppingName() && strlen(ped->GetLoadedShoppingDataSubsection()))
            ped->LoadShoppingDataSubsection(g_netEmptyShoppingSubsection);
    }
    CPlayerPool* players = pools->players;
    if (players)
        players->Process();
    CVehiclePool* vehicles = pools->vehicles;
    if (vehicles && (LONG)g_netVehicleProcessCounter > 2)
    {
        __try
        {
            vehicles->Process();
        }
        __except (GameExceptionFilter(GetExceptionCode(), GetExceptionInformation(), g_netVehicleException))
        {
            // Native continues to the counter reset after a handled error.
        }
        g_netVehicleProcessCounter = 0;
    }
    else
        ++g_netVehicleProcessCounter;
    CPickupPool* pickups = pools->pickups;
    if (pickups && (LONG)g_netPickupProcessCounter > 5)
    {
        __try
        {
            pickups->Process();
        }
        __except (GameExceptionFilter(GetExceptionCode(), GetExceptionInformation(), g_netPickupException))
        {
            // Native continues to the counter reset after a handled error.
        }
        g_netPickupProcessCounter = 0;
    }
    else
        ++g_netPickupProcessCounter;
    CMenuPool* menus = pools->menus;
    if (menus)
    {
        __try
        {
            menus->Process();
        }
        __except (GameExceptionFilter(GetExceptionCode(), GetExceptionInformation(), g_netMenuException))
        {
            // Native menu handler returns without additional cleanup.
        }
    }
}

void CNetGame::ProcessCustomModelWait()
{
    if (g_customModels && !modelWaitActive && pools->players->local->m_state.active && *(DWORD*)((BYTE*)g_customModels + 0x217) && g_customModels->MarkResourceRequests() > 0)
    {
        modelWaitActive = 1;
        g_pGame->GetPlayerPed()->SetControllable(FALSE);
        g_pGame->GetPlayerPed()->SetCollisionFlag(FALSE);
        g_pGame->GetPlayerPed()->SetCollisionProcessed(FALSE);
        g_pGame->m_pCamera->Restore();
    }
    g_pGame->EnableHUD(FALSE);
    if (modelWaitActive)
        modelWaitTick = (DWORD)RakNet::GetTime();
}

void CNetGame::InitialNotification()
{
    if (g_pChat)
        g_pChat->AddDebugMessage(g_netStartedMessage.c_str());
}

void CNetGame::InitializeGameLogic()
{
    if (settings->manualVehicleControl)
        GameApplyManualVehicleControlPatches();
    *(DWORD*)&settings->worldBounds[0] = 0x469c4000;
    *(DWORD*)&settings->worldBounds[1] = 0xc69c4000;
    *(DWORD*)&settings->worldBounds[2] = 0x469c4000;
    *(DWORD*)&settings->worldBounds[3] = 0xc69c4000;
}

void CNetGame::Packet_DisconnectionNotification(Packet* packet)
{
    (void)packet;
    g_pChat->AddDebugMessage(g_netDisconnectedMessage);
    if (g_renderAudioStream)
        g_renderAudioStream->Stop(true);
    rakClient->Disconnect(2000, 0);
}

void CNetGame::Packet_InvalidPassword(Packet* packet)
{
    (void)packet;
    g_pChat->AddDebugMessage(g_netInvalidPasswordMessage);
    rakClient->Disconnect(0, 0);
}

void CNetGame::ResetMapIcons()
{
    for (BYTE index = 0; index < 100; index++)
    {
        DWORD* icon = &mapIcons[index];
        if (*icon)
        {
            Scripting::ProcessCommand(&g_netRemoveMapIconCommand, *icon);
            *icon = 0;
        }
    }
}

void CNetGame::CreateMapIcon(BYTE index, CVector position, BYTE icon, DWORD color, int type)
{
    if (index < 100)
    {
        DWORD* slot = &mapIcons[index];
        if (*slot)
        {
            Scripting::ProcessCommand(&g_netRemoveMapIconCommand, *slot);
            *slot = 0;
        }
        *slot = g_pGame->CreateMarker(icon, position.x, position.y, position.z, color, type);
    }
}

void CNetGame::ResetPools()
{
    ResetVehiclePool();
    ResetActorPool();
    ResetTextDrawPool();
    ResetGangZonePool();
    ResetLabelPool();
    ResetMapIcons();
    ResetPickupPool();
    ResetObjectPool();
    ResetMenuPool();
}

void CNetGame::Destroy()
{
    rakClient->Disconnect(0, 0);
    UnRegisterRPCs(rakClient);
    UnRegisterScriptRPCs(rakClient);
    RakNetworkFactory::DestroyRakClientInterface(rakClient);
    CPlayerPool* players = pools->players;
    if (players)
    {
        delete players;
        pools->players = 0;
    }
    CVehiclePool* vehicles = pools->vehicles;
    if (vehicles)
    {
        delete vehicles;
        pools->vehicles = 0;
    }
    CActorPool* actors = pools->actors;
    if (actors)
    {
        delete actors;
        pools->actors = 0;
    }
    CTextDrawPool* textDraws = pools->textDraws;
    if (textDraws)
    {
        delete textDraws;
        pools->textDraws = 0;
    }
    CGangZonePool* gangZones = pools->gangZones;
    if (gangZones)
    {
        delete gangZones;
        pools->gangZones = 0;
    }
    CLabelPool* labels = pools->labels;
    if (labels)
    {
        delete labels;
        pools->labels = 0;
    }
    CPickupPool* pickups = pools->pickups;
    if (pickups)
    {
        delete pickups;
        pools->pickups = 0;
    }
    CObjectPool* objects = pools->objects;
    if (objects)
    {
        delete objects;
        pools->objects = 0;
    }
    CMenuPool* menus = pools->menus;
    if (menus)
    {
        delete menus;
        pools->menus = 0;
    }
    if (pools)
    {
        operator delete(pools);
        pools = 0;
    }
    void* savedSettings = settings;
    if (savedSettings)
    {
        operator delete(savedSettings);
        settings = 0;
    }
}

void CNetGame::Connect()
{
    DWORD(WINAPI * tick)() = GetTickCount;
    if (tick() - lastConnectAttempt > 3000)
    {
        if (g_pChat)
            g_pChat->AddDebugMessage(g_netConnectingMessage, hostOrIp, serverPort);
        rakClient->Connect(hostOrIp, (WORD)serverPort, 0, 0, 2);
        lastConnectAttempt = tick();
        gameState = StateConnecting;
    }
}

void CNetGame::SpawnScreen()
{
    if (g_pGame->GetPlayerPed()->IsInVehicle())
        g_pGame->GetPlayerPed()->WarpFromVehicle(1093.4000244140625f, -2036.5f, 82.710601806640625f);
    else
    {
        CVector position = {1133.0504150390625f, -2038.4034423828125f, 69.09999847412109375f};
        g_pGame->GetPlayerPed()->Teleport(position);
    }
    CVector position = {1093.0f, -2036.0f, 90.0f}, rotation = {0, 0, 0};
    g_pGame->m_pCamera->Set(position, rotation);
    CVector point = {384.0f, -1557.0f, 20.0f};
    g_pGame->m_pCamera->PointAt(point, 2);
    g_pGame->SetWorldWeather(1);
    g_pGame->EnableHUD(FALSE);
}

void CNetGame::Packet_RSAPublicKeyMismatch(Packet* packet)
{
    (void)packet;
    g_pChat->AddDebugMessage(g_netEncryptionFailedMessage);
}

void CNetGame::Packet_ConnectionBanned(Packet* packet)
{
    (void)packet;
    g_pChat->AddDebugMessage(g_netConnectionBannedMessage);
}

void CNetGame::Packet_ConnectionRequestAccepted(Packet* packet)
{
    (void)packet;
    g_pChat->AddDebugMessage(g_netConnectionAcceptedMessage);
}

void CNetGame::Packet_NoFreeIncomingConnections(Packet* packet)
{
    (void)packet;
    g_pChat->AddDebugMessage(g_netServerFullMessage);
    gameState = StateWaitingToConnect;
}

void CNetGame::Packet_ConnectionAttemptFailed(Packet* packet)
{
    (void)packet;
    g_pChat->AddDebugMessage(g_netNoResponseMessage);
    gameState = StateWaitingToConnect;
}

void CNetGame::InitializePools()
{
    pools = (CNetGamePools*)calloc(1, sizeof(CNetGamePools));
    CObjectPool* objects = new CObjectPool;
    pools->objects = objects;
    CMenuPool* menus = new CMenuPool;
    pools->menus = menus;
    CLabelPool* labels = new CLabelPool;
    pools->labels = labels;
    CTextDrawPool* textDraws = new CTextDrawPool;
    pools->textDraws = textDraws;
    CGangZonePool* gangZones = new CGangZonePool;
    pools->gangZones = gangZones;
    CPlayerPool* players = new CPlayerPool;
    pools->players = players;
    CVehiclePool* vehicles = new CVehiclePool;
    pools->vehicles = vehicles;
    CPickupPool* pickups = new CPickupPool;
    pools->pickups = pickups;
    CActorPool* actors = new CActorPool;
    pools->actors = actors;
}

void CNetGame::ResetPlayerPool()
{
    CPlayerPool* old = pools->players;
    if (old)
        delete old;
    CPlayerPool* created = new CPlayerPool;
    pools->players = created;
}

void CNetGame::ResetVehiclePool()
{
    CVehiclePool* old = pools->vehicles;
    if (old)
        delete old;
    CVehiclePool* created = new CVehiclePool;
    pools->vehicles = created;
}

void CNetGame::ResetTextDrawPool()
{
    CTextDrawPool* old = pools->textDraws;
    if (old)
        delete old;
    CTextDrawPool* created = new CTextDrawPool;
    pools->textDraws = created;
}

void CNetGame::ResetObjectPool()
{
    CObjectPool* old = pools->objects;
    if (old)
        delete old;
    CObjectPool* created = new CObjectPool;
    pools->objects = created;
}

void CNetGame::ResetGangZonePool()
{
    CGangZonePool* old = pools->gangZones;
    if (old)
        delete old;
    CGangZonePool* created = new CGangZonePool;
    pools->gangZones = created;
}

void CNetGame::ResetPickupPool()
{
    CPickupPool* old = pools->pickups;
    if (old)
        delete old;
    CPickupPool* created = new CPickupPool;
    pools->pickups = created;
}

void CNetGame::ResetMenuPool()
{
    CMenuPool* old = pools->menus;
    if (old)
        delete old;
    CMenuPool* created = new CMenuPool;
    pools->menus = created;
}

void CNetGame::ResetLabelPool()
{
    CLabelPool* old = pools->labels;
    if (old)
        delete old;
    CLabelPool* created = new CLabelPool;
    pools->labels = created;
}

void CNetGame::ResetActorPool()
{
    CActorPool* old = pools->actors;
    if (old)
        delete old;
    CActorPool* created = new CActorPool;
    pools->actors = created;
}

void CNetGame::RemoveMapIcon(BYTE index)
{
    if (index < 100)
    {
        DWORD* icon = &mapIcons[index];
        Scripting::ProcessCommand(&g_netRemoveMapIconCommand, *icon);
        *icon = 0;
    }
}

void CNetGame::SetGameState(DWORD state)
{
    gameState = state;
}

CPickupPool* CNetGame::GetPickupPool()
{
    return pools->pickups;
}

__declspec(naked) __int64 CNetGame::GetCounter()
{
    // clang-format off
    // Do not zero-extend a typed32-bit return: native forwards both registers.
    __asm jmp RakNet::GetTime
    // clang-format on
}

void CNetGame::ToggleNameTagStatus()
{
    if (!showNameTagStatus)
    {
        g_pChat->AddInfoMessage("NameTag Player Status: ON");
        if (g_pConfig)
            g_pConfig->Write("nonametagstatus", 0, FALSE);
        showNameTagStatus = 1;
    }
    else
    {
        g_pChat->AddInfoMessage("NameTag Player Status: OFF");
        if (g_pConfig)
            g_pConfig->Write("nonametagstatus", 1, FALSE);
        showNameTagStatus = 0;
    }
}

void CNetGame::UpdatePlayersInfo()
{
    if (GetTickCount() - g_netPlayersInfoTick > 3000)
    {
        g_netPlayersInfoTick = GetTickCount();
        RakNet::BitStream stream;
        RakClientInterface* client = rakClient;
        if (client)
            client->RPC(&g_netPlayersInfoRpc, &stream, HIGH_PRIORITY, RELIABLE, 0, false);
    }
}

CMenuPool* CNetGame::GetMenuPool()
{
    return pools->menus;
}

CObjectPool* CNetGame::GetObjectPool()
{
    return pools->objects;
}

CVehiclePool* CNetGame::GetVehiclePool()
{
    return pools->vehicles;
}

CPlayerPool* CNetGame::GetPlayerPool()
{
    return pools->players;
}

CLabelPool* CNetGame::GetLabelPool()
{
    return pools->labels;
}

void CNetGame::SetLanMode(DWORD value)
{
    lanMode = value;
}

DWORD CNetGame::GetUseCJWalk()
{
    return settings->useCJWalk;
}
