#include "net/textdrawpool.h"

#include "game/textdraw.h"
#include "gui/gui.h"
// R5 address to verify
CTextDraw* CTextDrawPool::Get(WORD id)
{
    if (id >= 2304)
        return 0;
    DWORD index = id;
    if (active[index] == 0)
        return 0;
    return draws[index];
}

// 0x0001e7a0
CTextDrawPool::CTextDrawPool()
{
    for (int i = 0; i != 2304; ++i)
    {
        draws[i] = 0;
        active[i] = 0;
    }
    GameInitializeTextDrawTextures();
}

// 0x0001e7f0
void CTextDrawPool::Delete(WORD id)
{
    CTextDraw* draw = draws[id];
    if (!draw)
        return;
    delete draw;
    draws[id] = 0;
    active[id] = 0;
}

// 0x0001e8d0
CTextDrawPool::~CTextDrawPool()
{
    for (int i = 0; i < 2304; ++i)
    {
        CTextDraw* draw = draws[i];
        if (!draw)
            continue;
        delete draw;
        draws[i] = 0;
        active[i] = 0;
    }
}

// 0x0001e910
CTextDraw* CTextDrawPool::Create(WORD id, const TextDrawTransmit* info, const char* text)
{
    DWORD index = id;
    if (draws[index])
        Delete(id);
    CTextDraw* draw = new CTextDraw(info, text);
    if (draw)
    {
        draws[index] = draw;
        active[index] = 1;
        return draw;
    }
    return 0;
}

// 0x0001e830
void CTextDrawPool::Draw()
{
    int i = 0;
    if (!g_deviceScoreboard || !*(DWORD*)g_deviceScoreboard)
    {
        do
        {
            if (active[i])
                draws[i]->Draw();
        }
        while (++i != 2304);
    }
}

// R5 address to verify
void CTextDrawPool::PrepareModelPreviews()
{
    for (int i = 0; i != 2304; i++)
        if (active[i])
            draws[i]->PrepareModelPreview();
}

// R5 address to verify
void CTextDrawPool::OnLostDevice()
{
    for (int i = 0; i != 2304; i++)
        if (active[i])
            draws[i]->OnLostDevice();
}
