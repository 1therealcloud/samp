#include "deathwindow.h"
#include <string.h>
// samp.dll RVA: 0x0011c330
const char g_deathAuxFontFace[9]="SAMPAUX3";
// samp.dll RVA: 0x0006a090
CDeathWindow::CDeathWindow(IDirect3DDevice9 *device){
    *(DWORD*)(nativeState+0x137)=0;*(DWORD*)(nativeState+0x13b)=0;
    *(DWORD*)(nativeState+0x13f)=0;*(DWORD*)(nativeState+0x143)=0;
    *(DWORD*)(nativeState+0x14b)=0;*(DWORD*)(nativeState+0x14f)=0;
    *(DWORD*)(nativeState+0x153)=0;*(DWORD*)nativeState=1;
    *(IDirect3DDevice9**)(nativeState+0x147)=device;
    ResetFonts();memset(nativeState+4,0,0x127);
}

// samp.dll RVA: 0x000695f0
void CDeathWindow::CreateAuxiliaryFonts() {
    IDirect3DDevice9 *device=*(IDirect3DDevice9**)(nativeState+0x147);
    D3DXCreateFontA(device,20,0,400,1,FALSE,2,0,0,0,g_deathAuxFontFace,
        (ID3DXFont**)(nativeState+0x14f));
    device=*(IDirect3DDevice9**)(nativeState+0x147);
    D3DXCreateFontA(device,22,0,400,1,FALSE,2,0,0,0,g_deathAuxFontFace,
        (ID3DXFont**)(nativeState+0x153));
    *(DWORD*)(nativeState+0x14b)=1;
}
