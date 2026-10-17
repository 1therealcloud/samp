#ifndef SAMP_GAME_BUILDING_REMOVAL_H
#define SAMP_GAME_BUILDING_REMOVAL_H
#include <windows.h>
struct BuildingRemovalRecord { int model;float x,y,z,radius; };
typedef char BuildingRemovalRecordExtent[(sizeof(BuildingRemovalRecord)==20)?1:-1];
// Physical extent up to the native count at RVA 0x0018dc18 is 1000 records.
// Native publication/search does not enforce a 1000-record bound. Adjacent
// out-of-range accesses are not proven layout-equivalent in this recovery.
// samp.dll RVA: 0x00188df8
extern BuildingRemovalRecord g_buildingRemovalRecords[1000];
// samp.dll RVA: 0x0018dc18
extern int g_buildingRemovalCount;
// samp.dll RVA: 0x000b4570
float __stdcall GameDistanceBetweenPoints(float x,float y,float z,float otherX,float otherY,float otherZ);
// samp.dll RVA: 0x0009cae0
BOOL GameShouldSuppressMarker(int model,float x,float y,float z);
#endif
