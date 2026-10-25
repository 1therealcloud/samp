#include "gui/device_objects.h"
#include "archive/provider.h"
#include "main.h"
#include <new>
#include <string.h>
// samp.dll RVA: 0x00070630
void CSpawnScreen::SetText(const char *value) {
    char *&text=*(char**)(unknown00+4);
    if(text)::operator delete(text);text=0;
    if(value){text=(char*)::operator new(strlen(value)+1);strcpy(text,value);}
}
// samp.dll RVA: 0x002aca2c
CFonts *g_deviceFonts;
// samp.dll RVA: 0x000702d0
void CObjectMaterialText::OnLostDevice() {
    if(second){second->Release();second=0;}
    if(first){first->Release();first=0;}
}
// samp.dll RVA: 0x00070300
void CObjectMaterialText::OnResetDevice() {
    D3DXCreateSprite(device,&first);D3DXCreateSprite(device,&second);
}
// samp.dll RVA: 0x0006d1f0
void CLicensePlate::OnLostDevice() {
    if(surface){surface->Release();surface=0;}
    if(texture){texture->Release();texture=0;}
    if(renderer){renderer->Release();renderer=0;}
    if(defaultTexture){defaultTexture->Release();defaultTexture=0;}
}
// samp.dll RVA: 0x0006d240
void CLicensePlate::OnResetDevice() {
    device->GetDisplayMode(0,&mode);
    if(SUCCEEDED(D3DXCreateTexture(device,128,32,1,D3DUSAGE_RENDERTARGET,mode.Format,D3DPOOL_DEFAULT,&texture))) {
        texture->GetSurfaceLevel(0,&surface);
        D3DSURFACE_DESC desc;surface->GetDesc(&desc);
        D3DXCreateRenderToSurface(device,desc.Width,desc.Height,desc.Format,TRUE,D3DFMT_D16,&renderer);
    }
}
// samp.dll RVA: 0x00070940
void CSpawnScreen::OnLostDevice() {
    if(texture)texture->Release();
    if(states[0])states[0]->Release();
    if(states[1])states[1]->Release();
    texture=0;states[0]=0;states[1]=0;
    if(fonts&&fonts!=g_deviceFonts){fonts->OnLostDevice();fonts=0;}
}
// samp.dll RVA: 0x00070690
void CSpawnScreen::OnResetDevice() {
    if(device) {
        if(texture)texture->Release();
        DWORD index=g_pArchive->GetFileIndex(0xbaddea6b);
        if(index!=0xffffffff) {
            BYTE *data=g_pArchive->GetFileData(index);
            DWORD size=g_pArchive->GetFileSize(index);
            D3DXCreateTextureFromFileInMemory(device,data,size,&texture);
        }
        for(int i=0;i<2;i++) {
            device->BeginStateBlock();device->SetTexture(0,texture);
            device->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
            device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);device->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
            device->SetRenderState(D3DRS_ALPHAREF,8);device->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
            device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
            device->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);device->SetRenderState(D3DRS_STENCILENABLE,FALSE);
            device->SetRenderState(D3DRS_CLIPPING,TRUE);device->SetRenderState(D3DRS_CLIPPLANEENABLE,FALSE);
            device->SetRenderState(D3DRS_VERTEXBLEND,D3DVBF_DISABLE);device->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE,FALSE);
            device->SetRenderState(D3DRS_FOGENABLE,FALSE);device->SetRenderState(D3DRS_COLORWRITEENABLE,0xf);
            device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
            device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
            device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
            device->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,0);device->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
            device->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);device->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
            device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
            device->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);device->EndStateBlock(&states[i]);
        }
    }
    if(fonts&&fonts!=g_deviceFonts)fonts->OnResetDevice();
}

#include "gui/device_objects.h"

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
