#ifndef SAMP_MENUPOOL_H
#define SAMP_MENUPOOL_H
#include "menu.h"
struct RPCParameters;
#pragma pack(push, 1)
// Native allocation is0x402 bytes; current-menu sentinel is128.
class CMenuPool
{
    // samp.dll RVA: 0x0001c390
    friend void ScrInitMenu(RPCParameters* parameters);
    CMenu* menus[128];
    DWORD active[128];
    // pendingClose is intentionally untouched by native construction.
    BYTE currentMenu, pendingClose;

public:
    // Native includes ID128, reading a DWORD at400 across the allocation edge.
    // samp.dll RVA: 0x000177b0
    DWORD GetActiveState(BYTE id);
    // samp.dll RVA: 0x00007ef0
    void Process();
    // samp.dll RVA: 0x00007d50
    char* GetTextPointer(const char* suffix);
    // samp.dll RVA: 0x00007b40
    CMenuPool();
    // samp.dll RVA: 0x00007eb0
    ~CMenuPool();
    // samp.dll RVA: 0x00007b90
    CMenu* Create(BYTE id, const char* title, float x, float y, BYTE columns, float width1, float width2, const void* interaction);
    // samp.dll RVA: 0x00007c60
    BOOL Delete(BYTE id);
    // samp.dll RVA: 0x00007cb0
    void Show(BYTE id);
    // samp.dll RVA: 0x00007d10
    void Hide(BYTE id);
};
#pragma pack(pop)
typedef char NativeMenuPoolExtent[(sizeof(CMenuPool) == 0x402) ? 1 : -1];
// samp.dll RVA: 0x001179c4
extern int g_menuSelectedRpc;
// samp.dll RVA: 0x001179c0
extern int g_menuExitedRpc;
#endif
