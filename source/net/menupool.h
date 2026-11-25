#ifndef MENUPOOL_H
#define MENUPOOL_H

#include "game/menu.h"

struct RPCParameters;
#pragma pack(push, 1)
// Native allocation is0x402 bytes; current-menu sentinel is128.
class CMenuPool
{

    friend void ScrInitMenu(RPCParameters* parameters);
    CMenu* menus[128];
    DWORD active[128];
    // pendingClose is intentionally untouched by native construction.
    BYTE currentMenu, pendingClose;

public:
    // native accepts id 128 and reads past the pool, we return 0 for it

    DWORD GetActiveState(BYTE id);

    void Process();

    char* GetTextPointer(const char* suffix);

    CMenuPool();

    ~CMenuPool();

    CMenu* Create(BYTE id, const char* title, float x, float y, BYTE columns, float width1, float width2, const void* interaction);

    BOOL Delete(BYTE id);

    void Show(BYTE id);

    void Hide(BYTE id);
};
#pragma pack(pop)
static_assert(sizeof(CMenuPool) == 0x402);

extern int g_menuSelectedRpc;

extern int g_menuExitedRpc;

#endif // MENUPOOL_H
