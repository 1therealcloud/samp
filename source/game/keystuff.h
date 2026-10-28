#ifndef SAMP_RECOVERED_KEYSTUFF_H
#define SAMP_RECOVERED_KEYSTUFF_H
#include <windows.h>
struct KeyPad
{
    WORD keys1[24], keys2[24];
    BYTE opaque[212];
};
typedef char KeyPadExtent[(sizeof(KeyPad) == 308) ? 1 : -1];
// 0x00152ac0
extern KeyPad* g_keysInternal;
// 0x00152ac4
extern BYTE* g_keysDriveLeft;
// 0x00152ac8
extern BYTE* g_keysDriveRight;
// 0x00190660
extern KeyPad g_keysLocal;
// 0x00190798
extern KeyPad g_keysPlayers[210];
// 0x001a0440
extern BYTE g_keysSavedDriveLeft;
// 0x001a0441
extern BYTE g_keysSavedDriveRight;
namespace KeyStuff
{
// 0x000a6fe0
void Initialize();
// 0x000a7000
void UpdateKeys();
// 0x000a7040
void ApplyLocalKeys();
// 0x000a7080
void SetKeys(int player, const KeyPad* keys);
// 0x000a70a0
void ApplyPlayerKeys(int player);
// 0x000a70f0
KeyPad* GetInternalKeys();
// 0x000a7100
KeyPad* GetLocalKeys();
// 0x000a7110
KeyPad* GetPlayerKeys(int player);
// 0x000a7120
void ResetKeys(int player);
// 0x000a7140
void ResetInternalKeys();
} // namespace KeyStuff
#endif
