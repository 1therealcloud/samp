#ifndef SAMP_MENU_H
#define SAMP_MENU_H
#include <windows.h>
#pragma pack(push,1)
// Native allocation extent3B8; storage retains unknown fields and untouched bytes.
class CMenu {
    BYTE nativeState[0x3b8];
public:
    // samp.dll RVA: 0x000a7160
    CMenu(const char *title,float x,float y,BYTE columns,float width1,float width2,const void *interaction);
    // samp.dll RVA: 0x000a7200
    void AddItem(BYTE column,BYTE row,const char *text);
    // samp.dll RVA: 0x000a7230
    void SetColumnTitle(BYTE column,const char *text);
    // samp.dll RVA: 0x000a7280
    char * GetItem(BYTE column,BYTE row);
    // samp.dll RVA: 0x000a72a0
    char * GetTitle();
    // samp.dll RVA: 0x000a72b0
    char * GetColumnTitle(BYTE column);
};
#pragma pack(pop)
typedef char NativeMenuExtent[(sizeof(CMenu)==0x3b8)?1:-1];
#endif
