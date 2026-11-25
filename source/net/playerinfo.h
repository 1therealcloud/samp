#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include <stddef.h>
#include <string>
#include <windows.h>

class CRemotePlayer;
class CPlayerInfo
{
public:

    int GetScore();

    DWORD GetPing();
    // Native VC7 string length is at info+24; no terminator scan.

    DWORD GetNameLength();

    const char* GetName();

    DWORD GetNpcFlag();

    void SetScore(int value);

    void SetPing(DWORD value);

    int score;
    DWORD npc;
    CRemotePlayer* remote;
    DWORD ping;
    std::string name;
#ifndef _DEBUG
    BYTE namePadding[4]; // Preserve the native record size.
#endif

    void SetName(const char* text);

    CPlayerInfo(const char* name, DWORD npc);

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
