#include <string.h>

#include "game/keystuff.h"
// 0x00152ac0
KeyPad* g_keysInternal = (KeyPad*)0xb73458;
// 0x00152ac4
BYTE* g_keysDriveLeft = (BYTE*)0xb6f1a4;
// 0x00152ac8
BYTE* g_keysDriveRight = (BYTE*)0xb6f1a5;
// 0x00190660
KeyPad g_keysLocal;
// 0x00190798
KeyPad g_keysPlayers[210];
// 0x001a0440
BYTE g_keysSavedDriveLeft;
// 0x001a0441
BYTE g_keysSavedDriveRight;
// 0x000a6fe0
void KeyStuff::Initialize()
{
    memset(&g_keysLocal, 0, sizeof(g_keysLocal));
    memset(g_keysPlayers, 0, sizeof(g_keysPlayers));
}
// 0x000a7000
void KeyStuff::UpdateKeys()
{
    memcpy(&g_keysLocal, g_keysInternal, sizeof(KeyPad));
    g_keysSavedDriveLeft = *g_keysDriveLeft;
    g_keysSavedDriveRight = *g_keysDriveRight;
}
// 0x000a7040
void KeyStuff::ApplyLocalKeys()
{
    memcpy(g_keysInternal, &g_keysLocal, sizeof(KeyPad));
    *g_keysDriveLeft = g_keysSavedDriveLeft;
    *g_keysDriveRight = g_keysSavedDriveRight;
}
// 0x000a7080
void KeyStuff::SetKeys(int player, const KeyPad* keys)
{
    memcpy(&g_keysPlayers[player], keys, sizeof(KeyPad));
}
// 0x000a70a0
void KeyStuff::ApplyPlayerKeys(int player)
{
    memcpy(g_keysInternal, &g_keysPlayers[player], sizeof(KeyPad));
    *g_keysDriveLeft = g_keysPlayers[player].keys1[5] != 0;
    *g_keysDriveRight = g_keysPlayers[player].keys1[7] != 0;
}
// 0x000a70f0
KeyPad* KeyStuff::GetInternalKeys()
{
    return g_keysInternal;
}
// 0x000a7100
KeyPad* KeyStuff::GetLocalKeys()
{
    return &g_keysLocal;
}
// 0x000a7110
KeyPad* KeyStuff::GetPlayerKeys(int player)
{
    return &g_keysPlayers[player];
}
// 0x000a7120
void KeyStuff::ResetKeys(int player)
{
    memset(&g_keysPlayers[player], 0, sizeof(KeyPad));
}
// 0x000a7140
void KeyStuff::ResetInternalKeys()
{
    memset(g_keysInternal, 0, 96);
}
