#include "gui/device_objects.h"
#include "gui/font_wrapper.h"
#include "chatwindow.h"
#include "main.h"

// samp.dll RVA: 0x00070350
IDirect3DTexture9 *CObjectMaterialText::Create(const char *text,const char *face,unsigned fontSize,int width,int height,D3DCOLOR color,D3DCOLOR background,BOOL bold,unsigned alignment) {
    if(!device || !g_deviceFonts)return 0;
    D3DDISPLAYMODE display;
    device->GetDisplayMode(0,&display);
    IDirect3DTexture9 *created;
    D3DFORMAT format=background?D3DFMT_X8R8G8B8:D3DFMT_A8R8G8B8;
    if(FAILED(D3DXCreateTexture(device,width,height,1,D3DUSAGE_RENDERTARGET,format,D3DPOOL_DEFAULT,&created)))return 0;
    IDirect3DSurface9 *surface;
    created->GetSurfaceLevel(0,&surface);
    D3DSURFACE_DESC desc;
    surface->GetDesc(&desc);
    ID3DXRenderToSurface *renderer;
    if(FAILED(D3DXCreateRenderToSurface(device,desc.Width,desc.Height,desc.Format,TRUE,D3DFMT_D16,&renderer)))return 0;
    DWORD flags=0;
    if((BYTE)alignment==1)flags=DT_CENTER|DT_VCENTER;
    else if((BYTE)alignment==2)flags=DT_RIGHT;
    ID3DXFont *rawFont;
    if(FAILED(D3DXCreateFontA(device,fontSize,0,bold?700:400,1,FALSE,1,0,0,0,face,&rawFont))) {
        g_pChat->AddDebugMessage("SignText: Can't create font %s",face);
        return 0;
    }
    // Native leaks this eight-byte proxy; Release forwards to the underlying font.
    CFontWrapper *font=new CFontWrapper;
    font->font=rawFont;
    renderer->BeginScene(surface,0);
    device->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,background,0.0f,0);
    first->Begin(D3DXSPRITE_ALPHABLEND);
    if(!background)device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_BLENDFACTOR);
    RECT rect={0,0,width,height};
    font->DrawTextA(first,text,-1,&rect,flags|DT_NOCLIP,color);
    // Capture the current sprite before End; subsequent calls use captured resources.
    ID3DXSprite *sprite=first;
    sprite->End();renderer->EndScene(0);
    font->Release();
    if(surface){surface->Release();surface=0;}
    if(renderer)renderer->Release();
    return created;
}
