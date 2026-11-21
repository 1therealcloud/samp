#ifndef PICKUPPOOL_H
#define PICKUPPOOL_H

#include <windows.h>

#include <static_assert.h>

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
    // 0x00013180
    void CreateWeapon(int model, float x, float y, float z, int ammo, WORD owner);
    // 0x00013440
    void SendNotification(int index);
    // 0x00013520
    void Process();
    // 0x000084a0
    CPickupPool();
    // 0x00013410
    ~CPickupPool();
    // 0x00013270
    void Create(const Pickup* pickup, int id);
    // 0x00013320
    void Delete(int id);
    // 0x00013380
    void DeleteWeapon(WORD owner);
    // 0x000133e0
    int GetIndex(int index);
};
#pragma pack(pop)
STATIC_ASSERT(sizeof(Pickup) == 20);
STATIC_ASSERT(sizeof(WeaponPickupState) == 3);
STATIC_ASSERT(sizeof(CPickupPool) == 0x23004);
// R5 address to verify
extern int g_pickupCollectedRpc;
// 0x000e60e4
extern int g_weaponPickupCollectedRpc;

#endif // PICKUPPOOL_H
