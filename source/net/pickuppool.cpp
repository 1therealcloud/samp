#include "net/pickuppool.h"
#include "game/game.h"
#include "game/scripting.h"
#include <stddef.h>
#include <string.h>
// samp.dll RVA: 0x00118158
const Scripting::OpcodeInfo g_pickupDestroyCommand = {0x215, "i"};
// samp.dll RVA: 0x00008190
CPickupPool::CPickupPool()
{
    typedef char CountOffset[(offsetof(CPickupPool, count) == 0) ? 1 : -1];
    typedef char HandlesOffset[(offsetof(CPickupPool, handles) == 4) ? 1 : -1];
    typedef char IndicesOffset[(offsetof(CPickupPool, indices) == 0x4004) ? 1 : -1];
    typedef char CooldownOffset[(offsetof(CPickupPool, cooldown) == 0x8004) ? 1 : -1];
    typedef char WeaponsOffset[(offsetof(CPickupPool, weapons) == 0xc004) ? 1 : -1];
    typedef char PickupsOffset[(offsetof(CPickupPool, pickups) == 0xf004) ? 1 : -1];
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
// samp.dll RVA: 0x00013170
void CPickupPool::Create(const Pickup* pickup, int id)
{
    if (count >= Capacity || id < 0 || id >= Capacity)
        return;
    if (handles[id])
        Scripting::ProcessCommand(&g_pickupDestroyCommand, handles[id]);
    for (int i = 0; i < 5; ++i)
        ((DWORD*)&pickups[id])[i] = ((const DWORD*)pickup)[i];
    weapons[id].enabled = 0;
    DWORD index;
    DWORD handle = g_pGame->CreatePickup(pickup->model, pickup->type, pickup->x, pickup->y, pickup->z, &index);
    handles[id] = handle;
    indices[id] = (int)index;
    ++count;
}
// samp.dll RVA: 0x00013220
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
// samp.dll RVA: 0x00013280
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
// samp.dll RVA: 0x000132e0
int CPickupPool::GetIndex(int index)
{
    for (int i = 0; i < Capacity; ++i)
        if (indices[i] == index)
            return i;
    return -1;
}
// samp.dll RVA: 0x00013310
CPickupPool::~CPickupPool()
{
    for (int i = 0; i < Capacity; ++i)
    {
        if (handles[i])
            Scripting::ProcessCommand(&g_pickupDestroyCommand, handles[i]);
    }
}

#include "BitStream.h"
#include "RakClientInterface.h"
#include "main.h"
typedef char PickupRpcOptions[(HIGH_PRIORITY == 1 && RELIABLE_ORDERED == 9 && RELIABLE_SEQUENCED == 10) ? 1 : -1];
// samp.dll RVA: 0x0011816c
int g_pickupCollectedRpc = 131;
// samp.dll RVA: 0x00118170
int g_weaponPickupCollectedRpc = 97;
// samp.dll RVA: 0x00118144
const Scripting::OpcodeInfo g_pickupCollectedCommand = {0x214, "i"};
// samp.dll RVA: 0x00013340
void CPickupPool::SendNotification(int index)
{
    int id = GetIndex(index);
    if (id == -1 || !handles[id] || cooldown[id] || weapons[id].enabled)
        return;
    RakNet::BitStream stream;
    stream.WriteBits((const BYTE*)&id, 32, true);
    (*(RakClientInterface**)((BYTE*)g_pNetGame + 0x2c))->RPC(&g_pickupCollectedRpc, &stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
    cooldown[id] = 15;
}
// samp.dll RVA: 0x00013420
void CPickupPool::Process()
{
    for (int i = 0; i < Capacity; ++i)
    {
        if (!handles[i])
            continue;
        if (!weapons[i].enabled && pickups[i].type != 14)
        {
            if (cooldown[i])
                --cooldown[i];
            continue;
        }
        if (!Scripting::ProcessCommand(&g_pickupCollectedCommand, handles[i]))
            continue;
        RakNet::BitStream stream;
        if (weapons[i].enabled)
        {
            WORD owner = weapons[i].owner;
            stream.WriteBits((const BYTE*)&owner, 16, true);
            (*(RakClientInterface**)((BYTE*)g_pNetGame + 0x2c))->RPC(&g_weaponPickupCollectedRpc, &stream, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false);
        }
        else
        {
            stream.WriteBits((const BYTE*)&i, 32, true);
            (*(RakClientInterface**)((BYTE*)g_pNetGame + 0x2c))->RPC(&g_pickupCollectedRpc, &stream, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, false);
        }
    }
}

// samp.dll RVA: 0x00013080
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
    memcpy(&pickups[id].x, &x, 4);
    memcpy(&pickups[id].y, &y, 4);
    float* storedZ = &pickups[id].z;
    // clang-format off
    __asm {fld z}
    __asm {mov eax,storedZ}
    __asm {fstp dword ptr [eax]}
    // clang-format on
    handles[id] = g_pGame->CreateWeaponPickup(model, ammo, x, y, z);
    weapons[id].enabled = 1;
    weapons[id].owner = owner;
    cooldown[id] = 0;
    indices[id] = -1;
    ++count;
}
