#ifndef SAMP_PICKUPPOOL_H
#define SAMP_PICKUPPOOL_H
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
    // 0x00013080
    void CreateWeapon(int model, float x, float y, float z, int ammo, WORD owner);
    // 0x00013340
    void SendNotification(int index);
    // 0x00013420
    void Process();
    // 0x00008190
    CPickupPool();
    // 0x00013310
    ~CPickupPool();
    // 0x00013170
    void Create(const Pickup* pickup, int id);
    // 0x00013220
    void Delete(int id);
    // 0x00013280
    void DeleteWeapon(WORD owner);
    // 0x000132e0
    int GetIndex(int index);
};
#pragma pack(pop)
typedef char NativePickupExtent[(sizeof(Pickup) == 20) ? 1 : -1];
typedef char NativeWeaponPickupExtent[(sizeof(WeaponPickupState) == 3) ? 1 : -1];
typedef char NativePickupPoolExtent[(sizeof(CPickupPool) == 0x23004) ? 1 : -1];
// 0x0011816c
extern int g_pickupCollectedRpc;
// 0x00118170
extern int g_weaponPickupCollectedRpc;
#endif
