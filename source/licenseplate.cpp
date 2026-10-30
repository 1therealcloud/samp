#include "licenseplate.h"
#include "archive/provider.h"
#include "chatwindow.h"
#include "d3dhook/ID3DXFontHook.h"
#include "game/entity.h"
#include "game/textdraw.h"
#include "gui.h"
#include "main.h"
#include <new>
#include <string.h>
// 0x0006d420
CLicensePlate::~CLicensePlate()
{
    OnLostDevice();
}

// 0x0006d1f0
void CLicensePlate::OnLostDevice()
{
    if (surface)
    {
        surface->Release();
        surface = 0;
    }
    if (texture)
    {
        texture->Release();
        texture = 0;
    }
    if (renderer)
    {
        renderer->Release();
        renderer = 0;
    }
    if (defaultTexture)
    {
        defaultTexture->Release();
        defaultTexture = 0;
    }
}

// 0x0006d240
void CLicensePlate::OnResetDevice()
{
    device->GetDisplayMode(0, &mode);
    if (SUCCEEDED(D3DXCreateTexture(device, 128, 32, 1, D3DUSAGE_RENDERTARGET, mode.Format, D3DPOOL_DEFAULT, &texture)))
    {
        texture->GetSurfaceLevel(0, &surface);
        D3DSURFACE_DESC desc;
        surface->GetDesc(&desc);
        D3DXCreateRenderToSurface(device, desc.Width, desc.Height, desc.Format, TRUE, D3DFMT_D16, &renderer);
    }
}

// 0x0006d3f0
CLicensePlate::CLicensePlate(IDirect3DDevice9* value)
{
    defaultTexture = 0;
    surface = 0;
    texture = 0;
    renderer = 0;
    device = value;
    OnResetDevice();
}

// 0x0006d2c0
IDirect3DTexture9* CLicensePlate::Create(const char* text)
{
    if (!device || !g_deviceFonts || !renderer || !texture || !surface)
        return 0;
    IDirect3DTexture9* created;
    if (FAILED(D3DXCreateTexture(device, 64, 32, 1, 0, mode.Format, D3DPOOL_DEFAULT, &created)))
        return 0;
    renderer->BeginScene(surface, 0);
    device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffbeb6a8, 1.0f, 0);
    RECT rect = {0, 3, 128, 32};
    g_deviceFonts->DrawLicensePlateText(text, rect, 0xee444470);
    renderer->EndScene(0);
    created->SetAutoGenFilterType(D3DTEXF_LINEAR);
    IDirect3DSurface9* copy;
    created->GetSurfaceLevel(0, &copy);
    D3DXLoadSurfaceFromSurface(copy, 0, 0, surface, 0, 0, 3, 0);
    copy->Release();
    return created;
}
