#include "menu.h"
#include <string.h>
// samp.dll RVA: 0x000a7160
CMenu::CMenu(const char *title,float x,float y,BYTE columns,float width1,float width2,const void *interaction) {
    BYTE *state=(BYTE*)this;
    for(int row=0;row<12;++row){state[0x21+row*64]=0;state[0x41+row*64]=0;}
    state[1]=0;state[0x321]=0;state[0x341]=0;
    char *destination=(char*)(state+1);do{*destination++=*title;}while(*title++);
    memcpy(state+0x361,&x,4);memcpy(state+0x365,&y,4);
    memcpy(state+0x369,&width1,4);memcpy(state+0x36d,&width2,4);
    state[0x371]=columns==2?2:1;
    for(int i=0;i<16;++i)*(DWORD*)(state+0x372+i*4)=*((const DWORD*)interaction+i);
    *(DWORD*)(state+0x3b4)=0;
}
// samp.dll RVA: 0x000a7200
void CMenu::AddItem(BYTE column,BYTE row,const char *text){char *destination=(char*)this+0x21+(column+row*2)*32;do{*destination++=*text;}while(*text++);}
// samp.dll RVA: 0x000a7230
void CMenu::SetColumnTitle(BYTE column,const char *text){char *destination=(char*)this+0x321+column*32;do{*destination++=*text;}while(*text++);}
// samp.dll RVA: 0x000a7280
char *CMenu::GetItem(BYTE column,BYTE row){return (char*)this+0x21+(column+row*2)*32;}
// samp.dll RVA: 0x000a72a0
char *CMenu::GetTitle(){return (char*)this+1;}
// samp.dll RVA: 0x000a72b0
char *CMenu::GetColumnTitle(BYTE column){return (char*)this+0x321+column*32;}
