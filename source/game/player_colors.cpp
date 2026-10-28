#include "game/player_colors.h"
#include <string.h>
// samp.dll RVA: 0x00151b20
DWORD g_playerDefaultColors[1000] = {
#include "game/player_color_values.inc"
};
// samp.dll RVA: 0x0018f6c0
DWORD g_playerColors[1000];
// samp.dll RVA: 0x000a6fc0
void PlayerColors::Initialize()
{
    memcpy(g_playerColors, g_playerDefaultColors, sizeof(g_playerColors));
}
// samp.dll RVA: 0x000a6f50
void __stdcall PlayerColors::Set(DWORD index, DWORD color)
{
    if (index < sizeof(g_playerColors))
        g_playerColors[index] = color;
}
// samp.dll RVA: 0x000a6f70
DWORD __stdcall PlayerColors::Get(DWORD index)
{
    if (index == 1004)
        return 0x89a4c440;
    if (index == 1005)
        return 0xaa0000ff;
    if (index == 1006)
        return 0xe2c063ff;
    if (index < sizeof(g_playerColors))
        return g_playerColors[index];
    return index;
}
