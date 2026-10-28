#ifndef SAMP_GAME_BUILDING_REMOVAL_H
#define SAMP_GAME_BUILDING_REMOVAL_H
#include <windows.h>
struct BuildingRemovalRecord
{
    int model;
    float x, y, z, radius;
};
typedef char BuildingRemovalRecordExtent[(sizeof(BuildingRemovalRecord) == 20) ? 1 : -1];
// 0x0009ca80
void GameHideObjectEntity(void* entity);
// 0x0009cab0
void GameHideBuildingEntity(void* entity);
// 0x0009cb50
void GameRemoveLodBuildings(float x, float y, float z, float radius);
// 0x0009cc10
void GameRemoveBuildingsInRange(int model, float x, float y, float z, float radius);
// Publishes an unbounded record after applying removal to the current world.
// 0x0009d210
void GameRemoveBuilding(int model, float x, float y, float z, float radius);
// 0x0011e338
extern const float g_buildingRemovalDrop;
// 0x0011e33c
extern const float g_buildingLodCoordinateScale;
// Physical extent up to the native count at RVA 0x0018dc18 is 1000 records.
// Native publication/search does not enforce a 1000-record bound. Adjacent
// out-of-range accesses are not proven layout-equivalent in this recovery.
// 0x00188df8
extern BuildingRemovalRecord g_buildingRemovalRecords[1000];
// 0x0018dc18
extern int g_buildingRemovalCount;
// 0x000b4570
float __stdcall GameDistanceBetweenPoints(float x, float y, float z, float otherX, float otherY, float otherZ);
// 0x0009cae0
BOOL GameShouldSuppressMarker(int model, float x, float y, float z);
#endif

// Fixed native pool spans, without metadata/null/occupied-slot guards.
// 0x0009cdd0
void GameHideAllWorldEntities();
