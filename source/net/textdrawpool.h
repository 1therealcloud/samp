#ifndef TEXTDRAWPOOL_H
#define TEXTDRAWPOOL_H

#include "game/textdraw.h"

#include <windows.h>

#include <static_assert.h>

struct RPCParameters;
#pragma pack(push, 1)
class CTextDrawPool
{
    // 0x0001cf70
    friend void ScrTextDrawSetString(RPCParameters* parameters);
    DWORD active[2304];
    CTextDraw* draws[2304];

public:
    // R5 address to verify
    CTextDraw* Get(WORD id);
    // 0x0001e7a0
    CTextDrawPool();
    // 0x0001e8d0
    ~CTextDrawPool();
    // 0x0001e7f0
    void Delete(WORD id);
    // 0x0001e910
    CTextDraw* Create(WORD id, const TextDrawTransmit* info, const char* text);
    // 0x0001e830
    void Draw();
    // R5 address to verify
    void PrepareModelPreviews();
    // R5 address to verify
    void OnLostDevice();
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CTextDrawPool) == 0x4800);

#endif // TEXTDRAWPOOL_H
