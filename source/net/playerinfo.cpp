#include "net/playerinfo.h"

#include "net/remoteplayer.h"

#include <string.h>
// R5 address to verify
int CPlayerInfo::GetScore()
{
    return score;
}

// R5 address to verify
DWORD CPlayerInfo::GetPing()
{
    return ping;
}

// R5 address to verify
DWORD CPlayerInfo::GetNameLength()
{
    return (DWORD)name.size();
}

// R5 address to verify
const char* CPlayerInfo::GetName()
{
    return name.c_str();
}

// R5 address to verify
void CPlayerInfo::SetName(const char* text)
{
    name.assign(text, strlen(text));
}

// 0x000141b0
CPlayerInfo::CPlayerInfo(const char* text, DWORD isNpc)
{
    npc = isNpc;
    ping = 0;
    score = 0;
    name.assign(text, strlen(text));
    remote = new CRemotePlayer;
}

// 0x00013f60
CPlayerInfo::~CPlayerInfo()
{
    if (remote)
    {
        delete remote;
        remote = 0;
    }
}

// R5 address to verify
DWORD CPlayerInfo::GetNpcFlag()
{
    return npc;
}

// R5 address to verify
void CPlayerInfo::SetScore(int value)
{
    score = value;
}

// R5 address to verify
void CPlayerInfo::SetPing(DWORD value)
{
    ping = value;
}
