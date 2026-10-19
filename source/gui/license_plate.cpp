#include "gui/device_objects.h"
#include "fontrender.h"

// samp.dll RVA: 0x0006d2c0
IDirect3DTexture9 *CLicensePlate::Create(const char *text) {
    if(!device || !g_deviceFonts || !renderer || !texture || !surface)return 0;
    IDirect3DTexture9 *created;
    if(FAILED(D3DXCreateTexture(device,64,32,1,0,mode.Format,D3DPOOL_DEFAULT,&created)))return 0;
    renderer->BeginScene(surface,0);
    device->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,0xffbeb6a8,1.0f,0);
    RECT rect={0,3,128,32};g_deviceFonts->DrawLicensePlateText(text,rect,0xee444470);
    renderer->EndScene(0);
    created->SetAutoGenFilterType(D3DTEXF_LINEAR);
    IDirect3DSurface9 *copy;
    created->GetSurfaceLevel(0,&copy);
    D3DXLoadSurfaceFromSurface(copy,0,0,surface,0,0,3,0);
    copy->Release();return created;
}
