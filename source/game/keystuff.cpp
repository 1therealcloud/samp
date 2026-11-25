#include "game/keystuff.h"

#include <string.h>

KeyPad* g_keysInternal = (KeyPad*)0xb73458;

BYTE* g_keysDriveLeft = (BYTE*)0xb6f1a4;

BYTE* g_keysDriveRight = (BYTE*)0xb6f1a5;

KeyPad g_keysLocal;

KeyPad g_keysPlayers[210];

BYTE g_keysSavedDriveLeft;

BYTE g_keysSavedDriveRight;

void KeyStuff::Initialize()
{
    memset(&g_keysLocal, 0, sizeof(g_keysLocal));
    memset(g_keysPlayers, 0, sizeof(g_keysPlayers));
}

void KeyStuff::UpdateKeys()
{
    memcpy(&g_keysLocal, g_keysInternal, sizeof(KeyPad));
    g_keysSavedDriveLeft = *g_keysDriveLeft;
    g_keysSavedDriveRight = *g_keysDriveRight;
}

void KeyStuff::ApplyLocalKeys()
{
    memcpy(g_keysInternal, &g_keysLocal, sizeof(KeyPad));
    *g_keysDriveLeft = g_keysSavedDriveLeft;
    *g_keysDriveRight = g_keysSavedDriveRight;
}

void KeyStuff::SetKeys(int player, const KeyPad* keys)
{
    memcpy(&g_keysPlayers[player], keys, sizeof(KeyPad));
}

void KeyStuff::ApplyPlayerKeys(int player)
{
    memcpy(g_keysInternal, &g_keysPlayers[player], sizeof(KeyPad));
    *g_keysDriveLeft = g_keysPlayers[player].keys1[5] != 0;
    *g_keysDriveRight = g_keysPlayers[player].keys1[7] != 0;
}

KeyPad* KeyStuff::GetInternalKeys()
{
    return g_keysInternal;
}

KeyPad* KeyStuff::GetLocalKeys()
{
    return &g_keysLocal;
}

KeyPad* KeyStuff::GetPlayerKeys(int player)
{
    return &g_keysPlayers[player];
}

void KeyStuff::ResetKeys(int player)
{
    memset(&g_keysPlayers[player], 0, sizeof(KeyPad));
}

void KeyStuff::ResetInternalKeys()
{
    memset(g_keysInternal, 0, 96);
}
