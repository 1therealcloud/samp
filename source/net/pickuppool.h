#ifndef SAMP_PICKUPPOOL_H
#define SAMP_PICKUPPOOL_H
#include <windows.h>
#pragma pack(push,1)
struct Pickup {int model,type;float x,y,z;};
struct WeaponPickupState {BYTE enabled;WORD owner;};
class CPickupPool {
    int count;
    DWORD handles[4096];
    int indices[4096];
    DWORD cooldown[4096];
    WeaponPickupState weapons[4096];
    Pickup pickups[4096];
public:
    // samp.dll RVA: 0x00013080
    void CreateWeapon(int model,float x,float y,float z,int ammo,WORD owner);
    // samp.dll RVA: 0x00013340
    void SendNotification(int index);
    // samp.dll RVA: 0x00013420
    void Process();
    // samp.dll RVA: 0x00008190
    CPickupPool();
    // samp.dll RVA: 0x00013310
    ~CPickupPool();
    // samp.dll RVA: 0x00013170
    void Create(const Pickup *pickup,int id);
    // samp.dll RVA: 0x00013220
    void Delete(int id);
    // samp.dll RVA: 0x00013280
    void DeleteWeapon(WORD owner);
    // samp.dll RVA: 0x000132e0
    int GetIndex(int index);
};
#pragma pack(pop)
typedef char NativePickupExtent[(sizeof(Pickup)==20)?1:-1];
typedef char NativeWeaponPickupExtent[(sizeof(WeaponPickupState)==3)?1:-1];
typedef char NativePickupPoolExtent[(sizeof(CPickupPool)==0x23004)?1:-1];
// samp.dll RVA: 0x0011816c
extern int g_pickupCollectedRpc;
// samp.dll RVA: 0x00118170
extern int g_weaponPickupCollectedRpc;
#endif
