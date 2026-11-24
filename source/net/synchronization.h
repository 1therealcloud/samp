// synchronization.h
#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H

#include "game/quaternion.h"

#include <cstddef>
#include <cstdint>

#pragma pack(push, 1)
struct LocalOnfootSyncData
{
    uint16_t leftRight, upDown, keys;
    CVector position;
    GameQuaternion rotation;
    uint8_t health, armour, weaponAim, specialAction;
    CVector speed, surfingOffset;
    uint16_t surfingVehicle;
    uint32_t animation;
};
static_assert(sizeof(LocalOnfootSyncData) == 68);

static_assert(offsetof(LocalOnfootSyncData, position) == 6);
static_assert(offsetof(LocalOnfootSyncData, rotation) == 18);
static_assert(offsetof(LocalOnfootSyncData, health) == 34);
static_assert(offsetof(LocalOnfootSyncData, speed) == 38);
static_assert(offsetof(LocalOnfootSyncData, surfingOffset) == 50);
static_assert(offsetof(LocalOnfootSyncData, animation) == 64);



struct LocalIncarSyncData
{
    uint16_t vehicle, leftRight, upDown, keys;
    GameQuaternion rotation;
    CVector position, speed;
    float health;
    uint8_t pedHealth, armour, weaponAim, siren, landingGear;
    uint16_t trailer;
    uint32_t modelData;
};
static_assert(sizeof(LocalIncarSyncData) == 63);

static_assert(offsetof(LocalIncarSyncData, rotation) == 8);
static_assert(offsetof(LocalIncarSyncData, position) == 24);
static_assert(offsetof(LocalIncarSyncData, speed) == 36);
static_assert(offsetof(LocalIncarSyncData, health) == 48);
static_assert(offsetof(LocalIncarSyncData, pedHealth) == 52);
static_assert(offsetof(LocalIncarSyncData, trailer) == 57);
static_assert(offsetof(LocalIncarSyncData, modelData) == 59);



struct LocalAimSyncData
{
    uint8_t cameraMode;
    uint8_t vectors[24];
    float aimZ;
    uint8_t zoomWeapon, aspect;
};
static_assert(sizeof(LocalAimSyncData) == 31);

static_assert(offsetof(LocalAimSyncData, aimZ) == 25);
static_assert(offsetof(LocalAimSyncData, zoomWeapon) == 29);



struct LocalTrailerSyncData
{
    uint16_t vehicle;
    CVector position;
    GameQuaternion rotation;
    CVector speed, turnSpeed;
};
static_assert(sizeof(LocalTrailerSyncData) == 54);

static_assert(offsetof(LocalTrailerSyncData, rotation) == 14);
static_assert(offsetof(LocalTrailerSyncData, speed) == 30);


struct LocalPassengerSyncData
{
    uint16_t vehicle;
    uint8_t seatFlags, weaponAim, health, armour;
    uint16_t leftRight, upDown, keys;
    CVector position;
};
static_assert(sizeof(LocalPassengerSyncData) == 24);

static_assert(offsetof(LocalPassengerSyncData, keys) == 10);
static_assert(offsetof(LocalPassengerSyncData, position) == 12);



struct UnoccupiedSyncData
{
    uint16_t vehicle;
    uint8_t seat;
    CVector right, up, position, speed, turnSpeed;
    float health;
};
static_assert(sizeof(UnoccupiedSyncData) == 67);

static_assert(offsetof(UnoccupiedSyncData, position) == 27);
static_assert(offsetof(UnoccupiedSyncData, health) == 63);



struct BulletSyncData
{
    uint8_t type;
    uint16_t targetId;
    CVector origin, target, offset;
    uint8_t weapon;
};
static_assert(sizeof(BulletSyncData) == 40);

static_assert(offsetof(BulletSyncData, targetId) == 1);
static_assert(offsetof(BulletSyncData, origin) == 3);
static_assert(offsetof(BulletSyncData, weapon) == 39);

#pragma pack(pop)

#endif // SYNCHRONIZATION_H
