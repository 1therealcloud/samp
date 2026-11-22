#include "netgame.h"
#include "console.h"
#include "query.h"

#include <RakNetworkFactory.h>
#include <rakserverinterface.h>
#include <SocketLayer.h>

#include <string.h>

CNetGame* pNetGame;

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
        m_rak->DeallocatePacket(packet);
    }
}
