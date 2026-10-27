#include "net/textdrawpool.h"
#include "gui/device_lifecycle.h"
#include "gui/textdraw_texture.h"
// samp.dll RVA: 0x000177d0
CTextDraw *CTextDrawPool::Get(WORD id) {
    if(id>=2304||!active[id])return 0;
    return draws[id];
}
// samp.dll RVA: 0x0001e260
CTextDrawPool::CTextDrawPool() {
    for(int i=0;i<2304;++i){draws[i]=0;active[i]=0;}
    GameInitializeTextDrawTextures();
}
// samp.dll RVA: 0x0001e2b0
void CTextDrawPool::Delete(WORD id) {
    CTextDraw *draw=draws[id];
    if(!draw)return;
    delete draw;
    draws[id]=0;active[id]=0;
}
// samp.dll RVA: 0x0001e390
CTextDrawPool::~CTextDrawPool() {
    for(int i=0;i<2304;++i) {
        CTextDraw *draw=draws[i];if(!draw)continue;
        delete draw;
        draws[i]=0;active[i]=0;
    }
}
// samp.dll RVA: 0x0001e3d0
CTextDraw *CTextDrawPool::Create(WORD id,const TextDrawTransmit *info,const char *text) {
    if(draws[id])Delete(id);
    CTextDraw *draw=new CTextDraw(info,text);
    if(!draw)return 0;
    draws[id]=draw;active[id]=1;return draw;
}
// samp.dll RVA: 0x0001e2f0
void CTextDrawPool::Draw() {
    if(g_deviceScoreboard && *(DWORD*)g_deviceScoreboard)return;
    for(int i=0;i<2304;i++)if(active[i])draws[i]->Draw();
}
// samp.dll RVA: 0x0001e330
void CTextDrawPool::Native01E330() {
    for(int i=0;i<2304;i++)if(active[i])draws[i]->Native0B30A0();
}
// samp.dll RVA: 0x0001e360
void CTextDrawPool::OnLostDevice() {
    for(int i=0;i<2304;i++)if(active[i])draws[i]->OnLostDevice();
}
