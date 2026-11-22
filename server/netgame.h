#ifndef SAMPSRV_NETGAME_H
#define SAMPSRV_NETGAME_H

#include "system.h"

#include <MTUSize.h>
#include <NetworkTypes.h>
#include <SimpleMutex.h>
#include <bitset>
#include <deque>

class RakServerInterface;

#pragma pack(push, 1)
struct PlayerSpawnInfo
{
    unsigned char team;
    int skin;
    int customSkin;
    unsigned char reserved;
    float x, y, z, angle;
    int weapons[3];
    int ammunition[3];
};
#pragma pack(pop)

class CPlayerPool
{
public:
    enum { MaxPlayers = 1000, MaxNameLength = 20 };
    struct Slot
    {
        PlayerID address;
        char name[MaxNameLength + 1];
        PlayerSpawnInfo spawnInfo;
        float position[3];
        float health, armour;
        int virtualWorld;
        unsigned int lastStreamUpdate;
        bool hasSpawnInfo;
        bool spawnApproved;
        bool spawned;
        bool dead;
    };

    CPlayerPool();
    bool New(int index, PlayerID address, const char* name);
    int Find(PlayerID address) const;
    void Delete(int index);
    const Slot* GetAt(int index) const;
    Slot* GetAt(int index);
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
    int AddPlayerClass(const PlayerSpawnInfo& info);
    bool SetPlayerVirtualWorld(int index, int world);
    bool SetPlayerPos(int index, const float position[3], bool findGround);
    bool SetPlayerVitals(int index, float value, bool armour);

private:
    CNetGame(const CNetGame&);
    CNetGame& operator=(const CNetGame&);

    static bool HandleQuery(SOCKET socket, const char* data, int length, const sockaddr_in& from);
    static void ClientJoin(RPCParameters* parameters);
    static void RequestClass(RPCParameters* parameters);
    static void RequestSpawn(RPCParameters* parameters);
    static void Spawn(RPCParameters* parameters);
    static void Death(RPCParameters* parameters);
    CPlayerPool::Slot* GetRpcPlayer(RPCParameters* parameters, int& index);
    void InitGameForPlayer(PlayerID address, unsigned short index);
    void Packet_PlayerSync(const Packet& packet);
    void WorldPlayerAdd(unsigned short index, const CPlayerPool::Slot& player, PlayerID recipient);
    void WorldPlayerRemove(unsigned short index, PlayerID recipient);
    void UpdatePlayerStreaming(int index, CPlayerPool::Slot& player, bool refresh);
    void ProcessDisconnect(PlayerID address, unsigned char reason);

    struct QueryPacket
    {
        SOCKET socket;
        sockaddr_in from;
        int length;
        char data[MAXIMUM_MTU_SIZE];
    };

    RakServerInterface* m_rak;
    enum { MaxSpawnClasses = 319 };
    PlayerSpawnInfo m_spawnClasses[MaxSpawnClasses];
    int m_spawnClassCount;
    CPlayerPool m_players;
    std::bitset<CPlayerPool::MaxPlayers> m_streamed[CPlayerPool::MaxPlayers];
    SimpleMutex m_queryMutex;
    std::deque<QueryPacket> m_queries;
};

extern CNetGame* pNetGame;

#endif
