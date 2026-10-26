#include "gui/device_objects.h"
#include "game/entity.h"
#include "archive/provider.h"
#include "main.h"
#include <new>
#include <string.h>
#include "gui/textdraw_texture.h"
// samp.dll RVA: 0x0011c74c
const char g_spawnVersionText[]="SA-MP 0.3.DL-R1";
// samp.dll RVA: 0x000705c0
SpawnScreenVertex::SpawnScreenVertex() {}
// samp.dll RVA: 0x000705d0
SpawnScreenVertex::SpawnScreenVertex(const CVector &position,float w,DWORD color,float tu,float tv) {
    memcpy(&x,&position,12);
    memcpy(&rhw,&w,4);this->color=color;
    memcpy(&u,&tu,4);memcpy(&v,&tv,4);
}
// samp.dll RVA: 0x00070340
CObjectMaterialText::~CObjectMaterialText() {OnLostDevice();}
// samp.dll RVA: 0x0006d420
CLicensePlate::~CLicensePlate() {OnLostDevice();}
// samp.dll RVA: 0x000709d0
CSpawnScreen::~CSpawnScreen() {
    char *text=*(char**)(unknown00+4);if(text)::operator delete(text);
    if(texture||states[1]||states[0])OnLostDevice();
    CFonts *ownedFonts=fonts;if(ownedFonts!=g_deviceFonts&&ownedFonts){delete ownedFonts;fonts=0;}
}
// samp.dll RVA: 0x00070a30
void CSpawnScreen::Draw() {
    if(!fonts||!*(char**)(unknown00+4))return;
    FontSize measured;fonts->GetTextScreenSize(&measured,*(char**)(unknown00+4),0);
    // GTA SA virtual address: screen height 0x00C17048. Integer arithmetic wraps.
    LONG bottom=(LONG)(*(DWORD*)0xc17048-30),top=(LONG)((DWORD)bottom-(DWORD)measured.cy);
    RECT textRect={20,top,(LONG)(20u+(DWORD)measured.cx),bottom};
    fonts->DrawText(0,*(char**)(unknown00+4),textRect,0xffffffff,TRUE);
    int imageTop=(int)((DWORD)top-103),imageBottom=(int)((DWORD)imageTop+73);
    if(texture) {
        SpawnScreenVertex vertices[4];float topY,bottomY;
        __asm {
            fild imageTop
            fsub g_textDrawHalf
            fild imageBottom
            fadd g_textDrawHalf
            fstp bottomY
            fstp topY
        }
        for(int i=0;i<4;++i){vertices[i].z=0.1f;vertices[i].rhw=1.0f;vertices[i].color=0xffffffff;}
        vertices[0].x=19.5f;vertices[0].y=bottomY;vertices[0].u=0;vertices[0].v=1;
        vertices[1].x=19.5f;vertices[1].y=topY;vertices[1].u=0;vertices[1].v=0;
        vertices[2].x=205.5f;vertices[2].y=bottomY;vertices[2].u=1;vertices[2].v=1;
        vertices[3].x=205.5f;vertices[3].y=topY;vertices[3].u=1;vertices[3].v=0;
        states[0]->Capture();states[1]->Apply();
        device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
        device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
        device->SetFVF(0x144);device->SetPixelShader(0);
        device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,vertices,28);states[0]->Apply();
    }
    FontSize versionSize;FontSize *size=fonts->GetTextScreenSize(&versionSize,g_spawnVersionText,0);
    LONG left=(LONG)(20u+(DWORD)((int)(185u-(DWORD)size->cx)/2));
    RECT versionRect={left,imageBottom,(LONG)((DWORD)left+(DWORD)size->cx),(LONG)((DWORD)imageBottom+(DWORD)size->cy)};
    fonts->DrawText(0,g_spawnVersionText,versionRect,0xff27abfa,TRUE);
}
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
