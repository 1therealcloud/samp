#ifndef SAMP_GAME_BUILDING_REMOVAL_H
#define SAMP_GAME_BUILDING_REMOVAL_H
#include <windows.h>

struct BuildingRemovalRecord
{
    int model;
    float x, y, z, radius;
};
static_assert(sizeof(BuildingRemovalRecord) == 20);

void GameHideObjectEntity(void* entity);

void GameHideBuildingEntity(void* entity);

void GameRemoveLodBuildings(float x, float y, float z, float radius);

void GameRemoveBuildingsInRange(int model, float x, float y, float z, float radius);
// Publishes an unbounded record after applying removal to the current world.

void GameRemoveBuilding(int model, float x, float y, float z, float radius);

extern const float g_buildingRemovalDrop;

extern const float g_buildingLodCoordinateScale;
// Physical extent up to the native count at RVA 0x0018dc18 is 1000 records.
// Native publication/search does not enforce a 1000-record bound. Adjacent
// out-of-range accesses are not proven layout-equivalent in this recovery.

extern BuildingRemovalRecord g_buildingRemovalRecords[1000];

extern int g_buildingRemovalCount;

float __stdcall GameDistanceBetweenPoints(float x, float y, float z, float otherX, float otherY, float otherZ);

BOOL GameShouldSuppressMarker(int model, float x, float y, float z);
#endif

// Fixed native pool spans, without metadata/null/occupied-slot guards.

void GameHideAllWorldEntities();
