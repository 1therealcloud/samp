#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include <stddef.h>
#include <string>
#include <windows.h>

class CRemotePlayer;
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
#ifndef _DEBUG
    BYTE namePadding[4]; // Preserve the native record size.
#endif
    // 0x0001da20
    void SetName(const char* text);
    // 0x00014060
    CPlayerInfo(const char* name, DWORD npc);
    // 0x00013e10
    ~CPlayerInfo();
};
#ifdef _DEBUG
static_assert(sizeof(std::string) == 28);
#else
static_assert(sizeof(std::string) == 24);
#endif
static_assert(offsetof(CPlayerInfo, name) == 0x10);
static_assert(sizeof(CPlayerInfo) == 0x2c);

#endif // PLAYERINFO_H
