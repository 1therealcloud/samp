#include "net/playerinfo.h"
#include <string.h>
// samp.dll RVA: 0x0006e270
int CPlayerInfo::GetScore() {return score;}
// samp.dll RVA: 0x0006e280
DWORD CPlayerInfo::GetPing() {return ping;}
// samp.dll RVA: 0x0006e300
DWORD CPlayerInfo::GetNameLength() {return (DWORD)name.size();}
// samp.dll RVA: 0x000170c0
const char *CPlayerInfo::GetName() {return name.c_str();}
// samp.dll RVA: 0x0001da20
void CPlayerInfo::SetName(const char *text){name.assign(text,strlen(text));}
#include "net/remoteplayer.h"
#include <string.h>
// samp.dll RVA: 0x00014060
CPlayerInfo::CPlayerInfo(const char *text,DWORD isNpc) {
    npc=isNpc;ping=0;score=0;
    name.assign(text,strlen(text));remote=new CRemotePlayer;
}
// samp.dll RVA: 0x00013e10
CPlayerInfo::~CPlayerInfo() {if(remote){delete remote;remote=0;}}

// samp.dll RVA: 0x0000e2d0
DWORD CPlayerInfo::GetNpcFlag() {
    return npc;
}
// samp.dll RVA: 0x0000e2e0
void CPlayerInfo::SetScore(int value) {
    score=value;
}
// samp.dll RVA: 0x0000e2f0
void CPlayerInfo::SetPing(DWORD value) {
    ping=value;
}
