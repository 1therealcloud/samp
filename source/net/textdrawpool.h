#ifndef TEXTDRAWPOOL_H
#define TEXTDRAWPOOL_H

#include "game/textdraw.h"

#include <windows.h>

struct RPCParameters;
class CTextDrawPool
{
    friend void ScrTextDrawSetString(RPCParameters* parameters);
    DWORD active[2304];
    CTextDraw* draws[2304];

public:
    CTextDraw* Get(WORD id);

    CTextDrawPool();

    ~CTextDrawPool();

    void Delete(WORD id);

    CTextDraw* Create(WORD id, const TextDrawTransmit* info, const char* text);

    void Draw();

    void PrepareModelPreviews();

    void OnLostDevice();
};
static_assert(sizeof(CTextDrawPool) == 0x4800);

#endif // TEXTDRAWPOOL_H
