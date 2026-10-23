#ifndef SAMP_DEVICE_OBJECTS_H
#define SAMP_DEVICE_OBJECTS_H
#include <d3dx9.h>
#include "fontrender.h"
struct CVector;
class CDXUTDialog;
#pragma pack(push,1)
// Observed allocation extents and constructor/lifecycle fields; full render interfaces remain open.
#include "playertags.h"
#include "label.h"
class CObjectMaterialText {
public:
    // samp.dll RVA: 0x00070350
    IDirect3DTexture9 *Create(const char *text,const char *font,unsigned fontSize,int width,int height,D3DCOLOR color,D3DCOLOR background,BOOL bold,unsigned alignment);
    // samp.dll RVA: 0x00070320
    CObjectMaterialText(IDirect3DDevice9 *device);
    IDirect3DDevice9 *device;ID3DXSprite *first,*second;
    // samp.dll RVA: 0x000702d0
    void OnLostDevice();
    // samp.dll RVA: 0x00070300
    void OnResetDevice();
};
class CLicensePlate {
public:
    // samp.dll RVA: 0x0006d3f0
    CLicensePlate(IDirect3DDevice9 *device);
    IDirect3DDevice9 *device;ID3DXRenderToSurface *renderer;IDirect3DTexture9 *texture;
    IDirect3DSurface9 *surface;D3DDISPLAYMODE mode;IDirect3DTexture9 *defaultTexture;
    // samp.dll RVA: 0x0006d1f0
    void OnLostDevice();
    // samp.dll RVA: 0x0006d240
    void OnResetDevice();
    // samp.dll RVA: 0x0006d2c0
    IDirect3DTexture9 *Create(const char *text);
};
class CSpawnScreen {
public:
    // samp.dll RVA: 0x00070990
    CSpawnScreen(IDirect3DDevice9 *device);
    BYTE unknown00[8];CFonts *fonts;IDirect3DDevice9 *device;IDirect3DTexture9 *texture;
    IDirect3DStateBlock9 *states[2];DWORD unknown1c;
    // samp.dll RVA: 0x00070940
    void OnLostDevice();
    // samp.dll RVA: 0x00070690
    void OnResetDevice();
};
// Address interfaces for later UI/pool layers; never allocate these partial types.
#include "scoreboard.h"
#include "net/objectpool.h"
#include "net/textdrawpool.h"
#pragma pack(pop)
// samp.dll RVA: 0x002aca2c
extern CFonts *g_deviceFonts;
#endif
