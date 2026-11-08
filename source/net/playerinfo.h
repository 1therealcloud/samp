#ifndef SAMP_PLAYERINFO_H
#define SAMP_PLAYERINFO_H
#include <static_assert.h>
#include <stddef.h>
#include <string>
#include <windows.h>
class CRemotePlayer;
#pragma pack(push, 1)
class CPlayerInfo
{
public:
    // 0x0006e270
    int GetScore();
    // 0x0006e280
    DWORD GetPing();
    // Native VC7 string length is at info+24; no terminator scan.
    // 0x0006e300
    DWORD GetNameLength();
    // 0x000170c0
    const char* GetName();
    // 0x0000e2d0
    DWORD GetNpcFlag();
    // 0x0000e2e0
    void SetScore(int value);
    // 0x0000e2f0
    void SetPing(DWORD value);

    int score;
    DWORD npc;
    CRemotePlayer* remote;
    DWORD ping;
    std::string name;
    // 0x0001da20
    void SetName(const char* text);
    // 0x00014060
    CPlayerInfo(const char* name, DWORD npc);
    // 0x00013e10
    ~CPlayerInfo();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(std::string) == 28);
STATIC_ASSERT(offsetof(CPlayerInfo, name) == 0x10);
STATIC_ASSERT(sizeof(CPlayerInfo) == 0x2c);
#endif
