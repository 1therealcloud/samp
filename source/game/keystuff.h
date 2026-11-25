#ifndef KEYSTUFF_H
#define KEYSTUFF_H

#include <windows.h>

struct KeyPad
{
    WORD keys1[24], keys2[24];
    BYTE opaque[212];
};
static_assert(sizeof(KeyPad) == 308);

extern KeyPad* g_keysInternal;

extern BYTE* g_keysDriveLeft;

extern BYTE* g_keysDriveRight;

extern KeyPad g_keysLocal;

extern KeyPad g_keysPlayers[210];

extern BYTE g_keysSavedDriveLeft;

extern BYTE g_keysSavedDriveRight;
namespace KeyStuff
{

void Initialize();

void UpdateKeys();

void ApplyLocalKeys();

void SetKeys(int player, const KeyPad* keys);

void ApplyPlayerKeys(int player);

KeyPad* GetInternalKeys();

KeyPad* GetLocalKeys();

KeyPad* GetPlayerKeys(int player);

void ResetKeys(int player);

void ResetInternalKeys();
} // namespace KeyStuff

#endif // KEYSTUFF_H
