#include "gui/device_objects.h"
#include "archive/provider.h"
#include "client/bootstrap.h"
// samp.dll RVA: 0x002aca2c
CFonts *g_deviceFonts;
// samp.dll RVA: 0x0006d090
void CPlayerTags::OnLostDevice() {
    if(state){state->Release();state=0;}
    if(sprite)sprite->OnLostDevice();
}
// samp.dll RVA: 0x0006d0c0
void CPlayerTags::OnResetDevice() {
    if(sprite)sprite->OnResetDevice();
    if(!state)device->CreateStateBlock(D3DSBT_ALL,&state);
}
// samp.dll RVA: 0x0006b630
void CLabel::OnLostDevice() { if(sprite)sprite->OnLostDevice(); }
// samp.dll RVA: 0x0006b640
void CLabel::OnResetDevice() { if(sprite)sprite->OnResetDevice(); }
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
