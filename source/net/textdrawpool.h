#ifndef SAMP_TEXTDRAW_POOL_H
#define SAMP_TEXTDRAW_POOL_H
#include "game/textdraw.h"
#include <windows.h>
struct RPCParameters;
#pragma pack(push, 1)
class CTextDrawPool
{
    // 0x0001ca30
    friend void ScrTextDrawSetString(RPCParameters* parameters);
    DWORD active[2304];
    CTextDraw* draws[2304];

public:
    // 0x000177d0
    CTextDraw* Get(WORD id);
    // 0x0001e260
    CTextDrawPool();
    // 0x0001e390
    ~CTextDrawPool();
    // 0x0001e2b0
    void Delete(WORD id);
    // 0x0001e3d0
    CTextDraw* Create(WORD id, const TextDrawTransmit* info, const char* text);
    // 0x0001e2f0
    void Draw();
    // 0x0001e330
    void Native01E330();
    // 0x0001e360
    void OnLostDevice();
};
#pragma pack(pop)
typedef char NativeTextDrawPoolExtent[(sizeof(CTextDrawPool) == 0x4800) ? 1 : -1];
#endif
