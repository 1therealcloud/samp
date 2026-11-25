#include "net/playerinfo.h"

#include "net/remoteplayer.h"

#include <string.h>

int CPlayerInfo::GetScore()
{
    return score;
}

DWORD CPlayerInfo::GetPing()
{
    return ping;
}

DWORD CPlayerInfo::GetNameLength()
{
    return (DWORD)name.size();
}

const char* CPlayerInfo::GetName()
{
    return name.c_str();
}

void CPlayerInfo::SetName(const char* text)
{
    name.assign(text, strlen(text));
}

CPlayerInfo::CPlayerInfo(const char* text, DWORD isNpc)
{
    npc = isNpc;
    ping = 0;
    score = 0;
    name.assign(text, strlen(text));
    remote = new CRemotePlayer;
}

CPlayerInfo::~CPlayerInfo()
{
    if (remote)
    {
        delete remote;
        remote = 0;
    }
}

DWORD CPlayerInfo::GetNpcFlag()
{
    return npc;
}

void CPlayerInfo::SetScore(int value)
{
    score = value;
}

void CPlayerInfo::SetPing(DWORD value)
{
    ping = value;
}
