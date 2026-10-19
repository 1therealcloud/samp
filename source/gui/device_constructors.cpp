#include "gui/device_objects.h"
// samp.dll RVA: 0x0006b5f0
CLabel::CLabel(IDirect3DDevice9 *value) { device=value;D3DXCreateSprite(device,&sprite); }
// samp.dll RVA: 0x0006c730
CPlayerTags::CPlayerTags(IDirect3DDevice9 *value) { device=value;state=0;sprite=0;D3DXCreateSprite(device,&sprite); }
// samp.dll RVA: 0x00070320
CObjectMaterialText::CObjectMaterialText(IDirect3DDevice9 *value) { device=value;first=0;second=0;OnResetDevice(); }
// samp.dll RVA: 0x0006d3f0
CLicensePlate::CLicensePlate(IDirect3DDevice9 *value) { defaultTexture=0;surface=0;texture=0;renderer=0;device=value;OnResetDevice(); }
// samp.dll RVA: 0x00070990
CSpawnScreen::CSpawnScreen(IDirect3DDevice9 *value) {
    device=value;*(DWORD*)(unknown00+4)=0;texture=0;states[1]=0;states[0]=0;
    if(device)fonts=g_deviceFonts;
    OnResetDevice();
}
