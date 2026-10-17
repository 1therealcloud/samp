// SA-MP font orchestration; COM/D3DX implementations remain external.
#include "fontrender.h"
#include "gui/chat_dependencies.h"
#include <string.h>

// samp.dll RVA: 0x0006b530
CFonts::CFonts(IDirect3DDevice9 *device) {
    m_pDevice=device;
    m_pFont=0; m_pLittleFont=0; m_pShadow=0; m_pLittleShadow=0;
    m_pDefaultSprite=0; m_pLicensePlateFont=0;
    Reset();
}

#define RELEASE_FONT(p) if(p) { p->Release(); p=0; }
// samp.dll RVA: 0x0006ab40
CFonts::~CFonts() {
    RELEASE_FONT(m_pFont)
    RELEASE_FONT(m_pLittleFont)
    RELEASE_FONT(m_pShadow)
    RELEASE_FONT(m_pLittleShadow)
    RELEASE_FONT(m_pDefaultSprite)
    RELEASE_FONT(m_pLicensePlateFont)
}
// samp.dll RVA: 0x0006abc0
void CFonts::OnLostDevice() {
    m_pFont->OnLostDevice(); m_pLittleFont->OnLostDevice();
    m_pShadow->OnLostDevice(); m_pLittleShadow->OnLostDevice();
    m_pDefaultSprite->OnLostDevice(); m_pLicensePlateFont->OnLostDevice();
}
// samp.dll RVA: 0x0006ac00
void CFonts::OnResetDevice() {
    m_pFont->OnResetDevice(); m_pLittleFont->OnResetDevice();
    m_pShadow->OnResetDevice(); m_pLittleShadow->OnResetDevice();
    m_pDefaultSprite->OnResetDevice(); m_pLicensePlateFont->OnResetDevice();
}
// samp.dll RVA: 0x0006ac40
FontSize *CFonts::GetTextScreenSize(FontSize *output,const char *text,int format) {
    FontSize size;
    if(strlen(text)>100000) { *output=size;return output; }
    strcpy(m_szTempBuffer,text);
    ChatStripColorTags(m_szTempBuffer);
    RECT rect;
    m_pFont->DrawTextA(0,m_szTempBuffer,-1,&rect,format|DT_CALCRECT,0xff000000);
    size.cx=rect.right-rect.left; size.cy=rect.bottom-rect.top;
    *output=size;return output;
}
// samp.dll RVA: 0x0006acf0
FontSize *CFonts::GetLittleTextScreenSize(FontSize *output,const char *text,int format) {
    FontSize size;
    if(strlen(text)>100000) { *output=size;return output; }
    strcpy(m_szTempBuffer,text);
    ChatStripColorTags(m_szTempBuffer);
    RECT rect;
    m_pLittleFont->DrawTextA(0,m_szTempBuffer,-1,&rect,format|DT_CALCRECT,0xff000000);
    size.cx=rect.right-rect.left; size.cy=rect.bottom-rect.top;
    *output=size;return output;
}
// samp.dll RVA: 0x0006ada0
void CFonts::DrawText(ID3DXSprite *sprite,const char *text,RECT rect,D3DCOLOR color,BOOL shadow) {
    if(!text || !*text) return;
    BOOL ownSprite=FALSE;
    if(!sprite) { sprite=m_pDefaultSprite; ownSprite=TRUE; sprite->Begin(D3DXSPRITE_ALPHABLEND); }
    if(shadow) {
        if(strlen(text)>100000) return;
        strcpy(m_szTempBuffer,text); ChatStripColorTags(m_szTempBuffer);
        int length=(int)strlen(m_szTempBuffer);
        --rect.top;
        m_pShadow->DrawTextA(sprite,m_szTempBuffer,length,&rect,DT_NOCLIP,0xff000000);
        rect.top+=2;
        m_pShadow->DrawTextA(sprite,m_szTempBuffer,length,&rect,DT_NOCLIP,0xff000000);
        --rect.top; --rect.left;
        m_pShadow->DrawTextA(sprite,m_szTempBuffer,length,&rect,DT_NOCLIP,0xff000000);
        rect.left+=2;
        m_pShadow->DrawTextA(sprite,m_szTempBuffer,length,&rect,DT_NOCLIP,0xff000000);
        --rect.left;
    }
    m_pFont->DrawTextA(sprite,text,-1,&rect,DT_NOCLIP,color);
    if(ownSprite) sprite->End();
}
// samp.dll RVA: 0x0006af20
void CFonts::DrawLittleText(ID3DXSprite *sprite,const char *text,RECT rect,int format,D3DCOLOR color,BOOL shadow) {
    if(!text || !*text) return;
    BOOL ownSprite=FALSE;
    if(!sprite) { sprite=m_pDefaultSprite; ownSprite=TRUE; sprite->Begin(D3DXSPRITE_ALPHABLEND); }
    if(shadow) {
        if(strlen(text)>100000) return;
        strcpy(m_szTempBuffer,text); ChatStripColorTags(m_szTempBuffer);
        int length=(int)strlen(m_szTempBuffer);
        --rect.top;
        m_pLittleShadow->DrawTextA(sprite,m_szTempBuffer,length,&rect,format,0xff000000);
        rect.top+=2;
        m_pLittleShadow->DrawTextA(sprite,m_szTempBuffer,length,&rect,format,0xff000000);
        --rect.top; --rect.left;
        m_pLittleShadow->DrawTextA(sprite,m_szTempBuffer,length,&rect,format,0xff000000);
        rect.left+=2;
        m_pLittleShadow->DrawTextA(sprite,m_szTempBuffer,length,&rect,format,0xff000000);
        --rect.left;
    }
    m_pLittleFont->DrawTextA(sprite,text,-1,&rect,format,color);
    if(ownSprite) sprite->End();
}
// samp.dll RVA: 0x0006b090
void CFonts::DrawLicensePlateText(const char *text,RECT rect,D3DCOLOR color) {
    if(!text || !*text) return;
    ID3DXSprite *sprite=m_pDefaultSprite;
    sprite->Begin(D3DXSPRITE_ALPHABLEND);
    if(strlen(text)>100000) return;
    strcpy(m_szTempBuffer,text); ChatStripColorTags(m_szTempBuffer);
    int length=(int)strlen(m_szTempBuffer);
    rect.top-=2;
    m_pLicensePlateFont->DrawTextA(sprite,m_szTempBuffer,length,&rect,DT_NOCLIP|DT_CENTER|DT_VCENTER,0xb4f0eee4);
    rect.top+=2;rect.left+=2;
    m_pLicensePlateFont->DrawTextA(sprite,m_szTempBuffer,length,&rect,DT_NOCLIP|DT_CENTER|DT_VCENTER,0xb4f0eee4);
    rect.left-=2;rect.top+=2;
    m_pLicensePlateFont->DrawTextA(sprite,m_szTempBuffer,length,&rect,DT_NOCLIP|DT_CENTER|DT_VCENTER,0xb4f0eee4);
    rect.top-=2;rect.left-=2;
    m_pLicensePlateFont->DrawTextA(sprite,m_szTempBuffer,length,&rect,DT_NOCLIP|DT_CENTER|DT_VCENTER,0xb4f0eee4);
    rect.left+=2;
    m_pLicensePlateFont->DrawTextA(sprite,text,-1,&rect,DT_NOCLIP|DT_CENTER|DT_VCENTER,color);
    sprite->End();
}
