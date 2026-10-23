#ifndef SAMP_PLAYERINFO_H
#define SAMP_PLAYERINFO_H
#include <windows.h>
#include <string>
#include <stddef.h>
class CRemotePlayer;
#pragma pack(push,1)
class CPlayerInfo {public:
    int score;DWORD npc;CRemotePlayer *remote;DWORD ping;std::string name;
    // samp.dll RVA: 0x00014060
    CPlayerInfo(const char *name,DWORD npc);
    // samp.dll RVA: 0x00013e10
    ~CPlayerInfo();
};
#pragma pack(pop)
typedef char PlayerInfoStringExtent[(sizeof(std::string)==28)?1:-1];
typedef char PlayerInfoNameOffset[(offsetof(CPlayerInfo,name)==0x10)?1:-1];
typedef char PlayerInfoExtent[(sizeof(CPlayerInfo)==0x2c)?1:-1];
#endif
