#include "game/keystuff.h"

#include <string.h>
// 0x00114ae8
KeyPad* g_keysInternal = (KeyPad*)0xb73458;
// 0x00114aec
BYTE* g_keysDriveLeft = (BYTE*)0xb6f1a4;
// 0x00114af0
BYTE* g_keysDriveRight = (BYTE*)0xb6f1a5;
// 0x001527c8
KeyPad g_keysLocal;
// 0x00152900
KeyPad g_keysPlayers[210];
// 0x001625a8
BYTE g_keysSavedDriveLeft;
// 0x001625a9
BYTE g_keysSavedDriveRight;
// 0x000a72a0
void KeyStuff::Initialize()
{
    memset(&g_keysLocal, 0, sizeof(g_keysLocal));
    memset(g_keysPlayers, 0, sizeof(g_keysPlayers));
}

// 0x000a72c0
void KeyStuff::UpdateKeys()
{
    memcpy(&g_keysLocal, g_keysInternal, sizeof(KeyPad));
    g_keysSavedDriveLeft = *g_keysDriveLeft;
    g_keysSavedDriveRight = *g_keysDriveRight;
}

// 0x000a7300
void KeyStuff::ApplyLocalKeys()
{
    memcpy(g_keysInternal, &g_keysLocal, sizeof(KeyPad));
    *g_keysDriveLeft = g_keysSavedDriveLeft;
    *g_keysDriveRight = g_keysSavedDriveRight;
}

// 0x000a7340
void KeyStuff::SetKeys(int player, const KeyPad* keys)
{
    memcpy(&g_keysPlayers[player], keys, sizeof(KeyPad));
}

// 0x000a7360
void KeyStuff::ApplyPlayerKeys(int player)
{
    memcpy(g_keysInternal, &g_keysPlayers[player], sizeof(KeyPad));
    *g_keysDriveLeft = g_keysPlayers[player].keys1[5] != 0;
    *g_keysDriveRight = g_keysPlayers[player].keys1[7] != 0;
}

// 0x000a73b0
KeyPad* KeyStuff::GetInternalKeys()
{
    return g_keysInternal;
}

// 0x000a73c0
KeyPad* KeyStuff::GetLocalKeys()
{
    return &g_keysLocal;
}

// 0x000a73d0
KeyPad* KeyStuff::GetPlayerKeys(int player)
{
    return &g_keysPlayers[player];
}

// 0x000a73e0
void KeyStuff::ResetKeys(int player)
{
    memset(&g_keysPlayers[player], 0, sizeof(KeyPad));
}

// 0x000a7400
void KeyStuff::ResetInternalKeys()
{
    memset(g_keysInternal, 0, 96);
}
