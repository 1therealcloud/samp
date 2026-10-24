#include "net/playerinfo.h"
#include <string.h>
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
