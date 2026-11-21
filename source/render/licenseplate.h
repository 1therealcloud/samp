#ifndef LICENSEPLATE_H
#define LICENSEPLATE_H

#include "render/fontrender.h"

#include <d3dx9.h>
struct CVector;
#pragma pack(push, 1)
class CLicensePlate
{
public:
    // 0x0006d9b0
    CLicensePlate(IDirect3DDevice9* device);
    // 0x0006d9e0
    ~CLicensePlate();
    IDirect3DDevice9* device;
    ID3DXRenderToSurface* renderer;
    IDirect3DTexture9* texture;
    IDirect3DSurface9* surface;
    D3DDISPLAYMODE mode;
    IDirect3DTexture9* defaultTexture;
    // 0x0006d7b0
    void OnLostDevice();
    // 0x0006d800
    void OnResetDevice();
    // 0x0006d880
    IDirect3DTexture9* Create(const char* text);
};
#pragma pack(pop)

#endif // LICENSEPLATE_H
