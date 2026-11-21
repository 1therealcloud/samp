#ifndef KEYSTUFF_H
#define KEYSTUFF_H

#include <windows.h>

#include <static_assert.h>

struct KeyPad
{
    WORD keys1[24], keys2[24];
    BYTE opaque[212];
};
STATIC_ASSERT(sizeof(KeyPad) == 308);
// 0x00114ae8
extern KeyPad* g_keysInternal;
// 0x00114aec
extern BYTE* g_keysDriveLeft;
// 0x00114af0
extern BYTE* g_keysDriveRight;
// 0x001527c8
extern KeyPad g_keysLocal;
// 0x00152900
extern KeyPad g_keysPlayers[210];
// 0x001625a8
extern BYTE g_keysSavedDriveLeft;
// 0x001625a9
extern BYTE g_keysSavedDriveRight;
namespace KeyStuff
{
// 0x000a72a0
void Initialize();
// 0x000a72c0
void UpdateKeys();
// 0x000a7300
void ApplyLocalKeys();
// 0x000a7340
void SetKeys(int player, const KeyPad* keys);
// 0x000a7360
void ApplyPlayerKeys(int player);
// 0x000a73b0
KeyPad* GetInternalKeys();
// 0x000a73c0
KeyPad* GetLocalKeys();
// 0x000a73d0
KeyPad* GetPlayerKeys(int player);
// 0x000a73e0
void ResetKeys(int player);
// 0x000a7400
void ResetInternalKeys();
} // namespace KeyStuff

#endif // KEYSTUFF_H
