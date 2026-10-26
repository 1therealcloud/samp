#include "net/menupool.h"
// samp.dll RVA: 0x000177b0
DWORD CMenuPool::GetActiveState(BYTE id) {
    if(id>128)return 0;
    return *(DWORD*)((BYTE*)this+0x200+id*4);
}
#include <stdlib.h>
// samp.dll RVA: 0x00007b40
CMenuPool::CMenuPool(){for(int i=0;i<128;++i){active[i]=0;menus[i]=0;}currentMenu=128;}
// samp.dll RVA: 0x00007b90
CMenu *CMenuPool::Create(BYTE id,const char *title,float x,float y,BYTE columns,float width1,float width2,const void *interaction){
    if(menus[id]){free(menus[id]);menus[id]=0;}
    active[id]=0;
    CMenu *menu=new CMenu(title,x,y,columns,width1,width2,interaction);
    if(!menu)return 0;
    active[id]=1;menus[id]=menu;return menu;
}
// samp.dll RVA: 0x00007c60
BOOL CMenuPool::Delete(BYTE id){if(!active[id]||!menus[id])return FALSE;active[id]=0;free(menus[id]);menus[id]=0;return TRUE;}
// samp.dll RVA: 0x00007cb0
void CMenuPool::Show(BYTE id){
    if(id>=128||!active[id]||!menus[id])return;
    if(currentMenu!=128)menus[currentMenu]->Hide();
    menus[id]->Show();currentMenu=id;unknown401=0;
}
// samp.dll RVA: 0x00007d10
void CMenuPool::Hide(BYTE id){
    if(id>=128||currentMenu==128||!active[id]||!menus[id])return;
    menus[id]->Hide();currentMenu=128;
}
// samp.dll RVA: 0x00007eb0
CMenuPool::~CMenuPool(){
    if(currentMenu!=128)menus[currentMenu]->Hide();
    for(int i=0;i<128;++i)if(menus[i]){free(menus[i]);menus[i]=0;}
}

#include <string.h>
// samp.dll RVA: 0x001179bc
const char g_menuPoolTitleSuffix[4]="HED";
// samp.dll RVA: 0x001179b8
const char g_menuPoolColumn0Suffix[4]="RW1";
// samp.dll RVA: 0x001179b4
const char g_menuPoolColumn1Suffix[4]="RW2";
// samp.dll RVA: 0x00007d50
char *CMenuPool::GetTextPointer(const char *suffix){
    if(currentMenu==128)return 0;CMenu *menu=menus[currentMenu];if(!menu)return 0;
    if(!memcmp(suffix,g_menuPoolTitleSuffix,4))return menu->GetTitle();
    if(!memcmp(suffix,g_menuPoolColumn0Suffix,4))return menu->GetColumnTitle(0);
    if(!memcmp(suffix,g_menuPoolColumn1Suffix,4))return menu->GetColumnTitle(1);
    for(BYTE row=0;row<12;++row){
        if(!strcmp(suffix,((const char*)g_menuItemKeys)+row*64+4))return menu->GetItem(0,row);
        if(!strcmp(suffix,((const char*)g_menuItemKeys)+row*64+36))return menu->GetItem(1,row);
    }
    return 0;
}

#include "game/keystuff.h"
#include "main.h"
#include "BitStream.h"
#include "RakClientInterface.h"
typedef char MenuRpcOptions[(HIGH_PRIORITY==1 && RELIABLE==8)?1:-1];
// samp.dll RVA: 0x001179c4
int g_menuSelectedRpc=132;
// samp.dll RVA: 0x001179c0
int g_menuExitedRpc=140;
// samp.dll RVA: 0x00007ef0
void CMenuPool::Process(){
    if(currentMenu==128)return;
    BYTE *keys=(BYTE*)KeyStuff::GetInternalKeys();
    RakClientInterface *client=*(RakClientInterface**)((BYTE*)g_pNetGame+0x2c);
    if(*(WORD*)(keys+0x20) && !*(WORD*)(keys+0x50)){
        BYTE row=menus[currentMenu]->GetActiveRow();if(row==0xff)return;
        unknown401=1;RakNet::BitStream stream;stream.WriteBits(&row,8,true);
        client->RPC(&g_menuSelectedRpc,&stream,HIGH_PRIORITY,RELIABLE,0,false);
    } else if(*(WORD*)(keys+0x1e) && !*(WORD*)(keys+0x4e)){
        unknown401=1;client->RPC(&g_menuExitedRpc,(RakNet::BitStream*)0,HIGH_PRIORITY,RELIABLE,0,false);
    } else if(unknown401){Hide(currentMenu);unknown401=0;}
}
