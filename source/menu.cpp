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

#include "game/scripting.h"
// samp.dll RVA: 0x0013c0c8
extern const char g_menuItemKeys[24][32]={"SAMP000","SAMP100","SAMP001","SAMP101","SAMP002","SAMP102","SAMP003","SAMP103","SAMP004","SAMP104","SAMP005","SAMP105","SAMP006","SAMP106","SAMP007","SAMP107","SAMP008","SAMP108","SAMP009","SAMP109","SAMP010","SAMP110","SAMP011","SAMP111"};
// samp.dll RVA: 0x0011ecf4
extern const char g_menuDummy[6]="DUMMY";
// samp.dll RVA: 0x0011ecfc
const char g_menuHeaderKey1[]="SAMPRW2";
// samp.dll RVA: 0x0011ed04
const char g_menuHeaderKey0[]="SAMPRW1";
// samp.dll RVA: 0x0011ed0c
const char g_menuTitleKey[]="SAMPHED";
// samp.dll RVA: 0x0011ec7c
const Scripting::OpcodeInfo g_menuHideCommand={0x8da,"i"};
// samp.dll RVA: 0x0011ec90
const Scripting::OpcodeInfo g_menuCreateCommand={0x8d4,"sfffiiiiv"};
// samp.dll RVA: 0x0011eca4
const Scripting::OpcodeInfo g_menuItemsCommand={0x8db,"iisssssssssssss"};
// samp.dll RVA: 0x0011ecb8
const Scripting::OpcodeInfo g_menuWidthCommand={0x9db,"iii"};
// samp.dll RVA: 0x0011eccc
const Scripting::OpcodeInfo g_menuRowCommand={0x8d9,"iii"};
// samp.dll RVA: 0x0011ece0
const Scripting::OpcodeInfo g_menuSelectionCommand={0x8d7,"iv"};
// samp.dll RVA: 0x000a7260
void CMenu::Hide(){Scripting::ProcessCommand(&g_menuHideCommand,*(DWORD*)(nativeState+0x3b4));}
// samp.dll RVA: 0x000a72d0
const char *CMenu::MS(BYTE row,BYTE column){int index=column+row*2;return nativeState[0x21+index*32]?((const char*)g_menuItemKeys)+index*32:g_menuDummy;}
// samp.dll RVA: 0x000a7300
BYTE CMenu::GetActiveRow(){if(!*(DWORD*)(nativeState+0x372))return 0xff;DWORD row;Scripting::ProcessCommand(&g_menuSelectionCommand,*(DWORD*)(nativeState+0x3b4),&row);return (BYTE)row;}
// samp.dll RVA: 0x000a7330
void CMenu::Show(){
    BYTE *state=nativeState;const char *title=state[1]?g_menuTitleKey:g_menuDummy;
    int columns=state[0x371];double width;float *widths=(float*)(state+0x369);
    __asm {mov eax,widths}
    __asm {fld dword ptr [eax+4]}
    __asm {fadd dword ptr [eax]}
    __asm {fidiv columns}
    __asm {fstp qword ptr width}
    Scripting::ProcessCommand(&g_menuCreateCommand,title,(double)*(float*)(state+0x361),(double)*(float*)(state+0x365),width,columns,*(DWORD*)(state+0x372),1,1,(DWORD*)(state+0x3b4));
    for(int column=0;column<2;++column){
        if(column && state[0x371]!=2)break;
        const char *heading=state[0x321+column*32]?(column?g_menuHeaderKey1:g_menuHeaderKey0):g_menuDummy;
        const char *items[12];for(int row=11;row>=0;--row)items[row]=MS((BYTE)row,(BYTE)column);
        Scripting::ProcessCommand(&g_menuItemsCommand,*(DWORD*)(state+0x3b4),column,heading,items[0],items[1],items[2],items[3],items[4],items[5],items[6],items[7],items[8],items[9],items[10],items[11]);
        // VC7 uses its native _ftol2 conversion here, matching the original.
        Scripting::ProcessCommand(&g_menuWidthCommand,*(DWORD*)(state+0x3b4),column,(int)*(float*)(state+0x369));
    }
    for(int row=0;row<12;++row)Scripting::ProcessCommand(&g_menuRowCommand,*(DWORD*)(state+0x3b4),row,*(DWORD*)(state+0x376+row*4));
}
