#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include <stddef.h>
#include <string>
#include <windows.h>

#include <static_assert.h>

class CRemotePlayer;
#pragma pack(push, 1)
class CPlayerInfo
{
public:
    // R5 address to verify
    int GetScore();
    // R5 address to verify
    DWORD GetPing();
    // Native VC7 string length is at info+24; no terminator scan.
    // R5 address to verify
    DWORD GetNameLength();
    // R5 address to verify
    const char* GetName();
    // R5 address to verify
    DWORD GetNpcFlag();
    // R5 address to verify
    void SetScore(int value);
    // R5 address to verify
    void SetPing(DWORD value);

    int score;
    DWORD npc;
    CRemotePlayer* remote;
    DWORD ping;
    std::string name;
    // R5 address to verify
    void SetName(const char* text);
    // 0x000141b0
    CPlayerInfo(const char* name, DWORD npc);
    // 0x00013f60
    ~CPlayerInfo();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(std::string) == 28);
STATIC_ASSERT(offsetof(CPlayerInfo, name) == 0x10);
STATIC_ASSERT(sizeof(CPlayerInfo) == 0x2c);

#endif // PLAYERINFO_H
