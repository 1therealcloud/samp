#include <static_assert.h>
#include "net/menupool.h"
#include "BitStream.h"
#include "RakClientInterface.h"
#include "game/keystuff.h"
#include "main.h"
#include "net/netgame.h"
#include <string.h>
// 0x000177b0
DWORD CMenuPool::GetActiveState(BYTE id)
{
    if (id > 128)
        return 0;
    return *(DWORD*)((BYTE*)this + 0x200 + id * 4);
}
// 0x00007b40
CMenuPool::CMenuPool()
{
    for (BYTE i = 0; i < 128; ++i)
    {
        active[i] = 0;
        menus[i] = 0;
    }
    currentMenu = 128;
}
// 0x00007b90
CMenu* CMenuPool::Create(BYTE id, const char* title, float x, float y, BYTE columns, float width1, float width2, const void* interaction)
{
    if (menus[id])
    {
        delete menus[id];
        menus[id] = 0;
    }
    active[id] = 0;
    CMenu* menu = new CMenu(title, x, y, columns, width1, width2, interaction);
    if (menu)
    {
        active[id] = 1;
        menus[id] = menu;
        return menu;
    }
    return 0;
}
// 0x00007c60
BOOL CMenuPool::Delete(BYTE id)
{
    if (!active[id] || !menus[id])
        return FALSE;
    active[id] = 0;
    delete menus[id];
    menus[id] = 0;
    return TRUE;
}
// 0x00007cb0
void CMenuPool::Show(BYTE id)
{
    if (id >= 128 || !active[id] || !menus[id])
        return;
    if (currentMenu != 128)
        menus[currentMenu]->Hide();
    menus[id]->Show();
    currentMenu = id;
    pendingClose = 0;
}
// 0x00007d10
void CMenuPool::Hide(BYTE id)
{
    if (id >= 128 || currentMenu == 128 || !active[id] || !menus[id])
        return;
    menus[id]->Hide();
    currentMenu = 128;
}
// 0x00007eb0
CMenuPool::~CMenuPool()
{
    if (currentMenu != 128)
        menus[currentMenu]->Hide();
    for (BYTE i = 0; i < 128; ++i)
        if (menus[i])
        {
            delete menus[i];
            menus[i] = 0;
        }
}

// 0x00007d50
char* CMenuPool::GetTextPointer(const char* suffix)
{
    if (currentMenu == 128)
        return 0;
    CMenu* menu = menus[currentMenu];
    if (!menu)
        return 0;
    // Native string RVAs: HED 0x001179bc, RW1 0x001179b8, RW2 0x001179b4.
    if (!strcmp(suffix, "HED"))
        return menu->GetTitle();
    if (!strcmp(suffix, "RW1"))
        return menu->GetColumnTitle(0);
    if (!strcmp(suffix, "RW2"))
        return menu->GetColumnTitle(1);
    for (BYTE row = 0; row < 12; ++row)
    {
        if (!strcmp(suffix, ((const char*)g_menuItemKeys) + row * 64 + 4))
            return menu->GetItem(0, row);
        if (!strcmp(suffix, ((const char*)g_menuItemKeys) + row * 64 + 36))
            return menu->GetItem(1, row);
    }
    return 0;
}

STATIC_ASSERT(HIGH_PRIORITY == 1 && RELIABLE == 8);
// 0x001179c4
int g_menuSelectedRpc = 132;
// 0x001179c0
int g_menuExitedRpc = 140;
// 0x00007ef0
void CMenuPool::Process()
{
    if (currentMenu == 128)
        return;
    KeyPad* keys = KeyStuff::GetInternalKeys();
    RakClientInterface* client = ((CNetGame*)g_pNetGame)->rakClient;
    if (keys->keys1[16] && !keys->keys2[16])
    {
        BYTE row = menus[currentMenu]->GetActiveRow();
        if (row != 0xff)
        {
            pendingClose = 1;
            RakNet::BitStream stream;
            stream.Write(row);
            client->RPC(&g_menuSelectedRpc, &stream, HIGH_PRIORITY, RELIABLE, 0, false);
        }
    }
    else if (keys->keys1[15] && !keys->keys2[15])
    {
        pendingClose = 1;
        client->RPC(&g_menuExitedRpc, (RakNet::BitStream*)0, HIGH_PRIORITY, RELIABLE, 0, false);
    }
    else if (pendingClose)
    {
        Hide(currentMenu);
        pendingClose = 0;
    }
}
