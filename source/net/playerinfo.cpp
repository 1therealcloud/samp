#include "net/playerinfo.h"
#include <string.h>
// 0x0006e270
int CPlayerInfo::GetScore()
{
    return score;
}
// 0x0006e280
DWORD CPlayerInfo::GetPing()
{
    return ping;
}
// 0x0006e300
DWORD CPlayerInfo::GetNameLength()
{
    return (DWORD)name.size();
}
// 0x000170c0
const char* CPlayerInfo::GetName()
{
    return name.c_str();
}
// 0x0001da20
void CPlayerInfo::SetName(const char* text)
{
    name.assign(text, strlen(text));
}
#include "net/remoteplayer.h"
#include <string.h>
// 0x00014060
CPlayerInfo::CPlayerInfo(const char* text, DWORD isNpc)
{
    npc = isNpc;
    ping = 0;
    score = 0;
    name.assign(text, strlen(text));
    remote = new CRemotePlayer;
}
// 0x00013e10
CPlayerInfo::~CPlayerInfo()
{
    if (remote)
    {
        delete remote;
        remote = 0;
    }
}

// 0x0000e2d0
DWORD CPlayerInfo::GetNpcFlag()
{
    return npc;
}
// 0x0000e2e0
void CPlayerInfo::SetScore(int value)
{
    score = value;
}
// 0x0000e2f0
void CPlayerInfo::SetPing(DWORD value)
{
    ping = value;
}
