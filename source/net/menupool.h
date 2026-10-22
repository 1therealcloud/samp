#ifndef SAMP_MENUPOOL_H
#define SAMP_MENUPOOL_H
#include "menu.h"
#pragma pack(push,1)
// Observed storage through401; no broader CNetGame/pool ownership claim.
class CMenuPool {
    CMenu *menus[128];
    DWORD active[128];
    BYTE currentMenu,unknown401;
public:
    // samp.dll RVA: 0x00007d50
    char *GetTextPointer(const char *suffix);
    // samp.dll RVA: 0x00007b40
    CMenuPool();
    // samp.dll RVA: 0x00007eb0
    ~CMenuPool();
    // samp.dll RVA: 0x00007b90
    CMenu *Create(BYTE id,const char *title,float x,float y,BYTE columns,float width1,float width2,const void *interaction);
    // samp.dll RVA: 0x00007c60
    BOOL Delete(BYTE id);
    // samp.dll RVA: 0x00007cb0
    void Show(BYTE id);
    // samp.dll RVA: 0x00007d10
    void Hide(BYTE id);
};
#pragma pack(pop)
#endif
