#ifndef MENUPOOL_H
#define MENUPOOL_H

#include "game/menu.h"

#include <static_assert.h>

struct RPCParameters;
#pragma pack(push, 1)
// Native allocation is0x402 bytes; current-menu sentinel is128.
class CMenuPool
{
    // 0x0001c870
    friend void ScrInitMenu(RPCParameters* parameters);
    CMenu* menus[128];
    DWORD active[128];
    // pendingClose is intentionally untouched by native construction.
    BYTE currentMenu, pendingClose;

public:
    // Native includes ID128, reading a DWORD at400 across the allocation edge.
    // R5 address to verify
    DWORD GetActiveState(BYTE id);
    // 0x00008200
    void Process();
    // 0x00008060
    char* GetTextPointer(const char* suffix);
    // 0x00007e60
    CMenuPool();
    // 0x000081c0
    ~CMenuPool();
    // 0x00007eb0
    CMenu* Create(BYTE id, const char* title, float x, float y, BYTE columns, float width1, float width2, const void* interaction);
    // 0x00007f70
    BOOL Delete(BYTE id);
    // 0x00007fc0
    void Show(BYTE id);
    // 0x00008020
    void Hide(BYTE id);
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(CMenuPool) == 0x402);
// R5 address to verify
extern int g_menuSelectedRpc;
// R5 address to verify
extern int g_menuExitedRpc;

#endif // MENUPOOL_H
