#ifndef SAMPSRV_NETGAME_H
#define SAMPSRV_NETGAME_H

#include "system.h"

#include <MTUSize.h>
#include <SimpleMutex.h>
#include <deque>

class RakServerInterface;

class CNetGame
{
public:
    CNetGame();
    ~CNetGame();
    bool Start();
    void Process();

private:
    CNetGame(const CNetGame&);
    CNetGame& operator=(const CNetGame&);

    static bool HandleQuery(SOCKET socket, const char* data, int length, const sockaddr_in& from);

    struct QueryPacket
    {
        SOCKET socket;
        sockaddr_in from;
        int length;
        char data[MAXIMUM_MTU_SIZE];
    };

    RakServerInterface* m_rak;
    SimpleMutex m_queryMutex;
    std::deque<QueryPacket> m_queries;
};

extern CNetGame* pNetGame;

#endif
