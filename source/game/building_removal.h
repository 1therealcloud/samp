#ifndef SAMP_GAME_BUILDING_REMOVAL_H
#define SAMP_GAME_BUILDING_REMOVAL_H
#include <windows.h>

#include <static_assert.h>

struct BuildingRemovalRecord
{
    int model;
    float x, y, z, radius;
};
STATIC_ASSERT(sizeof(BuildingRemovalRecord) == 20);
// 0x0009cff0
void GameHideObjectEntity(void* entity);
// 0x0009d020
void GameHideBuildingEntity(void* entity);
// 0x0009d0c0
void GameRemoveLodBuildings(float x, float y, float z, float radius);
// 0x0009d180
void GameRemoveBuildingsInRange(int model, float x, float y, float z, float radius);
// Publishes an unbounded record after applying removal to the current world.
// 0x0009d3d0
void GameRemoveBuilding(int model, float x, float y, float z, float radius);
// 0x000ec310
extern const float g_buildingRemovalDrop;
// 0x000ec314
extern const float g_buildingLodCoordinateScale;
// Physical extent up to the native count at RVA 0x0018dc18 is 1000 records.
// Native publication/search does not enforce a 1000-record bound. Adjacent
// out-of-range accesses are not proven layout-equivalent in this recovery.
// 0x0014af68
extern BuildingRemovalRecord g_buildingRemovalRecords[1000];
// 0x0014fd88
extern int g_buildingRemovalCount;
// 0x000b4970
float __stdcall GameDistanceBetweenPoints(float x, float y, float z, float otherX, float otherY, float otherZ);
// 0x0009d050
BOOL GameShouldSuppressMarker(int model, float x, float y, float z);
#endif

// Fixed native pool spans, without metadata/null/occupied-slot guards.
// R5 address to verify
void GameHideAllWorldEntities();
