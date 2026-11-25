#ifndef PICKUPPOOL_H
#define PICKUPPOOL_H

#include <windows.h>

#pragma pack(push, 1)
struct Pickup
{
    int model, type;
    float x, y, z;
};
struct WeaponPickupState
{
    BYTE enabled;
    WORD owner;
};
class CPickupPool
{
    enum
    {
        Capacity = 4096
    };
    int count;
    DWORD handles[Capacity];
    int indices[Capacity];
    DWORD cooldown[Capacity];
    WeaponPickupState weapons[Capacity];
    Pickup pickups[Capacity];

public:

    void CreateWeapon(int model, float x, float y, float z, int ammo, WORD owner);

    void SendNotification(int index);

    void Process();

    CPickupPool();

    ~CPickupPool();

    void Create(const Pickup* pickup, int id);

    void Delete(int id);

    void DeleteWeapon(WORD owner);

    int GetIndex(int index);
};
#pragma pack(pop)
static_assert(sizeof(Pickup) == 20);
static_assert(sizeof(WeaponPickupState) == 3);
static_assert(sizeof(CPickupPool) == 0x23004);

extern int g_pickupCollectedRpc;

extern int g_weaponPickupCollectedRpc;

#endif // PICKUPPOOL_H
