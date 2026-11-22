#include "netgame.h"
#include "console.h"
#include "query.h"

#include <RakNetworkFactory.h>
#include <BitStream.h>
#include <PacketEnumerations.h>
#include <rakserverinterface.h>
#include <SocketLayer.h>
#include <ttmathuint.h>

#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <strings.h>
#endif

CNetGame* pNetGame;
unsigned int GetServerHandshakeSeed();

namespace
{
const unsigned int NetworkVersion = 4062;
int rpcClientJoin = 25;
int rpcRequestClass = 128;
int rpcConnectionRejected = 130;
int rpcServerJoin = 137;
int rpcServerQuit = 138;
int rpcInitGame = 139;

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
    m_slots[index].address = address;
    strcpy(m_slots[index].name, name);
    ++m_count;
    return true;
}

int CPlayerPool::Delete(PlayerID address)
{
    for (int i = 0; i < MaxPlayers; ++i)
    {
        if (GetAt(i) && m_slots[i].address == address)
        {
            m_slots[i].name[0] = 0;
            --m_count;
            return i;
        }
    }
    return -1;
}

CNetGame::CNetGame() : m_rak(RakNetworkFactory::GetRakServerInterface())
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
    response.Write(0);     // Spawn classes
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
    const int index = game.m_rak->GetIndexFromPlayerID(parameters->sender);
    const CPlayerPool::Slot* player = game.m_players.GetAt(index);
    if (!player || player->address != parameters->sender || parameters->numberOfBitsOfData < 32)
        return;

    RakNet::BitStream request(parameters->input, parameters->numberOfBitsOfData / 8, false);
    unsigned int requestedClass;
    if (!request.Read(requestedClass) || requestedClass >= 319)
        return;

    // Without registered classes, the player's spawn info stays zeroed.
    char response[51] = {1};
    RakNet::BitStream reply(reinterpret_cast<unsigned char*>(response), sizeof(response), false);
    game.m_rak->RPC(&rpcRequestClass, &reply, HIGH_PRIORITY, RELIABLE, 0, parameters->sender, false, false);
}

void CNetGame::ProcessDisconnect(PlayerID address, unsigned char reason)
{
    const int index = m_players.Delete(address);
    if (index < 0 || !m_players.GetCount())
        return;

    RakNet::BitStream response;
    response.Write(static_cast<unsigned short>(index));
    response.Write(reason);
    for (int i = 0; i < CPlayerPool::MaxPlayers; ++i)
    {
        const CPlayerPool::Slot* other = m_players.GetAt(i);
        if (other)
            m_rak->RPC(&rpcServerQuit, &response, HIGH_PRIORITY, RELIABLE_ORDERED, 0, other->address, false, false);
    }
}

int CNetGame::GetPlayerPing(PlayerID address) const
{
    return m_rak->GetLastPing(address);
}
