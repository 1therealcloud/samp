#ifndef SAMP_DEVICE_OBJECTS_H
#define SAMP_DEVICE_OBJECTS_H
#include <d3dx9.h>
#include "fontrender.h"
#pragma pack(push,1)
// Observed lifecycle prefixes only; constructors/complete interfaces remain open.
class CPlayerTags {
public:
    IDirect3DDevice9 *device;IDirect3DStateBlock9 *state;ID3DXSprite *sprite;
    // samp.dll RVA: 0x0006d090
    void OnLostDevice();
    // samp.dll RVA: 0x0006d0c0
    void OnResetDevice();
};
class CLabel {
public:
    IDirect3DDevice9 *device;ID3DXSprite *sprite;
    // samp.dll RVA: 0x0006b630
    void OnLostDevice();
    // samp.dll RVA: 0x0006b640
    void OnResetDevice();
};
class CObjectMaterialText {
public:
    IDirect3DDevice9 *device;ID3DXSprite *first,*second;
    // samp.dll RVA: 0x000702d0
    void OnLostDevice();
    // samp.dll RVA: 0x00070300
    void OnResetDevice();
};
class CLicensePlate {
public:
    IDirect3DDevice9 *device;ID3DXRenderToSurface *renderer;IDirect3DTexture9 *texture;
    IDirect3DSurface9 *surface;D3DDISPLAYMODE mode;IDirect3DTexture9 *defaultTexture;
    // samp.dll RVA: 0x0006d1f0
    void OnLostDevice();
    // samp.dll RVA: 0x0006d240
    void OnResetDevice();
};
class CSpawnScreen {
public:
    BYTE unknown00[8];CFonts *fonts;IDirect3DDevice9 *device;IDirect3DTexture9 *texture;
    IDirect3DStateBlock9 *states[2];DWORD unknown1c;
    // samp.dll RVA: 0x00070940
    void OnLostDevice();
    // samp.dll RVA: 0x00070690
    void OnResetDevice();
};
// Address interfaces for later UI/pool layers; never allocate these partial types.
class CScoreboard {
public:
    // samp.dll RVA: 0x0006e410
    void Close(bool restoreCursor);
    // samp.dll RVA: 0x0006e360
    void Recalc();
};
class CObjectPool {
public:
    // samp.dll RVA: 0x00012a50
    void ShutdownMaterials();
};
class CTextDrawPool {
public:
    // samp.dll RVA: 0x0001e360
    void OnLostDevice();
};
#pragma pack(pop)
// samp.dll RVA: 0x002aca2c
extern CFonts *g_deviceFonts;
#endif
