#ifndef SAMP_RECOVERED_KEYSTUFF_H
#define SAMP_RECOVERED_KEYSTUFF_H
#include <windows.h>
struct KeyPad
{
    WORD keys1[24], keys2[24];
    BYTE opaque[212];
};
typedef char KeyPadExtent[(sizeof(KeyPad) == 308) ? 1 : -1];
// samp.dll RVA: 0x00152ac0
extern KeyPad* g_keysInternal;
// samp.dll RVA: 0x00152ac4
extern BYTE* g_keysDriveLeft;
// samp.dll RVA: 0x00152ac8
extern BYTE* g_keysDriveRight;
// samp.dll RVA: 0x00190660
extern KeyPad g_keysLocal;
// samp.dll RVA: 0x00190798
extern KeyPad g_keysPlayers[210];
// samp.dll RVA: 0x001a0440
extern BYTE g_keysSavedDriveLeft;
// samp.dll RVA: 0x001a0441
extern BYTE g_keysSavedDriveRight;
namespace KeyStuff
{
// samp.dll RVA: 0x000a6fe0
void Initialize();
// samp.dll RVA: 0x000a7000
void UpdateKeys();
// samp.dll RVA: 0x000a7040
void ApplyLocalKeys();
// samp.dll RVA: 0x000a7080
void SetKeys(int player, const KeyPad* keys);
// samp.dll RVA: 0x000a70a0
void ApplyPlayerKeys(int player);
// samp.dll RVA: 0x000a70f0
KeyPad* GetInternalKeys();
// samp.dll RVA: 0x000a7100
KeyPad* GetLocalKeys();
// samp.dll RVA: 0x000a7110
KeyPad* GetPlayerKeys(int player);
// samp.dll RVA: 0x000a7120
void ResetKeys(int player);
// samp.dll RVA: 0x000a7140
void ResetInternalKeys();
} // namespace KeyStuff
#endif
