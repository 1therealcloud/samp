#ifndef SAMP_DEATHWINDOW_H
#define SAMP_DEATHWINDOW_H
#include <d3dx9.h>
class CDeathWindow {public:
    // samp.dll RVA: 0x000695f0
    void CreateAuxiliaryFonts();
    // Observed allocation extent, not a complete recovered interface.
    BYTE nativeState[0x157];
    // samp.dll RVA: 0x0006a090
    CDeathWindow(IDirect3DDevice9 *device);
    // Font/resource reset dependency; body remains open.
    // samp.dll RVA: 0x00069b90
    void ResetFonts();
    // samp.dll RVA: 0x00069d20
    void Draw();
};
// samp.dll RVA: 0x0011c330
extern const char g_deathAuxFontFace[9];
#endif
