#ifndef SAMP_LICENSEPLATE_H
#define SAMP_LICENSEPLATE_H
#include "fontrender.h"
#include <d3dx9.h>
struct CVector;
#pragma pack(push, 1)
class CLicensePlate
{
public:
    // 0x0006d3f0
    CLicensePlate(IDirect3DDevice9* device);
    // 0x0006d420
    ~CLicensePlate();
    IDirect3DDevice9* device;
    ID3DXRenderToSurface* renderer;
    IDirect3DTexture9* texture;
    IDirect3DSurface9* surface;
    D3DDISPLAYMODE mode;
    IDirect3DTexture9* defaultTexture;
    // 0x0006d1f0
    void OnLostDevice();
    // 0x0006d240
    void OnResetDevice();
    // 0x0006d2c0
    IDirect3DTexture9* Create(const char* text);
};
#pragma pack(pop)
#endif
