#ifndef SAMP_TEXTDRAW_ADDRESS_INTERFACE_H
#define SAMP_TEXTDRAW_ADDRESS_INTERFACE_H
#include <windows.h>
// Address-only view; native allocation, constructor and full layout remain open.
// Never instantiate this partial class.
class CTextDraw {public:
    // samp.dll RVA: 0x000b3080
    void Draw();
    // Preview coordinator; downstream preview renderers remain open.
    // samp.dll RVA: 0x000b30a0
    void Native0B30A0();
    // samp.dll RVA: 0x000b32b0
    void OnLostDevice();
    // Recovered drawing backends; original method names remain unestablished.
    // samp.dll RVA: 0x000b2c90
    void Native0B2C90();
    // samp.dll RVA: 0x000b2da0
    void Native0B2DA0();
};
// Shared texture-slot release entry.
// samp.dll RVA: 0x000b2750
void GameReleaseTextDrawTexture(int slot);
#endif
