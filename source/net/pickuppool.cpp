#include <static_assert.h>
#include "net/pickuppool.h"
#include "BitStream.h"
#include "RakClientInterface.h"
#include "game/game.h"
#include "game/scripting.h"
#include "main.h"
#include "net/netgame.h"
#include <stddef.h>
#include <string.h>
// 0x00118158
const Scripting::OpcodeInfo g_pickupDestroyCommand = {0x215, "i"};
// 0x00008190
CPickupPool::CPickupPool()
{
    STATIC_ASSERT(offsetof(CPickupPool, count) == 0);
    STATIC_ASSERT(offsetof(CPickupPool, handles) == 4);
    STATIC_ASSERT(offsetof(CPickupPool, indices) == 0x4004);
    STATIC_ASSERT(offsetof(CPickupPool, cooldown) == 0x8004);
    STATIC_ASSERT(offsetof(CPickupPool, weapons) == 0xc004);
    STATIC_ASSERT(offsetof(CPickupPool, pickups) == 0xf004);
    // Native deliberately leaves the weapon-owner records untouched.
    memset(pickups, 0, sizeof(pickups));
    count = 0;
    for (int i = 0; i < Capacity; ++i)
    {
        handles[i] = 0;
        cooldown[i] = 0;
        indices[i] = -1;
    }
}
// 0x00013170
void CPickupPool::Create(const Pickup* pickup, int id)
{
    if (count >= Capacity || id < 0 || id >= Capacity)
        return;
    if (handles[id])
        Scripting::ProcessCommand(&g_pickupDestroyCommand, handles[id]);
    memcpy(&pickups[id], pickup, sizeof(Pickup));
    weapons[id].enabled = 0;
    DWORD index;
    DWORD handle = g_pGame->CreatePickup(pickup->model, pickup->type, pickup->x, pickup->y, pickup->z, &index);
    handles[id] = handle;
    indices[id] = (int)index;
    ++count;
}
// 0x00013220
void CPickupPool::Delete(int id)
{
    if (count <= 0 || id < 0 || id >= Capacity || !handles[id])
        return;
    Scripting::ProcessCommand(&g_pickupDestroyCommand, handles[id]);
    handles[id] = 0;
    cooldown[id] = 0;
    indices[id] = -1;
    --count;
}
// 0x00013280
void CPickupPool::DeleteWeapon(WORD owner)
{
    for (int i = 0; i < Capacity; ++i)
    {
        if (handles[i] && weapons[i].enabled && weapons[i].owner == owner)
        {
            Scripting::ProcessCommand(&g_pickupDestroyCommand, handles[i]);
            handles[i] = 0;
            cooldown[i] = 0;
            --count;
        }
    }
}
// 0x000132e0
int CPickupPool::GetIndex(int index)
{
    for (int i = 0; i != Capacity; ++i)
        if (indices[i] == index)
            return i;
    return -1;
}
// 0x00013310
CPickupPool::~CPickupPool()
{
    for (int i = 0; i < Capacity; ++i)
    {
        if (handles[i])
            Scripting::ProcessCommand(&g_pickupDestroyCommand, handles[i]);
    }
}

STATIC_ASSERT(HIGH_PRIORITY == 1 && RELIABLE_ORDERED == 9 && RELIABLE_SEQUENCED == 10);
// 0x0011816c
int g_pickupCollectedRpc = 131;
// 0x00118170
int g_weaponPickupCollectedRpc = 97;
// 0x00118144
const Scripting::OpcodeInfo g_pickupCollectedCommand = {0x214, "i"};
// 0x00013340
void CPickupPool::SendNotification(int index)
{
    int id = GetIndex(index);
    if (id == -1 || !handles[id] || cooldown[id] || weapons[id].enabled)
        return;
    RakNet::BitStream stream;
    stream.Write(id);
    ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_pickupCollectedRpc, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
    cooldown[id] = 15;
}
// 0x00013420
void CPickupPool::Process()
{
    for (int i = 0; i < Capacity; ++i)
    {
        if (!handles[i])
            continue;
        if (!weapons[i].enabled && pickups[i].type != 14)
        {
            if (cooldown[i] > 0)
                --cooldown[i];
            continue;
        }
        if (!Scripting::ProcessCommand(&g_pickupCollectedCommand, handles[i]))
            continue;
        RakNet::BitStream stream;
        if (weapons[i].enabled)
        {
            stream.Write(weapons[i].owner);
            ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_weaponPickupCollectedRpc, &stream, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false);
        }
        else
        {
            stream.Write(i);
            ((CNetGame*)g_pNetGame)->rakClient->RPC(&g_pickupCollectedRpc, &stream, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false);
        }
    }
}

// 0x00013080
void CPickupPool::CreateWeapon(int model, float x, float y, float z, int ammo, WORD owner)
{
    if (count >= Capacity)
        return;
    int id = 0;
    while (id < Capacity && handles[id])
        ++id;
    if (id == Capacity)
        return;
    pickups[id].model = model;
    pickups[id].type = 4;
    pickups[id].x = x;
    pickups[id].y = y;
    pickups[id].z = z;
    handles[id] = g_pGame->CreateWeaponPickup(model, ammo, x, y, z);
    weapons[id].enabled = 1;
    weapons[id].owner = owner;
    cooldown[id] = 0;
    indices[id] = -1;
    ++count;
}
