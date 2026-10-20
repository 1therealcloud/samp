#ifndef SAMP_GAME_PAD_H
#define SAMP_GAME_PAD_H
#include <windows.h>
// Raw native argument words forwarded to GTA CPad::Clear.
// samp.dll RVA: 0x000b58e0
void GameClearLocalPad(DWORD first,DWORD second);
#endif
