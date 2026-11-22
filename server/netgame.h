#ifndef SAMPSRV_NETGAME_H
#define SAMPSRV_NETGAME_H

#include "system.h"

#include <MTUSize.h>
#include <NetworkTypes.h>
#include <SimpleMutex.h>
#include <deque>

class RakServerInterface;

class CPlayerPool
{
public:
    enum { MaxPlayers = 1000, MaxNameLength = 20 };
    struct Slot
    {
        PlayerID address;
        char name[MaxNameLength + 1];
    };

    CPlayerPool();
    bool New(int index, PlayerID address, const char* name);
    int Delete(PlayerID address);
    const Slot* GetAt(int index) const;
    unsigned short GetCount() const { return m_count; }

private:
    Slot m_slots[MaxPlayers];
    unsigned short m_count;
};

class CNetGame
{
public:
    CNetGame();
    ~CNetGame();
    bool Start();
    void Process();
    const CPlayerPool& GetPlayerPool() const { return m_players; }
    int GetPlayerPing(PlayerID address) const;

private:
    CNetGame(const CNetGame&);
    CNetGame& operator=(const CNetGame&);

    static bool HandleQuery(SOCKET socket, const char* data, int length, const sockaddr_in& from);
    static void ClientJoin(RPCParameters* parameters);
    static void RequestClass(RPCParameters* parameters);
    void InitGameForPlayer(PlayerID address, unsigned short index);
    void ProcessDisconnect(PlayerID address, unsigned char reason);

    struct QueryPacket
    {
        SOCKET socket;
        sockaddr_in from;
        int length;
        char data[MAXIMUM_MTU_SIZE];
    };

    RakServerInterface* m_rak;
    CPlayerPool m_players;
    SimpleMutex m_queryMutex;
    std::deque<QueryPacket> m_queries;
};

extern CNetGame* pNetGame;

#endif
