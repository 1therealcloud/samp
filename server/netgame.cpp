#include "netgame.h"
#include "console.h"
#include "query.h"

#include <RakNetworkFactory.h>
#include <BitStream.h>
#include <GetTime.h>
#include <PacketEnumerations.h>
#include <rakserverinterface.h>
#include <SocketLayer.h>
#include <ttmathuint.h>

#include "filterscripts.h"
#include "gamemodes.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef _WIN32
#include <strings.h>
#endif

#include "../source/static_assert.h"

CNetGame* pNetGame;
unsigned int GetServerHandshakeSeed();

namespace
{
STATIC_ASSERT(sizeof(PlayerSpawnInfo) == 50);
const unsigned int NetworkVersion = 4062;
int rpcClientJoin = 25;
int rpcSetPlayerPos = 12;
int rpcSetPlayerPosFindZ = 13;
int rpcSetPlayerHealth = 14;
int rpcSetPlayerArmour = 66;
int rpcWorldPlayerAdd = 32;
int rpcSetPlayerVirtualWorld = 48;
int rpcSpawn = 52;
int rpcDeath = 53;
int rpcRequestClass = 128;
int rpcRequestSpawn = 129;
int rpcConnectionRejected = 130;
int rpcServerJoin = 137;
int rpcServerQuit = 138;
int rpcInitGame = 139;
int rpcWorldPlayerRemove = 163;
int rpcWorldPlayerDeath = 166;

#pragma pack(push, 1)
struct OnFootSyncData
{
    unsigned short leftRight, upDown, keys;
    float position[3];
    float rotation[4];
    unsigned char health, armour, weapon, specialAction;
    float velocity[3];
    float surfingOffset[3];
    unsigned short surfingId;
    unsigned int animation;
};
#pragma pack(pop)

STATIC_ASSERT(sizeof(OnFootSyncData) == 68);

unsigned char CompressHealth(unsigned char value)
{
    return value >= 100 ? 15 : value / 7;
}

// 0x0048ed50 (Windows)
bool ValidSyncPosition(const float* position)
{
    return fabs(position[0]) < 20000.0f && fabs(position[1]) < 20000.0f &&
           position[2] > -1000.0f && position[2] < 200000.0f;
}

// 0x0048edc0 (Windows)
bool ValidSyncVelocity(const float* velocity)
{
    return fabs(velocity[0]) < 100.0f && fabs(velocity[1]) < 100.0f && fabs(velocity[2]) < 100.0f;
}

bool IsValidName(const char* name, unsigned char length)
{
    const char valid[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_[]()$@.=";
    return length >= 3 && length <= CPlayerPool::MaxNameLength && strspn(name, valid) == length;
}

bool SameName(const char* first, const char* second)
{
#ifdef _WIN32
    return _stricmp(first, second) == 0;
#else
    return strcasecmp(first, second) == 0;
#endif
}

bool IsValidGpci(const char* gpci, unsigned char length)
{
    if (!length || strspn(gpci, "0123456789ABCDEFabcdef") != length)
        return false;

    ttmath::UInt<100> value;
    ttmath::uint remainder;
    std::string quotient;
    if (value.FromString(gpci, 16) || value.DivInt(1001, remainder) || remainder)
        return false;
    value.ToString(quotient, 16);
    return quotient.size() > 1 && quotient.size() <= 40;
}

void SendServerJoin(RakServerInterface* rak, PlayerID target, unsigned short index, const char* name)
{
    RakNet::BitStream response;
    const unsigned char length = static_cast<unsigned char>(strlen(name));
    response.Write(index);
    response.Write(0U);
    response.Write(static_cast<unsigned char>(0));
    response.Write(length);
    response.Write(name, length);
    rak->RPC(&rpcServerJoin, &response, HIGH_PRIORITY, RELIABLE_ORDERED, 0, target, false, false);
}
}

CPlayerPool::CPlayerPool() : m_count(0)
{
    memset(m_slots, 0, sizeof(m_slots));
}

const CPlayerPool::Slot* CPlayerPool::GetAt(int index) const
{
    return index >= 0 && index < MaxPlayers && m_slots[index].name[0] ? &m_slots[index] : NULL;
}

bool CPlayerPool::New(int index, PlayerID address, const char* name)
{
    if (index < 0 || index >= MaxPlayers || GetAt(index))
        return false;
    memset(&m_slots[index], 0, sizeof(Slot));
    m_slots[index].address = address;
    strcpy(m_slots[index].name, name);
    ++m_count;
    return true;
}

int CPlayerPool::Find(PlayerID address) const
{
    for (int i = 0; i < MaxPlayers; ++i)
        if (GetAt(i) && m_slots[i].address == address)
            return i;
    return -1;
}

void CPlayerPool::Delete(int index)
{
    m_slots[index].name[0] = 0;
    --m_count;
}

CPlayerPool::Slot* CPlayerPool::GetAt(int index)
{
    return const_cast<Slot*>(static_cast<const CPlayerPool&>(*this).GetAt(index));
}

CNetGame::CNetGame() : m_rak(RakNetworkFactory::GetRakServerInterface()), m_spawnClassCount(0)
{
    pNetGame = this;
}

CNetGame::~CNetGame()
{
    m_rak->Disconnect(0);
    SocketLayer::Instance()->SetQueryHandler(0);
    RakNetworkFactory::DestroyRakServerInterface(m_rak);
    pNetGame = NULL;
}

// Network initialization from CNetGame::CNetGame at 0x004946b0 (Windows).
bool CNetGame::Start()
{
    const char* bindAddress = pConsole->GetStringVariable("bind");
    const int port = pConsole->GetIntVariable("port");
    const int maxPlayers = pConsole->GetIntVariable("maxplayers");
    if (port < 1 || port > 65535)
    {
        logprintf("Invalid server port: %d.", port);
        return false;
    }

    SocketLayer::Instance()->SetQueryHandler(HandleQuery);
    m_rak->RegisterAsRemoteProcedureCall(&rpcClientJoin, ClientJoin);
    m_rak->RegisterAsRemoteProcedureCall(&rpcRequestClass, RequestClass);
    m_rak->RegisterAsRemoteProcedureCall(&rpcRequestSpawn, RequestSpawn);
    m_rak->RegisterAsRemoteProcedureCall(&rpcSpawn, Spawn);
    m_rak->RegisterAsRemoteProcedureCall(&rpcDeath, Death);
    if (!m_rak->Start(static_cast<unsigned short>(maxPlayers), 0, 10, static_cast<unsigned short>(port),
                      bindAddress && *bindAddress ? bindAddress : NULL))
    {
        logprintf("Unable to start server on %s:%d.", bindAddress && *bindAddress ? bindAddress : "0.0.0.0", port);
        return false;
    }

    const char* password = pConsole->GetStringVariable("password");
    m_rak->SetPassword(password && *password ? password : NULL);
    m_rak->StartOccasionalPing();
    logprintf("Started server on %s:%d.", bindAddress && *bindAddress ? bindAddress : "0.0.0.0", port);
    return true;
}

bool CNetGame::HandleQuery(SOCKET socket, const char* data, int length, const sockaddr_in& from)
{
    if (length < 11 || length > MAXIMUM_MTU_SIZE)
        return true;

    QueryPacket packet = {0};
    switch (data[10])
    {
        case 'p':
            if (length != 15)
                return true;
            packet.length = 15;
            break;
        case 'i':
        case 'r':
        case 'c':
        case 'd':
            packet.length = 11;
            break;
        case 'x':
            packet.length = length;
            break;
        default:
            return true;
    }

    packet.socket = socket;
    packet.from = from;
    memcpy(packet.data, data, packet.length);

    // RecvFrom runs on the RakNet thread. CConsole belongs to the server thread.
    pNetGame->m_queryMutex.Lock();
    if (pNetGame->m_queries.size() < 128)
        pNetGame->m_queries.push_back(packet);
    pNetGame->m_queryMutex.Unlock();
    return true;
}

void CNetGame::Process()
{
    std::deque<QueryPacket> queries;
    m_queryMutex.Lock();
    queries.swap(m_queries);
    m_queryMutex.Unlock();

    for (std::deque<QueryPacket>::iterator it = queries.begin(); it != queries.end(); ++it)
    {
        QueryPacket& packet = *it;
        ProcessQueryPacket(packet.from.sin_addr.s_addr, ntohs(packet.from.sin_port),
                           packet.data, packet.length, packet.socket);
    }

    for (int i = 0; i < 128; ++i)
    {
        Packet* packet = m_rak->Receive();
        if (!packet)
            break;
        if (packet->length)
        {
            if (packet->data[0] == ID_DISCONNECTION_NOTIFICATION)
                ProcessDisconnect(packet->playerId, 1);
            else if (packet->data[0] == ID_CONNECTION_LOST)
                ProcessDisconnect(packet->playerId, 0);
            else if (packet->data[0] == ID_PLAYER_SYNC)
                Packet_PlayerSync(*packet);
        }
        m_rak->DeallocatePacket(packet);
    }
}

// 0x0049cea0 (Windows)
void CNetGame::ClientJoin(RPCParameters* parameters)
{
    CNetGame& game = *pNetGame;
    RakServerInterface* rak = game.m_rak;
    const PlayerID sender = parameters->sender;
    const int index = rak->GetIndexFromPlayerID(sender);
    if (!rak->IsActivePlayerID(sender) || index < 0 || index >= CPlayerPool::MaxPlayers ||
        game.m_players.GetAt(index))
        return;

    RakNet::BitStream stream(parameters->input, parameters->numberOfBitsOfData / 8, false);
    unsigned int version, challenge, repeatedChallenge;
    unsigned char mod, nameLength, gpciLength, versionLength;
    char name[256], gpci[256], clientVersion[256];
    if (!stream.Read(version) || !stream.Read(mod) ||
        !stream.Read(nameLength) || !stream.Read(name, nameLength) || !stream.Read(challenge) ||
        !stream.Read(gpciLength) || !stream.Read(gpci, gpciLength) ||
        !stream.Read(versionLength) || versionLength > 24 || !stream.Read(clientVersion, versionLength) ||
        !stream.Read(repeatedChallenge))
        return;

    name[nameLength] = 0;
    gpci[gpciLength] = 0;
    unsigned char rejection = 0;
    if (version != NetworkVersion || (challenge ^ NetworkVersion) != GetServerHandshakeSeed() ||
        repeatedChallenge != challenge)
        rejection = 1;
    else if (mod != 1)
        rejection = 3;
    else if (!IsValidName(name, nameLength))
        rejection = 2;

    for (int i = 0; !rejection && i < CPlayerPool::MaxPlayers; ++i)
    {
        const CPlayerPool::Slot* other = game.m_players.GetAt(i);
        if (other && SameName(other->name, name))
            rejection = 2;
    }
    if (!rejection && !IsValidGpci(gpci, gpciLength))
    {
        rak->Kick(sender);
        return;
    }
    if (!rejection && !game.m_players.New(index, sender, name))
        rejection = 4;
    if (rejection)
    {
        RakNet::BitStream response;
        response.Write(rejection);
        rak->RPC(&rpcConnectionRejected, &response, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, false, false);
        rak->Kick(sender);
        return;
    }

    for (int i = 0; i < CPlayerPool::MaxPlayers; ++i)
    {
        const CPlayerPool::Slot* other = game.m_players.GetAt(i);
        if (other && i != index)
        {
            SendServerJoin(rak, sender, static_cast<unsigned short>(i), other->name);
            SendServerJoin(rak, other->address, static_cast<unsigned short>(index), name);
        }
    }
    game.InitGameForPlayer(sender, static_cast<unsigned short>(index));
    const cell playerId = index;
    pFilterScripts->CallPublic("OnPlayerConnect", &playerId, 1, 1);
    pGameMode->CallPublic("OnPlayerConnect", &playerId, 1, 1);
}

// 0x00495dc0 (Windows)
void CNetGame::InitGameForPlayer(PlayerID address, unsigned short index)
{
    RakNet::BitStream response;
    response.Write(false); // Zone names
    response.Write(false); // CJ walk
    response.Write(false); // Weapons
    response.Write(false); // Limit global chat radius
    response.Write(10000.0f);
    response.Write(true);  // Stunt bonus
    response.Write(70.0f); // Nametag distance
    response.Write(false); // Disable enter/exits
    response.Write(true);  // Nametag line of sight
    response.Write(false); // Manual vehicle control
    response.Write(m_spawnClassCount);
    response.Write(index);
    response.Write(true);  // Show nametags
    response.Write(1);     // Player markers
    response.Write(static_cast<unsigned char>(12));
    response.Write(static_cast<unsigned char>(1)); // Initial world weather
    const char* gravity = pConsole->GetStringVariable("gravity");
    response.Write(gravity ? static_cast<float>(atof(gravity)) : 0.008f);
    response.Write(pConsole->GetBoolVariable("lanmode"));
    response.Write(0);     // Death drop money
    response.Write(false); // Instagib
    response.Write(pConsole->GetIntVariable("onfoot_rate"));
    response.Write(pConsole->GetIntVariable("incar_rate"));
    response.Write(pConsole->GetIntVariable("weapon_rate"));
    response.Write(2);     // Send multiplier
    response.Write(pConsole->GetIntVariable("lagcompmode"));
    const char* hostname = pConsole->GetStringVariable("hostname");
    const size_t length = hostname ? strlen(hostname) : 0;
    const unsigned char hostnameLength = static_cast<unsigned char>(length > 255 ? 255 : length);
    response.Write(hostnameLength);
    if (hostnameLength)
        response.Write(hostname, hostnameLength);
    response.Write(static_cast<unsigned char>(0));
    char models[212] = {0};
    response.Write(models, sizeof(models));
    response.Write(0);     // Vehicle damage filter
    m_rak->RPC(&rpcInitGame, &response, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address, false, false);
}

// 0x00498560 (Windows)
void CNetGame::RequestClass(RPCParameters* parameters)
{
    CNetGame& game = *pNetGame;
    int index;
    CPlayerPool::Slot* player = game.GetRpcPlayer(parameters, index);
    if (!player || parameters->numberOfBitsOfData < 32)
        return;

    RakNet::BitStream request(parameters->input, parameters->numberOfBitsOfData / 8, false);
    unsigned int requestedClass;
    if (!request.Read(requestedClass) || requestedClass >= MaxSpawnClasses)
        return;

    if (requestedClass < static_cast<unsigned int>(game.m_spawnClassCount))
    {
        player->spawnInfo = game.m_spawnClasses[requestedClass];
        player->hasSpawnInfo = true;
    }
    player->spawnApproved = false;
    const cell args[] = {index, static_cast<cell>(requestedClass)};
    pFilterScripts->CallPublic("OnPlayerRequestClass", args, 2, 1);
    const unsigned char outcome = static_cast<unsigned char>(pGameMode->CallPublic("OnPlayerRequestClass", args, 2, 1));
    RakNet::BitStream reply;
    reply.Write(outcome);
    reply.Write(reinterpret_cast<const char*>(&player->spawnInfo), sizeof(PlayerSpawnInfo));
    game.m_rak->RPC(&rpcRequestClass, &reply, HIGH_PRIORITY, RELIABLE, 0, parameters->sender, false, false);
}

CPlayerPool::Slot* CNetGame::GetRpcPlayer(RPCParameters* parameters, int& index)
{
    index = m_rak->GetIndexFromPlayerID(parameters->sender);
    CPlayerPool::Slot* player = m_players.GetAt(index);
    return player && player->address == parameters->sender ? player : NULL;
}

// 0x00498780 (Windows)
void CNetGame::RequestSpawn(RPCParameters* parameters)
{
    CNetGame& game = *pNetGame;
    int index;
    CPlayerPool::Slot* player = game.GetRpcPlayer(parameters, index);
    if (!player)
        return;

    const cell playerId = index;
    player->spawnApproved = player->hasSpawnInfo &&
                            pFilterScripts->CallPublic("OnPlayerRequestSpawn", &playerId, 1, 1) &&
                            pGameMode->CallPublic("OnPlayerRequestSpawn", &playerId, 1, 1);
    RakNet::BitStream reply;
    reply.Write(static_cast<unsigned char>(player->spawnApproved));
    game.m_rak->RPC(&rpcRequestSpawn, &reply, HIGH_PRIORITY, RELIABLE, 0, parameters->sender, false, false);
}

// 0x00497750 (Windows)
void CNetGame::Spawn(RPCParameters* parameters)
{
    int index;
    CPlayerPool::Slot* player = pNetGame->GetRpcPlayer(parameters, index);
    if (!player || (!player->spawnApproved && !player->dead) ||
        !player->hasSpawnInfo || player->spawnInfo.skin < 0 || player->spawnInfo.skin > 319)
        return;

    player->spawnApproved = false;
    player->dead = false;
    player->position[0] = player->spawnInfo.x;
    player->position[1] = player->spawnInfo.y;
    player->position[2] = player->spawnInfo.z;
    CNetGame& game = *pNetGame;
    player->spawned = true;
    game.UpdatePlayerStreaming(index, *player, true);
    const cell playerId = index;
    pFilterScripts->CallPublic("OnPlayerSpawn", &playerId, 1, 1);
    pGameMode->CallPublic("OnPlayerSpawn", &playerId, 1, 1);
}

// 0x00498950 and 0x00485020 (Windows)
void CNetGame::Death(RPCParameters* parameters)
{
    CNetGame& game = *pNetGame;
    int index;
    CPlayerPool::Slot* player = game.GetRpcPlayer(parameters, index);
    if (!player || !player->spawned || player->dead || parameters->numberOfBitsOfData != 24)
        return;

    RakNet::BitStream request(parameters->input, 3, false);
    unsigned char reason;
    unsigned short killer;
    if (!request.Read(reason) || !request.Read(killer))
        return;

    player->dead = true;
    player->spawnApproved = false;
    if (!game.m_players.GetAt(killer))
        killer = 0xffff;

    const cell args[] = {index, killer, reason};
    pFilterScripts->CallPublic("OnPlayerDeath", args, 3, 1);
    pGameMode->CallPublic("OnPlayerDeath", args, 3, 1);

    RakNet::BitStream reply;
    reply.Write(static_cast<unsigned short>(index));
    for (int i = 0; i < CPlayerPool::MaxPlayers; ++i)
    {
        const CPlayerPool::Slot* other = game.m_players.GetAt(i);
        if (other && game.m_streamed[index].test(i))
            game.m_rak->RPC(&rpcWorldPlayerDeath, &reply, HIGH_PRIORITY, RELIABLE_ORDERED, 0, other->address, false, false);
    }
}

void CNetGame::WorldPlayerAdd(unsigned short index, const CPlayerPool::Slot& player, PlayerID recipient)
{
    RakNet::BitStream reply;
    reply.Write(index);
    reply.Write(player.spawnInfo.team);
    reply.Write(player.spawnInfo.skin);
    reply.Write(player.spawnInfo.customSkin);
    for (int i = 0; i < 3; ++i)
        reply.Write(player.position[i]);
    reply.Write(player.spawnInfo.angle);
    reply.Write(0U); // Color
    reply.Write(static_cast<unsigned char>(4)); // Fighting style
    const char skills[22] = {0};
    reply.Write(skills, sizeof(skills));
    m_rak->RPC(&rpcWorldPlayerAdd, &reply, HIGH_PRIORITY, RELIABLE_ORDERED, 0, recipient, false, false);
}

void CNetGame::WorldPlayerRemove(unsigned short index, PlayerID recipient)
{
    RakNet::BitStream reply;
    reply.Write(index);
    m_rak->RPC(&rpcWorldPlayerRemove, &reply, HIGH_PRIORITY, RELIABLE_ORDERED, 0, recipient, false, false);
}

void CNetGame::UpdatePlayerStreaming(int index, CPlayerPool::Slot& player, bool refresh)
{
    player.lastStreamUpdate = RakNet::GetTime();
    const float distance = pConsole->GetFloatVariable("stream_distance");
    const float distanceSquared = distance * distance;
    for (int i = 0; i < CPlayerPool::MaxPlayers; ++i)
    {
        const CPlayerPool::Slot* other = m_players.GetAt(i);
        if (!other || !other->spawned || i == index)
            continue;

        const float x = player.position[0] - other->position[0];
        const float y = player.position[1] - other->position[1];
        const bool streamed = m_streamed[index].test(i);
        const bool visible = player.virtualWorld == other->virtualWorld && x * x + y * y < distanceSquared &&
                             (streamed || (!player.dead && !other->dead));
        if (visible == streamed)
        {
            if (visible && refresh)
                WorldPlayerAdd(static_cast<unsigned short>(index), player, other->address);
            continue;
        }

        m_streamed[index].set(i, visible);
        m_streamed[i].set(index, visible);
        if (visible)
        {
            WorldPlayerAdd(static_cast<unsigned short>(index), player, other->address);
            WorldPlayerAdd(static_cast<unsigned short>(i), *other, player.address);
        }
        else
        {
            WorldPlayerRemove(static_cast<unsigned short>(index), other->address);
            WorldPlayerRemove(static_cast<unsigned short>(i), player.address);
        }
    }
}

// 0x00466a20 (Windows)
bool CNetGame::SetPlayerVirtualWorld(int index, int world)
{
    CPlayerPool::Slot* player = m_players.GetAt(index);
    if (!player)
        return false;
    if (player->virtualWorld == world)
        return true;

    player->virtualWorld = world;
    RakNet::BitStream reply;
    reply.Write(world);
    m_rak->RPC(&rpcSetPlayerVirtualWorld, &reply, HIGH_PRIORITY, RELIABLE_ORDERED, 0, player->address, false, false);
    if (player->spawned)
        UpdatePlayerStreaming(index, *player, false);
    return true;
}

bool CNetGame::SetPlayerPos(int index, const float position[3], bool findGround)
{
    const CPlayerPool::Slot* player = m_players.GetAt(index);
    if (!player)
        return false;

    RakNet::BitStream reply;
    for (int i = 0; i < 3; ++i)
        reply.Write(position[i]);
    int* rpc = findGround ? &rpcSetPlayerPosFindZ : &rpcSetPlayerPos;
    m_rak->RPC(rpc, &reply, HIGH_PRIORITY, RELIABLE_ORDERED, 2, player->address, false, false);
    return true;
}

bool CNetGame::SetPlayerVitals(int index, float value, bool armour)
{
    const CPlayerPool::Slot* player = m_players.GetAt(index);
    if (!player)
        return false;

    RakNet::BitStream reply;
    reply.Write(value);
    int* rpc = armour ? &rpcSetPlayerArmour : &rpcSetPlayerHealth;
    m_rak->RPC(rpc, &reply, HIGH_PRIORITY, RELIABLE_ORDERED, 2, player->address, false, false);
    return true;
}

// 0x00494dc0 (Windows)
void CNetGame::Packet_PlayerSync(const Packet& packet)
{
    if (packet.length != 1 + sizeof(OnFootSyncData))
        return;

    const int index = m_rak->GetIndexFromPlayerID(packet.playerId);
    CPlayerPool::Slot* player = m_players.GetAt(index);
    if (!player || !player->spawned || player->dead || player->address != packet.playerId)
        return;

    OnFootSyncData data;
    memcpy(&data, packet.data + 1, sizeof(data));
    if (!ValidSyncPosition(data.position) || !ValidSyncVelocity(data.velocity) ||
        (data.surfingId && !ValidSyncPosition(data.surfingOffset)))
        return;
    for (int i = 0; i < 4; ++i)
        if (!(fabs(data.rotation[i]) <= 1.0f))
            return;

    memcpy(player->position, data.position, sizeof(player->position));
    player->health = data.health;
    player->armour = data.armour;
    const unsigned int streamRate = pConsole->GetIntVariable("stream_rate");
    if (RakNet::GetTime() - player->lastStreamUpdate >= streamRate)
        UpdatePlayerStreaming(index, *player, false);

    RakNet::BitStream reply;
    reply.Write(static_cast<unsigned char>(ID_PLAYER_SYNC));
    reply.Write(static_cast<unsigned short>(index));
    reply.Write(data.leftRight != 0);
    if (data.leftRight)
        reply.Write(data.leftRight);
    reply.Write(data.upDown != 0);
    if (data.upDown)
        reply.Write(data.upDown);
    reply.Write(data.keys);
    reply.Write(reinterpret_cast<const char*>(data.position), sizeof(data.position));
    reply.WriteNormQuat(data.rotation[0], data.rotation[1], data.rotation[2], data.rotation[3]);
    reply.Write(static_cast<unsigned char>((CompressHealth(data.health) << 4) | CompressHealth(data.armour)));
    reply.Write(data.weapon);
    reply.Write(data.specialAction);
    const float magnitude = static_cast<float>(sqrt(data.velocity[0] * data.velocity[0] +
                                                    data.velocity[1] * data.velocity[1] +
                                                    data.velocity[2] * data.velocity[2]));
    reply.Write(magnitude);
    if (magnitude > 0.00001f)
        for (int i = 0; i < 3; ++i)
            reply.WriteCompressed(data.velocity[i] / magnitude);
    reply.Write(data.surfingId != 0);
    if (data.surfingId)
    {
        reply.Write(data.surfingId);
        reply.Write(reinterpret_cast<const char*>(data.surfingOffset), sizeof(data.surfingOffset));
    }
    reply.Write(data.animation != 0);
    if (data.animation)
        reply.Write(data.animation);

    for (int i = 0; i < CPlayerPool::MaxPlayers; ++i)
    {
        const CPlayerPool::Slot* other = m_players.GetAt(i);
        if (other && m_streamed[index].test(i))
            m_rak->Send(&reply, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0, other->address, false);
    }
}

// 0x004943d0 (Windows)
int CNetGame::AddPlayerClass(const PlayerSpawnInfo& info)
{
    const int index = m_spawnClassCount;
    if (index < MaxSpawnClasses)
        m_spawnClasses[m_spawnClassCount++] = info;
    return index;
}

void CNetGame::ProcessDisconnect(PlayerID address, unsigned char reason)
{
    const int index = m_players.Find(address);
    if (index < 0)
        return;

    const cell args[] = {index, reason};
    pFilterScripts->CallPublic("OnPlayerDisconnect", args, 2, 1);
    pGameMode->CallPublic("OnPlayerDisconnect", args, 2, 1);
    m_players.Delete(index);

    RakNet::BitStream response;
    response.Write(static_cast<unsigned short>(index));
    response.Write(reason);
    for (int i = 0; i < CPlayerPool::MaxPlayers; ++i)
    {
        const CPlayerPool::Slot* other = m_players.GetAt(i);
        if (other)
        {
            if (m_streamed[index].test(i))
            {
                WorldPlayerRemove(static_cast<unsigned short>(index), other->address);
                m_streamed[i].reset(index);
            }
            m_rak->RPC(&rpcServerQuit, &response, HIGH_PRIORITY, RELIABLE_ORDERED, 0, other->address, false, false);
        }
    }
    m_streamed[index].reset();
}

int CNetGame::GetPlayerPing(PlayerID address) const
{
    return m_rak->GetLastPing(address);
}
