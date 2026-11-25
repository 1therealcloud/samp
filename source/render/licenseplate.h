#ifndef LICENSEPLATE_H
#define LICENSEPLATE_H

#include "render/fontrender.h"

#include <d3dx9.h>
struct CVector;
class CLicensePlate
{
public:

    CLicensePlate(IDirect3DDevice9* device);

    ~CLicensePlate();
    IDirect3DDevice9* device;
    ID3DXRenderToSurface* renderer;
    IDirect3DTexture9* texture;
    IDirect3DSurface9* surface;
    D3DDISPLAYMODE mode;
    IDirect3DTexture9* defaultTexture;

    void OnLostDevice();

    void OnResetDevice();

    IDirect3DTexture9* Create(const char* text);
};

static_assert(sizeof(CLicensePlate) == 36);

#endif // LICENSEPLATE_H
