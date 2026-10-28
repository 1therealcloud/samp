#ifndef SAMP_RECOVERED_PLAYER_COLORS_H
#define SAMP_RECOVERED_PLAYER_COLORS_H
#include <windows.h>
// 0x00151b20
extern DWORD g_playerDefaultColors[1000];
// 0x0018f6c0
extern DWORD g_playerColors[1000];
namespace PlayerColors
{
// 0x000a6fc0
void Initialize();
// 0x000a6f50
void __stdcall Set(DWORD index, DWORD color);
// 0x000a6f70
DWORD __stdcall Get(DWORD index);
} // namespace PlayerColors
#endif
